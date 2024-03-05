// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//--------------------------------------------------------------------
// AIController
//--------------------------------------------------------------------
#include "Navigation/PathFollowingComponent.h"
#include "GameplayTask.h"
#include "Perception/AIPerceptionListenerInterface.h"
#include "GenericTeamAgentInterface.h"
#include "VisualLogger/VisualLoggerDebugSnapshotInterface.h"
//--------------------------------------------------------------------

#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GsPlayerController.generated.h"


//--------------------------------------------------------------------
// AIController
//--------------------------------------------------------------------
class FDebugDisplayInfo;
struct FVisualLogEntry;

#if ENABLE_VISUAL_LOG
struct FVisualLogEntry;
#endif // ENABLE_VISUAL_LOG
struct FPathFindingQuery;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGsPlayerMoveCompletedSignature, FAIRequestID, RequestID, EPathFollowingResult::Type, Result);

//--------------------------------------------------------------------

/**
 * APlayerController 클래스를 상속 받고 
 * AAIController 클래스의 MoveTo 관련 기능을 구현
 */
UCLASS()
class T1PROJECT_API AGsPlayerController : public APlayerController, public IAIPerceptionListenerInterface, public IGameplayTaskOwnerInterface, public IGenericTeamAgentInterface, public IVisualLoggerDebugSnapshotInterface
{
	GENERATED_BODY()
	
//--------------------------------------------------------------------
// AIController
//--------------------------------------------------------------------
public:
	/** used for alternating LineOfSight traces */
	UPROPERTY()
	mutable uint32 bLOSflag : 1;

	/** Skip extra line of sight traces to extremities of target being checked. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	uint32 bSkipExtraLOSChecks : 1;

	/** Is strafing allowed during movement? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	uint32 bAllowStrafe : 1;

	/** Specifies if this AI wants its own PlayerState. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	uint32 bWantsPlayerState : 1;

private:
	/** Component used for moving along a path. */
	UPROPERTY(VisibleDefaultsOnly, Category = AI)
	UPathFollowingComponent* PathFollowingComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	TSubclassOf<UNavigationQueryFilter> DefaultNavigationFilterClass;

public:
	/** Makes AI go toward specified Goal actor (destination will be continuously updated), aborts any active path following
	 *  @param AcceptanceRadius - finish move if pawn gets close enough
	 *  @param bStopOnOverlap - add pawn's radius to AcceptanceRadius
	 *  @param bUsePathfinding - use navigation data to calculate path (otherwise it will go in straight line)
	 *  @param bCanStrafe - set focus related flag: bAllowStrafe
	 *  @param FilterClass - navigation filter for pathfinding adjustments. If none specified DefaultNavigationFilterClass will be used
	 *  @param bAllowPartialPath - use incomplete path when goal can't be reached
	 *	@note AcceptanceRadius has default value or -1 due to Header Parser not being able to recognize UPathFollowingComponent::DefaultAcceptanceRadius
	 */
	UFUNCTION(BlueprintCallable, Category = "AI|Navigation", Meta = (AdvancedDisplay = "bStopOnOverlap,bCanStrafe,bAllowPartialPath"))
	EPathFollowingRequestResult::Type MoveToActor(AActor* Goal, float AcceptanceRadius = -1, bool bStopOnOverlap = true,
			bool bUsePathfinding = true, bool bCanStrafe = true,
			TSubclassOf<UNavigationQueryFilter> FilterClass = NULL, bool bAllowPartialPath = true);

	/** Makes AI go toward specified Dest location, aborts any active path following
	 *  @param AcceptanceRadius - finish move if pawn gets close enough
	 *  @param bStopOnOverlap - add pawn's radius to AcceptanceRadius
	 *  @param bUsePathfinding - use navigation data to calculate path (otherwise it will go in straight line)
	 *  @param bProjectDestinationToNavigation - project location on navigation data before using it
	 *  @param bCanStrafe - set focus related flag: bAllowStrafe
	 *  @param FilterClass - navigation filter for pathfinding adjustments. If none specified DefaultNavigationFilterClass will be used
	 *  @param bAllowPartialPath - use incomplete path when goal can't be reached
	 *	@note AcceptanceRadius has default value or -1 due to Header Parser not being able to recognize UPathFollowingComponent::DefaultAcceptanceRadius
	 */
	UFUNCTION(BlueprintCallable, Category = "AI|Navigation", Meta = (AdvancedDisplay = "bStopOnOverlap,bCanStrafe,bAllowPartialPath"))
	EPathFollowingRequestResult::Type MoveToLocation(const FVector& Dest, float AcceptanceRadius = -1, bool bStopOnOverlap = true,
			bool bUsePathfinding = true, bool bProjectDestinationToNavigation = false, bool bCanStrafe = true,
			TSubclassOf<UNavigationQueryFilter> FilterClass = NULL, bool bAllowPartialPath = true);

