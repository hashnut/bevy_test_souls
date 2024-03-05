// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNpcShopManager.h"
#include "T1Project.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemConsumable.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Currency/GsCurrencyHelper.h"

#include "DataSchema/NpcShop/GsSchemaNpcShopData.h"
#include "DataSchema/NpcShop/EventShop/GsSchemaNpcEventShopData.h"
#include "DataSchema/Item/GsSchemaItemConsumeDetail.h"
#include "DataSchema/Item/GsSchemaItemIngredientDetail.h"

#include "GameObject/Data/GsGameObjectDataCreature.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsInventoryManager.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsNpcShopMessageParam.h"

#include "GameFlow/GameContents/ContentsGame/NpcShop/GsNpcShopBase.h"

#include "Management/ScopeGlobal/GsSoundManager.h"


void UGsNpcShopManager::Initialize()
{
	_tabFilter.Empty();

	//NpcShop GroupID DataCenter 참조하여 데이터 가공처리
	// 잡화 상점, 스킬북 상점, 장비 상점
	const UGsTable* shopTable = UGsTableManager::GetInstance().GetTable(FGsSchemaNpcShopData::StaticStruct());
	if (nullptr == shopTable)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("FGsSchemaNpcShopData is Not Exist"));
#endif
		return;
	}

	TArray<const FGsSchemaNpcShopData*> allShopRows;
	if (shopTable->GetAllRows(allShopRows))
	{
		for (const FGsSchemaNpcShopData* iter : allShopRows)
		{
			FGsNpcShopItemInfo* storeItemInfo = new FGsNpcShopItemInfo(
				iter->disPlayPriority, iter->itemID, iter->itemID,
				iter->costType, iter->currencyType,
				iter->currencyItemId,
				iter->pcLimitLevel, 0,
				GuildMemberGradeType::UNKNOWN, PurchaseLimitBuyerType::NONE, 
				0, ResetTimeTicket::RTT_NONE, iter->autoPurchaseMaxCount,
				iter->currencyAmount);

			if (_npcShopItemInfoMap.Contains(iter->groupID))
			{
				TArray<FGsNpcShopItemInfo*>& infoList = _npcShopItemInfoMap[iter->groupID];
				infoList.Emplace(storeItemInfo);
				continue;
			}

			TArray<FGsNpcShopItemInfo*> itemInfoList;
			itemInfoList.Emplace(storeItemInfo);
			_npcShopItemInfoMap.Emplace(iter->groupID, itemInfoList);
		}
	}

	// 이벤트 상점
	const UGsTable* eventShopTable = UGsTableManager::GetInstance().GetTable(FGsSchemaNpcEventShopData::StaticStruct());
	if (nullptr == eventShopTable)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("FGsSchemaNpcEventShopData is Not Exist"));
#endif
		return;
	}
	
	TArray<const FGsSchemaNpcEventShopData*> allEventShopRows;
	if (eventShopTable->GetAllRows(allEventShopRows))
	{
		for (const FGsSchemaNpcEventShopData* iter : allEventShopRows)
		{
			FGsNpcShopItemInfo* storeItemInfo = new FGsNpcShopItemInfo(
				iter->displayPriority, iter->productId, iter->itemId,
				iter->costType, iter->currencyType,
				iter->currencyItemId, 
				iter->purchaseLimitLevel, iter->purchaseLimitGuildLevel,
				iter->purchaseLimitGuildMemberGradeType, iter->purchaseLimitType,
				iter->purchaseLimitAmount, iter->purchaseLimitResetType, 0,
				iter->currencyAmount);

			if (_npcEventShopItemInfoMap.Contains(iter->groupId))
			{
				TArray<FGsNpcShopItemInfo*>& infoList = _npcEventShopItemInfoMap[iter->groupId];
				infoList.Emplace(storeItemInfo);
				continue;
			}

			TArray<FGsNpcShopItemInfo*> infoList;
			infoList.Emplace(storeItemInfo);
			_npcEventShopItemInfoMap.Emplace(iter->groupId, infoList);
		}
	}

	// 자동 판매 가능 아이템 목록 캐싱
	// consume, ingredient Detail Id 와 ItemCommon id 와 다를 수도 있으니 주의가 필요하다.
	_autoSellSettingDataMap.Empty();
	if (const UGsTable* table = UGsTableManager::GetInstance().GetTable(FGsSchemaItemConsumeDetail::StaticStruct()))
	{
		TArray<const FGsSchemaItemConsumeDetail*> itemDetailAllRows;
		if (table->GetAllRows(itemDetailAllRows))
		{
			for (const FGsSchemaItemConsumeDetail* rowData : itemDetailAllRows)
			{
				if (false == rowData->isAllowedAutoSell)
					continue;

				_autoSellSettingDataMap.Emplace(rowData->id, 0);
			}
		}
	}
	else
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("FGsSchemaItemConsumeDetail is Not Exist"));
#endif
	}

	if (const UGsTable* table = UGsTableManager::GetInstance().GetTable(FGsSchemaItemIngredientDetail::StaticStruct()))
	{
		TArray<const FGsSchemaItemIngredientDetail*> itemDetailAllRows;
		if (table->GetAllRows(itemDetailAllRows))
		{
			for (const FGsSchemaItemIngredientDetail* rowData : itemDetailAllRows)
			{
				if (false == rowData->isAllowedAutoSell)
					continue;

				_autoSellSettingDataMap.Emplace(rowData->id, 0);
			}
		}
	}
	else
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("FGsSchemaItemIngredientDetail is Not Exist"));
#endif
	}

	// 자동판매 장착류 default 세팅
	TArray<ItemCategoryMain> mainCategoryList;
	mainCategoryList.Emplace(ItemCategoryMain::WEAPON);
	mainCategoryList.Emplace(ItemCategoryMain::ARMOR);
	mainCategoryList.Emplace(ItemCategoryMain::ACCESSORY);

	_autoSellEquipmentSettingDataMap.Empty();
	for (ItemCategoryMain category : mainCategoryList)
	{
		for (uint8 i = 0; i < (uint8)ItemGrade::MAX; ++i)
		{
			if ((uint8)ItemGrade::NONE == i)
				continue;

			_autoSellEquipmentSettingDataMap.Emplace(TPair<ItemCategoryMain, ItemGrade>(category, (ItemGrade)i), false);
		}
	}
}

