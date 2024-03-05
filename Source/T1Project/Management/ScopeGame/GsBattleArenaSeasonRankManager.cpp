#include "GsBattleArenaSeasonRankManager.h"
#include "../Guild/GsGuildData.h"
#include "../Data/GsGlobalConstant.h"
#include "../Data/GsDataContainManager.h"
#include "../ScopeGlobal/GsGuildManager.h"
#include "../ScopeGlobal/GsGameFlowManager.h"
#include "../GsMessageHolder.h"
#include "../Message/GsMessageBattleArena.h"
#include "../Message/MessageParam/GsBattleArenaMessageParam.h"
#include "../UTIL/GsTimeSyncUtil.h"
#include "../Net/GsNetSendService.h"

#include "../GameFlow/GsGameFlowGame.h"
#include "../GameFlow/GameContents/GsContentsMode.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaGuildWarHandler.h"

#include "DataSchema/BattleArena/GsSchemaBattleArenaTierData.h"
#include "DataSchema/BattleArena/GsSchemaBattleArenaRankConfig.h"
#include "DataSchema/BattleArena/GsSchemaBattleArenaSeasonData.h"
#include "DataSchema/BattleArena/GsSchemaBattleArenaTierSeasonReward.h"
#include "DataSchema/BattleArena/GsSchemaBattleArenaRankingSeasonReward.h"

#include "../UI/UIContent/Helper/GsTimeStringHelper.h"

#include "T1Project.h"


void FGsBattleArenaSeasonRankManager::Initialize()
{
	// 2024/1/30 PKT - Cache~~ 초기화
	_cachedTierDataSet.Empty();
	_cachedTierRewardDataSet.Empty();
	_cachedRankRewardDataSet.Empty();

	// 2024/1/30 PKT - Data Reset
	ResetData(RankSearchType::MAX);

	// 2024/1/30 PKT - Schedule Init
	_schedleHandler.Initialize();

	// 2024/1/30 PKT - Bind Message
	RegisterMessages();
}

void FGsBattleArenaSeasonRankManager::Finalize()
{
	// 2024/1/30 PKT - UnBind Message
	UnegisterMessages();

	// 2024/1/30 PKT - Reset Data
	ResetData(RankSearchType::MAX);
	
	// 2024/1/30 PKT - schedule Exit
	_schedleHandler.Finalize();

	// 2024/1/30 PKT - Cache~~ 삭제
	_cachedTierDataSet.Empty();
	_cachedTierRewardDataSet.Empty();
	_cachedRankRewardDataSet.Empty();
}

void FGsBattleArenaSeasonRankManager::Update(float inTick)
{
	_schedleHandler.Update(inTick);

	for (auto& item : _nextUpdateTimer)
	{	// 2024/1/30 PKT - 정보 갱신 요청을 위한 타이머
		item.Value.Update(inTick);
	}
}

void FGsBattleArenaSeasonRankManager::ClearCurrentTask(RankSearchType InRankSearchType)
{
	// 2024/2/1 PKT - 응답을 기다리고 있다면 초기화 시킨다.
	auto StateChange = [](TMap<GuildWarTierDataId, FGsBattleArenaRankTierData>& InData)
	{
		for (auto& data : InData)
		{
			if (false == data.Value._isCompleted)
			{
				for (auto& item : data.Value._items)
				{	// 2024/2/5 PKT - 응답 대기 상태에 있는 것들을 모두 초기화 시켜 버림.
					if (ValidRankDataState::Validate != item._isValidate)
						item._isValidate = ValidRankDataState::Invalidate;
				}
			}
		}
	};

	if (RankSearchType::MAX == InRankSearchType)
	{
		StateChange(_personRankDataSet);
		StateChange(_guildRankDataSet);

		_nextUpdateTimer.Empty();
	}
	else
	{
		StateChange(RankSearchType::KNIGHTS == InRankSearchType ? _guildRankDataSet : _personRankDataSet);

		if (_nextUpdateTimer.Contains(InRankSearchType))
		{
			_nextUpdateTimer[InRankSearchType].Stop();
			_nextUpdateTimer.Remove(InRankSearchType);
		}
	}	
	// 2024/2/1 PKT - 스케쥴 활동 중단.
	_schedleHandler.ClearAll(InRankSearchType);
}