	/** Makes AI go toward specified destination
	 *  @param MoveRequest - details about move
	 *  @param OutPath - optional output param, filled in with assigned path
	 *  @return struct holding MoveId and enum code
	 */
	virtual FPathFollowingRequestResult MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath = nullptr, bool InIsIgnoreCost = false);

	/** Passes move request and path object to path following */
	virtual FAIRequestID RequestMove(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr Path);

	/** Finds path for given move request
	 *  @param MoveRequest - details about move
	 *  @param Query - pathfinding query for navigation system
	 *  @param OutPath - generated path
	 */
	virtual void FindPathForMoveRequest(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query, FNavPathSharedPtr& OutPath) const;

	/** Helper function for creating pathfinding query for this agent from move request data */
	bool BuildPathfindingQuery(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query) const;

	bool BuildPathfindingQueryWithStartPos(const FAIMoveRequest& MoveRequest, const FVector& In_startPos, FPathFindingQuery& Query) const;

	UE_DEPRECATED_FORGAME(4.13, "This function is now deprecated, please use FindPathForMoveRequest() for adjusting Query or BuildPathfindingQuery() for getting one.")
	virtual bool PreparePathfinding(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query);

	UE_DEPRECATED_FORGAME(4.13, "This function is now deprecated, please use FindPathForMoveRequest() for adjusting pathfinding or path postprocess.")
	virtual FAIRequestID RequestPathAndMove(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query);

	/** if AI is currently moving due to request given by RequestToPause, then the move will be paused */
	bool PauseMove(FAIRequestID RequestToPause);

	/** resumes last AI-performed, paused request provided it's ID was equivalent to RequestToResume */
	bool ResumeMove(FAIRequestID RequestToResume);

	/** Aborts the move the controller is currently performing */
	virtual void StopMovement() override;

	/** Called on completing current movement request */
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

	/** Returns the Move Request ID for the current move */
	FORCEINLINE FAIRequestID GetCurrentMoveRequestID() const { return GetPathFollowingComponent() ? GetPathFollowingComponent()->GetCurrentRequestId() : FAIRequestID::InvalidRequest; }

	/** Blueprint notification that we've completed the current movement request */
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "MoveCompleted"))
	FGsPlayerMoveCompletedSignature ReceiveMoveCompleted;

	/** Returns status of path following */
	UFUNCTION(BlueprintCallable, Category = "AI|Navigation")
	EPathFollowingStatus::Type GetMoveStatus() const;

	/** Returns true if the current PathFollowingComponent's path is partial (does not reach desired destination). */
	UFUNCTION(BlueprintCallable, Category = "AI|Navigation")
	bool HasPartialPath() const;

	/** Returns position of current path segment's end. */
	UFUNCTION(BlueprintCallable, Category = "AI|Navigation")
	FVector GetImmediateMoveDestination() const;

	/** Updates state of movement block detection. */
	UFUNCTION(BlueprintCallable, Category = "AI|Navigation")
	void SetMoveBlockDetection(bool bEnable);

	virtual void PostInitializeComponents() override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	void SetInitialSpawnLocation(const FVector& inLocation);
public:
	virtual bool ShouldPostponePathUpdates() const override;
	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos) override;

#if ENABLE_VISUAL_LOG
	virtual void GrabDebugSnapshot(FVisualLogEntry* Snapshot) const override;
#endif

	virtual void Reset() override;

	/**
	 * Checks line to center and top of other actor
	 * @param Other is the actor whose visibility is being checked.
	 * @param ViewPoint is eye position visibility is being checked from.  If vect(0,0,0) passed in, uses current viewtarget's eye position.
	 * @param bAlternateChecks used only in AIController implementation
	 * @return true if controller's pawn can see Other actor.
	 */
	virtual bool LineOfSightTo(const AActor* Other, FVector ViewPoint = FVector(ForceInit), bool bAlternateChecks = false) const override;
	//~ End AController Interface
	
	//----------------------------------------------------------------------//
	// INavAgentInterface
	//----------------------------------------------------------------------//
	virtual bool IsFollowingAPath() const override;
	virtual IPathFollowingAgentInterface* GetPathFollowingAgent() const override { return PathFollowingComponent; }

#ifdef TEST_INPUT_TOUCH_LOG
	/** Handles a touch screen action */
	virtual bool InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex) override;
#endif
public:
	//----------------------------------------------------------------------//
	// IGameplayTaskOwnerInterface
	//----------------------------------------------------------------------//
	virtual AActor* GetGameplayTaskOwner(const UGameplayTask* Task) const override { return const_cast<AGsPlayerController*>(this); }
	virtual AActor* GetGameplayTaskAvatar(const UGameplayTask* Task) const override 
	{ 
		return Cast<AActor>(GetPawn());
	}
	virtual uint8 GetGameplayTaskDefaultPriority() const { return FGameplayTasks::DefaultPriority - 1; }

	// add empty overrides to fix linker errors if project implements a child class without adding GameplayTasks module dependency
	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) override {}
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override {}

public:
	/** Returns PathFollowingComponent subobject **/
	UFUNCTION(BlueprintCallable, Category = "AI|Navigation")
	UPathFollowingComponent* GetPathFollowingComponent() const { return PathFollowingComponent; }

	/** Note that his function does not do any pathfollowing state transfer.
	 *	Intended to be called as part of initialization/setup process */
	UFUNCTION(BlueprintCallable, Category = "AI|Navigation")
	void SetPathFollowingComponent(UPathFollowingComponent* NewPFComponent);
//--------------------------------------------------------------------


	bool GetHitResultsAtScreenPosition(const FVector2D ScreenPosition, const TArray<TEnumAsByte<EObjectTypeQuery> >& ObjectTypes, bool bTraceComplex, TArray<FHitResult>& HitResults) const;

public:
	AGsPlayerController();

protected:
	virtual void BeginPlay() override;
};


