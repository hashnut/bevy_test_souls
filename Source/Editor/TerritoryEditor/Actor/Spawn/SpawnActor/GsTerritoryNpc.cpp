// Fill out your copyright notice in the Description page of Project Settings.

#include "GsTerritoryNpc.h"

//UE4
#include "Engine/Engine.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"

//T1 Project
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/GsSchemaShapeData.h"
#include "Level/Territory/GsTerritoryZoneDefine.h"
#include "RSQLEditor/Public/GsTableManagerEditor.h"
#include "Runtime/Public/GsTable.h"
#include "ActorComponentEx/WidgetComponentEx.h"
#include "UTIL/GsTableUtil.h"

//Territory editor
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "Util/GsTerritoryEditorUtil.h"
#include "Components/ChildActorComponent.h"
#include "Data/GsResourceManager.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "GameFramework/Character.h"

#include "T1Project/UTIL/GsUIUtil.h"
#include "Editor/TerritoryEditor/GsTerritoryLevelManager.h"
#include "Editor/TerritoryEditor/Component/GsTerrEdWidgetComponent.h"

#include "Editor/TerritoryEditor/GsTerritoryEditorSetting.h"
#include "UTIL/GsText.h"

const FName OFFENSIVE_NAME = FName(TEXT("Offensive"));

AGsTerritoryNpc::AGsTerritoryNpc()
{
	_arrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	if (_arrowComponent)
	{
		RootComponent = _arrowComponent;
	}

	_npcChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("NpcChildActor"));
	if (_npcChildActor)
	{
		_npcChildActor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		_npcChildActor->SetRelativeLocation(FVector::ZeroVector);
	}

	_sightRadiusComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SightRadiusComponent"));
	if (_sightRadiusComponent)
	{
		_sightRadiusComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		_sightRadiusComponent->SetRelativeLocation(FVector::ZeroVector);
	}

	if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
	{
		_descWidgetBGColor = settings->_npcColor;
	}

	PrimaryActorTick.bCanEverTick = true;
}

void AGsTerritoryNpc::OnConstruction(const FTransform& in_transform)
{	
	Super::OnConstruction(in_transform);	
}

void AGsTerritoryNpc::BeginDestroy()
{
	if (_npcChildActor)
	{
		_npcChildActor->DestroyChildActor();
	}	

	Super::BeginDestroy();
}

void AGsTerritoryNpc::PostEditChangeProperty(FPropertyChangedEvent& inPropertyChangedEvent)
{
	Super::PostEditChangeProperty(inPropertyChangedEvent);

	if (FProperty * property = inPropertyChangedEvent.Property)
	{
		if (property->GetFName() == OFFENSIVE_NAME)
		{
			InvalidSightRadius();
		}
	}
}

void AGsTerritoryNpc::TickActorInEditor(float inDelta)
{
	Super::TickActorInEditor(inDelta);

	
	if (UGsUIUtil::TryGetEditorCameraLocation(_cameraLocation))
	{
		_isIn = ((_cameraLocation - GetActorLocation()).Size() < FGsTerritoryLevelManager::_shapeVisibleDistance);
		if (_isIn)
		{
			if (FGsTerritoryLevelManager::_shapeShowType == EGsTerrEdShowType::HideAll)
			{
				if (_npcChildActor->IsVisible())
				{
					_npcChildActor->SetVisibility(false);
				}
			}
			else if (FGsTerritoryLevelManager::_shapeShowType == EGsTerrEdShowType::ShowAll)
			{
				if (false == _npcChildActor->IsVisible())
				{
					_npcChildActor->SetVisibility(true);
				}
			}
			else if (FGsTerritoryLevelManager::_shapeShowType == EGsTerrEdShowType::ShowOnlySelect)
			{
				if (false == _npcChildActor->IsVisible()
					&& IsSelectedInEditor())
				{
					_npcChildActor->SetVisibility(true);
				}
				else if (_npcChildActor->IsVisible()
					&& false == IsSelectedInEditor())
				{
					_npcChildActor->SetVisibility(false);
				}
			}

			//sight radius
			if (FGsTerritoryLevelManager::_sightRadiusShowType == EGsTerrEdShowType::HideAll)
			{
				if (_sightRadiusComponent->IsVisible())
				{
					_sightRadiusComponent->SetVisibility(false);
				}
			}
			else if (FGsTerritoryLevelManager::_sightRadiusShowType == EGsTerrEdShowType::ShowAll)
			{
				if (false == _sightRadiusComponent->IsVisible())
				{
					_sightRadiusComponent->SetVisibility(true);
				}
			}
			else if (FGsTerritoryLevelManager::_sightRadiusShowType == EGsTerrEdShowType::ShowOnlySelect)
			{
				if (false == _sightRadiusComponent->IsVisible()
					&& IsSelectedInEditor())
				{
					_sightRadiusComponent->SetVisibility(true);
				}
				else if (_sightRadiusComponent->IsVisible()
					&& false == IsSelectedInEditor())
				{
					_sightRadiusComponent->SetVisibility(false);
				}
			}
		}
		else
		{
			if (_npcChildActor->IsVisible())
			{
				_npcChildActor->SetVisibility(false);
			}
		}
	}
}

