// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedTitleEnum.h"
#include "Net/GSNet.h"
#include "Management/GsScopeHolder.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "UTIL/GsIntervalChecker.h"
#include "Guild/GsGuildPlayerData.h"
#include "Buddy/GsBuddyInfo.h"
#include "Title/GsTitlePlayerData.h"
#include "PlayableEvent/GsPlayableEventDefine.h"
#include "Classes/SecureVar/GsSecureVar.h"


// 서버에서 받는 유저 정보 구조체
// 나중에 관련 구조체 정보가 커지면 파일 분리를 고려
struct T1PROJECT_API FGsNetUserData : public IGsMessageParam
{
	// PKT_SC_USER_BASIC_DATA_READ
	int64			mGameId;
	UserDBId		mUserDBId;
	FString			mUserName;
	uint32			mTid;
	UserStatus		mUserStatus = UserStatus::USER_STATE_NORMAL;	
	uint16			mLevel;
	Exp				mExp;
	UserLookInfo	mLookInfo;
	uint8			mUserPresetId;
	CreatureGenderType mGender = CreatureGenderType::MALE;
	CreatureWeaponType mLoginWeaponType = CreatureWeaponType::NONE;

	WorldId			mHomeWorldId;

	AccountGrade	mAccountGrade = AccountGrade::NORMAL;
	FString			mGMPrefix;

	TMap<CurrencyType, FGsSecureCurrency> mCurrencyMap; // 재화 정보

	CustomizeFaceData mFaceData;
	CustomizeBodyData mBodyData;

	ClientOptionData mOptionData;

	FGsGuildPlayerData mGuildData;

	// 2022/02/03 PKT - Title(칭호)
	FGsTitlePlayerData _titleData;

	// 2022/08/26 PKT - Invade Battle Level
	InvadeBattleLevel _InvadeBattleLevel = 0;

	// PKT_SC_SKILL_READ
	struct FSkillSlot
	{
		SkillId		mSkillId;
		SkillLevel	mSkillLevel;
		float		mCooldown;
	};
	TArray<FSkillSlot> listSkillSlot;

	//PKT_SC_QUEST_READ
	struct FQuestInfo
	{
		StoryId mStoryId;
		QuestId mQuestId;
		QuestType mQuestType;
		QuestState mQuestState;
		QuestIndex mQuestIndex;
		EventActionIndex mPreEventActionIndex;
		EventActionIndex mPostEventActionIndex;

		TArray<int32> mObjectiveValueList;
	};
	TArray<FQuestInfo> listQuestInfo;

	// PKT_SC_BOOKMARK_READ
	struct FBookmarkInfo
	{
		uint64 mid;
		FString mName;
		FVector mPos;
		int mMapId;
	};
	TArray<FBookmarkInfo> listBookmarkInfo;
	TArray<FieldBossId> bossBookMarkArray;

	//bonus stat
	int32 _statPoint;
	TMap<StatType, int32> _bonusStatMap;
	int32 _usedBonusStatItemCount = 0;
	int32 _bonusStatItemExtendCount = 0;

	//panelty
	TArray<DeathPenaltyExp> expPenalty;
	TArray<DeathPenaltyItem> itemPenalty;
	int32 freeExpRestoreCount;
	int32 freeExpRestoreMaxCount;
	int32 freeItemRestoreCount;
	int32 freeItemRestoreMaxCount;

	//death
	FString attackerName;
	WorldId attackerWorldId;
	FString attackerGuildName;
	GuildEmblemId attackerGuildEmblemId = INVALID_GUILD_EMBLEM_ID;
	
	int32 attackerNpcId;
	DeathPenaltyExp recentPenaltyExp;
	TArray<DeathPenaltyItem> recentPenaltyList;
	TArray<ResurrectType> resurrectTypeList;
	int32 autoRespawnSecond;
	uint64 immediateResurrectionCost;
	CurrencyType immediateResurrectionCurrencyType = CurrencyType::DIA;
	int64 InstantResurrectionSecond;

	int32 guildDungeonLevel;
	bool isClearGuildDungeon;
	time_t dungeonResetTime = 0;


