// Fill out your copyright notice in the Description page of Project Settings.


#include "GsPlayerController.h"
#include "Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/NavigationSystem/Public/NavMesh/NavMeshPath.h"
#include "DisplayDebugHelpers.h"
#include "NetworkingDistanceConstants.h"

#include "NavigationSystem.h"
#include "Cheat/GsCheatManager.h"
#include "Data/GsDataContainManager.h"

#include "Engine/Classes/GameFramework/HUD.h"

#include "Engine/Classes/Kismet/GameplayStatics.h"

#if ENABLE_VISUAL_LOG
#include "Runtime/Engine/Public/VisualLogger/VisualLogger.h"
#include "Runtime/Engine/Public/VisualLogger/VisualLoggerTypes.h"
#endif // ENABLE_VISUAL_LOG

#ifdef TEST_INPUT_TOUCH_LOG
#include "IXRTrackingSystem.h"
#endif


//----------------------------------------------------------------------//
// AGsPlayerController
//----------------------------------------------------------------------//
DECLARE_CYCLE_STAT(TEXT("MoveTo"), STAT_MoveTo, STATGROUP_AI);


void AGsPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (bWantsPlayerState && !IsPendingKill() && (GetNetMode() != NM_Client))
	{
		InitPlayerState();
	}

#if ENABLE_VISUAL_LOG
	for (UActorComponent* Component : GetComponents())
	{
		if (Component)
		{
			REDIRECT_OBJECT_TO_VLOG(Component, this);
		}
	}
#endif // ENABLE_VISUAL_LOG
}

void AGsPlayerController::Reset()
{
	Super::Reset();

	if (PathFollowingComponent)
	{
		PathFollowingComponent->AbortMove(*this, FPathFollowingResultFlags::OwnerFinished | FPathFollowingResultFlags::ForcedScript);
	}
}

void AGsPlayerController::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos)
{
	Super::DisplayDebug(Canvas, DebugDisplay, YL, YPos);

	static FName NAME_AI = FName(TEXT("AI"));
	if (DebugDisplay.IsDisplayOn(NAME_AI))
	{
		if (PathFollowingComponent)
		{
			PathFollowingComponent->DisplayDebug(Canvas, DebugDisplay, YL, YPos);
		}
	}
}

#if ENABLE_VISUAL_LOG

void AGsPlayerController::GrabDebugSnapshot(FVisualLogEntry* Snapshot) const
{
	FVisualLogStatusCategory MyCategory;
	MyCategory.Category = TEXT("AI Controller");
	MyCategory.Add(TEXT("Pawn"), GetNameSafe(GetPawn()));
	Snapshot->Status.Add(MyCategory);

	if (GetPawn())
	{
		Snapshot->Location = GetPawn()->GetActorLocation();
	}

	if (PathFollowingComponent)
	{
		PathFollowingComponent->DescribeSelfToVisLog(Snapshot);
	}
}
#endif // ENABLE_VISUAL_LOG

