// Fill out your copyright notice in the Description page of Project Settings.


#include "GsQuestInstance.h"

#include "Shared/Client/SharedEnums/SharedQuestEnum.h"

#include "GsQuestData.h"

#include "Quest/Table/GsQuestObjectiveTableCache.h"
#include "Quest/GsSchemaObjectiveKillCreatureTypeSet.h"
#include "Quest/GsSchemaObjectiveCraftSet.h"
#include "Quest/GsSchemaObjectiveCompleteQuestTypeSet.h"
#include "Quest/GsSchemaObjectiveInteractNpcSet.h"
#include "Quest/GsSchemaQuestObjectiveAreaPos.h"
#include "Quest/GsSchemaObjectiveKillSet.h"
#include "Quest/GsSchemaObjectiveGotoPosSet.h"
#include "Quest/GsSchemaObjectivePropSet.h"
#include "Quest/GsSchemaObjectiveWarpToInstanceFieldSet.h"
#include "Quest/GsSchemaObjectiveWarpToLastPublicFieldSet.h"
#include "Quest/GsSchemaObjectiveCommonSet.h"
#include "Quest/GsSchemaObjectiveKillSpaceCrackSet.h"
#include "Quest/GsSchemaObjectiveTeleportSet.h"

#include "Management/ScopeGame/GsClientSpawnManager.h"

#include "Map/Spot/GsSchemaSpotInfo.h"



// npc 로 이동
void FGsQuestInstance::SetObjectiveInteractNpcData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType)
{
	if (nullptr == inObjectiveTableCache)
		return;
	
	// npc interact 정보
 	const FGsSchemaObjectiveInteractNpcSet*  interactNpc = inObjectiveTableCache->GetSchemaObjectiveInteractNpcSet();
	if (nullptr == interactNpc)
		return;
		
	FGsObjectiveData newData;
	// 오브젝티브 Id
	newData._objectiveId = inObjectiveTableCache->GetObjectiveId();
	// 오브젝티브 타입 셋팅
	newData._type = QuestObjectiveType::INTERACT_NPC;

	// 목표 위치 산출
	const FGsSchemaNpcData* npcData = interactNpc->npcId.GetRow();
	if (nullptr == npcData)
		return;

	// npcId
	newData._tableIdList.Emplace(npcData->id);

	const FGsSchemaQuestObjective* schemaQuestObjective = inObjectiveTableCache->GetSchemaQuestObjective();
	if (nullptr == schemaQuestObjective)
		return;	
	
	if (FGsClientSpawnManager* spawnManager = GSClientSpawn())
	{
		for (FGsSchemaQuestObjectiveAreaPos iter : schemaQuestObjective->posList)
		{
			int32 mapId = iter.mapId;
			int32 spotId = iter.spotId;
			FVector	goalPos, frontPos;
			float radius;
			FGsSchemaSpotInfo frontSpotData;
			if (true == spawnManager->TryGetSpot(mapId, spotId, frontSpotData))
			{
				// 목표위치
				frontPos = frontSpotData.pos;
				radius = npcData->interactionRange;
				// 도착위치
				goalPos = FVector::ZeroVector;
			}

			newData._goalDataList.Emplace(mapId, spotId, goalPos, frontPos, radius);
			break;
		}
	}
	
	_objectiveDataList.Emplace(newData);
}

