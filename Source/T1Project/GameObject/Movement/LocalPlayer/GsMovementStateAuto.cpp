#include "GsMovementStateAuto.h"

#include "../Data/GsGameClientVersion.h"
#include "ControllerEx/GsPlayerController.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorEx/GsCampBase.h"
#include "ActorComponentEx/GsQueryFloorHeightComponent.h"

#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectCamp.h"
#include "GameObject/Movement/LocalPlayer/Auto/GsWaypointWork.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/Interaction/GsInteractionMoveType.h"

#include "GsMovementNetSender.h"

#include "AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "Runtime/Engine/Public/AI/Navigation/NavQueryFilter.h"
#include "Runtime/NavigationSystem/Public/NavFilters/NavigationQueryFilter.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsAIManager.h"

#include "Message/MessageParam/GsAIMessageParam.h"
#include "Message/MessageParam/GsMinimapMessageParam.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"

#include "DataCenter/Public/Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "Net/GsNetSendServiceWorld.h"

#include "AI/Quest/GsAIQuestHandler.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UI/UILib/Manager/GsUIController.h"

#include "T1Project.h"

#include "DrawDebugHelpers.h"

#include "UTIL/GsLevelUtil.h"
#include "UTIL/GsMove.h"
#include "UTIL/GsDir.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "../ActorEx/GsCharacterBase.h"
#include "NavigationSystem/Public/NavigationData.h"
#include "NavigationSystem/Public/NavMesh/RecastNavMesh.h"
#include "../../Target/GsTargetHandlerLocalPlayer.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "../../AI/GsAIAvoidCollisionHandler.h"


//-------------------------------------------------------------------------------------------------
// FGsMovementState
//-------------------------------------------------------------------------------------------------
FGsMovementStateAuto::FGsMovementStateAuto() : FGsMovementState(EGsMovementState::AUTO)
{
}

FGsMovementStateAuto::~FGsMovementStateAuto()
{
	// OnRequestFinished 바인딩 해제를 안하고 나가서 델리게이트 카운트가 계속 증가됨
	// 소멸자에서 해제
	if (_pathComponent != nullptr)
	{
		// 기존 Delegate 삭제 진행
		_pathComponent->PostProcessMove.Unbind();
		_pathComponent->OnRequestFinished.RemoveAll(this);
	}
}

int16 FGsMovementStateAuto::GetDirNetConverted() const
{
	return FGsDir::dirToAngle(GetDirection());
}

float FGsMovementStateAuto::GetFallingSpeed() const
{
	return 0.0f;
}

const FVector& FGsMovementStateAuto::GetDirection() const
{
	return _direction;
}

const FVector& FGsMovementStateAuto::GetLocation() const
{
	if (_characterLocal == nullptr)
	{
		return FVector::ZeroVector;
	}

	_location = _characterLocal->GetMovementComponent()->GetActorFeetLocation();
	return _location;
}

const FVector& FGsMovementStateAuto::GetDestination() const
{	
	// 목적위치 계산
	_destination = GetLocation() + (GetDirection() * _moveSpeed * FGsMovementNetSender::SendDestinationTime);
	_characterLocal->GetQueryFloorHeightComponent()->TryGetFloor(_destination, _destination);
	return _destination;
}

//-------------------------------------------------------------------------------------------------
// TGsState
//-------------------------------------------------------------------------------------------------
void FGsMovementStateAuto::Enter()
{	
	// 2023/10/12 PKT - 회피 기능 On
	if (FGsAIAvoidCollisionHandler* avoidCollision = _owner->GetAIAvoidCollision())
	{
		avoidCollision->BegineDetector();
	}
}

void FGsMovementStateAuto::Exit()
{
	if (nullptr != _playerController)
	{
		_playerController->StopMovement();
	}

	ClearData();
	
	// 2023/10/12 PKT - 회피 기능 Off( 확실하게는 ClearData 이후 초기화 진행 해야 함. )
	if (FGsAIAvoidCollisionHandler* avoidCollision = _owner->GetAIAvoidCollision())
	{
		avoidCollision->EndDetector();
	}
}

