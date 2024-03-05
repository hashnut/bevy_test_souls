// Fill out your copyright notice in the Description page of Project Settings.


#include "GsDropObjectManager.h"
#include "Data/GsDataContainManager.h"
#include "T1Project.h"
#include "GsGameObjectManager.h"
#include "GameObject/DropObject/GsSchemaDropObject.h"
#include "UTIL/GsItemUtil.h"
#include "GameObject/ObjectClass/GsGameObjectDropObject.h"
#include "ActorEx/GsActorDropObject.h"
#include "UTIL/GsTableUtil.h"
#include "GameObject/GsSchemaShapeData.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedSpawnEnum.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "UTIL/GsLevelUtil.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Define/GsGameObjectStruct.h"
#include "GameObject/Define/EGsDropItemCategory.h"
#include "DrawDebugHelpers.h"

void FGsDropObjectManager::SpawnDropObject(PD::SC::PKT_SC_SPAWN_DROP_OBJECT_READ* inPkt)
{	
	int64 gameId = inPkt->GameId();
	int32 itemId = inPkt->ItemId();
	ItemAmount itemCount = inPkt->ItemAmount();
	SPAWN_REASON reason = inPkt->Reason();
	FVector serverDestPos = inPkt->DestPos();
	FVector serverOriginPos = inPkt->OriginPos();
	//Player 소유의 드랍 아이템인가?
	bool isOwner = IsPlayerCanLoot(inPkt);
	//파티가 드랍 아이템의 소유권이 있을 때
	TArray<UserDBId> userDBIdArray = GetUserDBIdArray(inPkt);	
	//타유저가 드랍 아이템의 첫 소유권이 있었지만 소유권이 만료됐을 때 
	bool isExpired = inPkt->IsExpiredOwnership();	
	// 2022/08/02 PKT - Drop되는 아이템 중 Drop 형식에 따른 이펙트를 표현해야 할 때가 있음.(ex.정예던전)
	DropItemEffect dropItemEffect = inPkt->DropEffectType();
	UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == player || nullptr == player->GetActor())
	{
		return;
	}

	UWorld* world = player->GetActor()->GetWorld();
	UGsGameObjectBase* gameObject = nullptr;
	UGsGameObjectDropObject* dropObject = nullptr;	
	FGsSpawnParamDropObject param;
	if (SPAWN_REASON::NORMAL == reason)
	{
		FVector spawnPos = serverDestPos;
		FHitResult hit;		
		if (UGsLevelUtil::TryLineTraceToLand(hit, world, serverDestPos, serverDestPos.Z + 100.0f))
		{
			spawnPos = hit.Location;
#if WITH_EDITOR && DROP_OBJECT_DEBUG			
			FString hitActorLog = TEXT("hit actor name : ") + hit.GetActor()->GetActorLabel();

			FVector start = FVector(serverDestPos.X, serverDestPos.Y, serverDestPos.Z + 100.0f);
			DrawDebugLine(world, start, spawnPos, FColor::Yellow, false, 180.0f);
			DrawDebugSphere(world, spawnPos, 100, 8, FColor::Yellow, false, 180.0f);
			DrawDebugString(world
				, spawnPos
				, *hitActorLog
				, nullptr
				, FColor::Yellow
				, 180.0f);

			GSLOG(Log, TEXT("FGsDropObjectManager::SpawnDropObject - SPAWN_REASON::NORMAL - location : %s"), *spawnPos.ToString());
#endif
		}
		else
		{
#if WITH_EDITOR && DROP_OBJECT_DEBUG		
			DrawDebugSphere(world, spawnPos, 100, 8, FColor::Red, false, 180.0f);
#endif
			GSLOG(Error, TEXT("FGsDropObjectManager::SpawnDropObject - SPAWN_REASON::NORMAL - fail - location : %s"), *serverDestPos.ToString());
		}

		param._gameId = gameId;
		param._pos = spawnPos;
		param._rot = FRotator::ZeroRotator;
		param._tId = itemId;
		param._type = EGsGameObjectType::DropObject;

		gameObject = GSGameObject()->SpawnObject(&param);
		if (gameObject)
		{
			dropObject = Cast<UGsGameObjectDropObject>(gameObject);
			if (dropObject)
			{
				dropObject->SetPlayerOwn(isOwner);
				dropObject->SetItemAmount(itemCount);
				dropObject->SetInitOwnerDBIdArray(userDBIdArray);
				dropObject->SetExpiredOwnerShip(isExpired);

				dropObject->StartSpawn(spawnPos, serverOriginPos);				

#if DROP_OBJECT_DEBUG								
				GSLOG(Log, TEXT("FGsDropObjectManager::SpawnDropObject - ShowDropObject - game id  : %lld\towner : %s\texpire : %s")
					, inPkt->GameId()
					, (isOwner ? TEXT("true") : TEXT("false"))
					, (isExpired ? TEXT("true") : TEXT("false")));
#endif
			}			
		}
	}
	//Grid spawn
	else if (SPAWN_REASON::GRID == reason)
	{
		FVector spawnPos = serverDestPos;
		FHitResult hit;		
		if (UGsLevelUtil::TryLineTraceToLand(hit, world, spawnPos, spawnPos.Z + 100.0f))
		{
			spawnPos = hit.Location;
#if WITH_EDITOR && DROP_OBJECT_DEBUG				
			FString hitActorLog = TEXT("hit actor name : ") + hit.GetActor()->GetActorLabel();

			FVector start = FVector(spawnPos.X, spawnPos.Y, spawnPos.Z + 100.0f);
			FVector dest = hit.Location;
			DrawDebugLine(world, start, dest, FColor::Blue, false, 180.0f);
			DrawDebugSphere(world, dest, 100, 8, FColor::Blue, false, 180.0f);		
			DrawDebugString(world
				, dest
				, *hitActorLog
				, nullptr
				, FColor::Blue
				, 180.0f);

			GSLOG(Log, TEXT("FGsDropObjectManager::SpawnDropObject - SPAWN_REASON::GRID - game id : %lld\tlocation : %s"), inPkt->GameId(), *dest.ToString());
#endif
		}
		else
		{
#if WITH_EDITOR && DROP_OBJECT_DEBUG		
			DrawDebugSphere(world, spawnPos, 100, 8, FColor::Red, false, 180.0f);
#endif
			GSLOG(Error, TEXT("FGsDropObjectManager::SpawnDropObject - SPAWN_REASON::GRID - fail - location : %s"), *spawnPos.ToString());
		}

		param._gameId = gameId;
		param._pos = spawnPos;
		param._rot = FRotator::ZeroRotator;
		param._tId = itemId;
		param._type = EGsGameObjectType::DropObject;

		gameObject = GSGameObject()->SpawnObject(&param);
		if (gameObject)
		{
			dropObject = Cast<UGsGameObjectDropObject>(gameObject);
			if (dropObject)
			{
				//그리드 스폰시 아이템 떨어진 연출 끝난 걸로 설정
				dropObject->SetSpawnFinish(true);
				dropObject->SetPlayerOwn(isOwner);				
				// https://jira.com2us.com/jira/browse/C2URWQ-2905
				dropObject->SetItemAmount(itemCount);
				dropObject->SetInitOwnerDBIdArray(userDBIdArray);
				dropObject->SetExpiredOwnerShip(isExpired);

				dropObject->PlayDropItemEffect(dropItemEffect);

				ShowDropObject(dropObject);			

#if DROP_OBJECT_DEBUG								
				GSLOG(Log, TEXT("FGsDropObjectManager::SpawnDropObject - ShowDropObject - game id  : %lld\towner : %s\texpire : %s")
					, inPkt->GameId()
					, (isOwner ? TEXT("true") : TEXT("false"))
					, (isExpired ? TEXT("true") : TEXT("false")));
#endif
			}
		}
	}
}