void FGsBattleArenaSeasonRankManager::RegisterMessages()
{
	UnegisterMessages();

	FGsMessageHolder* messageHolder = GMessage();
	if (nullptr == messageHolder)
	{
		GSLOG(Error, TEXT("nullptr == messageHolder"));
		return;
	}

	_systemMsg.Emplace(
		messageHolder->GetSystemParam().AddRaw(
			MessageSystem::LOAD_TOPO_START, this, &FGsBattleArenaSeasonRankManager::OnLoadTopoStart)
	);

	_systemMsg.Emplace(
		messageHolder->GetSystemParam().AddRaw(
			MessageSystem::RECONNECT_START, this, &FGsBattleArenaSeasonRankManager::OnReconnectStart)
	);

	_gameObjectMsg.Emplace(
		messageHolder->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this,
			&FGsBattleArenaSeasonRankManager::OnLocalPlayerSpawnComplete)
	);	
}

void FGsBattleArenaSeasonRankManager::UnegisterMessages()
{
	FGsMessageHolder* messageHolder = GMessage();
	if (nullptr == messageHolder)
	{
		GSLOG(Error, TEXT("nullptr == messageHolder"));
		return;
	}
	
	for (auto iter : _systemMsg)
	{
		messageHolder->GetSystemParam().Remove(iter);
	}
	_systemMsg.Empty();

	for (auto iter : _gameObjectMsg)
	{
		messageHolder->GetGameObject().Remove(iter);
	}
	_gameObjectMsg.Empty();
}

void FGsBattleArenaSeasonRankManager::OnLoadTopoStart(const struct IGsMessageParam* InParam)
{
	// 2024/1/30 PKT - 포털 이동 등 발생 시 모든 Data 초기화
	ResetData(RankSearchType::MAX);
}

void FGsBattleArenaSeasonRankManager::OnReconnectStart(const struct IGsMessageParam* InParam)
{
	// 2024/1/30 PKT - 재연결 발생 시 모든 Data 초기화
	ResetData(RankSearchType::MAX);
}

void FGsBattleArenaSeasonRankManager::OnLocalPlayerSpawnComplete(const struct IGsMessageParam* InParam)
{
	// 2024/1/30 PKT - 케릭터 Spawn 모든 Data 초기화
	ResetData(RankSearchType::MAX);
}

void FGsBattleArenaSeasonRankManager::ResetData(RankSearchType InRankSearchType)
{	
	if (RankSearchType::MAX == InRankSearchType)
	{	// 2024/1/30 PKT - Reset All
		_personRankDataSet.Empty();
		_nextUpdateTimer.Empty();
		_localRankData.Empty();

		_schedleHandler.ClearAll(InRankSearchType);		
	}
	else
	{	// 2024/1/30 PKT - Type 에 따른 초기화
		if (RankSearchType::PERSONAL == InRankSearchType)
		{	// 2024/1/22 PKT - Type이 개인
			_personRankDataSet.Empty();
			_localRankData.Remove(RankSearchType::PERSONAL);
		}
		else if (RankSearchType::KNIGHTS == InRankSearchType)
		{	// 2024/1/22 PKT - Type이 기사단
			_guildRankDataSet.Empty();
			_localRankData.Remove(RankSearchType::KNIGHTS);
		}
		
		if (_nextUpdateTimer.Contains(InRankSearchType))
		{	// 2024/1/30 PKT - 갱신 타이머 초기화
			_nextUpdateTimer.Remove(InRankSearchType);
		}

		_schedleHandler.ClearAll(InRankSearchType);
	}
	
	if (FGsMessageHolder* messageHolder = GMessage())
	{	// 2024/1/20 PKT - Send Message(초기화)
		FGsBattleArenaRankResetData message(InRankSearchType);
		messageHolder->GetBattleArenaRank().SendMessage(MessageContentBattleRank::RESET_RANK_DATA, &message);
	}
}

