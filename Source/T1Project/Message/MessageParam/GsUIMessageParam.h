// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Message/MessageParam/GsMessageParam.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Skill/GsSkillUIHandler.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Skill/GsSkill.h"
#include "UI/UIContent/Tray/GsUITrayTickerSection.h"

#include "Map/GsMapIconType.h"
#include "Map/WorldMap/GsWorldMapDefine.h"

#include "UI/UIContent/Window/Boss/GsUIWindowBossInfo.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateQuestBoard.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapSpoilsMonsterLevel.h"

#include "UserContext/GsUserContextData.h"
#include "../Dungeon/GsDungeonCommonData.h"

///**
// *
// */
//struct FGsUIMsgParamBase : public IGsMessageParam
//{
//	FGsUIMsgParamBase() = default;
//	virtual ~FGsUIMsgParamBase() = default;
//};

/**
 * int32 값 전달 시 사용
 */
struct FGsUIMsgParamInt : public IGsMessageParam
{
	int32 _data;

	FGsUIMsgParamInt() = default;
	virtual ~FGsUIMsgParamInt() = default;
	explicit FGsUIMsgParamInt(int32 InData)
		: _data(InData)
	{
	}
};

/**
 * uint32 값 전달 시 사용
 */
struct FGsUIMsgParamUInt : public IGsMessageParam
{
	uint32 _data;

	FGsUIMsgParamUInt() = default;
	virtual ~FGsUIMsgParamUInt() = default;
	explicit FGsUIMsgParamUInt(uint32 InData)
		: _data(InData)
	{
	}
};

/**
 * bool 값 전달 시 사용
 */
struct FGsUIMsgParamBool : public IGsMessageParam
{
	bool _data;

	FGsUIMsgParamBool() = default;
	virtual ~FGsUIMsgParamBool() = default;
	explicit FGsUIMsgParamBool(bool InData)
		: _data(InData)
	{
	}
};

/**
 * float 값 전달 시 사용
 */
struct FGsUIMsgParamFloat : public IGsMessageParam
{
	float _data;

	FGsUIMsgParamFloat() = default;
	virtual ~FGsUIMsgParamFloat() = default;
	explicit FGsUIMsgParamFloat(float InData)
		: _data(InData)
	{
	}
};

/**
 * Text 값 전달 시 사용
 */
struct FGsUIMsgParamText : public IGsMessageParam
{
	FText _data;

	FGsUIMsgParamText() = default;
	virtual ~FGsUIMsgParamText() = default;
	explicit FGsUIMsgParamText(FText InData)
		: _data(InData)
	{
	}
};

/**
 * Text 값 전달 시 사용
 */
struct FGsUIMsgParamIntArray : public IGsMessageParam
{
	TArray<int32> _data;

	FGsUIMsgParamIntArray() = default;
	virtual ~FGsUIMsgParamIntArray() = default;
	explicit FGsUIMsgParamIntArray(const TArray<int32> InData)
		: _data(InData)
	{
	}
};

/**
 * 스킬 버튼 하나 갱신
 */
struct FGsUIMsgParamUpdateSkillSlot final : public IGsMessageParam
{
	const FGsSkill* _data;

	FGsUIMsgParamUpdateSkillSlot() = default;
	virtual ~FGsUIMsgParamUpdateSkillSlot() = default;
	explicit FGsUIMsgParamUpdateSkillSlot(const FGsSkill* InData)
		: _data(InData)
	{
	}
};

/**
 * 스킬 자동사용 요청
 */
struct FGsUIMsgParamRequestSkillAuto final : public IGsMessageParam
{
	SkillId _skillId;
	bool _bUseAuto;

	FGsUIMsgParamRequestSkillAuto() = default;
	virtual ~FGsUIMsgParamRequestSkillAuto() = default;
	explicit FGsUIMsgParamRequestSkillAuto(SkillId InSkillId, bool bInUseAuto)
		: _skillId(InSkillId), _bUseAuto(bInUseAuto)
	{
	}
};

/**
 * 랜덤보상으로 받은 BagIndex 전달
 */
struct FGsUIMsgParamRewardRandomItemBag final : public IGsMessageParam
{
	QuestId _questId;
	ItemBagIndex _mainBagIndex; // 0~5 사이의 값. INVALID는 -1
	ItemBagIndex _subBagIndex; // 0~5 사이의 값. INVALID는 -1

