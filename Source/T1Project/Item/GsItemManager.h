#pragma once


#include "GsItem.h"
#include "Net/GSNet.h"
#include "GsItemBuffers.h"

#include "Classes/GsManager.h"

#include "GameObject/Define/EGsPartsType.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "GameObject/Define/GsGameObjectDefine.h"

#include "DataSchema/Item/GsSchemaItemEquipDetail.h"

#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Define/EGsIngredientUse_UIType.h"

#include "Management/GsScopeGlobal.h"
#include "Management/GsScopeHolder.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageSystem.h"

#include "Item/Data/GsItemDeliveryBoxData.h"

#include "GsItemManager.generated.h"


class FGsCostPackage;
class FGsCostPackageMultiple;
class FGsInputBlockHandler;
class FGsItemEquipEnchantLoader;
class FGsItemIngredient;
class UGsUIIconItem;

struct FGsSchemaPartsShapeData;
struct FGsItemWeightPacketData;
struct FGsItemEquipEnchantBalance;
struct FGsItemEquipEnchantBonus;
struct ItemDBIdAmountPair;

/** 비정상 아이템그룹ID */
#define INVALID_ITEM_GROUP_ID 0

enum class EGsSortItemDisposal
{
	/** 기본 정렬로서 가장 효율적인 처분 순서로 정렬함 */
	Efficiency,

	/** 별도 규칙을 제외하고, 사용자 직접 정렬 */
	UserSpecific,

	/** 제작 대체 아이템 순서 지정을 위함. 거래 가능 아이템을 먼저 띄워준다 */
	CraftMaterial,
};

/**
 *  모든 Item 관리하는 Manager
 */