void FGsBattleArenaSeasonRankManager::CreateInitRankEntry(RankSearchType InRankSearchType, const TArray<TPair<GuildWarTierDataId, int32>>& InTierSizeSet)
{

	if (0 > InTierSizeSet.Num())
	{
		GSLOG(Error, TEXT("0 > InSize"));
		return;
	}
	
	auto CreateInit = [InTierSizeSet](TMap<GuildWarTierDataId, FGsBattleArenaRankTierData>& InData)
	{	// 2024/1/22 PKT - 생성과 동시에 빈 정보를 넣어 준다.
		for (int32 idx = 0; idx < InTierSizeSet.Num(); ++idx)
		{
			GuildWarTierDataId tierDataId = InTierSizeSet[idx].Key;
			int32 size = InTierSizeSet[idx].Value;
			
			InData.FindOrAdd(tierDataId)._items.Empty(0);
			InData.FindOrAdd(tierDataId)._items.Init(FGsBattleArenaRankListItem(), size);
			// 2024/1/30 PKT - 해당 Tier에 Size가 0인 경우는 데이터가 없는 경우인데 이때는 완료 처리
			InData.FindOrAdd(tierDataId)._isCompleted = (0 == size) ? true : false;
		}		
	};
	
	// 2024/1/30 PKT - 갯수에 맞게 Data 생성
	CreateInit(RankSearchType::KNIGHTS == InRankSearchType ? _guildRankDataSet : _personRankDataSet);

	if (FGsMessageHolder* messageHolder = GMessage())
	{
		FGsBattleArenaRankCreateData message(InRankSearchType);
		messageHolder->GetBattleArenaRank().SendMessage(MessageContentBattleRank::CREATE_RANK_DATA, &message);
	}
}

void FGsBattleArenaSeasonRankManager::FillRankData(RankSearchType InRankSearchType, GuildWarTierDataId InTierDataId, const TArray<FGsBattleArenaRankListItem>& InData)
{
	auto FillDatas = [InTierDataId, InData](TMap<GuildWarTierDataId, FGsBattleArenaRankTierData>& InDataSet, TArray<int32>& OutDataSet)
	{
		OutDataSet.Empty();

		if (false == InDataSet.Contains(InTierDataId))
		{	// 2024/1/30 PKT - 아직 생성되지 않은 Tier
			GSLOG(Warning, TEXT("false == InDataSet.Contains(InTierDataId)"));
			return;
		}

		if (InDataSet[InTierDataId]._isCompleted)
		{	// 2024/1/30 PKT - 이미 완료가 된 상태
			GSLOG(Warning, TEXT("InDataSet[InTierDataId]._isCompleted"));
			return;
		}

		// 2024/1/30 PKT - Array돌면서 Index로 Data를 찾아서 넣어 준다.
		for (const auto& item : InData)
		{
			if (InDataSet[InTierDataId]._items.IsValidIndex(item._index)
				&& ValidRankDataState::Validate != InDataSet[InTierDataId]._items[item._index]._isValidate)
			{
				InDataSet[InTierDataId]._items[item._index] = item;
				InDataSet[InTierDataId]._items[item._index]._isValidate = ValidRankDataState::Validate;

				OutDataSet.Emplace(item._index);
			}			
		}

		// 2024/1/30 PKT - 완료 되었는지 검사한다.
		bool isComplete = true;
		for (const auto& item : InDataSet[InTierDataId]._items)
		{
			if (ValidRankDataState::Validate != item._isValidate)
			{
				isComplete = false;
				break;
			}
		}
		InDataSet[InTierDataId]._isCompleted = isComplete;
	};
	
	// 2024/1/30 PKT - View 갱신해야 할 Index
	TArray<int32> updateRankSet;
	FillDatas(RankSearchType::KNIGHTS == InRankSearchType ? _guildRankDataSet : _personRankDataSet, updateRankSet);

	if (0 < updateRankSet.Num())
	{
		if (FGsMessageHolder* messageHolder = GMessage())
		{
			FGsBattleArenaRankUpdateData message(InRankSearchType, InTierDataId, updateRankSet);
			messageHolder->GetBattleArenaRank().SendMessage(MessageContentBattleRank::UPDATE_RANK_DATA, &message);
		}		
	}
}

