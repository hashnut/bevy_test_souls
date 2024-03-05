#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Classes/GsManager.h"
#include "Classes/Collection/GsCollectionFilter.h"
#include "EventProgress/GsEventProgressInterface.h"

#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "Costume/GsCostumeCollection.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Costume/GsSchemaCostumeSummonChangeCurrencyInfo.h"

#include "Management/GsScopeHolder.h"
#include "Message/GsMessageCostume.h"
#include "Message/GsMessageContentHud.h"
#include "Message/MessageParam/GsCostumeMessageParam.h"
#include "Util/GsTimeSyncUtil.h"
#include "EventProgress/GsEventProgressDefine.h"

#include "GsCostumeManager.generated.h"
/**
 * Costume 관련 Data Manager (Table, 보유 코스츔, 코스츔 콜렉션)
 */
class FGsCostumeItem;

struct FGsCostumeData;
struct FGsCostumeSummonConfirmData;
struct IGsMessageParam;

struct FGsSchemaItemEffectChangeCostume;
struct FGsSchemaWeaponTypeMenu;

using CostumeCollectionFilter = IGsCollectionFilter<FGsCostumeCollection>;
using CostumeCollectionReward = CollectionCompleteReward<FGsCostumeCollection, PassivityOwnerType::COSTUME_COLLECTION>; 
using CostumeSearchType = CostumeCollectionFilter::SearchType;
using CostumeSearchType_under = int8; // std::underlying_type<CostumeSearchType>::type;

struct FGsCostumeWeaponMenuEntry
{
	CreatureWeaponType _menuType;
	FText _menuName;
	FSoftObjectPath _iconPath{ FSoftObjectPath() };
	FVector2D	 _iconImgSize{ FVector2D::ZeroVector };
	FLinearColor _iconTintColor{ FLinearColor::Transparent };
	uint8 _order;
};

