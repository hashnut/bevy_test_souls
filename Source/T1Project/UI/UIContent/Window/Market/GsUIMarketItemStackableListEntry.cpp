// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMarketItemStackableListEntry.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsBorder.h"
#include "UI/UIControlLib/ContentWidget/GsCheckBox.h"
#include "UI/UIContent/Window/Market/List/GsMarketItemListItem.h"
#include "Market/GsMarketItemData.h"
#include "Market/GsMarketHelper.h"
#include "Item/GsItem.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Message/MessageParam/GsMarketMessageParam.h"


void UGsUIMarketItemStackableListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_borderLongPress->OnFinishedLongPress.BindUObject(this, &UGsUIMarketItemStackableListEntry::OnFinishedLongPress);
	_checkBoxSelect->OnCheckStateChangedEx.AddDynamic(this, &UGsUIMarketItemStackableListEntry::OnClickCheckBox);
}

void UGsUIMarketItemStackableListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UGsMarketItemListItem* listItem = Cast<UGsMarketItemListItem>(ListItemObject);
	if (nullptr == listItem)
	{
		return;
	}

	InvalidateAll(listItem);
}

void UGsUIMarketItemStackableListEntry::RefreshAll()
{
	InvalidateAll(GetListItem<UGsMarketItemListItem>());
}

bool UGsUIMarketItemStackableListEntry::IsSlotLongPressed() const
{
	return _borderLongPress->IsLongPressed();
}

void UGsUIMarketItemStackableListEntry::InvalidateAll(UGsMarketItemListItem* InListItem)
{
	_transactionId = INVALID_MARKET_TRANSACTION_ID;
	_bIsWorldMarket = InListItem->_bIsWorldMarket;

	const FGsMarketItemData* data = InListItem->_data;
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

	// 이름
	SetUIItemName(table->name, 0, table->grade);

	// 아이콘
	if (_iconPtr.IsValid())
	{
		_iconPtr->SetNotOwnItem(data->GetItemId(), data->GetAmount(), data->GetEnchantLevel());
		// 롱프레스 타겟으로 설정
		_borderLongPress->SetLongpressBarTarget(_iconPtr.Get());
	}

	// 개당 판매금액 소수점 4째자리까지 표기
	FText textPricePerUnit;
	FGsMarketHelper::GetMarketPriceText(data->GetPricePerUnit(), true, textPricePerUnit);
	_textBlockPricePerUnit->SetText(textPricePerUnit);

	_textBlockPrice->SetText(FText::AsNumber(data->_price));

	// 체크 표시
	bool bIsChecked = false;
	if (FGsMarketManager* marketMgr = GSMarket())
	{
		if (0 < _transactionId)
		{
			bIsChecked = marketMgr->IsMultiplePurchaseTarget(_bIsWorldMarket, _transactionId);
		}
	}

	_checkBoxSelect->SetIsSelected(bIsChecked);
}

void UGsUIMarketItemStackableListEntry::OnFinishedLongPress()
{
	OpenItemDetailInfo(GetItem());
}

void UGsUIMarketItemStackableListEntry::OnClickCheckBox(int32 IndexInGroup, bool bIsChecked)
{
	if (0 == _transactionId)
	{
		return;
	}

	// 최대 갯수 체크
	if (bIsChecked)
	{
		int32 count = GSMarket()->GetMultiplePurchaseCount(_bIsWorldMarket);
		int32 maxCount = FGsMarketHelper::GetMaxMultiplePurchaseCount();
		if (maxCount <= count)
		{
			// 선택 취소
			_checkBoxSelect->SetIsSelected(false);

			return;
		}
	}

	FGsMarketMsgParamId param(_transactionId);

	if (bIsChecked)
	{
		GMessage()->GetMarket().SendMessage(MessageContentMarket::ADD_TO_PURCHASE_LIST, &param);
	}
	else
	{
		GMessage()->GetMarket().SendMessage(MessageContentMarket::REMOVE_FROM_PURCHASE_LIST, &param);
	}
}
