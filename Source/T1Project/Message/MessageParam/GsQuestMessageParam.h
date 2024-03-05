#pragma once

#include "CoreMinimal.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Quest/GsQuestData.h"

struct FGsSchemaSubDialogData;
struct FGsSchemaNpcDialogData;
struct FGsSchemaRewardData;
/*
	퀘스트 보상창/수락창 메세지 Param
*/


struct FGsQuestPopUpParamBase : public IGsMessageParam
{
public:
	StoryId _storyId;
	QuestId _questId;
	QuestIndex _questIndex;
	FText _titleText;
	FText _descriptionText;
	RewardId _mainRewardId;
	RewardId _subRewardId;

public:
	FGsQuestPopUpParamBase() = default;
	explicit FGsQuestPopUpParamBase(StoryId inStoryId, QuestId inQuestId, QuestIndex inQuestIndex, 
		FText inTitleText, FText inDescriptionText, RewardId inMainRewardId, RewardId inSubRewardId) :
		_storyId(inStoryId), _questId(inQuestId), _questIndex(inQuestIndex), 
		_titleText(inTitleText), _descriptionText(inDescriptionText),
		_mainRewardId(inMainRewardId), _subRewardId(inSubRewardId) {}
	virtual ~FGsQuestPopUpParamBase() = default;
};

struct FGsQuestPopUpParam final : public FGsQuestPopUpParamBase
{
public:
	SpawnId _npcId;
	QuestType _questType;
	QuestContentsType _questContentsType;

public:
	FGsQuestPopUpParam() = default;
	explicit FGsQuestPopUpParam(StoryId inStoryId, QuestId inQuestId, QuestIndex inQuestIndex, FText inTitleText, FText inDescriptionText,
		RewardId inMainRewardId, RewardId inSubRewardId, SpawnId inNpcId, QuestType inQuestType, 
		QuestContentsType inQuestContentsType = QuestContentsType::NORMAL) :
		FGsQuestPopUpParamBase(inStoryId, inQuestId, inQuestIndex, inTitleText, inDescriptionText, inMainRewardId, inSubRewardId),
		_npcId(inNpcId), _questType(inQuestType), _questContentsType(inQuestContentsType) {}
	virtual ~FGsQuestPopUpParam() = default;
};

struct FGsQuestRepeatPopUpParam final : public FGsQuestPopUpParamBase
{
public:
	FSoftObjectPath _iconPath;	
	bool _isRefreshReward;

public:
	FGsQuestRepeatPopUpParam() = default;
	explicit FGsQuestRepeatPopUpParam(StoryId inStoryId, QuestId inQuestId, QuestIndex inQuestIndex, FText inTitleText, FText inDescriptionText, 
		RewardId inMainRewardId, RewardId inSubRewardId, FSoftObjectPath inIconPath, bool inIsRefreshReward) :
		FGsQuestPopUpParamBase(inStoryId, inQuestId, inQuestIndex, inTitleText, inDescriptionText, inMainRewardId, inSubRewardId),
		_iconPath(inIconPath), _isRefreshReward(inIsRefreshReward) {}
	virtual ~FGsQuestRepeatPopUpParam() = default;
};

/*
	퀘스트 타겟 파람(몬스터)
*/

struct FGsQuestTargetMonsterParam final : public IGsMessageParam
{
public:
	// 퀘스트Id
	QuestId _questId = INVALID_QUEST_ID;
	// 추가인지, 삭제인지
	bool _isAdd = false;
	// 몬스터 테이블 id array
	TArray<int32> _tableIdList;

public:
	FGsQuestTargetMonsterParam() = default;
	explicit FGsQuestTargetMonsterParam(QuestId inQuestId, bool inIsAdd, TArray<int32> inTableIdList) :
		_questId(inQuestId), _isAdd(inIsAdd), _tableIdList(inTableIdList)
	{
	}
	virtual ~FGsQuestTargetMonsterParam() = default;
};

/*
	퀘스트 타겟 파람(NPC)
*/

struct FGsQuestTargetNpcParam final : public IGsMessageParam
{
public:
	// 퀘스트Id
	QuestId _questId = INVALID_QUEST_ID;
	// 추가인지, 삭제인지
	bool _isAdd = false;
	// 몬스터 테이블 id array
	TArray<int64> _uniqueIdList;

public:
	FGsQuestTargetNpcParam() = default;
	explicit FGsQuestTargetNpcParam(QuestId inQuestId, bool inIsAdd, TArray<int64> inUniqueIdList) :
		_questId(inQuestId), _isAdd(inIsAdd), _uniqueIdList(inUniqueIdList)
	{
	}
	virtual ~FGsQuestTargetNpcParam() = default;
};

