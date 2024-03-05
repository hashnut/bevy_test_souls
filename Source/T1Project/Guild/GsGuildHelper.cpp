// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildHelper.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "DataSchema/Guild/GsSchemaGuildLevelDetailData.h"
#include "DataSchema/Guild/GsSchemaGuildBuilding.h"
#include "DataSchema/Guild/GsSchemaGuildBuildingLevelData.h"
#include "DataSchema/Guild/GsSchemaGuildQuestConfig.h"
#include "DataSchema/Guild/GsSchemaGuildStorageAcquireInfo.h"
#include "DataSchema/Guild/Agit/GsSchemaAgitConfig.h"
#include "DataSchema/Guild/Agit/GsSchemaAgitDungeonRaidConfig.h"
#include "DataSchema/GsSchemaResetTimeTicketConfig.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Item/GsSchemaItemEffectSpawnCamp.h"
#include "DataSchema/InterServer/World/GsSchemaWorldData.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "EffectText/GsEffectTextDefine.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "Option/GsGameUserSettings.h"
#include "UTIL/GsCronUtil.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsText.h"
#include "T1Project.h"
#include "DataSchema/Guild/GuildDungeon/GsSchemaGuildDungeon.h"


const FGsSchemaGuildConfig* FGsGuildHelper::GetGuildConfig()
{
	// 1개 열밖에 없음.
	const FGsSchemaGuildConfig* row = nullptr;
	if (const UGsTable* table = FGsSchemaGuildConfig::GetStaticTable())
	{
		if (table->FindRow(TEXT("0"), row))
		{
			return row;
		}
	}

#if WITH_EDITOR
	GSLOG(Error, TEXT("[Guild] Fail to get FGsSchemaGuildConfig."));
#endif
	return nullptr;
}

const FGsSchemaGuildEmblem* FGsGuildHelper::GetGuildEmblem(int32 InEmblemId)
{
	const UGsTableGuildEmblem* table = Cast<const UGsTableGuildEmblem>(FGsSchemaGuildEmblem::GetStaticTable());
	if (table)
	{
		const FGsSchemaGuildEmblem* row = nullptr;
		if (table->FindRowById(InEmblemId, row))
		{
			return row;
		}
	}

#if WITH_EDITOR
	GSLOG(Error, TEXT("[Guild] Fail to get FGsSchemaGuildEmblem. emblemId: %d"), InEmblemId);
#endif
	return nullptr;
}

const FGsSchemaGuildLevelDetailData* FGsGuildHelper::GetGuildLevelData(int32 InGuildLevel)
{
	const UGsTableGuildLevelDetailData* table = Cast<const UGsTableGuildLevelDetailData>(
		FGsSchemaGuildLevelDetailData::GetStaticTable());
	if (table)
	{
		const FGsSchemaGuildLevelDetailData* row = nullptr;
		if (table->FindRowById(InGuildLevel, row))
		{
			return row;
		}
	}

#if WITH_EDITOR	
	GSLOG(Error, TEXT("[Guild] Fail to get FGsSchemaGuildLevelDetailData. guildLevel: %d"), InGuildLevel);
#endif
	return nullptr;
}

const FGsSchemaGuildLevelDetailData* FGsGuildHelper::GetGuildMaxLevelData()
{
	if (const UGsTable* table = FGsSchemaGuildLevelDetailData::GetStaticTable())
	{
		TArray<const FGsSchemaGuildLevelDetailData*> rows;
		if (table->GetAllRows(rows))
		{
			if (0 < rows.Num())
			{
				return rows[rows.Num() - 1];
			}
		}
	}

	return nullptr;
}

const FGsSchemaGuildBuilding* FGsGuildHelper::GetGuildBuilding(int32 InBuildingId)
{
	const UGsTableGuildBuilding* table = Cast<const UGsTableGuildBuilding>(FGsSchemaGuildBuilding::GetStaticTable());
	if (table)
	{
		const FGsSchemaGuildBuilding* row = nullptr;
		if (table->FindRowById(InBuildingId, row))
		{
			return row;
		}
	}

#if WITH_EDITOR
	GSLOG(Error, TEXT("[Guild] Fail to get FGsSchemaGuildBuilding. buildingId: %d"), InBuildingId);
#endif
	return nullptr;
}

