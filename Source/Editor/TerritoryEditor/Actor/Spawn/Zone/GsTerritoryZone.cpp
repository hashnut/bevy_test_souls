// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryZone.h"

//UE4
#include "Editor/UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "Editor/UnrealEd/Public/UnrealEdGlobals.h"
#include "Editor/UnrealEd/Public/Editor.h"
#include "Editor/ComponentVisualizers/Public/SplineComponentVisualizer.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Runtime/Engine/Classes/Components/ChildActorComponent.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "Runtime/Core/Public/Misc/DateTime.h"
#include "Runtime/Engine/Classes/Engine/Selection.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

//T1 Project
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "RSQLEditor/Public/GsTableManagerEditor.h"
#include "Runtime/Public/GsTable.h"
#include "T1Project/UTIL/GsText.h"
#include "T1Project/ActorComponentEx/WidgetComponentEx.h"
#include "T1Project/Editor/Widget/GsEditorWidget.h"


//Territory editor
#include "Actor/Spawn/GsTerritoryActor.h"
#include "Actor/Spawn/SpawnActor/GsTerritorySpawnActor.h"
#include "GsTerritoryEditorSetting.h"
#include "Shape/GsTerritoryZoneCircleShape.h"
#include "Shape/GsTerritoryZonePolygonShape.h"
#include "Shape/GsTerritoryZoneLineShape.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Util/GsTerritoryZoneShapeUtil.h"
#include "GameObject/Prop/GsSchemaPropData.h"

#include "Editor/TerritoryEditor/TerritoryEditor.h"
#include "Editor/TerritoryEditor/Component/GsTerrEdWidgetComponent.h"

#include "Editor/TerritoryEditor/GsTerritoryEditorSetting.h"
#include "UTIL/GsDir.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "DataSchema/Map/Spawn/GsSchemaSpawnVisibleData.h"
#include "Private/Helper/GsNotificatorOnDestruction.h"
#include "SNotificationList.h"

AGsTerritoryZone::AGsTerritoryZone()
{
	if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
	{
		_descWidgetBGColor = settings->_zoneColor;
	}

	bRunConstructionScriptOnDrag = false;
}

void AGsTerritoryZone::OnConstruction(const FTransform& in_transform)
{
	Super::OnConstruction(in_transform);

#if INVALID_ZONE_SHAPE

	KeepTerritoryActorRelatvieLocation(in_transform);
#endif

	/*UE_LOG(LogTemp, Log, TEXT("OnConstruction zone - index : %lld \tname : %s\taddress : 0x%p"), GetTerritoryUniqueID(), *GetActorLabel(), this);*/
}

void AGsTerritoryZone::PostEditChangeProperty(FPropertyChangedEvent& inPropertyChangedEvent)
{
	Super::PostEditChangeProperty(inPropertyChangedEvent);

	if (FProperty* property = inPropertyChangedEvent.Property)
	{
		InvalidShape();
	}
}

void AGsTerritoryZone::DestroyManually()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryZone::DestroyManually - label : %s"), *GetActorLabel());

	TArray<AGsTerritorySpawnActor*> tempArray;
	for (AGsTerritorySpawnActor* spawnActor : _spawnActors)
	{
		if (nullptr == spawnActor)
		{
			continue;
		}
		
		tempArray.Add(spawnActor);		
	}
	_spawnActors.Empty();

	for (AGsTerritorySpawnActor* spawnActor : tempArray)
	{
		if (spawnActor->IsValid())
		{
			UnregisterTerritoryUniqueID(spawnActor, spawnActor->GetTerritoryObjType(), spawnActor->_instanceID);
		}

		spawnActor->_isCanDelete = true;
		spawnActor->Destroy();
	}

	Super::DestroyManually();
}

void AGsTerritoryZone::KeepTerritoryActorRelatvieLocation(const FTransform& in_trs)
{
	const FVector& origin = in_trs.GetLocation();
	for (AGsTerritorySpawnActor* spawnActor : _spawnActors)
	{
		if (nullptr == spawnActor)
		{
			continue;
		}

		spawnActor->SetActorLocation(origin + spawnActor->GetRelativeLocation());
		spawnActor->InvalidWidgetPosition(spawnActor->GetActorLocation());
		spawnActor->InvalidWidgetText();
	}
}

void AGsTerritoryZone::Destroyed()
{
	if (_selectHandle.IsValid())
	{
		USelection::SelectObjectEvent.Remove(_selectHandle);
	}

	Super::Destroyed();
}