void FGsMovementStateAuto::Update(float inDeltatime)
{	
	if (const UCharacterMovementComponent* moveComp = Cast<UCharacterMovementComponent>(_characterLocal->GetMovementComponent()))
	{
		SetDirection(moveComp->RequestedVelocity);
	}

	if (_waypointWork != nullptr)
	{
		_waypointWork->Update(inDeltatime);
	}

	if (GSAI())
	{
		if (GSAI()->GetIsDrawDebugPath() == true &&
			_currentNavPathPtr.IsValid())
		{
			//TArray<FNavPathPoint> pathPoints = _currentNavPathPtr->GetPathPoints();
			//FVector oldPos = FVector::ZeroVector;
			//for (auto iter : pathPoints)
			//{
			//	FVector newPos = iter.Location;
			//	if (oldPos.IsZero())
			//	{
			//		oldPos = newPos;
			//		continue;
			//	}

			//	DrawDebugLine(_characterLocal->GetWorld(), oldPos, newPos, FColor::Red, false, -1.0f);
			//	oldPos = newPos;
			//}

			FGsMove::DrawPathLine(_characterLocal->GetWorld(),_currentNavPathPtr, -1.0f, FColor::Red);
		}
	}
}

bool FGsMovementStateAuto::CanTransition(EGsMovementState inType)
{
	return true;
}

void FGsMovementStateAuto::SetDirection(const FVector& inDir)
{
	FGsMovementState::SetDirection(inDir);
	SetDirty(true);
}

//-------------------------------------------------------------------------------------------------
// FGsMovementStateAuto
//-------------------------------------------------------------------------------------------------
void FGsMovementStateAuto::UpdateDirection()
{
	
	
}

void FGsMovementStateAuto::CallbackMoving(UPathFollowingComponent* inComponent, FVector& inPos)
{
}

