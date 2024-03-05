// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameDataManager.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Shared/Shared/SharedInclude/SharedEnums_client.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"

#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Guild/GuildDungeon/GsSchemaGuildDungeon.h"
#include "User/GsSchemaUserData.h"

#include "Net/GsNetSendServiceWorld.h"
#include "UTIL/GsText.h"

#include "Buddy/GsBuddyInfo.h"
#include "Reward/GsRewardHelper.h"

#include "GsTable.h"
#include "UTIL/GsDir.h"
#include "T1Project.h"
#include "Net/GsNetSendService.h"
#include "Currency/GsCurrencyHelper.h"
#include "Guild/GsGuildHelper.h"
#include "Classes/SecureVar/GsSecureVar.h"
#include "Message/MessageParam/GsMessageParam.h"

//----------------------------------------------------------------------------------------------------------------------
Currency FGsNetUserData::GetCurrency(CurrencyType InType) const
{
	if (const FGsSecureCurrency* amountPtr = mCurrencyMap.Find(InType))
	{
		Currency Value = 0;
		amountPtr->Get(Value);
		return Value;
	}

	return 0;
}

void FGsNetUserData::SetCurrency(CurrencyType InType, Currency InAmount)
{	
	if (mCurrencyMap.Contains(InType))
	{
		mCurrencyMap[InType].Set(InAmount);
	}
	else
	{
		mCurrencyMap.Emplace(InType, InAmount);
	}
}

bool FGsNetUserData::IsValidAmount(CurrencyType InType, Currency InAmount, UpdateSignType InSignType)
{
	Currency currAmount = GetCurrency(InType);

	if (UpdateSignType::INCREASE == InSignType)
	{
		return (FGsCurrencyHelper::GetCurrencyMaxAmount(InType) >= (currAmount + InAmount)) ? true : false;
	}

	return (currAmount >= InAmount) ? true : false;
}

#ifdef R3_ARCAS_NOT_USE
void FGsNetUserData::SetEndMeditation()
{
	_expBless.mMeditationTime = 0.f;
	_expBless.mMeditaionPacketTime = 0;
}
#endif

void FGsNetUserData::GetMultiLevelRank(TArray<MultiLevelRankSlotInfo>& outRankSlot, TArray<MultiLevelRank>& outRewardRankList) const
{

}

void FGsNetUserData::Clear()
{
	ClearPenalty();
	ClearBonusStat();
	
	_titleData.CleanUp();
	_InvadeBattleLevel = 0;
	bossBookMarkArray.Empty();
}

void FGsNetUserData::ClearPenalty()
{
	expPenalty.Empty();
	itemPenalty.Empty();
	freeExpRestoreCount = 0;
	freeExpRestoreMaxCount = 0;
	freeItemRestoreCount = 0;
	freeItemRestoreMaxCount = 0;
}

const FGsTitlePlayerData& FGsNetUserData::GetTitleData() const
{
	return _titleData;
}

const int32 FGsNetUserData::GetBlessPoint() const
{
	return FMath::CeilToInt(_blessPoint);
}

//----------------------------------------------------------------------------------------------------------------------

void FGsGameDataManager::Initialize()
{
	ResetFairyData();

	_userData._titleData.CleanUp();
	_userData._InvadeBattleLevel = 0;
}

void FGsGameDataManager::Finalize()
{
	_channelList.Empty();	
}

void FGsGameDataManager::InitializeAfterResourceDownload()
{
	if (UGsDataContainManager* dataContainMgr = GData())
	{
		if (const UGsGlobalConstant* globalConst = dataContainMgr->GetGlobalData())
		{
			// _checkerChannelChange :  이 쿨타임은 서버가 보내준다			
			_checkerChannelList.SetIntervalSecond(globalConst->_requestChannelListCooltime);
		}
	}
}

