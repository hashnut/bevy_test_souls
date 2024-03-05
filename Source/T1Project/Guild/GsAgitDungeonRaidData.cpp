// Fill out your copyright notice in the Description page of Project Settings.


#include "GsAgitDungeonRaidData.h"
#include "DataSchema/Guild/Agit/GsSchemaAgitDungeonRaid.h"
#include "DataSchema/Guild/Agit/GsSchemaAgitDungeonRaidConfig.h"
#include "Guild/GsGuildHelper.h"
#include "Guild/GsGuildContributionData.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Net/GsNetSendServiceGuild.h"

const float AGIT_DUNGEON_DETAIL_PKT_INTERVAL = 5.f;

FGsAgitDungeonRaidBossData::~FGsAgitDungeonRaidBossData()
{
	Clear();
}

void FGsAgitDungeonRaidBossData::Clear()
{
	_raidId = 0;
	_table = nullptr;

	Reset();
}

void FGsAgitDungeonRaidBossData::Reset()
{
	_raidState = AgitDungeonRaidState::NONE;
	_bossHpPercent = 0;
	_completeTime = 0;
	_remainTime = 0;
	_pktReceiveTime = 0;

	_rankingList.Empty();
	_playerList.Empty();
}

void FGsAgitDungeonRaidBossData::SetTableData(const FGsSchemaAgitDungeonRaid* InTable)
{
	_raidId = InTable->id;
	_table = InTable;
}

void FGsAgitDungeonRaidBossData::SetDetailData(PD::GC::PKT_GC_ACK_AGIT_DUNGEON_RAID_DETAIL_READ* InPacket)
{
	_bossHpPercent = InPacket->BossHpRate();
	_completeTime = InPacket->ClearTime();
	_remainTime = InPacket->RemainTime();

	_pktReceiveTime = FGsTimeSyncUtil::GetServerNowSecond();

	// 랭킹
	_rankingList.Empty();
	for (PD::GC::PKT_GC_ACK_AGIT_DUNGEON_RAID_DETAIL_READ::RankingUserListIterator iter = InPacket->GetFirstRankingUserListIterator();
		iter != InPacket->GetLastRankingUserListIterator(); ++iter)
	{
		TSharedPtr<FGsAgitRaidRankingData> rankDataPtr = MakeShareable(new FGsAgitRaidRankingData());
		rankDataPtr->_strName = iter->UserName();
		rankDataPtr->_point = iter->ContributePoint();

		_rankingList.Emplace(rankDataPtr);
	}

	// 랭킹소팅
	_rankingList.Sort([](const TSharedPtr<FGsAgitRaidRankingData> InA, const TSharedPtr<FGsAgitRaidRankingData> InB)
		{
			return (InA->_point >= InB->_point) ? true : false;
		});

	// 랭킹 입력
	for (int32 i = 0; i < _rankingList.Num(); ++i)
	{
		_rankingList[i]->_rank = i + 1;
	}

	// 참여 유저
	_playerList.Empty();
	for (PD::GC::PKT_GC_ACK_AGIT_DUNGEON_RAID_DETAIL_READ::EnterUserListIterator iter = InPacket->GetFirstEnterUserListIterator();
		iter != InPacket->GetLastEnterUserListIterator(); ++iter)
	{
		TSharedPtr<FGsAgitRaidPlayerData> playerDataPtr = MakeShareable(new FGsAgitRaidPlayerData());
		playerDataPtr->_strName = iter->UserName();
		playerDataPtr->_level = iter->Level();

		_playerList.Emplace(playerDataPtr);
	}
}

float FGsAgitDungeonRaidBossData::GetBossHpPercent() const
{
	return _bossHpPercent;
}

time_t FGsAgitDungeonRaidBossData::GetCloseTime() const
{
	time_t closeTime = _pktReceiveTime + _remainTime;
	return (0 < closeTime) ? closeTime : 0;
}

int32 FGsAgitDungeonRaidBossData::GetLevelLimit() const
{
	if (_table)
	{
		return _table->requiredGuildLevel;
	}

	return 0;
}

FText FGsAgitDungeonRaidBossData::GetBossName() const
{
	if (_table)
	{
		if (const FGsSchemaNpcData* npcData = _table->bossId.GetRow())
		{
			return npcData->nameText;
		}
	}

	return FText::GetEmpty();
}

bool FGsAgitDungeonRaidBossData::IsReEnter() const
{
	if (AgitDungeonRaidState::OPEN == _raidState)
	{
		FString strMyName;
		if (const FGsNetUserData* userData = GGameData()->GetUserData())
		{
			strMyName = userData->mUserName;
		}

		for (TSharedPtr<FGsAgitRaidRankingData> iter : _rankingList)
		{
			if (strMyName == iter->_strName)
			{
				return true;
			}
		}
	}

	return false;
}

