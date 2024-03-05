// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryZoneActionUtility.h"

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

//Territory editor
#include "Util/GsTerritoryEditorUtil.h"
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "TerritoryEditor.h"
#include "GsTerritoryEditorSetting.h"
#include "TerritoryEditor.h"
#include "GsTerritoryUniqueIDManager.h"
#include "Editor/GsTerritoryPropLayout.h"
#include "NavigationSystem.h"
#include "Input/GsTerritorySpawnActorInput.h"

const FString ONLY_ONE_ZONE_SELECT = TEXT("하나의 Zone만 선택해 주세요");
const FString NONE_ZONE_SELECT = TEXT("Zone이 선택되지 않았습니다.");
const FString MOUSE_NOT_ON_ZONE = TEXT("마우스가 Zone위에 있지 않습니다.");
const FString SPAWN_COUNT_WARNNING = TEXT("1 ~ 30 사이의 정수를 입력해야합니다.");

void UGsTerritoryZoneActionUtility::SelectAllTerritoryActor()
{
	if (nullptr == GEditor)
	{
		return;
	}

	AGsTerritoryZone* zone = GetSelectedTerritoryZone();
	if (nullptr == zone)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, NONE_ZONE_SELECT);
		return;
	}

	const TArray<AGsTerritorySpawnActor*>& spawnActors = zone->_spawnActors;

	GEditor->SelectNone(true, true);

	for (AGsTerritorySpawnActor* spawnActor : spawnActors)
	{
		if (spawnActor)
		{
			GEditor->SelectActor(spawnActor, true, true);
		}
	}
}

void UGsTerritoryZoneActionUtility::DeleteZone()
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	for (AActor* actor : selectArray)
	{
		if (actor)
		{
			if (AGsTerritoryZone* zone = Cast<AGsTerritoryZone>(actor))
			{
				zone->DestroyManually();
			}
		}
	}
}

void UGsTerritoryZoneActionUtility::CopyZone()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	if (false == IsSelectsZone())
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, NONE_ZONE_SELECT);
		return;
	}

	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();

	FTerritoryEditor* instance = FTerritoryEditor::GetInstance();
	if (nullptr == instance)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *INSTANCE_MISSING);
		return;
	}

	int64 uniqueID;
	int64 instanceID;
	for (AActor* actor : selectArray)
	{
		if (nullptr == actor)
		{
			continue;
		}		

		if (false == instance->GetUniqueIDManager()->IsCanGenerateUniqueID(world->GetCurrentLevel(), EGsTerritoryObjType::Zone))
		{
			FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *ZONE_UNIQUE_ID_FULL);
			return;
		}

		if (false == AGsTerritoryActor::TryGenerateTerritoryUniqueID(actor, EGsTerritoryObjType::Zone, uniqueID, instanceID))
		{
			return;
		}
		
		if (AGsTerritoryZone* sourceZone = Cast<AGsTerritoryZone>(actor))
		{
			EGsTerritoryZoneShapeType shape = sourceZone->GetShapeType();

			if (AActor* dest = CreateZone(shape, sourceZone->GetActorLocation()))
			{
				if (AGsTerritoryZone* destZone = Cast<AGsTerritoryZone>(dest))
				{
					destZone->CopyZoneInfo(sourceZone);
					destZone->SetTerritoryUniqueID(uniqueID, instanceID);
				}
			}
		}
	}
}

void UGsTerritoryZoneActionUtility::DeleteAllSpawnActor()
{
	if (nullptr == GEditor)
	{
		return;
	}

	AGsTerritoryZone* zone = GetSelectedTerritoryZone();
	if (nullptr == zone)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, NONE_ZONE_SELECT);
		return;
	}

	TArray<AGsTerritorySpawnActor*> spawnActors = zone->_spawnActors;
	for (AGsTerritorySpawnActor* spawnActor : spawnActors)
	{
		if (spawnActor)
		{
			spawnActor->DestroyManually();
		}		
	}
}