void UGsNpcShopManager::Finalize()
{
	for (auto& iter : _npcShopItemInfoMap)
	{
		for (int32 i = 0; i < iter.Value.Num(); ++i)
		{
			delete iter.Value[i];
			iter.Value[i] = nullptr;
		}
	}

	for (auto& iter : _npcEventShopItemInfoMap)
	{
		for (int32 i = 0; i < iter.Value.Num(); ++i)
		{
			delete iter.Value[i];
			iter.Value[i] = nullptr;
		}
	}

	_autoBuySettingDataMap.Empty();
	_autoSellSettingDataMap.Empty();
	_autoSellEquipmentSettingDataMap.Empty();
	_tabFilter.Empty();
}

FGsNpcShopItemInfo* UGsNpcShopManager::GetShopItemInfoByItemTID(const NpcFunctionType InType, const int32 InGroupID, const int32 InItemTID)
{
	TMap<int32, TArray<FGsNpcShopItemInfo*>>& itemInfoMap =
		(NpcFunctionType::EVENT_SHOP == InType) ? _npcEventShopItemInfoMap : _npcShopItemInfoMap;
	
	if (false == itemInfoMap.Contains(InGroupID))
		return nullptr;

	TArray<FGsNpcShopItemInfo*>& infoList = itemInfoMap[InGroupID];
	for (int32 i = 0; i < infoList.Num(); ++i)
	{
		if (nullptr == infoList[i])
			continue;

		if (infoList[i]->_itemTID == InItemTID)
			return infoList[i];
	}

	return nullptr;
}

FGsNpcShopItemInfo* UGsNpcShopManager::GetShopItemInfoByProductId(const NpcFunctionType InType, const int32 InGroupID, const EventProductId InProductId)
{
	// 이벤트 상인에서만 ProductId 를 사용한다.
	if (NpcFunctionType::EVENT_SHOP != InType)
		return nullptr;

	if (false == _npcEventShopItemInfoMap.Contains(InGroupID))
		return nullptr;

	for (FGsNpcShopItemInfo* itemInfo : _npcEventShopItemInfoMap[InGroupID])
	{
		if (itemInfo->_productId == InProductId)
		{
			return itemInfo;
		}
	}

	return nullptr;
}

