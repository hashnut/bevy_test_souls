// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Management/GsScopeHolder.h"
#include "Ranking/GsRankingData.h"

struct FGsSchemaRankingReward;
struct FGsSchemaRankingCategory;
struct FGsSchemaRankingTypeObjective;
struct FGsSchemaRankingGroupCategory;
struct FGsRankingData;
struct FGsSchemaWorldData;
/**
 *  랭킹 시스템 패킷 데이터 및 테이블 데이터 관리 매니저
 */
class T1PROJECT_API FGsRankingManager final : public IGsManager
{
private:
	// 메인탭(상단탭) 데이터 목록
	TArray<const FGsSchemaRankingCategory*> _mainTabDataList;
	// (메인탭 ID, 서브탭 목록) 맵
	TMap<int32, TArray<const FGsSchemaRankingTypeObjective*>> _subTabDataListMap;
	// (서브탭 ID, 서브탭 보상 정보) 맵
	TMap<RankType, const FGsSchemaRankingReward*> _rewardDataMap;

	TMap<TPair<RankType, WorldId>, TMap<int32, FGsRankingData>> _currentRankingMap;
	TMap<TPair<RankType, WorldId>, TSet<int32>> _currentRankingRequestInfo;
	TMap<RankType, FGsRankingMyData> _currentMyRankingMap;
	
	int32 _mainTabIndex;
	int32 _subTabIndex;
	int32 _groupTabIndex;
	const FGsSchemaWorldData* _selectedServer = nullptr;
	bool _isSyncing = false;
	bool _isFirstOpen = false;	// 랭킹화면 첫 진입 플래그

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

public:
	// 랭킹 정보 요청 응답
	void Set(PD::GC::PKT_GC_ACK_RANK_READ& Packet);
	void ReleaseRankingData();

	void RequestRankingData(RankType InRankType, WorldId InWorldId, int32 InMinRank);

private:
	// 랭킹 탭 정보 캐싱
	void InitMainTabData();
	void InitSubTabData();
	void InitRewardData();

public:
	int32 GetNumMainTabs();
	int32 GetNumSubTabs(int32 InMainTabIndex);
	int32 GetNumGroupTabs(int32 InMainTabIndex, int32 InSubTabIndex);

public:
	const TArray<const FGsSchemaRankingCategory*>& GetMainTabDataList();
	const FGsSchemaRankingReward* GetRewardData(int32 InObjectiveId);
	const FGsSchemaRankingReward* GetRewardDataByRankType(RankType InRankingType);

public:
	const FGsSchemaRankingCategory* GetMainTabData(int32 InIndex);
	const FGsSchemaRankingTypeObjective* GetSubTabData(int32 InMainTabIndex, int32 InSubTabIndex);
	const FGsSchemaRankingTypeObjective* GetSubTabData(RankType InRankingType);
	const FGsSchemaRankingGroupCategory* GetGroupTabData(int32 InObjectiveId);
	const FGsSchemaRankingTypeObjective* GetCurrentSubTabData();

public:
	bool HasRankingRequest(RankType InRankingType, WorldId InWorldId, int32 InMinRank);
	bool HasMyRanking(RankType InRankingType);
	TArray<const FGsRankingData*> GetRankingList(RankType InRankingType, WorldId InWorldId, int32 InMinRank, int32 InMaxRank);
	FGsRankingMyData* GetMyRanking(RankType InRankingType);

public:
	EGsRankingDataType GetRankingDataType(RankType InRankingType);
	bool IsLevelDataType(RankType InRankingType);

public:
	void SetIsSyncing(bool InIsSyncing) { _isSyncing = InIsSyncing; }
	bool IsSyncing() { return _isSyncing; }

	void SetIsFirstOpen(bool InIsFirst) { _isFirstOpen = InIsFirst; }
	bool IsFirstOpen() { return _isFirstOpen; }

	void SetCurrentTabData(int32 InMainIndex, int32 InSubIndex, int32 InGroupIndex, const FGsSchemaWorldData* InSelectedServer);
	int32 GetMainTabIndex() { return _mainTabIndex; }
	int32 GetSubTabIndex() { return _subTabIndex; }
	int32 GetGroupTabIndex() { return _groupTabIndex; }
	const FGsSchemaWorldData* GetSelectedServer() { return _selectedServer; }
	
};

#define GSRanking() UGsScopeHolder::GetGameManagerFType<FGsRankingManager>(UGsScopeGame::EManagerType::Ranking)