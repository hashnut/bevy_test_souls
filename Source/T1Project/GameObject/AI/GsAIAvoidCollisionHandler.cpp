#include "GsAIAvoidCollisionHandler.h"
#include "../Data/GsGlobalConstant.h"
#include "../Data/GsDataContainManager.h"
#include "../Management/ScopeGame/GsAIManager.h"
#include "../Management/ScopeGlobal/GsLevelManager.h"
#include "../Movement/GsMovementHandlerLocalPlayer.h"
#include "../AI/Quest/GsAIQuestHandler.h"
#include "../ObjectClass/GsGameObjectBase.h"
#include "../ObjectClass/GsGameObjectLocalPlayer.h"
#include "../Interaction/GsInteractionHandlerLocalPlayer.h"
#include "../ActorEx/GsCharacterBase.h"
#include "../ActorComponentEx/GsQueryFloorHeightComponent.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "NavQueryFilter.h"
#include "NavigationSystem/Public/NavigationData.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavMesh/RecastNavMesh.h"
#include "AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "../AI/Function/GsAIFunc.h"

#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif


void FGsAIAvoidCollisionHandler::Initialize(UGsGameObjectBase* In_local)
{
	Reset();

	_localPlayer = Cast<UGsGameObjectLocalPlayer>(In_local);
}

void FGsAIAvoidCollisionHandler::Finalize()
{
	Reset();

	_localPlayer = nullptr;
}

void FGsAIAvoidCollisionHandler::Update(float inDeltaTime)
{
	if (false == _isAvoidingCollision)
	{
		return;
	}

	_elapsedTime -= inDeltaTime;
	if (0.f >= _elapsedTime)
	{	// 2023/10/13 PKT - 회피 기동이 일정 시간을 지났음에도 계속 동작 중이라면 실패 처리 해버린다.
		OnFailedAvoidanceMode(FPathFollowingResult());
	}
}

void FGsAIAvoidCollisionHandler::BegineDetector()
{
	Reset();

	// 2023/10/13 PKT - 충돌 회피 기능 On
	if (_localPlayer)
	{
		_localPlayer->SetPawnDetectionCollisionEnable(true);
	}
}

void FGsAIAvoidCollisionHandler::EndDetector()
{
	Reset();

	// 2023/10/13 PKT - 충돌 회피 기능 Off
	if (_localPlayer)
	{
		_localPlayer->SetPawnDetectionCollisionEnable(false);
	}	
}

void FGsAIAvoidCollisionHandler::Reset()
{
	_orgMoveInfo.Clear();

	_isAvoidingCollision = false;

	_OrgContentsType = EGsMovementAutoContentsType::None;

	_elapsedTime = 0.f;

	if (_localPlayer)
	{
		_localPlayer->ClearPawnDetectionCollisionOverlaps();
	}	
}


bool FGsAIAvoidCollisionHandler::IsDestinationReached(const FVector& InDest)
{
	if (true == IsMoveToInteractWithNPC())
	{
		return HasArrivedAtDestinationInteractWithNPC();
	}

	return HasArrivedAtDestinationQuestZone(InDest);
}

bool FGsAIAvoidCollisionHandler::IsMoveToInteractWithNPC()
{
	if (nullptr == _localPlayer || false == _orgMoveInfo.IsValid())
	{
		return false;
	}

	if (FGsAIManager* aiMgr = GSAI())
	{
		if (FGsAIQuestHandler* aiQuestHandler = aiMgr->GetAIQuestHandler())
		{
			QuestObjectiveType questObjectType = aiQuestHandler->GetQuestAutoMoveType();

			if (questObjectType == QuestObjectiveType::INTERACT_NPC)
			{
				if (FGsInteractionHandlerLocalPlayer* interactionHandler = _localPlayer->GetCastInteraction<FGsInteractionHandlerLocalPlayer>())
				{
					EGsInteractionMoveType interactionMoveType = interactionHandler->GetReserveMoveType();
					if (interactionMoveType == EGsInteractionMoveType::TargetMove)
					{
						return true;
					}
				}
			}

		}
	}

	return false;
}

bool FGsAIAvoidCollisionHandler::HasArrivedAtDestinationInteractWithNPC()
{
	if (nullptr == _localPlayer)
	{
		return false;
	}

	if (FGsInteractionHandlerLocalPlayer* interactionHandler = _localPlayer->GetCastInteraction<FGsInteractionHandlerLocalPlayer>())
	{
		UGsGameObjectBase* interactionTarget = interactionHandler->GetInteractionTarget();
		if (interactionTarget == nullptr)
		{
			return false;
		}
		const FVector& localLocation = _localPlayer->GetLocation();
		const FVector& targetLocation = interactionTarget->GetLocation();

		float acceptanceRadius = interactionHandler->GetInteractionRangeReserveNpcData();

		float dist = FVector::Dist(targetLocation, localLocation);
		return (acceptanceRadius >= dist);
	}
	return false;
}

