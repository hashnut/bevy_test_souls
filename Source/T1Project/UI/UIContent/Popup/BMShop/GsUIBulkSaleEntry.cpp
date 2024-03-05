// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/BMShop/GsUIBulkSaleEntry.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsCheckBox.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Item/GsItem.h"



void UGsUIBulkSaleEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(_iconSelector);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIBulkSaleEntry::OnRefreshItemIcon);
	}

	_btnPlus->OnClicked.AddDynamic(this, &UGsUIBulkSaleEntry::OnPlus);
	_btnMin->OnClicked.AddDynamic(this, &UGsUIBulkSaleEntry::OnMin);
}

void UGsUIBulkSaleEntry::BeginDestroy()
{
	if (nullptr != _iconSlotHelper)
	{
		_iconSlotHelper->Clear();
		_iconSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIBulkSaleEntry::OnShow()
{
	_maximumAmount = 0;
	_productID = 0;
	_amount = 0;
	_isSelected = false;

	_selectCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	if (false == _selectCheckBox->OnCheckStateChangedEx.IsBound())
	{
		_selectCheckBox->OnCheckStateChangedEx.AddDynamic(this, &UGsUIBulkSaleEntry::OnChangeCheckBox);
	}
}

void UGsUIBulkSaleEntry::OnHide()
{
	if (_selectCheckBox->OnCheckStateChangedEx.IsBound())
	{
		_selectCheckBox->OnCheckStateChangedEx.RemoveDynamic(this, &UGsUIBulkSaleEntry::OnChangeCheckBox);
	}
	if (OnClickCheckBox.IsBound())
	{
		OnClickCheckBox.Clear();
	}
	if (OnClickAmount.IsBound())
	{
		OnClickAmount.Clear();
	}

	_iconSelector->RemoveAllChildren();
}

void UGsUIBulkSaleEntry::SetProductInfo(const BMProductId InProductID, const bool InState, const ItemAmount InAmount)
{
	_productID = InProductID;
	_amount = InAmount;

	FGsBMShopManager* shopmgr = GBMShop();
	FGsBMShopProductData* data = shopmgr->FindProduct(_productID);
	if (nullptr == data)
		return;

	const FGsSchemaItemCommon* itemTableData = FGsItem::GetTableDataByID(data->GetItemTID());
	if (nullptr == itemTableData)
		return;

	_itemNameText->SetText(itemTableData->name);
	_maximumAmount = data->GetPurchaseQuantiyLimit() - data->GetPurchaseQuantiy();

	UpdateAmountCount();

	//_selectCheckBox->SetCheckedState((0 < shopmgr->GetBulkSaleAmount(_productID)) && (0 < _maximumAmount) && (true == data->IsEnoughLevelLimit())
		//? ECheckBoxState::Checked : ECheckBoxState::Unchecked);

	_selectCheckBox->SetCheckedState((shopmgr->GetBulkSaleState(_productID)) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);

	_limitRootPanel->SetVisibility(false == data->IsEnoughLevelLimit() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (false == data->IsEnoughLevelLimit())
	{
		FText findText;
		if (true == FText::FindText(TEXT("BMShopText"), TEXT("tagLevelLimit"), findText))
		{
			FText resultText = findText.Format(findText, data->GetLimitLevel());
			_limitReasonText->SetText(resultText);
		}
	}
	_isSelected = _selectCheckBox->IsChecked();
	_iconSlotHelper->RefreshAll(1);

	UpdateDimmed();
}

void UGsUIBulkSaleEntry::UpdateAmountCount()
{
	FString amountStr;
	amountStr = FString::FromInt(_amount) + " / " + FString::FromInt(_maximumAmount);
	_amountText->SetText(FText::FromString(amountStr));

	UpdateDimmed();
}

void UGsUIBulkSaleEntry::UpdateDimmed()
{
	_btnPlus->SetIsEnabled(_isSelected);
	_btnMin->SetIsEnabled(_isSelected);
	_amountText->SetIsEnabled(_isSelected);
}

void UGsUIBulkSaleEntry::OnRefreshItemIcon(int32 InIndex, class UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	FGsBMShopProductData* data = GBMShop()->FindProduct(_productID);
	if (nullptr == data)
		return;

	itemIcon->SetNotOwnItem(data->GetItemTID(), 1);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetItemNameToolTipFlag(false);
	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);
}

void UGsUIBulkSaleEntry::OnPlus()
{
	if (_amount >= _maximumAmount)
		return;

	++_amount;
	UpdateAmountCount();

	if (OnClickAmount.IsBound())
	{
		OnClickAmount.Broadcast(true, _productID, this);
	}

	/*
	if (false == _isSelected)
	{
		_selectCheckBox->SetCheckedState(ECheckBoxState::Checked);
	}
	*/
}

void UGsUIBulkSaleEntry::OnMin()
{
	if (0 >= _amount)
		return;

	--_amount;
	/*
	if (0 >= _amount && _isSelected)
	{
		_selectCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		_isSelected = false;
	}
	*/

	UpdateAmountCount();

	if (OnClickAmount.IsBound())
	{
		OnClickAmount.Broadcast(false, _productID, this);
	}
}

void UGsUIBulkSaleEntry::ValidCheck()
{

}

void UGsUIBulkSaleEntry::OnChangeCheckBox(int32 InIndexGroup, bool InState)
{
	_isSelected = InState;
	if (InState)
	{
		if (0 >= _maximumAmount)
		{
			/*
			_selectCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
			_isSelected = false;
			*/
		}
		else
		{
			if (0 >= _amount && _amount < _maximumAmount)
			{
				++_amount;
				UpdateAmountCount();	
			}
		}
	}

	if (OnClickCheckBox.IsBound())
	{
		OnClickCheckBox.Broadcast(_productID, _amount,_isSelected);
	}

	UpdateDimmed();
	//GSLOG(Error, TEXT("[UGsUIBulkSaleEntry::OnChangeCheckBox] - InIndexGroup : %d"), InIndexGroup);
	//GSLOG(Error, TEXT("[UGsUIBulkSaleEntry::OnChangeCheckBox] - InState : %d"), (uint8)InState);
}