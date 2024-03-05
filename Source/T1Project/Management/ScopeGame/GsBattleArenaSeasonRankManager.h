#pragma once

#include "CoreMinimal.h"
#include "../UTIL/GsTimeUtil.h"
#include "../Classes/GsManager.h"
#include "../Message/GsMessageSystem.h"
#include "../Message/GsMessageGameObject.h"
#include "../BattleArenaRank/GsBattleArenaRankData.h"
#include "../BattleArenaRank/GsBattleArenaRankScheduleHandler.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"


class T1PROJECT_API FGsBattleArenaSeasonRankManager final : public IGsManager
{
private:
	/**
	 * cached~~ 명은 Initialize / Finalize 할때 만 초기화.
	 */
	TMap<RankSearchType, TArray<const struct FGsSchemaBattleArenaTierData*>> _cachedTierDataSet;
	TMap<RankSearchType, TArray<const struct FGsSchemaBattleArenaTierSeasonReward*>> _cachedTierRewardDataSet;
	TMap<RankSearchType, TArray<const struct FGsSchemaBattleArenaRankingSeasonReward*>> _cachedRankRewardDataSet;

private:
	// 2024/1/19 PKT - 개인 전은 아직 없지만 만들어만 둠.
	TMap<GuildWarTierDataId, FGsBattleArenaRankTierData> _personRankDataSet;
	// 2024/1/19 PKT - 길드 전 랭킹 데이터
	TMap<GuildWarTierDataId, FGsBattleArenaRankTierData> _guildRankDataSet;
	// 2024/1/22 PKT - 내 정보
	TMap<RankSearchType, FGsBattleArenaRankLocalData> _localRankData;
	// 2024/1/19 PKT - Scheduler
	friend class FGsBattleArenaRankScheduleHandler;
	FGsBattleArenaRankScheduleHandler _schedleHandler;	
	// 2024/1/30 PKT - 다음 화면 갱신을 체크하여, 화면 갱신 때가 되면 서버에 요청하기 위해 타이머 둠.
	TMap<RankSearchType, FGsSimpleTimer> _nextUpdateTimer;

	/**
	 * Message Delegate
	 */
private:
	MsgSystemHandleArray _systemMsg;
	MsgGameObjHandleArray _gameObjectMsg;
	
public:
	virtual void Initialize() final;
	virtual void Finalize() final;
	virtual void Update(float inTick) final;

	void ClearCurrentTask(RankSearchType InRankSearchType);
	/**
	 * Message
	 */
private:
	void RegisterMessages();
	void UnegisterMessages();

	// 2024/1/30 PKT - Wrap 등의 대한 이벤트
	void OnLoadTopoStart(const struct IGsMessageParam* InParam);
	// 2024/1/30 PKT - 네트워크 단절 이벤트
	void OnReconnectStart(const struct IGsMessageParam* InParam);
	// 2024/1/30 PKT - 로컬 플레이어 Spawn 이벤트
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam* InParam);

	/**
	 * Set / Update
	 */
private:
	// 2024/1/22 PKT - 정보가 새롭게 갱신 및 초기화 단계에서 호출
	void ResetData(RankSearchType InRankSearchType);	
	// 2024/1/22 PKT - 각 티어별 Rank 정보를 생성 and 초기화
	void CreateInitRankEntry(RankSearchType InRankSearchType, const TArray<TPair<GuildWarTierDataId,int32>>& InTierSizeSet);
	// 2024/1/22 PKT - 서버로 부터 갱신되는 정보들을 받아 갱신함.
	void FillRankData(RankSearchType InRankSearchType, GuildWarTierDataId InTierDataId, const TArray<FGsBattleArenaRankListItem>& InData);
	// 2024/1/23 PKT - 서버에게 랭킹 정보를 요청하기 위해 스케쥴러에 등록
	void PushSchedulerData(RankSearchType InRankSearchType, GuildWarTierDataId InTierDataId, int32 InIndex);
	// 2024/2/1 PKT - 정보 갱신 요청 타이머 설정
	void SetRefreshTimer(RankSearchType InRankSearchType, bool InImmediate, time_t InNextUpdateTime = 0);