void FGsDropObjectManager::ShowDropObject(const UGsGameObjectDropObject* inDropObject)
{
	if (inDropObject->GetActor())
	{
		if (const AGsActorDropObject* dropObjectActor = Cast<AGsActorDropObject>(inDropObject->GetActor()))
		{
			dropObjectActor->Active(true);
		}
	}
}

bool FGsDropObjectManager::TryGetDropObjectShapeId(int32& outShapeId, const int32 inItemId)
{
	EGsDropItemCategory itemCategory = UGsItemUtil::GetDropObjectCategory(inItemId);
	if (itemCategory == EGsDropItemCategory::Max)
	{
		GSLOG(Error, TEXT("Missing item category - item id : %d"), inItemId);
		return false;
	}

	const FGsSchemaDropObject* dropObject = UGsTableUtil::FindRowById<UGsTableDropObject, FGsSchemaDropObject>(itemCategory);		
	if (nullptr == dropObject->shapeId.GetRow())
	{
		GSLOG(Error, TEXT("Missing drop object shape - item id : %d"), inItemId);
		return false;
	}

	outShapeId = dropObject->shapeId.GetRow()->id;
	return true;
}

UClass* FGsDropObjectManager::GetDropObjectClass(const int32 inShapeId)
{
	if (const FGsSchemaShapeData * shape = UGsTableUtil::FindRowById<UGsTableShapeData, FGsSchemaShapeData>(inShapeId))
	{
		FString path = FString::Printf(TEXT("%s_C"), *shape->bPClass.ToString());

		return LoadObject<UClass>(nullptr, *path);
	}

	return nullptr;
}

