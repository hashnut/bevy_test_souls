// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryGroundActionUtility.h"

//UE4
#include "Editor/UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "Editor/UnrealEd/Public/UnrealEdGlobals.h"
#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#include "Editor/UnrealEd/Public/Editor.h"
#include "Runtime/Engine/Classes/Engine/Selection.h"
#include "Runtime/Core/Public/Misc/MessageDialog.h"
#include "Runtime/Engine/Classes/Engine/Selection.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Editor/Blutility/Public/EditorUtilityLibrary.h"

//T1 Project
#include "UTIL/GsText.h"
#include "Level/Territory/GsTerritoryZoneDefine.h"
#include "Util/GsTerritoryEditorUtil.h"
#include "Define/GsTerritorySpawnDefine.h"
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "GsTerritoryEditorSetting.h"

//territory editor
#include "TerritoryEditor.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "GsTerritoryUniqueIDManager.h"
#include "Actor/Spot/GsTerritoryWarpSpot.h"
#include "Actor/Unique/GsTerrEdRTTActor.h"
#include "Actor/Line/GsTerritoryLinePatrol.h"

const FString ONLY_SEVER_LEVEL_OK = TEXT("서버 전용 레벨에서만 Zone을 생성할 수 있습니다. 서버 폴더 아래에 레벨을 배치하시거나 서버 폴더 아래의 레벨을 선택해 주세요.");
const FString SELECT_CORRECT = TEXT("하나의 터레인만 선택해 주세요.");
const FString UNIQUE_ID_GEN_FAIL = TEXT("Zone Unique id 생성 실패했습니다. 유니크 테이블 셋팅 또는 TerritoryEditor 셋팅을 확인해 주세요");

const FString CREATE_FAIL_CIRCLE_ZONE = TEXT("Circle Zone 생성 실패");
const FString CREATE_FAIL_POLYGON_ZONE = TEXT("Polygon Zone 생성 실패");
const FString CREATE_FAIL_ZONE = TEXT("Zone 생성 실패");

void UGsTerritoryGroundActionUtility::AddZone(NpcSpawnGroupType inSpawnGroupType, bool inByClient, bool inByDefault, EGsTerritoryZoneShapeType inShape)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	if (nullptr == world->GetCurrentLevel())
	{
		return;
	}	

	const FString& path = world->GetCurrentLevel()->GetPathName();
	if (false == UGsTerritoryLevelUtil::IsServerLevel(path))
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *ONLY_SEVER_LEVEL_OK);
		return;
	}

	if (false == IsCorrectSelect())
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *SELECT_CORRECT);
		return;
	}

	FTransform trs;
	trs.SetLocation(GEditor->ClickLocation);
	AGsTerritoryZone* zone = FGsTerritoryLevelManager::CreateZone(inSpawnGroupType, inByClient, inByDefault, inShape, trs);
}

void UGsTerritoryGroundActionUtility::AddSpotNormal(FText inName, float inRandomSpawnRange /*= 100*/)
{
	FTransform trs;
	trs.SetLocation(GEditor->ClickLocation);
	FGsTerritoryLevelManager::CreateNormalSpot(inName, inRandomSpawnRange, trs);	
}

void UGsTerritoryGroundActionUtility::AddSpotSpawn(FText inName, IffTeamType inTeamType, float inRandomSpawnRange)
{
	FTransform trs;
	trs.SetLocation(GEditor->ClickLocation);
	FGsTerritoryLevelManager::CreateSpawnSpot(inName, inTeamType, inRandomSpawnRange, trs);
}

void UGsTerritoryGroundActionUtility::AddSpotResurrection(FText inName, IffTeamType inTeamType, float inRandomSpawnRange)
{
	FTransform trs;
	trs.SetLocation(GEditor->ClickLocation);
	FGsTerritoryLevelManager::CreateResurrectionSpot(inName, inTeamType, inRandomSpawnRange, trs);
}

void UGsTerritoryGroundActionUtility::AddSpotTeleport(FText inName, IffTeamType inTeamType /*= IffTeamType::NEUTRAL*/, float inRandomSpawnRange /*= 100*/)
{
	FTransform trs;
	trs.SetLocation(GEditor->ClickLocation);
	FGsTerritoryLevelManager::CreateTeleportSpot(inName, inTeamType, inRandomSpawnRange, trs);
}

void UGsTerritoryGroundActionUtility::AddLinePatrolLine(float inRot, float inDist, bool inReturn, bool inRepeat, float inReatDelay, bool inIsSuicide)
{
	AGsTerritoryLinePatrol* patrol = CreatePatrolLine(inReturn, inRepeat, inReatDelay, inIsSuicide);
	if (nullptr == patrol)
	{
		return;
	}

	patrol->ChangeLineShape(inRot, inDist);
}

void UGsTerritoryGroundActionUtility::AddLinePatrolCircle(float inRadius, bool inReturn, bool inRepeat, float inReatDelay, bool inIsSuicide)
{
	AGsTerritoryLinePatrol* patrol = CreatePatrolLine(inReturn, inRepeat, inReatDelay, inIsSuicide);
	if (nullptr == patrol)
	{
		return;
	}

	patrol->ChangeCircleShape(inRadius);
}