UCLASS()
class T1PROJECT_API UGsItemManager :
	public UObject,
	public IGsManager
{
	GENERATED_BODY()

public:
	// 한 틱에 다량의 장착 아이템 습득 시, 분할 처리 될 갯수
	const uint8 GET_MAX_EQUIP_ITEM_COUNT = 50;
	// 추후 동일 악세사리에 대해 인덱스가 늘어날 시, 수정 필요
	const uint8 ACC_SLOT_MAX_COUNT = 2; 

public:
	enum SetNumType : uint8
	{
		None = 0,
		Set2,
		Set3,
		Set4,
		Set5,
		Set6,
		Set7,
		Set8,

		SetMax,
	};

private:
	/** 모든 세트아이템 정보 */
	struct FGsSetItemInfo
	{
		const int32 _setId;

		/** 대표 세트 아이템 목록 */
		const TArray<ItemId> _defaultItemIds;

		/** 세트 가능한 모든 아이템ID */
		const TSet<ItemId> _allItemIds;

		/** 세트 갯수 별 스탯 정보 */
		const TMap<SetNumType, TArray<FGsItemStatData>> _statsPerCount;

		FGsSetItemInfo(int32 InSetId, TArray<ItemId> InDefaultItemIds, TSet<ItemId> InAllItemIds, TMap<SetNumType, TArray<FGsItemStatData>> InStatsPerCount)
			: _setId(InSetId)
			, _defaultItemIds(MoveTemp(InDefaultItemIds))
			, _allItemIds(MoveTemp(InAllItemIds))
			, _statsPerCount(MoveTemp(InStatsPerCount))
		{}
	};

private:
	// 인벤토리 아이템 buffers
	FGsItemBuffers* _itemBuffers = new FGsItemBuffers();

	// 복구 아이템 데이터
	FGsItemDeliveryBoxData _itemDeliveryBoxData;

private:
	// 프리셋ID 별 장착중인 아이템 DBID
	// 현재 장착중인 아이템 정보 = _presetEquipItemContainer[_userPresetBitId]
	TMap<PresetValue, TMap<EGsEquipSlotType,uint64>> _presetEquipItemContainer;

	// ItemCategorySub <-> (SlotIndex, EGsEquipSlotType) 치환용 슬롯정보
	TMap<ItemCategorySub, TMap<uint8, EGsEquipSlotType>> _accSlotTypeContainer;

private:
	/** 모든 세트아이템 목록 */
	TMap<int32, FGsSetItemInfo> _allSetItemInfoMap;

private:
	// enum type 으로만 ItemData Table Data 만 가져와야 되는 경우가 있어서 별도로 캐싱처리
	TMap<ItemCategorySub, TArray<const FGsSchemaItemCommon*>> _itemTableDataMap;

private:
	// 로컬플레이어 Reactino 을 통한 아이템 락 정보 관련 맵
	// affectItemGroupId , affectItemSubCategory
	TMap<int32, TArray<ItemCategorySub>> _reactionAffectItemTableDataMap;	
	TArray<ItemCategorySub> _currentDisableAffectItemTypList;

private:
	/** 아이템 그룹 ID 로 모든 아이템 ID 를 보관. 최적화 필요. */
	static TMap<int32, TSet<ItemId>> _cachedItemIdsInItemGroup;

	/** 아이템그룹의 첫번째 아이템ID 를 보관. 최적화 필요. */
	static TMap<int32, ItemId> _cachedFirstItemIdInItemGroup;

private:
	// 현재 프리셋 ID;
	PresetId _userPresetId = 0;
	PresetValue _userPresetBitId = 0;
	int64 _localPlayerGameID = 0;
	// 천마석(정령탄) 
	uint64 _curHudSpiritShotDBID = 0;

private:
	bool _isLockByReaction = false;
	uint64 _loadedUserSettingDataDBID = 0;
	bool _isRecvSameEquipTypeProcPacket = true;
	bool _isRecvDiffEquipTypeProcPacket = true;

	// 무기 변경 쿨타임 시스템 추가로 인하여 
	// 로비에서 인게임 진입 시 마다 이전 착용주이였던
	// 무기 정보를 받음. (맨손 상태에서 로그아웃 하게되면 정보가 필요함)
	CreatureWeaponType _loginCreatureWeaponType;

private:
	bool _isUseRealDisplayStat = false;
	float _divideValue = 10.f;
	
private:
	WeightPenaltyGrade _itemWeightPenaltyGradeType;
	// 특정 아이템 사용 패킷 전송후 받기전까지 입료막기(막는 ui)위한 처리
	FGsInputBlockHandler* _inputBlockHandler = nullptr;

private:
	TSharedPtr<FGsItemEquipEnchantLoader> _equipEnchantLoader;

private:
	MsgSystemHandleArray _systemMsgDelegateList;
	MsgGameObjHandleArray _gameObjectMsgDelegateList;
	TMap<EGsIngredientUse_UIType, TFunction<void(const FGsItem*)>> _ingredientUseUIContentsMap;

public:
	virtual ~UGsItemManager();

public:
	//IGsManager
	virtual void Initialize() override;
	virtual void Finalize() override;

public:
	virtual void InitializeAfterResourceDownload() override;

public:
	// Find Table Data
	const FGsSchemaPartsShapeData* FindPartsSchemaDataByTID(int32 InItemTID) const;

private:
	void InitItemTableData();

public:
	// Find Owned Item 
	TWeakPtr<FGsItem> FindItem(const uint64 In_ItemDBID, const ItemType In_ItemType = ItemType::MAX);
	TArray<TWeakPtr<FGsItem>> FindByTID(const uint32 In_ItemTID, const ItemType In_ItemType = ItemType::MAX);
	TArray<TWeakPtr<FGsItem>> FindItem(const ItemType In_ItemType);
	TArray<TWeakPtr<FGsItem>> FindItem(const ItemType In_ItemType, const ItemCategoryMain In_ItemCategoryMain, const ItemCategorySub In_ItemCategorySub);
	TArray<TWeakPtr<FGsItem>> FindItem(const ItemType In_ItemType, const ItemCategoryMain In_ItemCategoryMain);
	TArray<TWeakPtr<FGsItem>> FindItem(const ItemType In_ItemType, const ItemCategorySub In_ItemCategorySub);
	TArray<TWeakPtr<FGsItem>> FindItem(const ItemCategorySub InSubCategory);
	TArray<TWeakPtr<FGsItem>> FindItemByPresetNum(const ItemType InType, const uint8 InPresetNum = 0);
	TArray<TWeakPtr<FGsItem>> FindAllStackableItems();
	TArray<TWeakPtr<FGsItem>> FindAllStackableItems(int32 inItemId);
	const TArray<TWeakPtr<FGsItem>> FindAllItems();
	const FGsSchemaItemCommon* FindItemSchemaDataBySubCategory(const ItemCategorySub InSubCategory, const ItemGrade InGradeType = ItemGrade::NONE);
	const bool IsMatchCategorySubByItemTID(const int32 InItemTID, const ItemCategorySub InCategorySub);

public:
	bool IsDefaultPartsShapeData(EGsPartsType InType, uint32 InPartsIndex);
	const bool IsInvenWeightFull(const float InTargetPerValue);
	const bool IsInvenWeightFull(int32 InAddWeight = 0);
	const bool IsInvenWeightFull(const TArray<TPair<int32, ItemAmount>>& InitemTIdAmountPairList);
	const bool IsInvenSlotFull();
	const bool IsInvenSlotFull(const uint16 InExpectInputSlotCount);
	const bool IsCanItemAdd(int32 inItemTID, ItemAmount inItemAmount);
	const bool IsCanItemListAdd(const TArray<TPair<int32, ItemAmount>>& InitemTIdAmountPairList);
	const bool IsReactionLockTypeItem(const ItemCategorySub InTargetItemType);
	const bool IsReactionLockTypeItem(const ItemDBId InTargetItemDbid);
	bool IsOwnedItem(const uint64 InItemDBID, const ItemType InItemType);
	bool IsOwnedItem(const uint64 InItemDBID);
	bool IsOverMaxStackCountByTID(const int32 InItemTID);

public:
	void ResetData();
	void BackToLobby();

	// Add Item
public:
	// Server Recv
	void AddItemList(TArray<ItemDataCreated> InItemList, bool InIsNewItem);
	void AddEquipItemList(TArray<ItemDataEquip> InItemList, bool InIsNewItem);

	// Client Add
	void AddItem(TArray<const ItemData*> InAddItemList, bool In_isNewItem);
	void AddItem(TArray<const ItemDataEquip*> InItemList, bool InIsNewItem);

public:
	bool TryUseItem(const FGsItem* In_Item, const ItemAmount InAmount = 0, const bool InIsOnlyValidCheck = false);
	bool TryUseItem(const uint64 InItemDBID, const ItemAmount InAmount = 0, const bool InIsOnlyValidCheck = false);
	bool TryDeleteItem(const FGsItem* InTargetItem, const ItemAmount InAmount);

public:
	void SendItemLock(uint64 InItemDbid, bool InLock, ItemStorageType InStorageType);
	bool IsCurrentPresetItem(const FGsItem* In_Item);
	bool IsEquippedBattleConsumeWeapon();
	bool IsUsableLevel(const FGsItem* In_Item);

private:
	void TryEquipItem(const FGsItem* In_Item);
	void TryConsumItem(const FGsItem* In_Item, const ItemAmount InAmount);
	void TryIngredientItem(const FGsItem* In_Item);
	bool IsIngredientUseItem(const FGsItem* In_Item);
	bool IsSameWeaponType(const FGsItem* In_Item);
	bool CheckConsumItemCondition(const FGsItem* InItem);
	bool CheckIngredientItemCondition(const FGsItemIngredient* InIngredientItem);
	//bool IsEquipSound(const ItemDataEquip* InEquipData);

	void AttachEquipItem(uint64 InEquipDBID, uint64 InUnEquipDBID, uint8 InSlotIndex);
	void DetachEquipItem(uint64 InUnEquipDBID, uint8 InSlotIndex);

public:
	void RemoveItem(const uint64 InItemDBID);
	void RemoveItemList(const TArray<ItemDBId>& InItemDBIdList);

public:
	void AllOffItemRedDotByType(const ItemType InType);
	bool CheckRedDotItemByItemType(const ItemType InType);
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam* InParam);
	void OnLocalPlayerReviveComplete(const struct IGsMessageParam* InParam);
	void FinishedPeriodItem(const struct IGsMessageParam* InParam);
	void OnReconnectEnd();
	ItemType GetItemTypeByTID(const int32 In_ItemTID);
	bool IsEnoughItemAmountByTID(const int32 InItemTID, const ItemAmount InAmount);
	//void PlayNotEnoughAquireCostTicker(const int32 InItemTID);
	void PickHudSpiritShotItem();
	// 정령탄 상태 갱신(ui 상태로)
	void SetSpiritShotStateByUIState(bool In_spiritShotUIState, bool In_invalidate = false);
	// 스킬북의 습득여부 마크 갱신
	void UpdateSkillBookState(ItemId InSkillBookItemId);