bool FGsAgitDungeonRaidBossData::SendReqAgitDungeonRaidDetail(bool bIsForced)
{
	if (AgitDungeonRaidState::NONE != _raidState)
	{
		if (bIsForced)
		{
			FGsNetSendServiceGuild::SendReqAgitDungeonRaidDetail(GetRaidId());
			return true;
		}
		else
		{
			time_t interval = FGsTimeSyncUtil::GetServerNowSecond() - _pktReceiveTime;
			if (AGIT_DUNGEON_DETAIL_PKT_INTERVAL <= interval)
			{
				FGsNetSendServiceGuild::SendReqAgitDungeonRaidDetail(GetRaidId());
				return true;
			}
		}
	}

	return false;
}

//----------------------------------------------------------------------------------------------------------------------
FGsAgitDungeonRaidData::~FGsAgitDungeonRaidData()
{
	Clear();
}

void FGsAgitDungeonRaidData::Initialize()
{
	// 테이블 파싱하여 FGsAgitDungeonRaidBossData 미리생성해둔다
	TArray<const FGsSchemaAgitDungeonRaid*> rowList;
	if (const UGsTable* table = FGsSchemaAgitDungeonRaid::GetStaticTable())
	{
		table->GetAllRows(rowList);
	}

	_bossDataMap.Empty();
	_bossDataList.Empty();
	for (const FGsSchemaAgitDungeonRaid* row : rowList)
	{
		TSharedPtr<FGsAgitDungeonRaidBossData> bossData = MakeShared<FGsAgitDungeonRaidBossData>();
		bossData->SetTableData(row);

		_bossDataList.Emplace(bossData);
		_bossDataMap.Emplace(row->id, bossData); // 편의를 위한 저장
	}
}

void FGsAgitDungeonRaidData::Clear()
{
	_raidClearCount = 0;
	_ticketCount = 0;
	_resetTime = 0;

	_bossDataMap.Empty();
	_bossDataList.Empty();
	_bookmarkList.Empty();

	ClearDungeonData();
}

void FGsAgitDungeonRaidData::ClearDungeonData()
{
	_dungeonRaidId = INVALID_AGIT_DUNGEON_RAID_ID;
	_clearSecond = 0;
	_dungeonRankingList.Empty();
}

void FGsAgitDungeonRaidData::Reset()
{
	_raidClearCount = 0; // 진행한 레이드 횟수
	_ticketCount = 0; // 유저 남은 입장 횟수
	_resetTime = 0;

	for (TSharedPtr<FGsAgitDungeonRaidBossData> iter : _bossDataList)
	{
		iter->Reset();
	}

	ClearDungeonData();
}

void FGsAgitDungeonRaidData::SetData(PD::GC::PKT_GC_ACK_AGIT_DUNGEON_RAID_READ* InPacket)
{
	_raidClearCount = InPacket->RaidClearCount();
	_ticketCount = InPacket->TicketCount();
	_resetTime = InPacket->ResetDateTime();

	for (PD::GC::PKT_GC_ACK_AGIT_DUNGEON_RAID_READ::RaidListIterator iter = InPacket->GetFirstRaidListIterator();
		iter != InPacket->GetLastRaidListIterator(); ++iter)
	{
		TWeakPtr<FGsAgitDungeonRaidBossData> bossData = GetBossDataById(iter->RaidId());
		if (bossData.IsValid())
		{
			bossData.Pin()->SetRaidState(iter->State());
		}
	}
}

void FGsAgitDungeonRaidData::SetDetailData(PD::GC::PKT_GC_ACK_AGIT_DUNGEON_RAID_DETAIL_READ* InPacket)
{
	TWeakPtr<FGsAgitDungeonRaidBossData> bossData = GetBossDataById(InPacket->RaidId());
	if (bossData.IsValid())
	{
		bossData.Pin()->SetDetailData(InPacket);
	}
}

void FGsAgitDungeonRaidData::SetRaidState(AgitDungeonRaidId InRaidId, AgitDungeonRaidState InState)
{
	TWeakPtr<FGsAgitDungeonRaidBossData> bossData = GetBossDataById(InRaidId);
	if (bossData.IsValid())
	{
		bossData.Pin()->SetRaidState(InState);
	}
}

void FGsAgitDungeonRaidData::SetBookmarkList(PD::SC::PKT_SC_ACK_AGIT_DUNGEON_RAID_BOOKMARK_LIST_READ* InPacket)
{
	_bookmarkList.Empty();

	for (PD::SC::PKT_SC_ACK_AGIT_DUNGEON_RAID_BOOKMARK_LIST_READ::RaidIdListIterator iter = InPacket->GetFirstRaidIdListIterator();
		iter != InPacket->GetLastRaidIdListIterator(); ++iter)
	{
		_bookmarkList.Emplace(iter->RaidId());
	}
}