void FGsGameDataManager::OnExitGameFlow()
{
	_marketSettlementCount = 0;
	_wdMarketSettlementCount = 0;
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_USER_BASIC_DATA_READ& Packet)
{
	_userData.Clear();

	_userData.mGameId			= Packet.GameId();
	_userData.mUserDBId			=  Packet.UserDBId();
	_userData.mUserName			= Packet.UserName();
	_userData.mTid				= Packet.CreatureId();	
	_userData.mUserStatus		= (UserStatus)Packet.UserStatus();
	_userData.mLevel			= Packet.Level();
	_userData.mExp				= Packet.Exp();
	_userData.mLookInfo			= Packet.LookInfo();
	_userData.mHomeWorldId		= Packet.HomeWorldId();
	_userData.mLoginWeaponType	= Packet.CurrWeaponType();
	_userData.mAccountGrade		= Packet.AccountGrade();
	_userData.mGMPrefix			= Packet.GmPrefix();

	_userData.mCurrencyMap.Empty();
	_userData.SetCurrency(CurrencyType::DIA, Packet.ConstRefCurrencyTable().mDia);
	_userData.SetCurrency(CurrencyType::FREE_DIA, Packet.ConstRefCurrencyTable().mFreeDia);
	_userData.SetCurrency(CurrencyType::GOLD, Packet.ConstRefCurrencyTable().mGold);
	_userData.SetCurrency(CurrencyType::MILEAGE, Packet.ConstRefCurrencyTable().mMileage);
	_userData.SetCurrency(CurrencyType::CASH_MILEAGE, Packet.ConstRefCurrencyTable().mCashMileage);
	_userData.SetCurrency(CurrencyType::MONSTER_BOOK_POINT, Packet.ConstRefCurrencyTable().mMonsterBookPoint);

	_userData.SetCurrency(CurrencyType::SUMMON_FAIRY_MILEAGE, Packet.ConstRefCurrencyTable().mSummonFairyMileage);
	_userData.SetCurrency(CurrencyType::SUMMON_COSTUME_MILEAGE, Packet.ConstRefCurrencyTable().mSummonCostumeMileage);
	_userData.SetCurrency(CurrencyType::SUMMON_UNIQUE_FAIRY_MILEAGE, Packet.ConstRefCurrencyTable().mSummonUniqueFairyMileage);
	_userData.SetCurrency(CurrencyType::SUMMON_UNIQUE_COSTUME_MILEAGE, Packet.ConstRefCurrencyTable().mSummonUniqueCostumeMileage);


	if (const FGsSchemaUserData* userTable = UGsTableUtil::FindRow<FGsSchemaUserData>(_userData.mTid))
	{
		_userData.mGender = userTable->genderType;
	}

	_userData.mUserPresetId = Packet.PresetId();

	// PKT_SC_USER_BASIC_DATA_READ 패킷은 게임 진입전에도 올수 있는 패킷
	if (GCoolTime())
	{
		GCoolTime()->SetCoolTime(EGsCoolTime::Preset, 0, 0, Packet.PresetChangeCoolDown(), true);
		if (0.f < Packet.ClassWeaponChangeCoolDown())
		{
			GCoolTime()->SetCoolTime(EGsCoolTime::Change_Weapon, 0, 0, Packet.ClassWeaponChangeCoolDown(), true);
		}
	}

	_userData.mFaceData = Packet.CustomizingFaceData();
	_userData.mBodyData = Packet.CustomizingBodyData();

	_userData.mOptionData = Packet.ClientOptionData();

	//stat
	_userData._statPoint = Packet.StatPoint();
	_userData._usedBonusStatItemCount = Packet.UsedBonusStatItemCount();
	_userData._bonusStatItemExtendCount = Packet.BonusStatItemExtendCount();

	// Guild
	_userData.mGuildData.Set(Packet);	

	_userData._dailyTradingCount = Packet.DailyTradingCnt();

	_userData.bossBookMarkArray.Empty();
	Packet.MakeBossBookMarkListVector<TArray<FieldBossId>, FieldBossId>(_userData.bossBookMarkArray);

	// 유저정보 받을 때 초기화 되어야하는 부분 처리
	_isUnReadMail = false;

	// Send Message
	GMessage()->GetGameObject().SendMessage(MessageGameObject::UPDATE_USER_DATA, &_userData);
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_USER_LOOK_INFO_CHANGED_READ& Packet)
{
	_userData.mLookInfo = Packet.LookInfo();
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_CREATURE_COMBAT_STATUS_READ& Packet)
{
	_userData.mUserStatus = Packet.IsCombat() ? UserStatus::USER_STATE_COMBAT : UserStatus::USER_STATE_NORMAL;
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_SKILL_READ& Packet)
{
	_userData.listSkillSlot.Empty();

	using skillSlotIter = PktIterator<PD::SC::PKT_SC_SKILL_READ::SkillSlotList>;
	for (skillSlotIter it = Packet.GetFirstSkillSlotListIterator(); 
		it != Packet.GetLastSkillSlotListIterator(); ++it)
	{
		FGsNetUserData::FSkillSlot newInfo;
		newInfo.mSkillId	= it->SkillId();
		newInfo.mSkillLevel = it->SkillLevel();
		newInfo.mCooldown	= it->Cooldown();

		_userData.listSkillSlot.Emplace(newInfo);
	}

	// Send Message
	GMessage()->GetGameObject().SendMessage(MessageGameObject::UPDATE_USER_SKILL, &_userData);
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_USER_EXP_UPDATE_READ& Packet)
{
	FGsMessageHolder* msgMgr = GMessage();

	if (UpdateSignType::INCREASE == Packet.UpdateSignType())
	{
		check(MAX_EXP >= (_userData.mExp + Packet.UpdateExp()));
		_userData.mExp += Packet.UpdateExp();

		msgMgr->GetUserBasicInfo().SendMessage(MessageUserInfo::INCREASE_EXP, Packet.UpdateExp());
	}
	else
	{
		check(Packet.UpdateExp() <= _userData.mExp);
		_userData.mExp -= Packet.UpdateExp();
	}
	
	msgMgr->GetUserBasicInfo().SendMessage(MessageUserInfo::EXP, _userData.mExp);
	
	//GSLOG(Log, TEXT("User now Exp [_userData.mExp:%u]"), _userData.mExp);
}
void FGsGameDataManager::Set(PD::SC::PKT_SC_USER_CURRENCY_FINAL_UPDATE_READ& Packet)
{
	TArray<CurrencyPair> listCurrency;
	Packet.MakeCurrencyFinalVector< TArray<CurrencyPair>, CurrencyPair>(listCurrency);

	for (CurrencyPair& el : listCurrency)
	{
		CurrencyType currencyType = el.mCurrencyType;
		Currency totalCurrency = el.mCurrency;
		Currency currCurrency = _userData.GetCurrency(currencyType);

		UpdateSignType signType = totalCurrency > currCurrency ? UpdateSignType::INCREASE : UpdateSignType::DECREASE;
		if (UpdateSignType::INCREASE == signType)
		{
			FGsMessageHolder* msgMgr = GMessage();

			// 서버가 최대값으로 절삭해서 주므로, 클라가 초과하는지 여부는 알 수 없다.
			if (FGsCurrencyHelper::GetCurrencyMaxAmount(currencyType) <= (totalCurrency))
			{
				// 최대값에 도달했다고 메시지 전송
				msgMgr->GetUserBasicInfo().SendMessage(MessageUserInfo::CURRENCY_MAX, static_cast<uint64>(currencyType));
			}

			if (CurrencyType::GOLD == currencyType)
			{
				// 변화량 정보 
				Currency currencyDelta = totalCurrency - currCurrency;

				// 골드 획득 연출을 위해 보냄
				// 음수에 대한 값을 처리했는지는 확인 필요함
				msgMgr->GetUserBasicInfo().SendMessage(MessageUserInfo::INCREASE_GOLD, currencyDelta);
			}
		}

		// 갱신
		_userData.SetCurrency(currencyType, totalCurrency);

		GMessage()->GetUserBasicInfo().SendMessage(MessageUserInfo::CURRENCY,
			static_cast<uint8>(currencyType));
	}
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_BONUS_STAT_READ& inPacket)
{
	TMap<StatType, int32> statInfoMap;
	int count = inPacket.StatInfoSize() / sizeof(StatInfo);
	if (count > 0)
	{
		using StatInfoArray = TArray<StatInfo>;
		StatInfoArray statInfoArray;
		inPacket.MakeStatInfoVector<StatInfoArray, StatInfo>(statInfoArray);

		for (int i = 0; i < statInfoArray.Num(); ++i)
		{
			StatType type = statInfoArray[i].mType;
			int32 value = statInfoArray[i].mValue;

			statInfoMap.Emplace(type, value);

#if WITH_EDITOR
			FString statName = FGsTextUtil::GetStringFromEnum(TEXT("StatType"), type);
			GSLOG(Log, TEXT("FGsNetMessageHandlerGameObject - UpdateBonusStat - stat : %s\tvalue : %d"), *statName, value);
#endif
		}

		_userData.SetBonusStat(statInfoMap);
	}
	else
	{
		_userData.ClearBonusStat();
	}	
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_DEATH_PENALTY_EXP_LIST_READ& inPacket)
{
	_userData.expPenalty.Empty();	
	for (PD::SC::PKT_SC_DEATH_PENALTY_EXP_LIST_READ::DeathPenaltyExpListIterator iter = inPacket.GetFirstDeathPenaltyExpListIterator();
		iter != inPacket.GetLastDeathPenaltyExpListIterator();
		++iter)
	{
		const DeathPenaltyExp& penalty = iter->DeathPenaltyExp();

		//만약 패널티 자동 복구 기간이 현재 시간 보다 빠르거나 같으면 패널티 정보에 포함하지 않는다. 
		if (FGsTimeSyncUtil::IsExpried(penalty.mExpireTime))
		{
			continue;
		}

		_userData.expPenalty.Add(penalty);		

#if PENALTY_DEBUG
		GSLOG(Log, TEXT("FGsGameDataManager - Set exp penalty list - penaltyId : %lld"), penalty.mDeathPenaltyId);
#endif
	}

	_userData.expPenalty.Sort([](const DeathPenaltyExp& source, const DeathPenaltyExp& dest)->bool
		{
			return source.mExpireTime > dest.mExpireTime;
		});
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_DEATH_PENALTY_EXP_ADD_READ& inPacket)
{
	const DeathPenaltyExp& penalty = inPacket.DeathPenaltyExp();
	if (FGsTimeSyncUtil::IsExpried(penalty.mExpireTime))
	{
		return;
	}

	_userData.expPenalty.Add(penalty);
	_userData.expPenalty.Sort([](const DeathPenaltyExp& source, const DeathPenaltyExp& dest)->bool
	{
		return source.mExpireTime > dest.mExpireTime;
	});

#if PENALTY_DEBUG
	GSLOG(Log, TEXT("FGsGameDataManager - Add exp penalty - penaltyId : %lld"), inPacket.DeathPenaltyExp().mDeathPenaltyId);
#endif
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_DEATH_PENALTY_ITEM_LIST_READ& inPacket)
{
	_userData.itemPenalty.Empty();
	for (PD::SC::PKT_SC_DEATH_PENALTY_ITEM_LIST_READ::DeathPenaltyItemListIterator iter = inPacket.GetFirstDeathPenaltyItemListIterator();
		iter != inPacket.GetLastDeathPenaltyItemListIterator();
		++iter)
	{			
		const DeathPenaltyItem& penalty = iter->DeathPenaltyItem();
		//만약 패널티 자동 복구 기간이 현재 시간 보다 빠르거나 같으면 패널티 정보에 포함하지 않는다. 
		if (FGsTimeSyncUtil::IsExpried(penalty.mExpireTime))
		{
			continue;
		}

		_userData.itemPenalty.Add(penalty);

#if PENALTY_DEBUG
		GSLOG(Log, TEXT("FGsGameDataManager - Set item penalty list - penaltyId : %lld"), iter->DeathPenaltyItem().mDeathPenaltyId);
#endif
	}

	_userData.itemPenalty.Sort([](const DeathPenaltyItem& source, const DeathPenaltyItem& dest)->bool
		{
			return source.mExpireTime > dest.mExpireTime;
		});
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_DEATH_PENALTY_ITEM_ADD_READ& inPacket)
{		
	for (PD::SC::PKT_SC_DEATH_PENALTY_ITEM_ADD_READ::DeathPenaltyItemListIterator iter = inPacket.GetFirstDeathPenaltyItemListIterator();
		iter != inPacket.GetLastDeathPenaltyItemListIterator();
		++iter)
	{
		const DeathPenaltyItem& penalty = iter->DeathPenaltyItem();
		//만약 패널티 자동 복구 기간이 현재 시간 보다 빠르거나 같으면 패널티 정보에 포함하지 않는다. 		
		if (FGsTimeSyncUtil::IsExpried(penalty.mExpireTime))
		{
			continue;
		}

		_userData.itemPenalty.Add(penalty);

#if PENALTY_DEBUG
		GSLOG(Log, TEXT("FGsGameDataManager - Add item penalty - penaltyId : %lld"), iter->DeathPenaltyItem().mDeathPenaltyId);
#endif
	}	

	_userData.itemPenalty.Sort([](const DeathPenaltyItem& source, const DeathPenaltyItem& dest)->bool
		{
			return source.mExpireTime > dest.mExpireTime;
		});
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_DEATH_PENALTY_FREE_COUNT_READ& inPacket)
{
	_userData.freeExpRestoreCount = inPacket.ExpDeathPenaltyFreeCount();
	_userData.freeExpRestoreMaxCount = inPacket.MaxExpDeathPenaltyFreeCount();

	_userData.freeItemRestoreCount = inPacket.ItemDeathPenaltyFreeCount();
	_userData.freeItemRestoreMaxCount = inPacket.MaxItemDeathPenaltyFreeCount();
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_ACK_EXP_RECOVERY_COUNT_READ& inPacket)
{
	_userData.SetCurrency(CurrencyType::DEATH_EXP_RECOVERY, inPacket.ExpRecoveryCount());
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_USER_DEATH_READ& inPacket)
{	
	TArray<DeathPenaltyItem> itemPenaltyList;
	for (PD::SC::PKT_SC_USER_DEATH_READ::DeathPenaltyItemListIterator iter = inPacket.GetFirstDeathPenaltyItemListIterator();
		iter != inPacket.GetLastDeathPenaltyItemListIterator(); ++iter)
	{
		itemPenaltyList.Add(iter->DeathPenaltyItem());
	}

	_userData.attackerName = inPacket.AttackerName();
	_userData.attackerWorldId = inPacket.AttackerWorldId();
	_userData.attackerGuildName = inPacket.GuildName();		
	_userData.attackerGuildEmblemId = inPacket.GuildEmblemId();
	_userData.attackerNpcId = inPacket.CreatureId();
	_userData.recentPenaltyExp = inPacket.DeathPenaltyExp();
	_userData.recentPenaltyList = itemPenaltyList;			
	_userData.immediateResurrectionCost = inPacket.ResurrectionCost();
	_userData.immediateResurrectionCurrencyType = inPacket.CurrencyType();
	_userData.InstantResurrectionSecond = inPacket.InstantResurrectionSecond();	
	_userData.invasionTime = inPacket.InvasionOutSecond();

#if RESURRECT_DEBUG
	GSLOG(Log, TEXT("RESURRECT_DEBUG - FGsGameDataManager::Set - attackerName : %s"), *_userData.attackerName);
	GSLOG(Log, TEXT("RESURRECT_DEBUG - FGsGameDataManager::Set - attackerGuildName : %s"), *_userData.attackerGuildName);
	GSLOG(Log, TEXT("RESURRECT_DEBUG - FGsGameDataManager::Set - attackerNpcId : %d"), _userData.attackerNpcId);
	GSLOG(Log, TEXT("RESURRECT_DEBUG - FGsGameDataManager::Set - recentPenaltyExp - percent : %f"), _userData.recentPenaltyExp.GetExpPercentValue());
	GSLOG(Log, TEXT("RESURRECT_DEBUG - FGsGameDataManager::Set - recentPenaltyList - Num : %d"), _userData.recentPenaltyList.Num());
	GSLOG(Log, TEXT("RESURRECT_DEBUG - FGsGameDataManager::Set - immediateResurrectionCost : %d"), _userData.immediateResurrectionCost);
	GSLOG(Log, TEXT("RESURRECT_DEBUG - FGsGameDataManager::Set - InstantResurrectionSecond : %lld\ttime : %s"), _userData.InstantResurrectionSecond, *FGsTimeSyncUtil::ConvertToDateTime(_userData.InstantResurrectionSecond).ToString());
	GSLOG(Log, TEXT("RESURRECT_DEBUG - FGsGameDataManager::Set - current time : %s"), *FGsTimeSyncUtil::GetServerNowDateTime().ToString());
	GSLOG(Log, TEXT("RESURRECT_DEBUG - FGsGameDataManager::Set - invasion out second : %lld"), _userData.invasionTime);
#endif

	_userData.resurrectTypeList.Empty();
	int32 count = inPacket.GetResurrectTypeListCount();
	for (PD::SC::PKT_SC_USER_DEATH_READ::ResurrectTypeListIterator iter = inPacket.GetFirstResurrectTypeListIterator();
		iter != inPacket.GetLastResurrectTypeListIterator(); ++iter)
	{
		_userData.resurrectTypeList.Add(iter->ResurrectType());

#if !RESURRECT_DEBUG
		FString resurrectType = FGsTextUtil::GetStringFromEnum(TEXT("ResurrectType"), iter->ResurrectType());
		GSLOG(Log, TEXT("RESURRECT_DEBUG - FGsGameDataManager::Set - ResurrectType : %s"), *resurrectType);
#endif
	}
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_ARCAS_READ& Packet)
{
#ifdef R3_ARCAS_NOT_USE
	_userData._expBless.mBlessPoint = Packet.ArcasPoint();
	_userData._expBless.mIsOnArcasBless = Packet.ArcasFlag();
	_userData._expBless.mIsOnFairyBless = Packet.FairyFlag();
	_userData._expBless.mMeditationTime = Packet.RemainTime();

	_userData._expBless.mMeditaionPacketTime = FDateTime::UtcNow();

	GMessage()->GetUserBasicInfo().SendMessage(MessageUserInfo::BLESS_POINT, _userData._expBless.mBlessPoint);
#else
	_userData._blessPoint = Packet.ArcasPoint();

	GMessage()->GetUserBasicInfo().SendMessage(MessageUserInfo::BLESS_POINT, _userData._blessPoint);
#endif
}	

