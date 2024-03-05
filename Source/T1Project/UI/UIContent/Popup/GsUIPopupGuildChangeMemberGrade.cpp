// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupGuildChangeMemberGrade.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Popup/Guild/GsUIGuildMemberGradeEntry.h"
#include "UI/UIContent/Popup/GsUIPopupYesNoTwoMessage.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Guild/GsGuildData.h"
#include "Guild/GsGuildHelper.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Net/GsNetSendServiceGuild.h"
#include "DataSchema/Guild/GsSchemaGuildLevelDetailData.h"


void UGsUIPopupGuildChangeMemberGrade::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupGuildChangeMemberGrade::OnClickCancel);
	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupGuildChangeMemberGrade::OnClickOk);

	_slotHelper.Initialize(_panel, true);
	_slotHelper.OnRefreshEntry.BindUObject(this, &UGsUIPopupGuildChangeMemberGrade::OnRefreshSlotEntry);

	_toggleGroup.AddToggleWidgetByParentPanel(_panel);

	_dataList.Empty();
	_dataList.Emplace(FGsGuildMemberGradeData(GuildMemberGradeType::CAPTAIN, 1, 1));
	_dataList.Emplace(FGsGuildMemberGradeData(GuildMemberGradeType::SUBCAPTAIN, 0, 0));
	_dataList.Emplace(FGsGuildMemberGradeData(GuildMemberGradeType::FIRSTKNIGHT, 0, 0));
	_dataList.Emplace(FGsGuildMemberGradeData(GuildMemberGradeType::SECONDKNIGHT, 0, 0));
	_dataList.Emplace(FGsGuildMemberGradeData(GuildMemberGradeType::KNIGHT, 0, 0));
}

void UGsUIPopupGuildChangeMemberGrade::NativeConstruct()
{
	Super::NativeConstruct();

	const FGsSchemaGuildLevelDetailData* levelData = FGsGuildHelper::GetGuildLevelData(GGuild()->GetGuildLevel());
	if (levelData)
	{
		_dataList[GetDataIndex(GuildMemberGradeType::SUBCAPTAIN)].SetCount(
			levelData->maxSubCaptain, GetMemberCountByGrade(GuildMemberGradeType::SUBCAPTAIN));

		_dataList[GetDataIndex(GuildMemberGradeType::FIRSTKNIGHT)].SetCount(
			levelData->maxFirstKnight, GetMemberCountByGrade(GuildMemberGradeType::FIRSTKNIGHT));

		_dataList[GetDataIndex(GuildMemberGradeType::SECONDKNIGHT)].SetCount(
			levelData->maxSecondKnight, GetMemberCountByGrade(GuildMemberGradeType::SECONDKNIGHT));

		// null일 수 없는 값이라 널체크 안함
		const FGsGuildDataMine* localData = GGuild()->GetMyGuildData();

		// 최대 일반 기사 수 = 전체 - 길드장
		int32 maxKnightCount = localData->_maxMemberNum - 1;

		_dataList[GetDataIndex(GuildMemberGradeType::KNIGHT)].SetCount(
			maxKnightCount, GetMemberCountByGrade(GuildMemberGradeType::KNIGHT));
	}

	_slotHelper.RefreshAll(static_cast<uint8>(GuildMemberGradeType::MAX));
}

void UGsUIPopupGuildChangeMemberGrade::NativeDestruct()
{
	_userDBId = 0;
	_prevGrade = GuildMemberGradeType::MAX;

	Super::NativeDestruct();
}

