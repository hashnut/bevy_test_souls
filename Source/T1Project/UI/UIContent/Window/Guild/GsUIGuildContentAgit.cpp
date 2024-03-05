// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildContentAgit.h"
#include "Components/TextBlock.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "DataSchema/Guild/Agit/GsSchemaAgitConfig.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Net/GsNetSendServiceGuild.h"
#include "Guild/GsGuildHelper.h"
#include "Currency/GsCurrencyHelper.h"
#include "Message/MessageParam/GsGuildMessageParam.h"


void UGsUIGuildContentAgit::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_checkRent.SetIntervalSecond(2.f);

	_currencyBtnRental->OnClickCurrencyButton.BindUObject(this, &UGsUIGuildContentAgit::OnClickBtnRental);
	_currencyBtnEnter->OnClickCurrencyButton.BindUObject(this, &UGsUIGuildContentAgit::OnClickBtnEnter);
}

void UGsUIGuildContentAgit::NativeDestruct()
{
	_rentalEndDate = 0;
	_checkRemainSec = 0;

	Super::NativeDestruct();
}

void UGsUIGuildContentAgit::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (0 < _checkRemainSec)
	{
		_checkRemainSec -= InDeltaTime;
		if (0 >= _checkRemainSec)
		{
			InvalidateRemainTime(true);
		}
	}
}

void UGsUIGuildContentAgit::InvalidateAllInternal()
{
	FGsGuildManager* guildMgr = GGuild();
	if (nullptr == guildMgr)
	{
		return;
	}

	// 남은 대여시간
	FGsAgitDataController* agitData = guildMgr->GetAgitDataController();
	if (nullptr == agitData)
	{
		return;
	}

	const FGsSchemaAgitConfig* agitConfig = FGsGuildHelper::GetAgitConfig();
	if (nullptr == agitConfig)
	{
		return;
	}

	GuildMemberGradeType myGrade = guildMgr->GetMemeberGrade();

	if (agitData->IsRented())
	{
		_rentalEndDate = agitData->GetRentalEndDate();

		// TEXT: 대여 {0}일 연장
		FText textFormat;
		if (FText::FindText(TEXT("GuildText"), TEXT("Agit_RentalExtensionButton"), textFormat))
		{
			FText textExtension = FText::Format(textFormat, agitConfig->rentalPeriodD);
			_currencyBtnRental->SetButtonName(textExtension);
		}
		_currencyBtnRental->SetData(CurrencyType::GOLD, agitConfig->rentalExtensionGold);
	}
	else
	{
		_rentalEndDate = 0;

		// TEXT: 영지 {0}일 대여
		FText textFormat;
		if (FText::FindText(TEXT("GuildText"), TEXT("Agit_RentalButton"), textFormat))
		{
			FText textRent = FText::Format(textFormat, agitConfig->rentalPeriodD);
			_currencyBtnRental->SetButtonName(textRent);
		}
		_currencyBtnRental->SetData(CurrencyType::GOLD, agitConfig->rentalGold);
	}

	if (FGsGuildHelper::IsInAgit())
	{
		// TEXT: 위치 복귀
		FText textEnter;
		FText::FindText(TEXT("GuildText"), TEXT("Agit_ReturnButton"), textEnter);
		_currencyBtnEnter->SetButtonName(textEnter);

		if (const FGsSchemaItemCommon* returnItem = agitConfig->returnItem.GetRow())
		{
			_currencyBtnEnter->SetData(returnItem->id, 1);

			// 락 걸린 아이템 빼고 개수 계산
			Currency myAmount = FGsCurrencyHelper::GetItemCurrencyAmount(returnItem->id, true);
			_currencyBtnEnter->SetAmountTextColor(
				(0 < myAmount) ? EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK);
		}
	}
	else
	{
		// TEXT: 영지 입장
		FText textEnter;
		FText::FindText(TEXT("GuildText"), TEXT("Agit_EnterButton"), textEnter);
		_currencyBtnEnter->SetButtonName(textEnter);

		if (const FGsSchemaItemCommon* enterItem = agitConfig->enterItem.GetRow())
		{
			_currencyBtnEnter->SetData(enterItem->id, 1);

			// 락 걸린 아이템 빼고 개수 계산
			Currency myAmount = FGsCurrencyHelper::GetItemCurrencyAmount(enterItem->id, true);
			_currencyBtnEnter->SetAmountTextColor(
				(0 < myAmount) ? EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK);
		}
	}

	InvalidateRemainTime(false);
}