bool AGsPlayerController::LineOfSightTo(const AActor* Other, FVector ViewPoint, bool bAlternateChecks) const
{
	if (Other == nullptr)
	{
		return false;
	}

	if (ViewPoint.IsZero())
	{
		FRotator ViewRotation;
		GetActorEyesViewPoint(ViewPoint, ViewRotation);

		// if we still don't have a view point we simply fail
		if (ViewPoint.IsZero())
		{
			return false;
		}
	}

	FVector TargetLocation = Other->GetTargetLocation(GetPawn());

	FCollisionQueryParams CollisionParams(SCENE_QUERY_STAT(LineOfSight), true, this->GetPawn());
	CollisionParams.AddIgnoredActor(Other);

	bool bHit = GetWorld()->LineTraceTestByChannel(ViewPoint, TargetLocation, ECC_Visibility, CollisionParams);
	if (!bHit)
	{
		return true;
	}

	// if other isn't using a cylinder for collision and isn't a Pawn (which already requires an accurate cylinder for AI)
	// then don't go any further as it likely will not be tracing to the correct location
	const APawn * OtherPawn = Cast<const APawn>(Other);
	if (!OtherPawn && Cast<UCapsuleComponent>(Other->GetRootComponent()) == NULL)
	{
		return false;
	}

	const FVector OtherActorLocation = Other->GetActorLocation();
	const float DistSq = (OtherActorLocation - ViewPoint).SizeSquared();
	if (DistSq > FARSIGHTTHRESHOLDSQUARED)
	{
		return false;
	}

	if (!OtherPawn && (DistSq > NEARSIGHTTHRESHOLDSQUARED))
	{
		return false;
	}

	float OtherRadius, OtherHeight;
	Other->GetSimpleCollisionCylinder(OtherRadius, OtherHeight);

	if (!bAlternateChecks || !bLOSflag)
	{
		//try viewpoint to head
		bHit = GetWorld()->LineTraceTestByChannel(ViewPoint, OtherActorLocation + FVector(0.f, 0.f, OtherHeight), ECC_Visibility, CollisionParams);
		if (!bHit)
		{
			return true;
		}
	}

	if (!bSkipExtraLOSChecks && (!bAlternateChecks || bLOSflag))
	{
		// only check sides if width of other is significant compared to distance
		if (OtherRadius * OtherRadius / (OtherActorLocation - ViewPoint).SizeSquared() < 0.0001f)
		{
			return false;
		}
		//try checking sides - look at dist to four side points, and cull furthest and closest
		FVector Points[4];
		Points[0] = OtherActorLocation - FVector(OtherRadius, -1 * OtherRadius, 0);
		Points[1] = OtherActorLocation + FVector(OtherRadius, OtherRadius, 0);
		Points[2] = OtherActorLocation - FVector(OtherRadius, OtherRadius, 0);
		Points[3] = OtherActorLocation + FVector(OtherRadius, -1 * OtherRadius, 0);
		int32 IndexMin = 0;
		int32 IndexMax = 0;
		float CurrentMax = (Points[0] - ViewPoint).SizeSquared();
		float CurrentMin = CurrentMax;
		for (int32 PointIndex = 1; PointIndex < 4; PointIndex++)
		{
			const float NextSize = (Points[PointIndex] - ViewPoint).SizeSquared();
			if (NextSize > CurrentMin)
			{
				CurrentMin = NextSize;
				IndexMax = PointIndex;
			}
			else if (NextSize < CurrentMax)
			{
				CurrentMax = NextSize;
				IndexMin = PointIndex;
			}
		}

		for (int32 PointIndex = 0; PointIndex < 4; PointIndex++)
		{
			if ((PointIndex != IndexMin) && (PointIndex != IndexMax))
			{
				bHit = GetWorld()->LineTraceTestByChannel(ViewPoint, Points[PointIndex], ECC_Visibility, CollisionParams);
				if (!bHit)
				{
					return true;
				}
			}
		}
	}
	return false;
}

DEFINE_LOG_CATEGORY_STATIC(LogTestAI, All, All);

void AGsPlayerController::OnPossess(APawn* InPawn)
{
	// don't even try possessing pending-kill pawns
	if (InPawn != nullptr && InPawn->IsPendingKill())
	{
		return;
	}

	Super::OnPossess(InPawn);

	if (GetPawn() == nullptr || InPawn == nullptr)
	{
		return;
	}

	// not calling UpdateNavigationComponents() anymore. The PathFollowingComponent 
	// is now observing newly possessed pawns (via OnNewPawn)

	if (PathFollowingComponent)
	{
		PathFollowingComponent->Initialize();
	}

	if (bWantsPlayerState)
	{
		ChangeState(NAME_Playing);
	}
}

void AGsPlayerController::OnUnPossess()
{
	APawn* CurrentPawn = GetPawn();

	Super::OnUnPossess();

	if (PathFollowingComponent)
	{
		PathFollowingComponent->Cleanup();
	}
}