void FGsBattleArenaSeasonRankManager::PushSchedulerData(RankSearchType InRankSearchType, GuildWarTierDataId InTierDataId, int32 InIndex)
{
	auto PushScheduler = [&](TMap<GuildWarTierDataId, FGsBattleArenaRankTierData>& InDataSet)
	{
		if (false == InDataSet.Contains(InTierDataId) || InDataSet[InTierDataId]._isCompleted)
		{	// 2024/1/30 PKT - 아직 Rank 정보를 생성하지 않았음. or 모든 정보를 다 받았음.
			return;
		}

		if (false == InDataSet[InTierDataId]._items.IsValidIndex(InIndex))
		{
			GSLOG(Error, TEXT("false == InDataSet[InTierDataId]._items.IsValidIndex(InIndex)"));
			return;
		}

		TArray<int32> indexSet;
		if (ValidRankDataState::Invalidate == InDataSet[InTierDataId]._items[InIndex]._isValidate)
		{
			indexSet.Emplace(InIndex);
		}

		// 2024/1/30 PKT - 한번에 서버에게 요청할 갯수
		int32 requestDataMaxCount = 50;
		if (UGsGlobalConstant* globalConstant = GData()->GetGlobalData())
		{
			requestDataMaxCount = globalConstant->_battleArenaRankMaxCount;
		}

		for (int32 idx = 1; idx < InDataSet[InTierDataId]._items.Num(); ++idx)
		{
			if (InDataSet[InTierDataId]._items.IsValidIndex(InIndex - idx))
			{
				if (ValidRankDataState::Invalidate == InDataSet[InTierDataId]._items[InIndex - idx]._isValidate)
				{
					indexSet.Emplace(InIndex - idx);
				}
			}

			if (InDataSet[InTierDataId]._items.IsValidIndex(InIndex + idx))
			{
				if (ValidRankDataState::Invalidate == InDataSet[InTierDataId]._items[InIndex + idx]._isValidate)
				{
					indexSet.Emplace(InIndex + idx);
				}
			}

			if (requestDataMaxCount <= indexSet.Num())
			{	// 2024/1/30 PKT - 대충 제한 갯수보다 크면 빠져 나온다.(제한 갯수보다 +1정도 더 클수도 있음.)
				break;
			}
		}

		// 2024/1/26 PKT - Push
		_schedleHandler.Push(InRankSearchType, InTierDataId, MoveTemp(indexSet));
	};

	PushScheduler(RankSearchType::KNIGHTS == InRankSearchType ? _guildRankDataSet : _personRankDataSet);
}

void FGsBattleArenaSeasonRankManager::SetRefreshTimer(RankSearchType InRankSearchType, bool InImmediate, time_t InNextUpdateTime /* = 0 */)
{
	int32 limitUpdateTime = 2;
	if (UGsGlobalConstant* globalConstant = GData()->GetGlobalData())
	{
		limitUpdateTime = globalConstant->_battleArenaLimitUpdateTimeS;
	}

	time_t currentTime = FGsTimeSyncUtil::GetServerNowSecond();
	time_t remainingTime = (InNextUpdateTime > currentTime) ? InNextUpdateTime - currentTime : 0;
	remainingTime = remainingTime < limitUpdateTime ? limitUpdateTime : remainingTime;
	if (true == InImmediate)
	{
		remainingTime = 0;
	}

	FGsSimpleTimer& timer = _nextUpdateTimer.FindOrAdd(InRankSearchType);
	timer.Stop();
	timer.SetInterval(static_cast<float>(remainingTime));
	timer.Router.AddLambda([]()
		{
			// 2024/2/1 PKT - 갱신 요청
			GuildDBId localGuildDBId = INVALID_GUILD_DB_ID;
			if (GGuild() && GGuild()->GetMyGuildData())
			{	// 2024/2/2 PKT - 내 길드 정보를 보내야 함.
				localGuildDBId = GGuild()->GetMyGuildData()->_guildDBId;
			}
			FGsNetSendService::SendReqBattleArenaLocalInfo(localGuildDBId);
		}
	);
	timer.Start();
};