public:
	// 2024/1/30 PKT - 내 정보
	void UpdateLocalData(RankSearchType InRankSearchType, const FGsBattleArenaRankLocalData& InLocalData, const TArray<TPair<GuildWarTierDataId,int32>>& InEntryMaxCount);
	// 2024/1/30 PKT - Rank 정보
	void UpdateRankData(RankSearchType InRankSearchType, time_t InNextUpdateTime, GuildWarTierDataId InTierDataId, const TArray<FGsBattleArenaRankListItem>& InData);

	/**
	 * Get / Find
	 */
public:
	// 2024/1/22 PKT - 시즌 정보가 활성화 될때 가장 먼저 호출
	bool BuildLocalRankData(RankSearchType InRankSearchType);
	// 2024/1/23 PKT - 각 티어별 랭킹 정보
	int32 BuildRankListByTier(RankSearchType InRankSearchType, GuildWarTierDataId InTierDataId);
		
	// 2024/1/25 PKT - Local User 의 시즌 Rank 정보
	const FGsBattleArenaRankLocalData* GetLocalSeasonRanklData(RankSearchType InRankSearchType) const;
	const struct FGsSchemaBattleArenaSeasonData* GetSeasonData(RankSearchType InRankSearchType) const;

	const struct FGsBattleArenaRankListItem* FindRankData(RankSearchType InRankSearchType, GuildWarTierDataId InTierDataId, int32 InIndex);

	// 2024/1/25 PKT - Tier Data
	int32 SeasonTierCount(RankSearchType InRankSearchType);
	const struct FGsSchemaBattleArenaTierData* FindTierData(RankSearchType InRankSearchType, int32 InIndex);
	const struct FGsSchemaBattleArenaTierSeasonReward* FindTierRewardData(RankSearchType InRankSearchType, int32 InIndex);
	
	int32 SeasonRankCount(RankSearchType InRankSearchType);
	const struct FGsSchemaBattleArenaRankingSeasonReward* FindRankRewardData(RankSearchType InRankSearchType, int32 InIndex);

	const struct FGsSchemaBattleArenaTierData* FindTierData(RankSearchType InRankSearchType, GuildWarTierDataId InTierDataId);

public:
	int32 SeasonNum(RankSearchType InRankSearchType) const;
	const FText SeasonDateText(RankSearchType InRankSearchType) const;

	GuildWarTierDataId GetTierDataIdByScore(RankSearchType InRankSearchType, int32 InRank, int32 InScore);
	/**
	 * Cached..
	 */
private:
	/**
	 * Caching Schema Data
	 */
	// 2024/1/23 PKT - 랭킹 설정 정보
	const struct FGsSchemaBattleArenaRankConfig* FindConfigData(RankSearchType InRankSearchType) const;
	// 2024/1/24 PKT - 각 Tier 별 보상 정보(티어 기준으로 정렬 되어 있음)
	const TArray<const struct FGsSchemaBattleArenaTierSeasonReward*>* FindTierRewardDataSet(RankSearchType InRankSearchType);
	// 2024/1/24 PKT - 각 순위 별 보상 정보(순위 기준으로 정렬 되어 있음)
	const TArray<const struct FGsSchemaBattleArenaRankingSeasonReward*>* FindRankRewardDataSet(RankSearchType InRankSearchType);
	// 2024/1/24 PKT - 각 Tier 정보(점수 기준으로 정렬 되어 있음)
	const TArray<const struct FGsSchemaBattleArenaTierData*>* FindTierDataSet(RankSearchType InRankSearchType);
};

#define GsBattleArenaRank() UGsScopeHolder::GetGameManagerFType<FGsBattleArenaSeasonRankManager>(UGsScopeGame::EManagerType::BattleArenaRank)