const FGsSchemaGuildBuildingLevelData* FGsGuildHelper::GetBuildingLevelData(const FGsSchemaGuildBuilding* InData, int32 InBuildingLevel)
{
	for (const FGsSchemaGuildBuildingLevelData& iter : InData->levelList)
	{
		if (InBuildingLevel == iter.level)
		{
			return &iter;
		}
	}

	// 다음 레벨이 있는지 체크 용도로도 쓰므로, 에러로그를 찍지 않음
	/*
#if WITH_EDITOR
	GSLOG(Error, TEXT("[Guild] Fail to get FGsSchemaGuildBuildingLevelData. buildingId: %d, level: %d"),
		InData->id, InBuildingLevel);
#endif
	*/

	return nullptr;
}

const FGsSchemaGuildBuildingLevelData* FGsGuildHelper::GetBuildingLevelData(int32 InBuildingId, int32 InBuildingLevel)
{
	if (const FGsSchemaGuildBuilding* building = GetGuildBuilding(InBuildingId))
	{
		for (const FGsSchemaGuildBuildingLevelData& iter : building->levelList)
		{
			if (InBuildingLevel == iter.level)
			{
				return &iter;
			}
		}
	}

	// 다음 레벨이 있는지 체크 용도로도 쓰므로, 에러로그를 찍지 않음
	/*
#if WITH_EDITOR
	GSLOG(Error, TEXT("[Guild] Fail to get FGsSchemaGuildBuildingLevelData. buildingId: %d, level:"),
		InBuildingId, InBuildingLevel);
#endif
	*/

	return nullptr;
}

const FGsSchemaGuildQuestConfig* FGsGuildHelper::GetQuestConfig()
{
	if (const UGsTable* table = FGsSchemaGuildQuestConfig::GetStaticTable())
	{
		TArray<const FGsSchemaGuildQuestConfig*> rowList;
		table->GetAllRows(rowList);

		if (0 < rowList.Num())
		{
			return rowList[0];
		}
	}

#if WITH_EDITOR
	GSLOG(Error, TEXT("[Guild] Fail to get FGsSchemaGuildQuestConfig."));
#endif
	return nullptr;
}

const FGsSchemaAgitConfig* FGsGuildHelper::GetAgitConfig()
{
	if (const UGsTable* table = FGsSchemaAgitConfig::GetStaticTable())
	{
		TArray<const FGsSchemaAgitConfig*> rowList;
		table->GetAllRows(rowList);

		if (0 < rowList.Num())
		{
			return rowList[0];
		}
	}

#if WITH_EDITOR
	GSLOG(Error, TEXT("[Agit] Fail to get FGsSchemaAgitConfig."));
#endif
	return nullptr;
}

const FGsSchemaAgitDungeonRaidConfig* FGsGuildHelper::GetAgitDungeonRaidConfig()
{
	if (const UGsTable* table = FGsSchemaAgitDungeonRaidConfig::GetStaticTable())
	{
		TArray<const FGsSchemaAgitDungeonRaidConfig*> rowList;
		table->GetAllRows(rowList);

		if (0 < rowList.Num())
		{
			return rowList[0];
		}
	}

#if WITH_EDITOR
	GSLOG(Error, TEXT("[Agit] Fail to get FGsSchemaAgitDungeonRaidConfig."));
#endif
	return nullptr;
}

bool FGsGuildHelper::GetMemberGradeText(GuildMemberGradeType InType, OUT FText& OutText)
{
	switch (InType)
	{
	case GuildMemberGradeType::CAPTAIN:
		// TEXT: 단장
		return FText::FindText(TEXT("GuildText"), TEXT("Member_Rank_Head"), OutText);
	case GuildMemberGradeType::SUBCAPTAIN:
		// TEXT: 부단장
		return FText::FindText(TEXT("GuildText"), TEXT("Member_Rank_viceHead"), OutText);
	case GuildMemberGradeType::FIRSTKNIGHT:
		// TEXT: 1급 기사
		return FText::FindText(TEXT("GuildText"), TEXT("Member_Rank_1stKnight"), OutText);
	case GuildMemberGradeType::SECONDKNIGHT:
		// TEXT: 2급 기사
		return FText::FindText(TEXT("GuildText"), TEXT("Member_Rank_2ndKnight"), OutText);
	case GuildMemberGradeType::KNIGHT:
		// TEXT: 일반 기사
		return FText::FindText(TEXT("GuildText"), TEXT("Member_Rank_generalKnight"), OutText);
	}

	return false;
}

