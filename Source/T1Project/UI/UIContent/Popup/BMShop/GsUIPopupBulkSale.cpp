// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/BMShop/GsUIPopupBulkSale.h"
#include "Components/CanvasPanel.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"
#include "GsUIBulkSaleEntry.h"
#include "Net/GsNetSendServiceBMShop.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"
#include "Currency/GsCurrencyHelper.h"
#include "DataSchema/GsSchemaCurrencyData.h"
#include "T1Project.h"


void UGsUIPopupBulkSale::BeginDestroy()
{
	if (nullptr != _productListPanelSlotHelper)
	{
		_productListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupBulkSale::ProductListRefresh);
		_productListPanelSlotHelper = nullptr;
	}
	if (nullptr != _currencyListPanelSlotHelper)
	{
		_currencyListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupBulkSale::CurrencyListRefresh);
		_currencyListPanelSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPopupBulkSale::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _productListPanelSlotHelper)
	{
		_productListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_productListPanelSlotHelper->Initialize(_productListEntryWidgetClass, _bulkSaleListRootPanel);
		_productListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupBulkSale::ProductListRefresh);
	}
	if (nullptr == _currencyListPanelSlotHelper)
	{
		_currencyListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_currencyListPanelSlotHelper->Initialize(_currencyListEntryWidgetClass, _currencyListRootPanel);
		_currencyListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupBulkSale::CurrencyListRefresh);
	}

	_btnPurchase->OnClicked.AddDynamic(this, &UGsUIPopupBulkSale::OnPurchase);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupBulkSale::OnCancel);

}

void UGsUIPopupBulkSale::NativeConstruct()
{
	Super::NativeConstruct();

}

void UGsUIPopupBulkSale::NativeDestruct()
{
	_bulkSaleSettingListMap.Empty();
	_bulkSalePurchaseListMap.Empty();
	for (int32 i = 0; i < _displayCostInfoList.Num(); ++i)
	{
		if (_displayCostInfoList[i])
		{
			delete _displayCostInfoList[i];
			_displayCostInfoList[i] = nullptr;
		}
	}
	_displayCostInfoList.Empty();

	Super::NativeDestruct();
}

void UGsUIPopupBulkSale::SetBulkSaleList(TArray<BMProductIdAmountPair> InList)
{
	FGsBMShopManager* shopmgr = GBMShop();
	for (BMProductIdAmountPair iter : InList)
	{
		FGsBMShopProductData* data = shopmgr->FindProduct(iter.mBMProductId);
		if (nullptr == data)
			continue;

		if (false == _bulkSaleSettingListMap.Contains(iter.mBMProductId))
		{
			_bulkSaleSettingListMap.Emplace(iter.mBMProductId,
				TPair<bool, ItemAmount>(shopmgr->GetBulkSaleState(iter.mBMProductId), iter.mAmount));
		}
		if (false == _bulkSalePurchaseListMap.Contains(iter.mBMProductId))
		{
			int32 maximumAmount = data->GetPurchaseQuantiyLimit() - data->GetPurchaseQuantiy();
			if (false == data->IsEnoughLevelLimit())
			{
				iter.mAmount = 0;
			}
			else
			{
				iter.mAmount = maximumAmount < iter.mAmount ? maximumAmount : iter.mAmount;
			}
			_bulkSalePurchaseListMap.Emplace(iter.mBMProductId,
				TPair<bool, ItemAmount>(shopmgr->GetBulkSaleState(iter.mBMProductId) , iter.mAmount));
		}
	}

	CalcCost();
	_productListPanelSlotHelper->RefreshAll(_bulkSalePurchaseListMap.Num());
	_btnPurchase->SetIsEnabled(IsEnoughCost() && IsEnoughLevel());
}