void FGsGameDataManager::Set(PD::SC::PKT_SC_UPDATE_ARCAS_POINT_READ& Packet)
{
#ifdef R3_ARCAS_NOT_USE
	_userData._expBless.mBlessPoint = Packet.ArcasPoint();

	GMessage()->GetUserBasicInfo().SendMessage(MessageUserInfo::BLESS_POINT, _userData._expBless.mBlessPoint);
#else
	_userData._blessPoint = Packet.ArcasPoint();

	GMessage()->GetUserBasicInfo().SendMessage(MessageUserInfo::BLESS_POINT, _userData._blessPoint);
#endif
}

#ifdef R3_ARCAS_NOT_USE
void FGsGameDataManager::Set(PD::SC::PKT_SC_UPDATE_ARCAS_MEDITATION_REMAINTIME_READ& Packet)
{
	_userData._expBless.mMeditationTime = Packet.RemainTime();
	_userData._expBless.mMeditaionPacketTime = FDateTime::UtcNow();

	GMessage()->GetUserBasicInfo().SendMessage(MessageUserInfo::BLESS_POINT, _userData._expBless.mBlessPoint);
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_UPDATE_ARCAS_BUFF_READ& Packet)
{
	_userData._expBless.mIsOnArcasBless = Packet.ArcasFlag();
	_userData._expBless.mIsOnFairyBless = Packet.FairyFlag();

	GMessage()->GetUserBasicInfo().SendMessage(MessageUserInfo::BLESS_POINT, _userData._expBless.mBlessPoint);
}
#endif