	FGsUIMsgParamRewardRandomItemBag() = default;
	virtual ~FGsUIMsgParamRewardRandomItemBag() = default;
	explicit FGsUIMsgParamRewardRandomItemBag(QuestId InQuestId, ItemBagIndex InMainBagIndex, ItemBagIndex InSubBagIndex)
		: _questId(InQuestId), _mainBagIndex(InMainBagIndex), _subBagIndex(InSubBagIndex)
	{
	}
};

/**
 * 친구 Param
 */
struct FGsUIMsgParamBuddy final : public IGsMessageParam
{
	uint64 _userDBId;
	FString _userName;

	FGsUIMsgParamBuddy() = default;
	virtual ~FGsUIMsgParamBuddy() = default;
	explicit FGsUIMsgParamBuddy(uint64 InUserDBId, FString InUserName)
		: _userDBId(InUserDBId), _userName(InUserName)
	{
	}
}; 

struct FGsUIMsgParamBuddyError final : public IGsMessageParam
{
	PD::Result _result;

	FGsUIMsgParamBuddyError() = default;
	virtual ~FGsUIMsgParamBuddyError() = default;
	explicit FGsUIMsgParamBuddyError(PD::Result InUserDBId)
		: _result(InUserDBId)
	{
	}
};


struct FGsUIMsgParamInvalidPenalty final : public IGsMessageParam
{
	FGsUIMsgParamInvalidPenalty() = default;
	virtual ~FGsUIMsgParamInvalidPenalty() = default;
};

/**
 * Mail Param
 */
struct FGsUIMsgParamMail final : public IGsMessageParam
{
	uint64 _mailDBId;

	FGsUIMsgParamMail() = default;
	virtual ~FGsUIMsgParamMail() = default;
	explicit FGsUIMsgParamMail(uint64 InMailDBId)
		: _mailDBId(InMailDBId)
	{
	}
};

/**
 * Chat Param
 */
struct FGsUIMsgParamChat : public IGsMessageParam
{
	uint16 _chatType;
	bool _isVisibleFullChat{ true };
	int  _addCount{ 0 };

	FGsUIMsgParamChat() = default;
	virtual ~FGsUIMsgParamChat() = default;
	explicit FGsUIMsgParamChat(uint16 chatType, bool fullVisible = true, int addCount = 0)
		: _chatType(chatType), _isVisibleFullChat(fullVisible), _addCount(addCount)
	{
	}
};

struct FGsUIMsgParamChatUser : public IGsMessageParam
{
	WorldId _worldId;
	bool _enableChat{ true };

	FGsUIMsgParamChatUser() = default;
	virtual ~FGsUIMsgParamChatUser() = default;
	explicit FGsUIMsgParamChatUser(WorldId worldId, bool enableChat = true)
		: _worldId(worldId), _enableChat(enableChat)
	{
	}
};

struct FGsUIMsgParamInvasionChat : public IGsMessageParam
{
	uint16 _currentCount;
	uint16 _maxCount;

	FGsUIMsgParamInvasionChat() = default;
	virtual ~FGsUIMsgParamInvasionChat() = default;
	explicit FGsUIMsgParamInvasionChat(uint16 currentCount, uint16 maxCount)
		: _currentCount(currentCount), _maxCount(maxCount)
	{
	}
};

struct FGsUIMsgParamChatGroup : public IGsMessageParam
{
	GuildGroupChatRoomId _roomId;
	GuildDBId _guildDBId;

	FGsUIMsgParamChatGroup() = default;
	virtual ~FGsUIMsgParamChatGroup() = default;
	explicit FGsUIMsgParamChatGroup(GuildGroupChatRoomId roomId, GuildDBId guildDBId = INVALID_GUILD_DB_ID)
		: _roomId(roomId), _guildDBId(guildDBId)
	{
	}
};

/*
System Message Param
*/
struct FGsUIMsgParamSystemMessage : public IGsMessageParam
{
	EGsNotiSection _sectionType;
	const FText _systemMessage;

