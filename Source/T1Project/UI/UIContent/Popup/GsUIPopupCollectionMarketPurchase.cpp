// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupCollectionMarketPurchase.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Popup/Market/GsUIMarketPriceInfo.h"
#include "UI/UIContent/Popup/ItemCollection/GsUICollectionMarketPurchaseListEntry.h"
#include "UI/UIContent/Popup/GsUIPopupCollectionMarketPurchaseResult.h"
#include "Market/GsMarketCollectionData.h"
#include "Market/GsMarketHelper.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Management/ScopeGame/GsItemCollectionManager.h"
#include "Message/MessageParam/GsMarketMessageParam.h"
#include "Net/GsNetSendServiceMarket.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Currency/GsCurrencyHelper.h"


void UGsUIPopupCollectionMarketPurchase::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIPopupCollectionMarketPurchase::OnCreateEntry);
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupCollectionMarketPurchase::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupCollectionMarketPurchase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelRootList);
	_slotHelper->OnCreateEntry.AddDynamic(this, &UGsUIPopupCollectionMarketPurchase::OnCreateEntry);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupCollectionMarketPurchase::OnRefreshEntry);

	_toggleGroupItem.OnSelectChanged.BindUObject(this, &UGsUIPopupCollectionMarketPurchase::OnSelectItem);

	_toggleGroupPriceType.AddToggleWidget(_sBtnPricePerUnit);
	_toggleGroupPriceType.AddToggleWidget(_sBtnPrice);
	_toggleGroupPriceType.OnSlotClicked.BindUObject(this, &UGsUIPopupCollectionMarketPurchase::OnClickPriceType);

	_btnPurchase->OnClicked.AddDynamic(this, &UGsUIPopupCollectionMarketPurchase::OnClickPurchase);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupCollectionMarketPurchase::OnClickClose);
}

void UGsUIPopupCollectionMarketPurchase::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();	
}

void UGsUIPopupCollectionMarketPurchase::NativeDestruct()
{
	UnbindMessages();

	_dataList.Empty();
	_selectedList.Empty();

	Super::NativeDestruct();
}

void UGsUIPopupCollectionMarketPurchase::OnInputOk()
{
	if (0 == _selectedList.Num() ||
		0 == _dataList.Num())
	{
		return;
	}

	TArray<const FGsMarketCollectionItemData*> selectedList;
	TArray<MarketTransactionId> purchaseList;
	TArray<MarketTransactionId> wdPurchaseList;

	int32 totalWeight = 0;
	MarketPrice totalPrice = 0;

	for (const FGsMarketCollectionItemData* iter : _dataList)
	{
		if (IsSelectedItem(iter))
		{
			if (const FGsSchemaItemCommon* table = iter->GetTable())
			{
				totalWeight += table->weight;
			}

			totalPrice += iter->_price;

			selectedList.Emplace(iter);

			if (iter->_bIsWorldMarket)
			{
				wdPurchaseList.Emplace(iter->_transactionId);
			}
			else
			{
				purchaseList.Emplace(iter->_transactionId);
			}
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

	// 판매 요청 패킷 전송(일반, 월드거래소 나누어 요청)
	if (0 < purchaseList.Num())
	{
		FGsNetSendServiceMarket::SendReqTransactionPurchase(false, purchaseList);
	}

	if (0 < wdPurchaseList.Num())
	{
		FGsNetSendServiceMarket::SendReqTransactionPurchase(true, wdPurchaseList);
	}	
	
	Close();

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupCollectionMarketPurchaseResult"));
	if (widget.IsValid())
	{
		if (UGsUIPopupCollectionMarketPurchaseResult* popup = Cast<UGsUIPopupCollectionMarketPurchaseResult>(widget.Get()))
		{
			popup->SetData(selectedList);
		}
	}
}

void UGsUIPopupCollectionMarketPurchase::OnInputCancel()
{
	Close();
}

void UGsUIPopupCollectionMarketPurchase::BindMessages()
{
	MMarket& msgMarket = GMessage()->GetMarket();

	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::RCV_COLLECTION_PRICE_LIST,
		this, &UGsUIPopupCollectionMarketPurchase::OnReceiveMarketPriceList));
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::RCV_ITEM_PRICE,
		this, &UGsUIPopupCollectionMarketPurchase::OnReceiveItemPriceInfo));
}

