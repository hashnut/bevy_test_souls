// Fill out your copyright notice in the Description page of Project Settings.


#include "CancleSkillNotify.h"

UCancleSkillNotify::UCancleSkillNotify()
	: Super()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(192, 255, 99, 255);
#endif // WITH_EDITORONLY_DATA
}

void UCancleSkillNotify::PostLoad()
{
	Super::PostLoad();
}

#if WITH_EDITOR
void UCancleSkillNotify::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		OnChangeCancleSkillNotify.ExecuteIfBound();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif