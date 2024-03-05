// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMarketPageSell.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Common/Inventory/GsUIInventoryCommon.h"
#include "UI/UIContent/Common/Inventory/GsInventoryLogicMarket.h"
#include "UI/UIContent/Window/Market/GsUIMarketSellListEntry.h"
#include "UI/UIContent/Popup/GsUIPopupMarketSell.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Management/GsMessageHolder.h"
#include "Market/GsMarketHelper.h"
#include "Market/GsMarketItemData.h"
#include "DataSchema/Market/GsSchemaMarketConfig.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Item/GsItemManager.h"
#include "Currency/GsCurrencyHelper.h"


void UGsUIMarketPageSell::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIMarketPageSell::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	if (_inventoryLogic)
	{
		delete _inventoryLogic;
	}
	_inventoryLogic = nullptr;

	Super::BeginDestroy();
}

void UGsUIMarketPageSell::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelList);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIMarketPageSell::OnRefreshEntry);
	_slotHelper->SetEmptyListPanel(_panelEmptyList);

	_inventoryLogic = new FGsInventoryLogicMarket();
	_inventoryUI->OnClickItem().BindUObject(this, &UGsUIMarketPageSell::OnClickInventoryItem);
}

void UGsUIMarketPageSell::NativeConstruct()
{
	Super::NativeConstruct();

	_inventoryUI->SetData(_inventoryLogic);
}

void UGsUIMarketPageSell::Show(bool bIsWorldMarket)
{
	Super::Show(bIsWorldMarket);

	if (_inventoryLogic)
	{
		_inventoryLogic->SetIsWorldMarket(bIsWorldMarket);
	}

	GSMarket()->SendReqSellItemList(IsWorldMarket(), false);

	if (const FGsSchemaMarketConfig* config = FGsMarketHelper::GetMarketConfig())
	{
		int32 maxRegistrationCount = bIsWorldMarket ? config->wdMaxRegistrationCount : config->maxRegistrationCount;
		_textBlockMaxCount->SetText(FText::AsNumber(maxRegistrationCount));
	}

	_textBlockTax->SetText(FGsCurrencyHelper::GetTextTotalTax(CurrencyType::DIA));
	
	_textBlockTaxSub->SetVisibility(ESlateVisibility::HitTestInvisible);
	_textBlockTaxSub->SetText(FGsCurrencyHelper::GetTextTaxInfo(CurrencyType::DIA));

	UpdateItemList();

	_inventoryUI->InvalidateData();
}

void UGsUIMarketPageSell::OnUpdateItemList()
{
	UpdateItemList();
}

void UGsUIMarketPageSell::OnRcvSellRegisterdItem(MarketTransactionId InTransactionId)
{
	UpdateItemList();
}

void UGsUIMarketPageSell::OnRcvSellCancelItem(MarketTransactionId InTransactionId)
{
	UpdateItemList();
}

void UGsUIMarketPageSell::OnRcvItemDetailInfo(MarketTransactionId InTransactionId)
{
	FGsMarketControllerBase* controller = GSMarket()->GetController(IsWorldMarket());
	if (nullptr == controller)
	{
		return;
	}

	if (const FGsMarketItemData* itemData = controller->GetItemData(InTransactionId))
	{
		if (itemData->_item)
		{
			FGsItemDetailOpenMessageParam param(itemData->_item, false, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
			GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
		}
	}
}

void UGsUIMarketPageSell::OnReqRefresh()
{
	GSMarket()->SendReqSellItemList(IsWorldMarket(), true);
}

void UGsUIMarketPageSell::UpdateItemList()
{
	FGsMarketControllerBase* controller = GSMarket()->GetController(IsWorldMarket());
	if (nullptr == controller)
	{
		return;
	}

	_dataList.Empty();
	TArray<FGsMarketItemData*> tempList = controller->GetSellItemList();
	_dataList.Append(tempList);

	_slotHelper->RefreshAll(_dataList.Num());

	// 판매개수
	_textBlockCount->SetText(FText::AsNumber(_dataList.Num()));

	// 판매총액
	int32 totalPrice = 0;
	for (FGsMarketItemData* data : _dataList)
	{
		totalPrice += data->_price;
	}
	_textBlockTotalPrice->SetText(FText::AsNumber(totalPrice));
}

void UGsUIMarketPageSell::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (_dataList.IsValidIndex(InIndex))
	{
		if (UGsUIMarketSellListEntry* entry = Cast<UGsUIMarketSellListEntry>(InEntry))
		{
			entry->SetData(IsWorldMarket(), _dataList[InIndex]);
		}
	}
}

void UGsUIMarketPageSell::OnClickInventoryItem(uint64 InItemDBId)
{
	if (INVALID_ITEM_DB_ID == InItemDBId)
	{
		return;
	}

	TWeakPtr<FGsItem> item = GItem()->FindItem(InItemDBId);
	if (item.IsValid())
	{
		if (_inventoryLogic)
		{
			if (false == _inventoryLogic->CanSellItem(item.Pin().Get(), true))
			{
				return;
			}
		}
	}

	if (const FGsSchemaMarketConfig* config = FGsMarketHelper::GetMarketConfig())
	{
		int32 maxRegistrationCount = IsWorldMarket() ? config->wdMaxRegistrationCount : config->maxRegistrationCount;
		if (maxRegistrationCount <= _dataList.Num())
		{
			// TEXT: 더 이상 아이템을 판매 등록할 수 없습니다.
			FText textMsg;
			if (FText::FindText(TEXT("MarketText"), TEXT("Ticker_Sales_Fail_MaxRegistration"), textMsg))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}

			return;			
		}
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupMarketSell"));
	if (widget.IsValid())
	{
		if (UGsUIPopupMarketSell* popup = Cast<UGsUIPopupMarketSell>(widget.Get()))
		{
 			popup->SetData(IsWorldMarket(), InItemDBId);
		}
	}
}