bool FGsAIAvoidCollisionHandler::HasArrivedAtDestinationQuestZone(const FVector& InGoalPos)
{
	if (nullptr == _localPlayer)
	{
		return false;
	}

	const FVector& localLocation = _localPlayer->GetLocation();

	const UGsGlobalConstant* globalData = GData()->GetGlobalData();
	float limit_dist = (nullptr == globalData) ? 200.f : globalData->_distanceToQuestGoalArea;

	FGsMovementHandlerLocalPlayer* movementHandler = _localPlayer->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (nullptr == movementHandler)
	{
		return false;
	}
	
	if (movementHandler->GetStateAutoMove().GetMoveToMapId() != GLevel()->GetCurrentLevelId())
	{
		// 2023/6/14 PKT - 목표 지역 Map Id가 현재 이동 중인 MapId가 다르다면.. 이건 워프 이동이 목적
		//					워프 이동이라면 좀더 타이트 하게 dist를 잡아 준다.
		static const float PORTAL_DIST = 40.f;
		limit_dist = PORTAL_DIST;
	}

#if WITH_EDITOR
	DrawDebugSphere(_localPlayer->GetWorld(), InGoalPos, limit_dist, 8, FColor::Red, false, 10.0f);
#endif

	float dist = FVector::Dist(InGoalPos, localLocation);
	return (limit_dist >= dist);
}

bool FGsAIAvoidCollisionHandler::ReversgDestination(const FGsAutoMoveInfo& InOrgMoveInfo, EGsMovementAutoContentsType InMoveAutoContentsType, bool InIsOtherMap)
{
	if (false == InOrgMoveInfo.IsValid())
	{
		return false;
	}

	// 2023/10/13 PKT - 본래 목적지로 다시 세팅 해줘야 한다.
	FGsMovementHandlerLocalPlayer* movementHandler = _localPlayer->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (movementHandler)
	{
		movementHandler->SetMovementAutoContentsType(InMoveAutoContentsType);

		if (true == IsMoveToInteractWithNPC())
		{	// 2023/10/13 PKT - Interaction NPC가 타게팅일때는 타겟을 다시 잡아줘야 한다.

			FVector newLocation = InOrgMoveInfo._moveRequest.GetDestination();
			float newRadius = InOrgMoveInfo._moveRequest.GetAcceptanceRadius();

			float acceptanceRadius = 0.0f;
			UGsGameObjectBase* interactionTarget = nullptr;
			if (nullptr != _localPlayer)
			{
				if (FGsInteractionHandlerLocalPlayer* interactionHandler = _localPlayer->GetCastInteraction<FGsInteractionHandlerLocalPlayer>())
				{
					interactionTarget = interactionHandler->GetInteractionTarget();
					acceptanceRadius = interactionHandler->GetInteractionRangeReserveNpcData();
				}
			}
			
			FGsInteractionHandlerLocalPlayer::GetShiftTargetNonplayerPosAndAcceptanceRaidus(_localPlayer, interactionTarget, acceptanceRadius, newLocation, newRadius);

			movementHandler->GetStateAutoMove().StartPos(_localPlayer, newLocation, newRadius, InOrgMoveInfo._funcSuccessDelegate, InOrgMoveInfo._funcFailDelegate, InIsOtherMap);
		}
		else
		{
			if (InOrgMoveInfo._moveRequest.IsMoveToActorRequest())
			{
				/*
				* 기존 목적지가 특정 Actor를 향해 가는 거였다면 해당 Actor가 아직 유효한지 확인 해야 함.
				*/
				AActor* goalActor = InOrgMoveInfo._moveRequest.GetGoalActor();
				if (nullptr == goalActor || goalActor->IsPendingKillPending())
				{
					return false;
				}

				// 2023/10/13 PKT - Actor 설정
				movementHandler->GetStateAutoMove().StartActor(_localPlayer
					, goalActor
					, InOrgMoveInfo._moveRequest.GetAcceptanceRadius()
					, InOrgMoveInfo._funcSuccessDelegate
					, InOrgMoveInfo._funcFailDelegate
					, InIsOtherMap);
			}
			else 
			{
				// 2023/10/13 PKT - Point 설정
				movementHandler->GetStateAutoMove().StartPos(_localPlayer
					, InOrgMoveInfo._moveRequest.GetDestination()
					, InOrgMoveInfo._moveRequest.GetAcceptanceRadius()
					, InOrgMoveInfo._funcSuccessDelegate
					, InOrgMoveInfo._funcFailDelegate
					, InIsOtherMap);
			}
		}
	}

	// 2023/10/13 PKT - 충돌 회피 기능 On
	_localPlayer->SetPawnDetectionCollisionEnable(true);

	return true;
}

