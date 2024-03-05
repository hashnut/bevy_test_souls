// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillEnchantProgress.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

void UGsUISkillEnchantProgress::NativeConstruct()
{
	Super::NativeConstruct();

	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
}

void UGsUISkillEnchantProgress::NativeDestruct()
{
	_remainTime = 0.f;

	Super::NativeDestruct();
}

void UGsUISkillEnchantProgress::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (0.f < _remainTime)
	{
		_remainTime -= InDeltaTime;
		if (0.f >= _remainTime)
		{
			_remainTime = 0.f;

			OnProgressTimeFinished.ExecuteIfBound();
		}
	}
}

 void UGsUISkillEnchantProgress::StartProgress()
 {
	 StartProgressAnimation();

	 _remainTime = _progressTime;
 }
