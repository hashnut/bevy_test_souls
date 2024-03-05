// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMarketControllerNormal.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"


bool FGsMarketControllerNormal::IsTradableItem(const FGsSchemaItemCommon* InRow) const
{
	if (InRow)
	{
		return InRow->tradable;
	}

	return false;
}

void FGsMarketControllerNormal::SetItemGroupList(PD::GC::PKT_GC_ACK_MARKET_SALES_ITEM_LIST_READ* InPacket)
{
	// 요청했던 카테고리랑 다르면 리턴처리. 패킷 지연상황에서 나올 수 있음
	if (InPacket->MarketCategory() != _lastUpdateCategory)
	{
		return;
	}

	bool bIsEmptyList = true;

	// 일부 리스트를 받는다
	for (PD::GC::PKT_GC_ACK_MARKET_SALES_ITEM_LIST_READ::SalesItemListIterator iter = InPacket->GetFirstSalesItemListIterator();
		iter != InPacket->GetLastSalesItemListIterator(); ++iter)
	{
		if (FGsMarketItemGroupData* itemGroupData = GetItemGroupDataInter(iter->ItemId()))
		{
			itemGroupData->SetData(&(*iter));

			// 포인터만 저장, 서버에서 이미 정렬해서 주는 정보이므로 더해주기만 하면 됨
			_serverItemGroupList.Emplace(itemGroupData);
		}

		_lastUpdateItemId = iter->ItemId();
		bIsEmptyList = false;
	}

	// 빈 리스트가 온 것은 다 받은 거라고 보면 된다.
	_bIsItemGroupListComplete = bIsEmptyList;
}

void FGsMarketControllerNormal::SetInstackableItemList(PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_LIST_INSTACKABLE_READ* InPacket)
{
	// 전체 리스트를 받는다
	ItemId itemId = InPacket->ItemId();
	FGsMarketItemGroupData* itemGroupData = GetItemGroupDataInter(itemId);
	if (nullptr == itemGroupData)
	{
		return;
	}

	itemGroupData->ClearItemData();

	FGsMarketItemGroupData* bookMarkItemGroupData = GetBookmarkItemGroupDataInter(itemId);
	if (bookMarkItemGroupData)
	{
		bookMarkItemGroupData->ClearItemData();
	}

	for (PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_LIST_INSTACKABLE_READ::TransactionListIterator iter = InPacket->GetFirstTransactionListIterator();
		iter != InPacket->GetLastTransactionListIterator(); ++iter)
	{
		FGsMarketItemData* itemData = GetItemDataInter(iter->MarketTransactionId());
		if (nullptr == itemData)
		{
			itemData = _poolItemData.Claim();
			_itemMap.Emplace(iter->MarketTransactionId(), itemData); // 원본 저장
		}

		itemData->SetInstackableItem(itemId, &(*iter));

		// 편의를 위해 포인터 저장
		itemGroupData->SetItemData(itemData);
		if (bookMarkItemGroupData)
		{
			bookMarkItemGroupData->SetItemData(itemData);
		}
	}

	// 마지막 시간 저장
	if (time_t* timePtr = _lastUpdateTimeMap.Find(itemId))
	{
		*timePtr = InPacket->LastUpdateTime();
	}
	else
	{
		_lastUpdateTimeMap.Emplace(itemId, InPacket->LastUpdateTime());
	}
}

