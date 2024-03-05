// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"

namespace PD
{
	namespace GC
	{
		struct PKT_GC_ACK_AGIT_DUNGEON_RAID_READ;
		struct PKT_GC_ACK_AGIT_DUNGEON_RAID_DETAIL_READ;
	}

	namespace SC
	{
		struct PKT_SC_ACK_AGIT_DUNGEON_RAID_BOOKMARK_LIST_READ;
		struct PKT_SC_INFORM_AGIT_DUNGEON_RAID_RANK_READ;
	}
}

struct FGsContributionData;
struct FGsSchemaAgitDungeonRaid;

//----------------------------------------------------------------------------------------------------------------------
/**
 * 길드 영지 던전 - 공략던전 순위
 */
struct FGsAgitRaidRankingData final
{
	FString _strName;
	ContributePoint _point = 0;
	int32 _rank = 0;

public:
	FGsAgitRaidRankingData() = default;
	~FGsAgitRaidRankingData() {}
};

//----------------------------------------------------------------------------------------------------------------------
/**
 * 길드 영지 던전 - 공략던전 참여자 정보
 */
struct FGsAgitRaidPlayerData final
{
	Level _level = 0;
	FString _strName;

public:
	FGsAgitRaidPlayerData() = default;
	~FGsAgitRaidPlayerData() {}
};

//----------------------------------------------------------------------------------------------------------------------
/**
 * 길드 영지 던전 - 공략던전 보스 정보
 */
class FGsAgitDungeonRaidBossData final
{
private:
	AgitDungeonRaidId _raidId = 0;
	const FGsSchemaAgitDungeonRaid* _table = nullptr;

private:
	AgitDungeonRaidState _raidState = AgitDungeonRaidState::NONE;
	float _bossHpPercent = 0; // 서버에서 퍼센트값으로 준다(0~100)
	time_t _completeTime = 0; // 완료 시간(해당 dateTime)
	time_t _remainTime = 0; // 남은 시간(초)
	time_t _pktReceiveTime = 0; // 패킷 받은 시간. 남은 시간 연산을 위해 저장

public:
	// 기여도 순위
	TArray<TSharedPtr<FGsAgitRaidRankingData>> _rankingList;
	// 현재 입장 인원
	TArray<TSharedPtr<FGsAgitRaidPlayerData>> _playerList;

public:
	FGsAgitDungeonRaidBossData() = default;
	~FGsAgitDungeonRaidBossData();

public:
	void Clear(); // 삭제 시
	void Reset(); // 컨텐츠 리셋 시
	void SetTableData(const FGsSchemaAgitDungeonRaid* InTable);

public:
	void SetRaidState(AgitDungeonRaidState InState) { _raidState = InState; }
	void SetDetailData(PD::GC::PKT_GC_ACK_AGIT_DUNGEON_RAID_DETAIL_READ* InPacket);

public:
	AgitDungeonRaidId GetRaidId() const { return _raidId; }
	const FGsSchemaAgitDungeonRaid* GetTableData() const { return _table; }

	AgitDungeonRaidState GetRaidState() const { return _raidState; }
	
	float GetBossHpPercent() const;
	time_t GetCompleteTime() const { return _completeTime; }
	time_t GetCloseTime() const;
	int32 GetLevelLimit() const;

	FText GetBossName() const;

	// 재입장 체크. 기여도 목록에 내가 있는지와 State 확인
	bool IsReEnter() const;

public:
	// 패킷량을 줄이기 위해 쿨타임 체크하며 요청. bIsForced 이면 패킷 받은 시간 상관 없이 무조건 요청
	bool SendReqAgitDungeonRaidDetail(bool bIsForced);
};

//----------------------------------------------------------------------------------------------------------------------
/**
 * 길드 영지 던전 - 공략던전 정보
 */
class FGsAgitDungeonRaidData final
{
public:
	int32 _raidClearCount = 0; // 진행한 레이드 횟수
	int32 _ticketCount = 0; // 유저 남은 입장 횟수
	time_t _resetTime = 0;
	TArray<TSharedPtr<FGsAgitDungeonRaidBossData>> _bossDataList;
	TArray<AgitDungeonRaidId> _bookmarkList;
	
	TMap<AgitDungeonRaidId, TWeakPtr<FGsAgitDungeonRaidBossData>> _bossDataMap; // 편의를 위한 데이터

	// 개선: 던전 안에서 사용할 값들. 다른 구조체고 만들어서 날리는게 나을지도.
public:
	AgitDungeonRaidId _dungeonRaidId = INVALID_AGIT_DUNGEON_RAID_ID;
	// 공헌도 정보
	TArray<TSharedPtr<FGsContributionData>> _dungeonRankingList;
	// 클리어 걸린시간 (결과창)
	int32 _clearSecond = 0;

public:
	FGsAgitDungeonRaidData() = default;
	~FGsAgitDungeonRaidData();

public:
	void Initialize();
	void Clear(); // 삭제 시
	void ClearDungeonData(); // 던전 닫을 때
	void Reset(); // 컨텐츠 리셋 시

	void SetData(PD::GC::PKT_GC_ACK_AGIT_DUNGEON_RAID_READ* InPacket);
	void SetDetailData(PD::GC::PKT_GC_ACK_AGIT_DUNGEON_RAID_DETAIL_READ* InPacket);
	void SetRaidState(AgitDungeonRaidId InRaidId, AgitDungeonRaidState InState);
	void SetBookmarkList(PD::SC::PKT_SC_ACK_AGIT_DUNGEON_RAID_BOOKMARK_LIST_READ* InPacket);
	void AddBookmark(AgitDungeonRaidId InRaidId);
	void RemoveBookmark(AgitDungeonRaidId InRaidId);
	void SetDungeonRaidRank(PD::SC::PKT_SC_INFORM_AGIT_DUNGEON_RAID_RANK_READ* InPacket);

public:
	int32 GetMaxRaidCount() const;
	int32 GetRemainRaidCount() const;
	void GetSortedBossList(OUT TArray<TWeakPtr<FGsAgitDungeonRaidBossData>>& OutList);

	TWeakPtr<FGsAgitDungeonRaidBossData> GetBossDataById(AgitDungeonRaidId InRaidId);
	TWeakPtr<FGsAgitDungeonRaidBossData> GetCurrOpenBossData(); // 현재 열려있는 보스정보(한개만 존재). 없으면 null
	bool IsBookmarked(AgitDungeonRaidId InRaidId) const;

	int32 GetTicketCount() const { return _ticketCount; }
	time_t GetResetTime() const { return  _resetTime; }

public:	
	void SetEnterRaidId(AgitDungeonRaidId InRaidId) { _dungeonRaidId = InRaidId; }
	time_t GetDungeonCloseTime();
	void GetSortedRankingList(OUT TArray<TWeakPtr<FGsContributionData>>& OutList);
};