	//침공전 남은 만료 시간
	int64 invasionTime;

#ifdef R3_ARCAS_NOT_USE
	struct FGsBlessData
	{
		ArcasPoint mBlessPoint{ 0 };
		bool  mIsOnFairyBless{ false };
		bool  mIsOnArcasBless{ false };
		float mMeditationTime{ 0.f };

		FDateTime mMeditaionPacketTime{ 0 };
	};
	FGsBlessData _expBless;
#else
	ArcasPoint _blessPoint{ 0 };
#endif

	//Pvp
	int64 pvpAlignmentPoint;

	//Buddy
	struct FBuddyData
	{
		bool mIsRedDot = false;
		bool mIsAcceptRedDot = false;
		TMap<UserDBId, TSharedPtr<FGsBuddyInfo>> mBuddyMap;			// 친구 목록
		TMap<UserDBId, TSharedPtr<FGsBuddyInfo>> mApplyBuddyMap;	// 친구 신청 목록
		TMap<UserDBId, TSharedPtr<FGsBuddyInfo>> mAcceptBuddyMap;	// 친구 수락 목록	
	};
	FBuddyData mBuddyData;

	// local 정령탄 ui on, off 상태
	bool mSpiritShotOnOff = false;

	struct FGsSpaceCrackData
	{
		SpaceCrackEntryCount mSpaceCrackEntryCount;
		SpaceCrackEntryCount mSpaceCrackMaxEntryCount;
		bool mEnableWeekReward;
		bool mIsRewardCompleted;
	};
	FGsSpaceCrackData _spaceCrack;

	time_t _createdTime;

	uint8 _dailyTradingCount;

public:
	void Clear();
	void ClearPenalty();
	void ClearBonusStat() { _bonusStatMap.Empty(); }
	void ClearQuest() { listQuestInfo.Empty(); }

public:
	Currency GetCurrency(CurrencyType InType) const;
	void SetCurrency(CurrencyType InType, Currency InAmount);
	bool IsValidAmount(CurrencyType InType, Currency InAmount, UpdateSignType InSignType);
	int32 GetStatPoint() const { return _statPoint; }
	void SetBonusStat(TMap<StatType, int32> inBonusStat) { _bonusStatMap = inBonusStat; }	
	TMap<StatType, int32> GetBonusStat() const {return _bonusStatMap;}

	int32 GetUsedBonusStatItemCount() const { return _usedBonusStatItemCount; }
	int32 GetBonusStatItemExtendCount() const { return _bonusStatItemExtendCount; }

#ifdef R3_ARCAS_NOT_USE
	const FGsBlessData& GetBlessData() const { return _expBless; }
	void SetEndMeditation();
#else
	const int32 GetBlessPoint() const;
#endif
	
	void GetMultiLevelRank(TArray<MultiLevelRankSlotInfo>& outRankSlot, TArray<MultiLevelRank>& outRewardRankList) const;	
	int32 GetGuildDungeonLevel() const {return guildDungeonLevel;}	
	bool IsClearGuildDungeon() const {return isClearGuildDungeon;}
	void SetClearGuildDungeon(bool inIsClear) { isClearGuildDungeon = inIsClear;}
	time_t GetDungeonResetTime() const { return dungeonResetTime; }

	// 2022/02/03 PKT - Title(칭호)
public:
	const FGsTitlePlayerData& GetTitleData() const;
};

// 채널 정보
struct T1PROJECT_API FGsMapChannelData
{
	ChannelId _channel;
	ChannelCongestionStatus _state;

	FGsMapChannelData(ChannelId InChannel, ChannelCongestionStatus InState)
		: _channel(InChannel), _state(InState)
	{
	}
};

struct FGsChatBlockUser
{
	UserDBId _uid;
	FString  _name;

	bool operator == (const FGsChatBlockUser& rhs) const 
	{
		return _uid == rhs._uid;
	}
};

/**
 * InGame내 서버 동적 정보 데이터 캐싱 클래스
 * 관련 데이터 동기화는 Network Recv -> FGsGameDataManager Set -> SendMessage -> RecvMessage 로 처리 요망
 */
class T1PROJECT_API FGsGameDataManager : public IGsManager
{
private:
	FGsNetUserData _userData;