void UGsUIGuildContentAgit::Show(bool bIsMyGuild, bool IsStackRecoverFlag)
{
	if (false == bIsMyGuild)
	{
		return;
	}

	// 이 부분은 한 번만 찍으면 되서 InvalidateAllInteranl 쪽에 넣지 않음
	if (const FGsSchemaAgitConfig* agitConfig = FGsGuildHelper::GetAgitConfig())
	{
		FText textGrade;
		FGsGuildHelper::GetMemberGradeText(agitConfig->rentalRequireMemberGrade, textGrade);

		FText textLimit;
		// TEXT: 기사단 레벨 {0}이상 / {1} 이상만 대여 가능합니다
		FText textFormat;
		if (FText::FindText(TEXT("GuildText"), TEXT("Agit_Tab_DownDesc"), textFormat))
		{
			textLimit = FText::Format(textFormat, agitConfig->rentalRequireGuildLevel, textGrade);
		}

		_textBlockRentLimit->SetText(textLimit);
	}
	else
	{
		_textBlockRentLimit->SetText(FText::GetEmpty());
	}

	InvalidateAll();
}

void UGsUIGuildContentAgit::OnNotifyChangeMemberGrade(const FGsGuildMsgParamMemberGrade* InParam)
{
	// 내 등급 변경 정보일 경우, 확인 팝업을 모두 닫고, 강제 갱신
	if (InParam)
	{
		if (FGsGameDataManager* gameDataMgr = GGameData())
		{
			if (const FGsNetUserData* myData = gameDataMgr->GetUserData())
			{
				if (myData->mUserName == InParam->_userName)
				{
					// 확인 팝업 닫기
					GUI()->CloseByKeyName(TEXT("PopupYesNo"));

					// TEXT: 정보가 수정되어 페이지를 갱신합니다
					FText textMsg;
					FText::FindText(TEXT("GuildText"), TEXT("Alert_Fail_And_Refresh"), textMsg);
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

					InvalidateAll();
				}
			}
		}
	}
}

void UGsUIGuildContentAgit::OnChangeGuildLevel()
{
	// UI 변할게 없으므로 처리 안함
	//InvalidateAll();
}

void UGsUIGuildContentAgit::OnAgitStateChanged()
{
	// 열려있던 팝업이 있으면 닫고 메시지 출력
	if (UGsUIManager* uiMgr = GUI())
	{
		if (uiMgr->IsActiveWidget("PopupYesNo"))
		{
			uiMgr->CloseByKeyName(TEXT("PopupYesNo"));

			// TEXT: 정보가 수정되어 페이지를 갱신합니다
			FText textMsg;
			FText::FindText(TEXT("GuildText"), TEXT("Alert_Fail_And_Refresh"), textMsg);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}
	}

	InvalidateAll();
}

void UGsUIGuildContentAgit::UpdateRentCheckTime()
{
	_checkRent.UpdateTime();
}

void UGsUIGuildContentAgit::InvalidateRemainTime(bool bCheckZero)
{
	// 튜토리얼 때문에 남은시간 UI부분을 끌 수가 없음. 그냥 "-" 로 표시
	time_t rentRemainTime = _rentalEndDate - FGsTimeSyncUtil::GetServerNowSecond();
	if (0 >= rentRemainTime)
	{
		_checkRemainSec = 0;
		_textBlockRemainTime->SetText(FText::FromString(TEXT("-")));
		
		// 시간이 0이되면 화면 갱신
		if (bCheckZero)
		{
			InvalidateAll();
		}
		return;
	}

	FString strTime;
	FTimespan timespan = FTimespan::FromSeconds(rentRemainTime);
	FGsTimeStringHelper::GetTimeStringCeil(timespan, strTime, false);

	_textBlockRemainTime->SetText(FText::FromString(strTime));

	// 1초에 한번씩 갱신
	_checkRemainSec = 1.f;
}

