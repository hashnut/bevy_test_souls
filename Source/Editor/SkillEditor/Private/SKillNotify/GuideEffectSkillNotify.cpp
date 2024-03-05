// Fill out your copyright notice in the Description page of Project Settings.


#include "GuideEffectSkillNotify.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"

UGuideEffectSkillNotify::UGuideEffectSkillNotify()
	: Super()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(192, 255, 99, 255);
#endif // WITH_EDITORONLY_DATA	
}

#if WITH_EDITOR
void UGuideEffectSkillNotify::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		OnChangeSkillNotify.ExecuteIfBound();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void UGuideEffectSkillNotify::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{

}