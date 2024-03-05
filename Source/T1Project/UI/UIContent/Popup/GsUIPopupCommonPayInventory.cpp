// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/GsUIPopupCommonPayInventory.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"

#include "Management/ScopeGame/GsInventoryManager.h"
#include "Management/ScopeGame/GsDepotManager.h"

#include "TextBlock.h"
#include "Currency/GsCurrencyHelper.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

void UGsUIPopupCommonPayInventory::OnClickYes()
{
	// 재화가 부족할 경우 cancel처럼 처리 후 재화부족 팝업 열기
	if (_amountHave < _amountNeeded)
	{
		FGsCurrencyHelper::OpenLackCurrencyPopup(_currencyType, _amountNeeded - _amountHave);
		return;
	}
	else
	{
		Super::OnClickYes();
	}
}

void UGsUIPopupCommonPayInventory::OnClickMinus()
{
	if (_isDepot)
	{
		if (FGsDepotManager* depotMgr = GSDepot())
		{
			Level currentExtendLevel = depotMgr->GetExtendedSlotNumber() + _curLevel;
			_amountNeeded -= depotMgr->GetExtendSlotCurrency(currentExtendLevel);
		}
	}
	else
	{
		if (UGsInventoryManager* invenMgr = GSInventory())
		{
			_amountNeeded -= _amountPerLevel;
		}
	}

	Super::OnClickMinus();
}

void UGsUIPopupCommonPayInventory::OnClickPlus()
{
	if (_isDepot)
	{
		if (FGsDepotManager* depotMgr = GSDepot())
		{
			Level currentExtendLevel = depotMgr->GetExtendedSlotNumber() + _curLevel;
			_amountNeeded += depotMgr->GetExtendSlotCurrency(currentExtendLevel+1);
		}
	}
	else
	{
		if (UGsInventoryManager* invenMgr = GSInventory())
		{
			_amountNeeded += _amountPerLevel;
		}
	}

	Super::OnClickPlus();
}

void UGsUIPopupCommonPayInventory::SetData(const FText& InContent, Level InPastLevel, Level InMaxLevel, bool InIsDepot, TFunction<void(bool, bool, Level)> InCallback /*= nullptr*/, TFunction<void(bool)> InDestructCallback /* = nullptr*/)
{
	_contentText->SetText(InContent);

	_pastLevel = InPastLevel;
	_curLevel = 1;
	_minLevel = 1;
	_maxLevel = InMaxLevel;
	
	_isDepot = InIsDepot;

	if (_isDepot)
	{
		if (FGsDepotManager* depotMgr = GSDepot())
		{
			CurrencyPair pairData;
			if (depotMgr->GetNextExtendSlotCurrenyData(pairData) == false)
				return;

			_currencyType = pairData.mCurrencyType;
			_amountHave = FGsCurrencyHelper::GetCurrencyAmount(_currencyType);
			_amountNeeded = pairData.mCurrency;
		}
	}
	else
	{
		if (UGsInventoryManager* invenMgr = GSInventory())
		{
			_currencyType = invenMgr->GetExtendSlotCurrencyType();
			_amountPerLevel = invenMgr->GetExtendSlotCost();
			_amountHave = FGsCurrencyHelper::GetCurrencyAmount(_currencyType);
			_amountNeeded = _curLevel * _amountPerLevel;
		}
	}

	_buttonCallback = InCallback;
	_destructCallback = InDestructCallback;

	RefreshUI();
}

void UGsUIPopupCommonPayInventory::RefreshUI()
{
	_amountText = FText::FromString(FString::Format(TEXT("{0} / {1}"), { _curLevel, _maxLevel }));

	_currencyCommonPay->SetData(_currencyType, _amountNeeded);
	_currencyCommonPay->SetAmountTextColor(_amountHave >= _amountNeeded ?
		EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK);
	_currencyLocalPay->SetData(_currencyType, _amountHave);

	_btnMin->SetIsEnabled(_curLevel > _minLevel);
	_btnPlus->SetIsEnabled(_curLevel < _maxLevel);
}