void UGsUIPopupCollectionMarketPurchase::UnbindMessages()
{
	MMarket& msgMarket = GMessage()->GetMarket();

	for (MsgHandleMarket& handle : _msgHandleMarketList)
	{
		msgMarket.Remove(handle);
	}
	_msgHandleMarketList.Empty();
}

void UGsUIPopupCollectionMarketPurchase::SetData(CollectionId InCollectionId, TArray<TPair<ItemId, Level>>& InKeyList)
{
	_collectionId = InCollectionId;
	_keyList.Empty();
	_keyList.Append(InKeyList);

	FGsMarketCollectionData* marketData = GSMarket()->CreateOrGetCollectionData(_collectionId);
	if (nullptr == marketData)
	{
		return;
	}

	// 기본적으로 단가로 표시
	_toggleGroupPriceType.SetSelectedIndex(0);

	// 리스트 표시
	InvalidateList();

	// 금액 표시	
	_textBlockMyAmount->SetText(FText::AsNumber(FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::DIA)));
	InvalidateTotalPrice();
	
}

void UGsUIPopupCollectionMarketPurchase::InvalidateList()
{
	FGsMarketCollectionData* marketData = GSMarket()->CreateOrGetCollectionData(_collectionId);
	
	_dataList.Empty();
	marketData->GetMarketItemList(_keyList, IsPricePerUnit(), _dataList);	

	_selectedList.Empty();
	for (int i = 0; i < _dataList.Num(); ++i)
	{
		_selectedList.Emplace(TPair<MarketTransactionId, bool>(_dataList[i]->_transactionId, _dataList[i]->_bIsWorldMarket));
	}

	_priceInfoUI->SetEmpty();

	// 리스트 세팅
	_slotHelper->RefreshAll(_dataList.Num());
	_toggleGroupItem.Clear();
	_toggleGroupItem.AddToggleWidgetByParentPanel(_panelRootList);

	// true이므로 무조건 OnSelectItem을 1회 호출, 제일 첫 아이템의 시세정보를 요청 함
	_toggleGroupItem.SetSelectedIndex(0, true);
}