// 현재 장바구니에 들어있는 아이템들의 총 가격 및 총 무게, 인벤토리 슬롯갯수 계산하여 가능한 갯수만 반환함
ItemAmount UGsNpcShopManager::GetMaximumCalculatorCount(const NpcFunctionType InNpcFunctionType, const FGsNpcShopItemInfo* InItemInfo, OUT EGsNotAllowedBuyReason& outNotAllowedBuyReasonType)
{
	if (nullptr == InItemInfo)
	{
		outNotAllowedBuyReasonType = EGsNotAllowedBuyReason::OTHER_REASON;
		return 0;
	}

	const FGsSchemaItemCommon* itemCommonData = FGsItem::GetTableDataByID(InItemInfo->_itemTID);
	if (nullptr == itemCommonData)
	{
		outNotAllowedBuyReasonType = EGsNotAllowedBuyReason::OTHER_REASON;
		return 0;
	}

	int32 shoppingBasketItemTotalWeight = 0;	// 장바구니 총 무게
	Currency shoppingBasketTotalPrice = 0;		// 장바구니 총 가격
	ItemAmount shoppingBasketCount = 0;				// 장바구니에 있는 같은 아이템의 개수
	for (const FGsNpcShopQuickSlotItemInfo& basketItemInfo : _basketList)
	{
		FGsNpcShopItemInfo* shopInfo = GetShopItemInfoByItemTID(InNpcFunctionType, basketItemInfo._shopGroupId, basketItemInfo._itemTID);
		if(nullptr == shopInfo)
			continue;

		const FGsSchemaItemCommon* shoppingBasketitemCommonData = FGsItem::GetTableDataByID(basketItemInfo._itemTID);
		if (nullptr == shoppingBasketitemCommonData)
			continue;

		shoppingBasketItemTotalWeight += (shoppingBasketitemCommonData->weight * basketItemInfo._itemAmount);
		shoppingBasketTotalPrice += (shopInfo->GetCurrencyAmount(InNpcFunctionType) * basketItemInfo._itemAmount);

		if (InItemInfo->_itemTID == basketItemInfo._itemTID)
		{
			shoppingBasketCount += basketItemInfo._itemAmount;
		}
	}

	// 캐릭터 무게 체크
	ItemAmount resultWeightCount = 0;
	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		FGsGameObjectDataCreature* creatureData = local->GetCastData<FGsGameObjectDataCreature>();
		if (nullptr == creatureData)
		{
			outNotAllowedBuyReasonType = EGsNotAllowedBuyReason::OTHER_REASON;
			return 0;
		}

		if (const FGsStatInfo* weightStat = creatureData->GetCreatureStatInfo(StatType::WEIGHT))
		{
			if (0 >= weightStat->GetMaxStatValue())
			{
				outNotAllowedBuyReasonType = EGsNotAllowedBuyReason::OTHER_REASON;
				return 0;
			}

			const FGsSchemaItemCommon* newItemCommonData = FGsItem::GetTableDataByID(InItemInfo->_itemTID);
			if (nullptr == newItemCommonData)
			{
				outNotAllowedBuyReasonType = EGsNotAllowedBuyReason::OTHER_REASON;
				return 0;
			}

			// 구매 가능한 남는 무게 = 플레이어 최대 무게 - (현재 무게 + 장바구니 무게)
			int32 realWeight = weightStat->GetMaxStatValue() - (GItem()->GetOwnedItemWeight() + shoppingBasketItemTotalWeight) - 1;
			// 구매 불가(무게 초과)
			if (0 >= realWeight)
			{
				outNotAllowedBuyReasonType = EGsNotAllowedBuyReason::OVER_INVENTORY_WEIGHT;
				return 0;
			}

			// 무게가 없으면 최대 개수, 있으면 남는 무게 / 아이템 무게
			ItemAmount calcWeightCount = (0 >= newItemCommonData->weight) 
				? newItemCommonData->maxStackCount : realWeight / newItemCommonData->weight;
			// 구매 불가(무게 초과)
			if (0 >= calcWeightCount)
			{
				outNotAllowedBuyReasonType = EGsNotAllowedBuyReason::OVER_INVENTORY_WEIGHT;
				return 0;
			}
			// 구매 가능
			else
			{
				// 남는 무게로 가질 수 있는 아이템 수와 최대 스택 수를 비교
				resultWeightCount = (calcWeightCount >= newItemCommonData->maxStackCount)
					? newItemCommonData->maxStackCount : calcWeightCount;
			}
		}
	}

	// 더 담을 수 있는 개수
	ItemAmount resultLimitCount = 0;
	// 구매에 제한이 없으면 그대로 유지
	if (InItemInfo->_pcLimitBuyerAmount == 0)
	{
		resultLimitCount = resultWeightCount;
	}
	// 구매 제한 개수와 이미 구매한 개수가 같으면 매진
	else if (InItemInfo->_pcLimitBuyerAmount == InItemInfo->_purchasedAmount)
	{
		outNotAllowedBuyReasonType = EGsNotAllowedBuyReason::SOLD_OUT;
		return 0;
	}
	// 구매 제한 개수가 이미 구매한 개수와 장바구니에 담은 수가 같으면 더이상 담지 못함
	else if (InItemInfo->_pcLimitBuyerAmount <= InItemInfo->_purchasedAmount + shoppingBasketCount)
	{
		outNotAllowedBuyReasonType = EGsNotAllowedBuyReason::OVER_LIMIT_AVAILABLE_AMOUNT;
		return 0;
	}
	else
	{
		resultLimitCount = InItemInfo->_pcLimitBuyerAmount - (InItemInfo->_purchasedAmount + shoppingBasketCount);
	}

	// 재화량으로 가질 수 있는 개수
	ItemAmount resultCurrencyCount = FMath::Min<ItemAmount>(resultLimitCount, resultWeightCount);
	// 가진 재화량 or 재화아이템 개수
	CostType costType = InItemInfo->_costType;
	Currency curValue;

	// 지불 댓가가 재화 타입일 경우
	if (CostType::CURRENCY == costType)
	{
		curValue = FGsCurrencyHelper::GetCurrencyAmount(InItemInfo->_currencyType);
	}
	// 지불 댓가가 아이템일 경우
	else
	{
		// 해당 아이템이 잠금 상태일 경우
		if (GItem()->IsItemLockByTId(InItemInfo->_currencyItemTID))
		{
			outNotAllowedBuyReasonType = EGsNotAllowedBuyReason::CURRENCY_ITEM_LOCKED;
			return 0;
		}
		else
		{
			curValue = GItem()->GetAllItemCountByTID(InItemInfo->_currencyItemTID);
		}
	}

	// 남는 재화량 = 가진 재화량 - 장바구니 재화량
	Currency realCurrency = curValue - shoppingBasketTotalPrice;
	// 구매불가(재화부족)
	if (0 >= realCurrency)
	{
		outNotAllowedBuyReasonType = EGsNotAllowedBuyReason::NOT_ENOUGH_COST;
		return 0;
	}

	// 필요 재화가 없으면 최대 개수, 있으면 남는 재화량 / 요구 재화량
	ItemAmount calcPriceCount = itemCommonData->maxStackCount;
	Currency inCurrencyAmount = InItemInfo->GetCurrencyAmount(InNpcFunctionType);
	if (0 < inCurrencyAmount)
	{
		if ((Currency)itemCommonData->maxStackCount > (realCurrency / inCurrencyAmount))
		{
			calcPriceCount = (realCurrency / inCurrencyAmount);
		}
	}
	// 구매불가(재화부족)
	if (0 >= calcPriceCount)
	{
		outNotAllowedBuyReasonType = EGsNotAllowedBuyReason::NOT_ENOUGH_COST;
		return 0;
	}

	ItemAmount resultCount = FMath::Min<ItemAmount>(resultCurrencyCount, calcPriceCount);;
	// 인벤토리 슬롯 카운트 체크
	if (true == GItem()->IsInvenSlotFull())
	{
		ItemAmount enoughSlotCount = 0;
		// 스택이 되는 아이템은 해당 아이템 tid 로 find  하여, stackCount 여유가 있는 갯수만큼 return 처리
		// GItem()->GetOwnedItemCount();
		if (1 < itemCommonData->maxStackCount) // 스택 가능 아이템
		{
			TArray<TWeakPtr<FGsItem>> findItemList = GItem()->FindByTID(itemCommonData->id, itemCommonData->type);
			for (int32 i = 0; i < findItemList.Num(); ++i)
			{
				if(false == findItemList[i].IsValid())
					continue;

				if (itemCommonData->maxStackCount > findItemList[i].Pin()->GetAmount())
				{
					enoughSlotCount += (itemCommonData->maxStackCount - findItemList[i].Pin()->GetAmount());
				}
			}
		}

		if (0 >= enoughSlotCount)
		{
			outNotAllowedBuyReasonType = EGsNotAllowedBuyReason::OVER_INVENTORY_SLOT;
		}
		return  FMath::Min<ItemAmount>(resultCount, enoughSlotCount);
	}

	return resultCount;
}

