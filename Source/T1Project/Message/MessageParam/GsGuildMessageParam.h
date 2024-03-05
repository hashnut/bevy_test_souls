// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


/**
 * int형  정보 하나 보낼 때
 */
struct FGsGuildMsgParamInt : public IGsMessageParam
{
	int32 _data;

	FGsGuildMsgParamInt() = default;
	explicit FGsGuildMsgParamInt(int32 InData)
		: _data(InData)
	{
	}
	virtual ~FGsGuildMsgParamInt() = default;
};

/**
 * uint64형  정보 하나 보낼 때
 */
struct FGsGuildMsgParamUInt64 : public IGsMessageParam
{
	uint64 _data;

	FGsGuildMsgParamUInt64() = default;
	explicit FGsGuildMsgParamUInt64(uint64 InData)
		: _data(InData)
	{
	}
	virtual ~FGsGuildMsgParamUInt64() = default;
};

/**
 * 특정 유저에 대한 처리를 할 때
 */
struct FGsGuildMsgParamUser : public IGsMessageParam
{
	UserDBId _userDBId;

	FGsGuildMsgParamUser() = default;
	explicit FGsGuildMsgParamUser(UserDBId InUserDBId)
		: _userDBId(InUserDBId)
	{
	}
	virtual ~FGsGuildMsgParamUser() = default;
};

/**
 * 유저 등급이 바뀔 때(노티 정보에 UserDBId가 없다)
 */
struct FGsGuildMsgParamMemberGrade : public IGsMessageParam
{
	FString _userName;
	GuildMemberGradeType _memberGrade;

	FGsGuildMsgParamMemberGrade() = default;
	explicit FGsGuildMsgParamMemberGrade(const FString& InUserName, GuildMemberGradeType InMemberGrade)
		: _userName(InUserName), _memberGrade(InMemberGrade)
	{
	}
	virtual ~FGsGuildMsgParamMemberGrade() = default;
};


/**
 * 특정 길드에 대한 처리를 할 때
 */
struct FGsGuildMsgParamGuild : public IGsMessageParam
{
	GuildDBId _guildDBId;

	FGsGuildMsgParamGuild() = default;
	explicit FGsGuildMsgParamGuild(GuildDBId InGuildDBId)
		: _guildDBId(InGuildDBId)
	{
	}
	virtual ~FGsGuildMsgParamGuild() = default;
};

/**
 * 길드의 특정 옵션을 바꿀 때
 */
struct FGsGuildMsgParamSetting : public IGsMessageParam
{
	enum EGsGuildSettingType
	{
		None = 0,
		Introduction,
		Notice,
		LimitLevel,
		JoinType,
		Emblem,
		Acquisition,
		SNSLink,

		All
	};

	EGsGuildSettingType _settingType;	

	FGsGuildMsgParamSetting() = default;
	explicit FGsGuildMsgParamSetting(EGsGuildSettingType InType)
		: _settingType(InType)
	{
	}
	virtual ~FGsGuildMsgParamSetting() = default;
};

/**
 * 길드 외교 길드 검색 결과
 */
struct FGsGuildMsgParamDiplomacySearchGuild final : public IGsMessageParam
{
	GuildDBId _guildDBId;
	GuildEmblemId _emblemId;
	FString _guildName;
	FString _masterName;

	explicit FGsGuildMsgParamDiplomacySearchGuild(GuildDBId InGuildDBId, GuildEmblemId InEmblemId, 
		const FString& InGuildName, const FString& InMasterName)
		:_guildDBId(InGuildDBId), _emblemId(InEmblemId), _guildName(InGuildName), _masterName(InMasterName)
	{
	}
	virtual ~FGsGuildMsgParamDiplomacySearchGuild() = default;
};

/**
 * 길드 외교 상태 변경
 */
struct FGsGuildMsgParamDiplomacyNotify final : public IGsMessageParam
{
	GuildDBId _guildDBId;
	bool _bIsDelete;