//월드 컴포지션을 사용하는 맵에서 플레이어 컨트롤러의 위치를 얻어 맵을 갱신한다.
//스폰 위치를 설정해줘야 한다. 
void AGsPlayerController::SetInitialSpawnLocation(const FVector& inLocation)
{
	Super::SetInitialLocationAndRotation(inLocation, FRotator::ZeroRotator);	
}

EPathFollowingRequestResult::Type AGsPlayerController::MoveToActor(AActor* Goal, float AcceptanceRadius, bool bStopOnOverlap, bool bUsePathfinding, bool bCanStrafe, TSubclassOf<UNavigationQueryFilter> FilterClass, bool bAllowPartialPaths)
{
	// abort active movement to keep only one request running
	if (PathFollowingComponent && PathFollowingComponent->GetStatus() != EPathFollowingStatus::Idle)
	{
		PathFollowingComponent->AbortMove(*this, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
			, FAIRequestID::CurrentRequest, EPathFollowingVelocityMode::Keep);
	}

	FAIMoveRequest MoveReq(Goal);
	MoveReq.SetUsePathfinding(bUsePathfinding);
	MoveReq.SetAllowPartialPath(bAllowPartialPaths);
	MoveReq.SetNavigationFilter(*FilterClass ? FilterClass : DefaultNavigationFilterClass);
	MoveReq.SetAcceptanceRadius(AcceptanceRadius);
	MoveReq.SetReachTestIncludesAgentRadius(bStopOnOverlap);
	MoveReq.SetCanStrafe(bCanStrafe);

	return MoveTo(MoveReq);
}

EPathFollowingRequestResult::Type AGsPlayerController::MoveToLocation(const FVector& Dest, float AcceptanceRadius, bool bStopOnOverlap, bool bUsePathfinding, bool bProjectDestinationToNavigation, bool bCanStrafe, TSubclassOf<UNavigationQueryFilter> FilterClass, bool bAllowPartialPaths)
{
	// abort active movement to keep only one request running
	if (PathFollowingComponent && PathFollowingComponent->GetStatus() != EPathFollowingStatus::Idle)
	{
		PathFollowingComponent->AbortMove(*this, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
			, FAIRequestID::CurrentRequest, EPathFollowingVelocityMode::Keep);
	}

	FAIMoveRequest MoveReq(Dest);
	MoveReq.SetUsePathfinding(bUsePathfinding);
	MoveReq.SetAllowPartialPath(bAllowPartialPaths);
	MoveReq.SetProjectGoalLocation(bProjectDestinationToNavigation);
	MoveReq.SetNavigationFilter(*FilterClass ? FilterClass : DefaultNavigationFilterClass);
	MoveReq.SetAcceptanceRadius(AcceptanceRadius);
	MoveReq.SetReachTestIncludesAgentRadius(bStopOnOverlap);
	MoveReq.SetCanStrafe(bCanStrafe);

	return MoveTo(MoveReq);
}