// 현재 플레이어 무게와 장바구니에 담긴 아이템 무게까지 계산한 값
int32 UGsNpcShopManager::GetExpectWeightValue(bool InIsBuyMode)
{
	int32 shoppingBasketItemTotalWeight = 0.f;
	for (FGsNpcShopQuickSlotItemInfo& basketItemInfo : _basketList)
	{
		const FGsSchemaItemCommon* itemCommonData = FGsItem::GetTableDataByID(basketItemInfo._itemTID);
		if (nullptr == itemCommonData)
			continue;

		shoppingBasketItemTotalWeight += itemCommonData->weight * basketItemInfo._itemAmount;
	}

	int32 ownedItemTotalWeight = GItem()->GetOwnedItemWeight(); // 실 소유 무게
	// 구매할때는 무게를 더하지만 팔때는 무게를 뺀다
	int32 finalWeight = true == InIsBuyMode ? ownedItemTotalWeight + shoppingBasketItemTotalWeight
		: ownedItemTotalWeight - shoppingBasketItemTotalWeight;

	if (0 >= finalWeight)
		return 0;

	return finalWeight;
}

//_npcStoreItemList 참조하여 InStoreGroupID 에 맞는 itemList out 처리
TArray<FGsNpcShopItemInfo*> UGsNpcShopManager::GetDisPlayItemListByGroupIDAndType(const NpcFunctionType InNpcFunctionType, const TArray<int32>& InStoreGroupIdList, const ENpcShopProductListTab InTabType)
{
	TMap<int32, TArray<FGsNpcShopItemInfo*>>& itemInfoMap =
		(NpcFunctionType::EVENT_SHOP == InNpcFunctionType) ? _npcEventShopItemInfoMap : _npcShopItemInfoMap;
	
	TArray<FGsNpcShopItemInfo*> resultShopList;
	resultShopList.Empty();
	for (int32 i = 0; i < InStoreGroupIdList.Num(); ++i)
	{
		if (false == itemInfoMap.Contains(InStoreGroupIdList[i]))
		{
			GSLOG(Warning, TEXT("[UGsNpcShopManager::GetDisPlayItemList] - InStoreGroupID is WrongID : %d"), InStoreGroupIdList[i]);
			continue;
		}

		TArray<FGsNpcShopItemInfo*>& productList = itemInfoMap[InStoreGroupIdList[i]];
		for (int32 j = 0; j < productList.Num(); ++j)
		{ 
			if (nullptr == productList[j])
				continue;

			if (InTabType == ENpcShopProductListTab::ProductList_Filter)
			{
				bool canDisplay = false;
				for (ENpcShopProductListTab tabType : _tabFilter)
				{
					if (CanDisplayItem(productList[j]->_itemTID, tabType))
					{
						canDisplay = true;
						break;
					}
				}
				if (false == canDisplay)
				{
					continue;
				}
			}
			else
			{
				if (false == CanDisplayItem(productList[j]->_itemTID, InTabType))
				{
					continue;
				}
			}
			

			resultShopList.Emplace(productList[j]);
		}
	}
	
	return resultShopList;
}

