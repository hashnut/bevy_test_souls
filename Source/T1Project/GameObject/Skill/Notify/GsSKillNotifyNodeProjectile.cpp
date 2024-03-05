// Fill out your copyright notice in the Description page of Project Settings.

#include "GsSKillNotifyNodeProjectile.h"
#include "Engine/World.h"
#include "GSGameInstance.h"
#include "GameObject/ObjectClass/GsGameObjectProjectile.h"
#include "Data/GsDataContainManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"


void FGsSKillNotifyNodeProjectile::Action(UGsGameObjectBase* Owner)
{
	Super::Action(Owner);

	//UGsSkillNotifyDataProjectile* notifyData = Cast<UGsSkillNotifyDataProjectile>(CurrNotifyData);
	//check(notifyData);

	//FVector pos = Owner->GetRotation().Vector() * notifyData->Distance + 
	//	(Owner->GetLocation() + FVector(0.f,0.f, notifyData->OffsetHeight));
	//FRotator rot = Owner->GetRotation() + FRotator::MakeFromEuler(FVector(0.f, 0.f, notifyData->Angle));
	//if (UGsGameObjectBase* obj = GSpawner()->SpawnProjectile(notifyData->ProjectileID, pos, rot))
	//{
	//	obj->GetActor()->SetActorScale3D(FVector(notifyData->Scale));
	//}
}