void UGsTerritoryZoneActionUtility::AddNpcDataAtLocation(FGsTerritoryNpcInput inData, int32 inSpawnCount /*= 1*/, EGsTerritorySpawnShapeType inShape /*= EGsTerritorySpawnShapeType::Random*/)
{
	FGsTerritorySpawnInfo spawnData;

	spawnData.Type = EGsTerritorySpawnType::Npc;
	spawnData.SpawnGroupMemberType = inData.SpawnGroupMemberType;
	spawnData.TargetID = inData.TargetID;
	spawnData.SpawnLevel = inData.SpawnLevel;
	spawnData.MaxSpawnNum = inData.MaxSpawnNum;
	spawnData.InitSpawnDelay = inData.InitSpawnDelay;
	spawnData.RespawnInterval = inData.RespawnInterval;
	spawnData.groupLimitRange = inData.groupLimitRange;
	spawnData.CanRandomLocationSpawn = inData.CanRandomLocationSpawn;
	spawnData.InitVisible = inData.InitVisible;
	spawnData.VisibleType = inData.VisibleType;
	spawnData.QuestCondition = inData.QuestCondition;	

	AddSpawnDataAtLocation(spawnData, inSpawnCount, inShape);
}

//시도 횟수만큼 추가하려고 시도
//만약 이전에 추가한 모든 점과 최소 거리가 안맞다면 실제로 추가하지 않는다.
//navmesh 위에 없어도 진나간다 
void UGsTerritoryZoneActionUtility::TryAddNpcDataInZone(FGsTerritoryNpcInput inNpc, int32 inTrySpawnCount, float inMinDistance)
{
	//첫번째 랜덤 좌표 추가
	AGsTerritoryZone* zone = GetSelectedTerritoryZone();
	EGsTerritoryZoneShapeType spawnType = zone->GetShapeType();
	if (spawnType != EGsTerritoryZoneShapeType::Circle)
	{
		return;
	}

	const FVector& zoneLocation = zone->GetActorLocation();
	const float radius = zone->_radius;	
	TArray<FVector> spawnLocationList;
	FVector spawnLocation;
	FHitResult result;
	UWorld* world = GEditor->GetEditorWorldContext().World();
	UNavigationSystemBase* navigaton = (world != nullptr ? world->GetNavigationSystem() : nullptr);
	UNavigationSystemV1* navigationSystemV1 = (navigaton != nullptr ? Cast<UNavigationSystemV1>(navigaton) : nullptr);	
	FNavLocation navLocation;
	float gap = TNumericLimits<float>::Max();

	for (int i = 0; i < inTrySpawnCount; ++i)
	{
		spawnLocation = GetRandomLocationInZone(zoneLocation, radius);

		//Check navmesh
		if (false == navigationSystemV1->ProjectPointToNavigation(spawnLocation, navLocation, FVector(1, 1, gap)))
		{
			continue;
		}

		//check distance
		if (CheckSpawnLocationDistance(spawnLocation, spawnLocationList, inMinDistance))
		{
			spawnLocationList.Add(spawnLocation);
		}
	}	

	FTerritoryEditor* instance = FTerritoryEditor::GetInstance();
	if (nullptr == instance)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *INSTANCE_MISSING);
		return;
	}

	for (const FVector& locationIter : spawnLocationList)
	{
		if (false == instance->GetUniqueIDManager()->IsCanGenerateUniqueID(world->GetCurrentLevel(), EGsTerritoryObjType::Npc))
		{
			FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *NPC_UNIQUE_ID_FULL);
			return;
		}

		FGsTerritorySpawnInfo spawnData;
		spawnData.Type = EGsTerritorySpawnType::Npc;
		spawnData.TargetID = inNpc.TargetID;
		spawnData.MaxSpawnNum = inNpc.MaxSpawnNum;
		spawnData.InitSpawnDelay = inNpc.InitSpawnDelay;
		spawnData.RespawnInterval = inNpc.RespawnInterval;
		spawnData.groupLimitRange = inNpc.groupLimitRange;
		spawnData.CanRandomLocationSpawn = inNpc.CanRandomLocationSpawn;
		spawnData.InitVisible = inNpc.InitVisible;
		spawnData.VisibleType = inNpc.VisibleType;
		spawnData.QuestCondition = inNpc.QuestCondition;
		
		zone->AddTerritorySpawnActor(spawnData, locationIter);

		DrawDebugSphere(GEditor->GetWorld(), locationIter, 50.0f, 8, FColor::Blue, false, 60.0f);
		DrawDebugLine(GEditor->GetWorld(), locationIter, locationIter + FVector(0, 0, -1000), FColor::Blue, false, 60.0f);
	}
}

