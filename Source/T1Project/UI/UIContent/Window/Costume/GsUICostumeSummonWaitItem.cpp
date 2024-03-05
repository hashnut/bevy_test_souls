#include "GsUICostumeSummonWaitItem.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGame/GsEffectTextManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"

#include "Costume/GsCostumeData.h"
#include "Costume/GsCostumeSummonConfirmData.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Popup/GsUIPopupSkillDetailInfo.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"

#include "GameObject/Stat/GsStatHelper.h"
#include "Currency/GsCurrencyHelper.h"
#include "UI/UIContent/Window/Fairy/GsUIFairyStat.h"
#include "Util/GsTimeSyncUtil.h"

#include "GsUICostumeItem.h"
#include "GsUICostumeSkillTooltip.h"
#include "GsUICostumeSkillIconItem.h"

#include "PaperSprite.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"

void UGsUICostumeSummonWaitItem::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUICostumeSummonWaitItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSummonConfirm->OnClicked.AddDynamic(this, &UGsUICostumeSummonWaitItem::OnClickSummonConfirm);
	_btnChange->OnClickCurrencyButton.BindUObject(this, &UGsUICostumeSummonWaitItem::OnClickSummonChange);

	_btnFindCostume->OnClicked.AddDynamic(this, &UGsUICostumeSummonWaitItem::OnClickFindCostume);
	_btnViewCollection->OnClicked.AddDynamic(this, &UGsUICostumeSummonWaitItem::OnClickViewCostumeCollection);
	_btnChangeInfo->OnClicked.AddDynamic(this, &UGsUICostumeSummonWaitItem::OnClickCostumeChangeInfo);

	_btnSummonConfirm->SetOnClickLockSec(1.f);
	_btnChange->SetOnClickLockTime(1.f);

	FText Change;
	FText::FindText(TEXT("UICommonText"), TEXT("Change"), Change);
	_btnChange->SetButtonName(Change);
}

void UGsUICostumeSummonWaitItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_useCheck)
	{
		RefreshRemainTimeSet();
	}
}

void UGsUICostumeSummonWaitItem::RefreshUI()
{
	if (auto data = _data.Pin())
	{
		_btnChange->SetData(data->_currencyType, data->_currencyValue);
		_btnChange->SetIsEnabled(data->_remainChangeCount > 0 &&
			FGsCurrencyHelper::CheckCurrency(data->_currencyType, data->_currencyValue, false));

		_portrait->WidgetType = UGsUICostumeItem::UIItemType::SUMMONWAIT;
		_portrait->SetData(data->_costumeData, _genderType);
		_portrait->RefreshUI();

		_txtRemainCount->SetText(FText::AsNumber(data->_remainChangeCount));
		_txtRemainCount->SetColorAndOpacity(GetRemainAmountTextColor());

		_btnChangeInfo->SetIsEnabled(data->_remainChangeCount > 0);
		
		RefreshRemainTimeSet();
		RefreshCostumeData(data->_costumeData);
	}
	else
	{
		TArray<UGsUIFairyStat*> statList(_itemMainStatList);
		statList.Append(_itemSubStatList);
		for (auto e : statList)
		{
			e->SetVisibility(ESlateVisibility::Collapsed);
		}
		_costumeName->SetText(FText::GetEmpty());
		_costumeGrade->SetText(FText::GetEmpty());
	}

}

void UGsUICostumeSummonWaitItem::OnClickSummonChange()
{
	if (auto data = _data.Pin())
	{
		OnSummonChange.ExecuteIfBound(data);
	}
}

void UGsUICostumeSummonWaitItem::OnClickSummonConfirm()
{
	if (auto data = _data.Pin())
	{
		OnSummonConfirm.ExecuteIfBound(data);
	}
}

void UGsUICostumeSummonWaitItem::OnClickFindCostume()
{
	if (auto summonData = _data.Pin())
	{
		if (auto costumeData = summonData->GetCostumeData().Pin())
		{
			OnFindCostumeInfo.ExecuteIfBound(costumeData->_id);
		}
	}
}

