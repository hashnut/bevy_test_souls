// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNpcShopBase.h"
#include "T1Project.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"

#include "Item/GsItemManager.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsNpcShopMessageParam.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UILib/Define/GsUIDefine.h"

#include "Net/GsNetSendServiceItem.h"

#include "NpcShop/GsNpcShopHelper.h"


void FGsNpcShopBase::CloseNpcShop()
{
	RemoveMessage();

	if (UGsNpcShopManager* shopManager = GSNpcShop())
	{
		shopManager->ClearBasketList();
	}

	_npcShopGroupIDList.Empty();
	_npcSchemaData = nullptr;

	GUI()->CloseByKeyName(TEXT("WindowNpcShop"));
	GUI()->ClearHideFlags(EGsUIHideFlags::STATE_NPC_SHOP);
}

// 상점 포인터가 소멸될때 호출
void FGsNpcShopBase::Close()
{
	CloseNpcShop();
}

// 단순 상점 나갈때 호출
void FGsNpcShopBase::Exit()
{
	CloseNpcShop();
}

void FGsNpcShopBase::EnterShop(const FGsSchemaNpcData* InTableData, const int64 InNpcGameId)
{
	if (nullptr == InTableData)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[FGsNpcShopBase::EnterShop] InTableData is nullptr !"));
#endif
		return;
	}

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	_npcSchemaData = InTableData;
	_npcGameId = InNpcGameId;

	_npcShopGroupIDList.Empty();
	TArray<int32> groupIdList = _npcSchemaData->shopGroupIdList;
	for (uint8 i = 0; i < groupIdList.Num(); ++i)
	{
		_npcShopGroupIDList.Emplace(groupIdList[i]);
	}

	_npcShopModeType = NpcShopModeType::BuyMode;

	uiManager->SetHideFlags(EGsUIHideFlags::SETTING_NPC_SHOP);

	RegisterMessage();

	TWeakObjectPtr<UGsUIWidgetBase> widgetWindow = uiManager->OpenAndGetWidget(TEXT("WindowNpcShop"));
	if (widgetWindow.IsValid())
	{
		FGsEnterNpcShop paramData(InTableData->npcFunctionType, _npcShopGroupIDList, ENpcShopProductListTab::ProductList_AllTab, true, InTableData);
		GMessage()->GetNpcShop().SendMessage(MessageContentNpcShop::OPEN, &paramData);
	}
}

