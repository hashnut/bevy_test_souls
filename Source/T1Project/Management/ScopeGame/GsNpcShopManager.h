// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "Management/GsScopeHolder.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "NpcShop/GsNpcShopEnum.h"
#include "NpcShop/GsNpcShopData.h"
#include "GsNpcShopManager.generated.h"

class FGsItem;

/**
 * NpcShop Manager
 */

UCLASS()
class T1PROJECT_API UGsNpcShopManager : 
	public UObject,
	public IGsManager
{
	GENERATED_BODY()

private:
	const int32 SHOPPINGBASKET_MAXIMUM_SLOTCOUNT = 30;

	// Store Group ID , StoreItemInfo List
	TMap<int32, TArray<FGsNpcShopItemInfo*>> _npcShopItemInfoMap;
	TMap<int32, TArray<FGsNpcShopItemInfo*>> _npcEventShopItemInfoMap;

private:
	// 자동 구매 설정된 리스트
	TMap<ItemId, ItemAmount> _autoBuySettingDataMap;
	// 자동 판매 설정된 리스트 (Consume, Ingredient)
	TMap<ItemId, ItemAmount> _autoSellSettingDataMap;
	// 장비 자동 판매 설정된 리스트
	TMap<TPair<ItemCategoryMain,ItemGrade>, bool> _autoSellEquipmentSettingDataMap;

	TArray<ENpcShopProductListTab> _tabFilter;

private:
	// 물품 이동 목록
	TArray<FGsNpcShopQuickSlotItemInfo> _basketList;

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

	void AckNpcShopPacket(bool InIsBuy);

	bool IsExistAutoBuyItemData();
	bool IsExistAutoSellItemData();

	ItemAmount GetMaximumCalculatorCount(const NpcFunctionType InNpcFunctionType, const FGsNpcShopItemInfo* InItemInfo, OUT EGsNotAllowedBuyReason& outNotAllowedBuyReasonType);
	int32 GetGroupIdByItemTID(const NpcFunctionType InNpcFunctionType, const TArray<int32>& InShopGroupIdList, const int32 InItemTID);
	int32 GetGroupIdByProductId(const NpcFunctionType InNpcFunctionType, const TArray<int32>& InShopGroupIdList, const EventProductId InProductId);
	int32 GetExpectWeightValue(bool InIsBuyMode);
	FGsNpcShopItemInfo* GetShopItemInfoByItemTID(const NpcFunctionType InType, const int32 InGroupID, const int32 InItemTID);
	FGsNpcShopItemInfo* GetShopItemInfoByProductId(const NpcFunctionType InType, const int32 InGroupID, const EventProductId InProductId);
	TArray<uint32> GetAutoSellItemTidList();

	void OnChangeAutoBuySettingData(TArray<ItemIdAmountPair>& InSettingList);
	void OnChangeAutoSellConsumSettingData(TArray<ItemId>& InSettingList);
	void OnChangeAutoSellEquipSettingData(TArray<AutoShopSellEquipPair>& InSettingList);

	const ItemAmount GetAutoBuySettingData(const uint32 InItemTID);

	bool IsEnableAutoSellEquipment(const ItemCategoryMain InItemCategory , const ItemGrade InItemGrade);
	bool IsEnableAutoSellConsumable(const uint32 InItemTID);

	TArray<FGsNpcShopItemInfo*> GetDisPlayItemListByGroupIDAndType(const NpcFunctionType InNpcFunctionType, const TArray<int32>& InStoreGroupIdList, const ENpcShopProductListTab InTabType);
	bool CanDisplayItem(const ItemId InItemId, const ENpcShopProductListTab InTabType);

	void SetItemPurchasedList(const FGsSchemaNpcData* InNpcData, const TArray<ItemIdAmountPair>& InItemList);

	void AddFilter(ENpcShopProductListTab InTab);
	void RemoveFilter(ENpcShopProductListTab InTab);
	bool IsInFilter(ENpcShopProductListTab InTab);
	bool IsFilterEmpty();

	// 물품 이동 목록
	void ClearBasketList(bool InSendMessage = false);
	TArray<FGsNpcShopQuickSlotItemInfo>& GetBasketList();
	int32 InputBasket(ItemDBId InItemDBID, ItemId InItemTID, EventProductId InProductId, ItemAmount InItemAmount, int32 InGroupId, int32 InEnchantLevel, bool InIsShowTicker = true);
	bool IsBasketFull();
	void RemoveBasketItemByIndex(int32 InIndex);
	void RemoveBasketItemByItemDBId(ItemDBId InItemDBId);
	void UpdateBasketItem(ItemDBId InItemDBId);
};

#define GSNpcShop() UGsScopeHolder::GetGameManager<UGsNpcShopManager>(UGsScopeGame::EManagerType::NpcShop)