void FGsMarketControllerNormal::SetStackableItemList(PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_LIST_STACKABLE_READ* InPacket)
{
	// 전체 리스트를 받는다
	ItemId itemId = InPacket->ItemId();
	FGsMarketItemGroupData* itemGroupData = GetItemGroupDataInter(itemId);
	if (nullptr == itemGroupData)
	{
		return;
	}

	itemGroupData->ClearItemData();

	FGsMarketItemGroupData* bookMarkItemGroupData = GetBookmarkItemGroupDataInter(itemId);
	if (bookMarkItemGroupData)
	{
		bookMarkItemGroupData->ClearItemData();
	}

	for (PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_LIST_STACKABLE_READ::TransactionListIterator iter = InPacket->GetFirstTransactionListIterator();
		iter != InPacket->GetLastTransactionListIterator(); ++iter)
	{
		FGsMarketItemData* itemData = GetItemDataInter(iter->MarketTransactionId());
		if (nullptr == itemData)
		{
			itemData = _poolItemData.Claim();
			_itemMap.Emplace(iter->MarketTransactionId(), itemData); // 원본 저장
		}

		itemData->SetStackableItem(itemId, &(*iter));

		// 편의를 위해 포인터 저장
		itemGroupData->SetItemData(itemData);
		if (bookMarkItemGroupData)
		{
			bookMarkItemGroupData->SetItemData(itemData);
		}
	}

	// 마지막 시간 저장
	if (time_t* timePtr = _lastUpdateTimeMap.Find(itemId))
	{
		*timePtr = InPacket->LastUpdateTime();
	}
	else
	{
		_lastUpdateTimeMap.Emplace(itemId, InPacket->LastUpdateTime());
	}
}

void FGsMarketControllerNormal::SetSettlementItemList(PD::GC::PKT_GC_ACK_MARKET_CALCULATE_LIST_READ* InPacket)
{
	_settlementList.Empty();

	for (PD::GC::PKT_GC_ACK_MARKET_CALCULATE_LIST_READ::CalculateListIterator iter = InPacket->GetFirstCalculateListIterator();
		iter != InPacket->GetLastCalculateListIterator(); ++iter)
	{

		FGsMarketSettlementData* data = GetSettlementDataInter(iter->MarketTransactionId());
		if (nullptr == data)
		{
			data = _poolSettlementData.Claim();
			_settlementMap.Emplace(iter->MarketTransactionId(), data); // 원본 저장
		}

		data->SetData(&(*iter));
		_settlementList.Emplace(data); // 포인터만 저장
	}
}

void FGsMarketControllerNormal::SetSellItemList(PD::GC::PKT_GC_ACK_MARKET_MY_TRANSACTION_LIST_READ* InPacket)
{
	_sellItemList.Empty();

	for (PD::GC::PKT_GC_ACK_MARKET_MY_TRANSACTION_LIST_READ::TransactionListIterator iter = InPacket->GetFirstTransactionListIterator();
		iter != InPacket->GetLastTransactionListIterator(); ++iter)
	{
		FGsMarketItemData* itemData = GetItemDataInter(iter->MarketTransactionId());
		if (nullptr == itemData)
		{
			itemData = _poolItemData.Claim();
			_itemMap.Emplace(iter->MarketTransactionId(), itemData); // 원본 저장
		}

		itemData->SetSellItem(&(*iter));

		_sellItemList.Emplace(itemData);
		// 아이템 그룹쪽에 저장하지 않음. 판매등록 중인 아이템이 아이템 리스트에 포함되지 않도록
	}
}

void FGsMarketControllerNormal::SetItemPrice(PD::GC::PKT_GC_ACK_MARKET_ITEM_PRICE_READ* InPacket)
{
	_priceData.SetData(InPacket);
}

void FGsMarketControllerNormal::SetSearchedList(PD::GC::PKT_GC_ACK_MARKET_ITEM_SEARCH_READ* InPacket)
{
	_serverSearchedItemGroupList.Empty();

	for (PD::GC::PKT_GC_ACK_MARKET_ITEM_SEARCH_READ::SalesItemListIterator iter = InPacket->GetFirstSalesItemListIterator();
		iter != InPacket->GetLastSalesItemListIterator(); ++iter)
	{
		FGsMarketItemGroupData* itemGroupData = GetItemGroupDataInter(iter->ItemId());
		if (itemGroupData)
		{
			itemGroupData->_lowestPrice = iter->LowestPrice();
			itemGroupData->_salesCount = iter->SalesCount();

			_serverSearchedItemGroupList.Emplace(itemGroupData);
		}
	}
}