void FGsGameDataManager::Set(PD::SC::PKT_SC_UPDATE_PVP_ALIGNMENT_POINT_READ& Packet)
{
	_userData.pvpAlignmentPoint = Packet.PvpAlignmentPoint();
}

//길드 던전 정보 중 레드 닷, UI와 던전에서 공통으로 쓰는 데이터는 UserData에 저장한다. 
void FGsGameDataManager::Set(PD::GC::PKT_GC_ACK_OPEN_GUILD_DUNGEON_INFO_READ& inPacket)
{
	SetGuildDungeonLevel(inPacket.Level());
	
	_userData.dungeonResetTime = inPacket.ResetDateTime();
}

//길드 던전 정보 중 레드 닷, UI와 던전에서 공통으로 쓰는 데이터는 UserData에 저장한다. 
void FGsGameDataManager::Set(PD::GC::PKT_GC_INFORM_GUILD_DUNGEON_LEVEL_READ& inPacket)
{
	SetGuildDungeonLevel(inPacket.Level());
}

void FGsGameDataManager::SetLevel(uint16 Level)
{
	_userData.mLevel = Level;
}

void FGsGameDataManager::SetTerritoryType(TerritoryType TerritoryType)
{
	_territoryType = TerritoryType;
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_QUEST_READ& Packet)
{
	_userData.listQuestInfo.Empty();	

	for (PD::SC::PKT_SC_QUEST_READ::QuestUpdateDataIterator iter = Packet.GetFirstQuestUpdateDataIterator();
		iter != Packet.GetLastQuestUpdateDataIterator(); ++iter)
	{
		FGsNetUserData::FQuestInfo newInfo;
		newInfo.mStoryId = iter->StoryId();
		newInfo.mQuestId = iter->QuestId();
		newInfo.mQuestType = iter->QuestType();
		newInfo.mQuestState = iter->QuestState();
		newInfo.mQuestIndex = iter->QuestIndex();
		newInfo.mPreEventActionIndex = iter->PreEventActionIndex();
		newInfo.mPostEventActionIndex = iter->PostEventActionIndex();

#if WITH_EDITOR
		GSLOG(Log, TEXT("[QUEST] QUEST_INFO- ID: %llu, STATE: %d"), newInfo.mQuestId, static_cast<int>(newInfo.mQuestState));
#endif
		int32 objectiveNum = iter->GetQuestUpdateObjectiveDataCount();
		for (PD::SC::PKT_SC_QUEST_READ::QuestUpdateData::QuestUpdateObjectiveDataIterator ObjectiveIter = iter->GetFirstQuestUpdateObjectiveDataIterator();
			ObjectiveIter != iter->GetLastQuestUpdateObjectiveDataIterator(); ++ObjectiveIter)
		{
			int32 value = ObjectiveIter->Value();
			newInfo.mObjectiveValueList.Emplace(value);
		}

		_userData.listQuestInfo.Emplace(newInfo);
	}

	// 재연결때에는 퀘스트매니져가 있어서 Initialize가 안불린다.
	// 바로 셋 해준다. 
	UGsQuestManager* questManager = GSQuest();
	if (nullptr != questManager)
	{
		questManager->UpdateActiveQuest();
	}
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_ACK_QUEST_REWARD_READ& Packet)
{
	
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_ACK_QUEST_SUB_REWARD_READ& Packet)
{
	// 주의 : 이 패킷에 담겨오는 Exp는 연출에 필요할까봐 넣어진 값으로 처리하지 않는다.
	// PKT_SC_USER_EXP_UPDATE_READ 에서 처리된다.
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_ACK_QUEST_REPEAT_REWARD_READ& Packet)
{
	// 주의 : 이 패킷에 담겨오는 Exp는 연출에 필요할까봐 넣어진 값으로 처리하지 않는다.
	// PKT_SC_USER_EXP_UPDATE_READ 에서 처리된다.
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_CHANNEL_LIST_READ& Packet)
{
	// 쿨타임 세팅
	_checkerChannelChange.SetEndTime(Packet.CoolTime());

	// 채널 리스트 세팅
	_channelList.Empty();
	using ChannelIter = PktIterator<PD::SC::PKT_SC_CHANNEL_LIST_READ::ChannelList>;
	for (ChannelIter iter = Packet.GetFirstChannelListIterator(); iter != Packet.GetLastChannelListIterator(); ++iter)
	{
		_channelList.Emplace(FGsMapChannelData(iter->ChannelId(), iter->Status()));
	}
}
void FGsGameDataManager::Set(PD::SC::PKT_SC_BOOKMARK_READ& Packet)
{
	_userData.listBookmarkInfo.Empty();

	using bookmarkIter = PD::SC::PKT_SC_BOOKMARK_READ::BookmakrListIterator;
	for (bookmarkIter iter = Packet.GetFirstBookmakrListIterator();
		iter != Packet.GetLastBookmakrListIterator(); ++iter)
	{
		FGsNetUserData::FBookmarkInfo bookmarkInfo;
		bookmarkInfo.mid = iter->Id();
		bookmarkInfo.mName = iter->Title();
		bookmarkInfo.mPos = iter->ConstRefPos();
		bookmarkInfo.mMapId = iter->MapId();

		_userData.listBookmarkInfo.Emplace(bookmarkInfo);
	}
}
void FGsGameDataManager::Set(UserStatus Status)
{
	_userData.mUserStatus = Status;
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_STAT_POINT_READ& inPacket)
{
	_userData._statPoint = inPacket.StatPoint();
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_SPAWN_ME_READ& Packet)
{
	_isSpawnAlive = Packet.IsAlive();
	_spawnPos = Packet.ConstRefPos();
	_spawnRot = FGsDir::AngleToRotator(Packet.Dir());
	SetTerritoryType(Packet.TerritoryType());
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_FAIRY_READ& Packet)
{
	auto datumSize = sizeof(FairyData);
	const uchar* fairyRawData = Packet.FairyDataList();
	auto rawSize = Packet.FairyDataListSize();

	_fairyDataComplete = Packet.Completed();
	_fairyRawDataSize += rawSize;
	_fairyRawData.Append(fairyRawData, rawSize);
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_FAIRY_COLLECTION_READ& Packet)
{
	if (Packet.Refresh())
	{
		Packet.MakeCollectionIdListVector<TArray<CollectionId>, CollectionId>(_fairyCollectionComplete);
		Packet.MakeCollectionConditionIdListVector<TArray<CollectionConditionId>, CollectionConditionId>(_fairyCollectionCompleteCondition);
	}
}