bool FGsAIAvoidCollisionHandler::ObstacleDetectedFront(const class UGsGameObjectBase* InBlockCreature, EGsMovementAutoContentsType InMoveAutoContentsType, const FGsAutoMoveInfo InAutoMoveInfo)
{
	if (nullptr == _localPlayer || nullptr == InBlockCreature || false == InAutoMoveInfo.IsValid())
	{	// 2023/10/12 PKT - 올바르지 않은 Param..
		return false;
	}

	if (nullptr == _localPlayer->GetMovementHandler())
	{
		return false;
	}
	
	if (EGsMovementState::AUTO != _localPlayer->GetMovementHandler()->GetState())
	{	// 2023/10/12 PKT - Auto 상태일 때만 가능
		return false;
	}

	if (true == _isAvoidingCollision)
	{
		return false;
	}

	if (EGsMovementAutoContentsType::None == InMoveAutoContentsType || EGsMovementAutoContentsType::AIAvoidCollision == InMoveAutoContentsType)
	{
		return false;
	}	

	// 2023/10/13 PKT - 충돌한 객체가 Local USer가 타겟으로 삼고 있는 대상이라면 충돌 회피 처리 하지 않음.
	FGsTargetHandlerBase* targetHandler = _localPlayer->GetCastTarget<FGsTargetHandlerBase>();
	if (targetHandler && targetHandler->GetTarget())
	{
		if (targetHandler->GetTarget()->GetGameId() == InBlockCreature->GetGameId())
		{
			return false;
		}
		
		if (EGsMovementAutoContentsType::MoveToSkill == InMoveAutoContentsType)
		{	// 2023/10/25 PKT - 스킬을 사용하기 위해 접근 중 타유저와의 충돌일 경우 공격가능 범위를 체크 한다.
			float dist = FVector::Dist(_localPlayer->GetLocation(), targetHandler->GetTarget()->GetLocation());
			float skillRange = FGsAIFunc::GetNextAutoSkillRange(_localPlayer);
			if (dist <= skillRange)
			{	// 2023/10/25 PKT - 공격 범위
				return false;
			}
		}
	}

	FVector nextPoint;
	if (false == findAvoidancePoint(_localPlayer, InBlockCreature, nextPoint))
	{	// 2023/10/12 PKT - 회피 지점을 찾지 못하였다면... 그냥 닥돌..
		return false;
	}

	FGsMovementHandlerLocalPlayer* movementHandler = _localPlayer->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (nullptr == movementHandler)
	{
		return false;
	}

	// 2023/10/19 PKT - 이전 정보 복사
	_orgMoveInfo.Clear();
	// 2023/10/12 PKT - 본래의 목적지를 "복사"
	_orgMoveInfo = InAutoMoveInfo;
	// 2023/10/19 PKT - 다른 맵으로 가기 위한 자동 이동 중이였는가?
	_isDifferentDestinationMap = movementHandler->GetStateAutoMove().IsMoveToOtherMap();
	// 2023/10/19 PKT - 컨텐츠 타입 저장
	_OrgContentsType = InMoveAutoContentsType;

	_isAvoidingCollision = true;

	const static float LIFE_TIME = 2.f;
	_elapsedTime = 2.f;

	// 2023/10/12 PKT - 탐지 Off
	_localPlayer->SetPawnDetectionCollisionEnable(false);

	// 2023/10/12 PKT - 목적지 변경	
	movementHandler->GetStateAutoMove().SetMovementAutoContentsType(EGsMovementAutoContentsType::AIAvoidCollision);
	movementHandler->GetStateAutoMove().StartPos(_localPlayer
		, nextPoint
		, 50.f
		, FGsAutoMoveCompletedDelegate::CreateRaw(this, &FGsAIAvoidCollisionHandler::OnFinishAvoidanceMode)
		, FGsAutoMoveCompletedOneDelegate::CreateRaw(this, &FGsAIAvoidCollisionHandler::OnFailedAvoidanceMode)
		, false
	);
	
	return true;
}

