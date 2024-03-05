#pragma once

#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"

#include "T1Project.h"

#include "Quest/Table/GsQuestTableCache.h"
#include "Quest/Table/GsQuestObjectiveTableCache.h"
#include "Quest/GsSchemaQuestObjectiveAreaPos.h"

struct QuestKey
{
	QuestId	_questId = INVALID_QUEST_ID;			// 퀘스트ID
	QuestIndex _questIndex = INVALID_QUEST_INDEX;	// 퀘스트ID가 중복일수있는 경우가 생겨 index로 구분

public:
	static const QuestKey& KeyNone()
	{	
		static const QuestKey StaticEmptyKey = QuestKey(INVALID_QUEST_ID, INVALID_QUEST_INDEX);
		return StaticEmptyKey;
	}

	bool IsKeyNone() const
	{
		return (this->_questId == INVALID_QUEST_ID) && (this->_questIndex == INVALID_QUEST_INDEX);
	}

public:
	QuestKey() = default;
	explicit QuestKey(QuestId questId, QuestIndex questIndex = INVALID_QUEST_INDEX)
		:
		_questId(questId),
		_questIndex(questIndex)
	{
	}

	bool operator == (const QuestKey& rhs)const noexcept
	{
		return (this->_questId == rhs._questId) && (this->_questIndex == rhs._questIndex);
	}

	bool operator != (const QuestKey& rhs)const noexcept
	{
		return (this->_questId != rhs._questId) || (this->_questIndex != rhs._questIndex);
	}

	QuestId QuestID()
	{
		return _questId;
	}
};

/**
 *	동적 오브젝티브 데이터(서버 -> 클라)
 */
struct FGsQuestDynamicData
{
private:
	QuestState		_state = QuestState::NONE;	// 퀘스트 상태
	TArray<int32>	_objectiveValueList;		// 오브젝티브 수행 카운트(값)
	EventActionIndex _preEventActionIndex = INVALID_EVENT_ACTION_INDEX;
	EventActionIndex _postEventActionIndex = INVALID_EVENT_ACTION_INDEX;
	EventActionIndex _postStoryEventActionIndex = INVALID_EVENT_ACTION_INDEX;

	uint8 _refreshCount = 0;								// 새로고침 남은 횟수
	uint32 _repeatScrollId = 0;								// FGsSchemaQuestRepeatScroll의 Id(새로고침 횟수별 재화리스트 참고 위함). 없으면 0
	TArray<RewardBoxItemIdPair> _rewardBoxItemIdPairList;	// 퀘스트 랜덤보상 목록리스트

public:
	void SetQuestState(QuestState inQuestState) { _state = inQuestState; }
	QuestState GetQuestState() { return _state; }

	void SetQuestObjectiveValueList(const TArray<int32>& inObjectiveValueList) { _objectiveValueList = inObjectiveValueList; }
	TArray<int32>& GetObjectiveValueList() { return _objectiveValueList; }

	void SetPostEventActionIndex(EventActionIndex inPostEventActionIndex) { _postEventActionIndex = inPostEventActionIndex; }
	void SetPreEventActionIndex(EventActionIndex inPreEventActionIndex) { _preEventActionIndex = inPreEventActionIndex; }
	void SetPostStoryEventActionIndex(EventActionIndex inPostStoryEventActionIndex) { _postStoryEventActionIndex = inPostStoryEventActionIndex; }
	
	EventActionIndex GetPostEventActionIndex() { return _postEventActionIndex; }
	EventActionIndex GetPreEventActionIndex() { return _preEventActionIndex; }
	EventActionIndex GetPostStoryEventActionIndex() { return _postStoryEventActionIndex; }

	void SetRefreshCount(uint8 inCount){ _refreshCount = inCount; }
	uint8 GetRefreshCount() const { return _refreshCount;	}
	void SetRepeatScrollId(uint32 inQuestRepeatScrollId) { _repeatScrollId = inQuestRepeatScrollId; }
	uint32 GetRepeatScrollId() const { return _repeatScrollId; }

	void SetRewardBoxItemIdPairList(IN const TArray<RewardBoxItemIdPair>& inRewardBoxItemIdPairList)
	{ 
		_rewardBoxItemIdPairList.Empty();
		_rewardBoxItemIdPairList.Append(inRewardBoxItemIdPairList); 
	}
	bool GetRewardBoxItemIdPairList(OUT TArray<RewardBoxItemIdPair>& outRewardBoxItemIdPairList)
	{
		if (0 >= _rewardBoxItemIdPairList.Num())
			return false;

		outRewardBoxItemIdPairList.Append(_rewardBoxItemIdPairList);
		return true;
	}