void UGsUIGuildContentAgit::OnClickBtnRental()
{
	if (false == _checkRent.IsTimeOver())
	{
		return;
	}

	FGsGuildManager* guildMgr = GGuild();
	if (nullptr == guildMgr)
	{
		return;
	}

	FGsAgitDataController* agitData = guildMgr->GetAgitDataController();
	if (nullptr == agitData)
	{
		return;
	}

	const FGsSchemaAgitConfig* agitConfig = FGsGuildHelper::GetAgitConfig();
	if (nullptr == agitConfig)
	{
		return;
	}

	// 기사단 레벨 체크
	if (guildMgr->GetGuildLevel() < agitConfig->rentalRequireGuildLevel)
	{
		// TEXT: 기사단 레벨이 부족해 영지를 이용할 수 없습니다
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Agit_Rental_LevelShortage"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

		return;
	}

	// 내 등급 체크
	if (guildMgr->GetMemeberGrade() < agitConfig->rentalRequireMemberGrade)
	{
		// TEXT: 기사단 등급이 낮아 영지를 대여할 수 없습니다
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Agit_Rental_NoRight"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

		return;
	}

	if (agitData->IsRented())
	{
		// 연장 유효한 기간인지 체크
		if (false == CanExtensionDate(_rentalEndDate, agitConfig->rentalExtensionMinD))
		{
			// TEXT: 남은 대여 기간이 {0}일 미만일 때에만 연장할 수 있습니다
			FText textFormat;
			if (FText::FindText(TEXT("GuildText"), TEXT("Agit_Rental_MinCount"), textFormat))
			{
				FText textMsg = FText::Format(textFormat, agitConfig->rentalExtensionMinD);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}

			return;
		}

		// 재화체크
		if (false == FGsCurrencyHelper::CheckCurrency(CurrencyType::GOLD, agitConfig->rentalExtensionGold, false))
		{
			Currency myAmount = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GOLD);
			FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType::GOLD, agitConfig->rentalExtensionGold - myAmount);

			return;
		}

		// TEXT: 영지 대여 기간을 연장하시겠습니까?
		TWeakObjectPtr<UGsUIGuildContentAgit> weakThis(this);
		FText textPopup;
		FText::FindText(TEXT("GuildText"), TEXT("Agit_RentalExtension_PopUp_Desc"), textPopup);
		FGsUIHelper::PopupYesNo(textPopup, [weakThis](bool bIsOk)
			{
				if (bIsOk)
				{
					// 연장요청
					FGsNetSendServiceGuild::SendReqAgitRentalExtension();

					if (weakThis.IsValid())
					{
						weakThis->UpdateRentCheckTime();
					}
				}
			});
	}
	else
	{
		// 재화체크
		if (false == FGsCurrencyHelper::CheckCurrency(CurrencyType::GOLD, agitConfig->rentalGold, false))
		{
			Currency myAmount = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GOLD);
			FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType::GOLD, agitConfig->rentalGold - myAmount);
			
			return;
		}

		// TEXT: 영지를 대여하시겠습니까?
		TWeakObjectPtr<UGsUIGuildContentAgit> weakThis(this);
		FText textPopup = FText::FromString(TEXT("[Test] Rental?"));
		FText::FindText(TEXT("GuildText"), TEXT("Agit_Rental_PopUp_Desc"), textPopup);
		FGsUIHelper::PopupYesNo(textPopup, [weakThis](bool bIsOk)
			{
				if (bIsOk)
				{
					// 대여 요청
					FGsNetSendServiceGuild::SendReqAgitRental();

					if (weakThis.IsValid())
					{
						weakThis->UpdateRentCheckTime();
					}
				}
			});
	}
}

void UGsUIGuildContentAgit::OnClickBtnEnter()
{
	FGsGuildManager* guildMgr = GGuild();
	if (nullptr == guildMgr)
	{
		return;
	}

	FGsAgitDataController* agitData = guildMgr->GetAgitDataController();
	if (nullptr == agitData)
	{
		return;
	}

	// 대여 상태인지 확인
	if (false == agitData->IsRented())
	{
		// TEXT: 기사단 영지를 먼저 대여해주세요
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Agit_EnterButton_Empty"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

		return;
	}

	const FGsSchemaAgitConfig* agitConfig = FGsGuildHelper::GetAgitConfig();
	if (nullptr == agitConfig)
	{
		return;
	}

	if (FGsGuildHelper::IsInAgit())
	{
		if (const FGsSchemaItemCommon* item = agitConfig->returnItem.GetRow())
		{
			// 아이템 갯수 확인(1개 고정), 락걸린 아이템 제외
			Currency myAmount = FGsCurrencyHelper::GetItemCurrencyAmount(item->id, true);
			if (0 == myAmount)
			{
				FGsCurrencyHelper::OpenLackItemPopup(item->id, 1);
				return;
			}
		}

		FGsUIHelper::ShowBlockUI();

		// 아이템 사용하여 마지막 위치로 나가기
		FGsNetSendServiceGuild::SendReqAgitLeave(true);
	}
	else
	{
		if (const FGsSchemaItemCommon* item = agitConfig->enterItem.GetRow())
		{
			// 아이템 갯수 확인(1개 고정), 락 걸린 아이템 제외
			Currency myAmount = FGsCurrencyHelper::GetItemCurrencyAmount(item->id, true);
			if (0 == myAmount)
			{
				FGsCurrencyHelper::OpenLackItemPopup(item->id, 1);
				return;
			}
		}

		FGsUIHelper::ShowBlockUI();

		// 입장
		FGsNetSendServiceGuild::SendReqAgitEnter();
	}
}

bool UGsUIGuildContentAgit::CanExtensionDate(const time_t InRentalEndDate, int32 InRentalExtensionMinD)
{
	time_t rentRemainTime = InRentalEndDate - FGsTimeSyncUtil::GetServerNowSecond();
	if (0 < rentRemainTime)
	{
		FTimespan rentMinTime = FTimespan::FromDays(InRentalExtensionMinD);
		if (rentRemainTime < rentMinTime.GetTotalSeconds())
		{
			return true;
		}
	}

	return false;
}
