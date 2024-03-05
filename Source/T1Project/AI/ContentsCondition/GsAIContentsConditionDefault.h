#pragma once
#include "AI/Data/GsAIDefine.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageSystem.h"
#include "Message/MessageParam/GsMessageParam.h"
//---------------------------------
// ai 컨텐츠 조건: 이동, 공격, 선공 기본적인것
//---------------------------------

class UGsGameObjectLocalPlayer;
class FGsAIManager;
class UGsGameObjectBase;
struct FGsGameObjectMessageParamBase;

// 이동 데이터
struct FGsAIMoveData : public IGsMessageParam
{
	FVector _movePos;
	float _acceptanceDist;
};

class FGsAIContentsConditionDefault
{
	// 멤버 변수들
private:
	// 타겟 id(타겟)
	int _targetId;
	// 이동할 위치(이동)
	FVector _movePos;
	// 타겟 거리(이거리면 도착으로 침)(이동)
	float _moveAcceptanceDist;

	// 이동요청이 있는가?
	bool _isRequestMove = false;

	// 선공, 비선공 처리
private:
	// 선공인가-> 내가 몬스터를 먼저 치는가?
	bool _isBattingFirst = true;

	// 스킬 예약
private:
	// 스킬 예약중인가?
	bool _hasReservedSlot = false;

	// 적 탐색 거리 출력
private:
	bool _isDrawSearchRange = false;

	// 자동 이동 실패 출력
private:
	bool _isShowAutoMoveFail = false;

	// loot server response check
private:
	bool _isLootServerResponseWait = false;

	// 자동 옵션
private:
	// 자동 줍기 등급
	EGsAIAutoLootDropItemGradeType _autoLootDropItemGrade = EGsAIAutoLootDropItemGradeType::All;

	// 퀘스트 자동이동후 드랍아이템(마지막 아이템)
private:
	// 내 드랍한 아이템
	TArray<int64> _arrayMyMissingItem;

	// 참조 데이터
private:
	// 플레이어
	UGsGameObjectLocalPlayer* _localPlayer;
	FGsAIManager* _aiManager;

	// 메시지 바인딩 해제용
private:
	// gameobjectg msg 모음
	MsgGameObjHandleArray _gameObjectMsg;
	// 시스템 no param msg 모음
	MsgSystemHandleArray _msgSystemNoParamDelegates;


	// auto loot cool time
private:
	bool _isAutoLootCooltime = false;	
	int64 _autoLootBlockStartTime = 0;

	// 서버 응답 대기 시작시간
	int64 _lootServerResponseWaitStartTime = 0;
	// 서버 응댑 대기 클리어 틱 타임
	float _lootServerResponseWaitClearTick;
	// 생성자, 소멸자
public:
	~FGsAIContentsConditionDefault();

	// 로직 함수
public:
	// 초기화
	void Initialize(FGsAIManager* In_mng);
	// 해제
	void Finalize();
	void SetCharacter(UGsGameObjectLocalPlayer* In_player);
	void RemoveCharacter();

	void Update();

	void StartAutoLootCooltime();

	// 메시지 바인딩
public:
	// 선공 on, off(타겟)
	void OnBattingFirst(const IGsMessageParam* In_isActive);
	// 이동 요청
	void OnRequestMove(const IGsMessageParam* In_moveData);
	// 검색 범위 그릴지 세팅
	void OnSetDrawSearchRange(const IGsMessageParam* In_isActive);
	// 자동 이동 실패 출력할지 세팅
	void OnSetShowAutoMoveFail(const IGsMessageParam* In_isActive);
	// 줍기 (등급) 변경시 호출
	void OnSetDropItemGrade(const IGsMessageParam* In_isActive);
	// 드랍 오브젝트 디스폰
	void OnDropObjectDespawn(const IGsMessageParam* In_param);
	// https://jira.com2us.com/jira/browse/C2URWQ-2421
	// 재연결후 ai에서 아이템 주으러 안가는 이슈
	// 재연결 성공시
	void OnGameServerReconnectionSuccess();

	// ai 매니저가 받은 메시지 전달
public:
	// 스킬 버튼 누름
	void OnPressSkill();
	// 조이스틱 누름
	void OnPressJoystick();
	// ai load player spawn complete call
	void OnLoadPlayerSpawnComplete();