void FGsAgitDungeonRaidData::AddBookmark(AgitDungeonRaidId InRaidId)
{
	if (false == _bookmarkList.Contains(InRaidId))
	{
		_bookmarkList.Emplace(InRaidId);
	}
}

void FGsAgitDungeonRaidData::RemoveBookmark(AgitDungeonRaidId InRaidId)
{
	if (_bookmarkList.Contains(InRaidId))
	{
		_bookmarkList.Remove(InRaidId);
	}
}

void FGsAgitDungeonRaidData::SetDungeonRaidRank(PD::SC::PKT_SC_INFORM_AGIT_DUNGEON_RAID_RANK_READ* InPacket)
{
	_dungeonRankingList.Empty();

	for (PD::SC::PKT_SC_INFORM_AGIT_DUNGEON_RAID_RANK_READ::RankingListIterator iter = InPacket->GetFirstRankingListIterator();
		iter != InPacket->GetLastRankingListIterator(); ++iter)
	{
		_dungeonRankingList.Emplace(MakeShareable(new FGsContributionData(0, iter->ContributeRate(), iter->UserName())));
	}
}

int32 FGsAgitDungeonRaidData::GetMaxRaidCount() const
{
	if (const FGsSchemaAgitDungeonRaidConfig* config = FGsGuildHelper::GetAgitDungeonRaidConfig())
	{
		return config->raidMaxClearCount;
	}

	return 0; 
}

int32 FGsAgitDungeonRaidData::GetRemainRaidCount() const
{ 
	int32 remainCount = GetMaxRaidCount() - _raidClearCount;
	if (0 < remainCount)
	{
		return remainCount;
	}

	return 0;
}

void FGsAgitDungeonRaidData::GetSortedBossList(OUT TArray<TWeakPtr<FGsAgitDungeonRaidBossData>>& OutList)
{
	_bossDataList.Sort([this](TSharedPtr<FGsAgitDungeonRaidBossData> InA, TSharedPtr<FGsAgitDungeonRaidBossData> InB)
		{
			// 1순위: 현재 공략 중 것. 활성화된 상태는 하나만 존재함
			if (AgitDungeonRaidState::OPEN == InA->GetRaidState())
			{
				return true;
			}
			else if (AgitDungeonRaidState::OPEN == InB->GetRaidState())
			{
				return false;
			}
			else
			{
				// 2순위: 즐겨찾기
				bool bIsBookMarkA = _bookmarkList.Contains(InA->GetRaidId());
				bool bIsBookMarkB = _bookmarkList.Contains(InB->GetRaidId());

				if (bIsBookMarkA && 
					false == bIsBookMarkB)
				{
					return true;
				}
				else if (false == bIsBookMarkA &&
					bIsBookMarkB)
				{
					return false;
				}
				else
				{
					// 3순위 RaidId 오름차순
					if (InA->GetRaidId() <= InB->GetRaidId())
					{
						return true;
					}
				}
			}

			return false;
		});

	OutList.Append(_bossDataList);
}

TWeakPtr<FGsAgitDungeonRaidBossData> FGsAgitDungeonRaidData::GetBossDataById(AgitDungeonRaidId InRaidId)
{
	if (TWeakPtr<FGsAgitDungeonRaidBossData>* ptr = _bossDataMap.Find(InRaidId))
	{
		if (ptr->IsValid())
		{
			return *ptr;
		}
	}

	return nullptr;
}

TWeakPtr<FGsAgitDungeonRaidBossData> FGsAgitDungeonRaidData::GetCurrOpenBossData()
{
	for (TSharedPtr<FGsAgitDungeonRaidBossData> iter : _bossDataList)
	{
		if (AgitDungeonRaidState::OPEN == iter->GetRaidState())
		{
			return iter;
		}
	}

	return nullptr;
}

bool FGsAgitDungeonRaidData::IsBookmarked(AgitDungeonRaidId InRaidId) const
{
	return _bookmarkList.Contains(InRaidId);
}

time_t FGsAgitDungeonRaidData::GetDungeonCloseTime()
{
	TWeakPtr<FGsAgitDungeonRaidBossData> bossData = GetBossDataById(_dungeonRaidId);
	if (bossData.IsValid())
	{
		return bossData.Pin()->GetCloseTime();
	}

	return 0;
}

void FGsAgitDungeonRaidData::GetSortedRankingList(OUT TArray<TWeakPtr<FGsContributionData>>& OutList)
{
	// 리스트 소팅
	_dungeonRankingList.Sort([](const TSharedPtr<FGsContributionData> InA, const TSharedPtr<FGsContributionData> InB)
		{
			return (InA->_contribution >= InB->_contribution) ? true : false;
		});

	// 랭킹입력
	for (int32 i = 0; i < _dungeonRankingList.Num(); ++i)
	{
		_dungeonRankingList[i]->_rank = i + 1;
	}

	OutList.Append(_dungeonRankingList);
}