	ChannelId _currentChannel = 0; // 현재 채널
	TArray<FGsMapChannelData> _channelList; // 채널 리스트. 요청해서 받는다
	FGsIntervalChecker _checkerChannelChange; // 채널 변경 타임 체크
	FGsIntervalChecker _checkerChannelList; // 채널 리스트 요청 타임 체크(잦은 요청 방지)

	FVector					_spawnPos;
	FRotator				_spawnRot;

	// 스폰시 살아있는지 죽었는지 동기화 처리 변수
	bool					_isSpawnAlive;

	// 테리토리 타입
	TerritoryType			_territoryType = TerritoryType::NONE;

	// fairy list
	bool					_fairyDataComplete{ false };
	int32					_fairyRawDataSize{ 0 };
	TArray<uchar>			_fairyRawData;

	TArray<CollectionId>	_fairyCollectionComplete;
	TArray<CollectionConditionId> _fairyCollectionCompleteCondition;

	FDateTime				 _fairySummonWaitingPacketTime;
	TArray<FairyConfirmData> _fairySummonWaitings;

	bool					_costumeDataComplete{ false };
	int32					_costumeRawDataSize{ 0 };
	TArray<uchar>			_costumeRawData;

	TArray<CollectionId>	_costumeCollectionComplete;
	TArray<CollectionConditionId> _costumeCollectionCompleteCondition;

	FDateTime				 _costumeSummonWaitingPacketTime;
	TArray<CostumeConfirmData> _costumeSummonWaitings;

	// mail
	bool					_isUnReadMail = false;
	MailDBId				_isUnReadMailDBId = 0;
	bool					_isNewSystemMail = false;
	UserDBId				_mailOwner = INVALID_USER_DB_ID;
	// buddy
	bool					_isNewDatabuddy = false;
	// Attendance
	bool					_isNewDataAttendance = false;	
	// Codex
	bool					_isCodexRedDot = false;	
		
	TArray<FGsChatBlockUser> _blockList;
	
	// 거래소 정산내역 유무 (월드 진입 시 ScopeGame인 MarketManager 생성 전에 노티가 올 수 있어서 여기에 저장)
	int32 _marketSettlementCount = 0;
	int32 _wdMarketSettlementCount = 0;

	// 인게임에서 사용할 카메라 특정 본 위치(캐릭터 선택창에서 저장한다)
	float _savedCameraTargetBoneHeight = 0.0f;

	// 테스트: 카메라 bone 높이 log 보여줄지
	bool _testIsShowLogCamBoneHeight = false;

	WorldId  _loadedWorldId{ INVALID_WORLD_ID };
	EnterWorldServerType _loadedWorldType{ EnterWorldServerType::NONE };
	bool _isInvasionDefenseState;

	// Exp 표시 효율을 위해 캐싱해 둠
	Level _cachedLevelForExp = 0;
	Exp _cachedCurrLevelMax = 0;
	Exp _cachedPrevLevelAccum = 0;

	// 체크 포인트(서버)
	EGsPlayableEventServerCheckPointType _playableEventCheckPoint = 
		EGsPlayableEventServerCheckPointType::First;

public:
	FGsGameDataManager() = default;
	virtual ~FGsGameDataManager() = default;

public:
	//FGsGameDataManager operator = (PD::SC::PKT_SC_USER_BASIC_DATA_READ& rhs) { Set(rhs); return *this; }
	//FGsGameDataManager operator = (PD::SC::PKT_SC_SKILL_READ& rhs)			{ Set(rhs); return *this; }

public:
	//IGsManager
	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual void InitializeAfterResourceDownload() override;

public:
	void OnExitGameFlow();

public:
	const FGsNetUserData* GetUserData() const { return &_userData; }
	Currency GetCurrency(CurrencyType InType) const;

	ChannelId GetCurrentChannel() const { return _currentChannel; }
	TerritoryType GetTerritoryType() const { return _territoryType; }
	const TArray<FGsMapChannelData>& GetChannelList() const { return _channelList; }
	float GetChangeChannelRemainTime() const;
	bool CanChangeChannel() const;
	bool SendRequestChannelList();

#ifdef R3_ARCAS_NOT_USE
	void SetEndMeditation();
#endif