struct FGsQuestTargetPosParam final : public IGsMessageParam
{
public:
	// 퀘스트Id
	QuestId _questId = INVALID_QUEST_ID;
	// 추가인지, 삭제인지
	bool _isAdd = false;
	// 몬스터 테이블 id array
	TArray<int64> _uniqueIdList;

public:
	FGsQuestTargetPosParam() = default;
	explicit FGsQuestTargetPosParam(QuestId inQuestId, bool inIsAdd, TArray<int64> inUniqueIdList) :
		_questId(inQuestId), _isAdd(inIsAdd), _uniqueIdList(inUniqueIdList)
	{
	}
	virtual ~FGsQuestTargetPosParam() = default;
};

struct FGsQuestSlotTouchParam final : public IGsMessageParam
{
public:
	// 퀘스트Key
	QuestKey _questKey = QuestKey::KeyNone();
	// 선택인지? 해지인지?
	bool _isSelected = false;
	// 스토리Id
	StoryId _storyId = INVALID_STORY_ID;
	// 자동인가? 유저선택인가?
	bool _isAuto = false;

public:
	FGsQuestSlotTouchParam() = default;
	explicit FGsQuestSlotTouchParam(QuestKey inQuestKey, bool inIsSelected, StoryId inStoryId, bool inIsAuto = false) :
		_questKey(inQuestKey), _isSelected(inIsSelected), _storyId(inStoryId), _isAuto(inIsAuto)
	{
	}
	virtual ~FGsQuestSlotTouchParam() = default;
};

struct FGsQuestBeginActionParam final : public IGsMessageParam
{
public:
	QuestEventType _questEventType = QuestEventType::EVENT_STROY_BEGIN;
	// 퀘스트Id
	QuestId _questId = INVALID_QUEST_ID;
	int32 _subDialogId = 0;

	EventActionGroupId _eventActionGroupId = INVALID_EVENT_ACTION_GROUP_ID;
	EventActionIndex _eventActionIndex = INVALID_EVENT_ACTION_INDEX;

	bool _isReStart = false;

public:
	FGsQuestBeginActionParam() = default;
	explicit FGsQuestBeginActionParam(
		QuestEventType inQuestEventType,
		QuestId inQuestId,	
		int32 inSubDialogId)
		:
		_questEventType(inQuestEventType),
		_questId(inQuestId),		
		_subDialogId(inSubDialogId)
	{
	}
	explicit FGsQuestBeginActionParam(
		QuestEventType inQuestEventType,
		QuestId inQuestId,	
		EventActionGroupId inEventActionGroupId,
		EventActionIndex inEventActionIndex,
		bool inReStart = false)
		:
		_questEventType(inQuestEventType),
		_questId(inQuestId), 				
		_eventActionGroupId(inEventActionGroupId),
		_eventActionIndex(inEventActionIndex),
		_isReStart(inReStart)
	{
	}
	virtual ~FGsQuestBeginActionParam() = default;
};

struct FGsQuestClearActionParam final : public IGsMessageParam
{
public:
	QuestEventType _questEventType = QuestEventType::EVENT_STROY_BEGIN;
	// 퀘스트Id
	QuestId _questId = INVALID_QUEST_ID;
	QuestId _nextQuestId = INVALID_QUEST_ID;
	int32 _subDialogId = 0;

	EventActionGroupId _preEventActionGroupId = INVALID_EVENT_ACTION_GROUP_ID;
	EventActionGroupId _postEventActionGroupId = INVALID_EVENT_ACTION_GROUP_ID;

	EventActionIndex _preEventActionIndex = INVALID_EVENT_ACTION_INDEX;
	EventActionIndex _postEventActionIndex = INVALID_EVENT_ACTION_INDEX;

public:
	FGsQuestClearActionParam() = default;
	explicit FGsQuestClearActionParam(
		QuestEventType inQuestEventType,
		QuestId inQuestId,
		int32 inSubDialogId)
		:
		_questEventType(inQuestEventType),
		_questId(inQuestId),
		_subDialogId(inSubDialogId)
	{
	}

	explicit FGsQuestClearActionParam(
		QuestEventType inQuestEventType,
		QuestId inQuestId,
		int32 inSubDialogId,
		int32 inEventActionGroupId,
		EventActionIndex inEventActionIndex)
		:
		_questEventType(inQuestEventType),
		_questId(inQuestId),
		_subDialogId(inSubDialogId),
		_postEventActionGroupId(inEventActionGroupId),
		_postEventActionIndex(inEventActionIndex)
	{
	}

