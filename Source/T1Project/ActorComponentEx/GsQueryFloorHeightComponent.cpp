#include "GsQueryFloorHeightComponent.h"

#include "DrawDebugHelpers.h"
#include "UTIL/GsLevelUtil.h"
#include "ActorEx/GsCharacterBase.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Cheat/GsCheatManager.h"

const float MAP_MAX_HEIGHT = 10000.0f;
const float MAP_MIN_HEIGHT = -10000.0f;
const FVector DEFAULT_PROJECT_POINT_EXTENSION = FVector(50, 50, 20000.0f);

bool UGsQueryFloorHeightComponent::CanDrawCharacterNavmesh = false;

UGsQueryFloorHeightComponent::UGsQueryFloorHeightComponent()
{
	_floorCollisionObjQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
#if (WITH_EDITOR)
	PrimaryComponentTick.bCanEverTick = true;
#else
	PrimaryComponentTick.bCanEverTick = false;
#endif
}

// Called every frame
void UGsQueryFloorHeightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


bool UGsQueryFloorHeightComponent::TryGetFloor(const FVector& inTargetLocation, FVector& outLocation)
{
	outLocation = inTargetLocation;

	if (TryGetNavFloor(inTargetLocation, _navLocation))
	{	
		outLocation = _navLocation;

		return true;		
	}

	if (TryGetGeometryFloor(inTargetLocation, _geometryLocation))
	{
		outLocation = _geometryLocation;

		return true;
	}			

	return false;
}

bool UGsQueryFloorHeightComponent::TryGetNavFloor(const FVector& inLocation, FVector& outLocation)
{
	outLocation = FVector::ZeroVector;

	if (INavigationDataInterface * navAgent = FNavigationSystem::GetNavDataForActor(*GetOwner()))
	{
		if (ARecastNavMesh * recast = Cast<ARecastNavMesh>(navAgent))
		{
			if (recast->ProjectPoint(inLocation, _navResult, DEFAULT_PROJECT_POINT_EXTENSION, nullptr, nullptr))
			{
				outLocation = _navResult.Location;

				return true;
			}
		}
	}

	return false;
}

bool UGsQueryFloorHeightComponent::TryGetTeleportLocation(FVector& outLocation, FHitResult& outResult, AGsCharacterBase* inCharacter, const FVector& inOrigin)
{
	if (inCharacter)
	{
		if (UWorld* world = inCharacter->GetWorld())
		{
			//Believe actor location is almost correct from server					
			return TryGetTeleportLocation(outLocation, outResult, world, inOrigin, inCharacter->GetDefaultHalfHeight());
		}
	}

	return false;
}

bool UGsQueryFloorHeightComponent::TryGetTeleportLocation(FVector& outLocation, FHitResult& outResult, UWorld* inWorld, const FVector& inOrigin, float inCapsuleHalfHeight, float inMinHeight)
{	
	if (inWorld)
	{
		//Believe actor location is almost correct from server
		FHitResult hitResult;
		//Get height from landscape or mesh using linetrace
		if (UGsLevelUtil::TryLineTraceToLand(hitResult, inWorld, inOrigin, inOrigin.Z + inCapsuleHalfHeight, inMinHeight))
		{
			//add capsule component's half height to collision location			
			outLocation = hitResult.Location + FVector(0, 0, inCapsuleHalfHeight);	
			outResult = hitResult;
			return true;
		}
	}

	return false;
}

bool UGsQueryFloorHeightComponent::TryGetTeleportLocationExceptGameObject(FVector& outLocation, FHitResult& outResult, AGsCharacterBase* inCharacter, const FVector& inOrigin)
{
	if (inCharacter)
	{
		if (UWorld* world = inCharacter->GetWorld())
		{
			//Believe actor location is almost correct from server					
			return TryGetTeleportLocationExceptGameObject(outLocation, outResult, world, inOrigin, inCharacter->GetDefaultHalfHeight());
		}
	}

	return false;
}

bool UGsQueryFloorHeightComponent::TryGetTeleportLocationExceptGameObject(FVector& outLocation, FHitResult& outResult, UWorld* inWorld, const FVector& inOrigin, float inCapsuleHalfHeight, float inMinHeight)
{
	if (inWorld)
	{
		//Believe actor location is almost correct from server
		FHitResult hitResult;
		//Get height from landscape or mesh using linetrace
		if (UGsLevelUtil::TryLineTraceToLandExceptGameObject(hitResult, inWorld, inOrigin, inOrigin.Z + inCapsuleHalfHeight, inOrigin.Z +inMinHeight))
		{
			//add capsule component's half height to collision location			
			outLocation = hitResult.Location + FVector(0, 0, inCapsuleHalfHeight);
			outResult = hitResult;
			return true;
		}
	}

	return false;
}


bool UGsQueryFloorHeightComponent::TryGetGeometryFloor(const FVector& inLocation, FVector& outLocation)
{
	FVector start = inLocation + FVector(0.0f, 0.0f, MAP_MAX_HEIGHT);
	FVector end = inLocation + FVector(0.0f, 0.0f, MAP_MIN_HEIGHT);

	if (GetWorld()->LineTraceSingleByObjectType(_floorHit, start, end
		, _floorCollisionObjQueryParams
		, _floorCollisionQueryParams))
	{
		outLocation = _floorHit.Location;

#if WITH_EDITOR
		if (CanDrawCharacterNavmesh)
		{
			DrawDebugLine(GetWorld(), start, outLocation, FColor::Blue, false, 1.0f, 0, 10.0f);
			DrawDebugSphere(GetWorld(), outLocation, 50.0f, 12, FColor::Blue, false, 1.0f, 0, 1.0f);
		}
#endif

		return true;
	}

	return false;
}

#if WITH_EDITOR
void UGsQueryFloorHeightComponent::DrawCharacterLocation()
{
	if (CanDrawCharacterNavmesh)
	{
		FVector location = GetOwner()->GetActorLocation();
		FVector floorLocation;

		if (TryGetFloor(location, floorLocation))
		{

		}
	}
}
#endif