void UGsTerritoryGroundActionUtility::AddLinePatrolPolygon(bool inReturn, bool inRepeat, float inReatDelay, bool inIsSuicide)
{
	AGsTerritoryLinePatrol* patrol = CreatePatrolLine(inReturn, inRepeat, inReatDelay, inIsSuicide);
	if (nullptr == patrol)
	{
		return;
	}

	patrol->ChangePolygonShape();
}

AGsTerritoryLinePatrol* UGsTerritoryGroundActionUtility::CreatePatrolLine(bool inReturn, bool inRepeat, float inReatDelay, bool inIsSuicide)
{
	FTransform trs;
	trs.SetLocation(GEditor->ClickLocation);
	AGsTerritoryLine* line = FGsTerritoryLevelManager::CreatePatrolLine(trs);
	if (nullptr == line)
	{
		return nullptr;
	}

	AGsTerritoryLinePatrol* patrol = Cast<AGsTerritoryLinePatrol>(line);
	if (nullptr == patrol)
	{
		return nullptr;
	}

	patrol->_isReturn = inReturn;
	patrol->_isRepeat = inRepeat;
	patrol->_repeatDelay = inReatDelay;
	patrol->_isSuicide = inIsSuicide;

	return patrol;
}

//void UGsTerritoryGroundActionUtility::AddSpaceCrackPortalSpot(FText inName, float inRandomSpawnRange /*= 100*/)
//{
//	FTransform trs;
//	trs.SetLocation(GEditor->ClickLocation);
//	FGsTerritoryLevelManager::CreateSpaceCrackPortalSpot(inName, inRandomSpawnRange, trs);
//}

void UGsTerritoryGroundActionUtility::AddSpotWarp(FGsTerritoryMapWarp inInput, WarpType inWarpType, float inRadius, int32 inShapeID, int32 inGroupId)
{
	FTransform trs;
	trs.SetLocation(GEditor->ClickLocation);
	FGsTerritoryLevelManager::CreateWarp(inInput, inWarpType, inRadius, inShapeID, trs, inGroupId);
}

void UGsTerritoryGroundActionUtility::EditRTT(FGsTerritoryMapSpot inSpot)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	ULevel* level = world->GetCurrentLevel();
	if (nullptr == level)
	{
		return;
	}

	FString path = level->GetPathName();
	if (false == UGsTerritoryLevelUtil::IsServerLevel(path))
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *ONLY_SEVER_LEVEL_OK);
		return;
	}
	
	/*if (false == IsCorrectSelect())
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *SELECT_CORRECT);
		return;
	}*/

	//RTT actor가 존재하는가?
	AActor** find = nullptr;
	AGsTerrEdRTTActor* rtt = nullptr;

	find = level->Actors.FindByPredicate([](const AActor* actor)->bool{
		if (actor)
		{
			return actor->IsA(AGsTerrEdRTTActor::StaticClass());
		}
		
		return false;
	});

	if (nullptr == find)
	{
		//RTT actor가 존재하지 않는다면 생성		
		rtt = CreateRTT();
	}
	else
	{
		rtt = Cast<AGsTerrEdRTTActor>(*find);
	}

	if (rtt)
	{
		rtt->_destMapId = inSpot._mapId;
		rtt->_destSpotId = inSpot._spotId;
		rtt->Modify();
	}
}

AGsTerrEdRTTActor* UGsTerritoryGroundActionUtility::CreateRTT()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return nullptr;
	}

	ULevel* level = world->GetCurrentLevel();
	if (nullptr == level)
	{
		return nullptr;
	}

	//RTT actor가 존재하지 않는다면 생성
	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	FTransform trs;
	AActor* actor = GEditor->AddActor(level, settings->_rttClass, trs);
	return Cast<AGsTerrEdRTTActor>(actor);
}

UClass* UGsTerritoryGroundActionUtility::GetZoneTemplate(EGsTerritoryZoneShapeType inShape)
{
	UClass* zoneClass = nullptr;

	if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
	{
		switch (inShape)
		{
		case EGsTerritoryZoneShapeType::Circle:
		{
			if (settings->_circleZoneClass)
			{
				zoneClass = settings->_circleZoneClass.Get();
			}

			break;
		}
		case EGsTerritoryZoneShapeType::Line:
		{
			if (settings->_lineZoneClass)
			{
				zoneClass = settings->_lineZoneClass.Get();
			}

			break;
		}
		case EGsTerritoryZoneShapeType::Polygon:
		{
			if (settings->_polygonZoneClass)
			{
				zoneClass = settings->_polygonZoneClass.Get();
			}

			break;
		}
		default:
			break;
		}
	}

	return zoneClass;
}

