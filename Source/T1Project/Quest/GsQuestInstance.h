// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/Public/TimerManager.h"

#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Quest/GsQuestData.h"


struct FGsSpawnParam;
struct FTimerHandle;
struct FGsSchemaQuest;
struct IGsMessageParam;
class FText;
class FGsQuestTableCache;
class FGsQuestObjectiveTableCache;
class FGsAIQuestHandler;
class UGsGameObjectBase;
class UGsGameObjectLocalPlayer;

/**
 *	진행 중인 퀘스트 인스턴스
 */
class T1PROJECT_API FGsQuestInstance
{
protected:
	UGsGameObjectLocalPlayer* _owner;
	StoryId						_storyId = 0;					// 스토리 아이디
	QuestType					_questType = QuestType::MAIN;	// 퀘스트 타입
	QuestId						_questId = 0;					// 퀘스트 아이디
	QuestKey					_questKey;						// 퀘스트 아이디
	QuestId						_nextQuestId = 0;				// 다음 퀘스트 아이디

	EGsQuestUIState				_questUIState;					// UI에서 사용 할 퀘스트 상태	
	float						_remainDistance;				// 자동진행 중 이동남은 거리
	QuestObjectiveId			_activeObjectiveId;				// 자동진행 중 오브젝트 아이디	
	const FGsSchemaQuest*		_schemaQuest;					// 퀘스트 스키마
	EGsQuestUIEffectType		_questUIEffectType;				// 퀘스트 허드UI 연출 타입

	TSharedPtr<FGsQuestData>		_questData = nullptr;
	TSharedPtr<FGsQuestTableCache>	_questTableCache = nullptr;		// 퀘스트매니저에서 QuestTableCache를 얻어온다.		
	TSharedPtr<FGsQuestTableCache>	_preQuestTableCache = nullptr;	// 퀘스트매니저에서 QuestTableCache를 얻어온다.	
	FTimerHandle					_goalRadiusHandle;				// 수동이동 체크 핸들러	
	TArray<FGsObjectiveData>		_objectiveDataList;				// 오브젝티브 데이터 리스트
	QuestState						_state = QuestState::NONE;		// 퀘스트 진행 상태	
	bool							_isShowHudUI = true;			// hudUI를 표시할건가?
	bool							_isTeleportAble = true;			// 기본 값은 true
	bool							_isAutoMoveAble = true;			// 기본 값은 true
	float							_drawQuestDistanceLimit = 0;	// 최소 거리안은 표식안함
	bool							_isTouchLock = false;			// 광클 방지용 퀘스트 롹

	// 오브젝티브 테이블 리스트
	TArray<TSharedPtr<FGsQuestObjectiveTableCache>> _objectiveTableCacheList;
	// 오브젝티브 타입데이터 셋팅을 위한 함수 Function
	TMap<QuestObjectiveType, TFunction<void(TSharedPtr<FGsQuestObjectiveTableCache>, QuestObjectiveType)>> _objectiveTypeFuncs;

public:
	FGsQuestInstance() = default;
	virtual ~FGsQuestInstance();

public:
	virtual void Initialize(UGsGameObjectLocalPlayer* inOwner, StoryId inStoryId, const QuestKey& inQuestKey);
	void Finalize();

public:
	// 퀘스트 정보 갱신
	void Accept();
	void Update(QuestState inState);
	virtual void UpdateUIState() {}

public:
	virtual void Clear();
	// 서버에서 받은 상태와 ui에서 표시 하는 상태 분류가 달라 따로 셋팅해준다.
	// 서버에서는 LOCKED 체크를 안하고 클라에서는 COMPLETED를 표시가 필요없다.
	virtual void SetQuestState(QuestState inState);
	// 퀘스트 습득레벨
	virtual const int32 GetRequireLevel() { return 0; }