FString AGsTerritoryNpc::GetSpawnDataText()
{
	FString log;
	if (_spawnInfo.TargetID == 0)
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("AGsTerritoryNpc::GetSpawnDataText - target id is 0 - ld : %s"), *GetActorLabel());
		return log;
	}

	int64 zoneUniqueId = 0;	
	if (_zone)
	{
		zoneUniqueId = _zone->GetTerritoryUniqueID();
	}	

	const FString& uniqueID = FString::Printf(TEXT("%lld"), _uniqueID);
	int32 balanceId = 0;
	const FGsSchemaNpcData* npcData = UGsTerritoryLevelUtil::FindNpcData(_spawnInfo.TargetID);
	if (nullptr == npcData)
	{
		return log;
	}

	const FGsSchemaNpcBalance* balanceData = npcData->balanceId.GetRow();
	if (nullptr == balanceData)
	{
		return log;
	}
		
	balanceId = balanceData->id;
	float sightRadius = GetSightRadius();
	const FString& aggroLinkText = (_spawnInfo.isAggroLink ? TEXT("true") : TEXT("false"));

	log = FString::Printf(TEXT("Zone unique id : %lld\n"), zoneUniqueId)
		+ FString::Printf(TEXT("Unique id : %s\n"), *uniqueID)		
		+ FString::Printf(TEXT("Target id : %d\n"), _spawnInfo.TargetID)
		+ FString::Printf(TEXT("Spawn level: %d\n"), _spawnInfo.SpawnLevel)
		+ FString::Printf(TEXT("Balance id id : %d\n"), balanceId)
		+ FString::Printf(TEXT("Text Name : %s\n"), *_targetName)
		+ FString::Printf(TEXT("Location : %s\n"), *GetActorLocation().ToString())
		+ FString::Printf(TEXT("Rotation : %s\n"), *GetActorRotation().ToString())
		+ FString::Printf(TEXT("Init Spawn Delay : %f\n"), _spawnInfo.InitSpawnDelay)
		+ FString::Printf(TEXT("Respawn Interval : %f\n"), _spawnInfo.RespawnInterval)
		+ FString::Printf(TEXT("SpawnRate : %d\n"), _spawnInfo.SpawnRate)
		+ FString::Printf(TEXT("Group limit range : %f\n"), _spawnInfo.groupLimitRange)
		+ FString::Printf(TEXT("Offensive : %s\n"), *FGsTextUtil::GetEnumValueAsString(TEXT("OffensiveType"), _spawnInfo.offensiveType))
		+ FString::Printf(TEXT("Sight radius : %f\n"), sightRadius)
		+ FString::Printf(TEXT("AggroLink : %s\n"), *aggroLinkText);

	return log;
}

void AGsTerritoryNpc::Destroyed()
{
	Super::Destroyed();
}

void AGsTerritoryNpc::InvalidShape()
{
	SetShape(_spawnInfo.TargetID);
}

const EGsTerritoryObjType AGsTerritoryNpc::GetTerritoryObjType() const
{
	return EGsTerritoryObjType::Npc;
}

bool AGsTerritoryNpc::SetShape(int32 inNpcId)
{	
	if (inNpcId == 0)
	{
		return false;
	}

	if (nullptr == _npcChildActor)
	{
		return false;
	}

	const FGsSchemaNpcData* npcData = UGsTerritoryLevelUtil::FindNpcData(inNpcId);
	if (nullptr == npcData)
	{
		return false;
	}	

	const FGsSchemaShapeData* shapeData = npcData->shapeId.GetRow();
	if (nullptr == shapeData)
	{
#if WITH_EDITOR		
		GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red,
			TEXT("캐릭터 외형 테이블에 외형 ID 가 존재하지 않습니다."));
#endif

		return false;
	}

	UClass* shapeClass = Cast<UClass>(UAssetManager::GetStreamableManager().LoadSynchronous<UClass>(shapeData->bPClass.ToString() + TEXT("_C")));
	if (nullptr == shapeClass)
	{
#if WITH_EDITOR		
		GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red,
			TEXT("캐릭터 외형 테이블에 외형 ID 가 존재하지 않습니다."));