void FGsAIAvoidCollisionHandler::OnFinishAvoidanceMode()
{
	if (nullptr == _localPlayer)
	{
		return;
	}

	if (false == _orgMoveInfo.IsValid())
	{	// 2023/10/12 PKT - 목적지를 세팅한적이 없다!!!! 여기서 걸리면 무조건 원인 찾아야 함!!
		return;
	}

	if (false == _isAvoidingCollision)
	{
		return;
	}

	FGsAutoMoveInfo destMoveInfo = _orgMoveInfo;
	bool isDifferentDestinationMap = _isDifferentDestinationMap;
	EGsMovementAutoContentsType prevAutoContentsType = _OrgContentsType;

	// 2023/10/19 PKT - 회피 기동 종료
	Reset();

	// 2023/10/12 PKT - 본래의 목적지로 되돌린다.
	if (false == ReversgDestination(destMoveInfo, prevAutoContentsType, isDifferentDestinationMap))
	{
		if (destMoveInfo._funcFailDelegate.IsBound() == true)
		{			
			destMoveInfo._funcFailDelegate.Execute(FPathFollowingResult());
		}		
	}
	
	// 2023/10/12 PKT - 탐지 On
	_localPlayer->SetPawnDetectionCollisionEnable(true);
	_localPlayer->ClearPawnDetectionCollisionOverlaps();
}

void FGsAIAvoidCollisionHandler::OnFailedAvoidanceMode(const FPathFollowingResult& InResult)
{
	if (nullptr == _localPlayer)
	{
		return;
	}

	if (false == _orgMoveInfo.IsValid())
	{	// 2023/10/12 PKT - 목적지를 세팅한적이 없다!!!! 여기서 걸리면 무조건 원인 찾아야 함!!
		return;
	}

	if (InResult.Code == EPathFollowingResult::Aborted)
	{	
		return;
	}

	if (false == _isAvoidingCollision)
	{
		return;
	}

	FGsAutoMoveInfo destMoveInfo = _orgMoveInfo;
	bool isDifferentDestinationMap = _isDifferentDestinationMap;
	EGsMovementAutoContentsType prevAutoContentsType = _OrgContentsType;

	Reset();

	// 2023/10/12 PKT - 본래의 목적지로 되돌린다.
	if (false == ReversgDestination(destMoveInfo, prevAutoContentsType, isDifferentDestinationMap))
	{
		if (destMoveInfo._funcFailDelegate.IsBound() == true)
		{
			destMoveInfo._funcFailDelegate.Execute(FPathFollowingResult());
		}
	}

	// 2023/10/12 PKT - 탐지 On
	_localPlayer->SetPawnDetectionCollisionEnable(true);
	_localPlayer->ClearPawnDetectionCollisionOverlaps();
}