	void ClearData() {
		_state = QuestState::NONE;
		_objectiveValueList.Reset(); 
		_preEventActionIndex = INVALID_EVENT_ACTION_INDEX;
		_postEventActionIndex = INVALID_EVENT_ACTION_INDEX;
		_postStoryEventActionIndex = INVALID_EVENT_ACTION_INDEX;
		_rewardBoxItemIdPairList.Empty();
	}
};

struct FGsQuestData
{
protected:
	StoryId _storyId = INVALID_STORY_ID;
	QuestId	_questId = INVALID_QUEST_ID;
	QuestId	_nextQuestId = INVALID_QUEST_ID;
	QuestId	_nextStoryQuestId = INVALID_QUEST_ID;
	QuestType _questType = QuestType::MAIN;
	TSharedPtr<FGsQuestTableCache> _questTableCache;
	TMap<QuestIndex, TSharedPtr<FGsQuestDynamicData>> _questDynamicDatas;

public:
	FGsQuestData() = default;

public:
	void SetQuestId(QuestId inQuestId) { _questId = inQuestId; }
	QuestId GetQuestId() { return _questId; }

	void SetStoryId(StoryId inStoryId) { _storyId = inStoryId; }
	StoryId GetStoryId() { return _storyId; }

	void SetNextQuestId(QuestId inQuestId) { _nextQuestId = inQuestId; }
	QuestId GetNextQuestId() { return _nextQuestId; }

	void SetQuestType(QuestType inQuestType) { _questType = inQuestType; }
	QuestType GetQuestType() { return _questType; }

	void SetQuestTableCache(TSharedPtr<FGsQuestTableCache> inQuestTableCache) { _questTableCache = inQuestTableCache; }
	TSharedPtr<FGsQuestTableCache> GetQuestTableCache() const { return _questTableCache; }

	TMap<QuestIndex, TSharedPtr<FGsQuestDynamicData>>& GetQuestDynamicDataList() { return _questDynamicDatas; }
	FGsQuestDynamicData* GetQuestDynamicData(QuestIndex inQuestIndex = INVALID_QUEST_INDEX)
	{
		if (false == _questDynamicDatas.Contains(inQuestIndex))
			return nullptr;

		return _questDynamicDatas.FindRef(inQuestIndex).Get();
	}

	FGsQuestDynamicData* ClaimQuestDynamicData(QuestIndex inQuestIndex = INVALID_QUEST_INDEX)
	{
		if (false == _questDynamicDatas.Contains(inQuestIndex))
		{
			TSharedPtr<FGsQuestDynamicData> dynamicData = MakeShareable(new FGsQuestDynamicData());
			_questDynamicDatas.Emplace(inQuestIndex, dynamicData);
		}

		return _questDynamicDatas.FindRef(inQuestIndex).Get();
	}

	bool IsQuestDynamicData(QuestIndex inQuestIndex = INVALID_QUEST_INDEX)
	{ 
		if (false == _questDynamicDatas.Contains(inQuestIndex))
			return false;

		TSharedPtr<FGsQuestDynamicData> DynamicDataPtr = _questDynamicDatas.FindRef(inQuestIndex);
		if (!DynamicDataPtr.IsValid())
			return false;

		return (nullptr == DynamicDataPtr.Get()) ? false : true;
	}

	void ResetDynamicData(QuestIndex inQuestIndex = INVALID_QUEST_INDEX)
	{
		if (false == _questDynamicDatas.Contains(inQuestIndex))
			return;

		TSharedPtr<FGsQuestDynamicData> dynamicDataPtr = _questDynamicDatas.FindRef(inQuestIndex);
		if (!dynamicDataPtr.IsValid())
			return;

		FGsQuestDynamicData* dynamicData = dynamicDataPtr.Get();
		if (nullptr == dynamicData)
			return;

		dynamicDataPtr->ClearData();
		_questDynamicDatas.Remove(inQuestIndex);
	}

	void AllResetDynamicData()
	{
		for (auto& iter : _questDynamicDatas)
		{
			TSharedPtr<FGsQuestDynamicData> dynamicDataPtr = iter.Value;
			if (!dynamicDataPtr.IsValid())
				continue;

			FGsQuestDynamicData* dynamicData = dynamicDataPtr.Get();
			if (nullptr == dynamicData)
				continue;

			dynamicData->ClearData();
		}

		_questDynamicDatas.Empty();
	}