void AGsTerritoryZone::AddTerritoryActor(AGsTerritorySpawnActor* in_actor)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryZone::AddTerritoryActor - label : %s"), *GetActorLabel());

	_spawnActors.Add(in_actor);
}

void AGsTerritoryZone::RemoveTerritoryActor(AGsTerritorySpawnActor* in_actor)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryZone::RemoveTerritoryActor - label : %s"), *GetActorLabel());

	_spawnActors.Remove(in_actor);
}

bool AGsTerritoryZone::IsTerritoryActorExist(AGsTerritorySpawnActor* in_actor)
{
	return (INDEX_NONE != _spawnActors.Find(in_actor));
}


void AGsTerritoryZone::InvalidWidgetPosition(const FVector& in_location)
{
#if TERR_ED_WIDGET_COMP
	if (nullptr == _widgetComp)
	{
		return;
	}

	FVector pos;
	FVector temp;
	float max = GetActorLocation().Z;

	if (_spawnActors.Num() > 0)
	{
		for (AGsTerritorySpawnActor* spawnActor : _spawnActors)
		{
			if (spawnActor)
			{
				temp = spawnActor->GetWidgetLocation();
				if (temp.Z > max)
				{
					max = temp.Z;
				}
			}

		}

		pos = FVector(in_location.X, in_location.Y, 0) + FVector(0, 0, max + _widgetHeight);
		_widgetComp->SetWorldLocation(pos);
	}
	else
	{
		pos = in_location + FVector(0, 0, _height + _widgetHeight);
		_widgetComp->SetWorldLocation(pos);
	}
#endif

}

void AGsTerritoryZone::InvalidDestription()
{
	SetTerritoryTitleText(TEXT("Zone"));
	
	if (_isValid)
	{
		FString spawnGroupText = FGsTextUtil::GetStringFromEnum(TEXT("NpcSpawnGroupType"), _spawnGroupType);
		FString clientText = _byClient ? TEXT("True") : TEXT("False");
		FString byDefaultText = _byDefault ? TEXT("True") : TEXT("False");

		_description = FString::Printf(TEXT("Unique id : %lld\n"), _uniqueID)
			+ FString::Printf(TEXT("group id : %d\n"), _groupID)
			+ FString::Printf(TEXT("spawnGroupType : %s\n"), *spawnGroupText)						
			+ FString::Printf(TEXT("byClient : %s\n"), *clientText)
			+ FString::Printf(TEXT("byDefault : %s\n"), *byDefaultText)
			+ FString::Printf(TEXT("radius : %f\n"), _radius)
			+ FString::Printf(TEXT("section : %d\n"), _section)
			+ FString::Printf(TEXT("Note : %s\n"), *_note)
			+ FString::Printf(TEXT("Line id : %lld\n"), _lineId.LineID)
			+ FString::Printf(TEXT("Location : %s\n"), *GetActorLocation().ToString());
	}
	else
	{
		_description = INVALID_TERRITORY_ACTOR;
	}
}

AGsTerritorySpawnActor* AGsTerritoryZone::AddTerritorySpawnActor(const FGsTerritorySpawnInfo& in_data, const FVector& in_worldLocaiton)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryZone::AddTerritorySpawnActor - label : %s"), *GetActorLabel());

	if (false == IsValideKey(in_data))
	{
		const FString& log = TEXT("Can't Add spawn data to zone - invalide key : ") + FString::FromInt(in_data.TargetID);
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, log);

		return nullptr;
	}

	FString invalidReason;
	if (AActor* actor = CreateTerritoryTemplate(in_data, in_worldLocaiton))
	{
		if (AGsTerritorySpawnActor* spawnActor = Cast<AGsTerritorySpawnActor>(actor))
		{
			spawnActor->SetSpawnInfo(in_data);
			spawnActor->SetTerritoryRelativeLocation(in_worldLocaiton - GetActorLocation());
			spawnActor->SetTerritoryZone(this);
			spawnActor->InvalidShape();
			if (false == spawnActor->InvalidTerritoryUniqueID(invalidReason))
			{
				FGsTerritoryLevelManager::SetInvalidTerritoryActor(spawnActor, invalidReason);
			}

			AddTerritoryActor(spawnActor);

			spawnActor->InvalidWidgetText();

			return spawnActor;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Add spawn data to zone : %s"), *GetName());

	return nullptr;
}

AActor* AGsTerritoryZone::CreateTerritoryTemplate(const FGsTerritorySpawnInfo& in_data, const FVector& in_worldLocation)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryZone::CreateTerritoryTemplate - label : %s"), *GetActorLabel());

	FTransform trs;
	trs.SetLocation(in_worldLocation);
	AActor* temp = nullptr;
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	switch (in_data.Type)
	{
	case EGsTerritorySpawnType::Npc:
	{
		if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
		{
			if (settings->_npcClass)
			{			
				trs.SetLocation(in_worldLocation);
				temp = GetWorld()->SpawnActor(settings->_npcClass, &trs, param);
				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(temp);
			}
		}

		break;
	}
	case EGsTerritorySpawnType::Prop:
	{
		if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
		{
			if (settings->_propClass)
			{				
				temp = GetWorld()->SpawnActor(settings->_propClass, &trs, param);
				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(temp);
			}
		}

		break;
	}
	}

	return temp;
}