void FGsMovementStateAuto::CallbackFinish(FAIRequestID inID, const FPathFollowingResult& inResult)
{
	// 도착지 찾기 실패시 재이동 요청시 (moveto) abort 떨어져서 콜백 처리 무시
	if (_isLockFinishCallback == true)
	{
		// 한번만 막자
		_isLockFinishCallback = false;
		return;
	}

	if (inResult.IsSuccess())
	{
		if (_autoMoveInfo.IsValid() && _autoMoveInfo._funcSuccessDelegate.IsBound())
		{
			_autoMoveInfo._funcSuccessDelegate.Execute();
		}

		// 다른맵 이동이면 추가 처리( 회피 기동 중이 아닐 때 )
		if (_isMoveToOtherMap == true)
		{
			CallbackMoveToWarpFinish();
		}
	}

	// 실패시
	if (inResult.IsFailure() && _autoMoveInfo.IsValid() && _autoMoveInfo._funcFailDelegate.IsBound())
	{

		// 수동입력으로 인한 취소가 아니면
		if (inResult.Code == EPathFollowingResult::Aborted &&
			inResult.HasFlag(FPathFollowingResultFlags::MovementStop) &&
			inResult.HasFlag(FPathFollowingResultFlags::ForcedScript))
		{
			// 위에 조건이 stop movement 로 생긴거임....
		}
		else
		{
			//GSLOG(Error, TEXT(">>>[MOVE_FAILED] result: %s"), *inResult.ToString());
		}
		_autoMoveInfo._funcFailDelegate.Execute(inResult);

		// 실패만 여기서 처리
		// 성공은 각자 컨텐츠에서
		_movementAutoContentsType = EGsMovementAutoContentsType::None;
	}

	if (_currentNavPathPtr.IsValid())
	{
		_currentNavPathPtr.Reset();
	}

	// 워프 값 클리어
	_warpId = 0;
	_isMoveToOtherMap = false;
}
// 워프로 이동 성공
void FGsMovementStateAuto::CallbackMoveToWarpFinish()
{
	if (_warpId == 0)
	{
		return;
	}

	bool isExistWidgetPreventAutoClose = false;
	UGsUIController* uiController = GUI()->GetUIController();
	if (uiController != nullptr)
	{
		isExistWidgetPreventAutoClose =
			uiController->IsExistWidgetPreventAutoClose();
	}

	// 닫기 처리 막는 컨텐츠가 열려있다면 중지
	if (isExistWidgetPreventAutoClose == true)
	{	
		if (_movementAutoContentsType == EGsMovementAutoContentsType::QuestMove)
		{
			// hud 액티브 끄기
			// 퀘스트 쪽에 stop 요청
			GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_ACTIVE_OFF, nullptr);
		}
	}
	else
	{
		if (UGsLevelUtil::IsCanWarp(_warpId))
		{
			// ai next move flag prevent
			GSAI()->SetNextMapWarpStart();

			GMessage()->GetGameObject().SendMessage(MessageGameObject::OBJ_WARP_START, nullptr);

			FGsNetSendServiceWorld::SendWarp(_warpId);
		}			
		// 워프 이동실패시도 같은 처리
		else
		{
			if (_movementAutoContentsType == EGsMovementAutoContentsType::QuestMove)
			{
				// hud 액티브 끄기
				// 퀘스트 쪽에 stop 요청
				GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_ACTIVE_OFF, nullptr);
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------

// 이전 실패 델리게이트 호출(새로 들어온것의 바인딩된 주체가 다르면)
void FGsMovementStateAuto::ClearOldFailDelegate(const FGsAutoMoveCompletedOneDelegate& In_newFailDelegate)
{
	// 연결된게 없다면 리턴
	if (_autoMoveInfo._funcFailDelegate.IsBound() ==false)
	{
		return;
	}
	
	const void* newPoint = In_newFailDelegate.GetObjectForTimerManager();
	const void* oldPoint = _autoMoveInfo._funcFailDelegate.GetObjectForTimerManager();
	// 새로 들어온거랑 다르면
	if (newPoint !=	oldPoint)
	{
		FPathFollowingResult result;
		result.Code = EPathFollowingResult::Aborted;
		_autoMoveInfo._funcFailDelegate.Execute(result);
	}
}

void FGsMovementStateAuto::StartActor(
	UGsGameObjectLocalPlayer* inOwner, const AActor* inTargetActor, 
	float inAcceptanceRadius, const FGsAutoMoveCompletedDelegate& inSuccessDelegate,
	const FGsAutoMoveCompletedOneDelegate& inFailDelegate,
	bool In_isMoveToOtherMap)
{
	ClearOldFailDelegate(inFailDelegate);
	//GSLOG(Warning, TEXT("[AUTO] FGsMovementStateAuto::Start set auto data"));

	check(inOwner);
	check(inTargetActor);


#ifdef TEST_AUTO_MOVE_LOG
	GSLOG(Warning,
		TEXT("<<<< [MOVE TEST] FGsMovementStateAuto Start - targetName: %s"),
		*inTargetActor->GetName());
	
#endif
	// move request 데이터 초기화 안된 상태에서
	// 데이터 밀어 넣으면 무시됨(이전 액터 쫓아 다님)
	// 이동은 이전넘한테 하고
	// 스킬 레인지 체크는 새놈한테 하면
	// 공격안하고 좀비처럼 따라 다니기만함
	// (언리얼 에디터에 포커스안할때 잘나옴)
	// 데이터 넣기전 초기화 해준다
	ClearData();
		
	_autoMoveInfo._moveRequest.SetGoalActor(inTargetActor);
	_autoMoveInfo._moveRequest.SetAcceptanceRadius(inAcceptanceRadius);
	_autoMoveInfo._moveRequest.SetReachTestIncludesAgentRadius(false);
	_autoMoveInfo._moveRequest.SetReachTestIncludesGoalRadius(false);
	if (_isAllowPartialPath == false)
	{
		// 불완전한 경로는 (목적지 못가면) 실패
		_autoMoveInfo._moveRequest.SetAllowPartialPath(false);
	}

	_autoMoveInfo._funcSuccessDelegate = inSuccessDelegate;
	_autoMoveInfo._funcFailDelegate = inFailDelegate;

	Start(inOwner, _autoMoveInfo);

	_isMoveToOtherMap = In_isMoveToOtherMap;
}

void FGsMovementStateAuto::StartPos(
	UGsGameObjectLocalPlayer* inOwner, const FVector& inTargetLocation, 
	float inAcceptanceRadius, const FGsAutoMoveCompletedDelegate& inSuccessDelegate,
	const FGsAutoMoveCompletedOneDelegate& inFailDelegate, 
	bool In_isMoveToOtherMap)
{
	// [B1] | ejrrb10 | https://jira.com2us.com/wiki/pages/viewpage.action?pageId=322939420
	// 페어리 연출이 끝나지 않았다면 자동 이동을 취소한다 -> 서버 쪽에서 제어하기로 하여 주석 처리
	//if (FGsMovementHandlerLocalPlayer* movement = _owner->GetCastMovement<FGsMovementHandlerLocalPlayer>())
	//{
	//	if (false == movement->GetMoveAble())
	//	{
	//		return;
	//	}
	//}

	ClearOldFailDelegate(inFailDelegate);
	check(inOwner);

	ClearData();

	FVector location = inTargetLocation;
	if (_characterLocal)
	{
		// 목적 좌표 높이 보정
		_characterLocal->GetQueryFloorHeightComponent()->TryGetFloor(location, location);
	}

	if (_isDrawPosSphere == true)
	{
		DrawDebugSphere(inOwner->GetWorld(), location, inAcceptanceRadius, 8, FColor::Red, false, 10.0f);
	}

		
	_autoMoveInfo._moveRequest.SetGoalLocation(location);
	_autoMoveInfo._moveRequest.SetAcceptanceRadius(inAcceptanceRadius);

	_autoMoveInfo._moveRequest.SetNavigationFilter(GData()->GetGlobalData()->_autoMoveFilterClass);


	_autoMoveInfo._funcSuccessDelegate = inSuccessDelegate;
	_autoMoveInfo._funcFailDelegate = inFailDelegate;
	if (_isAllowPartialPath == false)
	{
		// 불완전한 경로는 (목적지 못가면) 실패
		_autoMoveInfo._moveRequest.SetAllowPartialPath(false);
	}

	Start(inOwner, _autoMoveInfo);

	_isMoveToOtherMap = In_isMoveToOtherMap;
}

void FGsMovementStateAuto::StartMapIdPos(
	UGsGameObjectLocalPlayer* inOwner, int inMapId, const FVector& inTargetLocation,
	float inAcceptanceRadius, 
	const FGsAutoMoveCompletedDelegate& inSuccessDelegate,
	const FGsAutoMoveCompletedOneDelegate& inFailDelegate)
{
	ClearOldFailDelegate(inFailDelegate);
	// 초기화
	_warpId = 0;

	_moveToMapId = inMapId;

	FVector localPos = _characterLocal->GetActorLocation();

	// 자동 이동 워프 반지름 rate 값(원래 반지름보다 더들어가게 처리용)
	float autoMoveWarpAcceptanceRadiusRate =
		GData()->GetGlobalData()->_autoMoveWarpAcceptanceRadiusRate;

	// 대상맵이 현재맙과 같으면 바로 이동
	int currMapId = GLevel()->GetCurrentLevelId();
	if (currMapId == inMapId)
	{		
		
		//네비 메쉬로 이동 가능한가
		bool isMovePossible = FGsMove::IsMovePossible(_playerController, 
			_characterLocal,
			inTargetLocation, localPos,
			inAcceptanceRadius);

		bool isWarpInfoExist = false;
		if (isMovePossible == false)
		{
			// 현재맵이 그림자 전장이면
			// 같은 목적지를 가진 워프가 있을수 있음
			// https://jira.com2us.com/jira/browse/CHR-24660

			bool isMultiSameDestWarp = false;
			MapType currentMapType = GLevel()->GetCurrentMapType();
			if (currentMapType == MapType::GUILD_WAR)
			{
				isMultiSameDestWarp = true;
			}
		
			FGsSchemaSpotInfo spotInfo;
			if (true == GSClientSpawn()->TryGetCurrentMapWarpInfo(currMapId, localPos, inTargetLocation,
				_warpId, spotInfo, isMultiSameDestWarp))
			{
				_moveToPos = inTargetLocation;

				isWarpInfoExist = true;
				

				float capsuleRadius =
					inOwner->GetData()->GetScaledCapsuleRadius();

				// 서버에서 워프 거리 체크해서 완전히 들어가야함
				// 워프 스팟 반지름만큼만 다가가자
				float calcAcceptanceRadius = (spotInfo.radius - capsuleRadius) * autoMoveWarpAcceptanceRadiusRate;
				StartPos(inOwner, spotInfo.pos, calcAcceptanceRadius,
					inSuccessDelegate,
					inFailDelegate, 
					// https://jira.com2us.com/jira/browse/C2URWQ-4242
					// UGsGameObjectWarp::OnBeginOverlap 에서 처리함
					false);

				FGsAIMoveNextMapWarpInfoParam moveNextMapWarpParam(spotInfo.pos);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_MOVE_NEXT_MAP_WARP, &moveNextMapWarpParam);

				_moveToWarpPos = spotInfo.pos;

			}
		}
		// 이동 가능하거나
		// 이동 불가인데 워프 정보 없으면 그냥 시도(먼길 떠나면 안되어서 중간 retry 로 해야 할수도)
		if (isMovePossible == true ||
			isWarpInfoExist == false)
		{
			StartPos(inOwner, inTargetLocation, inAcceptanceRadius, inSuccessDelegate, inFailDelegate);
		}

	}
	// 다른맵이면 그위치로 가기위한 포털을 찾아서 거기로 간다
	else
	{
		_moveToPos = inTargetLocation;

		//워프 스팟 정보
		FGsSchemaSpotInfo spotInfo;		
		if (GSClientSpawn()->TryGetNextMapWarpPos(currMapId, inMapId,
			spotInfo, _warpId) == false)
		{
			// 못찾으면 티커와 실패 콜백을 날리자
			FText findText;
			FText::FindText(TEXT("SpaceCrackText"), TEXT("NoticeQuestAutoPlay"), findText);
			FGsUIHelper::TrayMessageTicker(findText);

#if WITH_EDITOR
			// 시련의 전장 -> 일반필드 지역맵 땅찍고 이동 같은 케이스로
			// 워프가 없는 곳으로 이동 시도가 있으므로 팝업 출력은 이제 안함(워프 데이터가 없는 케이스가 있으므로)
			// https://jira.com2us.com/jira/browse/C2URWQ-6497
			//const FString failTickerString =
			//	FString::Printf(TEXT("[Popup_EditorOnly]\n find fail from mapid: %d, to mapid: %d"), currMapId, inMapId);
			//FGsUIHelper::PopupOK(FText::FromString(failTickerString));

			GSLOG(Warning, TEXT("[AUTO] find fail from mapid: %d, to mapid: %d"), currMapId, inMapId);
#endif			
			// https://jira.com2us.com/jira/browse/C2URWQ-6497
			GMessage()->GetGameObject().SendMessage(MessageGameObject::AUTO_MOVE_CANT_FIND_NEXTMAP_WARP, nullptr);

			if (inFailDelegate.IsBound() == true)
			{
				inFailDelegate.Execute(FPathFollowingResult(
					EPathFollowingResult::OffPath, FPathFollowingResultFlags::None));
			}

			// clear auto contents type 
			// https://jira.com2us.com/jira/browse/C2URWQ-615
			_movementAutoContentsType = EGsMovementAutoContentsType::None;
			return;
		}
		
		FVector nextMapWarpPos = spotInfo.pos;

		float capsuleRadius =
			inOwner->GetData()->GetScaledCapsuleRadius();

		// 서버에서 워프 거리 체크해서 완전히 들어가야함
		// 워프 스팟 반지름만큼만 다가가자
		float calcAcceptanceRadius = (spotInfo.radius - capsuleRadius) * autoMoveWarpAcceptanceRadiusRate;


		// https://jira.com2us.com/jira/browse/CHR-15710
		// 다른맵 가는 워프 가는길도 못가는길일수도 있다(맵내 워프돌고 돌아)
		bool isMovePossible = FGsMove::IsMovePossible(_playerController,
			_characterLocal,
			nextMapWarpPos, localPos,
			calcAcceptanceRadius);

			
		bool isWarpInfoExist = false;
		if(isMovePossible == false)
		{
			FGsSchemaSpotInfo sameMapWarpspotInfo;
			if (true == GSClientSpawn()->TryGetCurrentMapWarpInfo(currMapId, localPos, nextMapWarpPos,
				_warpId, sameMapWarpspotInfo))
			{
				// 못가는 맵은(워프 이동이 필요하므로)
				// 최초 위치로 저장
				// 현재맵 내의 워프 위치로 저장시  목적지 표시가 이상하게 나올수 있다
				// https://jira.com2us.com/jira/browse/C2URWQ-5545
				
				//_moveToPos = nextMapWarpPos;

				isWarpInfoExist = true;


				StartPos(inOwner, sameMapWarpspotInfo.pos, calcAcceptanceRadius,
					inSuccessDelegate,
					inFailDelegate, 
					// https://jira.com2us.com/jira/browse/C2URWQ-4242
					// UGsGameObjectWarp::OnBeginOverlap 에서 처리함
					false);

				FGsAIMoveNextMapWarpInfoParam moveNextMapWarpParam(sameMapWarpspotInfo.pos);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_MOVE_NEXT_MAP_WARP, &moveNextMapWarpParam);

				_moveToWarpPos = sameMapWarpspotInfo.pos;
			}
		}

		if (isMovePossible == true ||
			isWarpInfoExist == false)
		{
			StartPos(inOwner, nextMapWarpPos, calcAcceptanceRadius,
				inSuccessDelegate,
				inFailDelegate, 
				// https://jira.com2us.com/jira/browse/C2URWQ-4242
				// UGsGameObjectWarp::OnBeginOverlap 에서 처리함
				false);

			FGsAIMoveNextMapWarpInfoParam moveNextMapWarpParam(nextMapWarpPos);
			GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_MOVE_NEXT_MAP_WARP, &moveNextMapWarpParam);

			_moveToWarpPos = nextMapWarpPos;
		}
	}
}