void UGsUIPopupCollectionMarketPurchase::InvalidateTotalPrice()
{
	Currency totalPrice = 0;
	for (const FGsMarketCollectionItemData* data : _dataList)
	{
		if (IsSelectedItem(data))
		{
			totalPrice += data->_price;
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

void UGsUIPopupCollectionMarketPurchase::OnReceiveMarketPriceList(const IGsMessageParam* InParam)
{
	const FGsMarketMsgParamId* param = InParam->Cast<const FGsMarketMsgParamId>();
	if (nullptr == param)
	{
		return;
	}

	if (param->_id != _collectionId)
	{
		return;
	}

	InvalidateList();
	InvalidateTotalPrice();
}

void UGsUIPopupCollectionMarketPurchase::OnReceiveItemPriceInfo(const IGsMessageParam*)
{
	// 시세 정보 수령
	const FGsMarketCollectionItemData* itemData = GetSelectedItem();
	if (nullptr == itemData)
	{
		return;
	}

	FGsMarketControllerBase* controller = GSMarket()->GetController(itemData->_bIsWorldMarket);
	if (nullptr == controller)
	{
		return;
	}

	const FGsMarketItemPriceData& priceData = controller->GetItemPriceData();
	if (itemData->_itemId == priceData._itemId)
	{
		_priceInfoUI->SetData(priceData);
	}
}

void UGsUIPopupCollectionMarketPurchase::OnSelectItem(int32 InIndex)
{
	if (const FGsMarketCollectionItemData* itemData = GetSelectedItem())
	{
		// 시세정보 요청
		FGsNetSendServiceMarket::SendReqItemPrice(itemData->_bIsWorldMarket, itemData->_itemId, itemData->_enchantLevel);
	}
}

void UGsUIPopupCollectionMarketPurchase::OnClickPriceType(int32 InIndex, bool bIsSelected)
{
	if (false == bIsSelected)
	{
		return;
	}

	FGsMarketCollectionData* marketData = GSMarket()->CreateOrGetCollectionData(_collectionId);
		
	bool bIsPricePerUnit = (0 == InIndex) ? true : false;

	if (marketData->IsCoolTime(bIsPricePerUnit))
	{
		InvalidateList();
		InvalidateTotalPrice();
	}
	else
	{
		const FGsItemCollectionField* collectionData = GSItemCollection()->FindFieldById(_collectionId);
		if (collectionData)
		{
			TArray<FGsMarketLowestPriceRequestInfo> reqList;
			FGsMarketCollectionData::GetLowestPriceRequestInfo(collectionData, reqList);

			// 서버에 요청 보내기
			FGsNetSendServiceMarket::SendReqCollectionItemMarketPriceLowest(bIsPricePerUnit, collectionData->Id(), reqList);

			marketData->UpdateCoolTime(bIsPricePerUnit);
		}
	}
}

void UGsUIPopupCollectionMarketPurchase::OnCreateEntry(UWidget* InEntry)
{
	if (UGsUICollectionMarketPurchaseListEntry* itemEntry = Cast<UGsUICollectionMarketPurchaseListEntry>(InEntry))
	{
		if (false == itemEntry->OnCheckItem.IsBound())
		{
			itemEntry->OnCheckItem.BindUObject(this, &UGsUIPopupCollectionMarketPurchase::OnClickCheckBox);
		}
	}			
}

void UGsUIPopupCollectionMarketPurchase::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _dataList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUICollectionMarketPurchaseListEntry* itemEntry = Cast<UGsUICollectionMarketPurchaseListEntry>(InEntry))
	{
		itemEntry->SetData(_dataList[InIndex], false);
		itemEntry->SetIsChecked(IsSelectedItem(_dataList[InIndex]));
	}
}

void UGsUIPopupCollectionMarketPurchase::OnClickCheckBox(MarketTransactionId InTransactionId, bool bIsWorldMarket, bool bIsChecked)
{
	if (bIsChecked)
	{
		_selectedList.AddUnique(TPair<MarketTransactionId, bool>(InTransactionId, bIsWorldMarket));
	}
	else
	{
		TPair<MarketTransactionId, bool> key(InTransactionId, bIsWorldMarket);
		if (_selectedList.Contains(key))
		{
			_selectedList.Remove(key);
		}
	}

	InvalidateTotalPrice();
}

void UGsUIPopupCollectionMarketPurchase::OnClickPurchase()
{
	OnInputOk();
}

void UGsUIPopupCollectionMarketPurchase::OnClickClose()
{
	OnInputCancel();
}

const FGsMarketCollectionItemData* UGsUIPopupCollectionMarketPurchase::GetSelectedItem() const
{
	if (_dataList.IsValidIndex(_toggleGroupItem.GetSelectedIndex()))
	{
		return _dataList[_toggleGroupItem.GetSelectedIndex()];
	}

	return nullptr;
}

bool UGsUIPopupCollectionMarketPurchase::IsPricePerUnit() const
{
	return (0 == _toggleGroupPriceType.GetSelectedIndex()) ? true : false;
}

bool UGsUIPopupCollectionMarketPurchase::IsSelectedItem(const FGsMarketCollectionItemData* InData)
{
	if (InData)
	{
		TPair<MarketTransactionId, bool> key(InData->_transactionId, InData->_bIsWorldMarket);
		return _selectedList.Contains(key);
	}

	return false;
}
