// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"

/**
 *  Item Packet Send Class
 */
class T1PROJECT_API FGsNetSendServiceItem
{
public:
	static void SendUseItem(const uint64 InItemDBID, const ItemCategorySub InSubCategory);
	static void SendSeveralUseItem(const ItemDBIdAmountPair InData);
	static void SendAttachEquipItem(const uint8 InPresetID, const uint64 InEquipItemDBID, const uint8 InEquipItemSlotIndex, const uint64 InUnEquipItemDBID, const uint8 InUnEquipItemSlotIndex);
	static void SendDetacheEquipItem(const uint8 InPresetID , const uint64 InUnEquipItemDBID, const uint8 InUnEquipSlotIndex);
	static void SendItemLockState(const uint64 InItemDBID , const bool InLock, const ItemStorageType InStorageType);
	static void SendDeleteItem(const uint64 InItemDBID, const ItemAmount InItemCount);
	static void SendItemDecompose(const TArray<ItemDBIdAmountPair> InDecomposeTargetItemList);
	static void SendUseAutoPotionState(const bool InIsActive);
	static void SendUseCampConsumItem(const uint64 InItemDBID, const CampPosition InCampPosInfo);

public:
	static void SendEnchantItem(const uint64 InUseConsumItemDBID, const ItemAmount InAmountConsumItem, const uint64 InTargetEquipItemDBID);
	static void SendMultiEnchantItem(TArray<ItemDBIdAmountPair> InIngredientItemList, TArray<ItemDBId> InEquipItemDbidList, Level InNextLevel);
	static void SendRefineOptionItem(const ItemDBId InTargetItemDBID , const ItemDBId InOptionStoneItemDBID);
	static void SendMagicalForgeItem(const ItemDBId InTargetItemDBId, const ItemDBId InIngredientItemDBId);

public:
	static void SendItemRegistOrRemoveQuickSlot(const QuickSlotId InSlotId, const uint64 InItemDBID, const uint32 InItemTID);
	static void SendQuickSlotRegistItemGroup(TMap<QuickSlotId,TPair<uint64, uint32>>& InSwapPairDataList);
	static void SendAutoUseStateQuickSlot(const QuickSlotId InSlotID , const uint64 InItemDBID , const bool InFlag);

public:
	static void SendBuyNpcShopItem(CreatureId InCreatureID, TArray<NpcShopItemBuyPair>& InBuyItemList);
	static void SendSellNpcShopItem(CreatureId InCreatureID, TArray<ItemDBIdAmountPair>& InSellItemList);

public:
	static void SendBuyNpcEventShopItem(int64 InGameID, TArray<NpcEventShopItemBuyPair>& InBuyItemList);

public:
	static void SendItemEquipChangeWeapon(ItemDBId InItemId, float InDuration);
	static void SendInventorySortValue(TArray<uint64> InSortItemList);

public:
	static void SendDepositItems(CreatureId InCreatureID, TArray<ItemDBIdAmountPair>& InItemList);
	static void SendTakeItems(CreatureId InCreatureID, TArray<ItemDBIdAmountPair>& InItemList);
	static void SendExtendDepotSlot(uint8 InExtendLevel);
	static void SendPickOneItem(const uint64 InItemDBID, const uint8 Index);
	static void SendSeveralPickOneItem(const uint64 InItemDBID, const ItemAmount InAmount, const uint8 Index);
	static void SendExtendInvenSlot(uint8 InExtendLevel);

public:
	static void SendOnChangeAutoDeposit(TArray<ItemId> InDataList);
	static void SendOnChangeAutoDepositGrade(const TArray<AutoDepotConfigPair>& InDataList);
	static void SendOnChangeAutoBuy(TArray<ItemIdAmountPair> InDataList);
	static void SendOnChangeAutoSellConsumable(TArray<ItemId> InDataList);
	static void SendOnChangeAutoSellEquipment(TArray<AutoShopSellEquipPair> InDataList);

public:
	static void SendItemUseDeliveryBoxEquip(ItemDBId InBoxItemDBId, ItemDBId InRecoverItemDBId);
	static void SendItemUseDeliveryBoxFairyCompose(ItemDBId InBoxItemDBId, FairyGrade InFairyGrade);
	static void SendItemUseDeliveryBoxCostumeCompose(ItemDBId InBoxItemDBId, CostumeGrade InCostumeGrade);

public:
	static void SendTryItemFusion(ItemDBId InSrcItemDBId, ItemDBId InDestItemDBId, ItemDBId InRefinedItemDBId);
};
