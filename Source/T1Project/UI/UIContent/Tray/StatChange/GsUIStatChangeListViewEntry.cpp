// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIStatChangeListViewEntry.h"
#include "Components/WidgetSwitcher.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "UTIL/GsText.h"
#include "Components/TextBlock.h"
#include "Management/ScopeGame/GsStatChangeNotifyManager.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"

void UGsUIStatChangeListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	if (nullptr == inListItemObject)
	{
		return;
	}

	UGsUIStatChangeListViewEntryData* data = Cast<UGsUIStatChangeListViewEntryData>(inListItemObject);
	if (nullptr == data)
	{
		return;
	}

	_statType = data->_statType;
	_statValue = 0;
	_newValue = data->_newValue;
	_oldValue = data->_oldVlaue;

	InvalidWidget();	

	PlayAnimation(_openAnim);
}

void UGsUIStatChangeListViewEntry::InvalidWidget()
{
	InvalidStatName();
	InvalidStatValue();
	InvalidUpDown();
}

void UGsUIStatChangeListViewEntry::InvalidStatValue()
{	
	FText statValueText;
	int value = FMath::Abs(_newValue - _oldValue);
	if (false == FGsStatHelper::GetDisPlayStatValueWithMarkFormat(_statType, value, statValueText))
	{
		FString statName = FGsTextUtil::GetStringFromEnum(TEXT("StatType"), _statType);
		GSLOG(Error, TEXT("Stat format not exist - stat name : %s"), *statName);
		return;
	}

	_statValueText->SetText(statValueText);
}

void UGsUIStatChangeListViewEntry::InvalidUpDown()
{
	if (_oldValue < _newValue)
	{
		_upDownWidgetSwitcher->SetActiveWidgetIndex((int)FGsStatChangeNotifyManager::EGsUpDown::Up);
		_statValueText->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::UP_COMPARE_STAT_COLOR));
	}
	else if(_oldValue > _newValue)
	{
		_upDownWidgetSwitcher->SetActiveWidgetIndex((int)FGsStatChangeNotifyManager::EGsUpDown::Down);
		_statValueText->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DOWN_COMPARE_STAT_COLOR));
	}
	else
	{
		_upDownWidgetSwitcher->SetActiveWidgetIndex((int)FGsStatChangeNotifyManager::EGsUpDown::Same);
		_statValueText->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::NORMAL_TEXT_COLOR));
	}
}