	//ClientOptionData* GetOptionData() const { return &_userData.mOptionData; }

public:
	void Set(PD::SC::PKT_SC_USER_BASIC_DATA_READ& Packet);
	void Set(PD::SC::PKT_SC_SKILL_READ& Packet);
	void Set(PD::SC::PKT_SC_USER_EXP_UPDATE_READ& Packet);
	void Set(PD::SC::PKT_SC_USER_CURRENCY_FINAL_UPDATE_READ& Packet);
	void Set(PD::SC::PKT_SC_USER_LOOK_INFO_CHANGED_READ& Packet);
	void Set(PD::SC::PKT_SC_CREATURE_COMBAT_STATUS_READ& Packet);
	void Set(PD::SC::PKT_SC_QUEST_READ& Packet);
	void Set(PD::SC::PKT_SC_ACK_QUEST_REWARD_READ& Packet);
	void Set(PD::SC::PKT_SC_ACK_QUEST_SUB_REWARD_READ& Packet);
	void Set(PD::SC::PKT_SC_ACK_QUEST_REPEAT_REWARD_READ& Packet);
	void Set(PD::SC::PKT_SC_CHANNEL_LIST_READ& Packet);
	void Set(PD::SC::PKT_SC_BOOKMARK_READ& Packet);
	void Set(PD::SC::PKT_SC_STAT_POINT_READ& inPacket);
	void Set(PD::SC::PKT_SC_SPAWN_ME_READ& Packet);
	void Set(PD::SC::PKT_SC_FAIRY_READ& Packet);
	void Set(PD::SC::PKT_SC_BONUS_STAT_READ& inPacket);
	void Set(PD::SC::PKT_SC_FAIRY_COLLECTION_READ& Packet);
	void Set(PD::SC::PKT_SC_DEATH_PENALTY_EXP_LIST_READ& Packet);
	void Set(PD::SC::PKT_SC_DEATH_PENALTY_ITEM_ADD_READ& Packet);
	void Set(PD::SC::PKT_SC_DEATH_PENALTY_ITEM_LIST_READ& Packet);
	void Set(PD::SC::PKT_SC_DEATH_PENALTY_EXP_ADD_READ& Packet);
	void Set(PD::SC::PKT_SC_DEATH_PENALTY_FREE_COUNT_READ& Packet);
	void Set(PD::SC::PKT_SC_ACK_EXP_RECOVERY_COUNT_READ& Packet);
	void Set(PD::SC::PKT_SC_USER_DEATH_READ& Packet);	
	void Set(PD::SC::PKT_SC_ARCAS_READ& Packet);
	void Set(PD::SC::PKT_SC_UPDATE_ARCAS_POINT_READ& Packet);
#ifdef R3_ARCAS_NOT_USE
	void Set(PD::SC::PKT_SC_UPDATE_ARCAS_MEDITATION_REMAINTIME_READ& Packet);
	void Set(PD::SC::PKT_SC_UPDATE_ARCAS_BUFF_READ& Packet);
#endif
	void Set(PD::SC::PKT_SC_NOTIFY_MAIL_UNREAD_READ& Packet);
	void Set(PD::GC::PKT_GC_NOTIFY_NEW_SYSTEM_MAIL_READ& Packet);
	void Set(PD::SC::PKT_SC_FAIRY_SUMMON_WAIT_READ& Packet);
	void Set(PD::SC::PKT_SC_COSTUME_COLLECTION_READ& Packet);
	void Set(PD::SC::PKT_SC_COSTUME_SUMMON_WAIT_READ& Packet);	
	void Set(UserStatus Status);
	void SetTerritoryType(TerritoryType TerritoryType);
	void SetLevel(uint16 Level);
	void SetCurrentChannel(ChannelId InChannelId) { _currentChannel = InChannelId; }

	// FGsCurrencyHelper::SetCurrency를 이용해주세요
	void SetGuildData(PD::SC::PKT_SC_USER_GUILD_INFO_READ& Packet);
	void ClearGuildData();
	void SetChangeGuildEmblem(GuildEmblemId InEmblemId);
	void Set(PD::SC::PKT_SC_COSTUME_READ& Packet);
	void Set(PD::SC::PKT_SC_UPDATE_PVP_ALIGNMENT_POINT_READ& Packet);

