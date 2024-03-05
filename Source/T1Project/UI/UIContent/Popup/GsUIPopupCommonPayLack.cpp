// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupCommonPayLack.h"
#include "Components/RichTextBlock.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "Currency/GsCurrencyHelper.h"
#include "DataSchema/GsSchemaCurrencyData.h"
#include "Currency/GsCurrencyHelper.h"


void UGsUIPopupCommonPayLack::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupCommonPayLack::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupCommonPayLack::OnClickCancel);
}

void UGsUIPopupCommonPayLack::NativeDestruct()
{
	_currencyType = CurrencyType::NONE;
	_itemId = INVALID_ITEM_ID;
	_bUseLink = false;

	Super::NativeDestruct();
}

void UGsUIPopupCommonPayLack::OnInputOk()
{
	Close();

	if (_bUseLink)
	{
		if (CurrencyType::NONE != _currencyType)
		{
			FGsCurrencyHelper::TryToNavigate(_currencyType);
		}
		else if (INVALID_ITEM_ID != _itemId)
		{
			FGsCurrencyHelper::TryToNavigate(_itemId);
		}

		if (_buttonCallback)
		{
			_buttonCallback(false);
		}
	}
	else
	{
		if (_buttonCallback)
		{
			_buttonCallback(true);
		}
	}
}

void UGsUIPopupCommonPayLack::OnInputCancel()
{
	Close();

	if (_buttonCallback)
	{
		_buttonCallback(false);
	}
}

void UGsUIPopupCommonPayLack::OnClickOk()
{
	OnInputOk();
}
	
void UGsUIPopupCommonPayLack::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupCommonPayLack::SetData(CurrencyType InCurrencyType, Currency InLackAmount, bool bInUseLink, 
	TFunction<void(bool)> InCallback)
{
	_currencyType = InCurrencyType;
	_itemId = INVALID_ITEM_ID;
	// 바로가기 사용이라도 가능할 때만 사용
	_bUseLink = bInUseLink && FGsCurrencyHelper::CanTryToNavigate(_currencyType);
	_buttonCallback = InCallback;	

	FText textMsg;
	if (_bUseLink)
	{
		// TEXT: 보유 재화가 부족합니다.\r\n해당 상점으로 이동하시겠습니까?
		FText::FindText(TEXT("BMShopText"), TEXT("purchaseFailLack2"), textMsg);

		_space->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_btnCancel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		// TEXT: 보유 재화가 부족합니다.
		FText::FindText(TEXT("BMShopText"), TEXT("purchaseFailLack1"), textMsg);

		_space->SetVisibility(ESlateVisibility::Collapsed);
		_btnCancel->SetVisibility(ESlateVisibility::Collapsed);
	}
	_richTextDesc->SetText(textMsg);

	_currencySlot->SetData(InCurrencyType, InLackAmount);
	_currencySlot->SetAmountTextColor(EGsUIColorType::DEFAULT_LACK);
}

void UGsUIPopupCommonPayLack::SetItemData(ItemId InItemId, ItemAmount InLackAmount, bool bInUseLink, 
	TFunction<void(bool)> InCallback)
{	
	_currencyType = CurrencyType::NONE;
	_itemId = InItemId;
	_bUseLink = bInUseLink && FGsCurrencyHelper::CanTryToNavigate(_itemId);	
	_buttonCallback = InCallback;
	
	FText textMsg;	
	if (_bUseLink)
	{
		// TEXT: 보유 아이템이 부족합니다.\r\n해당 상점으로 이동하시겠습니까?
		FText::FindText(TEXT("BMShopText"), TEXT("purchaseFailLack4"), textMsg);

		_space->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_btnCancel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		// TEXT: 보유 아이템이 부족합니다.
		FText::FindText(TEXT("BMShopText"), TEXT("purchaseFailLack3"), textMsg);

		_space->SetVisibility(ESlateVisibility::Collapsed);
		_btnCancel->SetVisibility(ESlateVisibility::Collapsed);
	}
	_richTextDesc->SetText(textMsg);

	_currencySlot->SetData(InItemId, InLackAmount);
	_currencySlot->SetAmountTextColor(EGsUIColorType::DEFAULT_LACK);
}