bool UGsNpcShopManager::CanDisplayItem(const ItemId InItemId, const ENpcShopProductListTab InTabType)
{
	const FGsSchemaItemCommon* itemCommonData = FGsItem::GetTableDataByID(InItemId);
	if (nullptr == itemCommonData)
		return false;

	if (ENpcShopProductListTab::ProductList_ConsumableTab == InTabType && ItemType::CONSUME != itemCommonData->type)
		return false;
	if (ENpcShopProductListTab::ProductList_EquipmentTab == InTabType && ItemType::EQUIP != itemCommonData->type)
		return false;
	if (ENpcShopProductListTab::ProductList_MaterialTab == InTabType && !(ItemType::INGREDIENT == itemCommonData->type || ItemType::CURRENCY == itemCommonData->type))
		return false;
	if (ENpcShopProductListTab::ProductList_Weapon == InTabType && ItemCategoryMain::WEAPON != itemCommonData->categoryMain)
		return false;
	if (ENpcShopProductListTab::ProductList_Armor == InTabType && ItemCategoryMain::ARMOR != itemCommonData->categoryMain)
		return false;
	if (ENpcShopProductListTab::ProductList_Accessory == InTabType && ItemCategoryMain::ACCESSORY != itemCommonData->categoryMain)
		return false;
	if (ENpcShopProductListTab::ProductList_Sword == InTabType && !(ItemCategorySub::SWORD_SKILL_BOOK == itemCommonData->categorySub || ItemCategorySub::COMMON_SKILL_BOOK == itemCommonData->categorySub))
		return false;
	if (ENpcShopProductListTab::ProductList_Dagger == InTabType && !(ItemCategorySub::DAGGER_SKILL_BOOK == itemCommonData->categorySub || ItemCategorySub::COMMON_SKILL_BOOK == itemCommonData->categorySub))
		return false;
	if (ENpcShopProductListTab::ProductList_Bow == InTabType && !(ItemCategorySub::BOW_SKILL_BOOK == itemCommonData->categorySub || ItemCategorySub::COMMON_SKILL_BOOK == itemCommonData->categorySub))
		return false;
	if (ENpcShopProductListTab::ProductList_SHOTGUN == InTabType && !(ItemCategorySub::SHOTGUN_SKILL_BOOK == itemCommonData->categorySub || ItemCategorySub::COMMON_SKILL_BOOK == itemCommonData->categorySub))
		return false;
	if (ENpcShopProductListTab::ProductList_Wand == InTabType && !(ItemCategorySub::WAND_SKILL_BOOK == itemCommonData->categorySub || ItemCategorySub::COMMON_SKILL_BOOK == itemCommonData->categorySub))
		return false;
	if (ENpcShopProductListTab::ProductList_Orb == InTabType && !(ItemCategorySub::ORB_SKILL_BOOK == itemCommonData->categorySub || ItemCategorySub::COMMON_SKILL_BOOK == itemCommonData->categorySub))
		return false;

	return true;
}

