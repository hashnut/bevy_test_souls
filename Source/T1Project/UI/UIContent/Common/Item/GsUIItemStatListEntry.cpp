// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIItemStatListEntry.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"

void UGsUIItemStatListEntry::NativeConstruct()
{
	Super::NativeConstruct();

	_isValidStat = false;
	SetCompareObjectVisibility(false);
}

void UGsUIItemStatListEntry::SetTextName(const FText& InText)
{
	_textBlockName->SetText(InText);
}

void UGsUIItemStatListEntry::SetTextValue(const FText& InText)
{
	_textBlockValue->SetText(InText);
	_isValidStat = true;
}

void UGsUIItemStatListEntry::SetColorAllText(const FLinearColor& InColor)
{
	_textBlockName->SetColorAndOpacity(InColor);
	_textBlockValue->SetColorAndOpacity(InColor);
}

void UGsUIItemStatListEntry::SetCompareObjectVisibility(const bool InState)
{
	// 0 arrow , 1 same 2 new or remove stat
	_compareTypeSwitcher->SetVisibility(false == InState ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIItemStatListEntry::SetCompareArrowSwticher(const bool InIsUpArrow)
{
	_compareTypeSwitcher->SetActiveWidgetIndex(0);

	// 0 down , 1 up
	_compareArrowSwitcher->SetActiveWidgetIndex((uint8)InIsUpArrow);
	_eCompareExpectStatType = true == InIsUpArrow ? CompareExpectStatStateType::UpGrade : CompareExpectStatStateType::DownGrade;
}

void UGsUIItemStatListEntry::SetCompareValueText(const FText& InValueText, const FLinearColor& InColor)
{
	_compareTypeSwitcher->SetActiveWidgetIndex(0);
	_compareValueText->SetText(InValueText);
	_compareValueText->SetColorAndOpacity(InColor);
}

void UGsUIItemStatListEntry::SetCompareExptectNewOrRemoveStatText(const bool InIsNewStat, const FLinearColor& InColor)
{
	FText text;
	FTextKey key = FTextKey(TEXT(""));
	_compareTypeSwitcher->SetActiveWidgetIndex(2);
	key = true == InIsNewStat ? TEXT("ItemUi_Compare_New") : TEXT("ItemUi_Compare_Exclusion");
	FText::FindText(TEXT("ItemUIText"), key, text);
	_onChangeStatText->SetText(text);
	_onChangeStatText->SetColorAndOpacity(InColor);

	_eCompareExpectStatType = true == InIsNewStat ? CompareExpectStatStateType::NewStat : CompareExpectStatStateType::RemoveStat;
}

void UGsUIItemStatListEntry::SetSameValueStat()
{
	_compareTypeSwitcher->SetActiveWidgetIndex(1);
	_eCompareExpectStatType = CompareExpectStatStateType::Same;
}

void UGsUIItemStatListEntry::SetColorByCompareStateType(const bool InState)
{
	if (false == InState || CompareExpectStatStateType::Same == _eCompareExpectStatType)
	{
		SetColorAllText(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_STAT_COLOR));
		return;
	}

	SetColorAllText(FGsUIColorHelper::GetColor(CompareExpectStatStateType::RemoveStat == _eCompareExpectStatType ?
		EGsUIColorType::DISABLE_STAT_COLOR : EGsUIColorType::DEFAULT_STAT_COLOR));
}

bool UGsUIItemStatListEntry::IsValidDislayStat()
{
	//int32 value = FCString::Atoi(*_textBlockValue->GetText().ToString());
	return _isValidStat;
}