// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupMarketMultiplePurchase.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Popup/Market/GsUIMarketPriceInfo.h"
#include "UI/UIContent/Popup/Market/GsUIMarketMultiplePurchaseListEntry.h"
#include "UI/UIContent/Popup/GsUIPopupMarketMultiplePurchaseResult.h"
#include "Market/GsMarketItemData.h"
#include "Market/GsMarketHelper.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Message/MessageParam/GsMarketMessageParam.h"
#include "Net/GsNetSendServiceMarket.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Currency/GsCurrencyHelper.h"


void UGsUIPopupMarketMultiplePurchase::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIPopupMarketMultiplePurchase::OnCreateEntry);
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupMarketMultiplePurchase::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupMarketMultiplePurchase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelRootList);
	_slotHelper->OnCreateEntry.AddDynamic(this, &UGsUIPopupMarketMultiplePurchase::OnCreateEntry);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupMarketMultiplePurchase::OnRefreshEntry);

	_toggleGroupItem.OnSelectChanged.BindUObject(this, &UGsUIPopupMarketMultiplePurchase::OnSelectItem);

	_btnPurchase->OnClicked.AddDynamic(this, &UGsUIPopupMarketMultiplePurchase::OnClickPurchase);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupMarketMultiplePurchase::OnClickClose);
}

void UGsUIPopupMarketMultiplePurchase::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();	
}

void UGsUIPopupMarketMultiplePurchase::NativeDestruct()
{
	UnbindMessages();

	_dataList.Empty();
	_selectedList.Empty();

	Super::NativeDestruct();
}

void UGsUIPopupMarketMultiplePurchase::OnInputOk()
{
	if (0 == _selectedList.Num() ||
		0 == _dataList.Num())
	{
		return;
	}

	int32 totalWeight = 0;
	MarketPrice totalPrice = 0;

	for (const FGsMarketItemData* iter : _dataList)
	{
		if (_selectedList.Contains(iter->_transactionId))
		{
			if (const FGsSchemaItemCommon* table = iter->GetTable())
			{
				totalWeight += table->weight;
			}

			totalPrice += iter->_price;
		}
	}

	// 인벤토리 상태 체크
	if (false == FGsMarketHelper::CheckInventoryState(totalWeight, true))
	{
		return;
	}

	// 재화 상태 체크
	if (false == FGsCurrencyHelper::CheckCurrency(CurrencyType::DIA, totalPrice, false))
	{
		Currency lackAmount = totalPrice - FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::DIA);
		FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType::DIA, lackAmount);

		return;
	}	

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupMarketMultiplePurchaseResult"));
	if (widget.IsValid())
	{
		if (UGsUIPopupMarketMultiplePurchaseResult* popup = Cast<UGsUIPopupMarketMultiplePurchaseResult>(widget.Get()))
		{
			popup->SetData(_bIsWorldMarket, _selectedList);
		}
	}

	// 판매 요청 패킷 전송(_dataList의 item이 구매되면 지워질 수 있으므로 순서에 유의)
	FGsNetSendServiceMarket::SendReqTransactionPurchase(_bIsWorldMarket, _selectedList);

	// 리스트 삭제
	if (FGsMarketManager* marketMgr = GSMarket())
	{
		marketMgr->ClearMultiplePurchaseList();
	}
	
	Close();
}

void UGsUIPopupMarketMultiplePurchase::OnInputCancel()
{
	Close();
}

void UGsUIPopupMarketMultiplePurchase::BindMessages()
{
	MMarket& msgMarket = GMessage()->GetMarket();

	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::RCV_ITEM_PRICE,
		this, &UGsUIPopupMarketMultiplePurchase::OnReceiveItemPriceInfo));
}

void UGsUIPopupMarketMultiplePurchase::UnbindMessages()
{
	MMarket& msgMarket = GMessage()->GetMarket();

	for (MsgHandleMarket& handle : _msgHandleMarketList)
	{
		msgMarket.Remove(handle);
	}
	_msgHandleMarketList.Empty();
}

void UGsUIPopupMarketMultiplePurchase::SetData(bool bIsWorldMarket, TArray<MarketTransactionId>& InIdList)
{
	_bIsWorldMarket = bIsWorldMarket;
	_dataList.Empty();
	_selectedList.Empty();

	if (0 == InIdList.Num())
	{
		return;
	}

	FGsMarketManager* marketMgr = GSMarket();
	if (nullptr == marketMgr)
	{
		return;
	}

	FGsMarketControllerBase* controller = marketMgr->GetController(_bIsWorldMarket);
	if (nullptr == controller)
	{
		return;
	}
	
	// 처음엔 전부 선택
	_selectedList.Append(InIdList);

	// 데이터 채우기
	for (int i = 0; i < InIdList.Num(); ++i)
	{
		if (const FGsMarketItemData* itemData = controller->GetItemData(InIdList[i]))
		{
			_dataList.Emplace(itemData);
		}
	}

	// 리스트 표시
	InvalidateList();

	// 금액 표시	
	_textBlockMyAmount->SetText(FText::AsNumber(FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::DIA)));
	InvalidateTotalPrice();
}

