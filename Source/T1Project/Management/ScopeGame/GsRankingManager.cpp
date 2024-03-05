// Fill out your copyright notice in the Description page of Project Settings.

#include "Management/ScopeGame/GsRankingManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"

#include "T1Project.h"

#include "Management/GsMessageHolder.h"
#include "DataSchema/Ranking/GsSchemaRankingReward.h"
#include "DataSchema/Ranking/GsSchemaRankingCategory.h"
#include "DataSchema/Ranking/GsSchemaRankingTypeObjective.h"
#include "DataSchema/Ranking/GsSchemaRankingGroupCategory.h"
#include "DataSchema/InterServer/World/GsSchemaWorldData.h"
#include "Ranking/GsRankingData.h"

#include "Net/GsNetSendService.h"


void FGsRankingManager::Initialize()
{
	InitMainTabData();
	InitSubTabData();
	InitRewardData();
	_isSyncing = false;
}

void FGsRankingManager::Finalize()
{
	_mainTabDataList.Empty();
	_subTabDataListMap.Empty();
	_rewardDataMap.Empty();
	_currentRankingMap.Empty();
	_currentMyRankingMap.Empty();
	_currentRankingRequestInfo.Empty();

	_selectedServer = nullptr;

	_mainTabIndex = 0;

	_subTabIndex = 0;
	_groupTabIndex = 0;
	_isSyncing = false;
	_isFirstOpen = false;
}

void FGsRankingManager::ReleaseRankingData()
{
	_currentRankingMap.Empty();
	_currentMyRankingMap.Empty();
	_currentRankingRequestInfo.Empty();
}

void FGsRankingManager::RequestRankingData(RankType InRankType, WorldId InWorldId, int32 InMinRank)
{
	// 랭킹을 열고 한번만 랭킹 정보를 요청한다.
	TPair<RankType, WorldId> rankingKey(InRankType, InWorldId);
	if (false == _currentRankingRequestInfo.Contains(rankingKey))
	{
		TSet<int32> tempInfo;
		_currentRankingRequestInfo.Emplace(rankingKey, tempInfo);
	}

	TSet<int32>& requestInfo = _currentRankingRequestInfo[rankingKey];
	if (false == requestInfo.Contains(InMinRank))
	{
		requestInfo.Emplace(InMinRank);
		FGsNetSendService::SendReqRankingList(InRankType, InWorldId, InMinRank);
	}
}

