// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRecomposeListEntry.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "Summon/GsSummonFunc.h"


void UGsUIRecomposeListEntry::BeginDestroy()
{
	if (nullptr != _btnSlot)
	{
		_btnSlot->OnClicked.RemoveDynamic(this, &UGsUIRecomposeListEntry::OnClickedGradeSlot);
	}
	_btnSlot = nullptr;

	Super::BeginDestroy();
}

void UGsUIRecomposeListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIRecomposeListEntry::OnClickedGradeSlot);
}

void UGsUIRecomposeListEntry::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsSelected(false);

	_costumeGrade = CostumeGrade::NONE;
	_fairyGrade = FairyGrade::NONE;
}

void UGsUIRecomposeListEntry::OnClickedGradeSlot()
{
	if (_canRecompose)
	{
		OnClickGradeSlot.ExecuteIfBound(this);
	}
}

void UGsUIRecomposeListEntry::SetCostumeGrade(CostumeGrade InGrade, bool InCanRecompose)
{
	_costumeGrade = InGrade;

	const FLinearColor& gradeColor = FGsUIColorHelper::GetColorSummonCostumeGradeText(_costumeGrade);
	_textBlockName->SetColorAndOpacity(gradeColor);
	_textBlockName->SetText(FGsSummonFunc::GetCostumeGradeText(_costumeGrade));

	_widgetFocus->SetColorAndOpacity(gradeColor);

	_canRecompose = InCanRecompose;
	if (InCanRecompose)
	{
		// 재합성 가능
	}
	else
	{
		// 재합성 불가
	}
}

void UGsUIRecomposeListEntry::SetFairyGrade(FairyGrade InGrade, bool InCanRecompose)
{
	_fairyGrade = InGrade;

	const FLinearColor& gradeColor = FGsUIColorHelper::GetColorSummonFairyGradeText(_fairyGrade);
	_textBlockName->SetColorAndOpacity(gradeColor);
	_textBlockName->SetText(FGsSummonFunc::GetFairyGradeText(_fairyGrade));

	_widgetFocus->SetColorAndOpacity(gradeColor);

	_canRecompose = InCanRecompose;
	if (InCanRecompose)
	{
		// 재합성 가능
	}
	else
	{
		// 재합성 불가
	}
}

void UGsUIRecomposeListEntry::SetIsSelected(bool InIsSelected)
{
	_widgetFocus->SetVisibility(InIsSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}
