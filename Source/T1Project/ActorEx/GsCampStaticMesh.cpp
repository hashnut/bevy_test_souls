// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorEx/GsCampStaticMesh.h"
#include "GsCharacterLocalPlayer.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "Engine/Classes/Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsCampManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "NavigationSystem.h"
#include "NavigationSystem/Public/NavMesh/RecastNavMesh.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Kismet/GameplayStatics.h"
#include "T1Project.h"

//for Debug
#include "DrawDebugHelpers.h"

// Sets default values
AGsCampStaticMesh::AGsCampStaticMesh() : Super()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Camp StaticMesh
	_campStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CampStaticMesh"));
	_campStaticMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGsCampStaticMesh::BeginPlay()
{
	Super::BeginPlay();

	_isReadyToCompleteStaticMesh = false;
	_fenceHandler = GFence();

	_campMgr = GSCamp();
	_campMgr->SetIsOkBuildCampArea(false);

	FVector originVec;
	this->GetActorBounds(true, originVec, _actorBoundsExtends);
}

void AGsCampStaticMesh::EndPlay(const EEndPlayReason::Type in_type)
{
	if (UWorld* world = GetWorld())
	{
		world->GetTimerManager().ClearAllTimersForObject(this);
	}

	Super::EndPlay(in_type);
}

// Called every frame
void AGsCampStaticMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetCampLocation();
	CheckBuilableProc();
	//GSLOG(Warning, TEXT("[AGsCamp::Tick] - Pos : %s"), *this->GetActorLocation().ToString());
}

void AGsCampStaticMesh::DestoryClientSpawn()
{
	_isClientSpawn = false;
	_localPlyerPtr.Reset();

	_campMgr = nullptr;

	Destroy();
}

bool AGsCampStaticMesh::CheckNavMeshRayCast()
{
	float R = (_actorBoundsExtends.X + _actorBoundsExtends.Y) * 0.5f;
	FVector DirRight = this->GetActorRightVector();
	FVector rightPos = this->GetActorLocation() + (DirRight * R);
	FVector leftPos = this->GetActorLocation() - (DirRight * R);
	//FVector rightPos = this->GetActorLocation() + (DirRight * (1.1f * R));
	//FVector leftPos = this->GetActorLocation() - (DirRight * (1.1f * R));

	FVector DirForward = this->GetActorForwardVector();
	FVector forwardPos = this->GetActorLocation() + (DirForward * R);
	FVector backPos = this->GetActorLocation() - (DirForward * R);
	//FVector forwardPos = this->GetActorLocation() + (DirForward * (1.1f * R));
	//FVector backPos = this->GetActorLocation() - (DirForward * (1.1f * R));
	if (!IsHitInNav(leftPos))
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[Reason] - is not navimesh area - leftPos"));
#endif
		return false;
	}
	if (!IsHitInNav(rightPos))
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[Reason] - is not navimesh area - rightPos"));
#endif
		return false;
	}
	if (!IsHitInNav(forwardPos))
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[Reason] - is not navimesh area - forwardPos"));
#endif
		return false;
	}
	if (!IsHitInNav(backPos))
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[Reason] - is not navimesh area - backPos"));
#endif
		return false;
	}
	if (!IsHitInNav(this->GetActorLocation()))
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[Reason] - is not navimesh area - GetActorLocation"));
#endif
		return false;
	}

	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, this->GetActorLocation(),
		this->GetActorLocation() - FVector(0, 0, 1000.f), ECC_Visibility, CollisionParams);
	if (bHit)
	{
		FVector terrainNormal = false == Hit.Normal.IsNormalized() ? Hit.Normal.GetSafeNormal() : Hit.Normal;
		float degrees = FMath::RadiansToDegrees(acosf(FVector::DotProduct(FVector::UpVector, terrainNormal)));
#if WITH_EDITOR
		if (_campMgr->_testAngleFlag)
		{
			GSLOG(Warning, TEXT("[CheckNavMeshRayCast] - RadiansToDegrees : %f "), degrees);
		}
#endif
		if (degrees > _angleMinValue && degrees < _angleMaxValue)
		{
			return true;
		}
		else
		{
#if WITH_EDITOR
			GSLOG(Warning, TEXT("[Reason] - is not allow slope angle"));
#endif
			FText findText;
			FText::FindText(TEXT("GuildText"), TEXT("Alert_CannotBuilding"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return false;
		}
	}

	return false;

	//DrawDebugDirectionalArrow(GetWorld(), rightPos, rightPos - FVector(0.f, 0.f, 500.f), 200.0f, FColor::Red, false, -1.0f, 0, 10.0f);
	//DrawDebugDirectionalArrow(GetWorld(), leftPos, leftPos - FVector(0.f, 0.f, 500.f), 200.0f, FColor::Red, false, -1.0f, 0, 10.0f);
	//DrawDebugDirectionalArrow(GetWorld(), forwardPos, forwardPos - FVector(0.f, 0.f, 500.f), 200.0f, FColor::Red, false, -1.0f, 0, 10.0f);
	//DrawDebugDirectionalArrow(GetWorld(), backPos, backPos - FVector(0.f, 0.f, 500.f), 200.0f, FColor::Red, false, -1.0f, 0, 10.0f);
}

bool AGsCampStaticMesh::IsHitInNav(FVector InTargetStartPos)
{
	const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this->GetWorld());
	if (nullptr != NavSys)
	{
		const AGsCharacterBase* owner = Cast<AGsCharacterBase>(this);
		const ANavigationData* NavData = owner ? NavSys->GetNavDataForProps(owner->GetNavAgentPropertiesRef()) : nullptr;
		if (nullptr != NavData)
		{
			FSharedConstNavQueryFilter QueryFilter = NavData->GetDefaultQueryFilter();
			const ARecastNavMesh* RecastNavMesh = Cast<const ARecastNavMesh>(NavData);
			ARecastNavMesh::FRaycastResult hitResult;
			if (RecastNavMesh)
			{
				FHitResult Hit;
				FCollisionQueryParams CollisionParams;
				const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, InTargetStartPos,
					InTargetStartPos - FVector(0.f, 0.f, 10000.f), ECC_Visibility, CollisionParams);
				if (bHit)
				{
					// for Test
#if WITH_EDITOR
					DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 100.f, 30, FColor::White, false);
#endif
					FVector detect;
					if (ARecastNavMesh::NavMeshRaycast(RecastNavMesh, InTargetStartPos, Hit.ImpactPoint, detect, QueryFilter, this, hitResult))
					{
						//GSLOG(Error, TEXT("[NavMeshRaycast] - HitNormal Pos : %s") , *hitResult.HitNormal.ToString());
						return !hitResult.HitNormal.IsZero();
						//return !hitResult.HasHit();
					}
				}
				else
				{
#if WITH_EDITOR
					GSLOG(Warning, TEXT("[Reason] - is not hit trace terrain"));
#endif
					return false;
				}
			}
		}
	}

	return true;
}


