// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupGuildDonation.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "Guild/GsGuildData.h"
#include "Guild/GsGuildHelper.h"
#include "Currency/GsCurrencyHelper.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"
#include "DataSchema/Guild/GsSchemaGuildDailyActivity.h"
#include "Net/GsNetSendServiceGuild.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Reward/GsRewardHelper.h"


void UGsUIPopupGuildDonation::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupGuildDonation::OnClickClose);
	_btnNormal->OnClickCurrencyButton.BindUObject(this, &UGsUIPopupGuildDonation::OnClickNormal);
	_btnAdvanced->OnClickCurrencyButton.BindUObject(this, &UGsUIPopupGuildDonation::OnClickAdvanced);
}

void UGsUIPopupGuildDonation::NativeConstruct()
{
	Super::NativeConstruct();

	FGsMessageHolder* messageMgr = GMessage();
	_msgHandleGuild = messageMgr->GetGuild().AddUObject(MessageContentGuild::RECEIVE_DAILY_ACTIVITY,
		this, &UGsUIPopupGuildDonation::OnReceiveDonationResult);
	_msgHandleSystem = messageMgr->GetSystemParam().AddUObject(MessageSystem::RESET_TIME_CONTENTS,
		this, &UGsUIPopupGuildDonation::OnResetTimeContents);
	_msgHandleUserInfo = messageMgr->GetUserBasicInfo().AddUObject(MessageUserInfo::CURRENCY,
		this, &UGsUIPopupGuildDonation::OnChangeCurrency);


	const FGsSchemaGuildConfig* config = FGsGuildHelper::GetGuildConfig();
	if (nullptr == config)
	{
		return;
	}

	// TEXT: 기부
	FText textBtnName;
	FText::FindText(TEXT("GuildText"), TEXT("UI_Donation_Application"), textBtnName);

	_donationMaxCount = config->dailyDonateLimits;

	//---------------------------------------------------------
	auto FuncGetActivity = [](const FGsSchemaGuildConfig* InConfig, GuildDailyActivityType InActivityType)-> const FGsSchemaGuildDailyActivity*
	{
		for (const FGsSchemaGuildDailyActivity& iter : InConfig->guildDailyActivityList)
		{
			if (iter.type == InActivityType)
			{
				return &iter;
			}
		}

		return nullptr;
	};
	//---------------------------------------------------------

	// 0은 출석정보. 1이 일반기부
	if (config->guildDailyActivityList.IsValidIndex(1))
	{
		const FGsSchemaGuildDailyActivity* normalData = FuncGetActivity(config, GuildDailyActivityType::NORMAL_DONATE);
		if (normalData)
		{
			_normalType = normalData->currencyType;
			_normalAmount = normalData->currencyAmount;

			_btnNormal->SetData(_normalType, _normalAmount);
			_btnNormal->SetButtonName(textBtnName);

			_textBlockNormalContribution->SetText(FText::AsNumber(normalData->rewardContribution));

			Currency assetAmount = 0;
			int32 coinAmount = 0;
			if (const FGsSchemaRewardData* rewardData = normalData->rewardId.GetRow())
			{
				GetRewardAmount(rewardData, assetAmount, coinAmount);
			}
			
			_textBlockNormalGuildAsset->SetText(FText::AsNumber(assetAmount));
			_textBlockNormalGuildCoin->SetText(FText::AsNumber(coinAmount));
		}
	}

	if (config->guildDailyActivityList.IsValidIndex(2))
	{
		const FGsSchemaGuildDailyActivity* advancedData = FuncGetActivity(config, GuildDailyActivityType::ADVANCED_DONATE);
		if (advancedData)
		{
			_advancedType = advancedData->currencyType;
			_advancedAmount = advancedData->currencyAmount;

			_btnAdvanced->SetData(_advancedType, _advancedAmount);
			_btnAdvanced->SetButtonName(textBtnName);

			_textBlockAdvancedContribution->SetText(FText::AsNumber(advancedData->rewardContribution));

			Currency assetAmount = 0;
			int32 coinAmount = 0;
			if (const FGsSchemaRewardData* rewardData = advancedData->rewardId.GetRow())
			{
				GetRewardAmount(rewardData, assetAmount, coinAmount);
			}

			_textBlockAdvancedGuildAsset->SetText(FText::AsNumber(assetAmount));
			_textBlockAdvancedGuildCoin->SetText(FText::AsNumber(coinAmount));			
		}
	}

	// 기부 횟수 표시
	InvalidateCount();
}

void UGsUIPopupGuildDonation::NativeDestruct()
{
	FGsMessageHolder* messageMgr = GMessage();
	messageMgr->GetGuild().Remove(_msgHandleGuild);
	messageMgr->GetSystemParam().Remove(_msgHandleSystem);
	messageMgr->GetUserBasicInfo().Remove(_msgHandleUserInfo);

	Super::NativeDestruct();
}

void UGsUIPopupGuildDonation::OnInputCancel()
{
	Close();
}

