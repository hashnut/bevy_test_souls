#pragma once

#include "NavMesh/RecastNavMesh.h"
#include "AI/Navigation/NavigationTypes.h"
#include "NavigationSystem.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Math/NumericLimits.h"

#include "GsQueryFloorHeightComponent.generated.h"

class AGsCharacterBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class T1PROJECT_API UGsQueryFloorHeightComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	static bool CanDrawCharacterNavmesh;

private:
	FNavLocation				_navResult;
	FHitResult					_floorHit;
	FCollisionQueryParams		_floorCollisionQueryParams;
	FCollisionObjectQueryParams _floorCollisionObjQueryParams;

	FVector						_navLocation;
	FVector						_geometryLocation;
public:
	// Sets default values for this component's properties
	UGsQueryFloorHeightComponent();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Navigation
public:
	//Get navmesh height, if there is no navmesh get height using line trace to land
	bool TryGetFloor(const FVector& inTargetLocation, FVector& outLocation);
	bool TryGetGeometryFloor(const FVector& inLocation, FVector& outLocation);

protected:
	
	bool TryGetNavFloor(const FVector& inLocation, FVector& outLocation);

public:
	//Set character on land from reliable location
	static bool TryGetTeleportLocation(FVector& outLocation, FHitResult& outResult, AGsCharacterBase* inCharacter, const FVector& inDest);
	//Set character on land from max to min height
	static bool TryGetTeleportLocation(FVector& outLocation, FHitResult& outResult, UWorld* inWorld, const FVector& inOrigin, float inCapsuleHalfHeight, float inMinHeight = -TNumericLimits<int32>::Max());

	//Set character on land from reliable location
	static bool TryGetTeleportLocationExceptGameObject(FVector& outLocation, FHitResult& outResult, AGsCharacterBase* inCharacter, const FVector& inDest);
	//Set character on land from max to min height
	static bool TryGetTeleportLocationExceptGameObject(FVector& outLocation, FHitResult& outResult, UWorld* inWorld, const FVector& inOrigin, float inCapsuleHalfHeight, float inMinHeight = -10000.0f);

#if WITH_EDITOR
public:
	void DrawCharacterLocation();
#endif
};
