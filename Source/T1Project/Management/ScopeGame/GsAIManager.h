#pragma once

#include "Classes/GsManager.h"

#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"

#include "AI/Data/GsAIDefine.h"
#include "AI/Movement/GsNextMapMoveInfo.h"

#include "AI/Data/EGsAIContentsType.h"

#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"

#include "Message/GsMessageInput.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageSystem.h"
#include "Message/GsMessageStage.h"

#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"

class FGsAIStateManager;
class FGsAITaskResolver;
class UGsGameObjectBase;
class UGsGameObjectLocalPlayer;
class FGsInputEventMsgBase;
class FGsAIContentsConditionDefault;
class FGsAIContentsConditionAttOrder;
class FGsAIContentsConditionAutoReturn;
class FGsAIContentsConditionAutoOff;
class FGsAIQuestHandler;
class FGsMannerModeInfo;

struct IGsMessageParam;
//-----------------------------------------
// ai 관리
//-----------------------------------------

class FGsAIManager :
	public IGsManager
{
	// 매니징할 데이터
private:
	// 상태 관리자
	FGsAIStateManager* _stateManager = nullptr;
	// 컨텐츠별 데이터
	TMap<EGsAIContentsType, FGsAITaskResolver*> _mapTaskResolver;
	// 퀘스트 관련 처리
	FGsAIQuestHandler* _questHandler = nullptr;
	// 다음맵 이동 정보
	FGsNextMapMoveInfo _nextMapMoveInfo;

	// 다음 맵 이동 예약 검(서버 immediate stop 으로 인해 이동이 멈춰지므로, 워프 연출)
	bool _isDirtyNextMapMove = false;

	// 참조 데이터
private:
	// 플레이어
	UGsGameObjectLocalPlayer* _localPlayer = nullptr;

	// 외부에서 세팅한 데이터
private:
	// 현재 컨텐츠 타입
	EGsAIContentsType _currentContentsType = EGsAIContentsType::Auto_off;
	// 진행중 ai 멈출지(연출 등)
	bool _isAIStop = false;
	// ai on 인지(자동전투 on/off)
	// warp시 local이 aimanager init 시키는데 ui 는 그대로 있음
	// static 으로 바꿔서 유지하게 처리
	// 매니저단으로 옮겨서 none static으로 변경
	bool _isAIOn = false;
	// 스킬 버튼 눌렸는가
	bool _isSkillBtnPressed = false;
	// 조이스틱 눌렸는가
	bool _isJoystickPressed = false;
	// 캐릭터 초기화 했는지?
	bool _isInitCharacter = false;
	// 스톱 쓸지 말지
	bool _isUseStop = true;

	// npc list 이동 시작 했는가
	bool _isNpcListMoveStart = false;

	// 스킬 전송 시작했는가(매너모드)
	// 응답시 나보다 타겟에 먼저 hit 들어가면 
	// 매너 대상
	bool _isSkillStartSend = false;
	// 스킬 응답 
	bool _isSkillStartRecv = false;
	// 매너 체크할 타겟 id
	int64 _mannerCheckTargetId = 0;
	// 매너모드(전투 옵션)
	EGsAIBattleOptionMannerMode _mannerMode = EGsAIBattleOptionMannerMode::OFF;

	// 매너 대상 정보
	TArray<FGsMannerModeInfo*> _arrayMannerTargetInfo;

	// 수동 이동시, 자동 전투 유지
	bool _isNonAutoMoveKeepAuto = true;
	
	// safety zone check flag
	bool _isSafetyZone = false;

	bool _isDrawDebugPath = false;
	// 디버그 예약 리스트 보여질지
	bool _isShowDebugReserveList = false;

	// 조건 체크 관련 클래스
private:
	// 컨텐츠 조건: 기본 동작
	FGsAIContentsConditionDefault* _contentsConditionDefault = nullptr;
	// 컨텐츠 조건: 공격 우선 순위
	FGsAIContentsConditionAttOrder* _contentsConditionAttOrder = nullptr;
	// 컨텐츠 조건: 제자리 돌아가기
	FGsAIContentsConditionAutoReturn* _contentsConditionAutoReturn = nullptr;
	// 컨텐츠 조건: 자동 off
	FGsAIContentsConditionAutoOff* _contentsConditionAutoOff = nullptr;

	// 메시지 (해지용)
private: 
	TArray<TPair<MessageInput, FDelegateHandle>> _actionDelegates;
	MsgGameObjHandleArray _aiDelegates;
	MsgSystemHandleArray _systemDelegates;
	MsgSystemHandleArray _systemParamDelegates;
	TArray<MsgStageHandle> _msgStageHandleList;
	
	// test
private:
	// 로그 출력할지
	bool _isShowLog = false;
	// ai 공격 체크 반경 보여줄지
	bool _isShowAIAttackCheckRadius = false;
	// 로드 투포 시작인지(load complete에서 해제)
	bool _isStartLoadToPo = false;
	// move to pos (waypoint) 남은 거리 찍을지
	bool _isShowMovetoPosRemainDist = false;

	// 생성자, 소멸자
public:
	virtual ~FGsAIManager();

	// 가상 함수
public:
	// 초기화(처음 한번만)
	virtual void Initialize() override;
	// 해제
	virtual void Finalize() override;
	// 갱신
	virtual void Update(float In_delta) override;

	// 로직 함수
public:
	// 초기화
	void SetCharacter(UGsGameObjectLocalPlayer* In_player);
	// 캐릭터 삭제 될때
	void RemoveCharacter();
	
private:
	// ai 데이터 로드
	void LoadAIData();
	// 조건 함수 연결
	void RegisterConditionFunc();
	// 메시지 바인딩 처리
	void BindMessage();
	// ai 킬지 말지
	void SetAIOnOff(bool In_isOn);

public:
	// 컨텐츠 타입 변경
	void ChangeContentsType(EGsAIContentsType In_type);
	// ai on, off 강제 세팅(외부로 메시지 전파한다)
	void SetActiveAI(bool In_isActive);
	// ai on off 토글
	void ToggleAIOnOff();
	// ai 멈출지(내부적으로 멈춤, 컷신 같은거, 버튼은 그대로)
	// 나중에 끝나면 그전상태로 돌아가기 때문에 업데이트만 막는다
	void SetAIStop(bool In_val);

	// 자동 이동(다음맵 진행)
	void MoveToPosAuto(EGsMovementAutoContentsType In_type, int In_mapId, 
		const FVector& In_pos, bool In_isAutoOff = true,
		float In_useAcceptanceRadius = 0.0f);

	// 자동 이동(현재맵 진행)
	void MoveToPosAutoCurrentMap(EGsMovementAutoContentsType In_type,
		const FVector& In_pos, bool In_isAutoOff = true);

	void CallbackMoveToPosEnd();
	void CallbackMoveToPosFailed(const FPathFollowingResult& Result);

	// 스킬 시작 SEND
	void SendStartSkill(int64 In_targetId);
	// 스킬 시작 RECV remote
	void RecvStartSkillRemote(int64 In_targetId, int64 In_remoteId);
	// 스킬 시작 RECV local
	void RecvStartSkillLocal(int64 In_targetId);
	// 매너 체크 타겟 추가
	void AddMannerCheckTarget(int64 In_targetId, int64 In_remoteId);
	// 매너 체크 타겟 삭제
	void RemoveMannerCheckTarget(int64 In_targetId);
	// 매너 타겟 오너 삭제
	void RemoveMannerTargetOwnerRemote(int64 In_remoteGameId);
	// 매너 체크 타겟 초기화
	void ClearMannerCheckTarget();
	// 터치 무브 이동 시작
	void StartTouchMove();
	// 매너 모드 정보 얻기(몬스터 game id로)
	FGsMannerModeInfo* FindMannerModeInfoByMonsterId(int64 In_gameId);
	// 매너 모드 정보 얻기(타유저 game id로)
	FGsMannerModeInfo* FindMannerModeInfoByOwnerRemoteId(int64 In_gameId);
	// req auto move cool time check
	// 1. fsm: auto move
	// 2. auto move contents type: loot item move
	void RequestAutoLootCooltime();

	void NpcListMoveFailed(EGsMoveFailedReason In_reason, int In_npcTblId = -1);
	// load complete에서 워프 연출있다면 이동대기 끝나고 할처리
	void SetNextMapMoveWait();
	// 지연된 다음맵 이동 시도
	void TryDelayedNextMapMove();

	// 메시지 바인딩
public:
	// input
	// 스킬 버튼 누름
	void OnPressSkill(const FGsInputEventMsgBase& In_msg);
	// 스킬 버튼 땜
	void OnReleaseSkill(const FGsInputEventMsgBase& In_msg);
	// 조이스틱 땜
	void OnReleaseJoystick(const FGsInputEventMsgBase& In_msg);
	// 조이스틱 누름
	void OnPressJoystick(const FGsInputEventMsgBase& In_msg);
	void OnLocalTargetChanged(const IGsMessageParam* In_param);
	// 로컬 사망
	// 내가 죽으면 auto 끄자(바로 npc 공격해서 불편하다는 건의 사항옴)
	void OnLocalPlayerDie(const IGsMessageParam*);
	// 로컬 타겟 클리어
	void OnLocalTargetClear(const IGsMessageParam*);
	// 로컬 스폰 페이드 인 끝
	void OnLocalSpawnFadeInEnd(const IGsMessageParam*);
	// stat load topo 시작
	void OnLoadTopoStart(const IGsMessageParam* inParam);

	void OnSequencePlayerStart(const IGsMessageParam* inParam);
	void OnSequencePlayerEnd(const IGsMessageParam* inParam);

	// 자동 이동 다음 워프 없을때 정보 지우는 처리(남아있으면 워프후 다시 시도하는 이슈 수정)
	// https://jira.com2us.com/jira/browse/C2URWQ-6497
	void OnAutoMoveCantFindNextMapWarp(const IGsMessageParam* In_param);
	// 네트워트 끊김
	// 네트워크 끊길때 자동 이동 멈추게 처리
	// https://jira.com2us.com/jira/browse/C2URWQ-2208
	void OnNetDisconected();

	// ai
public:
	// ai on, off toggle(외부로 메시지 전파한다)
	void OnActiveToggleAI(const IGsMessageParam*);
	// 컨텐츠 변경 casting: EGsAIContentsType
	void OnContentsChange(const IGsMessageParam* In_contentsType);
	// ai stop 플래그 쓸지 말지 toggle
	void OnStopCheckToggle(const IGsMessageParam* In_isActive);
	// 인터랙션 준비됨 메시지(도착)
	void OnInteractionContents(const IGsMessageParam* In_data);

	// 퀘스트 자동 이동 시작
	void OnQuestAutoMoveStart(const IGsMessageParam* In_data);
	// 로컬 스폰 끝
	void OnLocalPlayerSpawnComplete(const IGsMessageParam*);
	// 특정 테리토리영역 진입
	void OnChangeSafetyMode(const IGsMessageParam* InParam);
	// 인터랙션 버튼 클릭(ai 끄자)
	void OnInteractionButtonClick(const IGsMessageParam* In_param);

	// npc list 이동
public:
	// 자동 이동 실패
	void OnAutoMoveFailed(const IGsMessageParam* InParam);

	// log
public:
	// 출력할지 세팅
	void SetIsShowLog(bool In_val);
	// 출력할지 여부 가져오기
	bool GetIsShowLog() const { return _isShowLog; }

	bool GetIsShowAIAttackCheckRadius() { return _isShowAIAttackCheckRadius; }
	void SetIsShowAIAttackCheckRadius(bool In_val) { _isShowAIAttackCheckRadius = In_val; }
	// 디버그용 예약 리스트 갱신 요청
	void SendDebugReserveListUpdate();
public:
	// 스테이트 매니저 접근
	FGsAIStateManager* GetAIStateManager() const { return _stateManager; }
	
	FGsAIQuestHandler* GetAIQuestHandler() { return _questHandler; }
	// 컨텐츠 조건
public:
	// 기본들(아니면 어디두기 애매한거)
	FGsAIContentsConditionDefault* GetContentsConditionDefault() const { return _contentsConditionDefault; }
	// 공격 우선 순위
	FGsAIContentsConditionAttOrder* GetContentsConditionAttOrder() const { return _contentsConditionAttOrder; }
	// 제자리 돌아가기 
	FGsAIContentsConditionAutoReturn* GetContentsConditionAutoReturn() const { return _contentsConditionAutoReturn; }
	// 수동
	FGsAIContentsConditionAutoOff* GetContentsConditionAutoOff() const { return _contentsConditionAutoOff; }

public:
	// 조작이 있는가(수동 조작)
	bool IsThereInput() const;
	// ai를 플레이 해도 되는지
	bool CanAIPlay() const;
	// 오토 켰는지
	bool IsAIOn() const { return _isAIOn; }

	void SetIsShowMovetoPosRemainDist(bool In_val)
	{
		_isShowMovetoPosRemainDist = In_val;
	}
	bool GetIsShowMovetoPosRemainDist()
	{
		return _isShowMovetoPosRemainDist;
	}
	void SetNpcListMoveStart(bool In_val)
	{
		_isNpcListMoveStart = In_val;
	}
	// 매너모드 세팅
	void SetMannerMode(EGsAIBattleOptionMannerMode In_mode);
	// 매너 타겟인가
	bool GetIsMannerTarget(UGsGameObjectBase* In_object);

	void SetNonAutoMoveKeepAuto(bool In_value)
	{
		_isNonAutoMoveKeepAuto = In_value;
	}

	void SetNextMapWarpStart();
	
	bool GetIsLootTargetExistAndPossible();
	// auto return dist
	float GetReturnDist();
	bool GetIsAutoLootCoolTime();
	// 서버 이동 lock 상태인가
	bool IsServerMoveLock();

	void SetIsDrawDebugPath(bool In_val)
	{
		_isDrawDebugPath = In_val;
	}

	bool GetIsDrawDebugPath()
	{
		return _isDrawDebugPath;
	}
	// 디버그 예약 리스트 show
	void ShowDebugReserveList(bool In_isShow);
	// 디버그 예약 리스트 show 할지
	bool IsShowDebugReserveList()
	{
		return _isShowDebugReserveList;
	}
};

#define GSAI() UGsScopeHolder::GetGameManagerFType<FGsAIManager>(UGsScopeGame::EManagerType::AI)