void FGsGuildHelper::GetFormattedGuildName(const FString& InGuildName, WorldId InWorldId, bool bForcedShowPrefix, OUT FText& OutText)
{
	FString strOut = FGsTextUtil::CombineUserAndPrefix(FText::FromString(InGuildName), InWorldId, bForcedShowPrefix);
	OutText = FText::FromString(strOut);

	/*
	bool bShowPrefix = false;	
	if (bForcedShowPrefix)
	{
		bShowPrefix = true;
	}
	else
	{
		// 강제로 보여줘야 하는 경우가 아니면 나와 다른 월드일 때만 보여줌
		if (INVALID_WORLD_ID != InWorldId)
		{
			if (FGsGameDataManager* gameDataMgr = GGameData())
			{
				if (const FGsNetUserData* userData = gameDataMgr->GetUserData())
				{
					if (InWorldId != userData->mHomeWorldId)
					{
						bShowPrefix = true;
					}
				}
			}
		}
	}	

	if (bShowPrefix)
	{
		const FGsSchemaWorldData* worldData = UGsTableUtil::FindRowById<UGsTableWorldData, FGsSchemaWorldData>(InWorldId);
		if (worldData)
		{
			FString outStr = FString::Format(TEXT("{0}({1})"), { *InGuildName, *(worldData->worldPrefixText.ToString()) });
			OutText = FText::FromString(outStr);
		}
		else
		{
			OutText = FText::FromString(InGuildName);
		}
	}
	else
	{
		OutText = FText::FromString(InGuildName);
	}
	*/
}

bool FGsGuildHelper::CanEnterGuildContent(Level InLevel, bool bInMessage)
{
	if (const FGsSchemaGuildConfig* config = GetGuildConfig())
	{
		if (config->requestUserLevel <= InLevel)
		{
			return true;
		}

		if (bInMessage)
		{
			// TEXT: 길드 가입은 Lv.{0}부터 가능합니다.
			FText textFormat;
			if (FText::FindText(TEXT("GuildText"), TEXT("Alert_CannotJoinGuild_Level"), textFormat))
			{
				FText textMsg = FText::Format(textFormat, config->requestUserLevel);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}
		}
	}

	return false;
}

bool FGsGuildHelper::IsMasterAuthority(uint32 InAuthority)
{
	return ((InAuthority & GUILD_RIGHTS_LEVEL_MASTER) != 0) ? true : false;
}

bool FGsGuildHelper::CanManageMember(uint32 InAuthority)
{
	return ((InAuthority & GUILD_RIGHTS_LEVEL_MEMBER_MANAGEMENT) != 0) ? true : false;
}

bool FGsGuildHelper::CanManagePost(uint32 InAuthority)
{
	return ((InAuthority & GUILD_RIGHTS_LEVEL_POST_MANAGEMENT) != 0) ? true : false;
}

bool FGsGuildHelper::CanManageBuilding(uint32 InAuthority)
{
	return ((InAuthority & GUILD_RIGHTS_LEVEL_BUILDING_MANAGEMENT) != 0) ? true : false;
}

bool FGsGuildHelper::CanDiplomacy(uint32 InAuthority)
{
	return ((InAuthority & GUILD_RIGHTS_LEVEL_DIPLOMACY) != 0) ? true : false;
}

bool FGsGuildHelper::CanDistribute(uint32 InAuthority)
{
	return ((InAuthority & GUILD_RIGHTS_LEVEL_GUILD_STORE) != 0) ? true : false;
}

bool FGsGuildHelper::CanManageCamp(uint32 InAuthority)
{
	return ((InAuthority & GUILD_RIGHTS_LEVEL_GUILD_CAMP) != 0) ? true : false;
}

bool FGsGuildHelper::CanManageAcquisition(uint32 InAuthority, GuildMemberGradeType InGradeType)
{
	// 단장 || 부단장 || 기사단 분배 권한 보유자
	return InGradeType == GuildMemberGradeType::CAPTAIN || InGradeType == GuildMemberGradeType::SUBCAPTAIN || FGsGuildHelper::CanDivision(InAuthority);
}