	void Set(PD::GC::PKT_GC_CHAT_BLACKLIST_READ& Packet);
	void Set(PD::GC::PKT_GC_ACK_CHAT_BLACKLIST_ADD_READ& Packet);
	void Set(PD::GC::PKT_GC_ACK_CHAT_BLACKLIST_REMOVE_READ& Packet);

	void Set(PD::GC::PKT_GC_ACK_BUDDY_LIST_READ& Packet);
	void Set(PD::GC::PKT_GC_ACK_BUDDY_RECV_INVITATION_LIST_READ& Packet);
	void Set(PD::GC::PKT_GC_ACK_BUDDY_SEND_INVITATION_LIST_READ& Packet);

	void Set(PD::GC::PKT_GC_ACK_BUDDY_LIST_V2_READ& Packet);
	void Set(PD::GC::PKT_GC_ACK_BUDDY_RECV_INVITATION_LIST_V2_READ& Packet);
	void Set(PD::GC::PKT_GC_ACK_BUDDY_SEND_INVITATION_LIST_V2_READ& Packet);

	void Set(PD::SC::PKT_SC_GAME_OPTION_READ& Packet);
	void Set(PD::SC::PKT_SC_CONTENTS_STATE_READ& Packet);

	// 천마석 ui on, off 동기화
	// 처음 로비에서 게임 들어올때 받고
	// 그다음엔 바뀔때마다 바뀐정보 저장하기
	void SetSpiritShotOnOff(bool In_isOn);

	void Set(PD::SC::PKT_SC_USER_SPACE_CRACK_TICKET_READ& Packet);		
	void Set(PD::GC::PKT_GC_ACK_OPEN_GUILD_DUNGEON_INFO_READ& Packet);
	void Set(PD::GC::PKT_GC_INFORM_GUILD_DUNGEON_LEVEL_READ& Packet);	

	void SetBonusStatExtendCount(int32 InTotalCount);
	void SetUsedBonusStatItemCount(int32 InUsedItemCount);

	void SetUserPreset(PresetId InPresetId);

	// 현재는 플레이블 이벤트 캐릭터 설정용으로 사용
	void SetTId(uint32 In_TId);

public:
	const FVector& GetSpawnLocation() { return _spawnPos; }
	const FRotator& GetSpawnRotation() { return _spawnRot; }
	bool			IsSpawnAlive() { return _isSpawnAlive; }

public:
	bool IsFairyDataComplete() { return _fairyDataComplete; }
	int32 GetFairyRawDataSize() { return _fairyRawDataSize; }
	const uchar* GetFairyRawData() { return _fairyRawData.GetData(); }
	const TArray<CollectionId>& GetFairyCollectionCompleteList() const { return _fairyCollectionComplete; }
	const TArray<CollectionConditionId>& GetFairyCollectionCompleteConditionList() const { return _fairyCollectionCompleteCondition; }
	const TArray<FairyConfirmData>& GetFairySummonWaitingList() const { return _fairySummonWaitings; }
	FDateTime GetFairySummonWaitingsPacketTime() {return _fairySummonWaitingPacketTime; }

	void ResetFairyData();
	void ResetFairyCollectionData();
	void ResetFairySummonWaitingList() { _fairySummonWaitings.Reset();}

public:
	bool IsCostumeDataComplete() { return _costumeDataComplete; }
	int32 GetCostumeRawDataSize() { return _costumeRawDataSize; }
	const uchar* GetCostumeRawData() { return _costumeRawData.GetData(); }
	const TArray<CollectionId>& GetCostumeCollectionCompleteList() const { return _costumeCollectionComplete; }
	const TArray<CollectionConditionId>& GetCostumeCollectionCompleteConditionList() const { return _costumeCollectionCompleteCondition; }
	const TArray<CostumeConfirmData>& GetCostumeSummonWaitingList() const { return _costumeSummonWaitings; }
	FDateTime GetCostumeSummonWaitingsPacketTime() { return _costumeSummonWaitingPacketTime; }