	void ClearDynamicData(IN QuestIndex inQuestIndex = INVALID_QUEST_INDEX)
	{
		if (false == _questDynamicDatas.Contains(inQuestIndex))
			return;

		TSharedPtr<FGsQuestDynamicData> dynamicDataPtr = _questDynamicDatas.FindRef(inQuestIndex);
		if (!dynamicDataPtr.IsValid())
			return;

		FGsQuestDynamicData* dynamicData = dynamicDataPtr.Get();
		if (nullptr == dynamicData)
			return;

		dynamicDataPtr->ClearData();
	}

	void AllClearDynamicData()
	{
		AllResetDynamicData();
	}

	bool IsShowUIObjectiveCount(IN int inIndex)
	{
		if (0 > inIndex)
			return false;

		TArray<TSharedPtr<FGsQuestObjectiveTableCache>> objectiveTableCacheList = _questTableCache->GetObjectiveTableCache();
		if (inIndex >= objectiveTableCacheList.Num())
			return false;

		TSharedPtr<FGsQuestObjectiveTableCache> objectiveTableCache = objectiveTableCacheList[inIndex];
		if (objectiveTableCache.IsValid())
		{
			return objectiveTableCache->GetisCountHide()? false : true;
		}

		return false;
	}

public:
	int GetObjectiveNum() const
	{
		return _questTableCache->GetObjectiveTableCache().Num();
	}

	bool GetObjectiveDestText(IN int inIndex, OUT FText& outObjectiveText) const
	{
		if (0 > inIndex)
			return false;

		TArray<TSharedPtr<FGsQuestObjectiveTableCache>> objectiveTableCacheList = _questTableCache->GetObjectiveTableCache();
		if (inIndex >= objectiveTableCacheList.Num())
			return false;

		TSharedPtr<FGsQuestObjectiveTableCache> objectiveTableCache = objectiveTableCacheList[inIndex];
		if (objectiveTableCache.IsValid())
		{
			outObjectiveText = objectiveTableCache->GetDescText();
			return true;
		}

		return false;
	}

	bool GetObjectiveValue(IN int inIndex, OUT int& outValue) const
	{
		if (0 > inIndex)
			return false;

		TArray<TSharedPtr<FGsQuestObjectiveTableCache>> objectiveTableCacheList = _questTableCache->GetObjectiveTableCache();
		if (inIndex >= objectiveTableCacheList.Num())
			return false;

		TSharedPtr<FGsQuestObjectiveTableCache> objectiveTableCache = objectiveTableCacheList[inIndex];
		if (objectiveTableCache.IsValid())
		{
			outValue = objectiveTableCache->GetObjectiveValue();
			return true;
		}

		return false;
	}

	bool GetObjectiveDynamicValue(IN int inIndex, OUT int& outValue, IN QuestIndex inQuestIndex = INVALID_QUEST_INDEX) const
	{
		outValue = 0;

		if (0 > inIndex)
			return false;

		if (false == _questDynamicDatas.Contains(inQuestIndex))
			return false;

		TSharedPtr<FGsQuestDynamicData> dynamicDataPtr = _questDynamicDatas.FindRef(inQuestIndex);
		if (!dynamicDataPtr.IsValid())
			return false;

		FGsQuestDynamicData* dynamicData = dynamicDataPtr.Get();
		if (nullptr == dynamicData)
			return false;
				
		TArray<int32>& objectiveValueList = dynamicData->GetObjectiveValueList();
		if (inIndex >= objectiveValueList.Num())
			return false;

		outValue = objectiveValueList[inIndex];

		return true;
	}

	bool GetIsTickerHide(IN int inIndex) const
	{
		if (0 > inIndex)
			return false;

		TArray<TSharedPtr<FGsQuestObjectiveTableCache>> objectiveTableCacheList = _questTableCache->GetObjectiveTableCache();
		if (inIndex >= objectiveTableCacheList.Num())
			return false;

		TSharedPtr<FGsQuestObjectiveTableCache> objectiveTableCache = objectiveTableCacheList[inIndex];
		if (objectiveTableCache.IsValid())
		{
			return objectiveTableCache->GetisTickerHide();
		}

		return false;
	}
};

struct FGsQuestMainData : public FGsQuestData
{
private:
	EventActionGroupId _initEventActionGroupId = 0;
public:
	void SetInitEventActionGroupId(EventActionGroupId inEventActionGroupId) { _initEventActionGroupId = inEventActionGroupId; }
	EventActionGroupId GetInitEventActionGroupId() const { return _initEventActionGroupId; }
};

