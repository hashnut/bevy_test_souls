


#include "GsUITitleEquipGradeLoopEffect.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "../BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "T1Project.h"



void UGsUITitleEquipGradeLoopEffect::BeginDestroy()
{
	Super::BeginDestroy();


}

void UGsUITitleEquipGradeLoopEffect::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UWidgetBlueprintGeneratedClass* genClass = GetWidgetTreeOwningClass();

	TMap<TitleGrade, FString> textGrade;
	textGrade.Emplace(TitleGrade::NORMAL, TEXT("NORMAL"));
	textGrade.Emplace(TitleGrade::MAGIC, TEXT("MAGIC"));
	textGrade.Emplace(TitleGrade::RARE, TEXT("RARE"));
	textGrade.Emplace(TitleGrade::EPIC, TEXT("EPIC"));
	textGrade.Emplace(TitleGrade::UNIQUE, TEXT("UNIQUE"));
	textGrade.Emplace(TitleGrade::LEGEND, TEXT("LEGEND"));

	if (nullptr != genClass)
	{
		const int32 maxCount = StaticCast<int32>(TitleGrade::MAX);
		for (int32 idx = 0; idx < maxCount; ++idx)
		{
			TitleGrade findGrade = StaticCast<TitleGrade>(idx);

			for (UWidgetAnimation* anim : genClass->Animations)
			{
				if (anim->GetMovieScene())
				{
					GSLOG(Warning, TEXT("Move Scene Name : [%s]"), *anim->GetMovieScene()->GetName());

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

void UGsUITitleEquipGradeLoopEffect::NativeConstruct()
{
	Super::NativeConstruct();

	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);

	SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUITitleEquipGradeLoopEffect::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUITitleEquipGradeLoopEffect::SetData(bool InIsShow, TitleGrade InGrade)
{
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);

	if (false == InIsShow || false == _animationDatas.Contains(InGrade))
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		static int32 LOOPS_TO_PLAY = 1;
		UWidgetAnimation* currentAniamtion = _animationDatas[InGrade];
		PlayAnimation(currentAniamtion, 0.f, LOOPS_TO_PLAY);

		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}