void FGsBattleArenaSeasonRankManager::UpdateLocalData(RankSearchType InRankSearchType, const FGsBattleArenaRankLocalData& InLocalData, const TArray<TPair<GuildWarTierDataId, int32>>& InEntryMaxCount)
{
	const int32 INVALID_TIME = 0;
	time_t prevTime = _localRankData.Contains(InRankSearchType) ? _localRankData[InRankSearchType]._nextUpdateTime : INVALID_TIME;

	bool isNewData = false;
	if (prevTime != InLocalData._nextUpdateTime)
	{
		isNewData = true;

		ResetData(InRankSearchType);
		// 2024/1/23 PKT - 내 정보를 갱신.
		_localRankData.FindOrAdd(InRankSearchType) = InLocalData;
		// 2024/2/1 PKT - Rank 생성
		CreateInitRankEntry(InRankSearchType, InEntryMaxCount);				
	}
	else
	{
		// 2024/1/30 PKT - 기존 데이터는 있지만 Play Count는 실시간 갱신
		_localRankData.FindOrAdd(InRankSearchType)._playCount = InLocalData._playCount;
	}

	// 2024/2/1 PKT - 갱신 요청 타이머 설정
	SetRefreshTimer(InRankSearchType, false, InLocalData._nextUpdateTime);

	if (FGsMessageHolder* messageHolder = GMessage())
	{
		FGsPrimitiveInt32 message(static_cast<int32>(isNewData));
		messageHolder->GetBattleArenaRank().SendMessage(MessageContentBattleRank::LOCAL_RANK_DATA, &message);
	}
}

void FGsBattleArenaSeasonRankManager::UpdateRankData(RankSearchType InRankSearchType, time_t InNextUpdateTime, GuildWarTierDataId InTierDataId, const TArray<FGsBattleArenaRankListItem>& InData)
{
	if (_localRankData.Contains(InRankSearchType) && ValidRankDataState::Validate == _localRankData[InRankSearchType]._rankData._isValidate)
	{	// 2024/2/7 PKT - 내 정보가 정상적으로 들어 왔을때.
		if (_localRankData[InRankSearchType]._nextUpdateTime == InNextUpdateTime)
		{	// 2024/2/7 PKT - 버전이 같으면 넣어 준다.
			FillRankData(InRankSearchType, InTierDataId, InData);
		}
	}
}

bool FGsBattleArenaSeasonRankManager::BuildLocalRankData(RankSearchType InRankSearchType)
{
	SetRefreshTimer(InRankSearchType, true);

	return _localRankData.Contains(InRankSearchType);
}

// 2024/1/23 PKT - 각 티어별 랭킹 정보
int32 FGsBattleArenaSeasonRankManager::BuildRankListByTier(RankSearchType InRankSearchType, GuildWarTierDataId InTierDataId)
{
	auto CurrentSize = [InTierDataId](const TMap<GuildWarTierDataId, FGsBattleArenaRankTierData>& InList) -> int32
	{
		return InList.Contains(InTierDataId) ? InList[InTierDataId]._items.Num() : -1;
	};
	return CurrentSize(RankSearchType::KNIGHTS == InRankSearchType ? _guildRankDataSet : _personRankDataSet);
}

const FGsBattleArenaRankLocalData* FGsBattleArenaSeasonRankManager::GetLocalSeasonRanklData(RankSearchType InRankSearchType) const
{
	return (_localRankData.Contains(InRankSearchType)) ? &(_localRankData[InRankSearchType]) : nullptr;
}

const FGsSchemaBattleArenaSeasonData* FGsBattleArenaSeasonRankManager::GetSeasonData(RankSearchType InRankSearchType) const
{
	const FGsSchemaBattleArenaRankConfig* schemaConfigData = FindConfigData(InRankSearchType);
	return (schemaConfigData && schemaConfigData->seasonDataId.GetRow()) ? schemaConfigData->seasonDataId.GetRow() : nullptr;
}

const FGsBattleArenaRankListItem* FGsBattleArenaSeasonRankManager::FindRankData(RankSearchType InRankSearchType, GuildWarTierDataId InTierDataId, int32 InIndex)
{
	auto FindData = [&](const TMap<GuildWarTierDataId, FGsBattleArenaRankTierData>& InList) -> const FGsBattleArenaRankListItem*
	{
		if (false == InList.Contains(InTierDataId) || false == InList[InTierDataId]._items.IsValidIndex(InIndex))
		{
			GSLOG(Error, TEXT("false == InList.Contains(InTier) || false == InList[InTier]._items.IsValidIndex(InIndex)"));
			return nullptr;
		}

		if (ValidRankDataState::Validate != InList[InTierDataId]._items[InIndex]._isValidate)
		{
			if (ValidRankDataState::Invalidate == InList[InTierDataId]._items[InIndex]._isValidate)
			{	// 2024/1/26 PKT - 요청중이 아닌 정보일 경우 요청하도록 한다.
				PushSchedulerData(InRankSearchType, InTierDataId, InIndex);
			}

			return nullptr;
		}

		return &(InList[InTierDataId]._items[InIndex]);
	};

	return FindData(RankSearchType::KNIGHTS == InRankSearchType ? _guildRankDataSet : _personRankDataSet);
}

