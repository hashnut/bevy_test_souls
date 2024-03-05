#include "GsMove.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"

#include "ControllerEx/GsPlayerController.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "ActorComponentEx/GsQueryFloorHeightComponent.h"

#include "ControllerEx/GsPlayerController.h"

#include "AIModule/Classes/AITypes.h"
#include "AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "NavigationSystem/Public/NavigationSystemTypes.h"
#include "NavigationSystem/Public/NavigationData.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavMesh/RecastNavMesh.h"
#include "Engine/Classes/Engine/EngineTypes.h"

#include "DrawDebugHelpers.h"

// 이동 가능 한 곳인가
bool FGsMove::IsMovePossible(AGsPlayerController* In_controller, 
	AGsCharacterLocalPlayer* In_localChar,
	const FVector& In_destPos, const FVector& In_startPos,
	float In_acceptanceRadius, 
	bool In_isTestDrawLine)
{
	if (nullptr == In_controller)
	{
		return false;
	}
	// z값 보정 값(높게 떠있는 경우도 있어서)
	FVector calcZDestPos = In_destPos;
	FVector calcZStartPos = In_startPos;
	if (In_localChar)
	{
		// 목적 좌표 높이 보정
		In_localChar->GetQueryFloorHeightComponent()->TryGetFloor(In_destPos, calcZDestPos);
		In_localChar->GetQueryFloorHeightComponent()->TryGetFloor(In_startPos, calcZStartPos);
	}
	// check pos on 

	bool bCanRequestMove = true;
	if (calcZDestPos.ContainsNaN() || FAISystem::IsValidLocation(calcZDestPos) == false)
	{
		bCanRequestMove = false;
	}

	// fail if projection to navigation is required but it failed
	if (bCanRequestMove)
	{
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(In_localChar->GetWorld());
		const FNavAgentProperties& AgentProps = In_controller->GetNavAgentPropertiesRef();
		FNavLocation ProjectedLocation;

		if (NavSys && !NavSys->ProjectPointToNavigation(calcZDestPos, ProjectedLocation, INVALID_NAVEXTENT, &AgentProps))
		{
			bCanRequestMove = false;
		}

	}

	if(bCanRequestMove == false)
	{
		return false;
	}


	//네비 메쉬로 이동 가능한가
	FAIMoveRequest moveRequest;
	moveRequest.SetGoalLocation(calcZDestPos);
	moveRequest.SetAcceptanceRadius(In_acceptanceRadius);
	moveRequest.SetAllowPartialPath(false);

	FPathFindingQuery PFQuery;

	const bool bValidQuery = In_controller->BuildPathfindingQueryWithStartPos(
		moveRequest, calcZStartPos, PFQuery);

	bool isMovePossible = false;
	if (bValidQuery)
	{
		FNavPathSharedPtr Path;
		In_controller->FindPathForMoveRequest(moveRequest, PFQuery, Path);

		if (Path.IsValid())
		{
			isMovePossible = true;

			if (In_isTestDrawLine == true)
			{
				DrawPathLine(In_localChar->GetWorld() ,Path, 10.0f, FColor::Yellow);
			}

		}
	}
	return isMovePossible;
}

// check path move arrived
bool FGsMove::IsAlmostPathMoveArrived(AGsPlayerController* In_controller,
	const FAIMoveRequest& In_requestMoveData, const FGsMovementStateAuto& In_moveState, float In_addAcceptanceRadius)
{
	if (In_controller == nullptr)
	{
		return false;
	}

	IPathFollowingAgentInterface* pathInterface = In_controller->GetPathFollowingAgent();
	if (nullptr == pathInterface)
	{
		return false;
	}
	UPathFollowingComponent* pathComponent = Cast<UPathFollowingComponent>(pathInterface);

	if (nullptr == pathComponent)
	{
		return false;
	}

	uint32 currentIndex = pathComponent->GetCurrentPathIndex();
	FNavPathSharedPtr navPathPtr = pathComponent->GetPath();
	if (navPathPtr.IsValid() == false)
	{
		return false;
	}

	int lastIndex = navPathPtr->GetPathPoints().Num() - 1;

	FBasedPosition lastPosInfo = navPathPtr->GetPathPointLocation(lastIndex);
	// 거의 다와가면
	return In_moveState.HasReached(lastPosInfo.Position, In_requestMoveData.GetAcceptanceRadius() + In_addAcceptanceRadius);
}

//bool FGsMove::GetNearMovePossiblePos(const FVector& In_originGoal, FVector& Out_resultPos)
//{
//	UWorld* world = GSGameObject()->GetWorld();
//	if (world == nullptr)
//	{
//		return false;
//	}
//	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(world);
//
//	if (NavSys == nullptr)
//	{
//		return false;
//	}
//	UGsDataContainManager* dataManager = GData();
//	if (dataManager == nullptr)
//	{
//		return false;
//	}
//
//	const UGsGlobalConstant* globalData = dataManager->GetGlobalData();
//	if (globalData == nullptr)
//	{
//		return false;
//	}
//	float randomRadius = globalData->_findNearPosRandomRadius;
//
//	FNavLocation findLocation;
//	if (false == NavSys->GetRandomPointInNavigableRadius(In_originGoal, randomRadius, findLocation))
//	{
//		return false;
//	}
//
//	Out_resultPos = findLocation.Location;
//	return true;
//}

bool FGsMove::GetNearMovePossiblePos(const FVector& In_originGoal, FVector& Out_resultPos)
{
	UWorld* world = GSGameObject()->GetWorld();
	if (world == nullptr)
	{
		return false;
	}
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(world);

	if (NavSys == nullptr)
	{
		return false;
	}
	UGsDataContainManager* dataManager = GData();
	if (dataManager == nullptr)
	{
		return false;
	}
	ANavigationData* navData = NavSys->GetDefaultNavDataInstance();
	if (navData == nullptr)
	{
		return false;
	}
	ARecastNavMesh* recastMesh = static_cast<ARecastNavMesh*>(navData);
	if (recastMesh == nullptr)
	{
		return false;
	}

	const UGsGlobalConstant* globalData = dataManager->GetGlobalData();
	if (globalData == nullptr)
	{
		return false;
	}
	FVector extend = globalData->_findNearPosExtend;	
	NavNodeRef nodeRef= recastMesh->FindNearestPoly(In_originGoal, extend);

	if(recastMesh->GetPolyCenter(nodeRef, Out_resultPos) == false)
	{
		return false;
	}	
	return true;
}

void FGsMove::DrawPathLine(UWorld* In_world, 
	FNavPathSharedPtr In_pathSharedPtr, float In_second,  FColor In_color)
{
	if (In_pathSharedPtr.IsValid() == false)
	{
		return;
	}

	TArray<FNavPathPoint> pathPoints = In_pathSharedPtr->GetPathPoints();
	FVector oldPos = FVector::ZeroVector;
	for (auto iter : pathPoints)
	{
		FVector newPos = iter.Location;
		if (oldPos.IsZero())
		{
			oldPos = newPos;
			continue;
		}

		DrawDebugLine(In_world, oldPos, newPos, In_color, false, In_second);
		oldPos = newPos;
	}
}