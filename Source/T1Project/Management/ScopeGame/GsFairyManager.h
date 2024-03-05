// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "Classes/GsManager.h"
#include "Management/GsScopeHolder.h"
#include "Fairy/GsBasePortrait.h"
#include "Fairy/GsFairyCollection.h"
#include "Classes/Collection/GsCollectionFilter.h"
#include "DataSchema/Fairy/GsSchemaFairySummonChangeCurrencyInfo.h"
#include "Message/GsMessageFairy.h"
#include "Message/GsMessageContentHud.h"
#include "EventProgress/GsEventProgressInterface.h"
#include "Util/GsTimeSyncUtil.h"

#include "GsFairyManager.generated.h"

/**
 * Fairy관련 Data Manager (Table, 보유 페어리, 페어리 콜렉션)
 */

struct FGsFairyData;
struct FairyCollection;
struct FGsFairySummonConfirmData;
struct FGsSchemaFairyEnchant;
struct FGsFairyMsgParamChangeTab;
struct IGsMessageParam;

class FGsFairyItem;
class FGsFairyCollection;
class FGsFairyCollectionComplete;
class URichTextBlock;

using FairyCollectionFilter = IGsCollectionFilter<FGsFairyCollection>;
using FairyCollectionReward = CollectionCompleteReward<FGsFairyCollection, PassivityOwnerType::FAIRY_COLLECTION>;
using FairySearchType		= FairyCollectionFilter::SearchType;
using FairySearchType_under = int8; // std::underlying_type<FairySearchType>::type;

#define INVALID_FAIRY_LEVEL (uint32)(~((uint32)0))

constexpr FairySearchType_under toUnderSearchType(FairySearchType e)
{
	return static_cast<FairySearchType_under>(e);
}