void FGsRankingManager::Set(PD::GC::PKT_GC_ACK_RANK_READ& Packet)
{
	// 내 기사단 정보
	FString myGuildName;
	bool isExistGuild = false;
	if (FGsGuildManager* guildManager = GGuild())
	{
		if (const FGsGuildDataDetail* guildData = guildManager->GetGuildDetailData(true))
		{
			isExistGuild = guildData->IsExistGuild();
			if (isExistGuild)
			{
				myGuildName = guildData->_guildName;
			}
		}
	}

	// 내 기본 정보
	FString myUserName;
	WorldId myHomeWorldId = INVALID_WORLD_ID;
	if (const FGsNetUserData* userData = GGameData()->GetUserData())
	{
		myUserName = userData->mUserName;
		myHomeWorldId = userData->mHomeWorldId;
	}

	// 받은 랭킹의 기본 정보 및 랭킹을 저장할 공간
	RankType rankType = Packet.RankType();
	WorldId worldId = Packet.WorldId();
	TPair<RankType, WorldId> rankingKey(rankType, worldId);
	if (false == _currentRankingMap.Contains(rankingKey))
	{
		TMap<int32, FGsRankingData> tempMap;
		_currentRankingMap.Emplace(rankingKey, tempMap);
	}
	TMap<int32, FGsRankingData>& rankingMap = _currentRankingMap[rankingKey];

	// 받은 랭킹 정보는 요청했던 것으로 처리한다.
	if (false == _currentRankingRequestInfo.Contains(rankingKey))
	{
		TSet<int32> tempInfo;
		_currentRankingRequestInfo.Emplace(rankingKey, tempInfo);
	}

	TSet<int32>& requestInfo = _currentRankingRequestInfo[rankingKey];

	// MyPercentRank 정보를 갱신시켜줘야하는지 확인이 필요하다.
	if (false == _currentMyRankingMap.Contains(rankType))
	{
		FGsRankingMyData myData;
		myData._hasHighRanking = false;
		myData._ranking = Packet.MyPercentRank();
		myData._param1 = Packet.MyParam1();
		myData._param2 = Packet.MyParam2();
		_currentMyRankingMap.Emplace(rankType, myData);
	}

	EGsRankingDataType rankingDataType = GetRankingDataType(rankType);
	for (PD::GC::PKT_GC_ACK_RANK_READ::RankListIterator iter = Packet.GetFirstRankListIterator(); iter != Packet.GetLastRankListIterator(); ++iter)
	{
		if (rankingMap.Contains(iter->Rank()))
			continue;

		FGsRankingData rankingData;
		rankingData._ranking = iter->Rank();
		rankingData._previousRank = iter->PrevRank();
		switch (rankingDataType)
		{
		case EGsRankingDataType::Local:
			rankingData._detailData = MakeShareable(new FGsRankingDetailPersonal(iter));
			break;
		case EGsRankingDataType::World:
			rankingData._detailData = MakeShareable(new FGsRankingDetailWorld(iter));
			break;
		case EGsRankingDataType::Guild:
			rankingData._detailData = MakeShareable(new FGsRankingDetailGuild(iter));
			break;
		case EGsRankingDataType::GuildDungeon:
			rankingData._detailData = MakeShareable(new FGsRankingDetailGuildDungeon(iter));
			break;
		default:
			break;
		}
		rankingMap.Emplace(iter->Rank(), rankingData);
		requestInfo.Emplace(iter->Rank());

		// 내 상위권 랭킹 정보인지 확인
		if (rankingDataType == EGsRankingDataType::Local ||
			rankingDataType == EGsRankingDataType::World)
		{
			PlanetWorldId planetWorldId(iter->PlanetWorldId());
			WorldId rankingWorldId = planetWorldId.st.worldId;
			if (rankingWorldId == myHomeWorldId && myUserName.Equals(iter->UserName()))
			{
				_currentMyRankingMap[rankType]._highRanking = iter->Rank();
				_currentMyRankingMap[rankType]._previousRank = iter->PrevRank();
				_currentMyRankingMap[rankType]._hasHighRanking = true;
			}
		}
		else if (rankingDataType == EGsRankingDataType::Guild ||
			rankingDataType == EGsRankingDataType::GuildDungeon)
		{
			if (isExistGuild && myGuildName.Equals(iter->GuildName()))
			{
				_currentMyRankingMap[rankType]._highRanking = iter->Rank();
				_currentMyRankingMap[rankType]._previousRank = iter->PrevRank();
				_currentMyRankingMap[rankType]._hasHighRanking = true;
			}
		}
	}

	GMessage()->GetRanking().SendMessage(MessageContentRanking::INVALIDATE, nullptr);
}

void FGsRankingManager::InitMainTabData()
{
	const UGsTableRankingCategory* table = Cast<UGsTableRankingCategory>(FGsSchemaRankingCategory::GetStaticTable());
	if (nullptr == table)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[FGsRankingManager::InitMainTabData] MainTabData is Null"));
#endif
		return;
	}

	TArray<const FGsSchemaRankingCategory*> mainTabAllRows;
	if (false == table->GetAllRows(mainTabAllRows))
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[FGsRankingManager::InitMainTabData] MainTabData is Empty"));
#endif
		return;
	}

	_mainTabDataList.Empty();
	for (int32 i = 0; i < mainTabAllRows.Num(); ++i)
	{
		if (nullptr == mainTabAllRows[i])
			continue;

		_mainTabDataList.Emplace(mainTabAllRows[i]);
	}

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[FGsRankingManager::InitMainTabData] MainTabData is Empty"));
#endif
}

void FGsRankingManager::InitSubTabData()
{
	_subTabDataListMap.Empty();
	const FGsSchemaRankingCategory* mainTabData = nullptr;
	const FGsSchemaRankingTypeObjective* subTabData = nullptr;
	for (int32 i = 0; i < _mainTabDataList.Num(); ++i)
	{
		mainTabData = _mainTabDataList[i];
		if (0 >= mainTabData->rankingTypeObjective.Num())
		{
#if WITH_EDITOR
			GSLOG(Warning, TEXT("[FGsRankingManager::InitSubTabData] SubTabData is Empty"));
#endif
			continue;
		}

		for (int32 j = 0; j < mainTabData->rankingTypeObjective.Num(); ++j)
		{
			subTabData = mainTabData->rankingTypeObjective[j].GetRow();
			if (nullptr == subTabData)
				continue;

			if (_subTabDataListMap.Contains(mainTabData->index))
			{
				_subTabDataListMap[mainTabData->index].Emplace(subTabData);
			}
			else
			{
				_subTabDataListMap.Emplace(mainTabData->index, TArray<const FGsSchemaRankingTypeObjective*>{ subTabData });
			}
		}
	}
}