EGsTerritoryObjType AGsTerritoryZone::ConverTerritorySpawnType(EGsTerritorySpawnType inSpawn)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryZone::ConverTerritorySpawnType - label : %s"), *GetActorLabel());

	switch (inSpawn)
	{
	case EGsTerritorySpawnType::Npc:
		return EGsTerritoryObjType::Npc;
		break;
	}

	return EGsTerritoryObjType::Max;
}

void AGsTerritoryZone::SetAllNpcSpawnLevel(int32 inLevel)
{
	for (AGsTerritorySpawnActor* spawnActor : _spawnActors)
	{
		if (spawnActor)
		{
			spawnActor->SetSpawnLevel(inLevel);
		}
	}
}

void AGsTerritoryZone::SetSection(const int32 inSection)
{
	_section = inSection;

	InvalidWidgetText();
}

void AGsTerritoryZone::SetNote(const FString& inNote)
{
	_note = inNote;

	InvalidWidgetText();
}

bool AGsTerritoryZone::IsValideKey(const FGsTerritorySpawnInfo& inData) const
{
	switch (inData.Type)
	{
	case EGsTerritorySpawnType::Npc:
	{
		const UGsTableNpcData* npcTbl = FGsTerritoryLevelManager::GetCachedNpcData();
		if (npcTbl)
		{
			const FGsSchemaNpcData* npcData = nullptr;
			return npcTbl->FindRowById(inData.TargetID, npcData);
		}

		break;
	}
	case EGsTerritorySpawnType::Prop:
	{
		const UGsTablePropData* propTable = FGsTerritoryLevelManager::GetCachedPropData();
		if (propTable)
		{
			const FGsSchemaPropData* propData = nullptr;
			return propTable->FindRowById(inData.TargetID, propData);
		}

		break;
	}
	}

	return false;
}

bool AGsTerritoryZone::RecoveryTerritoryUniqueID()
{
	/*if (_uniqueID == INDEX_NONE)
	{
		_uniqueID = _zone.UniqueID;

		return Super::RecoveryTerritoryUniqueID();

		return true;
	}*/

	return false;
}

void AGsTerritoryZone::Copy()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT(" AGsTerritoryZone::Copy - label : %s"), *GetActorLabel());

	//copy zone child
	TArray<AGsTerritorySpawnActor*> sourceActors = _spawnActors;
	_spawnActors.Empty();
	AGsTerritoryActor* spawnChildActor = nullptr;
	FString invalidReason;
	for (const AGsTerritorySpawnActor* source : sourceActors)
	{
		if (nullptr == source)
		{
			continue;
		}

		spawnChildActor = AddTerritorySpawnActor(source->_spawnInfo, source->GetActorLocation());
		if (nullptr == spawnChildActor)
		{
			continue;
		}

		if (false == spawnChildActor->InvalidTerritoryUniqueID(invalidReason))
		{
			FGsTerritoryLevelManager::SetInvalidTerritoryActor(spawnChildActor, invalidReason);
		}
	}

	for (AGsTerritorySpawnActor* spawnActor : _spawnActors)
	{
		GEditor->SelectActor(spawnActor, false, false);
	}

	sourceActors.Empty();
}