// 몬스터 처치
void FGsQuestInstance::SetObjectiveKillData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType)
{	
	if (nullptr == inObjectiveTableCache)
		return;
	
	// kill정보
	const FGsSchemaObjectiveKillSet*  killset = inObjectiveTableCache->GetSchemaObjectiveKillSet();
	if (nullptr == killset)
		return;
	
	FGsObjectiveData newData;
	// 오브젝티브 Id
	newData._objectiveId = inObjectiveTableCache->GetObjectiveId();
	// 오브젝티브 타입 셋팅
	newData._type = QuestObjectiveType::KILL;

	// 처치 몬스터 리스트
	for (int32 monsterId : killset->idList)
	{
		newData._tableIdList.Emplace(monsterId);
	}

	if (0 >= killset->idList.Num())
	{
		_isAutoMoveAble = false;
	}

	// 목표 위치 산출
	const FGsSchemaQuestObjective* schemaQuestObjective = inObjectiveTableCache->GetSchemaQuestObjective();
	if (nullptr == schemaQuestObjective)
		return;

	if (FGsClientSpawnManager* spawnManager = GSClientSpawn())
	{		
		for (FGsSchemaQuestObjectiveAreaPos iter : schemaQuestObjective->posList)
		{
			int32 mapId = iter.mapId;
			int32 spotId = iter.spotId;
			FVector	goalPos, frontPos;
			float radius;
			FGsSchemaSpotInfo frontSpotData;
			if (true == spawnManager->TryGetSpot(mapId, spotId, frontSpotData))
			{
				// 목표위치
				frontPos = frontSpotData.pos;
				radius = frontSpotData.radius;
				// 도착위치
				goalPos = frontSpotData.pos;
			}

			newData._goalDataList.Emplace(mapId, spotId, goalPos, frontPos, radius);
		}
	}

	_objectiveDataList.Emplace(newData);
}

// GotoPos
void FGsQuestInstance::SetObjectiveGotoPosData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType)
{	
	if (nullptr == inObjectiveTableCache)
		return;

	// GotoPos 데이터
	const FGsSchemaObjectiveGotoPosSet*  gotoPos = inObjectiveTableCache->GetSchemaObjectiveGotoPosSet();
	if (nullptr == gotoPos)
		return;	
	
	FGsObjectiveData newData;
	// 오브젝티브 Id
	newData._objectiveId = inObjectiveTableCache->GetObjectiveId();
	// 오브젝티브 타입 셋팅
	newData._type = QuestObjectiveType::GOTO_POS;

	// 목표 위치 산출
	const FGsSchemaQuestObjective* schemaQuestObjective = inObjectiveTableCache->GetSchemaQuestObjective();
	if (nullptr == schemaQuestObjective)
		return;

	if (FGsClientSpawnManager* spawnManager = GSClientSpawn())
	{
		for (FGsSchemaQuestObjectiveAreaPos iter : schemaQuestObjective->posList)
		{
			int32 mapId = iter.mapId;
			int32 spotId = iter.spotId;
			FVector	goalPos, frontPos;

			FGsSchemaSpotInfo goalSpotData;
			FGsSchemaSpotInfo frontSpotData;
			if (true == spawnManager->TryGetSpot(mapId, gotoPos->spotId, goalSpotData))
			{
				// 최종위치
				goalPos = goalSpotData.pos;
			}
			if (true == spawnManager->TryGetSpot(mapId, spotId, frontSpotData))
			{
				// 도착위치
				frontPos = frontSpotData.pos;
			}
			newData._goalDataList.Emplace(mapId, spotId, goalPos, frontPos, gotoPos->range);
			break;
		}
	}

	_objectiveDataList.Emplace(newData);	
}

// 프랍 채집
void FGsQuestInstance::SetObjectivePropData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType)
{
	if (nullptr == inObjectiveTableCache)
		return;

	// Prop정보
	const FGsSchemaObjectivePropSet*  propSet = inObjectiveTableCache->GetSchemaObjectivePropSet();
	if (nullptr == propSet)
		return;

	FGsObjectiveData newData;
	// 오브젝티브 Id
	newData._objectiveId = inObjectiveTableCache->GetObjectiveId();
	// 오브젝티브 타입 셋팅
	newData._type = QuestObjectiveType::INTERACT_PROP_TOUCH;

	// 목표 위치 산출
	const FGsSchemaPropData* propData = propSet->propId.GetRow();
	if (nullptr == propData)
		return;

	// propId
	newData._tableIdList.Emplace(propData->id);	

	// 목표 위치 산출
	const FGsSchemaQuestObjective* schemaQuestObjective = inObjectiveTableCache->GetSchemaQuestObjective();
	if (nullptr == schemaQuestObjective)
		return;

	if (FGsClientSpawnManager* spawnManager = GSClientSpawn())
	{
		for (FGsSchemaQuestObjectiveAreaPos iter : schemaQuestObjective->posList)
		{
			int32 mapId = iter.mapId;
			int32 spotId = iter.spotId;
			FVector	goalPos, frontPos;
			FGsSchemaSpotInfo frontSpotData;
			if (true == spawnManager->TryGetSpot(mapId, spotId, frontSpotData))
			{
				// 목표위치
				frontPos = frontSpotData.pos;
				// 도착위치
				goalPos = frontSpotData.pos;
			}
			newData._goalDataList.Emplace(mapId, spotId, goalPos, frontPos, propData->touchInteractionRange);
			break;
		}
	}
	_objectiveDataList.Emplace(newData);
}