	explicit FGsQuestClearActionParam(
		QuestEventType inQuestEventType,
		QuestId inQuestId,
		QuestId inNextQuestId,
		int32 inSubDialogId,
		int32 inPreEventActionGroupId,
		int32 inPostEventActionGroupId, 
		EventActionIndex inPreEventActionIndex,
		EventActionIndex inPostEventActionIndex)
		:
		_questEventType(inQuestEventType),
		_questId(inQuestId),
		_nextQuestId(inNextQuestId),
		_subDialogId(inSubDialogId),
		_preEventActionGroupId(inPreEventActionGroupId),
		_postEventActionGroupId(inPostEventActionGroupId),
		_preEventActionIndex(inPreEventActionIndex),
		_postEventActionIndex(inPostEventActionIndex)
	{
	}
	virtual ~FGsQuestClearActionParam() = default;
};

struct FGsFinishedQuestEventParam final : public IGsMessageParam
{
public:
	QuestEventType _questEventType = QuestEventType::EVENT_STROY_BEGIN;
	QuestId _questId = INVALID_QUEST_ID;

public:
	FGsFinishedQuestEventParam() = default;
	explicit FGsFinishedQuestEventParam(
		QuestEventType inQuestEventType,
		QuestId inQuestId) :
		_questEventType(inQuestEventType),
		_questId(inQuestId)
	{
	}
	virtual ~FGsFinishedQuestEventParam() = default;
};


struct FGsQuestTeleportParam final : public IGsMessageParam
{
public:
	// 퀘스트Id
	QuestId _questId = INVALID_QUEST_ID;
	// 스토리Id
	StoryId _storyId = INVALID_STORY_ID;
	// QuestIndex 
	QuestIndex _questIndex = INVALID_QUEST_INDEX;
	// npcId
	int32 _npcId = 0;	

public:
	FGsQuestTeleportParam() = default;
	explicit FGsQuestTeleportParam(
		QuestId inQuestId,
		StoryId inStoryId,
		QuestIndex inQuestIndex,
		int32 inNpcId = 0)
		:
		_questId(inQuestId),
		_storyId(inStoryId),
		_questIndex(inQuestIndex),
		_npcId(inNpcId)
	{
	}
	virtual ~FGsQuestTeleportParam() = default;
};

struct FGsQuestParam final : public IGsMessageParam
{
public:
	// 퀘스트Id
	QuestId _questId = INVALID_QUEST_ID;
	QuestIndex _questIndex = INVALID_QUEST_INDEX;

public:
	FGsQuestParam() = default;
	explicit FGsQuestParam(
		QuestId inQuestId, QuestIndex inQuestIndex = INVALID_QUEST_INDEX)
		:
		_questId(inQuestId),
		_questIndex(inQuestIndex)
	{
	}
	virtual ~FGsQuestParam() = default;
};

struct FGsStoryParam final : public IGsMessageParam
{
public:
	// 스토리Id
	StoryId _storyId = INVALID_STORY_ID;
	QuestKey _questKey = QuestKey::KeyNone();

public:
	FGsStoryParam() = default;
	explicit FGsStoryParam(
		StoryId inStoryId, QuestKey inQuestKey = QuestKey::KeyNone())
		:
		_storyId(inStoryId),
		_questKey(inQuestKey)
	{
	}
	virtual ~FGsStoryParam() = default;
};


struct FGsQuestRankParam final : public IGsMessageParam
{
public:
	// 퀘스트Id
	QuestId _questId = INVALID_QUEST_ID;
	// 스토리Id
	StoryId _storyId = INVALID_STORY_ID;

public:
	FGsQuestRankParam() = default;
	explicit FGsQuestRankParam(
		QuestId inQuestId,
		StoryId inStoryId)
		:
		_questId(inQuestId),
		_storyId(inStoryId)
	{
	}
	virtual ~FGsQuestRankParam() = default;
};

/*
	퀘스트 서브 인터랙션 npcMark 파람
*/
struct FGsQuesSubInterationNpcMarkParam final : public IGsMessageParam
{
public:
	TArray<FGsSubQuestNpcMarkData> _npcMarkDataList;

public:
	FGsQuesSubInterationNpcMarkParam() = default;
	FGsQuesSubInterationNpcMarkParam(TArray<FGsSubQuestNpcMarkData>& inNpcMarkDataList):
		_npcMarkDataList(inNpcMarkDataList)
	{
	}
	virtual ~FGsQuesSubInterationNpcMarkParam() = default;
};

