// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotifyState_GsWeaponHide.h"
#include "ActorEx/GsWeaponActor.h"
#include "ActorEx/GsCharacterPlayer.h"

void UAnimNotifyState_GsWeaponHide::Restore()
{
	for (TWeakObjectPtr<AGsWeaponActor> el : _listCacheActor)
	{
		if (el.IsValid())
		{
			el->SetActorHiddenInGame(false);
			el->Reset();
		}
	}
	_listCacheActor.Empty();
}

void UAnimNotifyState_GsWeaponHide::BeginDestroy()
{
	Restore();

	Super::BeginDestroy();
}

void UAnimNotifyState_GsWeaponHide::NotifyBegin(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation, float in_TotalDuration)
{
	Super::NotifyBegin(in_MeshComp, in_Animation, in_TotalDuration);

	AActor* ownerActor = in_MeshComp->GetOwner();
	check(ownerActor);

	_listCacheActor.Empty();

	if (AGsCharacterPlayer* playActor = Cast<AGsCharacterPlayer>(ownerActor))
	{
		if (AGsWeaponActor* weaponActor = playActor->GetWeapon())
		{
			_listCacheActor.Emplace(weaponActor);
		}
		if (AGsWeaponActor* subWeaponActor = playActor->GetWeaponSub())
		{
			_listCacheActor.Emplace(subWeaponActor);
		}
	}

	for (TWeakObjectPtr<AGsWeaponActor> el : _listCacheActor)
	{
		if (el.IsValid())
		{
			el->SetActorHiddenInGame(true);
		}
	}
}

void UAnimNotifyState_GsWeaponHide::NotifyEnd(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation)
{
	Restore();

	Super::NotifyEnd(in_MeshComp, in_Animation);
}