UCLASS()
class T1PROJECT_API UGsFairyManager
	: public UObject
	, public IGsManager
	, public IGsEventProgressInterface
	, public IGsCollectionDataContainer
	, public FairyCollectionFilter
{
	GENERATED_BODY()

public:
	using FairyCollectConditionList = TArray<TWeakPtr<FGsFairyCollection>>;
	using MsgActFairyHandle = TPair<MessageFairy, FDelegateHandle>;

	using GradeId = uint32;

	static const FString HYPERLINK_ID;

protected:
	TArray<TSharedPtr<FGsFairyItem>> _fairyItem; //보유 페어리 정보
	TWeakPtr<FGsFairyItem> _equipFairy; //착용 페어리 정보

	TArray<TSharedPtr<FGsFairySummonConfirmData>> _fairySummonWaitings; //소환 확정 대기 페어리 정보
	TMap<GradeId, TArray<FGsSchemaFairySummonChangeCurrencyInfo>> _fairySummonChangeData; //소환 교체 비용 

	const struct FGsSchemaVehicleConfig* _fairyVehicleConfigTable = nullptr;	// 페어리 탈것 연관 정보 테이블 

	bool _isFairyRedDot{ false };
	bool _isFairyCollectionRedDot{ false };

protected:
	TArray<TSharedPtr<FGsFairyData>> _fairyDictionaryData; //sorted Fairy Page
	TMap<FairyId, TWeakPtr<FGsFairyData>> _fairyDictionaryList; //id 검색용

	TArray<TSharedPtr<FGsFairyCollection>> _fairyCollectionData; //Fairy Collection Page
	TMap<CollectionId, TWeakPtr<FGsFairyCollection>>  _fairyCollectionList; //collection id로 검색
	TMap<CollectionConditionId, FairyCollectConditionList> _fairyConditionList; //condition id로 해당 collection들을 모두 검색
	TMap<FairyId, FairyCollectConditionList> _fairyIdCollectionList;

	// 2024/1/17 PKT - 해당 컨텐츠에 속하는 모든 Stat 정보
	mutable TSet<StatType> _contentsStatAll;

	FairyCollectionReward _completeRewards;

protected:
	MsgActFairyHandle  _actMsgHandler;
	MsgHudHandle _hudMsgHandler;

	bool _isRunningSummon{ false };

	TSharedPtr<FGsFairyMsgParamChangeTab> _reserveTab;

	bool _isNotifyRewarded{ false };
	TMap<ItemId, ItemAmount> _reserveComposeRewards;

protected:
	TArray<FairyId> _summonList;

	enum class NotiProc { None, StatNoti, CmdNoti };
	NotiProc _statNotify{ NotiProc::None };

public:
	void Initialize() override;
	void Finalize() override;

public:
	const TArray<TSharedPtr<FGsFairyData>>& GetFairyDictionary() const { return _fairyDictionaryData; }
	const TWeakPtr<FGsFairyData> GetFairyData(FairyId fairyId) const;

	int32 GetGradeFairyDictionary(FairyGrade grade, TArray<TWeakPtr<FGsFairyData>>& list, bool equipApply = false);

	const TArray<TSharedPtr<FGsFairyCollection>>& GetFairyCollection() const { return _fairyCollectionData; }
	const TWeakPtr<FGsFairyCollection> GetFairyCollectionData(CollectionId collectionId);
	int GetOwnItemCollectionList(OUT TArray<TWeakPtr<FGsFairyCollection>>& collections, FairyId fairyId, FairyLevelId lv = INVALID_FAIRY_LEVEL) const;

	const FGsSchemaFairyEnchant* GetFairyEnchantData(FairyId fairyId, FairyLevelId level = INVALID_FAIRY_LEVEL) const;

	TWeakPtr<FGsFairyItem> GetEquipFairyData() { return _equipFairy; }
	const struct FGsSchemaVehicleConfig* GetVehicleConfigTable();

	const TArray<TSharedPtr<FGsFairySummonConfirmData>>& GetFairySummonWaitingList() { return _fairySummonWaitings; }
	
	virtual const class IGsCollectionData* GetCollectionData() const override { return &_completeRewards; }

	FairyId GetFairyId(PresetId InPresetId);

	TSet<StatType>& ContentsStatAll() const;

public:
	bool IsRedDot() { return _isFairyRedDot || _isFairyCollectionRedDot || IsFairySummonItemRedDot(); }

	bool IsFairyRedDot() { return _isFairyRedDot; }
	bool IsFairyCollectionRedDot() { return _isFairyCollectionRedDot; }
	bool IsFairySummonItemRedDot() { return GetFairySummonWaitingList().Num() > 0; }
	void DisableFairyRedDot() { _isFairyRedDot = false; }
	void DisableFairyCollectionRedDot() { _isFairyCollectionRedDot = false; }
	bool IsEffectPlaying() { return _isRunningSummon; }
	void SetEffectPlaying(bool run = true, bool saveCollectionStat = false);

	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;

public:
	void FairyEquipChange(FairyId fairyID = 0);

	void InitFairyItemList();
	void UpdateFairyItemList(TArray<FairyData>& arr, bool isComplete = false, bool isNewAdd = false);
	void UpdateAmountFairyItemList(TArray<FairyIdAmountPair>& arr);

	void UpdateFairyDatatoItem(const uchar* rawData, int32 size, bool isComplete = false);
	void UpdateFairySummonWaitingItemList(TArray<FairyConfirmData> list, FDateTime nowTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC(), bool isUpdate = false);
	void ChangeFairySummonWaitingItem(FairyConfirmData data, FDateTime nowTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC());
	void RemoveFairySummonWaitingItem(FairyConfirmId waitingId);

	void UpdateFairyItem(FairyData data, bool isNewAdd = false);
	void RemoveFairyItem(FairyId id);

	void InitFairyCollectionList();
	void UpdateFairyCollectionCompleteList(const TArray<CollectionId>& arr, bool isUpdate = false);
	void UpdateFairyCollectionCompleteConditionList(const TArray<CollectionConditionId>& arr, bool isUpdate = false);

	void OnAckEnchantResult(FairyId id, FairyEnchantResult result);
	void OnAckCeilingResult(bool success);
	void AckFairyComposeRewardList(TArray<ItemIdAmountPair>& rewardItems);

	void OnUpdateObtainedFairyList(FairyObtainReason obtainReason, TArray<FairyId>& resultList);

	bool OnSummonStart(FairyObtainReason obtainReason, TArray<FairyId>& resultList);
	void OnSummonComplete(const IGsMessageParam* InParam);

	bool OnProcessError(PD::Result res);

	void OnFairyStateSelectedPage(int32 tab, int32 optData = 0);

	bool IsEnableStackFairy(FairyGrade grade = FairyGrade::MAX);
	int32 GetRemainStackCountFairy();
	bool CheckComposeStackableFairy(TMap<FairyId, ItemAmount>& fairyList);

public:
	void ReserverStatNotify(NotiProc proc);
	void CollecitonStatNotify();
	void OnInvalidCombatPower();

protected:
	void InitFairyTable();

	int GetCollection(TArray<TWeakPtr<FGsFairyCollection>>& list) const override;
	void OpenCollectionTicker(TArray<CollectionId> collections);
	void OpenReservedReward();

public:
	void RegisterHyperLinkDelegate(URichTextBlock* InRichTextBlock);
	FText MakeRichText(IGsBasePortrait::PortraitType type, int32 detailId) const;

protected:
	void OnClickHyperLink(const TMap<FString, FString>& InParamMap);
};

#define GSFairy() UGsScopeHolder::GetGameManager<UGsFairyManager>(UGsScopeGame::EManagerType::Fairy)