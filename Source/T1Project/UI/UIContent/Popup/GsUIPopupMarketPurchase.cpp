// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupMarketPurchase.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Currency/GsCostPackage.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Common/Item/GsUIItemDefaultInfo.h"
#include "UI/UIContent/Common/Item/GsUIItemDetailInfo.h"
#include "UI/UIContent/Popup/Market/GsUIMarketPriceInfo.h"
#include "UI/UIContent/Popup/GsUIPopupCommonPay.h"
#include "Market/GsMarketItemData.h"
#include "Market/GsMarketHelper.h"
#include "Message/MessageParam/GsMarketMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Item/GsItemManager.h"
#include "Net/GsNetSendServiceMarket.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

void UGsUIPopupMarketPurchase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_sBtnSwap->OnClicked.AddDynamic(this, &UGsUIPopupMarketPurchase::OnClickSwap);
	_sBtnCompare->OnClicked.AddDynamic(this, &UGsUIPopupMarketPurchase::OnClickCompare);
	_btnPurchase->OnClicked.AddDynamic(this, &UGsUIPopupMarketPurchase::OnClickPurchase);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupMarketPurchase::OnClickClose);	
}

void UGsUIPopupMarketPurchase::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();

	// R2: 기획 요청으로 무조건 시세정보를 먼저 보여줌
	SetUISwapInfoPage(0);
}

void UGsUIPopupMarketPurchase::NativeDestruct()
{
	_itemData = nullptr;
	_transactionId = INVALID_MARKET_TRANSACTION_ID;

	UnbindMessages();

	Super::NativeDestruct();
}