//void FGsGameDataManager::Set(PD::SC::PKT_SC_ITEM_COLLECTION_READ& Packet)
//{
//	if (Packet.Refresh())
//	{
//		Packet.MakeCollectionIdListVector<TSet<CollectionId>, CollectionId>(_completedItemCollectionIds);
//		Packet.MakeCollectionConditionIdListVector<TSet<CollectionConditionId>, CollectionConditionId>(_completedItemCollectionConditionIds);
//	}
//}

void FGsGameDataManager::Set(PD::GC::PKT_GC_CHAT_BLACKLIST_READ& Packet)
{
	_blockList.Empty();

	auto iter = Packet.GetFirstChatblacklistsIterator();
	auto endIter = Packet.GetLastChatblacklistsIterator();
	for (; iter != endIter; ++iter)
	{	
		_blockList.Add(FGsChatBlockUser{ iter->UserDBId(), iter->UserName() });
	}
}

void FGsGameDataManager::Set(PD::GC::PKT_GC_ACK_CHAT_BLACKLIST_ADD_READ& Packet)
{
	_blockList.Add(FGsChatBlockUser{ Packet.UserDBId(), Packet.UserName() });
}

void FGsGameDataManager::Set(PD::GC::PKT_GC_ACK_CHAT_BLACKLIST_REMOVE_READ& Packet)
{
	auto dbId = Packet.UserDBId();

	_blockList.RemoveAll([dbId](FGsChatBlockUser& e) { return e._uid == dbId; });
}

// 접속 시 안읽은 메일이 있으면 서버에서 노티 내려준다. (상태 저장)
void FGsGameDataManager::Set(PD::SC::PKT_SC_NOTIFY_MAIL_UNREAD_READ& Packet)
{
	MailDBId lastMailDBId = Packet.LastMailDBId();
	_isUnReadMail = true;

	bool inNewTickerEffect = false;
	if (_mailOwner == INVALID_USER_DB_ID)
	{		
		inNewTickerEffect = true;
		_mailOwner = _userData.mUserDBId;
	}
	else
	{
		if (_mailOwner != _userData.mUserDBId)
		{
			inNewTickerEffect = true;
			_mailOwner = _userData.mUserDBId;
		}
		else
		{
			if (lastMailDBId != _isUnReadMailDBId)
			{
				inNewTickerEffect = true;
			}
		}
	}

	if (inNewTickerEffect)
	{
		_isUnReadMailDBId = lastMailDBId;
	}

	const FGsPrimitiveInt32 param(static_cast<int32>(inNewTickerEffect));
	GMessage()->GetMail().SendMessage(MessageContentMail::NOTIFY_MAIL_UNREAD, &param);
}