void FGsQuestInstance::SetObjectiveWarpToInstanceFieldData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType)
{
	if (nullptr == inObjectiveTableCache)
		return;

	if (1 < _objectiveDataList.Num())
		return;

	// WarpToInstanceField 데이터
	const FGsSchemaObjectiveWarpToInstanceFieldSet*  warpToInstanceField = inObjectiveTableCache->GetSchemaObjectiveWarpToInstanceFieldSet();
	if (nullptr == warpToInstanceField)
		return;

	FGsObjectiveData newData;
	// 오브젝티브 Id
	newData._objectiveId = inObjectiveTableCache->GetObjectiveId();
	// 오브젝티브 타입 셋팅
	newData._type = QuestObjectiveType::WARP_TO_INSTANCE_FIELD;

	FGsSchemaQuestObjectiveAreaPos pos = warpToInstanceField->pos;
	newData._goalDataList.Emplace(pos.mapId);

	_objectiveDataList.Emplace(newData);
}

void FGsQuestInstance::SetObjectiveWarpToLastPublicFieldData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType)
{
	if (nullptr == inObjectiveTableCache)
		return;

	if (1 < _objectiveDataList.Num())
		return;

	// WarpToInstanceField 데이터
	const FGsSchemaObjectiveWarpToLastPublicFieldSet*  warpToLastPublicField = inObjectiveTableCache->GetSchemaObjectiveWarpToLastPublicFieldSet();
	if (nullptr == warpToLastPublicField)
		return;

	FGsObjectiveData newData;
	// 오브젝티브 Id
	newData._objectiveId = inObjectiveTableCache->GetObjectiveId();
	// 오브젝티브 타입 셋팅
	newData._type = QuestObjectiveType::WARP_TO_LAST_PUBLIC_FIELD;

	FGsSchemaQuestObjectiveAreaPos pos = warpToLastPublicField->pos;
	newData._goalDataList.Emplace(pos.mapId);

	_objectiveDataList.Emplace(newData);
}

// 몬스터 타입별 데이터 셋팅
void FGsQuestInstance::SetObjectiveKillCreatureTypeData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType)
{
	if (nullptr == inObjectiveTableCache)
		return;

	// killCreatureType 정보
	const FGsSchemaObjectiveKillCreatureTypeSet* killCreatureTypeSet = inObjectiveTableCache->GetSchemaObjectiveKillCreatureTypeSet();
	if (nullptr == killCreatureTypeSet)
		return;

	FGsObjectiveData newData;
	// 오브젝티브 Id
	newData._objectiveId = inObjectiveTableCache->GetObjectiveId();
	// 오브젝티브 타입 셋팅
	newData._type = QuestObjectiveType::KILL_CREATURE_TYPE;

	// 목표 위치 산출
	const FGsSchemaQuestObjective* schemaQuestObjective = inObjectiveTableCache->GetSchemaQuestObjective();
	if (nullptr == schemaQuestObjective)
		return;

	if (FGsClientSpawnManager* spawnManager = GSClientSpawn())
	{
		for (FGsSchemaQuestObjectiveAreaPos iter : schemaQuestObjective->posList)
		{
			int32 mapId = iter.mapId;
			int32 spotId = iter.spotId;
			FVector	goalPos, frontPos;
			float radius;
			FGsSchemaSpotInfo frontSpotData;
			if (true == spawnManager->TryGetSpot(mapId, spotId, frontSpotData))
			{
				// 목표위치
				frontPos = frontSpotData.pos;
				radius = frontSpotData.radius;
				// 도착위치
				goalPos = frontSpotData.pos;
			}

			newData._goalDataList.Emplace(mapId, spotId, goalPos, frontPos, radius);
		}
	}

	_objectiveDataList.Emplace(newData);
}