	FGsUIMsgParamSystemMessage() = default;
	virtual ~FGsUIMsgParamSystemMessage() = default;
	explicit FGsUIMsgParamSystemMessage(EGsNotiSection sectionType, const FText& systemMessage)
		: _sectionType(sectionType), _systemMessage(systemMessage)
	{
	}
};

struct FGsChatItem;
struct FGsUIMsgParamChatItem final : public FGsUIMsgParamChat
{
	TWeakPtr<FGsChatItem> _item;
	bool _isVisibleFullChat{ true };
	uint64 _roomNumber{ 0 };

	FGsUIMsgParamChatItem() = default;
	virtual ~FGsUIMsgParamChatItem() = default;
	explicit FGsUIMsgParamChatItem(uint16 chatType, TWeakPtr<FGsChatItem> item, bool fullVisible = true, uint64 roomNumber = 0)
		: FGsUIMsgParamChat(chatType), _item(item), _isVisibleFullChat(fullVisible), _roomNumber(roomNumber)
	{
	}
};

struct FGsUIMsgParamChatBlock final : public IGsMessageParam
{
	uint64 _userDBId;

	FGsUIMsgParamChatBlock() = default;
	virtual ~FGsUIMsgParamChatBlock() = default;
	explicit FGsUIMsgParamChatBlock(uint64 userDBId)
		: _userDBId(userDBId)
	{
	}
};

struct FGsUIMsgParamChatUserInfo final : public IGsMessageParam
{
	GuildDBId _guildDBId;
	GuildEmblemId _guildEmblemId;
	FString _guildName;

	FGsUIMsgParamChatUserInfo() = default;
	virtual ~FGsUIMsgParamChatUserInfo() = default;
	explicit FGsUIMsgParamChatUserInfo(GuildDBId guildDBId, GuildEmblemId guildEmblemId, FString guildName)
		: _guildDBId(guildDBId), _guildEmblemId(guildEmblemId), _guildName(guildName)
	{
	}
};

/**
 * Dungeon Param
 */
struct FGsUIMsgParamDungeonTime final : public IGsMessageParam
{
	FDateTime _time;
	bool _isTimeLimit = true;
	//초까지 보여주는 변수 
	bool _isShowSencond = false;

	bool _isPause = false;		// 2023/8/3 PKT - 시간은 보여주된 흐르지 말아야 할때 사용( ex : 시련의 전당의 하늘 선착장)

	FGsUIMsgParamDungeonTime() = default;
	virtual ~FGsUIMsgParamDungeonTime() = default;
	explicit FGsUIMsgParamDungeonTime(FDateTime inTime, bool isTimeLimit = true, bool inIsShowSecond = false, bool inPause = false)
		: _time(inTime), _isTimeLimit(isTimeLimit), _isShowSencond(inIsShowSecond), _isPause(inPause)
	{
	}
};
// 2023/3/3 PKT - 파티 던전
struct FGsUIMsgParamDungeonTimeEX final : public IGsMessageParam
{
	int64 _begineTime;
	int64 _endTime;

	FGsUIMsgParamDungeonTimeEX() = default;
	virtual ~FGsUIMsgParamDungeonTimeEX() = default;
	explicit FGsUIMsgParamDungeonTimeEX(int64 InBeginTime, int64 InEndTime) : _begineTime(InBeginTime), _endTime(InEndTime)
	{
	}
};


/**
 * 2022/05/27 PKT - Dungeon Group Refresh
 */
struct FGsUIMsgParamDungeonGroup final : public IGsMessageParam
{
	DungeonGroupId _groupId = INVALID_DUNGEON_GROUP_ID;

	FGsUIMsgParamDungeonGroup() = default;
	virtual ~FGsUIMsgParamDungeonGroup() = default;
	explicit FGsUIMsgParamDungeonGroup(DungeonGroupId InGroupId) : _groupId(InGroupId)
	{
	}
};


/**
 * 2022/05/30 PKT - CoopParty Dungeon MemberInfo
 */
struct FGsUIMsgParamPartyDungeonMemberList final : public IGsMessageParam
{
	const TArray<FGsPartyDungeonPartyStatusList>*_dataList = nullptr;

	FGsUIMsgParamPartyDungeonMemberList() = default;
	virtual ~FGsUIMsgParamPartyDungeonMemberList() = default;
	explicit FGsUIMsgParamPartyDungeonMemberList(const TArray<FGsPartyDungeonPartyStatusList>*InDataList) : _dataList(InDataList)
	{

	}
};