	// 다음 스토리에 연퀘 시작
	virtual void AddNextStoryQuest();
	// 다음 연퀘 시작
	virtual void AddNextQuest();
	// 퀘스트 수락 연출
	virtual void PlayAcceptEffect() {};
	virtual void StartQuestEvent(QuestEventType inQuestEventType, int8 inindex = 0) {};

public:
	virtual void GetTitleText(OUT FText& outTitleText) {};			// 퀘스트 타이틀
	virtual void GetDescText(OUT FText& outDescText) {};			// 퀘스트 설명
	virtual const RewardId GetMainRewardId() const { return 0; }		// 메인 보상 아이디
	virtual const RewardId GetSubRewardId() const { return 0; }		// 서브 보상 아이디
	virtual bool IsShowHudUI() const;								// hudUI에 표시여부?
	virtual bool GetTeleportData(OUT int32& outTeleportGold, OUT QuestObjectiveId& outObjectiveId) const;	// 텔레포트 비용

	virtual void OnPostGotoField() {}

private:
	// Objective 셋팅
	void SetQuestObjective();
	// npc 인터렉션 타입 데이터 셋팅
	void SetObjectiveInteractNpcData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType);
	// 몬스터 사냥 타입 데이터 셋팅
	void SetObjectiveKillData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType);
	// 위치 이동 타입 데이터 셋팅
	void SetObjectiveGotoPosData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType);
	// 프랍 수집 타입 데이터 셋팅
	void SetObjectivePropData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType);
	// 인스턴스로 던전으로 이동 셋팅
	void SetObjectiveWarpToInstanceFieldData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType);
	// 인스턴스로 던전으로 이동 셋팅
	void SetObjectiveWarpToLastPublicFieldData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType);
	// 몬스터 타입별 데이터 셋팅
	void SetObjectiveKillCreatureTypeData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType);
	// 제작 데이터 셋팅
	void SetObjectiveCraftData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType);
	// 퀘스트 타입별 완료 체크 데이터 셋팅
	void SetObjectiveCompleteQuestTypeData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType);
	// 스토리 타입별 완료 체크 데이터 셋팅
	void SetObjectiveStoryQuestTypeData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType);
	// 시공의 틈새 몬스터 사냥타입 데이터 셋팅
	void SetObjectiveKillSpaceCrackData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType);
	// 같은 맵안에서 쓰일 텔레포트 셋팅
	void SetObjectiveTypeTeleportData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType);
	// 통합 Common 데이타
	void SetObjectiveCommonData(TSharedPtr<FGsQuestObjectiveTableCache> inObjectiveTableCache, QuestObjectiveType inObjectiveType);
	// Objective 상태 셋팅
	void SetQuestObjectiveState();
	// 타겟 업데이트
	void UpdateTarget(bool inIsAdd);

private:
	// 수동조작 일 경우 목표위치까지 도달했는지 체크
	void CheckGoalRadius();
	// 타이머를 종료한다.
	void StopTimer();
	// 수동으로 위치 도착했다.
	void ArrivedManually();
	// 0.5초 마다 목표위치에 도달했는지 타이머를 시작한다.
	void StartTimer();	

protected:
	virtual void PrAcceptBeginEvent(QuestEventType inQuestEventType);						// 재연결 시 끝내지 못한 연출 마저 연출
	virtual void StartQuestClearEvent(QuestEventType inQuestEventType);
	virtual void StartQuestBeginEvent(QuestEventType inQuestEventType);
	virtual void StartObjectiveClearEvent(QuestEventType inQuestEventType, int8 inindex);	// 오브젝티브 수행 완료 됐을 때 하는 연출

protected:
	// 	WARP_TO_INSTANCE_FIELD or WARP_TO_LAST_PUBLIC_FIELD 인가?
	bool IsGotoFieldType();
	// WARP_TO_INSTANCE_FIELD or WARP_TO_LAST_PUBLIC_FIELD 이동
	void OnGotoField();

