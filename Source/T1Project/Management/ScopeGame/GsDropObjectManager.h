// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsManager.h"
#include "UTIL/GsTimeUtil.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

/**
 * 
 */
 class UGsGameObjectBase;
 class UGsGameObjectLocalPlayer;
 class UGsGameObjectDropObject;

class T1PROJECT_API FGsDropObjectManager : public IGsManager
{
public:
	void SpawnDropObject(PD::SC::PKT_SC_SPAWN_DROP_OBJECT_READ* inPkt);	
	void DespawnDropObject(PD::SC::PKT_SC_DESPAWN_DROP_OBJECT_READ* inPkt);	
	void OwnershipExpired(PD::SC::PKT_SC_DROP_OBJECT_OWNERSHIP_EXPIRED_READ* inPkt);

private:	
	void ShowDropObject(const UGsGameObjectDropObject* inDropObject);
	bool TryGetDropObjectShapeId(int32& outShapeId, const int32 inItemId);
	UClass* GetDropObjectClass(const int32 inShapeId);	
	bool IsPlayerCanLoot(PD::SC::PKT_SC_SPAWN_DROP_OBJECT_READ* inPkt);	
	const TArray<UserDBId> GetUserDBIdArray(PD::SC::PKT_SC_SPAWN_DROP_OBJECT_READ* inPkt);
	
	
#define GSDropObject() UGsScopeHolder::GetGameManagerFType<FGsDropObjectManager>(UGsScopeGame::EManagerType::DropObject)
};