/**
 * 2022/06/18 PKT - Shotcut
 */
struct FGsUIMsgParamPartyDungeonShotcut final : public IGsMessageParam
{
	DungeonGroupId _dungeonGroupId;
	DungeonDifficulty _dungeonDifficulty;

	FGsUIMsgParamPartyDungeonShotcut() = default;
	virtual ~FGsUIMsgParamPartyDungeonShotcut() = default;
	explicit FGsUIMsgParamPartyDungeonShotcut(DungeonGroupId InDungeonGroupId, DungeonDifficulty InDifficulty) 
		: _dungeonGroupId(InDungeonGroupId), _dungeonDifficulty(InDifficulty)
	{

	}
};

struct FGsUIMsgParamPartyDungeonResult : public IGsMessageParam
{
	bool _result;
	FDateTime _playTime;

	TArray<ItemIdAmountPair> _basicRewardList;
	TArray<ItemIdAmountPair> _mainRewardList;
	CurrencyDeltaPair _basicCurrencyPair;
	CurrencyDeltaPair _personCurrencyPair;

	FGsUIMsgParamPartyDungeonResult() = default;
	explicit FGsUIMsgParamPartyDungeonResult(bool InResult, FDateTime & InClearTime
		, TArray<ItemIdAmountPair>&InBasicRewardList, TArray<ItemIdAmountPair>&InPersonRewardList
		, CurrencyDeltaPair InBasicCurrencyPair, CurrencyDeltaPair InPersonCurrencyPair)
		: _result(InResult), _playTime(InClearTime), _basicRewardList(InBasicRewardList)
		, _mainRewardList(InPersonRewardList), _basicCurrencyPair(InBasicCurrencyPair), _personCurrencyPair(InPersonCurrencyPair)
	{
	}

	virtual ~FGsUIMsgParamPartyDungeonResult() = default;
};

/**
 * Party Dungeon Clear Msg
 */
struct FGsPartyDungeonClearMsgParam : public IGsMessageParam
{
	bool _result;
	FDateTime _playTime;	
	DungeonGroupId _dungeonGroupId;
	DungeonDifficulty _dungeonDifficulty;

	TArray<ItemIdAmountPair> _basicRewardList;
	TArray<ItemIdAmountPair> _mainRewardList;
	CurrencyDeltaPair _basicCurrencyPair;
	CurrencyDeltaPair _personCurrencyPair;

	FGsPartyDungeonClearMsgParam() = default;
	explicit FGsPartyDungeonClearMsgParam(bool InResult, FDateTime& InClearTime, DungeonGroupId InDungeonGroupId, DungeonDifficulty InDungeonDifficulty
		, TArray<ItemIdAmountPair>& InBasicRewardList, TArray<ItemIdAmountPair>& InPersonRewardList
		, CurrencyDeltaPair InBasicCurrencyPair, CurrencyDeltaPair InPersonCurrencyPair)
		: _result(InResult), _playTime(InClearTime), _dungeonGroupId(InDungeonGroupId), _basicRewardList(InBasicRewardList)
		, _mainRewardList(InPersonRewardList), _basicCurrencyPair(InBasicCurrencyPair), _personCurrencyPair(InPersonCurrencyPair)
	{
	}

	virtual ~FGsPartyDungeonClearMsgParam() = default;
};



struct FGsUIMsgMsgUpdateList : public IGsMessageParam
{
	TArray<uint32> _dataList;

	FGsUIMsgMsgUpdateList() = default;
	virtual ~FGsUIMsgMsgUpdateList() = default;
	explicit FGsUIMsgMsgUpdateList(TArray<uint32>& InDataList)
		: _dataList(InDataList)
	{

	}
};

/**
 * SpaceCrack Minimap Icon Param
 */
struct FGsUIMsgParamSpaceCrackMinimapIcon final : public IGsMessageParam
{
	bool _isActive = false;
	bool _isShow = false;
	bool _isRedDot = false;

	FGsUIMsgParamSpaceCrackMinimapIcon() = default;
	virtual ~FGsUIMsgParamSpaceCrackMinimapIcon() = default;
	explicit FGsUIMsgParamSpaceCrackMinimapIcon(bool inIsActive, bool inIsShow, bool inIsRedDot)
		: _isActive(inIsActive), _isShow(inIsShow), _isRedDot(inIsRedDot)
	{
	}
};

