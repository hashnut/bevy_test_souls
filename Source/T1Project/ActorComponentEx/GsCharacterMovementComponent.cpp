#include "GsCharacterMovementComponent.h"
#include "Misc/ConfigCacheIni.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "ActorEx/GsCharacterBase.h"
// for cheat gs.CurrentFloor
#if WITH_EDITOR
#include "Runtime/Core/Public/HAL/IConsoleManager.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#endif

void UGsCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ini 파일에서 설정값 얻어오기
	float navMeshAgentMaxSlope = 0.f;
	float navMeshAgentMaxStepHeight = 0.f;
	GConfig->GetFloat(TEXT("/Script/NavigationSystem.RecastNavMesh"), TEXT("AgentMaxSlope"),
		navMeshAgentMaxSlope, GEngineIni);
	GConfig->GetFloat(TEXT("/Script/NavigationSystem.RecastNavMesh"), TEXT("AgentMaxStepHeight"),
		navMeshAgentMaxStepHeight, GEngineIni);

	SetWalkableFloorAngle(navMeshAgentMaxSlope);
	MaxStepHeight = navMeshAgentMaxStepHeight;

	if (UpdatedPrimitive)
	{
		UpdatedPrimitive->bReturnMaterialOnMove = true;
	}

	_owner = Cast<AGsCharacterBase>(CharacterOwner);
}

bool UGsCharacterMovementComponent::IsWalkable(const FHitResult& Hit) const
{
	// 캐릭터끼리 부딪힌 거라면 pass
	if (Hit.GetActor() && Hit.GetActor()->IsA(ACharacter::StaticClass()))
		return false;

	return Super::IsWalkable(Hit);
}

void UGsCharacterMovementComponent::ResetCheckOverlapActorCount()
{
	_prevCount = -1;
}

//
void UGsCharacterMovementComponent::CheckOverlapActor()
{
	if (nullptr == _owner || false == _owner->OnOverlapedCharacterBase.IsBound())
	{
		return;
	}

	TArray<AActor*> OverlappingActors;
	CharacterOwner->GetOverlappingActors(OverlappingActors, nullptr);
	int count = 0;
	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor->IsA(AGsCharacterBase::StaticClass()))
		{
			count++;
		}
	}

	if (_prevCount != count)
	{
		_owner->OnOverlapedCharacterBase.ExecuteIfBound(count);
		_prevCount = count;
	}
}

void UGsCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	_bMoved = !(FVector2D(UpdatedComponent->GetComponentLocation() - OldLocation).IsNearlyZero());
}

bool UGsCharacterMovementComponent::IsMoved() const
{
	return _bMoved;
}