struct FGsQuestSubStoryTableCache;
struct FGsQuestSubData : public FGsQuestData
{
private:
	TSharedPtr<FGsQuestSubStoryTableCache> _questStoryTableCache;
	SpawnId _npcId;

public:
	void SetQuestSubStoryTableCache(TSharedPtr<FGsQuestSubStoryTableCache> inSubStoryTableCache) 
													{ _questStoryTableCache = inSubStoryTableCache; }
	TSharedPtr<FGsQuestSubStoryTableCache> GetQuestSubStoryTableCache() const { return _questStoryTableCache; }
	void SetNpcID(SpawnId inNpcId) { _npcId = inNpcId; }
	SpawnId GetNpcID() { return _npcId; }
};

struct FGsQuestRepeatData : public FGsQuestData
{
public:
	void SetRefreshCount(IN QuestIndex inIndex, uint8 inCount) 
	{ 
		FGsQuestDynamicData* dynamicData = GetQuestDynamicData(inIndex);
		if (nullptr == dynamicData)
			return;
		
		dynamicData->SetRefreshCount(inCount);
	}

	uint8 GetRefreshCount(IN QuestIndex inIndex)
	{ 
		FGsQuestDynamicData* dynamicData = GetQuestDynamicData(inIndex);
		if (nullptr == dynamicData)
			return 0;

		return dynamicData->GetRefreshCount();
	}

	void SetRepeatScrollId(QuestIndex inIndex, uint32 inRepeatScrollId)
	{
		if (FGsQuestDynamicData* dynamicData = GetQuestDynamicData(inIndex))
		{
			dynamicData->SetRepeatScrollId(inRepeatScrollId);
		}
	}

	uint32 GetRepeatScrollId(QuestIndex inIndex)
	{
		if (FGsQuestDynamicData* dynamicData = GetQuestDynamicData(inIndex))
		{
			return dynamicData->GetRepeatScrollId();
		}

		return 0;
	}

	// 서버에서 내려준 랜덤 보상 목록 리스트
	bool GetDynamicRewardBoxItemIdPairList(IN QuestIndex inIndex, OUT TArray<RewardBoxItemIdPair>& outRewardBoxItemIdPairList)
	{
		FGsQuestDynamicData* dynamicData = GetQuestDynamicData(inIndex);
		if (nullptr == dynamicData)
			return false;

		return dynamicData->GetRewardBoxItemIdPairList(outRewardBoxItemIdPairList);
	}

	void SetDynamicRewardBoxItemIdPairList(IN QuestIndex inIndex, IN const TArray<RewardBoxItemIdPair>& inRewardBoxItemIdPairList)
	{
		FGsQuestDynamicData* dynamicData = GetQuestDynamicData(inIndex);
		if (nullptr == dynamicData)
			return;

		dynamicData->SetRewardBoxItemIdPairList(inRewardBoxItemIdPairList);
	}
};

/**
 *	오브젝티브 goal Data
 */
struct T1PROJECT_API FGsObjectiveGoalData
{	
public:
	FGsObjectiveGoalData(int32 inGoalMapId, int32 inSpotId, FVector	inGoalPos, FVector inFrontPos, float inGoalRadius) :
		_goalMapId(inGoalMapId), _spotId(inSpotId), _goalPos(inGoalPos), _frontPos(inFrontPos), _goalRadius(inGoalRadius)
	{}
	FGsObjectiveGoalData(int32 inGoalMapId) :
		_goalMapId(inGoalMapId)
	{}

	int32	_goalMapId = 0;						// 도착지점 맵아이디		
	int32	_spotId = 0;						// 최종지점 스팟아이디
	FVector	_goalPos = FVector::ZeroVector;		// 최종지점 포지션
	FVector	_frontPos = FVector::ZeroVector;	// 도착지점 포지션	
	float	_goalRadius = 0.f;					// 도착지점 위치에서의 반지름	
	FVector _warpPos = FVector::ZeroVector;		// 맵이동 시 워프(거점)
	bool	_isDrawPosition = true;				// 도착 또는 최종지점 가까이 오면 남은거리 표시 안하기 위함

	void Clear()
	{
		_goalMapId = 0;
		_spotId = 0;
		_goalPos = FVector::ZeroVector;
		_frontPos = FVector::ZeroVector;
		_warpPos = FVector::ZeroVector;
		_warpPos = FVector::ZeroVector;
		_goalRadius = 0.f;
		_isDrawPosition = true;
	}
};


/**
 *	오브젝티브 Data
 */