/**
 * SpaceCrack Time Param
 */
struct FGsUIMsgParamTime final : public IGsMessageParam
{
	int32 _startSecond;
	int32 _finishSecond;

	FGsUIMsgParamTime() = default;
	virtual ~FGsUIMsgParamTime() = default;
	explicit FGsUIMsgParamTime(int32 inStartSecond, int32 inFinishSecond)
		: _startSecond(inStartSecond), _finishSecond(inFinishSecond)
	{
	}
};

struct FGsUIMsgParamSpaceKillCount final : public IGsMessageParam
{
	SpaceCrackEnemyKill _enemyKillCount;

	FGsUIMsgParamSpaceKillCount() = default;
	virtual ~FGsUIMsgParamSpaceKillCount() = default;
	explicit FGsUIMsgParamSpaceKillCount(SpaceCrackEnemyKill inEnemyKillCount)
		: _enemyKillCount(inEnemyKillCount)
	{
	}
};

struct FGsUIMsgFieldBoss : public IGsMessageParam
{
	FieldBossId _fieldBossId;
	FVector _fieldBossPos;
	time_t _msgTime;

	FGsUIMsgFieldBoss() = default;
	virtual ~FGsUIMsgFieldBoss() = default;
	explicit FGsUIMsgFieldBoss(CreatureId inFieldBossId, const FVector& inFieldBossPos, time_t inTime)
		: _fieldBossId(inFieldBossId), _fieldBossPos(inFieldBossPos), _msgTime(inTime)
	{
	}

	// 정보 추가용(ADD_FIELD_BOSS_INFO)
	explicit FGsUIMsgFieldBoss(CreatureId inFieldBossId, time_t inTime)
		: _fieldBossId(inFieldBossId), _msgTime(inTime)
	{
	}
	// 정보 삭제용(DEL_FIELD_BOSS_INFO)
	explicit FGsUIMsgFieldBoss(CreatureId inFieldBossId)
		: _fieldBossId(inFieldBossId)
	{
	}
};



struct FGsUIMsgFieldBossItemGet : public IGsMessageParam
{
	FString _userName;
	CreatureId _fieldBossId;
	ItemId _itemId;
	time_t _msgTime;

	FGsUIMsgFieldBossItemGet() = default;
	virtual ~FGsUIMsgFieldBossItemGet() = default;
	explicit FGsUIMsgFieldBossItemGet(const FString& inUserName, CreatureId inBossId, ItemId inItemId, time_t inMsgTime)
		: _userName(inUserName), _fieldBossId(inBossId), _itemId(inItemId), _msgTime(inMsgTime)
	{
	}
};

struct FGsUIMsgFieldBossContribution : public IGsMessageParam
{
	TArray<int64> _gameIdArray;

	FGsUIMsgFieldBossContribution() = default;
	virtual ~FGsUIMsgFieldBossContribution() = default;
	explicit FGsUIMsgFieldBossContribution(TArray<int64>& inGameIdArray)
		: _gameIdArray(inGameIdArray)
	{
	}
};

struct FGsUIMsgAgrroTarget : public IGsMessageParam
{
	int64 _npcId;
	int64 _targetId;

	FGsUIMsgAgrroTarget() = default;
	virtual ~FGsUIMsgAgrroTarget() = default;
	explicit FGsUIMsgAgrroTarget(int64 inNpcId, int64 inTargetId)
		: _npcId(inNpcId), _targetId(inTargetId)
	{
	}
};

struct FGsUIMsgShortcutCreature : public IGsMessageParam
{	
	CreatureId _creatureId = 0;	

	FGsUIMsgShortcutCreature() = default;
	virtual ~FGsUIMsgShortcutCreature() = default;
	explicit FGsUIMsgShortcutCreature(CreatureId inCreatureId)
		: _creatureId(inCreatureId)
	{
	}	
};

struct FGsUIMsgShortcutItem : public IGsMessageParam
{
	ItemId _itemId = 0;

	FGsUIMsgShortcutItem() = default;
	virtual ~FGsUIMsgShortcutItem() = default;
	explicit FGsUIMsgShortcutItem(ItemId inItemId)
		: _itemId(inItemId)
	{
	}
};