void AGsTerritoryZone::CopyZoneInfo(AGsTerritoryZoneBase* in_zone)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryZone::CopyZoneInfo - label : %s"), *GetActorLabel());

	if (nullptr == in_zone)
	{
		return;
	}

	AGsTerritoryZone* zone = Cast<AGsTerritoryZone>(in_zone);
	if (nullptr == zone)
	{
		return;
	}
	
	_byClient = zone->_byClient;
	_byDefault = zone->_byDefault;
	_groupID = zone->_groupID;
	_radius = zone->_radius;
	_points = zone->_points;

	for (AGsTerritorySpawnActor* actor : zone->_spawnActors)
	{
		if (actor)
		{
			const FGsTerritorySpawnInfo& info = actor->_spawnInfo;
			AddTerritorySpawnActor(info, actor->GetActorLocation());
		}
	}

	_splineComponent->ClearSplinePoints();

	if (USplineComponent* sourceSpline = zone->_splineComponent)
	{
		int32 num = sourceSpline->GetNumberOfSplinePoints();
		FVector location;
		for (int32 i = 0; i < num; ++i)
		{
			location = sourceSpline->GetWorldLocationAtSplinePoint(i);
			_splineComponent->AddSplinePoint(location, ESplineCoordinateSpace::World);
		}
	}

	CreateView(GetTransform());
	CreateWidget(GetTransform());
}

void AGsTerritoryZone::Export(FGsSchemaSpawnData& outSpawnData)
{
	UGsTableMapData* mapTable = FGsTerritoryLevelManager::GetCachedMapData();
	if (nullptr == mapTable)
	{
		return;
	}

	FString levelName;
	FString levelKey;
	if (false == UGsTerritoryLevelUtil::IsValidServerLevel(GetLevel(), levelName, levelKey))
	{
		return;
	}

	FGsSchemaMapDataRow map;
	map.DataTable = mapTable->GetCachedDataTable();
	map.RowName = *levelKey;

	FVector location;
	if (UGsTerritoryLevelUtil::IsSpawnActorSpawnOnNavmesh(this, location))
	{
		location += FVector(0, 0, 100);
	}
	else
	{
		location = GetActorLocation();

		UE_LOG(LogTerritoryEditor, Warning, TEXT("Zone is not on navmeh - label : %s\tstance id : %lld"), *GetActorLabel(), _instanceID);
	}

	FGsSchemaTerritoryZoneShape shape;
	shape.Center = location;
	shape.Radius = static_cast<int32>(_radius);

	outSpawnData.Region = shape;
	//set zone uniuqe id
	outSpawnData.groupId = _groupID;
	outSpawnData.SpawnGroupId = GetTerritoryUniqueID();
	outSpawnData.SpawnGroupType = GetSpawnGroupType();
	outSpawnData.MapId = map;
	outSpawnData.byClient = _byClient;
	outSpawnData.byDefault = _byDefault;
	outSpawnData.lineId = _lineId.LineID;
	outSpawnData.groupLineMove = _isGroupLineMove;	

	AGsTerritorySpawnActor* spawnActor = nullptr;
	int32 actorNum = _spawnActors.Num();
	int32 questConditionNum = 0;
	QuestId startQuestId = 0;
	QuestId endQuestId = 0;
	TArray<FString> errorPropList;
	const UGsTableNpcData* castNpcTable = FGsTerritoryLevelManager::GetCachedNpcData();
	if (nullptr == castNpcTable)
	{
		return;
	}

	for (int32 actorIndex = 0; actorIndex < actorNum; ++actorIndex)
	{
		spawnActor = _spawnActors[actorIndex];
		if (nullptr == spawnActor)
		{
			continue;
		}

		questConditionNum = spawnActor->_spawnInfo.QuestCondition.Num();
		for (int questConditionIndex = 0; questConditionIndex < questConditionNum; ++questConditionIndex)
		{
			startQuestId = FCString::Strtoi64(*spawnActor->_spawnInfo.QuestCondition[questConditionIndex].StartConditionQuestId, NULL, 10);
			endQuestId = FCString::Strtoi64(*spawnActor->_spawnInfo.QuestCondition[questConditionIndex].EndConditionQuestId, NULL, 10);
			if (endQuestId != 0 && startQuestId > endQuestId)
			{
				errorPropList.Add(spawnActor->GetActorLabel());
				continue;
			}
		}

		FGsSchemaSpawnElement spawnElement;
		if(AGsTerritoryZone::TryConvertSpawnActorToSpawnElement(spawnActor, spawnElement))
		{
			outSpawnData.SpawnInfoList.Add(spawnElement);
		}		
	}

	if (errorPropList.Num() > 0)
	{
		FString errorLog = TEXT("spanw data quest error\n");
		for (int32 i = 0; i < errorPropList.Num(); ++i)
		{
			errorLog += (errorPropList[i] + TEXT("\n"));
		}

		FGsNotificatorOnDestruction::ShowNotificationNow(
			FText::FromString(errorLog)
			, SNotificationItem::ECompletionState::CS_Fail
			, nullptr
			, FGsNotificatorOnDestruction::eHyperLinkType::Select, 60.0f);
	}

	outSpawnData.randomSpotList.Empty();
	for (FGsTerritoryCurrentMapSpot spot : _randomSpotIdList)
	{
		outSpawnData.randomSpotList.Add(spot._spotId);
	}
}

