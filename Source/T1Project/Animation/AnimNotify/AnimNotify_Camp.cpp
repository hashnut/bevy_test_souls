// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_Camp.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"
#include "ActorEx/GsCampBase.h"
#include "ActorEx/GsCamp.h"
#include "ActorEx/GsCompleteCamp.h"
#include "Management/ScopeGame/GsGameObjectManager.h"


UAnimNotify_Camp::UAnimNotify_Camp()
{
}

void UAnimNotify_Camp::PostLoad()
{
	Super::PostLoad();

}

#if WITH_EDITOR
void UAnimNotify_Camp::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
}

void UAnimNotify_Camp::ValidateAssociatedAssets()
{
}
#endif

FString UAnimNotify_Camp::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_Camp::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AGsCampBase* campActor = Cast<AGsCampBase>(MeshComp->GetOwner());
	if (nullptr != campActor)
	{
		campActor->FinishedAnim();
	}
}

