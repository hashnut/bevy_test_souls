// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomCollisionSkillNotify.h"

URandomCollisionSkillNotify::URandomCollisionSkillNotify()
	: Super()
{

}

#if WITH_EDITOR
void URandomCollisionSkillNotify::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		OnChangeRandomCollisionSkillNotify.ExecuteIfBound();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void URandomCollisionSkillNotify::ValidateAssociatedAssets()
{
}
#endif