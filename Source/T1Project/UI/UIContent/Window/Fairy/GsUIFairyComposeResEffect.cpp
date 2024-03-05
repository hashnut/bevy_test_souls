// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/Fairy/GsUIFairyComposeResEffect.h"
#include "Animation/WidgetAnimation.h"
#include "PanelWidget.h"

void UGsUIFairyComposeResEffect::NativeConstruct()
{
	Super::NativeConstruct();

	_costumeGradeEffects.Reset();
	{
		_costumeGradeEffects.Add(CostumeGrade::NORMAL, nullptr);
		_costumeGradeEffects.Add(CostumeGrade::MAGIC, MAGIC);
		_costumeGradeEffects.Add(CostumeGrade::RARE, RARE);
		_costumeGradeEffects.Add(CostumeGrade::EPIC, EPIC);
		_costumeGradeEffects.Add(CostumeGrade::UNIQUE, UNIQUE);
		_costumeGradeEffects.Add(CostumeGrade::LEGEND, LEGEND);
	}

	_fairyGradeEffects.Reset();
	{
		_fairyGradeEffects.Add(FairyGrade::NORMAL, nullptr);
		_fairyGradeEffects.Add(FairyGrade::MAGIC, MAGIC);
		_fairyGradeEffects.Add(FairyGrade::RARE, RARE);
		_fairyGradeEffects.Add(FairyGrade::EPIC, EPIC);
		_fairyGradeEffects.Add(FairyGrade::UNIQUE, UNIQUE);
		_fairyGradeEffects.Add(FairyGrade::LEGEND, LEGEND);
	}

	_FxGrade->SetVisibility(ESlateVisibility::Collapsed);
	StopAllAnimations();
}


void UGsUIFairyComposeResEffect::NativeDestruct()
{
	StopAllAnimations();
	_costumeGradeEffects.Reset();
	_fairyGradeEffects.Reset();
}

void UGsUIFairyComposeResEffect::SetCostumeResult(CostumeGrade postGrade, bool isEffect /*= false*/)
{
	//construct 이전에 들어오면 데이터가 없어야 합

	StopAllAnimations();
	_FxGrade->SetVisibility(isEffect ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (auto effectIter = _costumeGradeEffects.Find(postGrade))
	{
		if (*effectIter && isEffect) PlayAnimation(*effectIter, 0, 0);
	}
}

void UGsUIFairyComposeResEffect::SetFairyResult(FairyGrade postGrade, bool isEffect /*= false*/)
{
	//construct 이전에 들어오면 데이터가 없어야 합

	StopAllAnimations();
	_FxGrade->SetVisibility(isEffect ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (auto effectIter = _fairyGradeEffects.Find(postGrade))
	{
		if (*effectIter && isEffect) PlayAnimation(*effectIter, 0, 0);
	}
}