void AGsCampStaticMesh::SetCampLocation()
{
	if (false == _localPlyerPtr.IsValid())
	{
		if (false == FindLocalPlayerPtr())
		{
			GSLOG(Error, TEXT("[AGsCamp::SetCampLocation()] - false == GetLocalPlayerPtr() !!!!"));
			return;
		}
	}

	float R = (_actorBoundsExtends.X + _actorBoundsExtends.Y) * 0.5f;
	FVector Dir = _localPlyerPtr->GetActorForwardVector();
	FVector Location = _localPlyerPtr->GetActorLocation() + (Dir * (2.0f * R)) + FVector(0.f, 0.f, (_actorBoundsExtends.Z / 1.5f));
	this->SetActorLocation(Location);
	this->SetActorRotation(_localPlyerPtr->GetActorRotation());
}

void AGsCampStaticMesh::SetCampStaticMesh(UStaticMesh* InStaticMesh, UMaterialInterface* InMaterial)
{
	if (nullptr == InStaticMesh || nullptr == InMaterial || nullptr == _campStaticMesh)
		return;

	_campStaticMesh->SetStaticMesh(InStaticMesh);
	_campStaticMesh->SetMaterial(0, InMaterial);

	_isReadyToCompleteStaticMesh = true;

	_actorBoundsExtends = InStaticMesh->GetExtendedBounds().BoxExtent;
	GSLOG(Error, TEXT("_actorBoundsExtends : %s"), *_actorBoundsExtends.ToString());
}


uint8 AGsCampStaticMesh::CheckBuilableProc()
{
	if (nullptr == _campStaticMesh && false == _isReadyToCompleteStaticMesh)
		return 2;

	if (false == _isClientSpawn)
		return 2;

	if (false == _localPlyerPtr.IsValid())
	{
		if (false == FindLocalPlayerPtr())
		{
			GSLOG(Error, TEXT("[AGsCamp::CheckBuilableProc()] - false == GetLocalPlayerPtr() !!!!"));
			return 2;
		}
	}

	// 네비메시 기반 설치유무 체크
	//bool checkNavMesh = CheckNavMeshRayCast(_localPlyerPtr->GetActorLocation(), this->GetActorLocation(), outDetectPos, outHitNormal);
	bool checkNavMesh = CheckNavMeshRayCast();
	if (checkNavMesh 
		&& _campMgr->IsOkBuildArea()
		&& _fenceHandler->CanCreateCampZone(GetActorLocation()))
	{
		_campStaticMesh->SetVectorParameterValueOnMaterials(TEXT("Ghost Color"), FVector(0.f, 0.235638f, 1.498511f));
		_campStaticMesh->SetVectorParameterValueOnMaterials(TEXT("Base Color"), FVector(0.f, 1.0f, 0.647124f));
		_campMgr->SetIsOkBuildCampArea(true);
		return 0;
	}
	else
	{
		_campStaticMesh->SetVectorParameterValueOnMaterials(TEXT("Ghost Color"), FVector(1.498511f, 0.203f, 0.f));
		_campStaticMesh->SetVectorParameterValueOnMaterials(TEXT("Base Color"), FVector(0.647124f, 0.422f, 0.f));
		_campMgr->SetIsOkBuildCampArea(false);

		FText findText;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_CannotBuilding"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		return 1;
	}
}

bool AGsCampStaticMesh::IsOkBuildCampArea()
{
	return _campMgr->IsOkBuildCampArea();
}

bool AGsCampStaticMesh::FindLocalPlayerPtr()
{
	UGsGameObjectManager* objMgr = GSGameObject();
	if (nullptr == objMgr)
		return false;

	if (UGsGameObjectBase* findLocal = objMgr->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsGameObjectLocalPlayer* localPlayer = findLocal->CastGameObject<UGsGameObjectLocalPlayer>();
		AActor* localActor = localPlayer->GetActor();
		_localPlyerPtr = Cast<AGsCharacterLocalPlayer>(localActor);

		return true;
	}

	return false;
}

float AGsCampStaticMesh::GetCampDiameter()
{
	return _actorBoundsExtends.X + _actorBoundsExtends.Y;
}