//void UGsCharacterMovementComponent::ComputeFloorDist(const FVector& CapsuleLocation, float LineDistance, float SweepDistance, FFindFloorResult& OutFloorResult, float SweepRadius, const FHitResult* DownwardSweepResult) const
//{
//	OutFloorResult.Clear();
//
//	float PawnRadius, PawnHalfHeight;
//	CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleSize(PawnRadius, PawnHalfHeight);
//
//	bool bSkipSweep = false;
//	if (DownwardSweepResult != NULL && DownwardSweepResult->IsValidBlockingHit())
//	{
//		// Only if the supplied sweep was vertical and downward.
//		if ((DownwardSweepResult->TraceStart.Z > DownwardSweepResult->TraceEnd.Z) &&
//			(DownwardSweepResult->TraceStart - DownwardSweepResult->TraceEnd).SizeSquared2D() <= KINDA_SMALL_NUMBER)
//		{
//			// Reject hits that are barely on the cusp of the radius of the capsule
//			if (IsWithinEdgeTolerance(DownwardSweepResult->Location, DownwardSweepResult->ImpactPoint, PawnRadius))
//			{
//				// Don't try a redundant sweep, regardless of whether this sweep is usable.
//				bSkipSweep = true;
//
//				const bool bIsWalkable = IsWalkable(*DownwardSweepResult);
//				const float FloorDist = (CapsuleLocation.Z - DownwardSweepResult->Location.Z);
//				OutFloorResult.SetFromSweep(*DownwardSweepResult, FloorDist, bIsWalkable);
//
//				if (bIsWalkable)
//				{
//					// Use the supplied downward sweep as the floor hit result.			
//					return;
//				}
//			}
//		}
//	}
//
//	// We require the sweep distance to be >= the line distance, otherwise the HitResult can't be interpreted as the sweep result.
//	if (SweepDistance < LineDistance)
//	{
//		ensure(SweepDistance >= LineDistance);
//		return;
//	}
//
//	bool bBlockingHit = false;
//	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(ComputeFloorDist), false, CharacterOwner);
//	FCollisionResponseParams ResponseParam;
//	InitCollisionParams(QueryParams, ResponseParam);
//	QueryParams.bReturnPhysicalMaterial = true;
//	const ECollisionChannel CollisionChannel = UpdatedComponent->GetCollisionObjectType();
//
//	// Sweep test
//	if (!bSkipSweep && SweepDistance > 0.f && SweepRadius > 0.f)
//	{
//		// Use a shorter height to avoid sweeps giving weird results if we start on a surface.
//		// This also allows us to adjust out of penetrations.
//		const float ShrinkScale = 0.9f;
//		const float ShrinkScaleOverlap = 0.1f;
//		float ShrinkHeight = (PawnHalfHeight - PawnRadius) * (1.f - ShrinkScale);
//		float TraceDist = SweepDistance + ShrinkHeight;
//		FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(SweepRadius, PawnHalfHeight - ShrinkHeight);
//
//		FHitResult Hit(1.f);
//		bBlockingHit = FloorSweepTest(Hit, CapsuleLocation, CapsuleLocation + FVector(0.f, 0.f, -TraceDist), CollisionChannel, CapsuleShape, QueryParams, ResponseParam);
//
//		if (bBlockingHit)
//		{
//			// Reject hits adjacent to us, we only care about hits on the bottom portion of our capsule.
//			// Check 2D distance to impact point, reject if within a tolerance from radius.
//			if (Hit.bStartPenetrating || !IsWithinEdgeTolerance(CapsuleLocation, Hit.ImpactPoint, CapsuleShape.Capsule.Radius))
//			{
//				// Use a capsule with a slightly smaller radius and shorter height to avoid the adjacent object.
//				// Capsule must not be nearly zero or the trace will fall back to a line trace from the start point and have the wrong length.
//				CapsuleShape.Capsule.Radius = FMath::Max(0.f, CapsuleShape.Capsule.Radius - SWEEP_EDGE_REJECT_DISTANCE - KINDA_SMALL_NUMBER);
//				if (!CapsuleShape.IsNearlyZero())
//				{
//					ShrinkHeight = (PawnHalfHeight - PawnRadius) * (1.f - ShrinkScaleOverlap);
//					TraceDist = SweepDistance + ShrinkHeight;
//					CapsuleShape.Capsule.HalfHeight = FMath::Max(PawnHalfHeight - ShrinkHeight, CapsuleShape.Capsule.Radius);
//					Hit.Reset(1.f, false);
//
//					bBlockingHit = FloorSweepTest(Hit, CapsuleLocation, CapsuleLocation + FVector(0.f, 0.f, -TraceDist), CollisionChannel, CapsuleShape, QueryParams, ResponseParam);
//				}
//			}
//
//			// Reduce hit distance by ShrinkHeight because we shrank the capsule for the trace.
//			// We allow negative distances here, because this allows us to pull out of penetrations.
//			const float MaxPenetrationAdjust = FMath::Max(MAX_FLOOR_DIST, PawnRadius);
//			const float SweepResult = FMath::Max(-MaxPenetrationAdjust, Hit.Time * TraceDist - ShrinkHeight);
//
//			OutFloorResult.SetFromSweep(Hit, SweepResult, false);
//			if (Hit.IsValidBlockingHit() && IsWalkable(Hit))
//			{
//				if (SweepResult <= SweepDistance)
//				{
//					// Hit within test distance.
//					OutFloorResult.bWalkableFloor = true;
//					return;
//				}
//			}
//		}
//	}
//
//	// Since we require a longer sweep than line trace, we don't want to run the line trace if the sweep missed everything.
//	// We do however want to try a line trace if the sweep was stuck in penetration.
//	if (!OutFloorResult.bBlockingHit && !OutFloorResult.HitResult.bStartPenetrating)
//	{
//		OutFloorResult.FloorDist = SweepDistance;
//		return;
//	}
//
//	// Line trace
//	if (LineDistance > 0.f)
//	{
//		const float ShrinkHeight = PawnHalfHeight;
//		const FVector LineTraceStart = CapsuleLocation;
//		const float TraceDist = LineDistance + ShrinkHeight;
//		const FVector Down = FVector(0.f, 0.f, -TraceDist);
//		QueryParams.TraceTag = SCENE_QUERY_STAT_NAME_ONLY(FloorLineTrace);
//
//		FHitResult Hit(1.f);
//		bBlockingHit = GetWorld()->LineTraceSingleByChannel(Hit, LineTraceStart, LineTraceStart + Down, CollisionChannel, QueryParams, ResponseParam);
//
//		if (bBlockingHit)
//		{
//			if (Hit.Time > 0.f)
//			{
//				// Reduce hit distance by ShrinkHeight because we started the trace higher than the base.
//				// We allow negative distances here, because this allows us to pull out of penetrations.
//				const float MaxPenetrationAdjust = FMath::Max(MAX_FLOOR_DIST, PawnRadius);
//				const float LineResult = FMath::Max(-MaxPenetrationAdjust, Hit.Time * TraceDist - ShrinkHeight);
//
//				OutFloorResult.bBlockingHit = true;
//				if (LineResult <= LineDistance && IsWalkable(Hit))
//				{
//					OutFloorResult.SetFromLineTrace(Hit, OutFloorResult.FloorDist, LineResult, true);
//					return;
//				}
//			}
//		}
//	}
//
//	// No hits were acceptable.
//	OutFloorResult.bWalkableFloor = false;
//}

// for cheat gs.CurrentFloor
#if WITH_EDITOR
#define GS_CHEAT_NAME "gs.CurrentFloor"

namespace
{
	bool bHudStatsEnabledCVar = false;
	FAutoConsoleVariableRef CVarHudStats(
		TEXT(GS_CHEAT_NAME),
		bHudStatsEnabledCVar,
		TEXT("Whether to show " GS_CHEAT_NAME " stats on HUD"),
		ECVF_Cheat
	);
}
#endif

void UGsCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckOverlapActor();
	

#if WITH_EDITOR
	if (!bHudStatsEnabledCVar)
	{
		return;
	}
	// bak1210
	static const int32 DebugMessageKeyName = FName(TEXT(GS_CHEAT_NAME)).GetNumber();
	const FString ActorName = AActor::GetDebugName(CurrentFloor.HitResult.GetActor());
	GEngine->AddOnScreenDebugMessage(DebugMessageKeyName, 0.f, FColor::Green, *FString::Printf(TEXT(GS_CHEAT_NAME ": %s"), *ActorName), false);
#undef GS_CHEAT_NAME
#endif
}



