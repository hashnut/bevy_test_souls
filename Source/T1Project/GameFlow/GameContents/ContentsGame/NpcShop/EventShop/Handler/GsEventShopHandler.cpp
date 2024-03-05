// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEventShopHandler.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsNpcShopMessageParam.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsNpcShopManager.h"

#include "Net/GsNetSendServiceItem.h"

#include "NpcShop/GsNpcShopHelper.h"

#include "UI/UIContent/Helper/GsUIHelper.h"


void FGsEventShopHandler::RegisterMessage()
{
	FGsMessageHolder* message = GMessage();
	MNpcShop& npcShopMsg = message->GetNpcShop();

	_npcShopMsgHandler.Emplace(npcShopMsg.AddRaw(MessageContentNpcShop::UPDATE_PURCHASE_AMOUNT,
		this, &FGsEventShopHandler::UpdatePurchasedItem));
}

int32 FGsEventShopHandler::InputBasketBuy(ItemDBId InItemDBID, ItemId InItemTID, EventProductId InProductId, ItemAmount InItemAmount, bool InIsAddedAmount)
{
	UGsNpcShopManager* npcShopManager = GSNpcShop();
	if (nullptr == npcShopManager)
		return -1;

	const FGsSchemaItemCommon* itemCommonData = FGsItem::GetTableDataByID(InItemTID);
	if (nullptr == itemCommonData)
		return -1;

	int32 groupId = 0;
	ItemAmount resultCount = 0;
	if (_npcSchemaData)
	{
		groupId = npcShopManager->GetGroupIdByProductId(_npcSchemaData->npcFunctionType, _npcSchemaData->shopGroupIdList, InProductId);
		FGsNpcShopItemInfo* itemInfo = npcShopManager->GetShopItemInfoByProductId(_npcSchemaData->npcFunctionType, groupId, InProductId);

		// 가격,인벤무게,인벤슬롯 카운트 확인하여 구매 가능한 최대 수량 확인
		EGsNotAllowedBuyReason type = EGsNotAllowedBuyReason::NONE;
		ItemAmount maximumCount = npcShopManager->GetMaximumCalculatorCount(_npcSchemaData->npcFunctionType, itemInfo, type);
		if (0 >= maximumCount)
			return -1;

		resultCount = InItemAmount > maximumCount ? maximumCount : InItemAmount;
	}

	bool isStackableItem = (1 < itemCommonData->maxStackCount);
	// 스택형 아이템인가?
	if (isStackableItem)
	{
		int32 slotIndex = -1;
		// 동일한 아이템이 존재하는지 확인
		TArray<FGsNpcShopQuickSlotItemInfo>& basketList = npcShopManager->GetBasketList();
		for (int32 i = 0; i < basketList.Num(); ++i)
		{
			FGsNpcShopQuickSlotItemInfo& data = basketList[i];
			// 올려진 아이템인가?
			if (data._itemTID == InItemTID && data._productId == InProductId)
			{
				// 이미 최대치만큼 올려졌다.
				if (data._itemAmount >= itemCommonData->maxStackCount)
					continue;

				ItemAmount totalAmount = (InIsAddedAmount) ? (data._itemAmount + resultCount) : resultCount;
				// 구매할 총 수량이 한 슬롯의 스택 가능 수량보다 많은가?
				if (totalAmount > itemCommonData->maxStackCount)
				{
					data._itemAmount = itemCommonData->maxStackCount;

					// 슬롯이 가득차지 않았으면 새로운 슬롯에 아이템을 추가한다.
					if (false == npcShopManager->IsBasketFull())
					{
						int32 overStackCount = totalAmount - itemCommonData->maxStackCount;
						slotIndex = npcShopManager->InputBasket(InItemDBID, InItemTID, InProductId, overStackCount, groupId, 0, false);
					}
				}
				else
				{
					data._itemAmount = totalAmount;
					slotIndex = i;
				}
				return slotIndex;
			}
		}
		return npcShopManager->InputBasket(InItemDBID, InItemTID, InProductId, resultCount, groupId, 0);
	}
	// 스택이 불가능한 경우 : 바로 장바구니 투입
	else
	{
		return npcShopManager->InputBasket(InItemDBID, InItemTID, InProductId, resultCount, groupId, 0);
	}

	return -1;
}

int32 FGsEventShopHandler::InputBasketSell(ItemDBId InItemDBID, ItemId InItemTID, ItemAmount InItemAmount, bool InIsAddedAmount)
{
	// 판매가 불가능하다.
	return -1;
}

bool FGsEventShopHandler::TryBuyItem()
{
	if (false == TryBuyEventNpcShopItem())
	{
		FGsInvalidateNpcShopItemList paramItemData(NpcFunctionType::EVENT_SHOP, _npcShopGroupIDList, ENpcShopProductListTab::ProductList_AllTab, true);
		GMessage()->GetNpcShop().SendMessage(MessageContentNpcShop::INVALIDATE_SHOP, &paramItemData);

#if WITH_EDITOR
		if (nullptr == _npcSchemaData)
		{
			GSLOG(Warning, TEXT("FGsEventShopHandler::TryBuyItem is EmptyData - _npcSchemaData"));
		}
		else if (NpcShopModeType::SellMode == _npcShopModeType)
		{
			GSLOG(Warning, TEXT("FGsEventShopHandler::TryBuyItem is SellMode"));
		}
#endif
		return false;
	}

	return true;
}

bool FGsEventShopHandler::TrySellItem()
{
	// 판매 기능이 없다.
	return false;
}

bool FGsEventShopHandler::TryBuyEventNpcShopItem()
{
	if (nullptr == _npcSchemaData || NpcShopModeType::BuyMode != _npcShopModeType)
		return false;

	UGsNpcShopManager* npcShopManager = GSNpcShop();
	if (nullptr == npcShopManager)
		return false;

	TArray<NpcEventShopItemBuyPair> packetList;
	const TArray<FGsNpcShopQuickSlotItemInfo>& basketList = npcShopManager->GetBasketList();
	for (const FGsNpcShopQuickSlotItemInfo& basketItemInfo : basketList)
	{
		FGsNpcShopItemInfo* itemInfo = npcShopManager->GetShopItemInfoByProductId(_npcSchemaData->npcFunctionType,
			basketItemInfo._shopGroupId, basketItemInfo._productId);
		EGsNotAllowedBuyReason type = EGsNotAllowedBuyReason::NONE;
		if (false == FGsNpcShopHelper::IsAllowedBuyItem(itemInfo, type))
		{
			// 구매 목록에 조건을 만족하지 않은 아이템이 있습니다.
			FText findText;
			FText::FindText(TEXT("NpcEventShopText"), TEXT("Purchase_Fail_Condition"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return false;
		}
		
		packetList.Emplace(basketItemInfo._shopGroupId, basketItemInfo._productId, basketItemInfo._itemAmount);
	}

	if (packetList.Num() > 0)
	{
		FGsNetSendServiceItem::SendBuyNpcEventShopItem(_npcGameId, packetList);
	}

	return true;
}

void FGsEventShopHandler::UpdatePurchasedItem(const IGsMessageParam* InParam)
{
	const FGsUpdatePurchasedItem* paramData = InParam->Cast<const FGsUpdatePurchasedItem>();
	if (nullptr == paramData)
		return;

	GSNpcShop()->SetItemPurchasedList(_npcSchemaData, paramData->_purchasedItemList);
}