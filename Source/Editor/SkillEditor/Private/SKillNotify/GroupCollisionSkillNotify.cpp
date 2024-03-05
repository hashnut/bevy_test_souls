// Fill out your copyright notice in the Description page of Project Settings.


#include "GroupCollisionSkillNotify.h"

UGroupCollisionSkillNotify::UGroupCollisionSkillNotify()
	: Super()
{
}

#if WITH_EDITOR
void UGroupCollisionSkillNotify::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		OnChangeGroupCollisionSkillNotify.ExecuteIfBound();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UGroupCollisionSkillNotify::ValidateAssociatedAssets()
{
}
#endif