int32 FGsBattleArenaSeasonRankManager::SeasonTierCount(RankSearchType InRankSearchType)
{
	const TArray<const FGsSchemaBattleArenaTierData*>* tierDataSet = FindTierDataSet(InRankSearchType);
	return (tierDataSet) ? (*tierDataSet).Num() : 0;
}

const FGsSchemaBattleArenaTierData* FGsBattleArenaSeasonRankManager::FindTierData(RankSearchType InRankSearchType, int32 InIndex)
{
	const TArray<const FGsSchemaBattleArenaTierData*>* tierDataSet = FindTierDataSet(InRankSearchType);
	return (tierDataSet && (*tierDataSet).IsValidIndex(InIndex)) ? (*tierDataSet)[InIndex] : nullptr;
}

const FGsSchemaBattleArenaTierSeasonReward* FGsBattleArenaSeasonRankManager::FindTierRewardData(RankSearchType InRankSearchType, int32 InIndex)
{
	const TArray<const struct FGsSchemaBattleArenaTierSeasonReward*>* tierRewardDataSet = FindTierRewardDataSet(InRankSearchType);
	return (tierRewardDataSet && (*tierRewardDataSet).IsValidIndex(InIndex)) ? (*tierRewardDataSet)[InIndex] : nullptr;
}

int32 FGsBattleArenaSeasonRankManager::SeasonRankCount(RankSearchType InRankSearchType)
{
	const TArray<const FGsSchemaBattleArenaRankingSeasonReward*>* rankRewardDataSet = FindRankRewardDataSet(InRankSearchType);
	return (rankRewardDataSet) ? (*rankRewardDataSet).Num() : 0;
}

const FGsSchemaBattleArenaRankingSeasonReward* FGsBattleArenaSeasonRankManager::FindRankRewardData(RankSearchType InRankSearchType, int32 InIndex)
{
	const TArray<const FGsSchemaBattleArenaRankingSeasonReward*>* rankRewardDataSet = FindRankRewardDataSet(InRankSearchType);
	return (rankRewardDataSet && (*rankRewardDataSet).IsValidIndex(InIndex)) ? (*rankRewardDataSet)[InIndex] : nullptr;
}

const FGsSchemaBattleArenaTierData* FGsBattleArenaSeasonRankManager::FindTierData(RankSearchType InRankSearchType, GuildWarTierDataId InTierDataId)
{
	int32 findIndex = INDEX_NONE;
	const TArray<const FGsSchemaBattleArenaTierData*>* tierDataSet = FindTierDataSet(InRankSearchType);
	if (tierDataSet)
	{
		findIndex = tierDataSet->IndexOfByPredicate([InTierDataId](const FGsSchemaBattleArenaTierData* InData)
			{
				return InData->id == InTierDataId;
			}
		);
	}	
	return (findIndex == INDEX_NONE) ? nullptr : (*tierDataSet)[findIndex];
}

int32 FGsBattleArenaSeasonRankManager::SeasonNum(RankSearchType InRankSearchType) const
{
	const FGsSchemaBattleArenaSeasonData* schemaSeasonData = GetSeasonData(InRankSearchType);
	return (schemaSeasonData) ? schemaSeasonData->seasonNumber : -1;
}

const FText FGsBattleArenaSeasonRankManager::SeasonDateText(RankSearchType InRankSearchType) const
{
	const FGsSchemaBattleArenaSeasonData* schemaSeasonData = GetSeasonData(InRankSearchType);
	if (nullptr == schemaSeasonData)
	{
		GSLOG(Error, TEXT("nullptr == schemaSeasonData"));
		return FText::GetEmpty();
	}

	if (schemaSeasonData->endDate.IsEmpty())
	{	// 2024/2/20 PKT - 종료 날짜가 없는 경우가 있다.(종료 날짜를 확정 짓지 않았을 경우)
		return FText::GetEmpty();
	}

	FDateTime endDate;
	if (false == FDateTime::Parse(schemaSeasonData->endDate, endDate))
	{
		GSLOG(Error, TEXT("false == FDateTime::Parse_schemaData->endDateTime, UpperDateTimeUtc), endDate:%s"), *schemaSeasonData->endDate);
		return FText::GetEmpty();
	}

	// 2022/12/28 PKT - 서버 -> 클라 시간
	FDateTime severToClientEndDate = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(endDate);

	FString endDateText;
	FGsTimeStringHelper::GetTimeStringNYMDHM(severToClientEndDate, endDateText);

	return FText::FromString(endDateText);
}