void FGsMovementStateAuto::StartMapIdPositions(UGsGameObjectLocalPlayer* inOwner, int inMapId, 
	const TArray<FVector>& inArrayTargetLocation,
	const FGsAutoMoveCompletedDelegate& inSuccessDelegate,
	const FGsAutoMoveCompletedOneDelegate& inFailDelegate,
	bool inIsCenterNode,
	float In_useAcceptanceRadius)
{
	if (nullptr == _waypointWork)
	{
		return;
	}

	if (inArrayTargetLocation.Num() == 0)
	{
		return;
	}

	// 1. 다른맵 가거나
	// 2. 같은맵내의 워프 타면(섬맵)
	// 첫번째 목적지만 처리하게 함
		
	bool isUseWarp = false;
	int currMapId = GLevel()->GetCurrentLevelId();
	if (currMapId == inMapId)
	{
		FVector localPos = _characterLocal->GetActorLocation();

		float acceptanceRadius = FGsMove::GetMovePosRange();
		// -값 들어가면 이동 실패 되므로 최소값으로 세팅
		acceptanceRadius = FMath::Max(acceptanceRadius, 0.1f);

		//네비 메쉬로 이동 가능한가
		bool isMovePossible = FGsMove::IsMovePossible(_playerController,
			_characterLocal,
			inArrayTargetLocation[0], localPos,
			acceptanceRadius);

		if (isMovePossible == false)
		{
			FGsSchemaSpotInfo spotInfo;
			int warpId = 0;
			if (true == GSClientSpawn()->TryGetCurrentMapWarpInfo(currMapId, localPos, inArrayTargetLocation[0],
				warpId, spotInfo))
			{
				isUseWarp = true;
			}
		}
	}
	else
	{
		isUseWarp = true;
	}

	TArray<FVector> singlePos;
	singlePos.Add(inArrayTargetLocation[0]);

	_waypointWork->StartWaypointData(inMapId, 
		(isUseWarp == true)? singlePos :inArrayTargetLocation, 
		0, inIsCenterNode,
		inSuccessDelegate, inFailDelegate,
		In_useAcceptanceRadius);

}