bool FGsGuildHelper::CanDivision(uint32 InAuthority)
{
	return ((InAuthority & GUILD_RIGHTS_LEVEL_GUILD_STORE) != 0) ? true : false;
}

bool FGsGuildHelper::GetBuildingEffectString(const FGsSchemaGuildBuilding* InData, int32 InBuildingLevel, 
	OUT FText& OutText)
{
	if (nullptr == InData)
	{
		OutText = FText::GetEmpty();
		return false;
	}

	if (0 == InBuildingLevel)
	{
		OutText = InData->openText;
		return true;
	}

	const FGsSchemaGuildBuildingLevelData* levelData = GetBuildingLevelData(InData, InBuildingLevel);
	if (nullptr == levelData)
	{
		OutText = FText::GetEmpty();
		return false;
	}

	switch (InData->guildStatType)
	{
	case GuildStatType::GUILD_USER_NUMBER:
	{
		//예: 기사단 최대인원 25
		FString strEffect = FString::Format(TEXT("{0} {1}"), {
				InData->guildStatValueText.ToString(), levelData->guildStatValue });
		OutText = FText::FromString(strEffect);
		return true;
	}
	case GuildStatType::GUILD_BUFF:
	case GuildStatType::GUILD_PASSIVE_STAT:
	case GuildStatType::GUILD_DUNGEON_BUFF:
	{
		//예: 패시비티 스탯이나 효과 명시
		const FGsSchemaPassivitySet* passivitySet = levelData->passivityId.GetRow();
		if (passivitySet)
		{
			int32 effectTextOption = 0;
			FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::SKIP_INVOKE_CAUSE, effectTextOption);
			FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::SKIP_INVOKE_RATE, effectTextOption);
			FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::SKIP_INVOKE_TARGET, effectTextOption);

			FString strEffect;
			if (FGsEffectStringHelper::GetPassivityEffectString(passivitySet->id, strEffect, effectTextOption))
			{
				OutText = FText::FromString(strEffect);
				return true;
			}
		}
	}
	break;
	}

	return false;
}

bool FGsGuildHelper::GetBuildingEffectStringList(const struct FGsSchemaGuildBuilding* InData, int32 InBuildingLevel,
	OUT TArray<TPair<FText, FText>>& OutList)
{
	if (nullptr == InData)
	{
		return false;
	}

	if (0 == InBuildingLevel)
	{
		TPair<FText, FText> pair(InData->openText, FText::GetEmpty());
		OutList.Emplace(pair);

		return true;
	}

	const FGsSchemaGuildBuildingLevelData* levelData = GetBuildingLevelData(InData, InBuildingLevel);
	if (nullptr == levelData)
	{
		return false;
	}

	switch (InData->guildStatType)
	{
	case GuildStatType::GUILD_USER_NUMBER:
	{
		//예: 기사단 최대인원 25
		TPair<FText, FText> pair(InData->guildStatValueText, FText::AsNumber(levelData->guildStatValue));
		OutList.Emplace(pair);

		return true;
	}
	case GuildStatType::GUILD_BUFF:
	case GuildStatType::GUILD_PASSIVE_STAT:
	case GuildStatType::GUILD_DUNGEON_BUFF:
	{
		//예: 패시비티 스탯이나 효과 명시
		const FGsSchemaPassivitySet* passivitySet = levelData->passivityId.GetRow();
		if (passivitySet)
		{
			int32 effectTextOption = 0;
			FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::SKIP_INVOKE_CAUSE, effectTextOption);
			FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::SKIP_INVOKE_RATE, effectTextOption);
			FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::SKIP_INVOKE_TARGET, effectTextOption);

			return FGsEffectStringHelper::GetPassivityEffectString(passivitySet, OutList, effectTextOption);
		}
	}
	break;
	}

	return false;
}

void FGsGuildHelper::FindItemCollectionSubTabName(const FString& InKey, OUT FText& OutText)
{
	if (false == FText::FindText(TEXT("GuildText"), InKey, OutText))
	{
		OutText = FText::GetEmpty();
	}
}