bool FGsNpcShopBase::TryBuyItem()
{
	if (nullptr == _npcSchemaData || NpcShopModeType::BuyMode != _npcShopModeType)
		return false;

	UGsNpcShopManager* shopManager = GSNpcShop();
	if (nullptr == shopManager)
		return false;

	TArray<NpcShopItemBuyPair> packetList;
	const TArray<FGsNpcShopQuickSlotItemInfo>& basketList = shopManager->GetBasketList();
	for (const FGsNpcShopQuickSlotItemInfo& basketItemInfo : basketList)
	{
		FGsNpcShopItemInfo* itemInfo = shopManager->GetShopItemInfoByItemTID(_npcSchemaData->npcFunctionType,
			basketItemInfo._shopGroupId, basketItemInfo._itemTID);

		EGsNotAllowedBuyReason reason = EGsNotAllowedBuyReason::NONE;
		if (false == FGsNpcShopHelper::IsAllowedBuyItem(itemInfo, reason))
		{
			// 구매 목록에 조건을 만족하지 않은 아이템이 있습니다.
			FText findText;
			FText::FindText(TEXT("NpcEventShopText"), TEXT("Purchase_Fail_Condition"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return false;
		}

		packetList.Emplace(basketItemInfo._shopGroupId, basketItemInfo._itemTID, basketItemInfo._itemAmount);
	}

	FGsNetSendServiceItem::SendBuyNpcShopItem(_npcSchemaData->id , packetList);

	return true;
}

bool FGsNpcShopBase::TrySellItem()
{
	if (nullptr == _npcSchemaData || NpcShopModeType::SellMode != _npcShopModeType)
		return false;

	UGsNpcShopManager* shopManager = GSNpcShop();
	if (nullptr == shopManager)
		return false;

	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return false;

	TArray<ItemDBIdAmountPair> packetList;
	const TArray<FGsNpcShopQuickSlotItemInfo>& basketList = shopManager->GetBasketList();
	for (const FGsNpcShopQuickSlotItemInfo& basketItemInfo : basketList)
	{
		TWeakPtr<FGsItem> itemData = itemManager->FindItem(basketItemInfo._itemDBID);
		if (false == itemData.IsValid())
			return false;

		EGsNotAllowedSellReaon reason = EGsNotAllowedSellReaon::NONE;
		if (false == FGsNpcShopHelper::IsAllowedSellItem(itemData.Pin().Get(), reason))
		{
			return false;
		}

		packetList.Emplace(basketItemInfo._itemDBID, basketItemInfo._itemAmount);
	}

	FGsNetSendServiceItem::SendSellNpcShopItem(_npcSchemaData->id, packetList);

	return true;
}

// 이미 장바구니에 들어가 있는 동일한 아이템이 또 들어오면 기존꺼부터 스택 모두 쌓고 넘어감
// 최대 담을수 있는 제한이 있음
void FGsNpcShopBase::InputShoppingBasket(const TArray<FGsBasketTryInputInfo>& InItemList, bool InIsBuyMode, bool InIsAddedAmount)
{
	// 모드 변경 확인 : 다를 경우 초기화
	NpcShopModeType modeType = (true == InIsBuyMode) ? NpcShopModeType::BuyMode : NpcShopModeType::SellMode;
	if (_npcShopModeType != modeType)
	{
		if (UGsNpcShopManager* shopManager = GSNpcShop())
		{
			shopManager->ClearBasketList(true);
		}

		_npcShopModeType = modeType;

		FGsOnChangeNpcShopMode param(InIsBuyMode);
		GMessage()->GetNpcShop().SendMessage(MessageContentNpcShop::CHANGE_MODE, &param);
	}

	int32 slotIndex = -1;
	if (InIsBuyMode)
	{
		for (int32 i = 0;i < InItemList.Num();++i)
		{
			int32 tempSlotIndex = InputBasketBuy(InItemList[i]._itemDBId, InItemList[i]._itemId, InItemList[i]._productId, InItemList[i]._itemAmount, InIsAddedAmount);
			if (tempSlotIndex > slotIndex)
			{
				slotIndex = tempSlotIndex;
			}
		}
	}
	else
	{
		for (int32 i = 0;i < InItemList.Num();++i)
		{
			int32 tempSlotIndex = InputBasketSell(InItemList[i]._itemDBId, InItemList[i]._itemId, InItemList[i]._itemAmount, InIsAddedAmount);
			if (tempSlotIndex > slotIndex)
			{
				slotIndex = tempSlotIndex;
			}
		}
	}

	FGsInvalidateShoppingBasket paramData(slotIndex);
	GMessage()->GetNpcShop().SendMessage(MessageContentNpcShop::INVALIDATE_BASKET, &paramData);
}

int32 FGsNpcShopBase::InputBasketBuy(ItemDBId InItemDBID, ItemId InItemTID, EventProductId InProductId, ItemAmount InItemAmount, bool InIsAddedAmount)
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
		groupId = npcShopManager->GetGroupIdByItemTID(_npcSchemaData->npcFunctionType, _npcSchemaData->shopGroupIdList, InItemTID);
		FGsNpcShopItemInfo* itemInfo = npcShopManager->GetShopItemInfoByItemTID(_npcSchemaData->npcFunctionType, groupId, InItemTID);

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
			if (data._itemTID == InItemTID)
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

int32 FGsNpcShopBase::InputBasketSell(ItemDBId InItemDBID, ItemId InItemTID, ItemAmount InItemAmount, bool InIsAddedAmount)
{
	UGsNpcShopManager* npcShopManager = GSNpcShop();
	if (nullptr == npcShopManager)
		return -1;

	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return -1;

	int32 enchantLevel = 0;
	TWeakPtr<FGsItem> item = itemManager->FindItem(InItemDBID);
	if (false == item.IsValid())
		return -1;

	enchantLevel = item.Pin()->GetLevel();

	const FGsSchemaItemCommon* itemCommonData = FGsItem::GetTableDataByID(InItemTID);
	if (nullptr == itemCommonData)
		return -1;

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
			if (data._itemDBID == InItemDBID)
			{
				if (InIsAddedAmount)
				{
					ItemAmount totalOwndedItemCount = item.Pin()->GetAmount();
					ItemAmount calc = totalOwndedItemCount - data._itemAmount;
					if (InItemAmount >= calc)
					{
						InItemAmount = calc;
					}
				}

				// 이미 최대치만큼 올려졌다.
				if (data._itemAmount >= itemCommonData->maxStackCount)
					continue;
				
				ItemAmount totalAmount = (InIsAddedAmount) ? (data._itemAmount + InItemAmount) : InItemAmount;
				// 판매할 총 수량이 한 슬롯의 스택 가능 수량보다 많은가?
				if (totalAmount > itemCommonData->maxStackCount)
				{
					data._itemAmount = itemCommonData->maxStackCount;

					// 슬롯이 가득차지 않았으면 새로운 슬롯에 아이템을 추가한다.
					if (false == npcShopManager->IsBasketFull())
					{
						int32 overStackCount = totalAmount - itemCommonData->maxStackCount;
						slotIndex = npcShopManager->InputBasket(InItemDBID, InItemTID, overStackCount, 0, enchantLevel, false);
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
		return npcShopManager->InputBasket(InItemDBID, InItemTID, 0, InItemAmount, 0, enchantLevel);;
	}
	// 스택이 불가능한 경우 : 바로 장바구니 투입
	else
	{
		// 동일한 아이템을 눌렀을 경우
		const TArray<FGsNpcShopQuickSlotItemInfo>& basketList = npcShopManager->GetBasketList();
		for (const FGsNpcShopQuickSlotItemInfo& basketItemInfo : basketList)
		{
			if (basketItemInfo._itemDBID == InItemDBID)
			{
				return -1;
			}
		}

		return npcShopManager->InputBasket(InItemDBID, InItemTID, 0, InItemAmount, 0, enchantLevel);
	}

	return -1;
}

void FGsNpcShopBase::RemoveMessage()
{
	FGsMessageHolder* message = GMessage();
	for (TPair<MessageContentNpcShop, FDelegateHandle>& elemItem : _npcShopMsgHandler)
	{
		message->GetNpcShop().Remove(elemItem);
	}
	_npcShopMsgHandler.Empty();
}