void FGsMovementStateAuto::Start(UGsGameObjectLocalPlayer* inOwner, const FGsAutoMoveInfo& inAutoMoveData)
{
	check(inOwner);
	_owner = inOwner;

	// 행동 제약 관련
	if (_owner->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT))
	{
		if (_autoMoveInfo._funcFailDelegate.IsBound())
		{
			FPathFollowingResult result;
			result.Code = EPathFollowingResult::Aborted;
			_autoMoveInfo._funcFailDelegate.Execute(result);
		}
		return;
	}

	if (_pathComponent)
	{
		_pathComponent->Cleanup();
	}
	_autoMoveInfo = inAutoMoveData;

	// 방향 수정
	FVector dir = (inAutoMoveData._moveRequest.GetDestination() - inOwner->GetLocation()).GetSafeNormal2D();
	SetDirection(dir);

	FGsGameObjectStateManager* fsm = inOwner->GetBaseFSM();
	check(fsm);

	fsm->ProcessEvent(EGsStateBase::AutoMove);
}

bool FGsMovementStateAuto::HasReached(const FVector& inTargetLocation, float inAcceptanceRadius) const
{
	if (nullptr == _pathComponent)
	{
		return false;
	}

	FAIMoveRequest req;
	req.SetGoalLocation(inTargetLocation);
	req.SetAcceptanceRadius(inAcceptanceRadius);
	return _pathComponent->HasReached(req);
}