void FGsGameDataManager::Set(PD::GC::PKT_GC_NOTIFY_NEW_SYSTEM_MAIL_READ& Packet)
{
	_isNewSystemMail = true;
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_FAIRY_SUMMON_WAIT_READ& Packet)
{
	_fairySummonWaitings.Reset();
	_fairySummonWaitingPacketTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC();
	Packet.MakeFairyConfirmDataListVector<TArray<FairyConfirmData>, FairyConfirmData>(_fairySummonWaitings);
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_COSTUME_COLLECTION_READ& Packet)
{
	if (Packet.Refresh())
	{
		Packet.MakeCollectionIdListVector<TArray<CollectionId>, CollectionId>(_costumeCollectionComplete);
		Packet.MakeCollectionConditionIdListVector<TArray<CollectionConditionId>, CollectionConditionId>(_costumeCollectionCompleteCondition);
	}
}

void  FGsGameDataManager::Set(PD::SC::PKT_SC_COSTUME_SUMMON_WAIT_READ& Packet)
{
	_costumeSummonWaitings.Reset();
	_costumeSummonWaitingPacketTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC();
	Packet.MakeCostumeConfirmDataListVector<TArray<CostumeConfirmData>, CostumeConfirmData>(_costumeSummonWaitings);
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_CONTENTS_STATE_READ& Packet)
{
	for (PD::SC::PKT_SC_CONTENTS_STATE_READ::ContentsListIterator iter = Packet.GetFirstContentsListIterator();
		iter != Packet.GetLastContentsListIterator(); ++iter)
	{
		ContentsState state = iter->ContentsState();
		ContentsType type = iter->ContentsType();
		switch (type)
		{
		case ContentsType::ATTENDANCE:
			{
				if (state == ContentsState::RED_DOT)
					SetNewDatAttendance(true);
			}
			break;
		case ContentsType::CODEX:
		{
			if (state == ContentsState::RED_DOT)
				SetCodexRedDot(true);
			break;
		}
		case ContentsType::MAX:
			break;
		default:
			break;
		}
	}
}

void FGsGameDataManager::SetTId(uint32 In_TId)
{
	_userData.mTid = In_TId;
}

void FGsGameDataManager::ResetFairyData()
{
	_fairyDataComplete = false;
	_fairyRawData.Reset();
	_fairyRawDataSize = 0;
}

void FGsGameDataManager::ResetFairyCollectionData()
{
	_fairyCollectionComplete.Reset();
	_fairyCollectionCompleteCondition.Reset();
}

//void FGsGameDataManager::ResetItemCollectionData()
//{
//	_completedItemCollectionIds.Reset();
//	_completedItemCollectionConditionIds.Reset();
//}

void FGsGameDataManager::RemoveDeathPenaltyList(TArray<DeathPenaltyRestore>& inPenaltyList)
{	
	for (DeathPenaltyRestore& restore : inPenaltyList)
	{		
		RemoveDeathPenaly(restore.mDeathPenaltyId);	
	}

	_userData.expPenalty.Sort([](const DeathPenaltyExp& source, const DeathPenaltyExp& dest)->bool
		{
			return source.mExpireTime > dest.mExpireTime;
		});
	_userData.itemPenalty.Sort([](const DeathPenaltyItem& source, const DeathPenaltyItem& dest)->bool
		{
			return source.mExpireTime > dest.mExpireTime;
		});
}

void FGsGameDataManager::RemoveDeathPenaly(DeathPenaltyId inPenaltyId)
{		
	for (int32 i = 0; i < _userData.expPenalty.Num(); ++i)
	{
		if (_userData.expPenalty[i].mDeathPenaltyId == inPenaltyId)
		{			
			_userData.expPenalty.RemoveAt(i);

#if PENALTY_DEBUG
			GSLOG(Log, TEXT("remove exp penalty : %uld"), inPenaltyId);
#endif
			break;
		}
	}
		
	for (int32 i = 0; i < _userData.itemPenalty.Num(); ++i)
	{
		if (_userData.itemPenalty[i].mDeathPenaltyId == inPenaltyId)
		{			
			_userData.itemPenalty.RemoveAt(i);

#if PENALTY_DEBUG
			GSLOG(Log, TEXT("remove item penalty : %uld"), inPenaltyId);
#endif
			break;
		}
	}
}

Currency FGsGameDataManager::GetCurrency(CurrencyType InType) const
{
	return _userData.GetCurrency(InType);
}

void FGsGameDataManager::SetGuildData(PD::SC::PKT_SC_USER_GUILD_INFO_READ& Packet)
{
	_userData.mGuildData.Set(Packet);
}

void FGsGameDataManager::ClearGuildData()
{
	_userData.mGuildData.Empty();
}

void FGsGameDataManager::SetChangeGuildEmblem(GuildEmblemId InEmblemId)
{
	_userData.mGuildData._emblemId = InEmblemId;
}

float FGsGameDataManager::GetChangeChannelRemainTime() const
{
	return _checkerChannelChange.GetRemainTime();
}

bool FGsGameDataManager::CanChangeChannel() const
{
	return _checkerChannelChange.IsTimeOver();
}

bool FGsGameDataManager::SendRequestChannelList()
{
	// 쿨타임 지났는지 확인
	if (_checkerChannelList.IsTimeOver())
	{
		// 요청 패킷 전송
		FGsNetSendServiceWorld::SendRequestChannelList();
		
		// 쿨타임 갱신
		_checkerChannelList.UpdateTime();
		return true;
	}
	
	return false;
}


#ifdef R3_ARCAS_NOT_USE
void FGsGameDataManager::SetEndMeditation() 
{ 
	_userData.SetEndMeditation(); 
	GMessage()->GetUI().SendMessage(MessageUI::BLESS_ARCAS_POINT, nullptr); 
}
#endif

void FGsGameDataManager::Set(PD::SC::PKT_SC_COSTUME_READ& Packet)
{
	auto datumSize = sizeof(FairyData);
	const uchar* costumeRawData = Packet.CostumeDataList();
	auto rawSize = Packet.CostumeDataListSize();

	_costumeDataComplete = Packet.Completed();
	_costumeRawDataSize += rawSize;
	_costumeRawData.Append(costumeRawData, rawSize);
}