bool FGsGuildHelper::GetGuildItemAcquireTypeText(GuildItemAcquireType InType, OUT FText& OutText)
{
	const UGsTableGuildStorageAcquireInfo* table = Cast<const UGsTableGuildStorageAcquireInfo>(FGsSchemaGuildStorageAcquireInfo::GetStaticTable());
	if (table)
	{
		const FGsSchemaGuildStorageAcquireInfo* row = nullptr;
		TArray<const FGsSchemaGuildStorageAcquireInfo*> allRow;
		if (false == table->GetAllRows<FGsSchemaGuildStorageAcquireInfo>(allRow))
			return false;

		for (uint8 i = 0; i < allRow.Num(); ++i)
		{
			if (nullptr == allRow[i])
				continue;

			if (InType == allRow[i]->itemAcquireType)
			{
				OutText = allRow[i]->text;
				return true;
			}
		}
	}
	return false;
}

bool FGsGuildHelper::IsGuildJoinedDay(const time_t& InJoinedTime)
{
	const UGsTableResetTimeTicketConfig* table = 
		Cast<const UGsTableResetTimeTicketConfig>(FGsSchemaResetTimeTicketConfig::GetStaticTable());
	if (nullptr == table)
	{
		return false;
	}

	const FGsSchemaResetTimeTicketConfig* row = nullptr;
	if (false == table->FindRowById(ResetTimeTicket::RTT_DAILY, row))
	{
		return false;
	}

	// UTC기준 리셋 시각
	FTimespan resetTimeUTC = FGsCronUtil::GetTimespanUTC(row->makeCron);
	if (FTimespan::MinValue() == resetTimeUTC)
	{
		return false;
	}
	
	FDateTime joinedDateTimeUTC = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(InJoinedTime);

	// 예: 리셋타임이 AM 5:00 이고, 12/3 AM 2:00 에 가입한 경우, 12/3 AM 5:00 가 리셋 시각
	FDateTime resetDateTimeUTC = joinedDateTimeUTC.GetDate() + resetTimeUTC;
	if (resetTimeUTC <= joinedDateTimeUTC.GetTimeOfDay())
	{
		// 예: 리셋타임이 AM 5:00 이고, 12/3 PM 2:00 에 가입한 경우, 12/4 AM 5:00 가 리셋 시각
		resetDateTimeUTC += FTimespan::FromDays(1);
	}

	// UTC기준 서버 현재 시각 
	FDateTime serverNowDateTimeUTC = FGsTimeSyncUtil::GetServerNowDateTimeUTC();

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[Guild] joinTimeUTC: %s, resetTimeUTC: %s, resetDateTimeUTC: %s, serverNowTimeUTC: %s"), 
		*joinedDateTimeUTC.ToString(),
		*resetTimeUTC.ToString(),
		*resetDateTimeUTC.ToString(),
		*serverNowDateTimeUTC.ToString());
#endif

	// 서버 현재시간이 리셋시간을 넘었는지 체크
	if (serverNowDateTimeUTC > resetDateTimeUTC)
	{
		return false;
	}

	return true;
}

bool FGsGuildHelper::IsExistQuest(QuestId InQuestId)
{
	if (UGsQuestManager* questMgr = GSQuest())
	{
		if (FGsQuestHandler* questHandler = questMgr->GetQuestHandler())
		{
			// 퀘스트 완료상황과 겹치며 리스트의 갯수변경이 일어나 
			// 크래시가 일어나는 것으로 추정되어 리스트를 주소로 받지 않도록 수정함
			const TArray<FGsQuestInstance*> questList = questHandler->GetQuesInstancetList();
			for (const FGsQuestInstance* questInstance : questList)
			{
				if (nullptr == questInstance)
				{
					continue;
				}

				if (questInstance->GetQuestType() == QuestType::GUILD)
				{
					if (questInstance->GetQuestId() == InQuestId)
					{
						return true;
					}
				}
			}
		}
	}	

	return false;
}