//-------------------------------------------------------------------------------------------------

void FGsMovementStateAuto::SetPathFollowingComponent(UPathFollowingComponent* inComponent)
{
	check(inComponent);
	_pathComponent = inComponent;

	// 기존 Delegate 삭제 진행
	_pathComponent->PostProcessMove.Unbind();
	_pathComponent->OnRequestFinished.RemoveAll(this);

	_pathComponent->PostProcessMove.BindRaw(this, &FGsMovementStateAuto::CallbackMoving);
	// OnRequestFinished 바인딩 해제를 안하고 나가서 델리게이트 카운트가 계속 증가됨
	// 소멸자에서 해제
	_pathComponent->OnRequestFinished.AddRaw(this, &FGsMovementStateAuto::CallbackFinish);

	// 최소 반경, 높이 허용치 기준 변경
	_pathComponent->SetPreciseReachThreshold(1.1f, 3.f);
}

void FGsMovementStateAuto::SetPlayerController(AGsPlayerController* inController)
{
	if (inController)
	{
		_playerController = inController;

		SetPathFollowingComponent(inController->GetPathFollowingComponent());
	}
}

void FGsMovementStateAuto::ClearData()
{ 
	_autoMoveInfo.Clear(); 

	if (_movementAutoContentsType != EGsMovementAutoContentsType::AIAvoidCollision)
	{
		// 2023/10/12 PKT - Auto 이 회피 기능을 위함이 아님 다른 사유로 값을 초기화 해야 할 때는 회피 기능도 새롭게 초기화 한다.
		if (FGsAIAvoidCollisionHandler* avoidCollision = _owner->GetAIAvoidCollision())
		{
			avoidCollision->BegineDetector();
		}
	}
}

