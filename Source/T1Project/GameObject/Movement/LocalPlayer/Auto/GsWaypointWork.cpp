#include "GsWaypointWork.h"

#include "Input/GsInputBindingLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Management/ScopeGame/GsAIManager.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGameClientVersion.h"

#include "Util/GsMove.h"

#include "Engine/Public/DrawDebugHelpers.h"

#include "T1Project.h"


// 예약 데이터가 있는가
bool FGsWaypointWork::IsReserveData()
{
	return (_reserveWaypointInfo._arrayPos.Num() == 0) ? false : true;
}

// 초기화
void FGsWaypointWork::Initialize(UGsGameObjectLocalPlayer* In_owner)
{
	_local = In_owner;

	FGsMessageHolder* msg = GMessage();

 	
	_actionDelegates.Emplace(
		msg->GetInput().AddRaw(MessageInput::PressJoystick, this, &FGsWaypointWork::OnPressJoystick)
	);
 	
	_gameObjectMsg.Emplace(
 	msg->GetGameObject().AddRaw(MessageGameObject::AUTO_MOVE_CANT_FIND_NEXTMAP_WARP, this,
 		&FGsWaypointWork::OnAutoMoveCantFindNextMapWarp));

	_gameObjectMsg.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::AUTO_MOVE_CAMP_HIT_RESTART, this,
			&FGsWaypointWork::OnAutoMoveCampHitRestart));
}

// 종료
void FGsWaypointWork::Finalize()
{
	_local = nullptr;

	// 바인딩 된 메시지 해제
	FGsMessageHolder* msg = GMessage();

	if (_actionDelegates.Num() != 0)
	{
		for (auto iter : _actionDelegates)
		{
			msg->GetInput().Remove(iter);
		}
		_actionDelegates.Empty();
	}


	if (_gameObjectMsg.Num() != 0)
	{
		for (auto iter : _gameObjectMsg)
		{
			msg->GetGameObject().Remove(iter);
		}
		_gameObjectMsg.Empty();
	}
}
// 웨이포인트 데이터 세팅
void FGsWaypointWork::StartWaypointData(int In_mapId, const TArray<FVector>& In_pos, int In_moveIndex,
	bool In_isCenterNode,
	const FGsAutoMoveCompletedDelegate& inSuccessDelegate,
	const FGsAutoMoveCompletedOneDelegate& inFailDelegate, 
	float In_useAcceptanceRadius)
{
	ClearReserveWaypointData();

	TArray<FVector> savePos;
	if (In_pos.Num() != 0)
	{
		// 현재위치 맵id랑 목표랑 다르면
		int currMapId = GLevel()->GetCurrentLevelId();
		if (In_mapId != currMapId)
		{
			// 첫번째 위치만 이동 처리한다(어짜피 다른맵 워프로 가기때문)
			savePos.Add(In_pos[0]);
		}
		else
		{
			savePos = In_pos;
		}
	}

	_reserveWaypointInfo._arrayPos = savePos;
	_reserveWaypointInfo._funcSuccessDelegate = inSuccessDelegate;
	_reserveWaypointInfo._funcFailDelegate = inFailDelegate;

	// 중간 노드 사용하면 
	// 적당한 노드 위치를 찾는다
	if (In_moveIndex == 0 &&
		In_isCenterNode == true)
	{
		// 현재 목표 인덱스 찾기
		_reserveWaypointInfo._currentIndex = FindNextTargetNodeId(_reserveWaypointInfo._arrayPos);
	}
	else
	{
		// 저장
		_reserveWaypointInfo._currentIndex = In_moveIndex;
	}
	_reserveWaypointInfo._mapId = In_mapId;
	_reserveWaypointInfo._useAcceptanceRadius = In_useAcceptanceRadius;

	ProcessWaypoint();
}