public:
	void UpdateEquipItem(TArray<const ItemDataEquip*> InItemsDataEquip);
	void UpdateItemAmountByDBID(const uint64 InDBID, const ItemAmount InAmount);
	void UpdateItemLockByDBID(const uint64 InDBID, const bool InLock, const ItemStorageType InStorageType);
	void SendItemEventUI(const ItemId InItemId);
	void SendItemEventUI(const uint64 InDBID);
	void SendItemEventUI(TArray<ItemDBId> InItemDbidList);
	void UsedItemEffect(const ItemId InItemId);
	void UsedItemEffect(const uint64 InDBID);
	void UpdateWeightPacketData(uint8 InPenaltyType);
	void UpdateInvenSlotValue(const TArray<ItemDbIdSortPair>& InItemList);
	void UseIngredientUseTypeItem(const EGsIngredientUse_UIType InType, const FGsItem* InItem);
	void UpdateReactionItemLock(TArray<int32>& InAffectIdList);

public:
	void ExpiredDeleteItem(TArray<ItemId> InItemTIDList);

public:
	void SortItemListByDBID(uint8 InSortTypeIndex, OUT TArray<uint64>& outDBIDList);
	void SortInvenValueIndex(OUT TArray<FGsItem*>& outItemList);
	void SortDefaultType(OUT TArray<FGsItem*>& outItemList);
	void SortGradeType(OUT TArray<FGsItem*>& outItemList);
	void SortWeightType(OUT TArray<FGsItem*>& outItemList);
	void TrySendInvenSlotIndex(TArray<ItemDBId> InItemDbidList, const bool InIsNeedRecord);