void FGsGameDataManager::ResetCostumeData()
{
	_costumeDataComplete = false;
	_costumeRawData.Reset();
	_costumeRawDataSize = 0;
}
void FGsGameDataManager::ResetCostumeCollectionData()
{
	_costumeCollectionComplete.Reset();
	_costumeCollectionCompleteCondition.Reset(); //collectionId 대신 costumeId를 쓸 것 같긴 하다 - 레벨이 없으므로
}

void FGsGameDataManager::Set(PD::GC::PKT_GC_ACK_BUDDY_LIST_READ& Packet)
{
	SetNewDatabuddy(true);
	_userData.mBuddyData.mBuddyMap.Empty();
	_userData.mBuddyData.mIsRedDot = Packet.IsRedDot();
	for (
		PD::GC::PKT_GC_ACK_BUDDY_LIST_READ::BuddyListIterator iterator = Packet.GetFirstBuddyListIterator();
		iterator != Packet.GetLastBuddyListIterator();
		++iterator
		)
	{
		UserDBId userDBId = iterator->UserDBId();
		time_t lastLoginTim = iterator->LastLoginTime();
		if (!_userData.mBuddyData.mBuddyMap.Contains(userDBId))
		{
			_userData.mBuddyData.mBuddyMap.Emplace(userDBId,
				MakeShareable(new FGsBuddyInfo(userDBId, iterator->UserName(), lastLoginTim)));
		}
	}
}

void FGsGameDataManager::Set(PD::GC::PKT_GC_ACK_BUDDY_RECV_INVITATION_LIST_READ& Packet)
{
	_userData.mBuddyData.mAcceptBuddyMap.Empty();
	_userData.mBuddyData.mIsAcceptRedDot = Packet.IsRedDot();
	for (
		PD::GC::PKT_GC_ACK_BUDDY_RECV_INVITATION_LIST_READ::RecvInvitationListIterator iterator = Packet.GetFirstRecvInvitationListIterator();
		iterator != Packet.GetLastRecvInvitationListIterator();
		++iterator
		)
	{
		UserDBId userDBId = iterator->UserDBId();
		if (!_userData.mBuddyData.mAcceptBuddyMap.Contains(userDBId))
		{
			_userData.mBuddyData.mAcceptBuddyMap.Emplace(userDBId,
				MakeShareable(new FGsBuddyInfo(userDBId, iterator->UserName())));
		}
	}
}

void FGsGameDataManager::Set(PD::GC::PKT_GC_ACK_BUDDY_SEND_INVITATION_LIST_READ& Packet)
{
	_userData.mBuddyData.mApplyBuddyMap.Empty();
	for (
		PD::GC::PKT_GC_ACK_BUDDY_SEND_INVITATION_LIST_READ::SendInvitationListIterator iterator = Packet.GetFirstSendInvitationListIterator();
		iterator != Packet.GetLastSendInvitationListIterator();
		++iterator
		)
	{
		UserDBId userDBId = iterator->UserDBId();
		if (!_userData.mBuddyData.mApplyBuddyMap.Contains(userDBId))
		{
			_userData.mBuddyData.mApplyBuddyMap.Emplace(userDBId,
				MakeShareable(new FGsBuddyInfo(userDBId, iterator->UserName())));
		}
	}
}

void FGsGameDataManager::Set(PD::GC::PKT_GC_ACK_BUDDY_LIST_V2_READ& Packet)
{
	SetNewDatabuddy(true);
	_userData.mBuddyData.mBuddyMap.Empty();
	_userData.mBuddyData.mIsRedDot = Packet.IsRedDot();
	for (
		PD::GC::PKT_GC_ACK_BUDDY_LIST_V2_READ::BuddyListIterator iterator = Packet.GetFirstBuddyListIterator();
		iterator != Packet.GetLastBuddyListIterator();
		++iterator
		)
	{
		UserDBId userDBId = iterator->UserDBId();
		time_t lastLoginTim = iterator->LastLoginTime();
		if (!_userData.mBuddyData.mBuddyMap.Contains(userDBId))
		{
			_userData.mBuddyData.mBuddyMap.Emplace(userDBId,
				MakeShareable(new FGsBuddyInfo(userDBId, iterator->UserName(), lastLoginTim, iterator->WorldId())));
		}
	}
}

void FGsGameDataManager::Set(PD::GC::PKT_GC_ACK_BUDDY_RECV_INVITATION_LIST_V2_READ& Packet)
{
	_userData.mBuddyData.mAcceptBuddyMap.Empty();
	_userData.mBuddyData.mIsAcceptRedDot = Packet.IsRedDot();
	for (
		PD::GC::PKT_GC_ACK_BUDDY_RECV_INVITATION_LIST_V2_READ::RecvInvitationListIterator iterator = Packet.GetFirstRecvInvitationListIterator();
		iterator != Packet.GetLastRecvInvitationListIterator();
		++iterator
		)
	{
		UserDBId userDBId = iterator->UserDBId();
		if (!_userData.mBuddyData.mAcceptBuddyMap.Contains(userDBId))
		{
			_userData.mBuddyData.mAcceptBuddyMap.Emplace(userDBId,
				MakeShareable(new FGsBuddyInfo(userDBId, iterator->UserName(), iterator->WorldId())));
		}
	}
}

void FGsGameDataManager::Set(PD::GC::PKT_GC_ACK_BUDDY_SEND_INVITATION_LIST_V2_READ& Packet)
{
	_userData.mBuddyData.mApplyBuddyMap.Empty();
	for (
		PD::GC::PKT_GC_ACK_BUDDY_SEND_INVITATION_LIST_V2_READ::SendInvitationListIterator iterator = Packet.GetFirstSendInvitationListIterator();
		iterator != Packet.GetLastSendInvitationListIterator();
		++iterator
		)
	{
		UserDBId userDBId = iterator->UserDBId();
		if (!_userData.mBuddyData.mApplyBuddyMap.Contains(userDBId))
		{
			_userData.mBuddyData.mApplyBuddyMap.Emplace(userDBId,
				MakeShareable(new FGsBuddyInfo(userDBId, iterator->UserName(), iterator->WorldId())));
		}
	}
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_GAME_OPTION_READ& Packet)
{
	_userData.mSpiritShotOnOff = Packet.GameOption().mSpiritShotOnOff;
}
// 천마석 ui on, off 동기화
// 처음 로비에서 게임 들어올때 받고
// 그다음엔 바뀔때마다 바뀐정보 저장하기
void FGsGameDataManager::SetSpiritShotOnOff(bool In_isOn)
{
	_userData.mSpiritShotOnOff = In_isOn;
}