int32 UGsNpcShopManager::GetGroupIdByItemTID(const NpcFunctionType InNpcFunctionType, const TArray<int32>& InShopGroupIdList, const int32 InItemTID)
{
	TMap<int32, TArray<FGsNpcShopItemInfo*>>& itemInfoMap =
		(NpcFunctionType::EVENT_SHOP == InNpcFunctionType) ? _npcEventShopItemInfoMap : _npcShopItemInfoMap;

	for (int32 i = 0; i < InShopGroupIdList.Num(); ++i)
	{
		int32 groupId = InShopGroupIdList[i];
		if (itemInfoMap.Contains(groupId))
		{
			TArray<FGsNpcShopItemInfo*>& dataList = itemInfoMap[groupId];
			for (int32 j = 0; j < dataList.Num(); ++j)
			{
				if(dataList[j]->_itemTID != InItemTID)
					continue;

				return groupId;
			}
		}
	}

	return 0;
}

int32 UGsNpcShopManager::GetGroupIdByProductId(const NpcFunctionType InNpcFunctionType, const TArray<int32>& InShopGroupIdList, const EventProductId InProductId)
{
	// 이벤트 상인에서만 ProductId 를 사용한다.
	if (NpcFunctionType::EVENT_SHOP != InNpcFunctionType)
		return 0;

	for (int32 i = 0; i < InShopGroupIdList.Num(); ++i)
	{
		int32 groupId = InShopGroupIdList[i];
		if (_npcEventShopItemInfoMap.Contains(groupId))
		{
			TArray<FGsNpcShopItemInfo*>& dataList = _npcEventShopItemInfoMap[groupId];
			for (int32 j = 0; j < dataList.Num(); ++j)
			{
				if (dataList[j]->_productId != InProductId)
					continue;

				return groupId;
			}
		}
	}

	return 0;
}

void UGsNpcShopManager::AckNpcShopPacket(bool InIsBuy)
{
	ClearBasketList(true);

	if (InIsBuy)
	{
		// 구매 완료 티커 출력
		FText findText;
		FText::FindText(TEXT("NpcShopText"), TEXT("Notice_Buy"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);

		//  구매 사운드 출력
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			soundPlayer->PlaySound2D(TEXT("UI_BuyItem"));
		}

		UGsItemManager* itemMgr = GItem();
		if (WeightPenaltyGrade::NONE == itemMgr->GetWeightPenaltyGradeType())
			return;

		FTextKey textKey = (itemMgr->GetWeightPenaltyGradeType() == WeightPenaltyGrade::FIRST) ?
			TEXT("ItemUi_Weight_Penalty_1stGrade") : TEXT("ItemUi_Weight_Penalty_2ndGrade");

		int32 value = GSInventory()->GetOwnedItemPenaltyValueByType(itemMgr->GetWeightPenaltyGradeType());
		if (FText::FindText(TEXT("ItemUIText"), textKey, findText))
		{
			FText msg = FText::Format(findText, value);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, msg);
		}
	}
	else
	{
		// 판매 완료 티커 출력
		FText findText;
		FText::FindText(TEXT("NpcShopText"), TEXT("Notice_Sell"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);

		//  판매 사운드 출력
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			soundPlayer->PlaySound2D(TEXT("UI_SellItem"));
		}
	}
}

TArray<uint32> UGsNpcShopManager::GetAutoSellItemTidList()
{
	TArray<uint32> tidList;
	tidList.Empty();
	for (const auto& iter : _autoSellSettingDataMap)
	{
		tidList.Emplace(iter.Key);
	}

	return tidList;
}