void UGsUIPopupMarketMultiplePurchase::InvalidateList()
{
	_priceInfoUI->SetEmpty();

	// 리스트 세팅
	_slotHelper->RefreshAll(_dataList.Num());
	_toggleGroupItem.Clear();
	_toggleGroupItem.AddToggleWidgetByParentPanel(_panelRootList);

	// true이므로 무조건 OnSelectItem을 1회 호출, 제일 첫 아이템의 시세정보를 요청 함
	_toggleGroupItem.SetSelectedIndex(0, true);
}

void UGsUIPopupMarketMultiplePurchase::InvalidateTotalPrice()
{
	Currency totalPrice = 0;
	for (const FGsMarketItemData* data : _dataList)
	{
		if (data)
		{
			if (_selectedList.Contains(data->_transactionId))
			{
				totalPrice += data->_price;
			}
		}
	}

	_textBlockTotalPrice->SetText(FText::AsNumber(totalPrice));

	_btnPurchase->SetIsEnabled((0 < _selectedList.Num()) ? true : false);

	if (FGsCurrencyHelper::CheckCurrency(CurrencyType::DIA, totalPrice, false))
	{
		_textBlockLowCurrency->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_textBlockLowCurrency->SetVisibility(ESlateVisibility::Visible);		
	}
}

void UGsUIPopupMarketMultiplePurchase::OnReceiveItemPriceInfo(const IGsMessageParam*)
{
	// 시세 정보 수령
	const FGsMarketItemData* itemData = GetSelectedItem();
	if (nullptr == itemData)
	{
		return;
	}

	if (FGsMarketControllerBase* controller = GSMarket()->GetController(_bIsWorldMarket))
	{
		const FGsMarketItemPriceData& priceData = controller->GetItemPriceData();
		if (itemData->GetItemId() == priceData._itemId)
		{
			_priceInfoUI->SetData(priceData);
		}
	}
}

void UGsUIPopupMarketMultiplePurchase::OnSelectItem(int32 InIndex)
{
	if (const FGsMarketItemData* itemData = GetSelectedItem())
	{
		// 시세정보 요청
		FGsNetSendServiceMarket::SendReqItemPrice(_bIsWorldMarket, itemData->GetItemId(), itemData->GetEnchantLevel());
	}
}

void UGsUIPopupMarketMultiplePurchase::OnCreateEntry(UWidget* InEntry)
{
	if (UGsUIMarketMultiplePurchaseListEntry* itemEntry = Cast<UGsUIMarketMultiplePurchaseListEntry>(InEntry))
	{
		if (false == itemEntry->OnCheckItem.IsBound())
		{
			itemEntry->OnCheckItem.BindUObject(this, &UGsUIPopupMarketMultiplePurchase::OnClickCheckBox);
		}
	}
}

void UGsUIPopupMarketMultiplePurchase::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _dataList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIMarketMultiplePurchaseListEntry* itemEntry = Cast<UGsUIMarketMultiplePurchaseListEntry>(InEntry))
	{
		itemEntry->SetData(_dataList[InIndex], false);
		if (_dataList[InIndex])
		{
			itemEntry->SetIsChecked(_selectedList.Contains(_dataList[InIndex]->_transactionId));
		}
	}
}

void UGsUIPopupMarketMultiplePurchase::OnClickCheckBox(MarketTransactionId InTransactionId, bool bIsChecked)
{
	if (bIsChecked)
	{
		_selectedList.AddUnique(InTransactionId);
	}
	else
	{
		_selectedList.Remove(InTransactionId);
	}

	InvalidateTotalPrice();
}

void UGsUIPopupMarketMultiplePurchase::OnClickPurchase()
{
	OnInputOk();
}

void UGsUIPopupMarketMultiplePurchase::OnClickClose()
{
	OnInputCancel();
}

const FGsMarketItemData* UGsUIPopupMarketMultiplePurchase::GetSelectedItem() const
{
	int selectedIndex = _toggleGroupItem.GetSelectedIndex();
	
	if (_dataList.IsValidIndex(selectedIndex))
	{
		return _dataList[selectedIndex];
	}

	return nullptr;
}