	void ResetCostumeData();
	void ResetCostumeCollectionData();
	void ResetCostumeSummonWaitingList() { _costumeSummonWaitings.Reset(); }

public:
	void RemoveDeathPenaltyList(TArray<DeathPenaltyRestore>& inPenaltyList);
	void RemoveDeathPenaly(DeathPenaltyId inPenaltyId);

	// mail
public:
	bool IsUnReadMail() { return _isUnReadMail; }
	void SetUnReadMail(bool inValue) { _isUnReadMail = inValue; }
	bool IsNewSystemMail() { return _isNewSystemMail; }
	void SetNewSystemMail(bool inValue) { _isNewSystemMail = inValue; }

	// buddy
public:
	bool IsNewDataBuddy() { return _isNewDatabuddy; }
	void SetNewDatabuddy(bool inValue) { _isNewDatabuddy = inValue; }

	// buddy
public:
	bool IsNewDataAttendance() { return _isNewDataAttendance; }
	void SetNewDatAttendance(bool inValue) { _isNewDataAttendance = inValue; }	

	// Codex
	bool IsCodexRedDot() { return _isCodexRedDot; }
	void SetCodexRedDot(bool inValue) { _isCodexRedDot = inValue; }

public:
	const TArray<FGsChatBlockUser>& GetChatBlockList() const {
		return _blockList;
	}

	// 천마석(정령탄)
public:
	// on, off 인가
	bool GetIsSpiritShotOnOff()
	{
		return _userData.mSpiritShotOnOff;
	}

	// 인터서버 침공
public:
	// 로드된 월드 Set, InterServerManager에 상태 체크
	void SetLoadWorldInfo(WorldId loadedWorldId, EnterWorldServerType worldServerType);
	WorldId GetLoadWorldId() { return _loadedWorldId; }
	EnterWorldServerType GetLoadWorldType() { return _loadedWorldType;  }
	// 다른 월드를 침공한 상태인지 체크하기 위한 인터페이스 함수.
	bool IsInvadeWorld() const;
	bool IsInInterWorld() const;
	void SetInvasionDefenseState(bool inActive);
	bool GetInvasionDefenseState() {return _isInvasionDefenseState;}

	// 거래소
public:
	int32 GetMarketSettlementCount(bool bIsWroldMarket) const;
	void SetMarketSettlementCount(bool bIsWroldMarket, int32 InSettlementCount);

	// 카메라
public:
	void SetSavedCameraTargetBoneHeight(float In_val)
	{
		_savedCameraTargetBoneHeight = In_val;
	}
	float GetSavedCameraTargetBoneHeight()
	{
		return _savedCameraTargetBoneHeight;
	}
	void SetIsShowTestCamBoneLog(bool In_isShow)
	{
		_testIsShowLogCamBoneHeight = In_isShow;
	}
	bool GetIsShowTestCamBoneLog()
	{
		return _testIsShowLogCamBoneHeight;
	}

	// 2022/02/03 PKT - Title(칭호)
public:
	void SetTitleData(const FGsTitlePlayerData::TitleSet& InFrontData, const FGsTitlePlayerData::TitleSet& InBackData);
	// 2022/08/26 PKT - Invade Battle Level
	void SetInvadeBattleLevel(InvadeBattleLevel InLevel);

public:
	void ResetGuildPlayerData();
	void ResetGuildDungoen();

public:
	void RemoveBossBookmark(int32 inBookMark);
	void AddBossBookmark(int32 inBookmark);
	bool IsBossBookmark(int32 inBookmark);

public:
	void SetCharacterCreatedTime(time_t InCreatedTime);

public:
	float GetCurrentLevelExpRate();

public:
	void SetGuildDungeonLevel(int32 inLevel);

public:
	void SetPlayableEventCheckPoint(EGsPlayableEventServerCheckPointType In_checkPoint);
	EGsPlayableEventServerCheckPointType GetPlayableEventCheckPoint()
	{
		return _playableEventCheckPoint;
	}
	
	void SetDailyTradingCount(int32 InCount);
};
#define GGameData() UGsScopeHolder::GetGlobalManagerFType<FGsGameDataManager>(UGsScopeGlobal::EManagerType::GameData)

