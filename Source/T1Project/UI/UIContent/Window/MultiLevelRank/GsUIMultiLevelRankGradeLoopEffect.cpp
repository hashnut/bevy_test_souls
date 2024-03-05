// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIMultiLevelRankGradeLoopEffect.h"

#include "Animation/WidgetAnimation.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"


void UGsUIMultiLevelRankGradeLoopEffect::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UWidgetBlueprintGeneratedClass* genClass = GetWidgetTreeOwningClass();

	TMap<ItemGrade, FString> textGrade;
	textGrade.Emplace(ItemGrade::NORMAL, TEXT("NORMAL"));
	textGrade.Emplace(ItemGrade::MAGIC, TEXT("MAGIC"));
	textGrade.Emplace(ItemGrade::RARE, TEXT("RARE"));
	textGrade.Emplace(ItemGrade::EPIC, TEXT("EPIC"));
	textGrade.Emplace(ItemGrade::UNIQUE, TEXT("UNIQUE"));
	textGrade.Emplace(ItemGrade::LEGEND, TEXT("LEGEND"));

	if (nullptr != genClass)
	{
		const int32 maxCount = StaticCast<int32>(ItemGrade::MAX);
		for (int32 idx = StaticCast<int32>(ItemGrade::NORMAL); idx < maxCount; ++idx)
		{
			ItemGrade findGrade = StaticCast<ItemGrade>(idx);
			if (false == textGrade.Contains(findGrade))
				continue;

			for (UWidgetAnimation* anim : genClass->Animations)
			{
				if (anim->GetMovieScene())
				{
					if (0 == anim->GetMovieScene()->GetName().Compare(textGrade[findGrade], ESearchCase::IgnoreCase))
					{
						_animationDatas.FindOrAdd(findGrade) = anim;
						break;
					}
				}
			}
		}
	}
}

void UGsUIMultiLevelRankGradeLoopEffect::NativeConstruct()
{
	Super::NativeConstruct();
	EndEffect();
}

void UGsUIMultiLevelRankGradeLoopEffect::StartEffect(ItemGrade InGrade)
{
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);

	if (_animationDatas.Contains(InGrade))
	{
		static int32 LOOPS_TO_PLAY = 0;
		UWidgetAnimation* currentAniamtion = _animationDatas[InGrade];
		PlayAnimation(currentAniamtion, 0.f, LOOPS_TO_PLAY);

		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIMultiLevelRankGradeLoopEffect::EndEffect()
{
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	SetVisibility(ESlateVisibility::Collapsed);
}