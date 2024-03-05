// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICollectionMarketPurchaseListEntry.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "Market/GsMarketCollectionData.h"
#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"


void UGsUICollectionMarketPurchaseListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUICollectionMarketPurchaseListEntry::OnClickSlot);
	_btnCheck->OnClicked.AddDynamic(this, &UGsUICollectionMarketPurchaseListEntry::OnClickCheck);
}

void UGsUICollectionMarketPurchaseListEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;
	_imgSelected->SetVisibility(_bIsSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

bool UGsUICollectionMarketPurchaseListEntry::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUICollectionMarketPurchaseListEntry::SetData(const FGsMarketCollectionItemData* InData, bool bIsResult)
{
	if (nullptr == InData)
	{
		return;
	}

	const FGsSchemaItemCommon* table = InData->GetTable();
	if (nullptr == table)
	{
		return;
	}

	_transactionId = InData->_transactionId;
	_bIsWorldMarket = InData->_bIsWorldMarket;

	// 이름
	SetUIItemName(table->name, InData->_enchantLevel, table->grade);

	// 아이콘
	if (_iconPtr.IsValid())
	{
		_iconPtr->SetNotOwnItem(InData->_itemId, InData->_amount, InData->_enchantLevel);
	}

	// 가격
	_textBlockPrice->SetText(FText::AsNumber(InData->_price));

	if (bIsResult)
	{
		_switcherResult->SetVisibility(ESlateVisibility::HitTestInvisible);
		_btnCheck->SetVisibility(ESlateVisibility::Collapsed);
		
		_btnSlot->SetVisibility(ESlateVisibility::Collapsed);
		SetIsSelected(false);

		SetPurchaseResult(EGsMarketPurchaseResult::WAIT);
	}
	else
	{
		_switcherResult->SetVisibility(ESlateVisibility::Collapsed);
		_btnCheck->SetVisibility(ESlateVisibility::Visible);

		_btnSlot->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGsUICollectionMarketPurchaseListEntry::SetIsChecked(bool bIsChecked)
{
	_btnCheck->SetIsSelected(bIsChecked);
}

void UGsUICollectionMarketPurchaseListEntry::SetPurchaseResult(EGsMarketPurchaseResult InResult)
{
	int32 index = static_cast<int32>(InResult);

	_switcherResult->SetActiveWidgetIndex(index);
}

void UGsUICollectionMarketPurchaseListEntry::SetTextFailResult(int32 InResult)
{
	PD::Result result = static_cast<PD::Result>(InResult);

	FText textFail;
	if (PD::Result::MARKET_ERROR_CANT_PURCHASE_MY_TRANSACTION == result)
	{
		// TEXT: 본인 등록 상품
		FText::FindText(TEXT("ItemCollectionMainText"), 
			TEXT("Collection_MarketPurchaseResult_PurchaseList_Fail_SelfEnroll"), textFail);
	}
	else
	{
		// TEXT: 구매 실패
		FText::FindText(TEXT("ItemCollectionMainText"), 
			TEXT("Collection_MarketPurchaseResult_PurchaseList_Result_Fail"), textFail);
	}

	_textBlockFail->SetText(textFail);
}

void UGsUICollectionMarketPurchaseListEntry::OnClickSlot()
{
	if (_toggleCallback)
	{
		// 토글을 위해 반대로 보냄
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}

void UGsUICollectionMarketPurchaseListEntry::OnClickCheck()
{
	bool bIsChecked = !_btnCheck->GetIsSelected();
	SetIsChecked(bIsChecked);

	OnCheckItem.ExecuteIfBound(_transactionId, _bIsWorldMarket, bIsChecked);
}