struct FGsUIMsgShortcutMap : public IGsMessageParam
{
	int32 _mapId = 0;
	DungeonDifficulty _difficulty;	// 2022/06/14 PKT - 파티 던전 난이도

	FGsUIMsgShortcutMap() = default;
	virtual ~FGsUIMsgShortcutMap() = default;
	explicit FGsUIMsgShortcutMap(int32 inMapId, DungeonDifficulty inDifficulty = DungeonDifficulty::NONE)
		: _mapId(inMapId), _difficulty(inDifficulty)
	{
	}
};

/**
 * 스킬 강화/업그레이드 시작
 */
struct FGsUIMsgParamEnchantSkill final : public IGsMessageParam
{
	SkillId _skillId;

	FGsUIMsgParamEnchantSkill() = default;
	virtual ~FGsUIMsgParamEnchantSkill() = default;
	explicit FGsUIMsgParamEnchantSkill(SkillId InSkillId)
		: _skillId(InSkillId)
	{
	}
};

/**
 * 스킬 강화/업그레이드에 사용
 */
struct FGsUIMsgParamSkillEnchantResult : public IGsMessageParam
{
	SkillEnchantResult _result = SkillEnchantResult::NONE;
	SkillId _skillId = INVALID_SKILL_ID;
	SkillId _resultSkillId = INVALID_SKILL_ID;

	FGsUIMsgParamSkillEnchantResult() = default;
	virtual ~FGsUIMsgParamSkillEnchantResult() = default;
	explicit FGsUIMsgParamSkillEnchantResult(SkillEnchantResult InResult, SkillId InSkillId, SkillId InResultSkillId)
		: _result(InResult), _skillId(InSkillId), _resultSkillId(InResultSkillId)
	{
	}

	bool IsUpgrade() const { return (_skillId != _resultSkillId) ? true : false; }
};

/**
 * SpaceCrack Minimap Icon Param
 */
struct FGsUIMsgParamContributionMinimapIcon final : public IGsMessageParam
{
	bool _isActive = false;		

	FGsUIMsgParamContributionMinimapIcon() = default;
	virtual ~FGsUIMsgParamContributionMinimapIcon() = default;
	explicit FGsUIMsgParamContributionMinimapIcon(bool inIsActive)
		: _isActive(inIsActive)
	{
	}
};

/**
 * uint32 값 전달 시 사용
 */
struct FGsUIMsgParamLevelUp : public IGsMessageParam
{
	uint16 _preLevel;
	uint16 _currentLevel;

	FGsUIMsgParamLevelUp() = default;
	virtual ~FGsUIMsgParamLevelUp() = default;
	explicit FGsUIMsgParamLevelUp(uint16 inPreLevel, uint16 inCurrentLevel)
		: _preLevel(inPreLevel), _currentLevel(inCurrentLevel)
	{
	}
};

struct FGsUIMsgParamRegionMapIconFiltering final : public IGsMessageParam
{
	EGsRegionMapIconFiltering _filteringType;
	bool _isVisible = true;

	FGsUIMsgParamRegionMapIconFiltering() = default;
	virtual ~FGsUIMsgParamRegionMapIconFiltering() = default;
	explicit FGsUIMsgParamRegionMapIconFiltering(EGsRegionMapIconFiltering In_filteringType,
		bool In_isVisible)
		:_filteringType(In_filteringType), _isVisible(In_isVisible)
	{
	}
};

struct FGsUIMsgParamInvadeMapInfo : public IGsMessageParam
{
	TArray<MapId> _updateMapList;
	int32 _mapUpdateCount;

	FGsUIMsgParamInvadeMapInfo() = default;
	virtual ~FGsUIMsgParamInvadeMapInfo() = default;

	explicit FGsUIMsgParamInvadeMapInfo(const TArray<MapId>& updateMapList, int32 mapUpdateCount = 0)
	{
		_updateMapList.Reset(0);
		_updateMapList.Append(updateMapList);

		_mapUpdateCount = mapUpdateCount;
	}

};
struct FGsUIMsgParamInvadeState : public IGsMessageParam
{
	enum class InvadeState : uint8 {Start, End};