UCLASS()
class T1PROJECT_API UGsCostumeManager : 
	public UObject, 
	public IGsManager, 
	public IGsCollectionDataContainer, 
	public CostumeCollectionFilter, 
	public IGsEventProgressInterface
{
	GENERATED_BODY()

public:
	using CostumeCollectConditionList = TArray<TWeakPtr<FGsCostumeCollection>>;
	using MsgActCostumeHandle = TPair<MessageCostume, FDelegateHandle>;

protected:
	TWeakPtr<FGsCostumeItem> _equipCostume; //착용 코스츔 정보
	TWeakPtr<FGsCostumeItem> _appearanceCostume; //현재 외형 코스춤
	SkillId _equipCostumeActiveSkillId{ 0 };
	
	TMap<CostumeId, TWeakPtr<FGsCostumeData>> _costumeDictionaryList; //코스춤 데이터
	TMap<CollectionId, TWeakPtr<FGsCostumeCollection>>  _costumeCollectionList; //collection 데이터
	TMap<CollectionConditionId, CostumeCollectConditionList> _costumeConditionList; //condition별 collection List
	TMap<CostumeId, CostumeCollectConditionList> _costumeIdCollectionList; //코스춤 Id별 Collection List

	CostumeCollectionReward _completeRewards;

	mutable TSet<StatType> _contentsStatAll;

	TArray<FGsCostumeWeaponMenuEntry> _constumeMenus;

private:
	bool _isCostumeRedDot{ false };
	bool _isCostumeCollectionRedDot{ false };

	TArray<TSharedPtr<FGsCostumeItem>> _costumeItems; //보유 코스츔 정보
	TArray<TSharedPtr<FGsCostumeData>> _costumeDictionaryData; //costume data
	TArray<TSharedPtr<FGsCostumeCollection>> _costumeCollectionData; //collection
	TArray<TSharedPtr<FGsCostumeSummonConfirmData>> _summonWaitings; //소환 확정 대기 코스춤 정보

	TMap<CostumeGrade, TArray<FGsSchemaCostumeSummonChangeCurrencyInfo>> _summonChangeData; //소환 교체 비용 

protected:
	MsgActCostumeHandle _actMsgHandler;
	MsgHudHandle _hudMsgHandler;

	bool _isRunningSummon{ false };

	bool _isNotifyRewarded{ false };
	TMap<ItemId, ItemAmount> _reserveComposeRewards;

protected:
	TArray<FairyId> _summonList;

	enum class NotiProc { None, StatNoti, CmdNoti };
	NotiProc _statNotify{ NotiProc::None };

public:
	void Initialize() override;
	void Finalize() override;

	const TArray<TSharedPtr<FGsCostumeData>>& GetCostumeDictionary() const { return _costumeDictionaryData; }
	const TArray<TSharedPtr<FGsCostumeCollection>>& GetCostumeCollection() const { return _costumeCollectionData; }
	const TArray<TSharedPtr<FGsCostumeSummonConfirmData>>& GetSummonWaitingList() { return _summonWaitings; }

	const TWeakPtr<FGsCostumeData> GetCostumeData(CostumeId costumeId) const;
	const TWeakPtr<FGsCostumeCollection> GetCollectionData(CollectionId id);

	const TArray<TWeakPtr<FGsCostumeCollection>>& GetOwnItemCollectionList(CostumeId costumeId);

	void AckCostumeComposeRewardList(TArray<ItemIdAmountPair>& rewardItems);
	int32 GetGradeCostumeDictionary(CostumeGrade grade, TArray<TWeakPtr<FGsCostumeData>>& list, bool equipApply = false);
	int32 GetGradeCostumeDictionary(CostumeGrade grade, CreatureWeaponType selectWeapon, TArray<TWeakPtr<FGsCostumeData>>& list, bool equipApply = false);
	int32 GetGradeAppearenceCostume(CostumeGrade grade, TArray<TWeakPtr<FGsCostumeData>>& list);

	TWeakPtr<FGsCostumeData> GetWearOnCostumeData();
	SkillId GetEquipCostumeActiveSkillId() { return _equipCostumeActiveSkillId; }

	CostumeId GetCostumeId(PresetId InPresetId);

	bool IsRedDot() { return _isCostumeRedDot || _isCostumeCollectionRedDot || IsCostumeSummonItemRedDot() ; }
	bool IsCostumeRedDot() { return _isCostumeRedDot; }
	bool IsCostumeCollectionRedDot() { return _isCostumeCollectionRedDot; }
	bool IsCostumeSummonItemRedDot() { return GetSummonWaitingList().Num() > 0; }
	bool IsEffectPlaying() { return _isRunningSummon; }
	void SetEffectPlaying(bool run = true, bool saveCollectionStat = false);
	
	void DisableCostumeRedDot() { _isCostumeRedDot = false; }
	void DisableCostumeCollectionRedDot() { _isCostumeCollectionRedDot = false; }

	virtual const class IGsCollectionData* GetCollectionData() const override { return &_completeRewards; }

	const TArray<FGsCostumeWeaponMenuEntry>& GetCostumeSelectMenuData();

public:
	void CostumeEquipChange(CostumeId costumeId = 0);
	void AppearanceChange(CostumeId costumeId = 0, int equipType = 0);

	void InitCostumeItemList();
	void UpdateCostumeItemList(TArray<CostumeData>& arr, bool isComplete = false, bool isNewAdd = false);
	void UpdateAmountCostumeItemList(TArray<CostumeIdAmountPair>& arr, bool isNewAdd = false);
	void UpdateCostumeDatatoItem(const uchar* rawData, int32 size, bool isComplete = false);

	void UpdateCostumeItem(CostumeData& data, bool isNewAdd = false);
	void RemoveCostumeItem(CostumeId costumeId, bool isUpdate = false);
	void SetCostumeItemIsLearnSkill(CostumeId costumeId, bool isLearn);

	void InitCollectionList();
	void UpdateCollectionCompleteList(const TArray<CollectionId>& arr, bool isUpdate = false);
	void UpdateCollectionCompleteResetList(const TArray<CollectionId>& arr, bool isUpdate = true);
	void UpdateCollectionCompleteConditionList(const TArray<CollectionConditionId>& arr, bool isUpdate = false);
	void RemovedCollectionCompleteConditionList(const TArray<CollectionConditionId>& arr, bool isUpdate = false);

	void UpdateCostumeSummonWaitingItemList(TArray<CostumeConfirmData> list, FDateTime nowTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC(), bool isUpdate = false);
	void ChangeCostumeSummonWaitingItem(CostumeConfirmData data, FDateTime nowTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC());
	void RemoveCostumeSummonWaitingItem(CostumeConfirmId waitingId);

	void UpdateObtainedCostumeList(CostumeObtainReason obtainReason ,TArray<CostumeId>& resultList);
	void OnAckCeilingResult(bool success);
	void OnAckExchangeCostume(CostumeId id);

	bool OnSummonStart(CostumeObtainReason obtainReason, TArray<CostumeId>& resultList);
	void OnSummonComplete(const IGsMessageParam* InParam);
	void OpenCollectionTicker(TArray<CollectionId> collections);
	void OpenReservedReward();

	bool IsEnableStackCostume(CostumeGrade grade = CostumeGrade::MAX);

	int GetRemainStackCountCostume();
	bool CheckComposeStackableCostume(TMap<CostumeId, ItemAmount>& costumeList);

public:
	void ReserverStatNotify(NotiProc proc);
	void CollecitonStatNotify();
	void OnInvalidCombatPower();

public:
	TSharedPtr<FGsCostumeMsgParamExchangeCostume> _eventParam;
	// 이벤트 시작
	virtual void StartEvent(EGsEventProgressType InStartType, EGsEventProgressType InPreType) override;
	// 이벤트 종료
	virtual void FinishEvent(EGsEventProgressType InFinishType, EGsEventProgressType InNextType) override;

	void OnCostumeExchangeStateSelected(const FGsItem* item);

	TSet<StatType> GetContentsStatAll() const;
public:
	static bool FindCostumeExchangeTicketEffectData(ItemId itemId, const FGsSchemaItemEffectChangeCostume*& effectData);
	
public:
	bool OnProcessError(PD::Result res);

protected:
	void InitCostumeTable();

protected:
	int GetCollection(TArray<TWeakPtr<FGsCostumeCollection>>& list) const override;
};

#define GSCostume() UGsScopeHolder::GetGameManager<UGsCostumeManager>(UGsScopeGame::EManagerType::Costume)