void UGsUIPopupBulkSale::CalcCost()
{
	FGsBMShopManager* bmShopMgr = GBMShop();
	if (nullptr == bmShopMgr)
		return;

	TMap<BMProductId, DisplayCostInfo> displayAllInfoMap;
	for (TPair<BMProductId, TPair<bool, ItemAmount>> iter : _bulkSalePurchaseListMap)
	{
		BMProductId id = iter.Key;
		TPair<bool, ItemAmount> pairData = iter.Value;

		FGsBMShopProductData* data = bmShopMgr->FindProduct(id);
		if (nullptr == data)
			continue;

		bool isItemCurrency = data->IsItemCurrencyType();
		ItemId itemId = data->GetPurchaseCurrencyItemId();
		CurrencyType type = data->GetPurchaseCurrencyType();
		Currency amountResult = data->GetPurchasePrice() * pairData.Value;
		if (false == pairData.Key)
		{
			if (displayAllInfoMap.Contains(id))
			{
				displayAllInfoMap.Remove(id);
			}
			continue;
		}

		if (displayAllInfoMap.Contains(id))
		{
			if (0 >= amountResult)
			{
				displayAllInfoMap.Remove(id);
			}
			else
			{
				DisplayCostInfo costData = displayAllInfoMap[id];
				if (isItemCurrency)
				{
					costData._currency = costData._itemId == itemId ? costData._currency += amountResult : amountResult;
				}
				else
				{
					costData._currency = costData._currencyType == type ? costData._currency += amountResult : amountResult;
				}
			}
		}
		else
		{
			if (0 < amountResult)
			{
				displayAllInfoMap.Emplace(id, DisplayCostInfo(data->IsItemCurrencyType(), id, itemId, type, amountResult));
			}
		}
	}

	if (0 < _displayCostInfoList.Num())
	{
		for (int32 i = 0; i < _displayCostInfoList.Num(); ++i)
		{
			if (_displayCostInfoList[i])
			{
				delete _displayCostInfoList[i];
				_displayCostInfoList[i] = nullptr;
			}
		}
		_displayCostInfoList.Empty();
	}

	for (TPair<BMProductId, DisplayCostInfo>& iter : displayAllInfoMap)
	{
		bool find = false;
		DisplayCostInfo costInfoData = iter.Value;
		for (DisplayCostInfo* displayCostInfoData : _displayCostInfoList)
		{
			if (nullptr == displayCostInfoData)
				continue;

			if (0 < costInfoData._itemId)
			{
				if (displayCostInfoData->_itemId == costInfoData._itemId)
				{
					displayCostInfoData->_currency += costInfoData._currency;
					find = true;
					break;
				}
			}
			else
			{
				if (displayCostInfoData->_currencyType == costInfoData._currencyType)
				{
					displayCostInfoData->_currency += costInfoData._currency;
					find = true;
					break;
				}
			}
		}
		if (false == find)
		{
			_displayCostInfoList.Emplace(new DisplayCostInfo(costInfoData._isItemCurrency, costInfoData._productId, costInfoData._itemId, costInfoData._currencyType, costInfoData._currency));
		}
	}

	SortDisplayCurrency();
	_currencyListPanelSlotHelper->RefreshAll(_displayCostInfoList.Num());
	_btnPurchase->SetIsEnabled(IsEnoughCost() && IsEnoughLevel());

	/*
	for (DisplayCostInfo* displayCostInfoData : _displayCostInfoList)
	{
		if (nullptr == displayCostInfoData)
			continue;

		GSLOG(Error, TEXT("[displayCostInfoData] - displayCostInfoData->_currency : %llu"), displayCostInfoData->_currency);
	}
	*/

}

bool UGsUIPopupBulkSale::IsEnoughCost()
{
	if (0 >= _displayCostInfoList.Num())
		return false;

	bool isEnoughCost = true;
	for (DisplayCostInfo* iter : _displayCostInfoList)
	{
		if(nullptr == iter)
			continue;

		if (iter->_isItemCurrency)
		{
			if (false == FGsCurrencyHelper::CheckCurrency(iter->_itemId, iter->_currency, false))
			{
				isEnoughCost = false;
				break;
			}
		}
		else
		{
			if (false == FGsCurrencyHelper::CheckCurrency(iter->_currencyType, iter->_currency, false))
			{
				isEnoughCost = false;
				break;
			}
		}
		isEnoughCost = true;
	}
	return isEnoughCost;
}

bool UGsUIPopupBulkSale::IsEnoughLevel()
{
	FGsBMShopManager* bmShopMgr = GBMShop();
	if (nullptr == bmShopMgr)
		return false;

	for (TPair<BMProductId, TPair<bool, ItemAmount>> iter : _bulkSalePurchaseListMap)
	{
		BMProductId id = iter.Key;
		TPair<bool, ItemAmount> pair = iter.Value;
		if (0 < pair.Value)
		{
			FGsBMShopProductData* data = bmShopMgr->FindProduct(id);
			if (nullptr == data)
				continue;

			if (false == data->IsEnoughLevelLimit())
			{
				return false;
			}
		}
	}

	return true;
}