GuildWarTierDataId FGsBattleArenaSeasonRankManager::GetTierDataIdByScore(RankSearchType InRankSearchType, int32 InRank, int32 InScore)
{
	if (0 < InRank)
	{	// 2024/2/20 PKT - 경기에 한번도 참가하지 않은 정보는 Rank정보가 없음.
		const TArray<const FGsSchemaBattleArenaTierData*>* tierDataSet = FindTierDataSet(InRankSearchType);
		if (tierDataSet)
		{
			for (auto item : (*tierDataSet))
			{
				if (item && (item->scroeRange.rankingGroupRangeMin <= InScore && InScore <= item->scroeRange.rankingGroupRangeMax))
				{
					return item->id;
				}
			}
		}
	}
	
	return INVALID_GUILD_WAR_TIER_DATA_ID;
}

const FGsSchemaBattleArenaRankConfig* FGsBattleArenaSeasonRankManager::FindConfigData(RankSearchType InRankSearchType) const
{
	const UGsTable* table = FGsSchemaBattleArenaRankConfig::GetStaticTable();
	const TMap<FName, uint8*>* OutRowMap;
	if (nullptr == table || false == table->GetRowMap(OutRowMap))
	{
		GSLOG(Error, TEXT("nullptr == table || false == table->GetRowMap(OutRowMap)"));
		return nullptr;
	}

	for (const auto& pair : (*OutRowMap))
	{
		const FGsSchemaBattleArenaRankConfig* findData = reinterpret_cast<const FGsSchemaBattleArenaRankConfig*>(pair.Value);
		if (findData && findData->rankSearchType == InRankSearchType)
		{
			return findData;
		}
	}

	return nullptr;
}

const TArray<const FGsSchemaBattleArenaTierSeasonReward*>* FGsBattleArenaSeasonRankManager::FindTierRewardDataSet(RankSearchType InRankSearchType)
{
	if (false == _cachedTierRewardDataSet.Contains(InRankSearchType) || 0 >= _cachedTierRewardDataSet[InRankSearchType].Num())
	{
		const FGsSchemaBattleArenaRankConfig* schemaConfigData = FindConfigData(InRankSearchType);
		if (nullptr == schemaConfigData || nullptr == schemaConfigData->seasonDataId.GetRow())
		{
			GSLOG(Error, TEXT("nullptr == schemaConfigData || nullptr == schemaConfigData->seasonDataId.GetRow()"));
			return nullptr;
		}

		const FGsSchemaBattleArenaSeasonData* schemaSeasonData = schemaConfigData->seasonDataId.GetRow();
		if (nullptr == schemaSeasonData || 0 >= schemaSeasonData->tierRewardIdSet.Num())
		{
			GSLOG(Error, TEXT("nullptr == schemaSeasonData || 0 >= schemaSeasonData->tierRewardIdSet.Num()"));
			return nullptr;
		}

		TArray<const FGsSchemaBattleArenaTierSeasonReward*> tierRewardDataSet;
		for (const FGsSchemaBattleArenaTierSeasonRewardRow& itemRow : schemaSeasonData->tierRewardIdSet)
		{
			if (const FGsSchemaBattleArenaTierSeasonReward* seasonReward = itemRow.GetRow())
			{
				if (seasonReward && seasonReward->tierDataId.GetRow())
				{
					tierRewardDataSet.Emplace(seasonReward);
				}
			}
		}

		// 2024/1/24 PKT - Sort
		tierRewardDataSet.Sort([](const FGsSchemaBattleArenaTierSeasonReward& lhr, const FGsSchemaBattleArenaTierSeasonReward& rhr)
			{
				return lhr.tierDataId.GetRow()->scroeRange.rankingGroupRangeMin > rhr.tierDataId.GetRow()->scroeRange.rankingGroupRangeMin;
			}
		);

		// 2024/1/24 PKT - Move
		_cachedTierRewardDataSet.FindOrAdd(InRankSearchType) = MoveTemp(tierRewardDataSet);
	}

	return _cachedTierRewardDataSet.Contains(InRankSearchType) ? &_cachedTierRewardDataSet[InRankSearchType] : nullptr;
}