void UGsUIPopupMarketPurchase::OnInputOk()
{
	if (nullptr == _itemData ||
		nullptr == _itemData->_item)
	{
		// TEXT: 구매에 실패하였습니다.
		FText textMsg;
		if (FText::FindText(TEXT("MarketText"), TEXT("Ticker_Purchase_Fail"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}

		Close();

		return;
	}

	// 팝업을 열 때와 동일한 아이템인지 안전을 위해 다시한 번 체크
	if (INVALID_MARKET_TRANSACTION_ID == _transactionId ||		
		_transactionId != _itemData->_transactionId)
	{
		// TEXT: 구매에 실패하였습니다.
		FText textMsg;
		if (FText::FindText(TEXT("MarketText"), TEXT("Ticker_Purchase_Fail"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}

		Close();

		return;
	}
	
	// 각종 예외사항 체크
	if (false == FGsMarketHelper::CheckInventoryState(_itemData->_item->GetWeight(), true))
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupCommonPay"));
	if (widget.IsValid())
	{
		if (UGsUIPopupCommonPay* popup = Cast<UGsUIPopupCommonPay>(widget.Get()))
		{
			// TEXT: 구매하시겠습니까?
			FText textMsg;
			FText::FindText(TEXT("MarketText"), TEXT("ConfirmBuy"), textMsg);

			MarketTransactionId transactionId = _itemData->_transactionId;
			bool bIsWorldMarket = _bIsWorldMarket;
			popup->SetData(textMsg,
				FGsCostPackage::CreateSimple(CurrencyType::DIA, _itemData->_price),
				[bIsWorldMarket, transactionId](bool bInIsOk)
				{
					if (bInIsOk)
					{
						// 서버에 구매패킷 전송
						FGsNetSendServiceMarket::SendReqTransactionPurchase(bIsWorldMarket, transactionId);
					}
				});
		}
	}

	// 체크 리스트 리셋
	if (FGsMarketManager* marketMgr = GSMarket())
	{
		marketMgr->ClearMultiplePurchaseList();
	}

	Close();
}

void UGsUIPopupMarketPurchase::OnInputCancel()
{
	Close();
}

void UGsUIPopupMarketPurchase::BindMessages()
{
	MMarket& msgMarket = GMessage()->GetMarket();

	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::RCV_ITEM_PRICE,
		this, &UGsUIPopupMarketPurchase::OnReceiveItemPriceInfo));
}

void UGsUIPopupMarketPurchase::UnbindMessages()
{
	MMarket& msgMarket = GMessage()->GetMarket();

	for (MsgHandleMarket& handle : _msgHandleMarketList)
	{
		msgMarket.Remove(handle);
	}
	_msgHandleMarketList.Empty();
}

void UGsUIPopupMarketPurchase::SetData(bool bIsWorldMarket, const FGsMarketItemData* InData)
{
	_bIsWorldMarket = bIsWorldMarket;
	_itemData = InData;	

	if (nullptr == InData ||
		nullptr == InData->_item)
	{
		_transactionId = INVALID_MARKET_TRANSACTION_ID;
		return;
	}

	_transactionId = _itemData->_transactionId;

	// 시세정보 요청
	FGsNetSendServiceMarket::SendReqItemPrice(_bIsWorldMarket, InData->GetItemId(), InData->GetEnchantLevel());

	_defaultInfoUI->SetData(InData->_item, true);

	// 여기도 재화 부족 컬러가 적용되어야 하는가?
	_textBlockPrice->SetText(FText::AsNumber(InData->_price));

	// 개당 판매금액 소수점 표기
	FText textPricePerUnit;
	FGsMarketHelper::GetMarketPriceText(InData->GetPricePerUnit(), true, textPricePerUnit);
	_textBlockPricePerUnit->SetText(textPricePerUnit);

	// 비교 불가능한 아이템일 경우 버튼 막기 처리
	_sBtnCompare->SetSwitcherIndex(CanCompare(InData->_item) ? 1 : 0);

	// 일단 기본세팅
	_detailInfoUI->SetData(_itemData->_item);
	
	// 시세정보는 0으로 비우고 패킷 받으면 채움
	_priceInfoUI->SetEmpty();
}

void UGsUIPopupMarketPurchase::SetUISwapInfoPage(int32 InIndex)
{
	_sBtnSwap->SetSwitcherIndex(InIndex);
	_switcherInfo->SetActiveWidgetIndex(InIndex);
}

bool UGsUIPopupMarketPurchase::CanCompare(const FGsItem* InItem)
{
	if (InItem->IsEquipment())
	{
		UGsItemManager* itemMgr = GItem();

		EGsEquipSlotType equipSlotType = itemMgr->GetEquipSlotType(InItem->GetSubCategory(), 0);
		TWeakPtr<FGsItem> equippedItem = itemMgr->GetPlayerEquipedItemBySlotType(equipSlotType);
		if (equippedItem.IsValid())
		{
			return true;
		}
		else
		{
			// 좌우가 있는 아이템의 경우, 두 슬롯 다 찾아봄
			EGsEquipSlotType equipSlotTypeR = itemMgr->GetEquipSlotType(InItem->GetSubCategory(), 1);
			if (equipSlotType != equipSlotTypeR)
			{
				equippedItem = itemMgr->GetPlayerEquipedItemBySlotType(equipSlotTypeR);
				if (equippedItem.IsValid())
				{
					return true;
				}
			}
		}
	}

	return false;
}

void UGsUIPopupMarketPurchase::OnReceiveItemPriceInfo(const IGsMessageParam*)
{
	if (_itemData)
	{
		FGsMarketControllerBase* controller = GSMarket()->GetController(_bIsWorldMarket);
		if (nullptr == controller)
		{
			return;
		}

		const FGsMarketItemPriceData& priceData = controller->GetItemPriceData();
		if (_itemData->GetItemId() == priceData._itemId)
		{
			_priceInfoUI->SetData(priceData);
		}
	}
}

void UGsUIPopupMarketPurchase::OnClickSwap()
{
	// 스왑 인덱스 처리후 세팅
	int32 index = (_sBtnSwap->GetSwitcherIndex() + 1) % 2;
	SetUISwapInfoPage(index);
}

void UGsUIPopupMarketPurchase::OnClickCompare()
{
	if (nullptr == _itemData ||
		nullptr == _itemData->_item)
	{
		return;
	}

	if (false == CanCompare(_itemData->_item))
	{
		// TEXT: 비교할 장비가 없거나 비교할 수 없는 아이템입니다.
		FText textMsg;
		if (FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Compare_Notice_Cannot"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}

		return;
	}

	FGsItemDetailCompareOpenMsgParam param(_itemData->_item, false, true, false);
	GMessage()->GetItemContents().SendMessage(
		MessageContentItem::DETAIL_DISPLAY_ITEM_COMPARE, &param);
}

void UGsUIPopupMarketPurchase::OnClickPurchase()
{
	OnInputOk();
}

void UGsUIPopupMarketPurchase::OnClickClose()
{
	OnInputCancel();
}