// 다음 타겟 노드 id 구하기
int FGsWaypointWork::FindNextTargetNodeId(const TArray<FVector>& In_pos)
{
	FVector nowPos = _local->GetLocation();
	int posLen = In_pos.Num();
	// 최소값에 일단 제일 큰수 넣기
	float lineNodeMinDist = BIG_NUMBER;
	int nearLineNodeId = 0;
	bool isCenterLine = false;
	for (int i = 0; i < posLen; i++)
	{
		float resDist = -1.0f;
		FVector pos1 = In_pos[i];
		if (i < posLen - 1)
		{
			FVector pos2 = In_pos[i + 1];
			float resFactor = NearestPointFactor(pos1, pos2, nowPos);

			// 0과 1 사이일때만
			if (resFactor > 0 && resFactor <= 1)
			{
				// 라인에 투영된는 점
				FVector nearPoint = NearestPoint(pos1, pos2, nowPos);

				// 투영점과 나와의 제곱거리
				resDist = FVector::DistSquared(nearPoint, nowPos);

				if (resDist >= 0 &&
					resDist < lineNodeMinDist)
				{
					lineNodeMinDist = resDist;
					nearLineNodeId = i;
					isCenterLine = true;
				}
			}
		}
		// 노드와의 나의 거리
		resDist = FVector::DistSquared(pos1, nowPos);
		if (resDist > 0 &&
			resDist < lineNodeMinDist)
		{
			lineNodeMinDist = resDist;
			nearLineNodeId = i;
			isCenterLine = false;
		}
	}
	// 라인 중간이면 다음 노드
	// 아니면 해당 노드
	int targetIndex = (isCenterLine == true) ? nearLineNodeId + 1 : nearLineNodeId;

	// 소속 노드id 다음 놈이 목표점되어야 한다
	// 하지만 마지막 노드는 넘으면 안된다
	int targetId = FMath::Min((posLen - 1), targetIndex);
	return targetId;
}
// 선에서 제일 가까운 위치의 점 구하기(투영)
FVector FGsWaypointWork::NearestPoint(const FVector& In_startPos, const FVector& In_endPos, const FVector& In_nowPos)
{
	FVector lineDirection = In_endPos - In_startPos;
	lineDirection.Normalize();
	FVector nowPosDirection = In_nowPos - In_startPos;

	float closestPoint = FVector::DotProduct(nowPosDirection, lineDirection);
	return In_startPos + (closestPoint * lineDirection);
}
// 라인에서 비율값 구하기
float FGsWaypointWork::NearestPointFactor(const FVector& In_startPos, const FVector& In_endPos, const FVector& In_nowPos)
{
	FVector lineDirection = In_endPos - In_startPos;

	float magnitude = lineDirection.Size();

	bool isZeroSize = FMath::IsNearlyZero(magnitude);
	lineDirection = isZeroSize ? FVector::ZeroVector : (lineDirection / magnitude);

	FVector nowPosDirection = In_nowPos - In_startPos;
	float closestPoint = FVector::DotProduct(nowPosDirection, lineDirection);
	return (isZeroSize) ? 0.0f : (closestPoint / magnitude);
}


// 웨이포인트 진행
void FGsWaypointWork::ProcessWaypoint()
{
	// 끝에 도달했으면
	if (IsFinishedWaypoint() == true)
	{
		ArriveWaypoint();
		return;
	}
	// 다음 타겟 위치
	FVector targetPos = GetCurrentIndexReservePos();
	int mapId = _reserveWaypointInfo._mapId;

	if (IsInsideWaypointMovePosRange(_local, mapId, targetPos))
	{
	}
	else
	{
		// 이동
		OnMoveToWaypoint(mapId, targetPos);
		return;
	}
	// 범위 안에 도착이다
	// 마지막 위치면 way point 상태로 간다
	if (true == IsLastIndexReservePos())
	{
		ArriveWaypoint();
		return;
	}		
	// quest pos 1, 2 번위치가 같거나
	// 1번 도착지점에서 호출하면 다음으로 넘겨야 한다
	TryNextPoint();
}

// 목적지 도달
void FGsWaypointWork::ArriveWaypoint()
{
	if (_reserveWaypointInfo._funcSuccessDelegate.IsBound() == true)
	{
		_reserveWaypointInfo._funcSuccessDelegate.Execute();
	}

	// auto move 멈춤
	_local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);

	// 웨이포인트 도착 메시지 쏘기
	GMessage()->GetGameObject().SendMessage(MessageGameObject::WAYPOINT_MOVE_END, nullptr);

	// 클리어
	ClearReserveWaypointData();
}
// 데이터 클리어
void FGsWaypointWork::ClearReserveData()
{	
	ClearReserveWaypointData();	
}