struct FGsObjectiveData
{
public:
	QuestObjectiveId				_objectiveId = 0;
	QuestObjectiveType				_type;						// 오브젝티브 타입	
	bool							_isComplete = false;		// 오브젝티브의 목표가 수행되었는가?
	bool							_isReady = false;			// 오브젝티브가 순차진행을 위해 대기 중인가?
	TArray<int32>					_tableIdList;				// 오브젝티브 수행 목표(테이블id_몬스터아이디, NPC아이디...)
	int64							_uniqueId = 0;				// 오브젝티브 수행 목표의 실제 게임오브젝트 uniqueId
	
	// goal 정보
	TArray<FGsObjectiveGoalData>	_goalDataList;				// 목표지점 위치 데이터
	int8							_currGoalIndex = 0;			// 목표지점 현재 인덱스
	bool							_isCheckGoal = false;		// 도착했는가?	

	void Clear()
	{
		for (FGsObjectiveGoalData data : _goalDataList)
		{
			data.Clear();
		}
		_tableIdList.Empty();

		// goal 정보 초기화
		_goalDataList.Empty();		
		_currGoalIndex = 0;
		_isCheckGoal = false;		
		_isReady = false;
	}

	bool IsRandomSpot() 
	{ 
		return (
			_type == QuestObjectiveType::KILL || 
			_type == QuestObjectiveType::KILL_CREATURE_TYPE) ? true : false; 
	}

	int GetGoalIndex()
	{
		int listNum = _goalDataList.Num();
		if (listNum <= 1)
			return 0;

		if (true == _isCheckGoal)
		{
			int8 goalindex = 0;
			if (IsRandomSpot())
			{
				while (true)
				{
					goalindex = FMath::RandRange(0, listNum - 1);
					if(goalindex != _currGoalIndex)
						break;
				}

				_currGoalIndex = goalindex;
			}
			else
			{
				goalindex = _currGoalIndex + 1;
				_currGoalIndex = (listNum <= goalindex) ? 0 : goalindex;
			}
		}
		
		return _currGoalIndex;
	}
};

struct FGsSubQuestNpcMoveData
{
private:
	EQuestNpcState _npcState;
	StoryId _storyId;
	QuestId _questId;
	QuestObjectiveType _objectType = QuestObjectiveType::INTERACT_NPC;
	int32 _mapId;
	int32 _npcInteractionRange;
	int32 _npcId;
	FGsSchemaQuestObjectiveAreaPos _npcPos;

public:
	explicit FGsSubQuestNpcMoveData(EQuestNpcState inNpcState, StoryId inStoryId, int32 inNpcId)
		:_npcState(inNpcState), _storyId(inStoryId), _npcId(inNpcId)
	{
	}

	explicit FGsSubQuestNpcMoveData(EQuestNpcState inNpcState, StoryId inStoryId, QuestId inQuestId,
		QuestObjectiveType inObjectiveType, int32 inMapId, int32 inNpcInteractionRange, int32 inNpcId,
		FGsSchemaQuestObjectiveAreaPos inNpcPos)
		:_npcState(inNpcState),
		_storyId(inStoryId),
		_questId(inQuestId),
		_objectType(inObjectiveType),
		_mapId(inMapId),
		_npcInteractionRange(inNpcInteractionRange),
		_npcId(inNpcId),
		_npcPos(inNpcPos)
	{
	}

public:
	// getter
	const EQuestNpcState GetQuestNpcState() const { return _npcState; }
	const StoryId GetStoryId() const { return _storyId; }
	const QuestId GetQuestId() const { return _questId; }
	const QuestObjectiveType GetQuestObjectiveType() const { return _objectType; }
	const int32 GetLevel() const { return _mapId; }
	const int32 GetInNpcInteractionRange() const { return _npcInteractionRange; }
	const int32 GetNpcId() const { return _npcId; }
	const FGsSchemaQuestObjectiveAreaPos& GetNpcPosInfo() const { return _npcPos; }
};

struct FGsSubQuestNpcMarkData
{
private:
	int8 _npcStateIndex;
	bool _isActive;
	bool _isMiniMapOn;
	int32 _npcId;

public:
	explicit FGsSubQuestNpcMarkData(int8 inNpcStateIndex, bool inIsActive, bool inIsMiniMapOn, int32 inNpcId)
		:_npcStateIndex(inNpcStateIndex), _isActive(inIsActive), _isMiniMapOn(inIsMiniMapOn), _npcId(inNpcId)
	{
	}

	const int8 GetQuestNpcStateIndex() const { return _npcStateIndex; }
	const bool GetIsActive() const { return _isActive; }
	const bool GetIsMiniMapOn() const { return _isMiniMapOn; }
	const int32 GetNpcId() const { return _npcId; }
};