const ItemAmount UGsNpcShopManager::GetAutoBuySettingData(const uint32 InItemTID)
{
	if (0 >= InItemTID)
		return 0;

	if (true == _autoBuySettingDataMap.Contains(InItemTID))
	{
		return _autoBuySettingDataMap[InItemTID];
	}

	return 0;
}

bool UGsNpcShopManager::IsExistAutoBuyItemData()
{
	for (const auto& iter : _autoBuySettingDataMap)
	{
		if (0 < iter.Value)
		{
			return true;
		}
	}
	return false;
}

bool UGsNpcShopManager::IsExistAutoSellItemData()
{
	for (const auto& iter : _autoSellSettingDataMap)
	{
		if (0 < iter.Value)
		{
			return true;
		}
	}
	for (const auto& iter : _autoSellEquipmentSettingDataMap)
	{
		if (true == iter.Value)
		{
			return true;
		}
	}

	return false;
}

bool UGsNpcShopManager::IsEnableAutoSellEquipment(const ItemCategoryMain InItemCategory, const ItemGrade InItemGrade)
{
	TPair<ItemCategoryMain, ItemGrade> keyData = TPair<ItemCategoryMain, ItemGrade>(InItemCategory, InItemGrade);
	if (_autoSellEquipmentSettingDataMap.Contains(keyData))
	{
		return _autoSellEquipmentSettingDataMap[keyData];
	}

	return false;
}

bool UGsNpcShopManager::IsEnableAutoSellConsumable(const uint32 InItemTID)
{
	if (_autoSellSettingDataMap.Contains(InItemTID))
	{
		return 0 < _autoSellSettingDataMap[InItemTID];
	}
	
	return false;
}

void UGsNpcShopManager::SetItemPurchasedList(const FGsSchemaNpcData* InNpcData, const TArray<ItemIdAmountPair>& InItemList)
{
	TMap<int32, TArray<FGsNpcShopItemInfo*>>& itemInfoMap =
		(NpcFunctionType::EVENT_SHOP == InNpcData->npcFunctionType) ? _npcEventShopItemInfoMap : _npcShopItemInfoMap;

	const TArray<int32>& shopGroupIdList = InNpcData->shopGroupIdList;
	bool checkItemFind = false;
	for (const ItemIdAmountPair& InItem : InItemList)
	{
		for (int32 groupId : shopGroupIdList)
		{
			if (false == itemInfoMap.Contains(groupId))
				continue;

			TArray<FGsNpcShopItemInfo*>& shopItemList = itemInfoMap[groupId];
			for (FGsNpcShopItemInfo* shopItem : shopItemList)
			{
				if (NpcFunctionType::EVENT_SHOP == InNpcData->npcFunctionType)
				{
					// 이벤트 상점은 productId로 판단한다.
					if (shopItem->_productId == InItem.mItemId)
					{
						// 수량 정보 갱신
						shopItem->_purchasedAmount = InItem.mItemAmount;
						checkItemFind = true;
						break;
					}
				}
				else
				{
					// 이벤트 상점 외에 들어올 로직은 없지만 로직은 그대로 둔다.
					if (shopItem->_itemTID == InItem.mItemId)
					{
						shopItem->_purchasedAmount = InItem.mItemAmount;
						checkItemFind = true;
						break;
					}
				}
			}

			if (true == checkItemFind)
			{
				checkItemFind = false;
				break;
			}
		}
	}

	GMessage()->GetNpcShop().SendMessage(MessageContentNpcShop::INVALIDATE_PRODUCTS, nullptr);
}

void UGsNpcShopManager::OnChangeAutoBuySettingData(TArray<ItemIdAmountPair>& InSettingList)
{
	_autoBuySettingDataMap.Empty();
	for (const ItemIdAmountPair pairData : InSettingList)
	{
		if (false == _autoBuySettingDataMap.Contains(pairData.mItemId))
		{
			_autoBuySettingDataMap.Emplace(pairData.mItemId, pairData.mItemAmount);
		}
		else
		{
			_autoBuySettingDataMap[pairData.mItemId] = pairData.mItemAmount;
		}
	}
}

void UGsNpcShopManager::OnChangeAutoSellConsumSettingData(TArray<ItemId>& InSettingList)
{	
	for (auto& iter : _autoSellSettingDataMap)
	{
		iter.Value = 0;
	}

	for (const ItemId itemID : InSettingList)
	{
		if (false == _autoSellSettingDataMap.Contains(itemID))
		{
			// UGsNpcShopManager::Initialize 에서 설정되지 않은 데이터는 isAllowedAutoSell 가 False 이거나 존재하지 않는 데이터이므로 신규로 추가하면 안된다.
			//_autoSellSettingDataMap.Emplace(itemID, 1);
		}
		else
		{
			_autoSellSettingDataMap[itemID] = 1;
		}
	}
}