public:
	void SetCurrentLoginCreatureWeaponType();

private:
	void SendUpdateMessage(uint64 InItemDBID, bool InSkipUpdateDetailInfo = true, bool InIsSendComsumable = true);
	void SendUpdateMessage(uint64 InItemDBID, ItemStorageType InItemStroageType);
	void SendUpdateMessage(TArray<FGsItem*> InItemDataList, bool InSkipUpdateDetailInfo = true);
	void SendAddMessage(TArray<FGsItem*> InAddItemList);

private:
	void InitSetItemData();
	void InitEquipItemContainer();
	void InitIngredientUseSetting();

private:
	void IngredientUse_FairyUI(const FGsItem* InItem);
	void IngredientUse_MonsterKnowledgeUI(const FGsItem* InItem);
	void IngredientUse_GuildCampUI(const FGsItem* InItem);
	void IngredientUse_ItemEnchantUI(const FGsItem* InItem);
	void IngredientUse_ItemMultiDecomposeUI(const FGsItem* InItem);
	void IngredientUse_ItemCoupon(const FGsItem* InItem);
	void IngredientUse_ExchangeCostumeUI(const FGsItem* InItem);

private:
	void OnUserBasicDataChange(const struct IGsMessageParam* Data);
	void OnUpdateUserPresetId(const struct IGsMessageParam* InParam);
	void SetEquipItemData(TArray<FGsItem*> InEquipItemList);
	bool IsPairConsumeItemFromCurWeaponType(const FGsItem* InConsumeItem);
	void OnLoadLocalPlyerSettingData();
	bool IsPickOneTypeRewardItem(const FGsItem* InItem);
	FGsItem* GetGsItemLowPointer(TWeakPtr<FGsItem> InWeakPtr);

	uint64 GetEquipedItemDBID(const ItemCategorySub InSubCategory , const uint8 InSlotIndex);

public:
	void ShowErrorTidTicker(const int32 InItemTid);
		
	//stat reset
private:
	bool IsStatResetScroll(const FGsItem* inItem);
	
