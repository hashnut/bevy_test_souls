#pragma once

#include "Classes/GsManager.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Message/GsMessageContents.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsCostPackage;
class FGsSealNode;
class FGsSealPack;
class FGsSealSlot;
struct FGsSchemaSealNodeLevel;
struct FGsSchemaSealNodeRerollGain;
struct FGsSchemaSealSlotRerollGainColorPool;
struct FGsSchemaSealSlotRerollGainStatPool;
struct IGsMessageParam;

//typedef int32 SealPackId;
#define INVALID_SEAL_PACK_ID 0

/**
 *	봉인 매니저
 */
class FGsSealManager final :
	public IGsManager
{
public:
	using SealPackGainColorId = int32;
	using MatchCount = int32;

private:
	TMap<int32, TSet<const FGsSchemaSealSlotRerollGainColorPool*>> _collectedRerollGainColorMap;
	TMap<int32, TSet<const FGsSchemaSealSlotRerollGainStatPool*>> _collectedRerollGainStatMap;
	
	/** 봉인팩의 획득 스탯 모음 */
	TMap<SealPackGainColorId, TMap<SealSlotColor, TMap<MatchCount, TArray<TPair<StatType, StatValue>>>>> _collectedSealPackGainStatMap;

private:
	TArray<TPair<MessageContentSeal, FDelegateHandle>> _sealDelegates;

	TMap<SealPackId, TSharedRef<FGsSealPack>> _sealPackMap;
	TArray<SealPackId> _sealPackList;

	TMap<SealId, TSharedRef<FGsSealNode>> _sealNodeMap;

	TMap<SealSlotId, TSharedRef<FGsSealSlot>> _sealSlotMap;

	/** 언락 컨텐츠 Id 와 SealPackId 매핑 */
	TMap<UnlockContentsId, SealPackId> _unlockIdToSealPackId;

	/************************************************************************/
	/* 인스턴싱																*/
	/************************************************************************/

public:
	void Initialize() final;
	void Finalize() final;
    
private:
    bool InitliazeData();
	void RegisterMessages();
	void UnegisterMessages();

public:
	TWeakPtr<const FGsSealPack> GetSealPack(const SealPackId InSealPackId) const;
	TWeakPtr<const FGsSealNode> GetSealNode(const SealId InSealNodeId) const;

	TWeakPtr<const FGsSealPack> GetSealPackWithIndex(const uint8 InIndex) const;
	int32 GetSealPackIndexWithId(const SealPackId InSealPackId) const;
	int32 GetTotalSealPackCount() const;

public:
	bool CanDisplay(const SealPackId InSealPackId) const;
	bool CanSealNow(const SealId InSealNodeId) const;

public:
	/** [패킷] 서버에서 봉인 데이터 수신 */
	void Seal(const IGsMessageParam* InParam);

	/** [패킷] 서버에서 봉인 슬롯 잠금 데이터 수신 */
	void SealSlotLock(const IGsMessageParam* InParam);

	/** [패킷] 서버에서 봉인 레벨업 응답 수신 */
	void AckSealLevelUp(const IGsMessageParam* InParam);

	/** [패킷] 서버에서 봉인 리롤 응답 수신 */
	void AckSealSlotChangeAll(const IGsMessageParam* InParam);

	/** [패킷] 서버에서 봉인 슬롯 잠금 데이터 저장 응답 수신 */
	void AckSealSlotLockSave(const IGsMessageParam* InParam);

	/** [메세지] 특정 봉인 슬롯의 리롤언락을 잠금/해제 요청 */
	void TrySetRerollSealSlotUnlock(const IGsMessageParam* InParam);

	/** [메시지] 특정 카테고리 열렸을 때 언락 데이터 갱신 */
	bool OnUnlockCategory(UnlockContentsId InContentsId);

private:
	void SetSealNodeDatas(const TArray<SealData>& InSealNodeDatas);

	void SetSealNodeData(const SealData& InSealNodeData);

	void SetSealSlotDatas(const TArray<SealSlotData>& InSealSlotDatas);

public:
	void TempResetAllSealNodeSelections();

public:
	/** UI 오픈 등 시에 자동으로 활성화할 봉인팩ID로, 기본적으로 테이블 최상단의 봉인팩ID를 활용함. */
	static SealPackId GetDefaultSealPackId();

	/** 봉인색상을 등급 텍스트로 변환 */
	static FText MakeSealGradeText(const SealSlotColor InSealSlotColor);

	/** 봉인팩 인스턴스 생성 시, 이 함수로 하위 봉인 노드 목록을 생성하여 보유함. */
	TArray<TWeakPtr<FGsSealNode>> MakeInitSealNodes(const SealPackId InSealPackId) const;

	/** 봉인팩획득테이블 목록 */
	const TMap<FGsSealManager::MatchCount, TArray<TPair<StatType, StatValue>>>* GetSealPackAllGainsInColor(FGsSealManager::SealPackGainColorId InSealPackGainColorId, SealSlotColor InLineColor);

	/** 봉인노드 연결선 획득 스탯 정보 조회 */
	const TArray<TPair<StatType, StatValue>>* GetSealPackGainStats(FGsSealManager::SealPackGainColorId InSealPackGainColorId, SealSlotColor InLineColor, FGsSealManager::MatchCount InLineCount);

	/** 봉인슬롯 저장 */
	void SetSealSlotData(const SealSlotId InSlotId, TSharedRef<FGsSealSlot> InSealSlot);

	/** SealSlotId 로 봉인슬롯 조회 */
	TSharedRef<FGsSealSlot> GetSealSlotData(const SealSlotId InSlotId);

	/** 봉인노드레벨정보 찾기 */
	static const FGsSchemaSealNodeLevel* FindSealNodeLevel(int32 InSealNodeLevelId, Level InSealNodeLevel);

	/** 봉인노드레벨의 모든 레벨 정보 수집 */
	static TMap<Level, TArray<TPair<StatType, StatValue>>> CollectSealNodeStatInfoAllLevel(int32 InSealNodeLevelId);

	/** 봉인노드의 모든 레벨의 최소/최대값을 계산함. */
	static TRange<Level> FindSealNodeAllLevelRange(int32 InSealNodeLevelId);

	/** 봉인노드레벨의 획득 스탯 목록 조회 */
	static TArray<TPair<StatType, StatValue>> FindSealNodeLevelGainStats(int32 InSealNodeLevelGainId);

	/** 봉인슬롯 색상별 획득 비율 */
	//static TArray<TPair<SealSlotColor, int32>> MakeSealNodeRerollColorShare(SealId InSealNodeId, SealSlotId InSealSlotId = INVALID_SEAL_SLOT_ID, Level InSealNodeLevel = INVALID_LEVEL);

	/** 봉인슬롯 스탯별 획득 비율. 스탯, 스탯 */
	//static TArray<TTuple<StatType, StatValue, int32>> MakeSealNodeRerollStatShare(SealId InSealNodeId, SealSlotId InSealSlotId = INVALID_SEAL_SLOT_ID, Level InSealNodeLevel = INVALID_LEVEL);

	/** 리롤 시 봉인노드의 모든 레벨별 획득 색상/스탯의 확률 등 데이터 수집. */
	TMap<Level, TPair<TArray<TPair<SealSlotColor, int32>>, TArray<TTuple<StatType, StatValue, int32>>>> CollectSealRerollGainsSharePerLevel(SealId InSealNodeId);

private:
	/** 특정 봉인노드의 모든 리롤획득ID를 수집 */
	static TSet<int32> CollectAllRerollGainIdSet(SealId InSealNodeId, SealSlotId InSealSlotId = INVALID_SEAL_SLOT_ID);

	/** 봉인노드의 지정된 혹은 모든 슬롯의 리롤획득데이터를 수집 */
	static TSet<const FGsSchemaSealNodeRerollGain*> CollectSealNodeRerollGainDataSet(SealId InSealNodeId, SealSlotId InSealSlotId = INVALID_SEAL_SLOT_ID, Level InSealNodeLevel = INVALID_LEVEL);

	const TMap<int32, TSet<const FGsSchemaSealSlotRerollGainColorPool*>>& CollectedRerollGainColorMap();

	const TMap<int32, TSet<const FGsSchemaSealSlotRerollGainStatPool*>>& CollectedRerollGainStatMap();

	/** 봉인노드 연결선 획득 스탯 정보 수집 */
	static const TMap<FGsSealManager::SealPackGainColorId, TMap<SealSlotColor, TMap<FGsSealManager::MatchCount, TArray<TPair<StatType, StatValue>>>>> CollectSealPackGainStats();
};

#define GSSeal() UGsScopeHolder::GetGameManagerFType<FGsSealManager>(UGsScopeGame::EManagerType::Seal)