	InvadeState _invadeState{ InvadeState::Start };
	MapId		_invadeMapId;
	FDateTime   _enterTime{ 0 };
	FDateTime   _leaveTime{ 0 };
	int			_secRemainStayable{ 0 };
	int			_secAddStayable{ 0 };

	FGsUIMsgParamInvadeState() = default;
	virtual ~FGsUIMsgParamInvadeState() = default;

	explicit FGsUIMsgParamInvadeState(InvadeState invadeState, MapId invadeMapId, 
		FDateTime enterTime, FDateTime leaveTime, int remainTime, int secAddStayable = 0)
		: _invadeState(invadeState)
		, _invadeMapId(invadeMapId)
		, _enterTime(enterTime)
		, _leaveTime(leaveTime)
		, _secRemainStayable(remainTime)
		, _secAddStayable(secAddStayable)
	{
	}
};

/**
 * 언어변경 전달 시 사용
 */
struct FGsUIMsgParamLanguege : public IGsMessageParam
{
	int32 _index;
	bool _bIsVoice;

	FGsUIMsgParamLanguege() = default;
	virtual ~FGsUIMsgParamLanguege() = default;
	explicit FGsUIMsgParamLanguege(int32 InIndex, bool InIsVoice)
		: _index(InIndex), _bIsVoice(InIsVoice)
	{
	}
};

/**
 * 유저 컨텍스트 UI
 */
struct FGsUIMsgParamContextUserInfo : public IGsMessageParam
{
	FGsUserContextInfo _userContextInfo;

	FGsUIMsgParamContextUserInfo() = default;
	virtual ~FGsUIMsgParamContextUserInfo() = default;

	explicit FGsUIMsgParamContextUserInfo(FGsUserContextInfo& InUserContextInfo) : _userContextInfo(InUserContextInfo)
	{
	}
};

struct FGsUIMsgParamSelectBoss : public IGsMessageParam
{
public:
	EGsBossTab _bossType;

public:
	const struct FGsSchemaWorldBossData* _worldBossData = nullptr;
	FDateTime _bossRoomOpenTime;
	FDateTime _bossSpawnTime;
	int _leftRewardCount;

public:
	const struct FGsSchemaFieldBossData* _fieldBoss = nullptr;
	const struct FGsSchemaFieldBossData* _mutantFieldBoss = nullptr;
	bool _isShowFieldBossMutant;
	time_t _fieldBossLastDeadTime;

public:
	FGsUIMsgParamSelectBoss() = default;
	virtual ~FGsUIMsgParamSelectBoss() = default;
	explicit FGsUIMsgParamSelectBoss(const struct FGsSchemaFieldBossData* inBoss
		, const struct FGsSchemaFieldBossData* inMutant
		, bool inIsShowMutant)
		: _fieldBoss(inBoss)
		, _mutantFieldBoss(inMutant)
		, _isShowFieldBossMutant(inIsShowMutant)	
	{
		_bossType = EGsBossTab::FieldBoss;
	}

	explicit FGsUIMsgParamSelectBoss(const struct FGsSchemaWorldBossData* inBoss
	, const FDateTime& inBossRoomOpenTime
	, const FDateTime& inBossSpawnTime
		, int inLeftRewardCount)
		: _worldBossData(inBoss)
		, _bossRoomOpenTime(inBossRoomOpenTime)
		, _bossSpawnTime(inBossSpawnTime)
		, _leftRewardCount(inLeftRewardCount)
	{
		_bossType = EGsBossTab::WorldBoss;
	}
};

struct FGsUIMsgParamFieldBossDeathTime : public IGsMessageParam
{
public:
	time_t _fieldBossLastDeadTime;

public:
	FGsUIMsgParamFieldBossDeathTime() = default;
	virtual ~FGsUIMsgParamFieldBossDeathTime() = default;
	explicit FGsUIMsgParamFieldBossDeathTime(time_t inLastDeadTime)
		: _fieldBossLastDeadTime(inLastDeadTime)
	{
		
	}
};

struct FGsUIMsgParamSelectQuestBoard : public IGsMessageParam
{	
	QuestStoryIdPair _quest;	
	QuestIndex _newQuestIndex;
	