void AGsTerritoryZone::Import(const FGsSchemaSpawnData* inSpawnData)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryZone::Import - label : %s"), *GetActorLabel());

	//set shape
	_radius = inSpawnData->Region.Radius;

	//set element
	int32 spawnInfoNum = inSpawnData->SpawnInfoList.Num();
	for (int32 i = 0; i < spawnInfoNum; ++i)
	{
		const FGsSchemaSpawnElement& element = inSpawnData->SpawnInfoList[i];
		ConvertSpawnElementToSpawnActor(element);
	}

	_randomSpotIdList.Empty();
	for (int64 lineId : inSpawnData->randomSpotList)
	{
		FGsTerritoryCurrentMapSpot spotData;
		spotData._spotId = lineId;
		_randomSpotIdList.Add(spotData);
	}	

	CreateView(GetTransform());
}

const AGsTerritorySpawnActor* AGsTerritoryZone::ConvertSpawnElementToSpawnActor(const FGsSchemaSpawnElement& inElement)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryZone::ConvertSpawnElementToSpawnActor - label : %s"), *GetActorLabel());

	FGsTerritorySpawnInfo spawnInfo;
	spawnInfo.Type = FGsTerritoryLevelManager::ConvertCreatureTypeToSpawnType(inElement.SpawnType);
	spawnInfo.SpawnGroupMemberType = inElement.memberType;
	spawnInfo.TargetID = inElement.CreatureId;
	spawnInfo.MaxSpawnNum = inElement.SpawnCount;
	spawnInfo.RespawnInterval = inElement.RespawnInterval / 1000.0f;
	spawnInfo.SpawnRate = inElement.spawnRate;
	spawnInfo.groupLimitRange = inElement.groupLimitRange;
	spawnInfo.CanRandomLocationSpawn = inElement.RandomPos;
	spawnInfo.InitVisible = inElement.InitVisible;
	spawnInfo.VisibleType = inElement.VisibleType;
	spawnInfo.RelativeLocation = GetActorLocation() - inElement.Pos;
	spawnInfo.Rotation = FGsDir::AngleToRotator(inElement.Dir).Vector();
	spawnInfo.offensiveType = inElement.offensiveType;
	spawnInfo.sightRadius = inElement.sightRadius;
	spawnInfo.isAggroLink = inElement.aggroLink;

	FGsTerrEdSpawnObjectShowHide showHide;
	for (const FGsSchemaSpawnObjectShowHide& element : inElement.QuestCondition)
	{		
		const FGsSchemaQuestRow& startQuest = element.StartConditionQuestId;
		const FGsSchemaQuestRow& endQuest = element.EndConditionQuestId;

		showHide.StartConditionQuestId = (startQuest.GetRow() ? (FString::Printf(TEXT("%llu"), startQuest.GetRow()->id)) : "");
		showHide.EndConditionQuestId = (endQuest.GetRow() ? (FString::Printf(TEXT("%llu"), endQuest.GetRow()->id)) : "");
		showHide.QuestCheckType = element.QuestCheckType;

		spawnInfo.QuestCondition.Emplace(showHide);
	}

	FTransform trs;
	trs.SetLocation(inElement.Pos);
	trs.SetRotation(FGsDir::AngleToRotator(inElement.Dir).Quaternion());

	const TArray<AGsTerritorySpawnActor*>& spawnActorArray = FGsTerritoryLevelManager::CreateSpawnActor(spawnInfo, 1, EGsTerritorySpawnShapeType::Random, this, trs);
	if (spawnActorArray.Num() > 0)
	{
		spawnActorArray[0]->SetTerritoryUniqueIDForce(inElement.SpawnId);
		return spawnActorArray[0];
	}

	return nullptr;
}

