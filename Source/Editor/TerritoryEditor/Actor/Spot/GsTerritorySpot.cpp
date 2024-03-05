// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritorySpot.h"

//ue4
#include "UObject/ConstructorHelpers.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Engine/Texture2D.h"

//t1 project
#include "UTIL/GsText.h"
#include "UTIL/GsDir.h"

//territory editor
#include "Util/GsTerritoryZoneShapeUtil.h"
#include "Actor/Warp/GsTerritoryWarp.h"
#include "Editor/TerritoryEditor/GsTerritoryEditorSetting.h"
#include "TerritoryEditor/Util/GsTerritoryLevelUtil.h"

// Sets default values
AGsTerritorySpot::AGsTerritorySpot()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_arrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (_arrowComponent)
	{
		SetRootComponent(_arrowComponent);
	}

	_splineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	if (_splineComponent)
	{
		_splineComponent->SetClosedLoop(true);
		_splineComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}

	_AreaComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Area"));
	if (_AreaComponent)
	{
		_AreaComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

		_AreaComponent->SetCollisionProfileName(TEXT("NoCollision"));
	}

	SetHidden(true);
	SetCanBeDamaged(false);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> transMaterial(TEXT("/Game/TerritoryEditor/Actor/TransparentMaterial"));
	_transMaterial = Cast<UMaterial>(transMaterial.Object);
}

void AGsTerritorySpot::OnConstruction(const FTransform& inTransform)
{
	Super::OnConstruction(inTransform);
	
	TransparentFloor();
}

void AGsTerritorySpot::InvalidShape()
{
	int32 index;

	if (UGsTerritoryZoneShapeUtil::TryGetSelectedIndexInSpline(index))
	{
		float radius;

		if (UGsTerritoryZoneShapeUtil::TryGetRadius(GetActorLocation(), _splineComponent, index, radius))
		{
			_radius = radius;
		}
	}

	TArray<FVector> points = UGsTerritoryZoneShapeUtil::CreateCircleRelativePoints(_radius, 8);

	if (_splineComponent)
	{
		_splineComponent->ClearSplinePoints();

		for (FVector& point : points)
		{
			_splineComponent->AddSplinePoint(point, ESplineCoordinateSpace::Local);
		}
	}

	float scale = _radius / DEFAULT_SPOT_MESH_RADIUS;

	FVector newScale = FVector(scale, scale, 0.01f);

	if (_AreaComponent)
	{
		_AreaComponent->SetRelativeScale3D(newScale);
	}
}

void AGsTerritorySpot::TransparentFloor()
{
	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return;
	}

	UMaterialInterface* material = _AreaComponent->GetMaterial(0);
	UMaterialInstanceDynamic* dynamicMaterial = Cast<UMaterialInstanceDynamic>(material);
	if (nullptr == dynamicMaterial
		|| false == dynamicMaterial->IsChildOf(_transMaterial))
	{		
		dynamicMaterial = UMaterialInstanceDynamic::Create(settings->_transparentMaterial.LoadSynchronous(), nullptr);
		if (dynamicMaterial)		
		{
			_AreaComponent->SetMaterial(0, dynamicMaterial);
		}
	}

	dynamicMaterial = Cast<UMaterialInstanceDynamic>(_AreaComponent->GetMaterial(0));
	if (nullptr == dynamicMaterial)
	{
		return;
	}

	FColor spotColor = _spotColor;
	if (0 == _spotColor.A)
	{
		TryGetDefualtColor(spotColor, GetSpotType());
	}

	if (FGsTerritoryLevelManager::_isTransparentFloor)
	{	
		spotColor.A = FGsTerritoryLevelManager::_transparentFloorAlpha;
	}

	dynamicMaterial->BlendMode = EBlendMode::BLEND_Translucent;
	dynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), spotColor);
}