public:
	static const FGsSchemaItemEquipDetail* GetEquipItemTableDataByTID(int32 In_ItemTID);
	static const FGsSchemaItemCommon* GetItemTableDataByTID(int32 In_ItemTID);

public:
	bool IsEquippedItemByTid(const int32 InItemTid);
	bool IsMultiSlotItem(const ItemCategorySub InSubCategory);

public:
	void SetIsRecvSameEquipTypeProcPacket(bool InState) { _isRecvSameEquipTypeProcPacket = InState; }
	void SetIsRecvDiffEquipTypeProcPacket(bool InState) { _isRecvDiffEquipTypeProcPacket = InState; }
	void SetIsUseRealDisplayStatValue(bool InState) { _isUseRealDisplayStat = InState; }
	void SetRealDisplayDivideValue(float InVal);

public:
	uint64 GetSpiritShotItemDBID() { return _curHudSpiritShotDBID; }
	const CreatureWeaponType GetLoginCreatureWeaponType() { return _loginCreatureWeaponType; }
	bool IsUseRealDisplayStatValue() { return _isUseRealDisplayStat; }
	float GetRealDisplayDivideValue() { return _divideValue; }
	bool GetRecvDiffEquipTypeProcPacket() { return _isRecvDiffEquipTypeProcPacket; }

public:
	const ItemAmount GetAllItemAmountByTypes(ItemType Intype, ItemCategoryMain InMainType, ItemCategorySub InSubType);
	const uint16 GetOwnedItemCount();
	const int32 GetOwnedItemWeight();
	/** 세트아이템의 세트 최대 갯수 */
	uint8 GetMaxSetCountBySetId(const int32 InSetId) const;
	uint8 GetOpenSetCountBySetId(const int32 InSetId);
	TArray<TPair<bool, FText>> GetAllEquipStatusAndItemNames(const int32 InSetId);
	const TSet<ItemId>* GetAllSetItemTidBySetId(const int32 InSetId) const;
	const TArray<ItemId>* GetDefaultSetItemTids(const int32 InSetId) const;
	bool GetSetItemStatInfo(const int32 InSetId, SetNumType InType, OUT const TArray<FGsItemStatData>*& OutItemStatDatas);
	ItemAmount GetAllItemCountByTID(const int32 InItemTID, bool InCheckLock = false);
	ItemAmount GetAllItemCountByDBID(const uint64 InItemDBID, bool InCheckLock = false);
	uint32 GetTIDByDBID(const uint64 InItemDBID);
	int32 GetItemGroupByDBID(const uint64 InItemDBID);
	int32 GetEquipGroupIdByDBID(const uint64 InItemDBID);

	TWeakPtr<FGsItem> GetPlayerEquipedItemBySlotType(const EGsEquipSlotType InSlotType);
	uint64 GetEquipedItemDBIDBySlotType(const EGsEquipSlotType InSlotType);
	uint64 GetEquipedItemDBIDBySlotType(const PresetId InPresetId, const EGsEquipSlotType InSlotType);
	EGsEquipSlotType GetEquippedSlotTypeByDBID(const uint64 InItemDBID);
	EGsEquipSlotType GetEquipSlotType(const ItemCategorySub InSubCategory, const uint8 InSlotIndex) const;
	bool HasAnyEquippedMultiSlot(const ItemCategorySub InSubCategory);

	const WeightPenaltyGrade GetWeightPenaltyGradeType() { return _itemWeightPenaltyGradeType; }
	float GetInvenItemWithAddItemWeightRate(float In_addItemWeight = 0.0f);
	const bool GetReactionLockFlag() { return _isLockByReaction; }
	TArray<ItemCategorySub> GetReactionLockItemTypeList() { return _currentDisableAffectItemTypList; }
	

	FGsInputBlockHandler* GetInputBlockHandler() { return _inputBlockHandler; }

	/* 아이템 등급 조회 */
	ItemGrade GetItemGradeByDBId(const uint64 InItemDBId);

	/* 아이템 강화 단계 조회 */
	int32 GetItemEnchantLevelByDBId(const uint64 InItemDBId);

	/* 아이템 잠금 상태 조회 | 스택가능한 여러 슬롯 중 하나라도 잠겨 있으면 잠겨 있다고 판단 */
	bool IsItemLockByTId(const int32 InItemTID);

	/** 해당 아이템이 장비인지 조회(DB Id로) */
	bool IsItemEquipmentByDBId(const uint64 InItemDBId);

	/** 해당 아이템이 장비인지 조회(테이블 Id 로) */
	bool IsItemEquipmentByTId(const ItemId InItemTId);

	/** 아이템 이름 조회 */
	FText GetItemNameByDBId(const uint64 InItemDBId);
	FText GetItemNameByTId(const ItemId InItemTId);

	/** 아이템 DBId 로 아이템 TID 조회 */
	ItemId GetItemTIdByDBId(const uint64 InItemDBId);