void UGsUIPopupBulkSale::OnPurchase()
{
	TArray<BMProductIdAmountPair> purchaseList;
	for (TPair<BMProductId, TPair<bool, ItemAmount>> iter : _bulkSalePurchaseListMap)
	{
		BMProductId id = iter.Key;
		TPair<bool, ItemAmount> pair = iter.Value;
		if ((true == pair.Key) && (0 < pair.Value))
		{
			purchaseList.Emplace(BMProductIdAmountPair(id, pair.Value));
		}
	}

	if (0 < purchaseList.Num())
	{
		// 일괄구매 요청
		MakeShared<FGsBMShopCheckValidProduct>(purchaseList, true, false, 0)->Send();
	}

	OnCancel();
}

void UGsUIPopupBulkSale::OnClickListCheckBox(BMProductId InProductID, ItemAmount InAmount, bool InState)
{
	//GSLOG(Error, TEXT("[UGsUIPopupBulkSale::OnClickListCheckBox] - InProductID : %d"), InProductID);
	//GSLOG(Error, TEXT("[UGsUIPopupBulkSale::OnClickListCheckBox] - InAmount : %d"), InAmount);
	//GSLOG(Error, TEXT("[UGsUIPopupBulkSale::OnClickListCheckBox] - InState : %d"), (uint8)InState);

	if (_bulkSaleSettingListMap.Contains(InProductID))
	{
		TPair<bool, ItemAmount>& pair = _bulkSaleSettingListMap[InProductID];
		pair.Key = InState;
	}

	if (_bulkSalePurchaseListMap.Contains(InProductID))
	{
		TPair<bool, ItemAmount>& pair = _bulkSalePurchaseListMap[InProductID];
		pair.Key = InState;
		pair.Value = InAmount;
	}

	CalcCost();
	//_btnPurchase->SetIsEnabled(IsEnoughCost() && IsEnoughLevel());
}

void UGsUIPopupBulkSale::OnCancel()
{
	TArray<BMProductBulkSale> expectSettingList;
	for (TPair<BMProductId, TPair<bool, ItemAmount>> iter : _bulkSaleSettingListMap)
	{
		BMProductId id = iter.Key;
		TPair<bool, ItemAmount> pair = iter.Value;
		expectSettingList.Emplace(BMProductBulkSale(pair.Key, BMProductIdAmountPair(id, pair.Value)));
	}
	// 일괄구매 세팅 저장
	FGsNetSendServiceBMShop::SendSaveBulkSaleSetting(1, expectSettingList);

	Close();
}

void UGsUIPopupBulkSale::OnClickAmount(bool InState, BMProductId InProductID, UGsUIBulkSaleEntry* InEntry)
{
	FGsBMShopManager* bmShopMgr = GBMShop();
	if (nullptr == bmShopMgr)
		return;

	FGsBMShopProductData* data = bmShopMgr->FindProduct(InProductID);
	if (nullptr == data)
		return;

	if (InState) // plus
	{
		// 옵션 수량 정보 갱신
		if (_bulkSaleSettingListMap.Contains(InProductID))
		{
			TPair<bool, ItemAmount>& pair = _bulkSaleSettingListMap[InProductID];
			pair.Key = InEntry->IsSelectedCheckBox();
			pair.Value = InEntry->GetAmount();
		}
		else
		{
			_bulkSaleSettingListMap.Emplace(InProductID, TPair<bool, ItemAmount>(InEntry->IsSelectedCheckBox(), InEntry->GetAmount()));
		}

		// 구매 수량 정보 갱신
		if (_bulkSalePurchaseListMap.Contains(InProductID))
		{
			TPair<bool, ItemAmount>& pair = _bulkSalePurchaseListMap[InProductID];
			pair.Key = InEntry->IsSelectedCheckBox();
			pair.Value = InEntry->GetAmount();
		}
		else
		{
			_bulkSalePurchaseListMap.Emplace(InProductID, TPair<bool, ItemAmount>(InEntry->IsSelectedCheckBox(), InEntry->GetAmount()));
		}
	
	}
	else // min
	{
		if (_bulkSaleSettingListMap.Contains(InProductID))
		{
			TPair<bool, ItemAmount>& pair = _bulkSaleSettingListMap[InProductID];
			pair.Key = InEntry->IsSelectedCheckBox();
			pair.Value = InEntry->GetAmount();
		}

		if (_bulkSalePurchaseListMap.Contains(InProductID))
		{
			TPair<bool, ItemAmount>& pair = _bulkSalePurchaseListMap[InProductID];
			pair.Key = InEntry->IsSelectedCheckBox();
			pair.Value = InEntry->GetAmount();		
		}	
	}

	CalcCost();	
}

