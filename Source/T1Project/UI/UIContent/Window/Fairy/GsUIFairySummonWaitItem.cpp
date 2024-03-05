#include "GsUIFairySummonWaitItem.h"
#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsEffectTextManager.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Fairy/GsFairyData.h"
#include "Fairy/GsFairySummonConfirmData.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"

#include "UI/UIContent/Window/Fairy/GsUIFairySkillItem.h"
#include "UI/UIContent/Window/Fairy/GsUIFairyItem.h"
#include "UI/UIContent/Window/Fairy/GsUIFairyStat.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/CanvasPanel.h"
#include "Components/WidgetSwitcher.h"

#include "Currency/GsCurrencyHelper.h"
#include "Util/GsTimeSyncUtil.h"

void UGsUIFairySummonWaitItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_fairyLevel->SetVisibility(ESlateVisibility::Collapsed);

	_btnSummonConfirm->OnClicked.AddDynamic(this, &UGsUIFairySummonWaitItem::OnClickSummonConfirm);
	_btnChange->OnClickCurrencyButton.BindUObject(this, &UGsUIFairySummonWaitItem::OnClickSummonChange);

	_btnFindFairy->OnClicked.AddDynamic(this, &UGsUIFairySummonWaitItem::OnClickFindFairy);
	_btnViewCollection->OnClicked.AddDynamic(this, &UGsUIFairySummonWaitItem::OnClickViewFairyCollection);
	_btnChangeInfo->OnClicked.AddDynamic(this, &UGsUIFairySummonWaitItem::OnClickFairyChangeInfo);

	_btnSummonConfirm->SetOnClickLockSec(1.f);
	_btnChange->SetOnClickLockTime(1.f);

	FText Change;
	FText::FindText(TEXT("UICommonText"), TEXT("Change"), Change);
	_btnChange->SetButtonName(Change);
}

void UGsUIFairySummonWaitItem::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIFairySummonWaitItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_useCheck)
	{
		if (auto data = _data.Pin())
		{
			FTimespan spanTime = data->_confirmTime - FGsTimeSyncUtil::GetServerNowDateTimeUTC();
			int32 days = spanTime.GetTotalDays();
			int32 hours = 0;
			int32 minutes = 0;

			FString strRemainTime;
			if (days > 0)
			{
				strRemainTime.Append(FGsTimeStringHelper::GetDayString(days));

				if ((hours = spanTime.GetHours()) > 0)
					strRemainTime.Append(TEXT(" ") + FGsTimeStringHelper::GetHourString(hours));
			}
			else
			{
				hours = static_cast<int32>(spanTime.GetTotalHours());
				if (hours > 0)
				{
					strRemainTime.Append(FGsTimeStringHelper::GetHourString(hours));
					if ((minutes = spanTime.GetMinutes()) > 0)
						strRemainTime.Append(TEXT(" ") + FGsTimeStringHelper::GetMinuteString(minutes));
				}
				else
				{
					if ((minutes = static_cast<int32>(spanTime.GetTotalMinutes())) > 0)
					{
						strRemainTime.Append(FGsTimeStringHelper::GetMinuteString(minutes));
					}
				}
			}

			_useCheck = (days + hours + minutes > 0) && (data->_remainChangeCount > 0);

			_btnChange->SetIsEnabled(_useCheck);
			_txtRemainTime->SetText(FText::FromString(strRemainTime));
		}
	}
}

