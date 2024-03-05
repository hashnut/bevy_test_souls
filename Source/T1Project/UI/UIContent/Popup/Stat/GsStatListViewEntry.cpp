// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStatListViewEntry.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Stat/GsPlayerStatHandler.h"
#include "Components/WidgetSwitcher.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "Components/TextBlock.h"
#include "Management/GsMessageHolder.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UILib/Base/GsUIVFX.h"

void UGsStatListViewEntry::NativeConstruct()
{
	Super::NativeConstruct();

	_changedStatEffect = Cast<UGsUIVFX>(GetWidgetFromName(TEXT("C_StatEffect")));
	_statValue = 0;
}

void UGsStatListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	if (nullptr == inListItemObject)
	{
		return;
	}

	UGsStatListViewEntryData* data = Cast<UGsStatListViewEntryData>(inListItemObject);
	if (nullptr == data)
	{
		return;
	}
	
	_statType = data->_statType;
	_statValue = 0;

	InvalidWidget();
}

void UGsStatListViewEntry::InvalidWidget()
{
	InvalidStatName();
	InvalidStatValue();
}

void UGsStatListViewEntry::InvalidStatName()
{
	FText statNameText;
	FGsStatHelper::GetDisPlayStatName(_statType, statNameText);	
	_statNameText->SetText(statNameText);
}

void UGsStatListViewEntry::InvalidStatValue()
{
	FText statValueText;
	FGsPlayerStatHandler::GetInstance()->TryGetPlayerStatText(_statType, statValueText);
	_statValueText->SetText(statValueText);
}

void UGsStatListViewEntry::OnInvalidWidget()
{
	InvalidWidget();
}

void UGsStatListViewEntry::OnInvalidWidget(const struct IGsMessageParam* inParam)
{
	InvalidWidget();
}

void UGsStatListViewEntry::SetStatName(StatType inStatType)
{
	if (_statNameText)
	{
		FText statName;
		if (FGsStatHelper::GetDisPlayStatName(inStatType, statName))
		{
			_statNameText->SetText(statName);
		}
	}

	_statValue = 0;
}

void UGsStatListViewEntry::SetStatValue(const FText& inStatValueText, int32 inStatValue)
{
	if (_statValueText)
	{
		_statValueText->SetText(inStatValueText);
	}

	_statValue = inStatValue;
}

void UGsStatListViewEntry::SetHighlight(bool inHighlight)
{
	if (nullptr == _statNameText)
	{
		return;
	}

	if (nullptr == _statValueText)
	{
		return;
	}

	if (inHighlight)
	{
		FLinearColor highlight = FGsUIColorHelper::GetColor(EGsUIColorType::HIGHLIGHT_TEXT_COLOR);
		_statValueText->SetColorAndOpacity(highlight);
	}
	else
	{
		FLinearColor normal = FGsUIColorHelper::GetColor(EGsUIColorType::NORMAL_TEXT_COLOR);
		_statValueText->SetColorAndOpacity(normal);
	}
}

void UGsStatListViewEntry::ShowEffect()
{
	if (_changedStatEffect)
	{
		_changedStatEffect->ShowVFX();
	}
}