public:
	// 오토진행 시작
	virtual bool StartAutoProgress(bool inIsAuto = false);
	// 오토진행 중지
	void StopAutoProgress();
	// 오토진행 도착
	void ArrivedAutoProgress(const IGsMessageParam* In_data);
	// 오토진행으로 다음 맵이동 시 도착위치 알림
	void NextMapAutoProgress(const IGsMessageParam* In_data);
	// 오브젝트가 목표치를 채웠는가? 오크 처치 10/10
	bool IsObjectiveCompleted(int32 inIndex) const;
	// 목표위치에 도착하거나, npc위치로 이동 했을 때
	void SendQuestUpdate(QuestObjectiveId inQuestObjectiveId, SpawnId inSpawnId, int64 inGameId, MapId inMapId = 0) const;
	void SendQuestUpdateIntractNpc(int64 inUniqueId);
	// Prop Interaction을 했다.
	bool IsInteractionPropContents(int32 inTableId, OUT QuestObjectiveId& outObjectiveId) const;
	// npc Interaction을 했다.
	bool OnInteractionContents(const IGsMessageParam* In_data);
	// npc Interaction 이후 연출(npc다이얼로그) 시작
	void TryInteract(const int64 inGameId);
	void SetShowHudUI(bool inIsShowHudUI) { _isShowHudUI = inIsShowHudUI; }
	// hud ui slot effectType
	void UpdateUIEffect(EGsQuestUIState inValue);

	// 자동진행 업데이트 (루팅이후 자동진행 재 요청을 위해 private -> public) 변경
	void UpdateAutoProgress();
	// 캠프랑 부딪혔을때
	void OnCampHit(const class AGsCampBase* In_camp);

public:
	bool RequestAutoMove(bool inIsAuto = true);

public:
	// Get
	const StoryId GetStoryId() const { return _storyId; }
	const QuestId GetQuestId() const { return _questId; }
	const QuestKey& GetQuestKey() const { return _questKey; }
	const QuestKey GetQuestNextKey() const { return QuestKey(_nextQuestId); }
	const QuestIndex GetQuestIndex() const { return _questKey._questIndex; }
	const QuestId GetNextQuestId() const { return _nextQuestId; }
	TSharedPtr<FGsQuestTableCache> GatQuestTableCache() const { return _questTableCache; }
	FGsAIQuestHandler* GetAIQuestHandler() const;
	UGsGameObjectBase* GetGameObject(int inTableId) const;
	int64 GetNpcUniqueId(int inTableId) const;
	const QuestState& GetQuestState() const { return _state; }							// 퀘스트 상태	

	void SetTouchLock(IN bool inValue) { _isTouchLock = inValue; }
	bool GetTouchLock() { return _isTouchLock; }

public:
	// UI에서 필요한 데이터
	const QuestType GetQuestType() const { return _questType; }							// 퀘스트 타입 (메인, 일일....)
	void GetQuestUIState(OUT EGsQuestUIState& outUiState);								// 퀘스트UI 상태	
	float GetRemainDistance() const { return _remainDistance; }							// 자동 이동 남은 거리	
	int32 GetObjectiveNum() const;														// 퀘스트에 부여된 오브젝티브 갯수
	void GetObjectiveContentsText(IN int inIndex, OUT FText& outObjectiveText) const;	// 오브젝티브 수행 목표 TEXT
	bool GetObjectiveCountText(IN int inIndex, OUT bool& outIsComplete,
		OUT bool& outIsReady, OUT int& outCurrValue, OUT int& outTableValue);			// 오브젝티브 수행 갯수 TEXT
	int32 GetObjectiveValue(int32 inIndex) const;										// 오브젝티브 수행 갯수	
	bool GetIsTeleportAble() const;														// 텔레포트가 가능한가?	
	void ClosePopupTeleport();
	bool IsObjectiveReady(int32 inIndex) const;

	void GetQuestUIEffectType(OUT EGsQuestUIEffectType& outUiEffectType);				// 퀘스트 허드UI 이펙트 타입	
	void SetQuestUIEffectType(IN EGsQuestUIEffectType inUiEffectType);

	bool GetActiveObjectiveType(OUT QuestObjectiveType& outObjectiveType);				// 현재 엑티브한 오브젝티브의 타입
	QuestContentsType GetQuestContentsType();											// 퀘스트 컨텐츠 타입

	bool IsInGoalArea(OUT bool& outIsInGalArea);
};