void UGsUIPopupGuildDonation::InvalidateCount()
{
	const FGsGuildDataMine* guildData = GGuild()->GetMyGuildData();
	if (nullptr == guildData)
	{
		return;
	}
	
	// 일반
	int32 remainCount = _donationMaxCount - guildData->_donationCount;
	_textBlockRemainCount->SetText(FText::AsNumber(remainCount));
	_textBlockRemainCount->SetColorAndOpacity((0 < remainCount) ?
		FGsUIColorHelper::GetColor(EGsUIColorType::GUILD_DONATION_VALID) :
		FGsUIColorHelper::GetColor(EGsUIColorType::GUILD_DONATION_INVALID));	
	   
	_btnNormal->UpdateAmountTextColorLocalPlayer();
	_btnNormal->SetIsEnabled(CheckCountNormal());

	_btnAdvanced->UpdateAmountTextColorLocalPlayer();
	_btnAdvanced->SetIsEnabled(CheckCountAdvanced());	
}

bool UGsUIPopupGuildDonation::CheckCountNormal()
{
	const FGsGuildDataMine* guildData = GGuild()->GetMyGuildData();
	if (guildData)
	{
		if (_donationMaxCount > guildData->_donationCount)
		{
			return true;
		}
	}

	return false;
}

bool UGsUIPopupGuildDonation::CheckCountAdvanced()
{
	const FGsGuildDataMine* guildData = GGuild()->GetMyGuildData();
	if (guildData)
	{
		if (_donationMaxCount > guildData->_donationCount)
		{
			return true;
		}
	}

	return false;
}

void UGsUIPopupGuildDonation::OnReceiveDonationResult(const IGsMessageParam*)
{
	InvalidateCount();
}

void UGsUIPopupGuildDonation::OnResetTimeContents(const IGsMessageParam*)
{
	// 뭘받았건 상관없이 카운트 갱신
	InvalidateCount();
}

void UGsUIPopupGuildDonation::OnChangeCurrency(uint64 InCurrncyType)
{
	//CurrencyType currencyType = static_cast<CurrencyType>(InCurrncyType);
	// 타입 검사해서 갱신해도 되지만, 검사 안해도 크게 부하는 없을 것
	InvalidateCount();
}

void UGsUIPopupGuildDonation::OnClickNormal()
{
	if (false == CheckCountNormal())
	{
		// TEXT: 일일 기부 횟수를 모두 소진하였습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_DailyDonationEnd"), textMsg);
		FGsUIHelper::TrayMessageTicker(textMsg);
		return;
	}

	// 재화가 충분한지 체크. 부족하면 팝업
	Currency myAmount = FGsCurrencyHelper::GetCurrencyAmount(_normalType);
	if (_normalAmount > myAmount)
	{
		FGsCurrencyHelper::OpenLackCurrencyPopup(_normalType, _normalAmount - myAmount);
	}
	else
	{
		FGsNetSendServiceGuild::SendReqDailyActivity(GuildDailyActivityType::NORMAL_DONATE);
	}
}

void UGsUIPopupGuildDonation::OnClickAdvanced()
{
	if (false == CheckCountAdvanced())
	{
		// TEXT: 일일 기부 횟수를 모두 소진하였습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_DailyDonationEnd"), textMsg);
		FGsUIHelper::TrayMessageTicker(textMsg);
		return;
	}

	// 재화가 충분한지 체크. 부족하면 부족팝업
	Currency myAmount = FGsCurrencyHelper::GetCurrencyAmount(_advancedType);
	if (_advancedAmount > myAmount)
	{
		FGsCurrencyHelper::OpenLackCurrencyPopup(_advancedType, _advancedAmount - myAmount);
	}
	else
	{
		// TEXT: 고급 기부를 진행하시겠습니까?
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("UI_FineDonate_Check"), textMsg);

		FGsUIHelper::PopupYesNo(textMsg, [](bool bInIsOk)
			{
				if (bInIsOk)
				{
					FGsNetSendServiceGuild::SendReqDailyActivity(GuildDailyActivityType::ADVANCED_DONATE);
				}
			});
	}
}

void UGsUIPopupGuildDonation::OnClickClose()
{
	OnInputCancel();
}

void UGsUIPopupGuildDonation::GetRewardAmount(const FGsSchemaRewardData* InReward,
	OUT Currency& OutGuildAssetAmount, OUT int32& OutGuildCoinAmount)
{
	FGsRewardUnboxingData unboxData;
	FGsRewardHelper::UnboxingRewardData(InReward, unboxData);

	// 길드 에셋
	if (CurrencyType::GUILD_ASSET == unboxData._currencyType)
	{
		OutGuildAssetAmount = unboxData.GetCommonCurrencyAmount();
	}

	// 길드 코인	
	if (0 < unboxData._itemDataList.Num())
	{
		OutGuildCoinAmount = unboxData._itemDataList[0]->itemCountMin;
	}
}
