// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupMarketSellConfirm.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/Item/GsUIItemDefaultInfo.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "Item/GsItemManager.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"
#include "Market/GsMarketHelper.h"


void UGsUIPopupMarketSellConfirm::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupMarketSellConfirm::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupMarketSellConfirm::OnClickCancel);
}

void UGsUIPopupMarketSellConfirm::NativeConstruct()
{
	Super::NativeConstruct();

	_bIsOk = false;
	_callbackOkCancel = nullptr;
}

void UGsUIPopupMarketSellConfirm::NativeDestruct()
{
	if (_callbackOkCancel)
	{
		_callbackOkCancel(_bIsOk, _itemDBId);
	}

	_bIsOk = false;
	_callbackOkCancel = nullptr;
	_itemDBId = INVALID_ITEM_DB_ID;

	Super::NativeDestruct();
}

bool UGsUIPopupMarketSellConfirm::OnBack()
{
	_bIsOk = false;

	return Super::OnBack();
}

void UGsUIPopupMarketSellConfirm::OnInputOk()
{
	_bIsOk = true;

	Close();
}

void UGsUIPopupMarketSellConfirm::OnInputCancel()
{
	_bIsOk = false;

	Close();
}

void UGsUIPopupMarketSellConfirm::OnClickOk()
{
	OnInputOk();
}

void UGsUIPopupMarketSellConfirm::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupMarketSellConfirm::SetData(bool bIsWorldMarket, ItemDBId InItemDBId, int64 InAmount, 
	int32 InPrice, int32 InRegistrationFee)
{
	TWeakPtr<FGsItem> item = GItem()->FindItem(InItemDBId);
	if (false == item.IsValid())
	{
		_bIsOk = false;

		Close();
		
		return;
	}

	_itemDBId = InItemDBId;

	// 아이템 정보
	_defaultInfoUI->SetData(item.Pin().Get(), true);
	_defaultInfoUI->SetItemAmount(InAmount);

	// 가격
	_textBlockPrice->SetText(FText::AsNumber(InPrice));

	// 개당 판매단가. 1개일 때는 보여주지 않음
	if (1 < InAmount)
	{
		_panelPricePerUnit->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		MarketPrice perPrice = 0.f;
		if (0 < InAmount && 0 < InPrice)
		{
			perPrice = SharedFunc::GetItemPricePerUnit(static_cast<MarketPrice>(InPrice), InAmount);
		}

		FText textPerPrice;
		FGsMarketHelper::GetMarketPriceText(perPrice, true, textPerPrice);
		_textBlockPricePerUnit->SetText(textPerPrice);
	}
	else
	{
		_panelPricePerUnit->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	// 등록 수수료
	_switcherRegistrationFeeType->SetActiveWidgetIndex(bIsWorldMarket ? 1 : 0);
	_textBlockRegistrationFee->SetText(FText::AsNumber(InRegistrationFee));
}

void UGsUIPopupMarketSellConfirm::SetCallback(TFunction<void(bool, ItemDBId)> InCallback)
{
	_callbackOkCancel = InCallback;
}