bool FGsAIAvoidCollisionHandler::findAvoidancePoint(const UGsGameObjectLocalPlayer* InLocalPlayer, const UGsGameObjectBase* InTargetObject, FVector& OutPoint)
{
	OutPoint = FVector::ZeroVector;

	if (nullptr == InLocalPlayer || nullptr == InLocalPlayer->GetActor() || InLocalPlayer->GetActor()->IsPendingKill())
	{	// 2023/10/12 PKT - Param 에러 검사
		return false;
	}

	if (nullptr == InTargetObject || nullptr == InTargetObject->GetActor() || InTargetObject->GetActor()->IsPendingKill())
	{	// 2023/10/12 PKT - Param 에러 검사
		return false;
	}

	// 2022/11/11 PKT - Local 
	const FVector localLocation = InLocalPlayer->GetLocation();
	const FVector localLookAt = InLocalPlayer->GetActor()->GetActorRotation().Vector();

	// 2022/11/08 PKT - Block Object Pos / Dir
	const FVector blockLocation = InTargetObject->GetLocation();

	// 2022/11/08 PKT - Normalize..(간소화)
	FVector dirLocalToBlock = (blockLocation - localLocation) * FVector(1.0f, 1.0f, 0.f);
	dirLocalToBlock.Normalize();

	// 2022/11/24 PKT - 좌/우 판별
	FVector sideDir = FVector::CrossProduct(localLookAt, dirLocalToBlock);

	enum class DirType { Left, Right };
	DirType dirType = (sideDir.Z < 0.f) ? DirType::Right : DirType::Left;

	// 2022/11/24 PKT - 추후 우회 하기 기능에 대한 고도화를 대비 하여 네비 메시 검사 결과에 대한 응답을 디테일하게 잡는다.
	enum class Reason { SUCCESS, FAILED, INVALID };
	auto CheckNavMeshRaycast = [](const UGsGameObjectLocalPlayer* InOwner, DirType InDir, const FVector& InBlockLocation, FVector& InDest) -> Reason
	{
		const FVector localLocation = InOwner->GetLocation();
		const FVector localLookAt = InOwner->GetActor()->GetActorRotation().Vector();

		float radian = 50.f * ((InDir == DirType::Right) ? 1.0f : -1.f);
		FVector roatateDir = UKismetMathLibrary::RotateAngleAxis(localLookAt, radian, FVector::UpVector);

		const static float MAX_DISTANCE = 200.f;
		float distance = FMath::Max(MAX_DISTANCE, FVector::Distance(localLocation, InBlockLocation));

		// 2022/11/24 PKT - 목적지.(우회해서 갈 곳.)
		InDest = localLocation + (roatateDir * distance);

		const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(InOwner->GetWorld());
		if (NavSys)
		{
			const AGsCharacterBase* AIOwner = Cast<AGsCharacterBase>(InOwner->GetActor());
			const ANavigationData* NavData = AIOwner ? NavSys->GetNavDataForProps(AIOwner->GetNavAgentPropertiesRef()) : nullptr;
			const ARecastNavMesh* RecastNavMesh = (NavData) ? Cast<const ARecastNavMesh>(NavData) : nullptr;
			if (RecastNavMesh)
			{
				// 2022/11/24 PKT - Start 좌표 ( Feet.. )
				FVector StartFeetLocation = AIOwner->GetMovementComponent()->GetActorFeetLocation();
				AIOwner->GetQueryFloorHeightComponent()->TryGetFloor(StartFeetLocation, StartFeetLocation);

				// 2022/11/24 PKT - 좌표 ( Feet.. )
				FVector EndFeetLocation = InDest;
				AIOwner->GetQueryFloorHeightComponent()->TryGetFloor(InDest, EndFeetLocation);

				FSharedConstNavQueryFilter QueryFilter = NavData->GetDefaultQueryFilter();

				ARecastNavMesh::FRaycastResult hitResult;
				FVector hitVector;

				bool isBlock = ARecastNavMesh::NavMeshRaycast(RecastNavMesh, StartFeetLocation, EndFeetLocation, hitVector, QueryFilter, AIOwner, hitResult);

				if (false == isBlock && hitResult.bIsRaycastEndInCorridor && false == hitResult.HasHit())
				{	// 2022/11/24 PKT - 갈수 있음.
					return Reason::SUCCESS;
				}
				else
				{
					// 2022/11/24 PKT - 가지 못함.
					return Reason::FAILED;
				}
			}
		}
		// 2022/11/24 PKT - Data 이상.(Navi 이상 ???)
		return Reason::INVALID;
	};

#if WITH_EDITOR
	auto Debug_Shape_Draw = [](const UGsGameObjectLocalPlayer* InOwner, const FVector& InCenter, Reason InReason) -> void
	{
		const static float Radius = 100.f;
		FColor color = (Reason::SUCCESS == InReason) ? FColor::Blue : (Reason::FAILED == InReason) ? FColor::Red : FColor::Yellow;

		DrawDebugSphere(InOwner->GetWorld(), InCenter, Radius, 8, color, false, 10.0f);
	};
#endif

	FVector dest = FVector::ZeroVector;
	Reason reason = CheckNavMeshRaycast(InLocalPlayer, dirType, blockLocation, dest);
#if WITH_EDITOR
	Debug_Shape_Draw(InLocalPlayer, dest, reason);
#endif
	if (Reason::SUCCESS != reason)
	{
		// 2022/11/24 PKT - 기존 방향에서 성공을 못하면 반대 방향(좌우 반전)해서 다시 검사 한다.
		FVector preDest = dest;
		DirType prevDir = dirType;
		Reason prevReason = reason;

		dirType = (dirType == DirType::Right) ? DirType::Left : DirType::Right;
		reason = CheckNavMeshRaycast(InLocalPlayer, dirType, blockLocation, dest);
#if WITH_EDITOR
		Debug_Shape_Draw(InLocalPlayer, dest, reason);
#endif
		// 2022/11/24 PKT - 첫 검사가 유효하지 않을때 두번째 검사가 실패 or 유효하지 않다면 첫번째 검사의 값으로 대처 함.
		if (Reason::INVALID == prevReason && Reason::SUCCESS != reason)
		{
			dest = preDest;
			dirType = prevDir;
			reason = prevReason;
		}
	}

	OutPoint = dest;

	return (Reason::FAILED != reason);
}