void FGsRankingManager::InitRewardData()
{
	_rewardDataMap.Empty();
	const UGsTableRankingReward* table = Cast<UGsTableRankingReward>(FGsSchemaRankingReward::GetStaticTable());
	if (nullptr == table)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[FGsRankingManager::InitRewardData] RankingReward is Null"));
#endif
		return;
	}

	TArray<const FGsSchemaRankingReward*> rewardAllRows;
	if (false == table->GetAllRows(rewardAllRows))
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[FGsRankingManager::InitRewardData] RankingReward is Empty"));
#endif
		return;
	}

	for (int32 i = 0; i < rewardAllRows.Num(); ++i)
	{
		if (nullptr == rewardAllRows[i])
			continue;

		const FGsSchemaRankingTypeObjective* subTabData = rewardAllRows[i]->rankingTypeObjectiveID.GetRow();
		if (nullptr == subTabData)
			continue;

		if (false == _rewardDataMap.Contains(subTabData->rankingType))
		{
			_rewardDataMap.Emplace(subTabData->rankingType, rewardAllRows[i]);
		}
		else
		{
			return;
		}
	}
}

const FGsSchemaRankingCategory* FGsRankingManager::GetMainTabData(int32 InIndex)
{
	if (_mainTabDataList.IsValidIndex(InIndex))
	{
		return _mainTabDataList[InIndex];
	}
	
	return nullptr;
}

const FGsSchemaRankingTypeObjective* FGsRankingManager::GetSubTabData(int32 InMainTabIndex, int32 InSubTabIndex)
{
	const FGsSchemaRankingCategory* mainTabData = GetMainTabData(InMainTabIndex);
	if (nullptr == mainTabData)
		return nullptr;

	if (false == mainTabData->rankingTypeObjective.IsValidIndex(InSubTabIndex))
		return nullptr;

	return mainTabData->rankingTypeObjective[InSubTabIndex].GetRow();
}

const FGsSchemaRankingTypeObjective* FGsRankingManager::GetSubTabData(RankType InRankingType)
{
	for (const TPair<int32, TArray<const FGsSchemaRankingTypeObjective*>>& mainTabData : _subTabDataListMap)
	{
		for (const FGsSchemaRankingTypeObjective* subTabData : mainTabData.Value)
		{
			if (subTabData->rankingType == InRankingType)
			{
				return subTabData;
			}
		}
	}
	return nullptr;
}

const FGsSchemaRankingGroupCategory* FGsRankingManager::GetGroupTabData(int32 InObjectiveId)
{
	const UGsTableRankingGroupCategory* table = Cast<UGsTableRankingGroupCategory>(FGsSchemaRankingGroupCategory::GetStaticTable());
	if (nullptr == table)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[FGsRankingManager::GetGroupTabData] GroupCategoryData is Null"));
#endif
		return nullptr;
	}

	TArray<const FGsSchemaRankingGroupCategory*> subTabGroupAllRows;
	if (false == table->GetAllRows(subTabGroupAllRows))
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[FGsRankingManager::GetGroupTabData] GroupCategoryData is Empty"));
#endif
		return nullptr;
	}

	for (const FGsSchemaRankingGroupCategory* subTabGroupData : subTabGroupAllRows)
	{
		if (nullptr == subTabGroupData)
			continue;

		if (subTabGroupData->rankingTypeObjectiveID.IsNull())
			continue;

		const FGsSchemaRankingTypeObjective* tempSubTabData = subTabGroupData->rankingTypeObjectiveID.GetRow();
		if (nullptr == tempSubTabData)
			continue;

		if (tempSubTabData->index == InObjectiveId)
		{
			return subTabGroupData;
		}
	}

	return nullptr;
}

const FGsSchemaRankingTypeObjective* FGsRankingManager::GetCurrentSubTabData()
{
	return GetSubTabData(_mainTabIndex, _subTabIndex);
}

int32 FGsRankingManager::GetNumMainTabs()
{
	return _mainTabDataList.Num();
}

int32 FGsRankingManager::GetNumSubTabs(int32 InMainTabIndex)
{
	const FGsSchemaRankingCategory* mainTabData = GetMainTabData(InMainTabIndex);
	if (nullptr == mainTabData)
		return 0;

	if (_subTabDataListMap.Contains(mainTabData->index))
	{
		return _subTabDataListMap[mainTabData->index].Num();
	}
	
	return 0;
}

