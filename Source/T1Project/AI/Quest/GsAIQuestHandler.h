#pragma once

#include "CoreMinimal.h"
#include "Message/GsMessageGameObject.h"
#include "AI/Data/GsAIDefine.h"
#include "AI/Quest/GsReserveQuestKillMoveInfo.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../../GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"

struct IGsMessageParam;
class UGsGameObjectLocalPlayer;
class FGsQuestAutoMoveResolver;
class FGsQuestAutoMoveTask;
/*
	ai 퀘스터 관련 처리
*/
class FGsAIQuestHandler
{
	// 멤버 변수
private:
	// 자동 진행 할 퀘스트 id
	QuestId _autoActiveQuestId = 0;
	// 타겟 npc id
	int _targetNpcId = 0;
	// 이동할 위치 map id
	int _moveToMapId = 0;
	// 이동할 위치
	TArray<FVector> _movePos;
	// 이동할때 유지할 거리(quest kill 일때 사용)
	float _acceptanceRange = 0.0f;
	// 메시지 델리게이트
	MsgGameObjHandleArray _msgDelegates;
	UGsGameObjectLocalPlayer* _local = nullptr;
	// 퀘스트 자동진행 타입
	QuestObjectiveType _questAutoMoveType;
	// 노드 중간으로 갈지
	bool _isCenterNode = false;
	// 퀘스트 kill 이동 예약 정보
	FGsReserveQuestKillMoveInfo _reserveQuestKillMoveInfo;
	FGsReserveQuestKillCenterMoveInfo _reserveQuestKillCenterMoveInfo;

	// 퀘스트 자동 이동 조건, 행동 관련 처리
	FGsQuestAutoMoveResolver* _resolver = nullptr;
	// 강제 이동 요청 있었나
	bool _isDirtyForceMove = false;
	// 이동 실패 이유
	EGsMoveFailedReason _moveFailedReason = EGsMoveFailedReason::None;
	// path 이동 실패 이유
	EPathFollowingResult::Type _moveFailedPathResult;
	// npc 대기 시작했는지
	bool _isNPCWaitStart = false;
	// npc 대기 시간 끝났는지
	bool _isNPCWaitTimeOver = false;
	// npc 대기 시작 시간
	int64 _npcWaitStartTime = 0;
	// 이동 실패 시간
	int64 _moveFailedTime = 0;
	// 방금 이동 실패 했는가
	bool _isDirtyFailedMove = false;
	// auto off 상태에서 루팅이 있었나(퀘스트 이동중 줍기)
	// 루팅후 다시 이동을 해야 함
	// https://jira.com2us.com/jira/browse/C2URWQ-8303
	bool _isAutoOffLooting = false;


	// check quest kill auto
	bool _isQuestKillAutoStart = false;
	FVector _questKillCenterPos;	

	bool _isDrawKillCenterMove = false;

	bool _isQuestKillCenterMove = false;


	float _killCenterCheckSqVal = 0.0f;

	// 로직 함수
public:
	// 초기화
	void Initialize();
	// 클리어
	void Finalize();
	// 캐릭터 세팅
	void SetCharacter(UGsGameObjectLocalPlayer* In_local);
	// 캐릭터 지움
	void RemoveCharacter();
	// 업데이트
	void Update(float In_delta);
	// 퀘스트 이동 시작
	void StartQuestAutoMove();
	// 데이터 클리어
	void ClearData();
	// 예약된 quest kill 이동 진행
	void ProcessReserveQuestKillMove();
	void ProcessReserveQuestKillCenterMove();
	// 예약된 정보 클리어
	void ClearReserveQuestKillMoveData();
	void ClearReserveQuestKillCenterMoveData();
	// 퀘스트 kill 이동 시작
	void StartQuestKillMove(int In_mapId, const FVector& In_pos, float In_acceptanceRadius);
	void StartQuestKillCenterMove(const FVector& In_pos, float In_acceptanceRadius);
	// 퀘스트 kill 이동 도착 처리
	void ArriveQuestKillMove();
	void ArriveQuestKillCenterMove();
	// 테스트 정보 만들기
	void MakeQuestAutoMoveTaskInfo();
	// 이동 실패
	void FailedAutoMove(EGsMoveFailedReason In_reason, EPathFollowingResult::Type In_pathResult);
	// task들 체크
	void EvaluateConditions();
	// task 체크
	bool EvaluateCondition(const FGsQuestAutoMoveTask* In_task);