FPathFollowingRequestResult AGsPlayerController::MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath /* = nullptr */, bool InIsIgnoreCost /* = false */)
{
	// both MoveToActor and MoveToLocation can be called from blueprints/script and should keep only single movement request at the same time.
	// this function is entry point of all movement mechanics - do NOT abort in here, since movement may be handled by AITasks, which support stacking 

	SCOPE_CYCLE_COUNTER(STAT_MoveTo);

	FPathFollowingRequestResult ResultData;
	ResultData.Code = EPathFollowingRequestResult::Failed;

	if (MoveRequest.IsValid() == false)
	{
		return ResultData;
	}

	if (PathFollowingComponent == nullptr)
	{
		return ResultData;
	}

	ensure(MoveRequest.GetNavigationFilter() || !DefaultNavigationFilterClass);

	bool bCanRequestMove = true;
	bool bAlreadyAtGoal = false;

	if (!MoveRequest.IsMoveToActorRequest())
	{
		if (MoveRequest.GetGoalLocation().ContainsNaN() || FAISystem::IsValidLocation(MoveRequest.GetGoalLocation()) == false)
		{
			bCanRequestMove = false;
		}

		// fail if projection to navigation is required but it failed
		if (bCanRequestMove && MoveRequest.IsProjectingGoal())
		{
			UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
			const FNavAgentProperties& AgentProps = GetNavAgentPropertiesRef();
			FNavLocation ProjectedLocation;

			if (NavSys && !NavSys->ProjectPointToNavigation(MoveRequest.GetGoalLocation(), ProjectedLocation, INVALID_NAVEXTENT, &AgentProps))
			{
				bCanRequestMove = false;
			}

			MoveRequest.UpdateGoalLocation(ProjectedLocation.Location);
		}

		bAlreadyAtGoal = bCanRequestMove && PathFollowingComponent->HasReached(MoveRequest);
	}
	else
	{
		bAlreadyAtGoal = bCanRequestMove && PathFollowingComponent->HasReached(MoveRequest);
	}

	if (bAlreadyAtGoal)
	{
		ResultData.MoveId = PathFollowingComponent->RequestMoveWithImmediateFinish(EPathFollowingResult::Success);
		ResultData.Code = EPathFollowingRequestResult::AlreadyAtGoal;
	}
	else if (bCanRequestMove)
	{
		FPathFindingQuery PFQuery;

		const bool bValidQuery = BuildPathfindingQuery(MoveRequest, PFQuery);
		
		if (InIsIgnoreCost)		
		{
			// 2023/8/22 PKT - UNavArea_Road::DefaultCost 참조
			const static float COST_LIMIT = 0.5f;
			PFQuery.CostLimit = COST_LIMIT;
		}

		if (bValidQuery)
		{
			FNavPathSharedPtr Path;
			FindPathForMoveRequest(MoveRequest, PFQuery, Path);

			const FAIRequestID RequestID = Path.IsValid() ? RequestMove(MoveRequest, Path) : FAIRequestID::InvalidRequest;
			if (RequestID.IsValid())
			{
				bAllowStrafe = MoveRequest.CanStrafe();
				ResultData.MoveId = RequestID;
				ResultData.Code = EPathFollowingRequestResult::RequestSuccessful;

				if (OutPath)
				{
					*OutPath = Path;
				}
			}
		}
	}

	if (ResultData.Code == EPathFollowingRequestResult::Failed)
	{
		ResultData.MoveId = PathFollowingComponent->RequestMoveWithImmediateFinish(EPathFollowingResult::Invalid);
	}

	return ResultData;
}

FAIRequestID AGsPlayerController::RequestMove(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr Path)
{
	uint32 RequestID = FAIRequestID::InvalidRequest;
	if (PathFollowingComponent)
	{
		RequestID = PathFollowingComponent->RequestMove(MoveRequest, Path);
	}

	return RequestID;
}

bool AGsPlayerController::PauseMove(FAIRequestID RequestToPause)
{
	if (PathFollowingComponent != NULL && RequestToPause.IsEquivalent(PathFollowingComponent->GetCurrentRequestId()))
	{
		PathFollowingComponent->PauseMove(RequestToPause, EPathFollowingVelocityMode::Reset);
		return true;
	}
	return false;
}

bool AGsPlayerController::ResumeMove(FAIRequestID RequestToResume)
{
	if (PathFollowingComponent != NULL && RequestToResume.IsEquivalent(PathFollowingComponent->GetCurrentRequestId()))
	{
		PathFollowingComponent->ResumeMove(RequestToResume);
		return true;
	}
	return false;
}

void AGsPlayerController::StopMovement()
{
	// @note FPathFollowingResultFlags::ForcedScript added to make AITask_MoveTo instances 
	// not ignore OnRequestFinished notify that's going to be sent out due to this call
	PathFollowingComponent->AbortMove(*this, FPathFollowingResultFlags::MovementStop | FPathFollowingResultFlags::ForcedScript);
}