const TArray<const FGsSchemaBattleArenaRankingSeasonReward*>* FGsBattleArenaSeasonRankManager::FindRankRewardDataSet(RankSearchType InRankSearchType)
{
	if (false == _cachedRankRewardDataSet.Contains(InRankSearchType) || 0 >= _cachedRankRewardDataSet[InRankSearchType].Num())
	{
		const FGsSchemaBattleArenaRankConfig* schemaConfigData = FindConfigData(InRankSearchType);
		if (nullptr == schemaConfigData || nullptr == schemaConfigData->seasonDataId.GetRow())
		{
			GSLOG(Error, TEXT("nullptr == schemaConfigData || nullptr == schemaConfigData->seasonDataId.GetRow()"));
			return nullptr;
		}

		const FGsSchemaBattleArenaSeasonData* schemaSeasonData = schemaConfigData->seasonDataId.GetRow();
		if (nullptr == schemaSeasonData || 0 >= schemaSeasonData->tierRewardIdSet.Num())
		{
			GSLOG(Error, TEXT("nullptr == schemaSeasonData || 0 >= schemaSeasonData->tierRewardIdSet.Num()"));
			return nullptr;
		}

		TArray<const FGsSchemaBattleArenaRankingSeasonReward*> rankRewardDataSet;
		for (const FGsSchemaBattleArenaRankingSeasonRewardRow& itemRow : schemaSeasonData->rankRewardIdSet)
		{
			if (itemRow.GetRow())
			{
				rankRewardDataSet.Emplace(itemRow.GetRow());
			}
		}

		// 2024/1/24 PKT - Sort
		rankRewardDataSet.Sort([](const FGsSchemaBattleArenaRankingSeasonReward& lhr, const FGsSchemaBattleArenaRankingSeasonReward& rhr)
			{
				return lhr.rankingGroupData.rankingGroupRangeMin < rhr.rankingGroupData.rankingGroupRangeMin;
			}
		);

		// 2024/1/24 PKT - Move
		_cachedRankRewardDataSet.FindOrAdd(InRankSearchType) = MoveTemp(rankRewardDataSet);
	}

	return _cachedRankRewardDataSet.Contains(InRankSearchType) ? &_cachedRankRewardDataSet[InRankSearchType] : nullptr;
}

const TArray<const FGsSchemaBattleArenaTierData*>* FGsBattleArenaSeasonRankManager::FindTierDataSet(RankSearchType InRankSearchType)
{
	if (false == _cachedTierDataSet.Contains(InRankSearchType) || 0 >= _cachedTierDataSet[InRankSearchType].Num())
	{
		const TArray<const FGsSchemaBattleArenaTierSeasonReward*>* tierRewardDataSet = FindTierRewardDataSet(InRankSearchType);
		if (nullptr == tierRewardDataSet || 0 >= (*tierRewardDataSet).Num())
		{
			GSLOG(Error, TEXT("nullptr == tierRewardDataSet || 0 >= (*tierRewardDataSet).Num()"));
			return nullptr;
		}

		TArray<const FGsSchemaBattleArenaTierData*> tierDataSet;
		for (const FGsSchemaBattleArenaTierSeasonReward* item : (*tierRewardDataSet))
		{
			if (item && item->tierDataId.GetRow())
			{
				tierDataSet.Emplace(item->tierDataId.GetRow());
			}
		}

		// 2024/1/24 PKT - Sort
		tierDataSet.Sort([](const FGsSchemaBattleArenaTierData& lhr, const FGsSchemaBattleArenaTierData& rhr)
			{
				return lhr.scroeRange.rankingGroupRangeMin > rhr.scroeRange.rankingGroupRangeMin;
			}
		);

		// 2024/1/24 PKT - Move
		_cachedTierDataSet.FindOrAdd(InRankSearchType) = MoveTemp(tierDataSet);
	}

	return _cachedTierDataSet.Contains(InRankSearchType) ? &_cachedTierDataSet[InRankSearchType] : nullptr;
}