void UGsUICostumeSummonWaitItem::OnClickViewCostumeCollection()
{
	if (auto summonData = _data.Pin())
	{
		if (auto costumeData = summonData->GetCostumeData().Pin())
		{
			OnCollectionPopup.ExecuteIfBound(costumeData->_id);
		}
	}
}

void UGsUICostumeSummonWaitItem::OnClickCostumeChangeInfo()
{
	if (auto data = _data.Pin())
	{
		OnProbabilityPopup.ExecuteIfBound(_data);
	}
}

void UGsUICostumeSummonWaitItem::RefreshRemainTimeSet()
{
	FString strRemainTime;
	if (auto data = _data.Pin())
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
				{
					strRemainTime.Append(FGsTimeStringHelper::GetMinuteString(minutes));
				}
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

	_txtRemainTime->SetText(FText::FromString(strRemainTime));
	_txtRemainTime->SetColorAndOpacity(GetRemainTimeTextColor());
}

void UGsUICostumeSummonWaitItem::RefreshCostumeData(TWeakPtr<FGsCostumeData> data)
{
	_costumeName->SetText(FText::GetEmpty());
	_costumeGrade->SetText(FText::GetEmpty());

	if (auto costumeData = data.Pin())
	{
		FLinearColor gradeColor = FGsUIColorHelper::GetColorGradeWidthItemGrade(costumeData->GetCostumeGrade());
		FText gradeText;
		switch (costumeData->GetCostumeGrade())
		{
		case CostumeGrade::NORMAL:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Normal"), gradeText);	break;
		case CostumeGrade::MAGIC:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Magic"), gradeText);	break;
		case CostumeGrade::RARE:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Rare"), gradeText);		break;
		case CostumeGrade::UNIQUE:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Unique"), gradeText);	break;
		case CostumeGrade::EPIC:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Epic"), gradeText);		break;
		case CostumeGrade::LEGEND:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Legend"), gradeText);	break;
		default: break;
		}

		_selectGrade->SetActiveWidgetIndex(static_cast<int>(costumeData->GetCostumeGrade()) - 1);

		_costumeName->SetText(costumeData->GetCostumeName());
		_costumeGrade->SetText(gradeText);
		_costumeName->SetColorAndOpacity(gradeColor);
		_costumeGrade->SetColorAndOpacity(gradeColor);

		_skillList.Empty();
		if (const auto activeSkill = costumeData->GetCostumeActiveSkill())
		{
			if (const auto useSkill = activeSkill->userSkillSetId.GetRow())
			{
				const auto skillData = (_genderType == CreatureGenderType::MALE) ?
					useSkill->maleSkillId.GetRow() : useSkill->femaleSkillId.GetRow();

				_skillList.Add({ true, skillData->id, activeSkill->weaponType, useSkill->icon });
			}
		}

		for (auto weaponType : { CreatureWeaponType::ALL, CreatureWeaponType::SWORD, CreatureWeaponType::DAGGER
								,CreatureWeaponType::BOW, CreatureWeaponType::WAND, CreatureWeaponType::HAND })
		{
			auto selectPassiveSkills = costumeData->GetCostumePassivityList(weaponType);
			for (const auto passiveSkill : selectPassiveSkills)
			{
				_skillList.Add({ false, passiveSkill->id, weaponType, passiveSkill->iconPath });
			}
		}
	}
}

FSlateColor UGsUICostumeSummonWaitItem::GetRemainAmountTextColor() const
{
	static FSlateColor _defaultRemainTextColor = _txtRemainCount->ColorAndOpacity;

	if (auto data = _data.Pin())
	{
		return data->_remainChangeCount > 0 ? _defaultRemainTextColor : FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_LACK);
	}

	return _defaultRemainTextColor;
}

FSlateColor UGsUICostumeSummonWaitItem::GetRemainTimeTextColor() const
{
	static FSlateColor _defaultRemainTextColor = _txtRemainTime->ColorAndOpacity;

	if (auto data = _data.Pin())
	{
		FTimespan spanTime = data->_confirmTime - FGsTimeSyncUtil::GetServerNowDateTimeUTC();

		return spanTime.GetTotalMinutes() > 0 ? _defaultRemainTextColor : FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_LACK);
	}

	return _defaultRemainTextColor;
}