bool AGsTerritorySpot::SetIndexForce(uint32 inInstanceId)
{
	//Check can generate unique id
	if (false == FTerritoryEditor::GetInstance()->GetUniqueIDManager()->IsCanGenerateUniqueID(GetLevel(), GetTerritoryObjType(), inInstanceId, true))
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, UNIQUE_ID_GENERATE_INVALID);
#endif
		return false;
	}

	//remove unique id
	FTerritoryEditor::GetInstance()->GetUniqueIDManager()->Remove(GetLevel(), GetTerritoryObjType(), _instanceID);

	int64 uniqueID;
	int64 instanceID;

	//add unique id
	if (false == FTerritoryEditor::GetInstance()->GetUniqueIDManager()->TryGenerateUniqueID(GetLevel(), GetTerritoryObjType(), inInstanceId, uniqueID, instanceID))
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("AGsTerritorySpot::SetIndexForce - Generate unique id fail - instance id : %d"), inInstanceId);
		return false;
	}
	SetTerritoryUniqueID(uniqueID, instanceID);

	const FString& levelName = UGsTerritoryLevelUtil::GetLevelName(GetLevel());
	FString persistantLevelName;
	FString levelKey;

	if (UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevelName, levelKey))
	{
		const FString& newName = FGsTerritoryUniqueIDManager::CombineTerritoryActorName(levelKey, GetTerritoryObjType(), _instanceID);
		SetActorLabel(newName);
	}
	else
	{
		const FString& newName = FGsTerritoryUniqueIDManager::CombineInvalidTerrritoryActorName(GetTerritoryObjType());
		SetActorLabel(newName);
	}

	InvalidSpotIndex();
	InvalidWidgetText();
	Modify();

	return true;
}

bool AGsTerritorySpot::TryGetDefualtColor(OUT FColor& outColor, EGsTerritorySpotType inSpotType)
{
	//get territory setting
	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();

	//get default color
	switch (inSpotType)
	{
	case EGsTerritorySpotType::Normal:
		outColor = settings->_normalSpotColor;
		break;
	case EGsTerritorySpotType::Spawn:
		outColor = settings->_spawnSpotColor;
		break;
	case EGsTerritorySpotType::Resurrect:
		outColor = settings->_resurrectionSpotColor;
		break;
	case EGsTerritorySpotType::Warp:
		outColor = settings->_warpSpotColor;
		break;
	case EGsTerritorySpotType::Teleport:
		outColor = settings->_teleportSpotColor;
		break;
	case EGsTerritorySpotType::SpaceCrackPortal:
		outColor = settings->_spawnSpotColor;
		break;
	default:
		break;
	}

	return false;
}

void AGsTerritorySpot::SetTerritoryUniqueID(int64& inUniqueID, int64& inInstanceID)
{
	_index = inInstanceID;

	Super::SetTerritoryUniqueID(inUniqueID, inInstanceID);
}

void AGsTerritorySpot::Export(FGsSchemaSpotInfo& outSpot)
{
	outSpot.id = _index;

	//서버에서는 x,y 좌표를 이용해 스팟 충돌을 한다. 그래서 z축을 네비메쉬에 위에 접해서 그릴 필요는 없지만 디버깅을 위해 보정해서 저장
	FVector location;

	if (IsNeedToCheckOnNavmesh())
	{
		if (UGsTerritoryLevelUtil::IsSpawnActorSpawnOnNavmesh(this, location))
		{
			location += FVector(0, 0, 100);
		}
		else
		{
			location = GetActorLocation();

			UE_LOG(LogTerritoryEditor, Warning, TEXT("Spot is not on navmeh - label : %s\tindex : %lld"), *GetActorLabel(), _index);
		}
	}
	else
	{
		location = GetActorLocation();

		UE_LOG(LogTerritoryEditor, Warning, TEXT("Spot is not on navmeh - label : %s\tindex : %lld"), *GetActorLabel(), _index);
	}

	outSpot.pos = location;
	outSpot.dir = FGsDir::RotatorToAngle(GetActorRotation());
	outSpot.radius = _radius;
	outSpot.name = _name;
	outSpot.isInTown = false;
	outSpot.randomDir = _randomDir;
	outSpot.isExceptRandomTeleport = _isExceptRandomTeleport;
}

void AGsTerritorySpot::SetText(const FText& inText)
{
	_name = inText;
}

AGsTerritoryNormalSpot::AGsTerritoryNormalSpot()
{
	if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
	{
		_descWidgetBGColor = settings->_normalSpotColor;
	}
}

void AGsTerritoryNormalSpot::InvalidWidgetText()
{
	if (_isValid)
	{
		FString description = FString(TEXT("Index : ")) + FString::FromInt(_index) + FString(TEXT("\n"))
			+ FString(TEXT("Name : ")) + _name.ToString() + FString(TEXT("\n"))
			+ FString(TEXT("Random range : ")) + FString::SanitizeFloat(_radius);

		_description = FString(TEXT("Normal Spot\n")) + description;
	}

	SetTerritoryTitleText(TEXT("Normal spot"));

	Super::InvalidWidgetText();
}