bool AGsPlayerController::ShouldPostponePathUpdates() const
{
	return GetPathFollowingComponent()->HasStartedNavLinkMove() || Super::ShouldPostponePathUpdates();
}

bool AGsPlayerController::BuildPathfindingQuery(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query) const
{
	bool bResult = false;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	const ANavigationData* NavData = (NavSys == nullptr) ? nullptr :
		MoveRequest.IsUsingPathfinding() ? NavSys->GetNavDataForProps(GetNavAgentPropertiesRef()) :
		NavSys->GetAbstractNavData();

	if (NavData)
	{
		FVector GoalLocation = MoveRequest.GetGoalLocation();
		if (MoveRequest.IsMoveToActorRequest())
		{
			const INavAgentInterface* NavGoal = Cast<const INavAgentInterface>(MoveRequest.GetGoalActor());
			if (NavGoal)
			{
				const FVector Offset = NavGoal->GetMoveGoalOffset(this);
				GoalLocation = FQuatRotationTranslationMatrix(MoveRequest.GetGoalActor()->GetActorQuat(), NavGoal->GetNavAgentLocation()).TransformPosition(Offset);
			}
			else
			{
				GoalLocation = MoveRequest.GetGoalActor()->GetActorLocation();
			}
		}

		FSharedConstNavQueryFilter NavFilter = UNavigationQueryFilter::GetQueryFilter(*NavData, this, MoveRequest.GetNavigationFilter());
		Query = FPathFindingQuery(*this, *NavData, GetNavAgentLocation(), GoalLocation, NavFilter);
		Query.SetAllowPartialPaths(MoveRequest.IsUsingPartialPaths());

		if (PathFollowingComponent)
		{
			PathFollowingComponent->OnPathfindingQuery(Query);
		}

		bResult = true;
	}

	return bResult;
}

bool AGsPlayerController::BuildPathfindingQueryWithStartPos(const FAIMoveRequest& MoveRequest,  const FVector& In_startPos,
	FPathFindingQuery& Query) const
{
	bool bResult = false;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	const ANavigationData* NavData = (NavSys == nullptr) ? nullptr :
		MoveRequest.IsUsingPathfinding() ? NavSys->GetNavDataForProps(GetNavAgentPropertiesRef()) :
		NavSys->GetAbstractNavData();

	if (NavData)
	{
		FVector GoalLocation = MoveRequest.GetGoalLocation();
		if (MoveRequest.IsMoveToActorRequest())
		{
			const INavAgentInterface* NavGoal = Cast<const INavAgentInterface>(MoveRequest.GetGoalActor());
			if (NavGoal)
			{
				const FVector Offset = NavGoal->GetMoveGoalOffset(this);
				GoalLocation = FQuatRotationTranslationMatrix(MoveRequest.GetGoalActor()->GetActorQuat(), NavGoal->GetNavAgentLocation()).TransformPosition(Offset);
			}
			else
			{
				GoalLocation = MoveRequest.GetGoalActor()->GetActorLocation();
			}
		}

		FSharedConstNavQueryFilter NavFilter = UNavigationQueryFilter::GetQueryFilter(*NavData, this, MoveRequest.GetNavigationFilter());
		Query = FPathFindingQuery(*this, *NavData, In_startPos, GoalLocation, NavFilter);
		Query.SetAllowPartialPaths(MoveRequest.IsUsingPartialPaths());

		if (PathFollowingComponent)
		{
			PathFollowingComponent->OnPathfindingQuery(Query);
		}

		bResult = true;
	}

	return bResult;
}