#endif
		return false;
	}

	_npcChildActor->SetChildActorClass(shapeClass);
	if (_npcChildActor->GetChildActor())
	{
		_npcChildActor->GetChildActor()->SetActorHiddenInGame(true);
	}

	if (const FGsSchemaCreatureCommonInfo* info = npcData->commonInfoId.GetRow())
	{
		_npcChildActor->SetRelativeLocation(FVector(0, 0, info->cylinderHeight));
	}	
	else
	{
		_npcChildActor->SetRelativeLocation(FVector::ZeroVector);
	}

	InvalidSightRadius();	

	return true;
}

void AGsTerritoryNpc::SelectInternal(UObject* inSelect)
{
	Super::SelectInternal(inSelect);

	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryNpc::SelectInternal - label : %s"), *GetActorLabel());

	if (this == inSelect)
	{		
		if (FGsTerritoryLevelManager::_sightRadiusShowType == EGsTerrEdShowType::ShowOnlySelect)
		{
			FGsTerritoryLevelManager::ActiveSightRadius(FGsTerritoryLevelManager::_sightRadiusShowType);
		}
	}
}

void AGsTerritoryNpc::InvalidSightRadius()
{
	float sightRadius = GetSightRadius();
	if (_sightRadiusComponent)
	{
		float scale = sightRadius / 100.0f;
		_sightRadiusComponent->SetWorldScale3D(FVector(scale, scale, 0.01f));
	}
}

void AGsTerritoryNpc::InvalidWidgetPosition(const FVector& in_location)
{
#if TERR_ED_WIDGET_COMP
	if (_widgetComp)
	{
		const FVector& location = GetWidgetLocation();
		_widgetComp->SetWorldLocation(location);
	}
#endif
}

void AGsTerritoryNpc::InvalidWidgetText()
{	
	SetTerritoryTitleText(TEXT("Npc"));

	_description = TEXT("Npc spawn data\n") + GetSpawnDataText();

	Super::InvalidWidgetText();
}

/// <summary>
/// 위젯의 월드 좌표를 가져온다
/// </summary>
/// <returns></returns>
FVector AGsTerritoryNpc::GetWidgetLocation()
{		
	if (_widgetComp)
	{
		if (_npcChildActor
			&& _npcChildActor->GetChildActor())
		{
			ACharacter* character = Cast<ACharacter>(_npcChildActor->GetChildActor());
			if (character)
			{
				const FVector& headPos = character->GetMesh()->GetBoneLocation(TEXT("Bip001-Head"));
				if (headPos == FVector::ZeroVector)
				{
					return GetActorLocation() + FVector(0, 0, _widgetHeight);
				}
				else
				{
					return headPos + FVector(0, 0, _widgetHeight);
				}
			}
		}
	}

	return GetActorLocation() + FVector(0, 0, _widgetHeight);
}

void AGsTerritoryNpc::SetShapeVisible(bool inShow)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryNpc::SetShapeVisible - label : %s"), *GetActorLabel());

	_npcChildActor->SetVisibility(inShow);
}

float AGsTerritoryNpc::GetSightRadius()
{	
	float sightRadius = 0;
	if (_spawnInfo.TargetID == 0)
	{
		return sightRadius;
	}

	const FGsSchemaNpcData* npcData = UGsTerritoryLevelUtil::FindNpcData(_spawnInfo.TargetID);
	if (nullptr == npcData)
	{
		return sightRadius;
	}

	switch (_spawnInfo.offensiveType)
	{
	case OffensiveType::OFFENSIVE:
	case OffensiveType::NORMAL:
	{
		sightRadius = _spawnInfo.sightRadius;
	}
	break;
	default:
	{
		sightRadius = npcData->sightRadius;
	}
	break;
	}

	return sightRadius;
}

void AGsTerritoryNpc::SetSightRidusVisible(bool inVisible)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryNpc::SetSightRidusVisible - label : %s"), *GetActorLabel());

	_sightRadiusComponent->SetVisibility(inVisible);
}
