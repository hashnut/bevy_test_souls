#pragma once

#include "GameObject/Waypoint/GsReserveWaypointInfo.h"
#include "GameObject/Reserve/GsReserveWork.h"
#include "Message/GsMessageInput.h"
#include "Message/GsMessageGameObject.h"
#include "Input/GsInputEventMsgBase.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"
/*
	여러개 포인트 이동
	- 현재는 퀘스트 위치 이동시 2개 지점을 이동할때 쓴다
	- 몬스터 사냥 1개 지점 이동에도 씀
*/
class UGsGameObjectLocalPlayer;

struct IGsMessageParam;

class FGsWaypointWork :
	public IGsReserveWork
{
private:
	// 예약 웨이 포인트 정보
	FGsReserveWaypointInfo _reserveWaypointInfo;
	UGsGameObjectLocalPlayer* _local = nullptr;
	// 디버그 위치 그릴지 
	bool _isDrawDebugPos = false;

	// 인풋 메시지
	TArray<TPair<MessageInput, FDelegateHandle>> _actionDelegates;
	// game object 메세지 해제용
	MsgGameObjHandleArray _gameObjectMsg;
public:
	// 데이터 예약이 있는가
	virtual bool IsReserveData() override;
	// 재시도 해라
	virtual void DoRetry() override;

	// 예약된 데이터 클리어해라
	virtual void ClearReserveData() override;

	// 메시지 바인딩
public:
	// input
	// 스킬 버튼 누름
	void OnPressSkill(const FGsInputEventMsgBase& In_msg);
	// 조이스틱 누름
	void OnPressJoystick(const FGsInputEventMsgBase& In_msg);
	// 스킬 클릭
	void OnClickSkill(const FGsInputEventMsgBase& In_msg);
	// 1차 타겟
	void OnLocalTargetChanged(const IGsMessageParam* In_param);
	// 다음맵 워프 못찾음
	// https://jira.com2us.com/jira/browse/C2URWQ-6497
	void OnAutoMoveCantFindNextMapWarp(const IGsMessageParam* In_param);
	// 캠프 hit 후 재이동(무조건 성공용)시 삭제되어야 될것 처리
	// https://jira.com2us.com/jira/browse/C2URWQ-6363
	void OnAutoMoveCampHitRestart(const IGsMessageParam* In_param);

	// 로직 함수
public:

	// 초기화
	void Initialize(UGsGameObjectLocalPlayer* In_owner);
	// 종료
	void Finalize();
	// 갱신
	void Update(float In_delta);
	// 웨이포인트 시작
	void StartWaypointData(int In_mapId, const TArray<FVector>& In_pos, int In_moveIndex = 0,
		bool In_isCenterNode = false,
		const FGsAutoMoveCompletedDelegate& inSuccessDelegate = nullptr,
		const FGsAutoMoveCompletedOneDelegate& inFailDelegate = nullptr, 
		float In_useAcceptanceRadius = 0.0f);

	// 다음 타겟 노드 id 구하기
	int FindNextTargetNodeId(const TArray<FVector>& In_pos);
	// 선에서 제일 가까운 위치의 점 구하기(투영)
	FVector NearestPoint(const FVector& In_startPos, const FVector& In_endPos, const FVector& In_nowPos);
	// 라인에서 비율값 구하기
	float NearestPointFactor(const FVector& In_startPos, const FVector& In_endPos, const FVector& In_nowPos);

	// 웨이포인트 진행
	void ProcessWaypoint();
	// 목적지 도달
	void ArriveWaypoint();

	// 이동 성공
	void CallbackMoveToEnd();
	// 이동 실패
	void CallbackMoveToFailed(const FPathFollowingResult& Result);
	// 이동 요청 시작
	void OnMoveToWaypoint(int In_mapId, const FVector& In_pos);
	// 다음 인덱스로 진행
	void TryNextPoint();
	// 예약 데이터 삭제와 auto move stop으로 상태 전환
	// 스킬에서 특정 이동일때만 처리하는거 때문에 처리
	// https://jira.com2us.com/jira/browse/C2URWQ-6334
	void ClearReserveDataAndAutoMoveStop();
	// waypoint 예약데이터 삭제
	void ClearReserveWaypointData();

	// get, set
public: 
	// 끝났었는지(마지막 인덱스 넘어감)
	bool IsFinishedWaypoint();
	// 마지막 인덱스 인가
	bool IsLastIndexReservePos();
	// 최근 인덱스위 예약된 위치 구하기
	const FVector& GetCurrentIndexReservePos();
	// 웨이포인트 위치 이동 범위 안인가
	bool IsInsideWaypointMovePosRange(
		class UGsGameObjectBase* In_owner,
		int In_mapId,
		const FVector& In_targetPos);

	void SetIsDrawDebugPos(bool In_val) { _isDrawDebugPos = In_val; }
	// 웨이포인트 남은 거리
	float GetWayPointMoveTargetDist();

	int GetReserveMapId()
	{
		return _reserveWaypointInfo._mapId;
	}
};