void FGsMarketControllerNormal::SetItemDetailInfo(PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_DETAIL_EQUIP_READ* InPacket)
{
	FGsMarketItemData* itemData = GetItemDataInter(InPacket->MarketTransactionId());
	if (itemData)
	{
		itemData->SetDetailData(InPacket);
	}

	FGsMarketSettlementData* settlementData = GetSettlementDataInter(InPacket->MarketTransactionId());
	if (settlementData)
	{
		settlementData->SetDetailData(InPacket);
	}
}

void FGsMarketControllerNormal::SetBookmarkList(PD::GC::PKT_GC_ACK_MARKET_BOOKMARK_LIST_READ* InPacket)
{
	// pool에 반환
	for (FGsMarketItemGroupData* itemGroup : _bookmarkItemGroupList)
	{
		itemGroup->ClearItemGroupData();
		itemGroup->ClearItemData();

		_poolItemGroupData.Release(itemGroup);
	}
	_bookmarkItemGroupList.Empty();
	_bookmarkList.Empty();

	for (PD::GC::PKT_GC_ACK_MARKET_BOOKMARK_LIST_READ::MarketBookmarkListIterator iter = InPacket->GetFirstMarketBookmarkListIterator();
		iter != InPacket->GetLastMarketBookmarkListIterator(); ++iter)
	{
		if (FGsMarketItemGroupData* tempData = GetItemGroupDataInter(iter->ItemId()))
		{
			if (FGsMarketItemGroupData* itemGroupData = _poolItemGroupData.Claim())
			{
				itemGroupData->Set(tempData->_table);
				itemGroupData->SetDataBookmark(&(*iter));

				_bookmarkItemGroupList.Emplace(itemGroupData);
				_bookmarkList.Emplace(iter->ItemId());
			}
		}
	}
}

void FGsMarketControllerNormal::SetSettlementLogList(PD::GC::PKT_GC_ACK_MARKET_CALCULATED_LIST_READ* InPacket)
{
	// 마지막 받은 정보가 있으면 뒤에다 붙여야 한다
	if (INVALID_MARKET_TRANSACTION_ID == _lastUpdateLogPair.Key)
	{
		_settlementLogList.Empty();
	}

	bool bIsEmpty = true;
	for (PD::GC::PKT_GC_ACK_MARKET_CALCULATED_LIST_READ::CalculatedListIterator iter = InPacket->GetFirstCalculatedListIterator();
		iter != InPacket->GetLastCalculatedListIterator(); ++iter)
	{
		FGsMarketSettlementData* data = GetSettlementDataInter(iter->MarketTransactionId());
		if (nullptr == data)
		{
			data = _poolSettlementData.Claim();
			_settlementMap.Emplace(iter->MarketTransactionId(), data); // 원본 저장
		}

		data->SetLogData(&(*iter));

		_settlementLogList.Emplace(data); // 원본저장

		_lastUpdateLogPair.Key = data->_transactionId;
		_lastUpdateLogPair.Value = data->_timeOfSale;

		bIsEmpty = false;
	}

	// 비어있다는 것은 다 받았다는 뜻
	_bIsLogListComplete = bIsEmpty;

	_logTotalCount = InPacket->TotalCalculatedCount();
	_logTotalPrice = InPacket->TotalCalculatedPrice();
}

void FGsMarketControllerNormal::SetSellRegisteredItem(PD::SC::PKT_SC_ACK_MARKET_TRANSACTION_REGISTER_READ* InPacket)
{
	FGsMarketItemData* itemData = GetItemDataInter(InPacket->MarketTransactionId());
	if (nullptr == itemData)
	{
		itemData = _poolItemData.Claim();
		_itemMap.Emplace(InPacket->MarketTransactionId(), itemData); // 원본 저장
	}

	itemData->SetSellRegisteredItem(InPacket);

	_sellItemList.Emplace(itemData);
	// 아이템 그룹쪽에 저장하지 않음. 판매등록 중인 아이템이 아이템 리스트에 포함되지 않도록
}