void UGsNpcShopManager::OnChangeAutoSellEquipSettingData(TArray<AutoShopSellEquipPair>& InSettingList)
{
	//TMap<TPair<ItemCategoryMain, ItemGrade>, bool>
	for (auto& iter : _autoSellEquipmentSettingDataMap)
	{
		iter.Value = false;
	}

	for (const AutoShopSellEquipPair pairData : InSettingList)
	{	
		TPair<ItemCategoryMain, ItemGrade> tempData = TPair<ItemCategoryMain, ItemGrade>(pairData.mItemCatergoryMain, pairData.mItemGrade);
		if (true == _autoSellEquipmentSettingDataMap.Contains(tempData))
		{
			_autoSellEquipmentSettingDataMap[tempData] = true;
		}
	}
}

void UGsNpcShopManager::AddFilter(ENpcShopProductListTab InTab)
{
	if (false == _tabFilter.Contains(InTab))
	{
		_tabFilter.Emplace(InTab);
	}
}

void UGsNpcShopManager::RemoveFilter(ENpcShopProductListTab InTab)
{
	if (_tabFilter.Contains(InTab))
	{
		_tabFilter.Remove(InTab);
	}
}

bool UGsNpcShopManager::IsInFilter(ENpcShopProductListTab InTab)
{
	return _tabFilter.Contains(InTab);
}

bool UGsNpcShopManager::IsFilterEmpty()
{
	return (_tabFilter.Num() == 0);
}

void UGsNpcShopManager::ClearBasketList(bool InSendMessage)
{
	_basketList.Empty();

	if (InSendMessage)
	{
		FGsInvalidateShoppingBasket paramData(0);
		GMessage()->GetNpcShop().SendMessage(MessageContentNpcShop::INVALIDATE_BASKET, &paramData);
	}
}

TArray<FGsNpcShopQuickSlotItemInfo>& UGsNpcShopManager::GetBasketList()
{
	return _basketList;
}

int32 UGsNpcShopManager::InputBasket(ItemDBId InItemDBID, ItemId InItemTID, EventProductId InProductId, ItemAmount InItemAmount, int32 InGroupId, int32 InEnchantLevel, bool InIsShowTicker /*= true*/)
{
	if (0 >= InItemAmount)
		return -1;

	if (IsBasketFull())
	{
		if (InIsShowTicker)
		{
			// 물품 이동 슬롯이 가득 차 있습니다.
			FText findText;
			FText::FindText(TEXT("DepotText"), TEXT("Alert_FullSlot"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
		}
		return -1;
	}

	_basketList.Emplace(InItemDBID, InItemTID, InProductId, InItemAmount, InGroupId, InEnchantLevel);
	return _basketList.Num() - 1;
}

bool UGsNpcShopManager::IsBasketFull()
{
	return _basketList.Num() >= SHOPPINGBASKET_MAXIMUM_SLOTCOUNT;
}

void UGsNpcShopManager::RemoveBasketItemByIndex(int32 InIndex)
{
	if (false == _basketList.IsValidIndex(InIndex))
		return;

	_basketList.RemoveAt(InIndex);

	int32 displayedSlotIndex = (InIndex == 0 ? 0 : (InIndex < _basketList.Num() ? InIndex : InIndex - 1));
	FGsInvalidateShoppingBasket paramData(displayedSlotIndex);
	GMessage()->GetNpcShop().SendMessage(MessageContentNpcShop::INVALIDATE_BASKET, &paramData);
}

void UGsNpcShopManager::RemoveBasketItemByItemDBId(ItemDBId InItemDBId)
{
	for (int32 i = 0; i < _basketList.Num(); ++i)
	{
		if (InItemDBId == _basketList[i]._itemDBID)
		{
			_basketList.RemoveAt(i);
			return;
		}
	}
}

void UGsNpcShopManager::UpdateBasketItem(ItemDBId InItemDBId)
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	for (FGsNpcShopQuickSlotItemInfo& basketItemInfo : _basketList)
	{
		if (InItemDBId == basketItemInfo._itemDBID)
		{
			TWeakPtr<FGsItem> itemData = itemManager->FindItem(InItemDBId);
			if (false == itemData.IsValid())
				return;

			basketItemInfo._itemAmount = FMath::Min(basketItemInfo._itemAmount, itemData.Pin()->GetAmount());
			basketItemInfo._enchantLevel = itemData.Pin()->GetLevel();
			return;
		}
	}
}