// 제작 데이터 셋팅
void FGsQuestInstance::SetObjectiveCraftData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType)
{
	if (nullptr == inObjectiveTableCache)
		return;

	// Craft 정보
	const FGsSchemaObjectiveCraftSet* CraftSet = inObjectiveTableCache->GetSchemaObjectiveCraftSet();
	if (nullptr == CraftSet)
		return;

	FGsObjectiveData newData;
	// 오브젝티브 Id
	newData._objectiveId = inObjectiveTableCache->GetObjectiveId();
	// 오브젝티브 타입 셋팅
	newData._type = QuestObjectiveType::ITEM_CRAFT;

	_objectiveDataList.Emplace(newData);
}

// 퀘스트 타입별 완료 체크 데이터 셋팅
void FGsQuestInstance::SetObjectiveCompleteQuestTypeData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType)
{
	if (nullptr == inObjectiveTableCache)
		return;

	// CompleteQuest 정보
	const FGsSchemaObjectiveCompleteQuestTypeSet* completeQuestTypeSet = inObjectiveTableCache->GetObjectiveCompleteQuestTypeSet();
	if (nullptr == completeQuestTypeSet)
		return;

	FGsObjectiveData newData;
	// 오브젝티브 Id
	newData._objectiveId = inObjectiveTableCache->GetObjectiveId();
	// 오브젝티브 타입 셋팅
	newData._type = QuestObjectiveType::COMPLETE_QUEST_TYPE;

	_objectiveDataList.Emplace(newData);
}

// 스토리 타입별 완료 체크 데이터 셋팅
void FGsQuestInstance::SetObjectiveStoryQuestTypeData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType)
{
	if (nullptr == inObjectiveTableCache)
		return;

	// CompleteQuest 정보
	const FGsSchemaObjectiveCompleteQuestTypeSet* completeQuestTypeSet = inObjectiveTableCache->GetObjectiveCompleteQuestTypeSet();
	if (nullptr == completeQuestTypeSet)
		return;

	FGsObjectiveData newData;
	// 오브젝티브 Id
	newData._objectiveId = inObjectiveTableCache->GetObjectiveId();
	// 오브젝티브 타입 셋팅
	newData._type = QuestObjectiveType::COMPLETE_STORY_TYPE;

	_objectiveDataList.Emplace(newData);
}


void FGsQuestInstance::SetObjectiveKillSpaceCrackData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType)
{
	if (nullptr == inObjectiveTableCache)
		return;

	// killSpaceCrackSet 정보
	const FGsSchemaObjectiveKillSpaceCrackSet* killSpaceCrackSet = inObjectiveTableCache->GetObjectiveKillSpaceCrackSet();
	if (nullptr == killSpaceCrackSet)
		return;

	FGsObjectiveData newData;
	// 오브젝티브 Id
	newData._objectiveId = inObjectiveTableCache->GetObjectiveId();
	// 오브젝티브 타입 셋팅
	newData._type = QuestObjectiveType::KILL_SPACE_CRACK;

	_objectiveDataList.Emplace(newData);
}

void FGsQuestInstance::SetObjectiveTypeTeleportData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType)
{
	if (nullptr == inObjectiveTableCache)
		return;

	// Teleport 데이터
	const FGsSchemaObjectiveTeleportSet* teleportSet = inObjectiveTableCache->GetObjectiveTeleportSet();
	if (nullptr == teleportSet)
		return;

	FGsObjectiveData newData;
	// 오브젝티브 Id
	newData._objectiveId = inObjectiveTableCache->GetObjectiveId();
	// 오브젝티브 타입 셋팅
	newData._type = QuestObjectiveType::TELEPORT;
	
	_objectiveDataList.Emplace(newData);
}

void FGsQuestInstance::SetObjectiveCommonData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType)
{
	if (nullptr == inObjectiveTableCache)
		return;

	// Guide 정보
	const FGsSchemaObjectiveCommonSet* commonSet = inObjectiveTableCache->GetObjectiveCommonSet();
	if (nullptr == commonSet)
		return;

	FGsObjectiveData newData;
	// 오브젝티브 Id
	newData._objectiveId = inObjectiveTableCache->GetObjectiveId();
	// 오브젝티브 타입 셋팅
	newData._type = inObjectiveType;

	_objectiveDataList.Emplace(newData);
}