	// 이벤트
public:
	// 퀘스트 자동 이동 시작
	void OnQuestAutoMoveStart(const IGsMessageParam* In_data);
	// 인터랙션 준비됨 메시지(도착)
	void OnInteractionContents(const IGsMessageParam* In_data);
	// 웨이 포인트 이동 끝 메시지(도착)
	void OnWaypointMoveEnd(const IGsMessageParam*);
	// 퀘스트 자동 이동 중지(퀘스트 hud toggle로 끔)
	void OnQuestAutoMoveStop(const IGsMessageParam*);
	// 자동이동 실패(인터랙션에서 쏘는 메시지)
	void OnAutoMoveFailed(const IGsMessageParam* In_data);	

	// call back
public:
	
	// 퀘스트 이동 실패
	void CallbackQuestMoveFailed(const FPathFollowingResult& Result);
	// 퀘스트 kill 이동 성공
	void CallbackQuestKillMoveEnd();
	void CallbackQuestKillCenterMoveEnd();
	// 퀘스트 kill 이동 실패
	void CallbackQuestKillMoveFailed(const FPathFollowingResult& Result);
	void CallbackQuestKillCenterMoveFailed(const FPathFollowingResult& Result);

	// 조건 함수
public:
	// 퀘스트 자동 진행중인가
	bool IsAutoProgressing();
	// 예약된 퀘스트 이동 데이터 있는가
	bool IsReserveQuestMoveData();
	// 로컬 상태 idle 인가
	bool IsLocalStateIdle();
	// 강제 이동 요청 있었는가
	bool IsDirtyForceStart();
	// 인터랙션 대상 npc 없는지
	bool IsNPCInteractionTargetNotExist();
	// npc 대기 시작 했는가
	bool IsNPCWaitStart();
	// 취소에 의한 실패인가
	bool IsAbortFaield();
	// 재시도 가능한 실패 결과인지
	bool IsRetryPossibleFailedResult();
	// npc 대기시간 끝났는지
	bool IsNPCWaitTimeOver();
	// path 에러고 취소가 아닌지
	bool IsPathAndNotAbortResult();
	// 최근 이동 실패 한적이 있나
	bool IsDirtyMoveFailed();
	// 수동 공격 타겟(죽을때 까지 때릴)이 있는지
	bool IsManualAutoTargetExist();
	// check loot possible
	bool IsLootPossible();

	bool IsQuestKillAutoStart();
	bool IsEnemyExist();
	bool IsQuestKillCenterInside();

	bool IsServerLock();

	// 행동 함수
public:
	// 퀘스트 이동 시작
	void DoQuestMoveStart();
	// 퀘스트 이동 정지
	void DoQueatMoveStop();
	// npc 스폰 대기 시작
	void DoNPCSpawnWaitStart();
	// 이동 실패고 에러 팝업 출력(이동 중지)
	void DoShowMoveFailedErrorPopup();
	// quest monster kill center move
	void DoQuestMonsterKillCenterMove();

	// get, set
public:
	void SetIsCenterNode(bool In_val) { _isCenterNode = In_val; }
	bool GetIsCenterNode() { return _isCenterNode; }
	// 퀘스트 kill 이동 예약된게 있나
	bool IsReserveQuestKillMoveData();
	// 퀘스트 kill 이동 범위 안인가
	bool IsInsideQuestKillMovePosRange(int In_mapId, const FVector& In_targetPos, 
		float In_acceptanceRadius);

	bool IsInsideQuestKillCenterMovePosRange(const FVector& In_targetPos,
		float In_acceptanceRadius);

	QuestObjectiveType GetQuestAutoMoveType()
	{
		return _questAutoMoveType;
	}
	void SetDrawKillCenterMove(bool In_val)
	{
		_isDrawKillCenterMove = In_val;
	}

	bool IsQuestKillCenterMove()
	{
		return _isQuestKillCenterMove;
	}

	int GetQuestMoveTargetNpcId()
	{
		return _targetNpcId;
	}
	// auto off 루팅 했는가
	void SetIsAutoOffLooting(bool In_val)
	{
		_isAutoOffLooting = In_val;
	}
};