// 초기화
void FGsMovementStateAuto::Initialize(UGsGameObjectLocalPlayer* In_local)
{
	_waypointWork = new FGsWaypointWork();
	_waypointWork->Initialize(In_local);
	_owner = In_local;
}
// 클리어
void FGsMovementStateAuto::Finalize()
{
	if (_waypointWork != nullptr)
	{
		_waypointWork->Finalize();
		delete _waypointWork;
	}
}

void FGsMovementStateAuto::SetIsDrawWaypointDebugPos(bool In_isDraw)
{
	if (nullptr == _waypointWork)
	{
		return;
	}
	_waypointWork->SetIsDrawDebugPos(In_isDraw);
}

void FGsMovementStateAuto::SetAcceptanceRadius(float inAcceptanceRadius)
{
	if (_autoMoveInfo.IsValid())
	{
		_autoMoveInfo._moveRequest.SetAcceptanceRadius(inAcceptanceRadius);
	}
}

// 현재 이동 목표 mapId
int FGsMovementStateAuto::GetMoveToMapId() const 
{
	bool isLocalMap = false;
	// 워프 id가 없으면 현재맵
	if (_warpId == 0)
	{
		isLocalMap = true;
	}

	int moveToMapId = 0;
	// 현재맵이면 level에서 정보 가져옴
	if (isLocalMap == true)
	{
		moveToMapId = GLevel()->GetCurrentLevelId();
	}
	else
	{
		moveToMapId = _moveToMapId;
	}
	return moveToMapId;
}
// 현재 nav path 저장
void FGsMovementStateAuto::SetCurrentNavPath(FNavPathSharedPtr In_path)
{
	_currentNavPathPtr = In_path;

	if (_currentNavPathPtr.IsValid() == false)
	{
		return;
	}
	// path 마지막 위치가 목표 위치랑 다르면
	// 목적지 도달시 재검색을 한다
	TArray<FNavPathPoint> pathPoints = _currentNavPathPtr->GetPathPoints();
	int pointSize = pathPoints.Num();
	if (pointSize == 0)
	{
		return;
	}
	int lastIndex = pointSize - 1;
	FVector lastPathPos =  pathPoints[lastIndex].Location;

	// 액터 이동이면 액터의 위치
	FVector targetPos = FVector::ZeroVector;
	if (_autoMoveInfo._moveRequest.IsMoveToActorRequest())
	{
		AActor* goalActor =
			_autoMoveInfo._moveRequest.GetGoalActor();
		if (goalActor != nullptr)
		{
			targetPos = goalActor->GetActorLocation();
		}
	}
	else
	{
		targetPos = _autoMoveInfo._moveRequest.GetGoalLocation();
	}

	FVector ToGoal = targetPos - lastPathPos;
	// 목적지와 path 마지막 노드 위치 거리 제곱값
	float dist2dSq = ToGoal.SizeSquared2D();

	float acceptanceRadius =
		_autoMoveInfo._moveRequest.GetAcceptanceRadius();

	// 거리가 세팅한 허용 거리보다 멀리 있으면 목적지 도달 실패
	if (dist2dSq > FMath::Square(acceptanceRadius))
	{
		_isNotReachTargetPos = true;
	}
	else
	{
		_isNotReachTargetPos = false;
	}

	// 월드맵 개선
	// 미니맵 자동 경로 표시 처리
	// path 꼭지점만(변경전)
	// https://jira.com2us.com/jira/browse/CHR-23756
	// 스킬은 빼고
	if (_movementAutoContentsType == EGsMovementAutoContentsType::MoveToSkill)
	{
		return;
	}
	TArray<FVector> arrayPos;
	for (auto& iter: pathPoints)
	{
		arrayPos.Add(iter.Location);
	}

	if (arrayPos.Num() != 0)
	{
		FGsMinimapAutoMovePathParam param(true, arrayPos);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_AUTO_MOVE_PATH_ICON_INFO, &param);
	}
}