void AGsPlayerController::FindPathForMoveRequest(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query, FNavPathSharedPtr& OutPath) const
{
	SCOPE_CYCLE_COUNTER(STAT_AI_Overall);

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys)
	{
		FPathFindingResult PathResult = NavSys->FindPathSync(Query);
		if (PathResult.Result != ENavigationQueryResult::Error)
		{
			if (PathResult.IsSuccessful() && PathResult.Path.IsValid())
			{
				if (MoveRequest.IsMoveToActorRequest())
				{
					PathResult.Path->SetGoalActorObservation(*MoveRequest.GetGoalActor(), 100.0f);
				}

				PathResult.Path->EnableRecalculationOnInvalidation(true);

				if (UGsDataContainManager* dataContainerManager = GData())
				{
					if (const UGsGlobalConstant* globalConstant = dataContainerManager->GetGlobalData())
					{
						((FNavMeshPath*)PathResult.Path.Get())->OffsetFromCorners(globalConstant->_offsetFromCornersDistance);
					}					
				}
				OutPath = PathResult.Path;
			}
		}
	}
}

// DEPRECATED FUNCTION SUPPORT
bool AGsPlayerController::PreparePathfinding(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query)
{
	return BuildPathfindingQuery(MoveRequest, Query);
}

// DEPRECATED FUNCTION SUPPORT
FAIRequestID AGsPlayerController::RequestPathAndMove(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query)
{
	FAIRequestID MoveId = FAIRequestID::InvalidRequest;

	FNavPathSharedPtr FoundPath;
	FindPathForMoveRequest(MoveRequest, Query, FoundPath);

	if (FoundPath.IsValid())
	{
		MoveId = RequestMove(MoveRequest, FoundPath);
	}

	return MoveId;
}

EPathFollowingStatus::Type AGsPlayerController::GetMoveStatus() const
{
	return (PathFollowingComponent) ? PathFollowingComponent->GetStatus() : EPathFollowingStatus::Idle;
}

bool AGsPlayerController::HasPartialPath() const
{
	return (PathFollowingComponent != NULL) && (PathFollowingComponent->HasPartialPath());
}

bool AGsPlayerController::IsFollowingAPath() const
{
	return (PathFollowingComponent != nullptr) && (PathFollowingComponent->GetStatus() != EPathFollowingStatus::Idle);
}

FVector AGsPlayerController::GetImmediateMoveDestination() const
{
	return (PathFollowingComponent) ? PathFollowingComponent->GetCurrentTargetLocation() : FVector::ZeroVector;
}

void AGsPlayerController::SetMoveBlockDetection(bool bEnable)
{
	if (PathFollowingComponent)
	{
		PathFollowingComponent->SetBlockDetectionState(bEnable);
	}
}

void AGsPlayerController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	ReceiveMoveCompleted.Broadcast(RequestID, Result.Code);
}

void AGsPlayerController::SetPathFollowingComponent(UPathFollowingComponent* NewPFComponent)
{
	PathFollowingComponent = NewPFComponent;
#if ENABLE_VISUAL_LOG
	if (NewPFComponent)
	{
		REDIRECT_OBJECT_TO_VLOG(NewPFComponent, this);
	}
#endif // ENABLE_VISUAL_LOG
}
//--------------------------------------------------------------------


AGsPlayerController::AGsPlayerController()
{
	CheatClass = UGsCheatManager::StaticClass();

	//--------------------------------------------------------------------
	// AIController
	//--------------------------------------------------------------------
	PathFollowingComponent = CreateDefaultSubobject<UPathFollowingComponent>(TEXT("PathFollowingComponent"));
	PathFollowingComponent->OnRequestFinished.AddUObject(this, &AGsPlayerController::OnMoveCompleted);

	bSkipExtraLOSChecks = true;
	bWantsPlayerState = false;
	//--------------------------------------------------------------------
}