private:
	TSharedPtr<FGsItemEquipEnchantLoader> GetEquipEnchantLoader();

public:
	const FGsItemEquipEnchantBonus* GetItemEquipEnchantBonus(const int32 InId);
	const FGsItemEquipEnchantBalance* GetItemEquipEnchantBalance(const int32 InId);

public:
	/**
	 * InCostPackage 의 아이템 비용 중에서, 처분 가능한 아이템의 DBId 와 Amount 를 만듦.
	 * 주로 서버에 처분 가능한 아이템 목록을 보내기 위해 사용.
	 */
	bool MakeDisposableItems(OUT TArray<ItemDBIdAmountPair>& OutItemAndAmounts, const FGsCostPackage& InCostPackage);
	bool MakeDisposableItems(OUT TArray<ItemDBIdAmountPair>& OutItemAndAmounts, const FGsCostPackageMultiple& InCostPackageMultiple);

	/**
	* 해당 아이템을 실제로 보유하고 있는지 아닌지를 체크하기 위한 함수. 아이템 TID 와 강화 수치를 체크한다.
	* 인벤토리와 창고 두 곳을 모두 체크한다
	*/
	bool HasItemForCraft(const ItemId InItemTID, const Level InItemLevel);

public:
	FGsItemDeliveryBoxData& GetItemDeliveryBoxData() { return _itemDeliveryBoxData; }

	/**
	 * 아이템 그룹 관련 도구
	 */
public:
	/** 두 아이템이 동일한 그룹인지 여부를 반환. */
	static bool IsTheSameGroup(const ItemId InItemIdA, const ItemId InItemIdB);

	/** 해당 그룹에서 테이블에 첫번째로 입력된 아이템ID 를 반환 */
	static ItemId GetFirstItemIdInGroup(const ItemId InItemId, const bool InIncludeExpired = false);
	
	/** 특정 아이템ID에 설정된 아이템그룹ID와 동일한 그룹의 모든 ItemId 를 반환. 반환 요소가 0개일 수 있음. */
	static TSet<ItemId> GetAllItemIdsTheSameGroup(const ItemId InItemId, const bool InIncludeExpired = false);

	/** 보유한 아이템 중, 특정 아이템ID에 설정된 아이템그룹ID와 동일한 그룹의 모든 아이템정보를 반환. 반환 요소가 0개일 수 있음. */
	TArray<TWeakPtr<FGsItem>> GetAllOwnedItemsTheSameGroup(const ItemId InItemId, const bool InIncludeExpired = false);

	// For Test Only
public:
	void PrintItemListLog();
	void PrintEquipedItemListLog() const;

	// 처음엔 CheckConsumItemCondition를 public으로 바꿀려고 했는데
	// 다른데서도 사용하면 사이드 이펙트가 생길수 있어서 별도 함수 분리
	//	https://jira.com2us.com/jira/browse/C2URWQ-5204
	// 외부용 뽑기 아이템 유효한지 체크 함수
public:
	bool IsValidSummonItem(const FGsItem* InItem);
private:
	//	https://jira.com2us.com/jira/browse/C2URWQ-5204
	// 내부 함수 체크용(CheckConsumItemCondition 에서 호출)
	bool InnerCheck_IsValidSummonItem(const FGsItem* InItem,  const bool InCheckOnly = false);

//public:
	//void SendSentryMessage(FString InMsg);
};

#define GItem() UGsScopeHolder::GetGlobalManager<UGsItemManager>(UGsScopeGlobal::EManagerType::Item)