	// 체크 함수
public:
	// 위치 도착했는가 (이동)
	bool IsArrivePos();
	// 타겟 있는가 (타겟)
	bool IsTargetExist();
	// 공격 범위 안쪽 인가 (공격)
	bool IsInAttackRange();
	// 공격 범위 안쪽 인가 , 타겟(공격)
	bool IsInAttackRangeTarget(UGsGameObjectBase* In_target);
	// 게임 오브젝트 fsm 이 idle 인지(공격)
	bool IsGameObjectFSMIdle();
	// 시야내 적이 있는지(공격)
	bool IsInSearchRangeAnyoneEnemy();
	// 시야내 아이템이 있는지(루팅)
	// 인벤토리 루팅 가능한지
	bool IsInSearchRangeItem();
	// 채집(Prop) 퀘스트 대상이 있는지
	bool IsSearchRangePropObject();
	// skill useable state check
	bool IsGameObjectSkillUseableState();
	// quest auto active check
	bool IsQuestAutoPlay();
	// counter attack option check
	bool IsCounterAttackOptionOn();
	// counter attack target check
	bool IsCounterAttackTargetExist();
	// auto loot check
	bool IsAutoLootCooltime();
	// quest auto loot check
	bool IsQuestAutoLoot();
	// server response check
	bool IsLootServerResponseWait();
	// 1. first job loot target weight over(add current total weight)
	// 2. inven slot check
	bool IsLootImpossible();
	// check dialog play
	bool IsDialogPlay();
	// check input block
	bool IsInputBlock();
	// exist reserve looting data(proceeding looting)
	bool IsReserveLooting();
	// check move type: auto return move
	bool IsAutoReturnMove();
	// check auto move
	bool IsGameObjFSMAutoMove();
	// check quest center move
	bool IsQuestCenterMove();
	// check stay long time
	bool IsStayLongTime();
	// check first job == retry
	bool IsFirstJobRetry();
	// check: server move lock
	bool IsServerMoveLock();
	// check: target attackable
	bool IsAttackable();
	// check: 이미 중지 및 스킬 clear 액션이 등록되어있는지
	bool IsReservedActionAutoMoveStopWithClearReserveSkill();

	// get, set
public:
	// 거리값 세팅(이동)
	void SetAcceptanceDist(float In_dist) { _moveAcceptanceDist = In_dist; }
	// 거리값 가져오기(이동)
	float GetAcceptanceDist() { return _moveAcceptanceDist; }
	// 이동할 위치 저장(이동)
	void SetMovePos(const FVector& In_pos) { _movePos = In_pos; }
	// 이동할 위치 가져오기(이동)
	FVector GetMovePos() { return _movePos; }
	// 타겟 id 가져오기(타겟)
	int GetTargetId() { return _targetId; }
	// 타겟 id 저장(타겟)
	void SetTargetId(int In_id) { _targetId = In_id; }
	// 이동 요청이 있었나
	bool GetIsRequestMove() { return _isRequestMove; }
	// 이동 요청 세팅
	void SetIsRequestMove(bool In_val) { _isRequestMove = In_val; }
	// 사망이 아닌지 체크 하는 함수 가져오기
	TFunction<bool(UGsGameObjectBase * obj)> GetNotDeadCheckFunc();
	// 선공
public:
	// 선공인지 가져오기(타겟)
	bool GetIsBattingFirst() { return _isBattingFirst; }
	// 선공인지 세팅(타겟)
	void SetIsBattingFirst(bool In_val) { _isBattingFirst = In_val; }

	// 스킬예약
public:
	// 스킬 예약 중인가?
	bool HasReservedSlot() { return _hasReservedSlot; }

	// 적 탐색 거리 출력
public:
	bool GetIsDrawSearchRange() { return _isDrawSearchRange; }

	// 자동 이동 실패 출력
public:
	bool GetIsShowAutoMoveFail() { return _isShowAutoMoveFail; }
	

	// 자동 줍기 등급
public:
	EGsAIAutoLootDropItemGradeType GetAutoLootDropItemGrade() {
		return _autoLootDropItemGrade;
	}
	void SetAutoLootDropItemGrade(EGsAIAutoLootDropItemGradeType In_type)
	{
		_autoLootDropItemGrade = In_type;
	}

	// 퀘스트 자동이동후 드랍아이템(마지막 아이템)
public:
	bool GetIsMyMissingItemExist()
	{
		return (_arrayMyMissingItem.Num() != 0) ? true : false;
	}
	void AddMissingMyDropObjectId(int64 In_gameId);

public:
	// 루팅 서버 send 보내고 응답 대기 할지
	void SetIsLootServerResponseWait(bool In_val);
};