void AGsPlayerController::BeginPlay()
{
	Super::BeginPlay();
}
#ifdef TEST_INPUT_TOUCH_LOG
/** Handles a touch screen action */
bool AGsPlayerController::InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex)
{
	if (GEngine->XRSystem.IsValid())
	{
		auto XRInput = GEngine->XRSystem->GetXRInput();
		if (XRInput && XRInput->HandleInputTouch(Handle, Type, TouchLocation, DeviceTimestamp, TouchpadIndex))
		{
			return true;
		}
	}

	bool bResult = false;
	if (PlayerInput)
	{
		bResult = PlayerInput->InputTouch(Handle, Type, TouchLocation, Force, DeviceTimestamp, TouchpadIndex);

		if (bEnableTouchEvents || bEnableTouchOverEvents)
		{
			const ETouchIndex::Type FingerIndex = ETouchIndex::Type(Handle);

			FVector WorldOrigin;
			FVector WorldDirection;
			if (UGameplayStatics::DeprojectScreenToWorld(this, TouchLocation, WorldOrigin, WorldDirection) == true)
			{
				DrawDebugDirectionalArrow(GetWorld(), WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance, 20.f, FColor::Red, false, 10.f);
			}

			FHitResult HitResult;
			const bool bHit = GetHitResultAtScreenPosition(TouchLocation, CurrentClickTraceChannel, true, HitResult);

			UPrimitiveComponent* PreviousComponent = CurrentTouchablePrimitives[Handle].Get();
			UPrimitiveComponent* CurrentComponent = (bHit ? HitResult.Component.Get() : NULL);

			if (GetHUD())
			{
				if (Type == ETouchType::Began || Type == ETouchType::Ended)
				{
					if (GetHUD()->UpdateAndDispatchHitBoxClickEvents(TouchLocation, (Type == ETouchType::Began ? EInputEvent::IE_Pressed : EInputEvent::IE_Released)))
					{
						CurrentComponent = NULL;
					}
				}
			}

			switch (Type)
			{
			case ETouchType::Began:
				// Give it a begin touch
				if (bEnableTouchEvents && (CurrentComponent != NULL))
				{
					CurrentComponent->DispatchOnInputTouchBegin(FingerIndex);
				}

				// Give a touch enter event
				if (bEnableTouchOverEvents)
				{
					UPrimitiveComponent::DispatchTouchOverEvents(FingerIndex, PreviousComponent, CurrentComponent);
					CurrentTouchablePrimitives[Handle] = CurrentComponent;
				}
				break;
			case ETouchType::Ended:
				// Give it a touch exit
				if (bEnableTouchEvents && (CurrentComponent != NULL))
				{
					CurrentComponent->DispatchOnInputTouchEnd(FingerIndex);
				}

				// Give it a end touch
				if (bEnableTouchOverEvents)
				{
					// Handle the case where the finger moved faster than tick, and is being released over a different component than it was last dragged over
					if ((PreviousComponent != CurrentComponent) && (PreviousComponent != NULL))
					{
						// First notify the old component that the touch left it to go to the current component
						UPrimitiveComponent::DispatchTouchOverEvents(FingerIndex, PreviousComponent, CurrentComponent);
					}

					// Now notify that the current component is being released and thus the touch is leaving it
					PreviousComponent = CurrentComponent;
					CurrentComponent = NULL;
					UPrimitiveComponent::DispatchTouchOverEvents(FingerIndex, PreviousComponent, CurrentComponent);
					CurrentTouchablePrimitives[Handle] = CurrentComponent;
				}
				break;
			default:
				break;
			};
		}
	}

	return bResult;
}
#endif

bool AGsPlayerController::GetHitResultsAtScreenPosition(const FVector2D ScreenPosition, const TArray<TEnumAsByte<EObjectTypeQuery> >& ObjectTypes, bool bTraceComplex, TArray<FHitResult>& HitResults) const
{
	// Early out if we clicked on a HUD hitbox
	if (GetHUD() != NULL && GetHUD()->GetHitBoxAtCoordinates(ScreenPosition, true))
	{
		return false;
	}

	FVector WorldOrigin;
	FVector WorldDirection;
	if (UGameplayStatics::DeprojectScreenToWorld(this, ScreenPosition, WorldOrigin, WorldDirection) == true)
	{
		FCollisionObjectQueryParams const ObjParam(ObjectTypes);
		return GetWorld()->LineTraceMultiByObjectType(HitResults, WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance, ObjParam, FCollisionQueryParams(SCENE_QUERY_STAT(ClickableTrace), bTraceComplex));
	}

	return false;
}