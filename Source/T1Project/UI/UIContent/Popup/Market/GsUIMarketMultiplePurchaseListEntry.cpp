// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMarketMultiplePurchaseListEntry.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "Market/GsMarketItemData.h"
#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"


void UGsUIMarketMultiplePurchaseListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIMarketMultiplePurchaseListEntry::OnClickSlot);
	_btnCheck->OnClicked.AddDynamic(this, &UGsUIMarketMultiplePurchaseListEntry::OnClickCheck);
}

void UGsUIMarketMultiplePurchaseListEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;
	_imgSelected->SetVisibility(_bIsSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

bool UGsUIMarketMultiplePurchaseListEntry::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUIMarketMultiplePurchaseListEntry::SetData(const FGsMarketItemData* InData, bool bIsResult)
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

	// 이름
	SetUIItemName(table->name, InData->GetEnchantLevel(), table->grade);

	// 아이콘
	if (_iconPtr.IsValid())
	{
		_iconPtr->SetNotOwnItem(InData->_itemId, InData->GetAmount(), InData->GetEnchantLevel());
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

void UGsUIMarketMultiplePurchaseListEntry::SetIsChecked(bool bIsChecked)
{
	_btnCheck->SetIsSelected(bIsChecked);
}

void UGsUIMarketMultiplePurchaseListEntry::SetPurchaseResult(EGsMarketPurchaseResult InResult)
{
	int32 index = static_cast<int32>(InResult);

	_switcherResult->SetActiveWidgetIndex(index);
}

void UGsUIMarketMultiplePurchaseListEntry::SetTextFailResult(int32 InResult)
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

void UGsUIMarketMultiplePurchaseListEntry::OnClickSlot()
{
	if (_toggleCallback)
	{
		// 토글을 위해 반대로 보냄
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}

void UGsUIMarketMultiplePurchaseListEntry::OnClickCheck()
{
	bool bIsChecked = !_btnCheck->GetIsSelected();
	SetIsChecked(bIsChecked);

	OnCheckItem.ExecuteIfBound(_transactionId, bIsChecked);
}