void UGsTerritoryZoneActionUtility::AddPropDataAtLocation(FGsTerritoryPropInput inData, int32 inSpawnCount /*= 1*/, EGsTerritorySpawnShapeType inShape /*= EGsTerritorySpawnShapeType::Random*/)
{
	FGsTerritorySpawnInfo spawnData;

	spawnData.Type = EGsTerritorySpawnType::Prop;
	spawnData.TargetID = inData.TargetID;
	spawnData.MaxSpawnNum = inData.MaxSpawnNum;
	spawnData.InitSpawnDelay = inData.InitSpawnDelay;
	spawnData.RespawnInterval = inData.RespawnInterval;	
	spawnData.CanRandomLocationSpawn = inData.CanRandomLocationSpawn;
	spawnData.InitVisible = inData.InitVisible;
	spawnData.VisibleType = inData.VisibleType;
	spawnData.QuestCondition = inData.QuestCondition;	

	AddSpawnDataAtLocation(spawnData, inSpawnCount, inShape);
}

void UGsTerritoryZoneActionUtility::SetAllNpcSpawnLevel(int32 inLevel)
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	for (AActor* actor : selectArray)
	{
		if (actor)
		{
			if (AGsTerritoryZone* zone = Cast<AGsTerritoryZone>(actor))
			{
				zone->SetAllNpcSpawnLevel(inLevel);
			}
		}
	}
}

void UGsTerritoryZoneActionUtility::SetSection(const int32 inSection)
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	for (AActor* actor : selectArray)
	{
		if (actor)
		{
			if (AGsTerritoryZone* zone = Cast<AGsTerritoryZone>(actor))
			{
				zone->SetSection(inSection);
			}
		}
	}
}

void UGsTerritoryZoneActionUtility::SetNote(const FString& inNote)
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	for (AActor* actor : selectArray)
	{
		if (actor)
		{
			if (AGsTerritoryZone* zone = Cast<AGsTerritoryZone>(actor))
			{
				zone->SetNote(inNote);
			}
		}
	}
}

bool UGsTerritoryZoneActionUtility::CheckSpawnLocationDistance(const FVector& inLocation, const TArray<FVector>& inSpawnLocationList, float inMinDistance)
{
	float distance = 0;
	for (const FVector& location : inSpawnLocationList)
	{
		 distance = (location - inLocation).Size();
		if (distance < inMinDistance)
		{
			return false;
		}
	}

	return true;
}

void UGsTerritoryZoneActionUtility::AddSpawnDataAtLocation(const FGsTerritorySpawnInfo& inSpawn, int32 inSpawnCount /*= 1*/, EGsTerritorySpawnShapeType inShape /*= EGsTerritorySpawnShapeType::Random*/)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();

	//Check spawn count
	if (inSpawnCount < 1 || inSpawnCount > 30)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, SPAWN_COUNT_WARNNING);
		return;
	}

	if (false == IsSelectOnlyOneZone())
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, ONLY_ONE_ZONE_SELECT);
		return;
	}

	AGsTerritoryZone* zone = GetSelectedTerritoryZone();
	if (nullptr == zone)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, NONE_ZONE_SELECT);
		return;
	}

	FVector pickLocation;
	if (false == FGsTerritoryEditorUtil::TryGetLocationOfCollisionFromRightClick(world, pickLocation))
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, MOUSE_NOT_ON_ZONE);
	}

	FVector tempLocatin;
	float zoneRadius = zone->_radius;
	float radius = zoneRadius - (zone->GetActorLocation() - pickLocation).Size();

	FGsTerritorySpawnInfo spawnData = inSpawn;
	FTerritoryEditor* instance = FTerritoryEditor::GetInstance();
	if (nullptr == instance)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *INSTANCE_MISSING);
		return;
	}

	for (int i = 0; i < inSpawnCount; ++i)
	{
		if (false == instance->GetUniqueIDManager()->IsCanGenerateUniqueID(world->GetCurrentLevel(), EGsTerritoryObjType::Npc))
		{
			FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *NPC_UNIQUE_ID_FULL);
			return;
		}

		if (0 == i)
		{
			zone->AddTerritorySpawnActor(spawnData, pickLocation);
		}
		else
		{
			float dist = FMath::RandRange(0.0f, radius);
			float angle = FMath::RandRange(0, 360);
			FVector temp = (FVector::ForwardVector.RotateAngleAxis(angle, FVector::UpVector) * dist);
			FVector location = pickLocation + temp;

			zone->AddTerritorySpawnActor(spawnData, location);
		}
	}

	FGsTerritoryEditorUtil::RefreshForceDetailPanel();
}