void UGsTerritoryGroundActionUtility::AddFenceTown(FText inName, FGsSchemaSoundResDataRow inSound)
{
	FTransform trs;
	trs.SetLocation(GEditor->ClickLocation);

	FGsTerritoryLevelManager::CreateFenceTown(trs, inName, inSound);
}

void UGsTerritoryGroundActionUtility::AddFenceTownChaos(FText inName, FGsSchemaSoundResDataRow inSound)
{
	FTransform trs;
	trs.SetLocation(GEditor->ClickLocation);

	FGsTerritoryLevelManager::CreateFenceTownChaos(trs, inName, inSound);
}

void UGsTerritoryGroundActionUtility::AddFenceSafeZone(FText inName, FGsSchemaSoundResDataRow inSound)
{
	FTransform trs;
	trs.SetLocation(GEditor->ClickLocation);

	FGsTerritoryLevelManager::CreateFenceSafeZone(trs, inName, inSound);
}

void UGsTerritoryGroundActionUtility::AddFenceArena(FText inName, FGsSchemaSoundResDataRow inSound)
{
	FTransform trs;
	trs.SetLocation(GEditor->ClickLocation);

	FGsTerritoryLevelManager::CreateFenceArena(trs, inName, inSound);
}

void UGsTerritoryGroundActionUtility::AddFenceArenaReady(FText inName, FGsSchemaSoundResDataRow inSound)
{
	FTransform trs;
	trs.SetLocation(GEditor->ClickLocation);

	FGsTerritoryLevelManager::CreateFenceArenaReady(trs, inName, inSound);
}

void UGsTerritoryGroundActionUtility::AddFenceArrival(FText inName, FGsSchemaSoundResDataRow inSound)
{
	FTransform trs;
	trs.SetLocation(GEditor->ClickLocation);

	FGsTerritoryLevelManager::CreateFenceArrival(trs, inName, inSound);
}

void UGsTerritoryGroundActionUtility::AddFenceEnv(FText inName, TArray<FGsEnvCondition> inEnvConditionArray)
{
	FTransform trs;
	trs.SetLocation(GEditor->ClickLocation);

	FGsTerritoryLevelManager::CreateFenceEnv(trs, inName, inEnvConditionArray);
}

void UGsTerritoryGroundActionUtility::AddFenceSound(FText inName, TArray<FGsQuestSound> inQuestSoundArray)
{
	FTransform trs;
	trs.SetLocation(GEditor->ClickLocation);

	FGsTerritoryLevelManager::CreateFenceSound(trs, inName, inQuestSoundArray);
}

void UGsTerritoryGroundActionUtility::AddFenceSpaceCrack(FText inName, EGsEnvEvent inEnvEvent)
{
	FTransform trs;
	trs.SetLocation(GEditor->ClickLocation);

	FGsTerritoryLevelManager::CreateFenceSpaceCrack(trs, inName, inEnvEvent);
}

void UGsTerritoryGroundActionUtility::AddFenceNoCampZone(FText inName)
{
	FTransform trs;
	trs.SetLocation(GEditor->ClickLocation);

	FGsTerritoryLevelManager::CreateNoCampZone(trs, inName);
}

void UGsTerritoryGroundActionUtility::AddFenceNonPK(FText inName)
{
	FTransform trs;
	trs.SetLocation(GEditor->ClickLocation);

	FGsTerritoryLevelManager::CreateFenceNonPK(trs, inName);
}

bool UGsTerritoryGroundActionUtility::IsCorrectSelect()
{
	TArray<AActor*> selectArray = UEditorUtilityLibrary::GetSelectionSet();

	if (1 != selectArray.Num())
	{
		return false;
	}

	if (AActor * select = selectArray[0])
	{
		return true;
	}

	return false;
}


void UGsTerritoryGroundActionUtility::SaveRTT(const FString& inLevelKey, const FGsTerritoryMapSpot& inRtt)
{
#if SAVE_RTT
	//map 데이타에 맵 id가 등록되어 있어야만 서버용 맵으로 사용할 수 있기 때문에 수정할 때는 찾기만 하면된다. 
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(inLevelKey);
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* table = tableManager.GetTable(FGsSchemaMapTerritoryData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		return;
	}

	FString spotDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpotData>(inLevelKey);
	UGsTable* spotTable = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaSpotData::StaticStruct(), spotDataTableName);
	if (nullptr == spotTable)
	{
		return;
	}

	const FGsSchemaMapTerritoryData* row;
	FGsSchemaMapTerritoryData newMapData;

	table->FindRow<FGsSchemaMapTerritoryData>(*inLevelKey, row);
	if (nullptr != row)
	{
		newMapData = (*row);
		table->RemoveRow(row);
	}

	FGsSchemaSpotDataRow spotDataRow;
	spotDataRow.DataTable = spotTable->GetCachedDataTable();
	spotDataRow.RowName = *inLevelKey;

	newMapData.RowName = *inLevelKey;
	newMapData.RTT.mapId = spotDataRow;
	newMapData.RTT.spotIndex = inRtt._spotId;

	table->AddRow(newMapData);
	table->SaveDataTable();
#endif
}