bool FGsGuildHelper::IsWaitRewardQuest()
{
	if (UGsQuestManager* questMgr = GSQuest())
	{
		if (FGsQuestHandler* questHandler = questMgr->GetQuestHandler())
		{
			// 퀘스트 완료상황과 겹치며 리스트의 갯수변경이 일어나 
			// 크래시가 일어나는 것으로 추정되어 리스트를 주소로 받지 않도록 수정함
			const TArray<FGsQuestInstance*> questList = questHandler->GetQuesInstancetList();
			for (const FGsQuestInstance* questInstance : questList)
			{
				if (nullptr == questInstance)
				{
					continue;
				}

				if (questInstance->GetQuestType() == QuestType::GUILD)
				{
					if (questInstance->GetQuestState() == QuestState::COMPLETED)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool FGsGuildHelper::CheckDiplomacyGuildName(const FString& InGuildName, bool bInShowTicker, bool bInMyServerOnly)
{
	if (InGuildName.IsEmpty())
	{
		return false;
	}

	if (false == GGuild()->IsGuild())
	{
		return false;
	}

	// 주의: 월드 검색 시엔 동일한 기사단명이 존재할 수 있으므로 이름 체크는 생략
	if (bInMyServerOnly)
	{
		const FGsGuildDataMine* myGuildData = GGuild()->GetMyGuildData();
		if (nullptr == myGuildData)
		{
			return false;
		}

		if (myGuildData->_guildName == InGuildName)
		{
			if (bInShowTicker)
			{
				// TEXT: 현재 가입되어 있는 기사단입니다.
				FText textMsg;
				if (FText::FindText(TEXT("GuildText"), TEXT("Alert_Diplomacy_MyGuild"), textMsg))
				{
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
				}
			}

			return false;
		}

		if (const FGsGuildDiplomacyDataEnemy* data = myGuildData->GetEnemyDataByName(InGuildName))
		{
			if (bInShowTicker)
			{
				// TEXT: 적대 중인 기사단입니다.
				FText textMsg;
				if (FText::FindText(TEXT("GuildText"), TEXT("Alert_Diplomacy_Already_Enemy"), textMsg))
				{
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
				}
			}

			return false;
		}

		if (const FGsGuildDiplomacyDataAlly* data = myGuildData->GetAllyDataByName(InGuildName))
		{
			if (bInShowTicker)
			{
				// TEXT: 동맹 중인 기사단입니다.
				FText textMsg;
				if (FText::FindText(TEXT("GuildText"), TEXT("Alert_Diplomacy_Already_Ally"), textMsg))
				{
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
				}
			}

			return false;
		}

		if (const FGsGuildDiplomacyDataState* data = myGuildData->GetStateDataByName(InGuildName))
		{
			if (bInShowTicker)
			{
				// TEXT: 동맹 대기 중인 기사단 입니다.
				FText textMsg;
				if (FText::FindText(TEXT("GuildText"), TEXT("Alert_Diplomacy_Already_Ally_Request"), textMsg))
				{
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
				}
			}

			return false;
		}
	}

	return true;
}

bool FGsGuildHelper::CheckDiplomacyGuildDBId(const GuildDBId InGuildDBId, bool bInShowTicker)
{
	if (INVALID_GUILD_DB_ID == InGuildDBId)
	{
		return false;
	}

	if (false == GGuild()->IsGuild())
	{
		return false;
	}

	const FGsGuildDataMine* myGuildData = GGuild()->GetMyGuildData();
	if (nullptr == myGuildData)
	{
		return false;
	}

	if (myGuildData->_guildDBId == InGuildDBId)
	{
		if (bInShowTicker)
		{
			// TEXT: 현재 가입되어 있는 기사단입니다.
			FText textMsg;
			if (FText::FindText(TEXT("GuildText"), TEXT("Alert_Diplomacy_MyGuild"), textMsg))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}
		}

		return false;
	}

	if (myGuildData->IsEnemy(InGuildDBId))
	{
		if (bInShowTicker)
		{
			// TEXT: 적대 중인 기사단입니다.
			FText textMsg;
			if (FText::FindText(TEXT("GuildText"), TEXT("Alert_Diplomacy_Already_Enemy"), textMsg))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}
		}

		return false;
	}

	if (myGuildData->IsAlly(InGuildDBId))
	{
		if (bInShowTicker)
		{
			// TEXT: 동맹 중인 기사단입니다.
			FText textMsg;
			if (FText::FindText(TEXT("GuildText"), TEXT("Alert_Diplomacy_Already_Ally"), textMsg))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}
		}

		return false;
	}

	if (myGuildData->IsAllyState(InGuildDBId))
	{
		if (bInShowTicker)
		{
			// TEXT: 동맹 대기 중인 기사단 입니다.
			FText textMsg;
			if (FText::FindText(TEXT("GuildText"), TEXT("Alert_Diplomacy_Already_Ally_Request"), textMsg))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}
		}

		return false;
	}

	return true;
}

ItemId FGsGuildHelper::GetGuildCoinItemId()
{
	if (const FGsSchemaItemCommon* itemTable = GetGuildCoinItemTable())
	{
		return itemTable->id;
	}

	return INVALID_ITEM_ID;
}

const FGsSchemaItemCommon* FGsGuildHelper::GetGuildCoinItemTable()
{
	if (const FGsSchemaGuildConfig* config = GetGuildConfig())
	{
		return config->guildCoinId.GetRow();
	}

	return nullptr;
}

int32 FGsGuildHelper::GetConstructableCampLevel(CampId InCampId)
{
	if (const UGsTable* table = FGsSchemaItemEffectSpawnCamp::GetStaticTable())
	{
		TArray<const FGsSchemaItemEffectSpawnCamp*> effectList;

		table->GetAllRows(effectList);

		for (const FGsSchemaItemEffectSpawnCamp* iter : effectList)
		{
			if (iter->campId == InCampId)
			{
				return iter->campLevel;
			}
		}
	}

	return -1;
}

bool FGsGuildHelper::TryGetGuildDungeonMinMaxLevel(OUT int32& outMin, OUT int32& outMax)
{
	outMin = 0;
	outMax = 0;
	const UGsTable* table = FGsSchemaGuildDungeon::GetStaticTable();
	if (nullptr == table)
	{
		return false;
	}

	TArray<const FGsSchemaGuildDungeon*> guildDungeonArray;
	table->GetAllRows<const FGsSchemaGuildDungeon>(guildDungeonArray);
	guildDungeonArray.Sort([](const FGsSchemaGuildDungeon& source, const FGsSchemaGuildDungeon& dest)
		{
			return source.level < dest.level;
		});

	if (guildDungeonArray.Num() > 0)
	{
		outMin = guildDungeonArray[0]->level;
		outMax = guildDungeonArray.Last()->level;
		return true;
	}

	return false;
}

const struct FGsSchemaGuildDungeon* FGsGuildHelper::GetGuildDungeonData(int32 inLevel)
{
	const UGsTable* table = FGsSchemaGuildDungeon::GetStaticTable();
	const UGsTableGuildDungeon* castTable = Cast<UGsTableGuildDungeon>(table);
	if (nullptr == castTable)
	{
		return nullptr;
	}

	const struct FGsSchemaGuildDungeon* find = nullptr;
	if (false == castTable->FindRowById(inLevel, find))
	{		
		return nullptr;
	}

	return find;
}

bool FGsGuildHelper::CanOpenGuildMainOther()
{
	FGsUnlockManager* unlockManager = GSUnlock();
	if (nullptr == unlockManager)
	{
		return false;
	}

	TArray<EGsUnlockContentsType> contentList;
	contentList.Emplace(EGsUnlockContentsType::GUILD_MAIN_TAB); // 메인
	contentList.Emplace(EGsUnlockContentsType::GUILD_BUILDING_TAB); // 운영

	for (EGsUnlockContentsType contentType : contentList)
	{
		if (unlockManager->IsContentsUnlock(contentType, false))
		{
			return true;
		}
	}

	return false;
}

bool FGsGuildHelper::IsInAgit()
{
	if (UGsLevelManager* levelMgr = GLevel())
	{
		return IsAgitMap(levelMgr->GetCurrentLevelId());
	}

	return false;
}

bool FGsGuildHelper::IsAgitMap(MapId InMapId)
{
	if (const FGsSchemaAgitConfig* agitConfig = FGsGuildHelper::GetAgitConfig())
	{
		if (const FGsSchemaMapData* agitMapId = agitConfig->mapId.GetRow())
		{
			if (agitMapId->id == InMapId)
			{
				return true;
			}
		}
	}

	return false;
}