// InterServer 침공 처리 : 현재 월드가 내 월드가 아닌 침공 상태인지 또는 침공 가능 상태의 침공 가능 맵인지 처리
void FGsGameDataManager::SetLoadWorldInfo(WorldId loadedWorldId, EnterWorldServerType worldServerType)
{
	_loadedWorldId = loadedWorldId;
	_loadedWorldType = worldServerType;
}

bool FGsGameDataManager::IsInvadeWorld() const
{
	return _loadedWorldId != _userData.mHomeWorldId && _loadedWorldType == EnterWorldServerType::INVADE_SERVER;
}

bool FGsGameDataManager::IsInInterWorld() const
{
	return _loadedWorldId != _userData.mHomeWorldId && _loadedWorldType == EnterWorldServerType::INTER_SERVER;
}

void FGsGameDataManager::SetInvasionDefenseState(bool inActive)
{
#if INVASION_TOWN_BATTLE_DEBUG
	if (inActive)
	{
		GSLOG(Log, TEXT("Invasion town - set invasion defense true"));
	}
	else
	{
		GSLOG(Log, TEXT("Invasion town - set invasion defense false"));
	}
#endif
	_isInvasionDefenseState = inActive;
}

int32 FGsGameDataManager::GetMarketSettlementCount(bool bIsWroldMarket) const
{
	return bIsWroldMarket ? _wdMarketSettlementCount : _marketSettlementCount;
}

void FGsGameDataManager::SetMarketSettlementCount(bool bIsWroldMarket, int32 InSettlementCount)
{
	if (bIsWroldMarket)
	{
		_wdMarketSettlementCount = InSettlementCount;
	}
	else
	{
		_marketSettlementCount = InSettlementCount;
	}
}

void FGsGameDataManager::Set(PD::SC::PKT_SC_USER_SPACE_CRACK_TICKET_READ& Packet)
{
	_userData._spaceCrack.mIsRewardCompleted = Packet.WeekRewardCompleted();
	_userData._spaceCrack.mSpaceCrackEntryCount = Packet.CurEntryCount();
	_userData._spaceCrack.mSpaceCrackMaxEntryCount = Packet.MaxEntryCount();
	_userData._spaceCrack.mEnableWeekReward = Packet.EnableWeekReward();
}

// 2022/02/03 PKT - Title(칭호)
void FGsGameDataManager::SetTitleData(const FGsTitlePlayerData::TitleSet& InFrontData, const FGsTitlePlayerData::TitleSet& InBackData)
{
	_userData._titleData.Set(InFrontData, InBackData);
}

void FGsGameDataManager::SetInvadeBattleLevel(InvadeBattleLevel InLevel)
{
	_userData._InvadeBattleLevel = InLevel;
}

void FGsGameDataManager::ResetGuildPlayerData()
{
	_userData.mGuildData.Empty();
}

void FGsGameDataManager::ResetGuildDungoen()
{
	_userData.isClearGuildDungeon = false; 
}

void FGsGameDataManager::RemoveBossBookmark(int32 inBookMark)
{
	_userData.bossBookMarkArray.Remove(inBookMark);
}

void FGsGameDataManager::AddBossBookmark(int32 inBookmark)
{
	_userData.bossBookMarkArray.Add(inBookmark);
	_userData.bossBookMarkArray.Sort([](const FieldBossId& inSource, const FieldBossId& inDest)->bool {
		return inSource < inDest;
		});
}

bool FGsGameDataManager::IsBossBookmark(int32 inBookmark)
{
	return _userData.bossBookMarkArray.Contains(inBookmark);
}

void FGsGameDataManager::SetCharacterCreatedTime(time_t InCreatedTime)
{
	_userData._createdTime = InCreatedTime;
}

float FGsGameDataManager::GetCurrentLevelExpRate()
{
	// 매번 값을 얻어오는 부하를 줄이기 위해 캐싱해둔다
	if (0 == _cachedCurrLevelMax ||
		0 == _cachedLevelForExp ||
		_cachedLevelForExp != _userData.mLevel)
	{
		_cachedLevelForExp = _userData.mLevel;

		// 이전 레벨 누적값
		_cachedPrevLevelAccum = FGsRewardHelper::GetLevelAccumMaxExp(_userData.mLevel - 1);
		// 현재 레벨 최대값
		_cachedCurrLevelMax = FGsRewardHelper::GetLevelMaxExp(_userData.mLevel);
	}

	// 주의: _userData->mExp는 누적값임에 유의
	float expRate = 0.f;
	if (0 < _cachedCurrLevelMax)
	{
		// 이전 레벨 누적값을 빼서 현재 레벨 값을 얻음
		Exp currLevelExp = _userData.mExp - _cachedPrevLevelAccum;
		if (0 < currLevelExp)
		{
			double tempRate = static_cast<double>(currLevelExp) / _cachedCurrLevelMax;
			expRate = static_cast<float>(tempRate);
		}
	}

	// 최대레벨 초과 상황 등에도 0~1값으로 유지
	expRate = FMath::Clamp(expRate, 0.f, 1.f);

	return expRate;
}

void FGsGameDataManager::SetGuildDungeonLevel(int32 inLevel)
{
	int32 minLevel, maxLevel;
	FGsGuildHelper::TryGetGuildDungeonMinMaxLevel(minLevel, maxLevel);

	bool isClearGuildDungeon = false;
	int32 level = inLevel;
	if (maxLevel < level)
	{
		level = maxLevel;
		isClearGuildDungeon = true;
	}

	_userData.guildDungeonLevel = level;
	_userData.isClearGuildDungeon = isClearGuildDungeon;
}

void FGsGameDataManager::SetPlayableEventCheckPoint(EGsPlayableEventServerCheckPointType In_checkPoint)
{
	_playableEventCheckPoint = In_checkPoint;
	// 체크 포인트 서버 저장 전송
	FGsNetSendService::SendReqPlayableEventUpdate(static_cast<int>(_playableEventCheckPoint));
}

void FGsGameDataManager::SetBonusStatExtendCount(int32 InTotalCount)
{
	_userData._bonusStatItemExtendCount = InTotalCount;
}

void FGsGameDataManager::SetUsedBonusStatItemCount(int32 InUsedItemCount)
{
	_userData._usedBonusStatItemCount = InUsedItemCount;
}

void FGsGameDataManager::SetUserPreset(PresetId InPresetId)
{
	_userData.mUserPresetId = InPresetId;
}

void FGsGameDataManager::SetDailyTradingCount(int32 InCount)
{
	_userData._dailyTradingCount = InCount;
}