	FGsUIMsgParamSelectQuestBoard() = default;
	virtual ~FGsUIMsgParamSelectQuestBoard() = default;
	explicit FGsUIMsgParamSelectQuestBoard(const QuestStoryIdPair& inQuest
	, QuestIndex inNewQuestIndex = -1)
		: _quest(inQuest), _newQuestIndex(inNewQuestIndex)
	{
	}
};

struct FGsUIMsgParamSortQuestBoard : public IGsMessageParam
{
	EGsQuestStateType _sortType;	

	FGsUIMsgParamSortQuestBoard() = default;
	virtual ~FGsUIMsgParamSortQuestBoard() = default;
	explicit FGsUIMsgParamSortQuestBoard(EGsQuestStateType inSortType)
		: _sortType(inSortType)
	{
	}
};

struct FGsUIMsgParamInputEnable : public IGsMessageParam
{
	bool _isEnable{ false };

	FGsUIMsgParamInputEnable() = default;
	virtual ~FGsUIMsgParamInputEnable() = default;
	explicit FGsUIMsgParamInputEnable(bool isEnable) : _isEnable(isEnable)
	{
	}
};

struct FGsUIMsgParamSocialEmoji : public IGsMessageParam
{
	UserSocialEmotion _emotion;

	FGsUIMsgParamSocialEmoji() = default;
	virtual ~FGsUIMsgParamSocialEmoji() = default;
	explicit FGsUIMsgParamSocialEmoji(UserSocialEmotion emotion)
		: _emotion(emotion)
	{
	}
};

struct FGsUIMsgWorldMapItemDrop : public IGsMessageParam
{
	TArray<FGsSchemaWorldMapSpoilsMonsterLevel> _monsterArray;

	FGsUIMsgWorldMapItemDrop() = default;
	virtual ~FGsUIMsgWorldMapItemDrop() = default;
	explicit FGsUIMsgWorldMapItemDrop(const TArray<FGsSchemaWorldMapSpoilsMonsterLevel>& inMonsterArray)
		: _monsterArray(inMonsterArray)
	{
	}
};

struct FGsUIMsgToggleParam : public IGsMessageParam
{
	EGsToggleType _toggle;

	FGsUIMsgToggleParam() = default;
	virtual ~FGsUIMsgToggleParam() = default;
	explicit FGsUIMsgToggleParam(EGsToggleType inToggle)
		: _toggle(inToggle)
	{
	}
};

struct FGsUIMsgActiveParam : public IGsMessageParam
{
	bool _isActive;

	FGsUIMsgActiveParam() = default;
	virtual ~FGsUIMsgActiveParam() = default;
	explicit FGsUIMsgActiveParam(bool inActive)
		: _isActive(inActive)
	{
	}
};

struct FGsUISelectPortalParam : public IGsMessageParam
{
	int32 _spotId;

	FGsUISelectPortalParam() = default;
	virtual ~FGsUISelectPortalParam() = default;
	explicit FGsUISelectPortalParam(int32 inSpotId)
		: _spotId(inSpotId)
	{
	}
};

struct FGsUIMsgFieldBossBookmarkParam : public IGsMessageParam
{
public:
	int32 _bossId;
	bool _addBookmark;

public:
	FGsUIMsgFieldBossBookmarkParam() = default;
	virtual ~FGsUIMsgFieldBossBookmarkParam() = default;
	explicit FGsUIMsgFieldBossBookmarkParam(int32 inBookmarkId, bool inAddBookmark)
		: _bossId(inBookmarkId), _addBookmark(inAddBookmark)
	{
	}
};
// 지역맵 타입 파람
struct FGsUIMsgRegionMapTypeParam : public IGsMessageParam
{
	EGsRegionMapType _regionMapType;
	// 성소 탭 페이지 쪽 클릭시 토글을 강제로 처리안하면
	// 갱신이 안됭서 강제로 토글 처리하게하니까
	// 기존 획득처 쪽 메시지에서 재귀호출이 타는거 같다...
	// 토글 메시지 강제로 쏘는거는 특정 상황에서만 쓰자...

	FGsUIMsgRegionMapTypeParam() = default;
	virtual ~FGsUIMsgRegionMapTypeParam() = default;
	explicit FGsUIMsgRegionMapTypeParam(EGsRegionMapType In_type)
		: _regionMapType(In_type)
	{
	}
};