void FGsDropObjectManager::DespawnDropObject(PD::SC::PKT_SC_DESPAWN_DROP_OBJECT_READ* inPkt)
{
	if (UGsGameObjectBase* find = GSGameObject()->FindObject(EGsGameObjectType::DropObject, inPkt->GameId()))
	{
		GSGameObject()->DespawnObject(find);

#if DROP_OBJECT_DEBUG
		GSLOG(Log, TEXT("FGsDropObjectManager::DespawnDropObject - game id  : %lld"), inPkt->GameId());
#endif
	}
}

bool FGsDropObjectManager::IsPlayerCanLoot(PD::SC::PKT_SC_SPAWN_DROP_OBJECT_READ* inPkt)
{
	if (0 == inPkt->GetUserDBIdListCount())
	{
		return true;
	}

	int32 num = inPkt->GetUserDBIdListCount();
	UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == player)
	{
		return false;
	}

	UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(player);
	if (nullptr == localPlayer)
	{
		return false;
	}

	FGsGameObjectDataLocalPlayer* localData = localPlayer->GetCastData<FGsGameObjectDataLocalPlayer>();
	if (nullptr == localData)
	{
		return false;
	}

	uint64 playerDBId = localData->GetUserDBID();
	uint64 userDBId;

	for (int32 i = 0; i < num; ++i)
	{
		using userDBIdListIterator = PktIterator<PD::SC::PKT_SC_SPAWN_DROP_OBJECT_READ::UserDBIdList>;
		for (userDBIdListIterator it = inPkt->GetFirstUserDBIdListIterator();
			it != inPkt->GetLastUserDBIdListIterator(); ++it)
		{
			userDBId = it->UserDBId();

			if (userDBId == playerDBId)
			{
				return true;
			}
		}
	}

	return false;
}

const TArray<UserDBId> FGsDropObjectManager::GetUserDBIdArray(PD::SC::PKT_SC_SPAWN_DROP_OBJECT_READ* inPkt)
{
	TArray<UserDBId> userDBIDList;
	using userDBIdListIterator = PktIterator<PD::SC::PKT_SC_SPAWN_DROP_OBJECT_READ::UserDBIdList>;
	for (userDBIdListIterator it = inPkt->GetFirstUserDBIdListIterator();
		it != inPkt->GetLastUserDBIdListIterator(); ++it)
	{
		userDBIDList.Add(it->UserDBId());
	}

	return userDBIDList;
}

void FGsDropObjectManager::OwnershipExpired(PD::SC::PKT_SC_DROP_OBJECT_OWNERSHIP_EXPIRED_READ* inPkt)
{
	if (UGsGameObjectBase* find = GSGameObject()->FindObject(EGsGameObjectType::DropObject, inPkt->GameId()))
	{
		if (UGsGameObjectDropObject* dropObject = Cast<UGsGameObjectDropObject>(find))
		{		
			dropObject->SetExpiredOwnerShip(true);

#if DROP_OBJECT_DEBUG
			GSLOG(Log, TEXT("FGsDropObjectManager::OwnershipExpired - game id  : %lld"), inPkt->GameId());
#endif
		}
	}
}