void UGsUIPopupBulkSale::SortDisplayCurrency()
{
	if (1 >= _displayCostInfoList.Num())
	{
		return;
	}

	const UGsTableCurrencyData* DataTable = Cast<UGsTableCurrencyData>(FGsSchemaCurrencyData::GetStaticTable());
	if (nullptr == DataTable)
	{
		return;
	}

	_displayCostInfoList.Sort([&, DataTable](DisplayCostInfo& dataA, DisplayCostInfo& dataB)
		{
			int32 diff = (uint8)dataA._isItemCurrency - (uint8)dataB._isItemCurrency;
			if (0 != diff)
			{
				return (uint8)dataA._isItemCurrency < (uint8)dataB._isItemCurrency;
			}

			const FGsSchemaCurrencyData* RowA = nullptr;
			const FGsSchemaCurrencyData* RowB = nullptr;
			if (DataTable->FindRowById(dataA._currencyType, RowA) && DataTable->FindRowById(dataB._currencyType, RowB))
			{
				diff = (uint8)RowA->dropDownOrder - (uint8)RowB->dropDownOrder;
				if (0 != diff)
				{
					return (uint8)RowA->dropDownOrder < (uint8)RowB->dropDownOrder;
				}
			}
			return true;
		}
	);
}

void UGsUIPopupBulkSale::ProductListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUIBulkSaleEntry* entry = Cast<UGsUIBulkSaleEntry>(InEntry);
	if (nullptr == entry)
		return;

	int32 index = 0;
	for (TPair<BMProductId, TPair<bool, ItemAmount>> iter : _bulkSalePurchaseListMap)
	{
		if (InIndex != index)
		{
			++index;
			continue;
		}
		TPair<bool, ItemAmount> pair = iter.Value;
		entry->SetProductInfo(iter.Key, pair.Key, pair.Value);
		if (false == entry->OnClickAmount.IsBoundToObject(this))
		{
			entry->OnClickAmount.AddUObject(this, &UGsUIPopupBulkSale::OnClickAmount);
		}
		if (false == entry->OnClickCheckBox.IsBoundToObject(this))
		{
			entry->OnClickCheckBox.AddUObject(this, &UGsUIPopupBulkSale::OnClickListCheckBox);
		}

		break;
	}
}

void UGsUIPopupBulkSale::CurrencyListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUICurrencySlot* entry = Cast<UGsUICurrencySlot>(InEntry);
	if (nullptr == entry)
		return;

	if (_displayCostInfoList.IsValidIndex(InIndex))
	{
		DisplayCostInfo* costInfo = _displayCostInfoList[InIndex];
		if (costInfo)
		{
			if (0 < costInfo->_itemId)
			{
				entry->SetData(costInfo->_itemId, costInfo->_currency);
			}
			else
			{
				entry->SetData(costInfo->_currencyType, costInfo->_currency);
			}
			entry->UpdateAmountTextColorLocalPlayer();
		}
	}
}

void UGsUIPopupBulkSale::InvalidateCurrency()
{
	_currencyListPanelSlotHelper->RefreshAll(_displayCostInfoList.Num());
	_btnPurchase->SetIsEnabled(IsEnoughCost() && IsEnoughLevel());
}

void UGsUIPopupBulkSale::InvalidateLevel()
{
	_productListPanelSlotHelper->RefreshAll(_bulkSalePurchaseListMap.Num());
	_btnPurchase->SetIsEnabled(IsEnoughCost() && IsEnoughLevel());
}