void UGsUIFairySummonWaitItem::RefreshUI()
{
	if (auto data = _data.Pin())
	{
		_btnChange->SetData(data->_currencyType, data->_currencyValue);
		_btnChange->SetIsEnabled(data->_remainChangeCount > 0 && 
			FGsCurrencyHelper::CheckCurrency(data->_currencyType, data->_currencyValue, false));

		_fairyPortrait->WidgetType = UGsUIFairyItem::UIItemType::SUMMONWAIT;
		_fairyPortrait->SetData(data->GetFairyData());
		_fairyPortrait->RefreshUI();

		_txtRemainCount->SetText(FText::AsNumber(data->_remainChangeCount));
		_txtRemainCount->SetColorAndOpacity(GetRemainAmountTextColor());

		_btnChangeInfo->SetIsEnabled(data->_remainChangeCount > 0);

		FString strRemainTime;
		if (data->_remainChangeCount > 0)
		{
			FTimespan spanTime = data->_confirmTime - FGsTimeSyncUtil::GetServerNowDateTimeUTC();
			int32 days = spanTime.GetTotalDays();
			int32 hours = 0;
			int32 minutes = 0;
			
			if (days > 0)
			{
				strRemainTime.Append(FGsTimeStringHelper::GetDayString(days));

				if ((hours = spanTime.GetHours()) > 0)
					strRemainTime.Append(TEXT(" ") + FGsTimeStringHelper::GetHourString(hours));
			}
			else
			{
				hours = static_cast<int32>(spanTime.GetTotalHours());
				if (hours > 0)
				{
					strRemainTime.Append(FGsTimeStringHelper::GetHourString(hours));
					if ((minutes = spanTime.GetMinutes()) > 0)
						strRemainTime.Append(TEXT(" ") + FGsTimeStringHelper::GetMinuteString(minutes));
				}
				else
				{
					if ((minutes = static_cast<int32>(spanTime.GetTotalMinutes())) > 0)
						strRemainTime.Append(FGsTimeStringHelper::GetMinuteString(minutes));
					else
					{
						FText FairyUI_SummonConfirm_Expired;
						FText::FindText(TEXT("FairyText"), TEXT("FairyUI_SummonConfirm_Expired"), FairyUI_SummonConfirm_Expired);

						strRemainTime = FairyUI_SummonConfirm_Expired.ToString();
					}
				}
			}

			_useCheck = (days + hours + minutes > 0) && (data->_remainChangeCount > 0);
			_btnChange->SetIsEnabled(_useCheck);
		}
		else
		{
			_useCheck = false;
			_btnChange->SetIsEnabled(false);
		}

		_txtRemainTime->SetColorAndOpacity(GetRemainTimeTextColor());
		_txtRemainTime->SetText(FText::FromString(strRemainTime));

		if (auto fairyData = data->GetFairyData().Pin())
		{
			FLinearColor color = FGsUIColorHelper::GetColorFairyGrade(fairyData->GetFairyGrade());

			_selectGrade->SetActiveWidgetIndex(static_cast<int>(fairyData->GetFairyGrade()) - 1);
			_fairyName->SetText(fairyData->GetFairyName());
			_fairyName->SetColorAndOpacity(color);
		}
	}
	else
	{
		
	}
}

void UGsUIFairySummonWaitItem::OnClickSummonChange()
{
	if (auto data = _data.Pin())
	{
		OnSummonChange.ExecuteIfBound(_data);
	}
}

void UGsUIFairySummonWaitItem::OnClickSummonConfirm()
{
	if (auto data = _data.Pin())
	{
		OnSummonConfirm.ExecuteIfBound(_data);
	}
}

void UGsUIFairySummonWaitItem::OnClickFindFairy()
{
	if (auto fairySummonData = _data.Pin())
	{
		if (auto fairyData = fairySummonData->GetFairyData().Pin())
		{
			OnFindFairyInfo.ExecuteIfBound(fairyData->_id);
		}
	}
}

void UGsUIFairySummonWaitItem::OnClickViewFairyCollection()
{
	if (auto fairySummonData = _data.Pin())
	{
		if (auto fairyData = fairySummonData->GetFairyData().Pin())
		{
			OnCollectionPopup.ExecuteIfBound(fairyData->_id);
		}
	}
}

void UGsUIFairySummonWaitItem::OnClickFairyChangeInfo()
{
	if (auto data = _data.Pin())
	{
		OnProbabilityPopup.ExecuteIfBound(_data);
	}
}

FSlateColor UGsUIFairySummonWaitItem::GetRemainAmountTextColor() const
{
	static FSlateColor _defaultRemainTextColor = _txtRemainCount->ColorAndOpacity;

	if (auto data = _data.Pin())
	{
		return data->_remainChangeCount > 0 ? _defaultRemainTextColor : FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_LACK);
	}

	return _defaultRemainTextColor;
}

FSlateColor UGsUIFairySummonWaitItem::GetRemainTimeTextColor() const
{
	static FSlateColor _defaultRemainTextColor = _txtRemainTime->ColorAndOpacity;

	if (auto data = _data.Pin())
	{
		FTimespan spanTime = data->_confirmTime - FGsTimeSyncUtil::GetServerNowDateTimeUTC();

		return spanTime.GetTotalMinutes() > 0 ? _defaultRemainTextColor : FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_LACK);
	}

	return _defaultRemainTextColor;
}