// 최근 인덱스위 예약된 위치 구하기
const FVector& FGsWaypointWork::GetCurrentIndexReservePos()
{
	int arrLen = _reserveWaypointInfo._arrayPos.Num();
	if (arrLen == 0)
	{
		return FVector::ZeroVector;
	}

	int index = FMath::Min(arrLen - 1, _reserveWaypointInfo._currentIndex);
	return _reserveWaypointInfo._arrayPos[index];
}
// 웨이포인트 위치 이동 범위 안인가
bool FGsWaypointWork::IsInsideWaypointMovePosRange(
	UGsGameObjectBase* In_owner,
	int In_mapId,
	const FVector& In_targetPos)
{
	// 현재위치 맵id랑 목표랑 다르면
	int currMapId = GLevel()->GetCurrentLevelId();
	if (In_mapId != currMapId)
	{
		// 범위 밖
		return false;
	}

	if (In_owner != nullptr)
	{
		float distanceSq = FVector::DistSquared2D(In_targetPos, In_owner->GetLocation());

		if (GSAI()->GetIsShowMovetoPosRemainDist() == true)
		{
			float distance =  FMath::Sqrt(distanceSq);
			GSLOG(Log, TEXT("waypoint remain distance:%f"),distance );
		}

		float interactionRange = FGsMove::GetMovePosRange(true);

		if (distanceSq <= interactionRange * interactionRange)
		{
			return true;
		}
	}

	return false;
}

// 이동 성공
void FGsWaypointWork::CallbackMoveToEnd()
{	
	TryNextPoint();
}
// 다음 인덱스로 진행
void FGsWaypointWork::TryNextPoint()
{
	// 다음 스텝 진행
	if (IsReserveData())
	{
		// 인덱스 증가
		_reserveWaypointInfo._currentIndex += 1;
		ProcessWaypoint();
	}
	// 데이터가 없으니 끝내자
	else
	{
		// auto move 멈춤
		_local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);
		ClearReserveWaypointData();
	}
}
// 이동 실패
void FGsWaypointWork::CallbackMoveToFailed(const FPathFollowingResult& Result)
{
	if (_reserveWaypointInfo._funcFailDelegate.IsBound())
	{
		_reserveWaypointInfo._funcFailDelegate.Execute(Result);
	}
	// auto move 멈춤
	_local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);


	// 피격시 자동 이동 취소 되는 현상 수정하기 위해
	// 여기서 데이터 클리어 안함
	// 1. 수동 입력(이동, 스킬)시 클리어
	// 2. 다른 reserve data 입력시 클리어
	// https://jira.com2us.com/jira/browse/C2URWQ-6334
	
}

// 이동 요청 시작
void FGsWaypointWork::OnMoveToWaypoint(int In_mapId, const FVector& In_pos)
{
	if (_local)
	{
		FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>();

		float acceptanceRadius = FGsMove::GetMovePosRange();
		// -값 들어가면 이동 실패 되므로 최소값으로 세팅
		acceptanceRadius = FMath::Max(acceptanceRadius, 0.1f);

		// 성소 지도 이동에서 안을 파고 들어서
		// 별도 도착 거리세팅
		if (_reserveWaypointInfo._useAcceptanceRadius != 0.0f)
		{
			acceptanceRadius = _reserveWaypointInfo._useAcceptanceRadius;
		}

		// waypoint 이동 시작때 나머지 reserve 데이터 삭제(현재는 waypoint, input handler (터치 이동) 이동만 처리)
		// https://jira.com2us.com/jira/browse/C2URWQ-6334
		_local->ClearReserveDataByReserveWork(this);

		// 이전 이동 타입 복구
		// https://jira.com2us.com/jira/browse/C2URWQ-6334
		if(true == IsBranch(EGsGameClientBranchType::AUTO_MOVE_REMAIN_LENGTH_SHOW))
		{ 
			movement->RestoreBackupMovementAutoContentsType();
		}

		movement->ChangeAutoMoveState().StartMapIdPos(_local,
			In_mapId,
			In_pos, acceptanceRadius,
			FGsAutoMoveCompletedDelegate::CreateRaw(this, &FGsWaypointWork::CallbackMoveToEnd),
			FGsAutoMoveCompletedOneDelegate::CreateRaw(this, &FGsWaypointWork::CallbackMoveToFailed));
	}
}

// 끝났었는지(마지막 인덱스 넘어감)
bool FGsWaypointWork::IsFinishedWaypoint()
{
	int arrLen = _reserveWaypointInfo._arrayPos.Num();
	// 마지막 인덱스 보다 크면 이미 끝난거다
	return (_reserveWaypointInfo._currentIndex > arrLen - 1);
}