const MotionType FGsMovementStateAuto::GetMotionType() const
{
	if (_owner->IsVehicleMode())
	{
		return MotionType::VEHICLE;
	}
	return MotionType::NORMAL;
}

// 웨이포인트 남은 거리
float FGsMovementStateAuto::GetWayPointMoveTargetDist() const
{
	if (_waypointWork == nullptr)
	{
		return 0.0f;
	}
	int currMapId = GLevel()->GetCurrentLevelId();
	float remainDist = 0.0f;
	if (_waypointWork->GetReserveMapId() != currMapId)
	{
		FVector2D targetPos = FVector2D(_moveToWarpPos);
		FVector2D localPos = FVector2D(_characterLocal->GetActorLocation());

		remainDist = (localPos - targetPos).Size();
	}
	else
	{
		remainDist =
			_waypointWork->GetWayPointMoveTargetDist();
	}

	return remainDist;
}

void FGsMovementStateAuto::SetMovementAutoContentsType(EGsMovementAutoContentsType In_type)
{ 
	EGsMovementAutoContentsType oldContentType = _movementAutoContentsType;

	_movementAutoContentsType = In_type; 
	_backupMovementAutoContentsType = In_type;

	// 타입 변경된거 메시지로 쏜다
	// https://jira.com2us.com/jira/browse/C2URWQ-3193
	FGsAutoMoveContentsChangedParam param(oldContentType, _movementAutoContentsType);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AUTO_MOVE_CONTENTS_TYPE_CHANGED, &param);
}
// stop 후 재시도시 다시 데이터 복구용
// https://jira.com2us.com/jira/browse/C2URWQ-6526
void FGsMovementStateAuto::RestoreBackupMovementAutoContentsType()
{
	// 백업이 none 이거나
	// 현재가 none이 아니면 복구 안한다
	if (_backupMovementAutoContentsType == EGsMovementAutoContentsType::None &&
	_movementAutoContentsType != EGsMovementAutoContentsType::None)
	{
		return;
	}

	_movementAutoContentsType = _backupMovementAutoContentsType;
}

void FGsMovementStateAuto::ObstacleDetectedFront(const UGsGameObjectBase* InBlockCreature)
{
	// 2023/10/12 PKT - 회피 Handler로 정보를 전라.
	if (FGsAIAvoidCollisionHandler* avoidCollision = _owner->GetAIAvoidCollision())
	{
		avoidCollision->ObstacleDetectedFront(InBlockCreature, _movementAutoContentsType, _autoMoveInfo);
	}
}

void FGsMovementStateAuto::AddPathPoint(FVector In_pos)
{
	if (_currentNavPathPtr.IsValid() == false)
	{
		return;
	}
	
	_currentNavPathPtr->GetPathPoints().Add(FNavPathPoint(In_pos));

}

// 목적지가 대상 캠프 안에 있는가
// 충돌체 안으로 들어온 이벤트(수호탑 내에 퀘스트 목표가 있을 때 진행 불가 이슈)
// https://jira.com2us.com/jira/browse/C2URWQ-6363
bool FGsMovementStateAuto::IsGolPosInTargetCamp(const AActor* In_targetCamp, float& Out_dist)
{
	if (In_targetCamp == nullptr)
	{
		return false;
	}

	// 액터 이동이면 액터의 위치
	FVector targetPos = FVector::ZeroVector;
	if (_autoMoveInfo._moveRequest.IsMoveToActorRequest())
	{
		AActor* goalActor =
			_autoMoveInfo._moveRequest.GetGoalActor();
		if (goalActor != nullptr)
		{
			targetPos = goalActor->GetActorLocation();
		}
	}
	else
	{
		targetPos = _autoMoveInfo._moveRequest.GetGoalLocation();
	}

	const FVector& campLocation = In_targetCamp->GetActorLocation();
	const AGsCampBase* campBase = Cast<AGsCampBase>(In_targetCamp);
	if (campBase == nullptr)
	{
		return false;
	}
	UCapsuleComponent* capsule = campBase->GetCapsuleComponent();
	if (capsule == nullptr)
	{
		return false;
	}
	
	float acceptanceRadius = capsule->GetScaledCapsuleRadius();

	float dist = FVector::Dist(targetPos, campLocation);
	Out_dist = dist;
	return (acceptanceRadius >= dist);
}