// Fill out your copyright notice in the Description page of Project Settings.

#include "GsTerritoryProp.h"

//UE4
#include "Engine/Engine.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/AssetManager.h"

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
#include "GameObject/Prop/GsSchemaPropData.h"

#include "T1Project/UTIL/GsUIUtil.h"
#include "Editor/TerritoryEditor/GsTerritoryLevelManager.h"
#include "Editor/TerritoryEditor/Component/GsTerrEdWidgetComponent.h"

#include "Editor/TerritoryEditor/GsTerritoryEditorSetting.h"

AGsTerritoryProp::AGsTerritoryProp()
{
	_arrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	if (_arrowComponent)
	{
		RootComponent = _arrowComponent;
	}

	_childActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor"));
	if (_childActor)
	{
		_childActor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
	{
		_descWidgetBGColor = settings->_propColor;
	}
}

void AGsTerritoryProp::OnConstruction(const FTransform& in_transform)
{
	Super::OnConstruction(in_transform);	
}

void AGsTerritoryProp::TickActorInEditor(float inDelta)
{
	Super::TickActorInEditor(inDelta);

	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryProp::TickActorInEditor - label : %s"), *GetActorLabel());
}

void AGsTerritoryProp::Destroyed()
{
	Super::Destroyed();
}

void AGsTerritoryProp::InvalidShape()
{
	SetChildActor(_spawnInfo.TargetID);
}

const EGsTerritoryObjType AGsTerritoryProp::GetTerritoryObjType() const
{
	return EGsTerritoryObjType::Prop;
}

bool AGsTerritoryProp::TryGetShapePath(int32 in_id, FSoftObjectPath& out_path)
{	
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryProp::TryGetShapePath - label : %s"), *GetActorLabel());

	if (in_id == 0)
	{
		return false;
	}

	const UGsTablePropData* table = FGsTerritoryLevelManager::GetCachedPropData();
	if (nullptr == table)
	{
		return false;
	}

	const FGsSchemaPropData* find =  nullptr;
	table->FindRowById(in_id, find);
	if (nullptr == find)
	{
		const FString& log = TEXT("PROP 테이블에 PROP ID ")
			+ FString::FromInt(in_id)
			+ TEXT("가 존재하지 않습니다.");

		GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red, *log);

		return false;
	}

	_targetName = find->nameString.ToString();

	const FGsSchemaShapeData* shape = find->shapeId.GetRow();
	if (nullptr == shape)
	{
#if WITH_EDITOR
		/*FString log = TEXT("캐릭터 외형 테이블에 외형 ID ")
			+ FString::FromInt(npc->shapeId)
			+ TEXT("가 존재하지 않습니다.");*/

		GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red, 
			TEXT("Prop 외형 ID 가 존재하지 않습니다."));
#endif

		return false;
	}

	out_path = shape->staticMeshPath;

	return true;
}

bool AGsTerritoryProp::SetShape(int32 in_id)
{
	//if (_mesh)
	//{
	//	FSoftObjectPath loadAsset;
	//	if (TryGetShapePath(in_id, loadAsset))
	//	{
	//		//load 
	//		if (UObject* load = loadAsset.TryLoad())
	//		{
	//			if (USkeletalMesh* shape = Cast<USkeletalMesh>(load))
	//			{
	//				_mesh->SetSkeletalMesh(shape);

	//				return true;
	//			}
	//		}
	//	}
	//}

	return false;
}

bool AGsTerritoryProp::SetStaticMesh(int32 inId, UStaticMeshComponent* inMesh)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryProp::SetStaticMesh - label : %s"), *GetActorLabel());

	if (inId == 0)
	{
		return false;
	}

	if (inMesh)
	{
		FSoftObjectPath loadAsset;
		if (TryGetShapePath(inId, loadAsset))
		{
			//load 
			if (UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(loadAsset))
			{
				if (UStaticMesh* shape = Cast<UStaticMesh>(load))
				{
					inMesh->SetStaticMesh(shape);
					return true;
				}
			}
		}
	}

	return false;
}

bool AGsTerritoryProp::SetChildActor(int32 inId)
{
	if (inId == 0)
	{
		return false;
	}

	FSoftObjectPath loadAsset;
	if (TryGetBlueprintPath(inId, loadAsset))
	{
		UBlueprint* blueprint = Cast<UBlueprint>(UAssetManager::GetStreamableManager().LoadSynchronous(loadAsset));
		if (blueprint)
		{
			//load 
			_childActor->SetChildActorClass(TSubclassOf<AActor>(blueprint->GeneratedClass));
			_childActor->CreateChildActor();
		}		
	}

	return false;
}

bool AGsTerritoryProp::TryGetBlueprintPath(int32 inId, FSoftObjectPath& outPath)
{
	if (inId == 0)
	{
		return false;
	}

	const UGsTablePropData* propTable = FGsTerritoryLevelManager::GetCachedPropData();
	if (nullptr == propTable)
	{
		return false;
	}

	const FGsSchemaPropData* findProp = nullptr;
	propTable->FindRowById(inId, findProp);
	if (nullptr == findProp)
	{
		const FString& log = TEXT("PROP 테이블에 PROP ID ")
			+ FString::FromInt(inId)
			+ TEXT("가 존재하지 않습니다.");

		GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red, *log);

		return false;
	}

	const UGsTableShapeData* shapeTable = FGsTerritoryLevelManager::GetCachedShapeData();
	if (nullptr == shapeTable)
	{
		return false;
	}

	if (nullptr == findProp->shapeId.GetRow())
	{
		UE_LOG(LogTemp, Error, TEXT("Shape id is missing - prop id : %d"), findProp->id);
	}

	outPath = findProp->shapeId.GetRow()->bPClass;

	return true;
}

void AGsTerritoryProp::InvalidWidgetPosition(const FVector& in_location)
{
#if TERR_ED_WIDGET_COMP
	if (_widgetComp)
	{
		const FVector& location = GetWidgetLocation();
		_widgetComp->SetWorldLocation(location);
	}
#endif
}

void AGsTerritoryProp::InvalidWidgetText()
{
	_description = TEXT("Prop spawn data\n") + GetSpawnDataText();

	SetTerritoryTitleText(TEXT("Prop"));

	Super::InvalidWidgetText();
}

void AGsTerritoryProp::SetShapeVisible(bool inShow)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryProp::SetShapeVisible - label : %s"), *GetActorLabel());

	_childActor->SetVisibility(inShow);
}