FVector UGsTerritoryZoneActionUtility::GetRandomLocationInZone(const FVector& inZoneLocaiton, float inRadius)
{	
	FRandomStream random(FMath::Rand());	
	random.GenerateNewSeed();
	int32 dist = random.RandRange(0, inRadius);
	random.GenerateNewSeed();
	int32 rotate = random.RandRange(0, 360);

	FVector forward = FVector::ForwardVector * dist;
	forward = forward.RotateAngleAxis(rotate, FVector::UpVector);
	
	return FVector(inZoneLocaiton.X + forward.X, inZoneLocaiton.Y + forward.Y, inZoneLocaiton.Z);
}

AGsTerritoryZone* UGsTerritoryZoneActionUtility::GetSelectedTerritoryZone()
{
	TArray<AActor*> selectArray = UEditorUtilityLibrary::GetSelectionSet();
	if (selectArray.Num() == 1)
	{
		if (AGsTerritoryZone * zone = Cast<AGsTerritoryZone>(selectArray[0]))
		{
			return zone;
		}
	}

	FString log = TEXT("영역이 없습니다.");
	FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, log);

	return nullptr;
}

bool UGsTerritoryZoneActionUtility::IsSelectOnlyOneZone()
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	if (selectArray.Num() == 1)
	{
		if (AGsTerritoryZone * zone = Cast<AGsTerritoryZone>(selectArray[0]))
		{
			return true;
		}
	}

	FString log = TEXT("하나의 영역만 선택해 주세요.");

	UE_LOG(LogTerritoryEditor, Error, TEXT("%s"), *log);	

	return false;
}

bool UGsTerritoryZoneActionUtility::IsSelectsZone()
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();

	if (selectArray.Num() <= 0)
	{
		FString log = TEXT("아무것도 선택되지 않았습니다.");
		UE_LOG(LogTerritoryEditor, Error, TEXT("%s"), *log);

		return false;
	}

	for (AActor* actor : selectArray)
	{
		if (AGsTerritoryZone* zone = Cast<AGsTerritoryZone>(actor))
		{
			if (nullptr == zone)
			{
				FString log = TEXT("터레인 영역만 선택해 주세요");					
				UE_LOG(LogTerritoryEditor, Error, TEXT("%s"), *log);

				return false;
			}
		}
	}

	return true;
}

AActor* UGsTerritoryZoneActionUtility::CreateZone(EGsTerritoryZoneShapeType in_type, const FVector& in_location)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();

	FTransform trs;
	trs.SetLocation(in_location);
	AActor* temp = nullptr;

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	switch (in_type)
	{
	case EGsTerritoryZoneShapeType::Circle:

		if (settings->_circleZoneClass)
		{
			temp = GEditor->AddActor(world->GetCurrentLevel(), settings->_circleZoneClass, trs);
		}
		else
		{			
			FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *MISSING_CIRCLE_ZONE);
		}

		break;
	case EGsTerritoryZoneShapeType::Polygon:

		if (settings->_polygonZoneClass)
		{
			temp = GEditor->AddActor(world->GetCurrentLevel(), settings->_polygonZoneClass, trs);
		}
		else
		{			
			FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *MISSING_POLYGON_ZONE);
		}

		break;
	case EGsTerritoryZoneShapeType::Line:

		if (settings->_lineZoneClass)
		{
			temp = GEditor->AddActor(world->GetCurrentLevel(), settings->_lineZoneClass, trs);
		}
		else
		{			
			FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *MISSING_LINE_ZONE);
		}

		break;
	}

	return temp;	
}