// 마지막 인덱스 인가
bool FGsWaypointWork::IsLastIndexReservePos()
{
	int arrLen = _reserveWaypointInfo._arrayPos.Num();
	// 마지막 인덱스 보다 크거나 같으면 끝이다
	return (_reserveWaypointInfo._currentIndex >= arrLen - 1);
}
// 갱신
void FGsWaypointWork::Update(float In_delta)
{
	if (_isDrawDebugPos == true)
	{
		UWorld* world = _local->GetWorld();
		if (world != nullptr)
		{
			int size = _reserveWaypointInfo._arrayPos.Num();
			for (int i = 0; i < size; i++)
			{
				FColor nowColor = (_reserveWaypointInfo._currentIndex == i) ? FColor::Red : FColor::Green;
				FVector pos = _reserveWaypointInfo._arrayPos[i] + FVector(0, 0, 50);
				// 시야 거리
				DrawDebugCircle(world, pos,
					100, 100, nowColor, false, 1.f,
					0, 0.0f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));

			}
		}
	}
}

// 웨이포인트 남은 거리
float FGsWaypointWork::GetWayPointMoveTargetDist()
{
	if (_local == nullptr)
	{
		return 0.0f;
	}

	if (_reserveWaypointInfo._arrayPos.Num() == 0)
	{
		return 0.0f;
	}
	int posArraySize = _reserveWaypointInfo._arrayPos.Num();

	FVector2D targetPos = FVector2D(_reserveWaypointInfo._arrayPos[posArraySize-1]);
	FVector2D localPos = FVector2D(_local->GetLocation());

	float remainDist = (localPos - targetPos).Size();

	return remainDist;
}
// 재시도 해라
void FGsWaypointWork::DoRetry()
{
	// idle 상태에서 처리
}
// 스킬 버튼 누름
void FGsWaypointWork::OnPressSkill(const FGsInputEventMsgBase& In_msg)
{
	// 예약된 데이터 삭제
	// https://jira.com2us.com/jira/browse/C2URWQ-6334

	// 신규 코드
	ClearReserveDataAndAutoMoveStop();
}
// 조이스틱 누름
void FGsWaypointWork::OnPressJoystick(const FGsInputEventMsgBase& In_msg)
{
	// 예약된 데이터 삭제
	// https://jira.com2us.com/jira/browse/C2URWQ-6334

	// 신규 코드
	ClearReserveDataAndAutoMoveStop();
}

// 스킬 클릭
void FGsWaypointWork::OnClickSkill(const FGsInputEventMsgBase& In_msg)
{
	// 예약된 데이터 삭제
	// https://jira.com2us.com/jira/browse/C2URWQ-6334

	// 신규 코드
	ClearReserveDataAndAutoMoveStop();
}

// 1차 타겟
void FGsWaypointWork::OnLocalTargetChanged(const IGsMessageParam* In_param)
{
	// 예약된 데이터 삭제
	// https://jira.com2us.com/jira/browse/C2URWQ-6334
	// 
	// 신규 코드
	ClearReserveDataAndAutoMoveStop();
}
// 예약 데이터 삭제와 auto move stop으로 상태 전환
// 스킬에서 특정 이동일때만 처리하는거 때문에 처리
// https://jira.com2us.com/jira/browse/C2URWQ-6334
void FGsWaypointWork::ClearReserveDataAndAutoMoveStop()
{	
	if(true == IsReserveData())
	{ 
		// 예약된 데이터 삭제
		// https://jira.com2us.com/jira/browse/C2URWQ-6334
		ClearReserveWaypointData();
	}
}

// waypoint 예약데이터 삭제
void FGsWaypointWork::ClearReserveWaypointData()
{
	_reserveWaypointInfo._currentIndex = 0;
	_reserveWaypointInfo._arrayPos.Empty();

	_reserveWaypointInfo._funcSuccessDelegate.Unbind();
	_reserveWaypointInfo._funcFailDelegate.Unbind();

	// 클리어
	_reserveWaypointInfo._useAcceptanceRadius = 0.0f;
}

// 다음맵 워프 못찾음
// https://jira.com2us.com/jira/browse/C2URWQ-6497
void FGsWaypointWork::OnAutoMoveCantFindNextMapWarp(const IGsMessageParam* In_param)
{
	ClearReserveDataAndAutoMoveStop();
}
// 캠프 hit 후 재이동(무조건 성공용)시 삭제되어야 될것 처리
// https://jira.com2us.com/jira/browse/C2URWQ-6363
void FGsWaypointWork::OnAutoMoveCampHitRestart(const IGsMessageParam* In_param)
{
	int arrLen = _reserveWaypointInfo._arrayPos.Num();
	// 강제로 마지막단계로 왔다 치고
	_reserveWaypointInfo._currentIndex =  arrLen;
}