void UGsUIPopupGuildChangeMemberGrade::OnInputOk()
{
	FGsGuildManager* guildMgr = GGuild();

	if (false == FGsGuildHelper::CanManageMember(guildMgr->GetAuthority()))
	{
		// TEXT: 권한이 없습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_NoAuthority"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		return;
	}

	GuildMemberGradeType myGrade = guildMgr->GetMemeberGrade();
	FGsGuildMemberGradeData& data = _dataList[_toggleGroup.GetSelectedIndex()];

	// 자기보다 등급이 낮을때만 가능. 단장은 예외.
	if (GuildMemberGradeType::CAPTAIN != myGrade &&
		data._gradeType >= myGrade)
	{
		// TEXT: 나보다 등급이 낮은 유저만 임명할 수 있습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_CannotAppoint_Rank"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		return;
	}

	// 등급이 바뀌었을때만 보냄
	if (_prevGrade == data._gradeType)
	{
		// TEXT: 동일한 등급입니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_SameGrade"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		return;
	}

	// 해당 등급 인원이 남았을때만 가능. 단장 넘길 때는 예외.
	if (GuildMemberGradeType::CAPTAIN != data._gradeType &&
		data.IsMaxCount())
	{
		// TEXT: 해당 등급의 인원이 이미 최대입니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_CannotAppoint_Full"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		return;
	}

	// 단장을 넘기는 상황에는 확인팝업 띄움
	if (GuildMemberGradeType::CAPTAIN == myGrade &&
		GuildMemberGradeType::CAPTAIN == data._gradeType)
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupYesNoTwoMessage"));
		if (widget.IsValid())
		{
			if (UGsUIPopupYesNoTwoMessage* popup = Cast<UGsUIPopupYesNoTwoMessage>(widget.Get()))
			{
				// TEXT: 기사단의 단장을 변경하시겠습니까?
				FText textMsg1;
				FText::FindText(TEXT("GuildText"), TEXT("UI_Popup_ChangeGrade_Title"), textMsg1);

				// TEXT: {0}님이 신규기사단장으로, {1}님이 일반 기사로 등급이 변경됩니다.
				FText textFormat;
				FText textMsg2;
				if (FText::FindText(TEXT("GuildText"), TEXT("UI_Popup_ChangeGrade_Desc"), textFormat))
				{
					FString myName;
					if (const FGsNetUserData* myData = GGameData()->GetUserData())
					{
						myName = myData->mUserName;
					}

					textMsg2 = FText::Format(textFormat, FText::FromString(_userName), FText::FromString(myName));
				}

				UserDBId userDBId = _userDBId;
				popup->SetData(textMsg1, textMsg2, [userDBId, this](bool InIsOk)
					{
						if (InIsOk)
						{
							FGsNetSendServiceGuild::SendReqChangeMemberGrade(userDBId, GuildMemberGradeType::CAPTAIN);

							// 열려있으면 닫을 것
							if (GUI()->IsActiveWidget(GetTableKey()))
							{
								Close();
							}
						}
					});
			}
		}

		return;
	}

	FGsNetSendServiceGuild::SendReqChangeMemberGrade(_userDBId, data._gradeType);

	Close();
}

void UGsUIPopupGuildChangeMemberGrade::OnInputCancel()
{
	Close();
}

void UGsUIPopupGuildChangeMemberGrade::SetData(const FGsGuildMemberData* InData)
{	
	_userDBId = InData->_userDBId;
	_userName = InData->_userName;
	_prevGrade = InData->_memberGrade;

	// 해당 유저의 현재 등급을 선택상태로 만든다
	int32 userGradeIndex = GetDataIndex(InData->_memberGrade);
	_toggleGroup.SetSelectedIndex(userGradeIndex);

	// 내 등급 인덱스
	GuildMemberGradeType myGrade = GGuild()->GetMemeberGrade();
	int32 myGradeIndex = GetDataIndex(myGrade);

	// 0: 길마, 1: 부길마, ...
	for (int32 i = 0, maxCount = _panel->GetChildrenCount(); i < maxCount; ++i)
	{
		UWidget* widget = _panel->GetChildAt(i);

		if (GuildMemberGradeType::CAPTAIN == myGrade)
		{
			// 길마는 길마를 넘겨야 하므로 모두 임명 가능. (길마 넘긴후엔 일반기사단원)
			widget->SetIsEnabled(true);
		}
		else
		{
			// 길마를 제외한 나머지는 나보다 등급이 같거나 낮은 사람만 임명 가능
			widget->SetIsEnabled((myGradeIndex < i ) ? true: false);
		}
	}
}

void UGsUIPopupGuildChangeMemberGrade::OnRefreshSlotEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _dataList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIGuildMemberGradeEntry* entry = Cast<UGsUIGuildMemberGradeEntry>(InEntry))
	{
		FGsGuildMemberGradeData& data = _dataList[InIndex];

		entry->SetData(data._gradeType, data._maxCount, data._currCount);
	}
}

void UGsUIPopupGuildChangeMemberGrade::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupGuildChangeMemberGrade::OnClickOk()
{	
	OnInputOk();
}

int32 UGsUIPopupGuildChangeMemberGrade::GetDataIndex(GuildMemberGradeType InGrade)
{
	// 0: 길마, 1: 부길마, ...
	return static_cast<int32>(GuildMemberGradeType::CAPTAIN) - static_cast<int32>(InGrade);
}

int32 UGsUIPopupGuildChangeMemberGrade::GetMemberCountByGrade(GuildMemberGradeType InGrade)
{
	int32 count = 0;
	TArray<FGsGuildMemberData*> guildMember = GGuild()->GetMemberList();
	for (FGsGuildMemberData* iter : guildMember)
	{
		if (InGrade == iter->_memberGrade)
		{
			++count;
		}
	}

	return count;
}