	explicit FGsGuildMsgParamDiplomacyNotify(GuildDBId InGuildDBId, bool bIsDelete)
		:_guildDBId(InGuildDBId), _bIsDelete(bIsDelete)
	{
	}
	virtual ~FGsGuildMsgParamDiplomacyNotify() = default;
};

struct FGsGuildMsgParamQuest final : public IGsMessageParam
{
	StoryId _storyId;
	QuestId _questId;
	bool _bIsReward;

	explicit FGsGuildMsgParamQuest(StoryId InStoryId, QuestId InQuestId, bool bIsReward)
		: _storyId(InStoryId), _questId(InQuestId), _bIsReward(bIsReward)
	{
	}
	virtual ~FGsGuildMsgParamQuest() = default;
};

/**
 * 길드 아이템 컬렉션 보상 정보
 */
struct FGsGuildMsgParamItemCollectionReward final : public IGsMessageParam
{
	CollectionConditionId collectionConditionId = INVALID_COLLECTION_CONDITION_ID;
	ItemDBId itemDBId = INVALID_ITEM_DB_ID;

	explicit FGsGuildMsgParamItemCollectionReward(CollectionConditionId InCollectionConditionId, ItemDBId InItemDBId)
		:collectionConditionId(InCollectionConditionId), itemDBId(InItemDBId)
	{
	}
	virtual ~FGsGuildMsgParamItemCollectionReward() = default;
};

/**
 * 기사단 창고 슬롯 확장 정보
 */
struct FGsGuildMsgParamStorageExtendSlot final : public IGsMessageParam
{
	ItemAmount _maxSlotCount = 0;
	SlotExtendNumber _slotExtendedCount = 0;

	explicit FGsGuildMsgParamStorageExtendSlot(ItemAmount InMaxSlotCount, SlotExtendNumber InSlotExtendedCount)
		:_maxSlotCount(InMaxSlotCount), _slotExtendedCount(InSlotExtendedCount)
	{
	}
	virtual ~FGsGuildMsgParamStorageExtendSlot() = default;
};

/**
 * 
 */
struct FGsGuildDungeonClearMsgParam : public IGsMessageParam
{
	FDateTime _clearTime;
	int32 _dungeonKey; // GuildDungeon에선 dungeonLevel, Agit던전에선 AgitDungeonRaidId로 활용
	TArray<ItemIdAmountPair> _guildRewardList;
	TArray<ItemIdAmountPair> _personRewardList;
	TArray<CurrencyPair> _guildRewardCurrencyList;
	TArray<CurrencyPair> _personRewardCurrencyList;

	FGsGuildDungeonClearMsgParam() = default;
	explicit FGsGuildDungeonClearMsgParam(FDateTime& inClearTime, int32 inDungeonKey, TArray<ItemIdAmountPair>& inGuildRewardList, TArray<ItemIdAmountPair>& inPersonRewardList
		, TArray<CurrencyPair> inGuildRewardCurrencyList, TArray<CurrencyPair> inPersonRewardCurrencyList)
		: _clearTime(inClearTime), _dungeonKey(inDungeonKey), _guildRewardList(inGuildRewardList), _personRewardList(inPersonRewardList)
		, _guildRewardCurrencyList(inGuildRewardCurrencyList), _personRewardCurrencyList(inPersonRewardCurrencyList)
	{
	}
	virtual ~FGsGuildDungeonClearMsgParam() = default;
};

/**
 * 던전의 상태가 변경될 때
 */
struct FGsGuildMsgParamAgitDungeonState : public IGsMessageParam
{
	AgitDungeonRaidId _raidId = 0;
	AgitDungeonRaidState _raidState = AgitDungeonRaidState::NONE;
	bool _bIsNotify = false;

	FGsGuildMsgParamAgitDungeonState() = default;
	explicit FGsGuildMsgParamAgitDungeonState(AgitDungeonRaidId InRaidId, AgitDungeonRaidState InRaidState, bool bInIsNotify)
		: _raidId(InRaidId), _raidState(InRaidState), _bIsNotify(bInIsNotify)
	{
	}
	virtual ~FGsGuildMsgParamAgitDungeonState() = default;
};
