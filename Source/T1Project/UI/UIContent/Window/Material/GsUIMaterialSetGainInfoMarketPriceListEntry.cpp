// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMaterialSetGainInfoMarketPriceListEntry.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemCollectionSlot.h"
#include "Market/GsMarketCollectionData.h"
#include "Item/Collection/GsItemCollectionCondition.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "UTIL/GsTableUtil.h"

void UGsUIMaterialSetGainInfoMarketPriceListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_sBtnPurchase->OnClicked.AddDynamic(this, &UGsUIMaterialSetGainInfoMarketPriceListEntry::OnClickPurchase);
}

void UGsUIMaterialSetGainInfoMarketPriceListEntry::NativeDestruct()
{	
	_index = 0;
	_data = nullptr;
	_price = 0;
	_bIsWorldMarket = false;

	Super::NativeDestruct();
}

void UGsUIMaterialSetGainInfoMarketPriceListEntry::SetData(int32 InIndex, const FGsItemCollectionCondition* InData,
	MarketPrice InPrice, bool bIsWorldMarket, bool bOtherLevelExist)
{
	_index = InIndex;
	_bIsWorldMarket = bIsWorldMarket;
	_price = InPrice;

	if (nullptr == InData)
	{
		return;
	}
	
	if (false == _iconPtr.IsValid())
	{
		return;
	}
	
	UGsUIIconItemCollectionSlot* itemIcon = Cast<UGsUIIconItemCollectionSlot>(_iconPtr.Get());
	if (nullptr == itemIcon)
	{
		return;
	}

	const TPair<ItemId, Level> keyPair = InData->EntryMaterialInfo();

	// 중복 동작을 줄이기 위해 같은 아이템인지 체크하여 처리
	if (_data)
	{
		const TPair<ItemId, Level> prevKeyPair = _data->EntryMaterialInfo();
		if (prevKeyPair.Key == keyPair.Key)
		{
			itemIcon->SetItemEnchantLevel(keyPair.Value);
		}
	}
	else
	{
		itemIcon->SetNotOwnItem(keyPair.Key, 1, keyPair.Value);
	}

	_data = InData;	

	itemIcon->SetData(*InData);
	itemIcon->SetSlotSelectionImage(false);

	if (const FGsSchemaItemCommon* itemTable = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(keyPair.Key))
	{
		_textBlockName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SetUIItemName(itemTable->name, keyPair.Value, itemTable->grade);
	}
	else
	{
		_textBlockName->SetVisibility(ESlateVisibility::Hidden);
	}	

	SetState((ECollectionMaterialActionType::Completed == _data->ActionType()) ? true : false, _bIsWorldMarket, bOtherLevelExist);
}

void UGsUIMaterialSetGainInfoMarketPriceListEntry::SetState(bool bIsCollected, bool bIsWorldMarket, bool bOtherLevelExist)
{	
	bool bCanPurchase = false;
	FText textState;

	if (bIsCollected)
	{
		// TEXT: 이미 등록된 아이템입니다.		
		FText::FindText(TEXT("ItemCollectionMainText"), TEXT("CollectionDetail_Market_ItemList_AlreadyCompleted"), textState);
	}
	else
	{
		if (_data)
		{
			if (0 < _price)
			{
				bCanPurchase = true;
				_textBlockPrice->SetText(FText::AsNumber(_price));
			}
			else
			{
				if (bOtherLevelExist)
				{
					// TEXT: 강화도가 다른 아이템을 구매할 수 있습니다
					FText::FindText(TEXT("ItemCollectionMainText"), TEXT("CollectionDetail_Market_itemList_EnhancePurchase"), textState);
				}
			}
		}
	}

	if (textState.IsEmpty())
	{
		// TEXT: 구매할 수 있는 상품이 없습니다.
		FText::FindText(TEXT("ItemCollectionMainText"), TEXT("CollectionDetail_Market_ItemList_NoPurchaseAvail"), textState);
	}

	// 슬롯 세팅
	_switcherState->SetActiveWidgetIndex(bCanPurchase ? 1 : 0);
	_textBlockState->SetText(textState);	

	// 버튼 세팅
	int32 btnStateIndex = bIsWorldMarket ? 1 : 0;
	if (bOtherLevelExist)
	{
		btnStateIndex = 2;
	}

	_sBtnPurchase->SetSwitcherIndex(btnStateIndex);
	_sBtnPurchase->SetIsEnabled((bCanPurchase || bOtherLevelExist) ? true : false);
}

void UGsUIMaterialSetGainInfoMarketPriceListEntry::OnClickPurchase()
{
	OnPurchase.ExecuteIfBound(_index);
}