bool AGsTerritoryZone::TryConvertSpawnActorToSpawnElement(const AGsTerritorySpawnActor* inZoneChild, OUT FGsSchemaSpawnElement& outSpawnElement)
{	
	outSpawnElement.SpawnId = inZoneChild->GetTerritoryUniqueID();
	outSpawnElement.memberType = inZoneChild->GetSpawnGroupMemberType();
	outSpawnElement.SpawnType = inZoneChild->GetCreatureSpawnType();
	outSpawnElement.Pos = inZoneChild->GetActorLocation();
	outSpawnElement.Dir = FGsDir::RotatorToAngle(inZoneChild->GetActorRotation());
	outSpawnElement.CreatureId = inZoneChild->_spawnInfo.TargetID;
	outSpawnElement.SpawnCount = inZoneChild->_spawnInfo.MaxSpawnNum;
	outSpawnElement.RespawnInterval = FTimespan::FromSeconds(inZoneChild->_spawnInfo.RespawnInterval).GetTotalMilliseconds();
	outSpawnElement.groupLimitRange = inZoneChild->_spawnInfo.groupLimitRange;
	outSpawnElement.RandomPos = inZoneChild->_spawnInfo.CanRandomLocationSpawn;
	outSpawnElement.ClientGameId = outSpawnElement.SpawnId | CLIENT_PACKET;
	outSpawnElement.InitVisible = inZoneChild->_spawnInfo.InitVisible;
	outSpawnElement.VisibleType = inZoneChild->_spawnInfo.VisibleType;
	outSpawnElement.QuestCondition.Empty();
	outSpawnElement.offensiveType = inZoneChild->_spawnInfo.offensiveType;
	outSpawnElement.sightRadius = inZoneChild->_spawnInfo.sightRadius;
	outSpawnElement.aggroLink = inZoneChild->_spawnInfo.isAggroLink;
	outSpawnElement.spawnLevel = inZoneChild->_spawnInfo.SpawnLevel;
	outSpawnElement.AiOptimizeOnOff = inZoneChild->_spawnInfo.AiOptimizeOnOff;
	outSpawnElement.isFixedLocation = inZoneChild->_spawnInfo.isFixedLocation;
	outSpawnElement.skipTerrainCheck = inZoneChild->_spawnInfo.SkipTerrainCheck;
	outSpawnElement.spawnRate = inZoneChild->_spawnInfo.SpawnRate;

	const UGsTableQuest* questTable = FGsTerritoryLevelManager::GetCachedQuestData();
	if (nullptr == questTable
		|| nullptr == questTable->GetCachedDataTable())
	{
		return false;
	}

	int32 questConditionNum = inZoneChild->_spawnInfo.QuestCondition.Num();
	FString startConditionQuestId = "";
	FString endConditionQuestId = "";
	const FGsSchemaQuest* startConditionQuest = nullptr;
	const FGsSchemaQuest* endConditionQuest = nullptr;
	FGsSchemaQuestRow startConditionQuestRow;
	FGsSchemaQuestRow endConditionQuestRow;
	FGsSchemaSpawnObjectShowHide spawnObjectShowHide;
	for (int32 questConditionIndex = 0; questConditionIndex < questConditionNum; ++questConditionIndex)
	{
		startConditionQuestId = inZoneChild->_spawnInfo.QuestCondition[questConditionIndex].StartConditionQuestId;
		endConditionQuestId = inZoneChild->_spawnInfo.QuestCondition[questConditionIndex].EndConditionQuestId;

		uint64 startQuestId = FCString::Strtoi64(*startConditionQuestId, NULL, 10);
		uint64 endQuestId = FCString::Strtoi64(*endConditionQuestId, NULL, 10);

		questTable->FindRowById(startQuestId, startConditionQuest);
		questTable->FindRowById(endQuestId, endConditionQuest);

		if (startConditionQuest)
		{
			startConditionQuestRow.DataTable = FGsSchemaQuest::GetStaticTable()->GetCachedDataTable();
			startConditionQuestRow.RowName = startConditionQuest->RowName;
		}

		if (endConditionQuest)
		{
			endConditionQuestRow.DataTable = FGsSchemaQuest::GetStaticTable()->GetCachedDataTable();
			endConditionQuestRow.RowName = endConditionQuest->RowName;
		}

		spawnObjectShowHide.StartConditionQuestId = startConditionQuestRow;
		spawnObjectShowHide.EndConditionQuestId = endConditionQuestRow;
		spawnObjectShowHide.QuestCheckType = inZoneChild->_spawnInfo.QuestCondition[questConditionIndex].QuestCheckType;

		outSpawnElement.QuestCondition.Add(spawnObjectShowHide);
	}

	return true;
}
