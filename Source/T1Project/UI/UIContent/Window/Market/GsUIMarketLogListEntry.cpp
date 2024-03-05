// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMarketLogListEntry.h"
#include "Components/TextBlock.h"
#include "UI/UIContent/Window/Market/List/GsMarketLogListItem.h"
#include "Market/GsMarketData.h"
#include "Net/GsNetSendServiceMarket.h"
#if WITH_EDITOR
#include "T1Project.h"
#endif


void UGsUIMarketLogListEntry::NativeDestruct()
{
	_transactionId = INVALID_MARKET_TRANSACTION_ID;
	_bIsEquipType = false;

	Super::NativeDestruct();
}

void UGsUIMarketLogListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UGsMarketLogListItem* listItem = Cast<UGsMarketLogListItem>(ListItemObject);
	if (nullptr == listItem)
	{
		return;
	}

	InvalidateAll(listItem);
}

void UGsUIMarketLogListEntry::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	// 정산내역에서는 서버가 아이템 상세정보를 주지 않음.
	Super::OnLongPressIcon(InIcon);

	/*
	// equip타입이면 상세정보 받고 각 페이지에서 열기
	if (_bIsEquipType)
	{
		FGsNetSendServiceMarket::SendReqTransactionDetailEquip(_transactionId);
	}
	else
	{
		Super::OnLongPress(InIcon);
	}
	*/
}

void UGsUIMarketLogListEntry::RefreshAll()
{
	InvalidateAll(GetListItem<UGsMarketLogListItem>());
}

void UGsUIMarketLogListEntry::InvalidateAll(UGsMarketLogListItem* InListItem)
{
	const FGsMarketSettlementData* data = InListItem->_data;
	if (nullptr == data)
	{
		return;
	}

	const FGsSchemaItemCommon* table = data->GetTable();
	if (nullptr == table)
	{
		return;
	}

	_transactionId = data->_transactionId;
	_bIsEquipType = (ItemType::EQUIP == table->type) ? true : false;

	// 이름
	SetUIItemName(table->name, data->_enchantLevel, table->grade);

	// 아이콘
	if (_iconPtr.IsValid())
	{
		_iconPtr->SetNotOwnItem(data->GetItemId(), data->_amount, data->_enchantLevel);
	}

	_textBlockPrice->SetText(FText::AsNumber(data->_price));
	_textBlockEarnings->SetText(FText::AsNumber(data->_earnings));

	FDateTime dateTimeSale;
	FText textSaleDate;
	FText textSaleTime;
	FGsMarketSettlementData::GetSaleTimeInfo(data->_timeOfSale, dateTimeSale, textSaleDate, textSaleTime);

	_textBlockSaleDate->SetText(textSaleDate);
	_textBlockSaleTime->SetText(textSaleTime);

#if WITH_EDITOR
	// 로그의 시간과 화면의 시간이 동일하게보여야 정상
	GSLOG(Log, TEXT("%s, time: %s"), *table->name.ToString(), *dateTimeSale.ToString());
#endif
}