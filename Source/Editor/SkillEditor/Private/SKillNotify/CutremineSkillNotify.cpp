// Fill out your copyright notice in the Description page of Project Settings.


#include "CutremineSkillNotify.h"

UCutremineSkillNotify::UCutremineSkillNotify()
	: Super()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(192, 255, 99, 255);
#endif // WITH_EDITORONLY_DATA
}

#if WITH_EDITOR
void UCutremineSkillNotify::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		OnChangeSkillNotify.ExecuteIfBound();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