int32 FGsRankingManager::GetNumGroupTabs(int32 InMainTabIndex, int32 InSubTabIndex)
{
	const FGsSchemaRankingTypeObjective* subTabData = GetSubTabData(InMainTabIndex, InSubTabIndex);
	if (nullptr == subTabData)
		return 0;

	const FGsSchemaRankingGroupCategory* groupTabData = GetGroupTabData(subTabData->index);
	if (nullptr == groupTabData)
		return 0;

	return groupTabData->rankingGroupData.Num();
}

const TArray<const FGsSchemaRankingCategory*>& FGsRankingManager::GetMainTabDataList()
{
	return _mainTabDataList;
}

bool FGsRankingManager::HasRankingRequest(RankType InRankingType, WorldId InWorldId, int32 InMinRank)
{
	TPair<RankType, WorldId> rankingKey(InRankingType, InWorldId);
	if (_currentRankingRequestInfo.Contains(rankingKey))
	{
		const TSet<int32>& requestInfo = _currentRankingRequestInfo[rankingKey];
		return requestInfo.Contains(InMinRank);
	}
	return false;
}

bool FGsRankingManager::HasMyRanking(RankType InRankingType)
{
	return _currentMyRankingMap.Contains(InRankingType);
}

TArray<const FGsRankingData*> FGsRankingManager::GetRankingList(RankType InRankingType, WorldId InWorldId, int32 InMinRank, int32 InMaxRank)
{
	TArray<const FGsRankingData*> rankingList;

	TPair<RankType, WorldId> rankingKey(InRankingType, InWorldId);
	if (_currentRankingMap.Contains(rankingKey))
	{
		const TMap<int32, FGsRankingData>& rankingMap = _currentRankingMap[rankingKey];
		for (int32 rank = InMinRank; rank <= InMaxRank; ++rank)
		{
			if (rankingMap.Contains(rank))
			{
				rankingList.Emplace(&rankingMap[rank]);
			}
		}
	}
	return rankingList;
}

const FGsSchemaRankingReward* FGsRankingManager::GetRewardData(int32 InObjectiveId)
{
	for (TPair<RankType, const FGsSchemaRankingReward*> rewardData : _rewardDataMap)
	{
		if (const FGsSchemaRankingTypeObjective* subTabData = rewardData.Value->rankingTypeObjectiveID.GetRow())
		{
			if (subTabData->index == InObjectiveId)
			{
				return rewardData.Value;
			}
		}
	}
	return nullptr;
}

const FGsSchemaRankingReward* FGsRankingManager::GetRewardDataByRankType(RankType InRankingType)
{
	if (_rewardDataMap.Contains(InRankingType))
	{
		return _rewardDataMap[InRankingType];
	}

	return nullptr;
}

FGsRankingMyData* FGsRankingManager::GetMyRanking(RankType InRankingType)
{
	if (_currentMyRankingMap.Contains(InRankingType))
	{
		return &_currentMyRankingMap[InRankingType];
	}
	return nullptr;
}

EGsRankingDataType FGsRankingManager::GetRankingDataType(RankType InRankingType)
{
	if (InRankingType == RankType::KNIGHTS_DUNGEON_CLEAR_RANK)
	{
		return EGsRankingDataType::GuildDungeon;
	}

	if (const FGsSchemaRankingTypeObjective* subTabData = GetSubTabData(InRankingType))
	{
		if (RankSearchType::PERSONAL == subTabData->rankingSearchType)
		{
			return (RankServerRangeType::LOCAL == subTabData->serverRangeType) ? EGsRankingDataType::Local : EGsRankingDataType::World;
		}
		else if (RankSearchType::KNIGHTS == subTabData->rankingSearchType)
		{
			return EGsRankingDataType::Guild;
		}
	}
	
	return EGsRankingDataType::MAX;
}

bool FGsRankingManager::IsLevelDataType(RankType InRankingType)
{
	switch (InRankingType)
	{
	case RankType::SERVER_LEVEL_RANK:
		return true;
	case RankType::ALL_SERVER_LEVEL_RANK:
		return true;
	}

	return false;
}

void FGsRankingManager::SetCurrentTabData(int32 InMainIndex, int32 InSubIndex, int32 InGroupIndex, const FGsSchemaWorldData* InSelectedServer)
{
	_mainTabIndex = InMainIndex;
	_subTabIndex = InSubIndex;
	_groupTabIndex = InGroupIndex;
	_selectedServer = InSelectedServer;
}