// Fill out your copyright notice in the Description page of Project Settings.


#include "GsItemDetailHandler.h"
#include "T1Project.h"
#include "UTIL/GsItemUtil.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemEquipment.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Management/ScopeGame/GsDepotManager.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailCompare.h"
#include "UI/UIContent/Popup/GsUIPopupSummonItemDetail.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsInvenMessageParam.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Market/GsMarketHelper.h"
#include "UTIL/GsTableUtil.h"


FGsItemDetailHandler::~FGsItemDetailHandler()
{
}

void FGsItemDetailHandler::Close()
{
	RemoveMessage();
}

void FGsItemDetailHandler::Init()
{
	InitMessage();

}

void FGsItemDetailHandler::RemoveMessage()
{
	FGsMessageHolder* msg = GMessage();

	for (TPair<MessageContentItem, FDelegateHandle>& elemItem : _listItemDetailDelegate)
	{
		msg->GetItemContents().Remove(elemItem);
	}
	_listItemDetailDelegate.Empty();

	for (TPair<MessageItem, FDelegateHandle>& elemItem : _listItemDelegate)
	{
		msg->GetItem().Remove(elemItem);
	}
	_listItemDelegate.Empty();

	//msg->GetInventoryEvent().Remove(_invenDelegate);
}

void FGsItemDetailHandler::InitMessage()
{
	FGsMessageHolder* msg = GMessage();
	// ItemDetail Contents Message
	_listItemDetailDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::DETAIL_DISPLAY_ITEM,
		this, &FGsItemDetailHandler::OpenItemDetail));
	_listItemDetailDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::DETAIL_DISPLAY_ITEM_EX,
		this, &FGsItemDetailHandler::OpenItemDetailEx));
	_listItemDetailDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::DETAIL_DISPLAY_ITEM_BOX,
		this, &FGsItemDetailHandler::OpenItemDetailBox));
	_listItemDetailDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::DETAIL_DISPLAY_ITEM_TID,
		this, &FGsItemDetailHandler::OpenItemDetailTID));
	_listItemDetailDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::DETAIL_DISPLAY_CHAR_DETAIL,
		this, &FGsItemDetailHandler::OpenItemDetail_CharDetail));
	_listItemDetailDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::DETAIL_DISPLAY_CURRENCY,
		this, &FGsItemDetailHandler::OpenCurrencyDetail));
	_listItemDetailDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::DETAIL_CLOSE,
		this, &FGsItemDetailHandler::CloseDetail));
	_listItemDetailDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::DETAIL_DISPLAY_ITEM_COMPARE,
		this, &FGsItemDetailHandler::OpenItemDetailCompare));
	_listItemDetailDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::DETAIL_OPEN_MARKET,
		this, &FGsItemDetailHandler::OpenMarket));
	_listItemDetailDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::DETAIL_ITEM_COLLECTION,
		this, &FGsItemDetailHandler::OpenItemDetailByItemCollcetion));
	_listItemDetailDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::DETAIL_ITEM_COLLECTION_TEXT,
		this, &FGsItemDetailHandler::SetPropertyText));
	_listItemDetailDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::DETAIL_OPEN_POPUP_SUMMONDETAIL,
		this, &FGsItemDetailHandler::OpenSummonDetailPopup));

	// Item
	_listItemDelegate.Emplace(msg->GetItem().AddRaw(MessageItem::UPDATEITEM,
		this, &FGsItemDetailHandler::UpdateItemDetail));
	_listItemDelegate.Emplace(msg->GetItem().AddRaw(MessageItem::REMOVEITEM,
		this, &FGsItemDetailHandler::RemoveItemMessage));

	// inven popup
	//_invenDelegate = msg->GetInventoryEvent().AddRaw(MessageContentInven::CloseInventory,
		//this, &FGsItemDetailHandler::InvenPopupClose);
}

void FGsItemDetailHandler::OpenItemDetail(const struct IGsMessageParam* InMessageParam)
{
	const FGsItemDetailOpenMessageParam* param = InMessageParam->Cast<const FGsItemDetailOpenMessageParam>();
	if (nullptr == param || false == FGsItem::IsValidItem(param->_item))
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	if (uiManager->IsActiveWidget(TEXT("PopupItemDetail")))
	{
		uiManager->CloseByKeyName(TEXT("PopupItemDetail"),true);	
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupItemDetail"));
	if (widget.IsValid())
	{
		if (UGsUIPopupItemDetailMain* popupDetail = Cast<UGsUIPopupItemDetailMain>(widget))
		{
			popupDetail->SetItemData(param->_item, param->_bHideButtons, param->_centerPosType, param->_bHideBG, param->_bHideButtonsBottom, param->_bUseLock);
		}
	}
}

void FGsItemDetailHandler::OpenItemDetailEx(const struct IGsMessageParam* InMessageParam)
{
	const FGsItemDetailOpenMessageParamEX* param = InMessageParam->Cast<const FGsItemDetailOpenMessageParamEX>();
	if (nullptr == param || false == FGsItem::IsValidItem(param->_item))
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	if (uiManager->IsActiveWidget(TEXT("PopupItemDetail")))
	{
		uiManager->CloseByKeyName(TEXT("PopupItemDetail"), true);
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupItemDetail"));
	if (widget.IsValid())
	{
		if (UGsUIPopupItemDetailMain* popupDetail = Cast<UGsUIPopupItemDetailMain>(widget))
		{
			const FGsItemDetailOpenMessageParamEX::DetailOption& option = param->_option;

			popupDetail->SetItemData(param->_item, option._bHideButtons, param->_posDir, option._bHideBG, option._bHideButtonsBottom, option._bUseLock, option._bUseCollectionMarking);
		}
	}
}

void FGsItemDetailHandler::OpenItemDetailBox(const struct IGsMessageParam* InMessageParam)
{
	const FGsItemDetailOpenMessageParamEX* param = InMessageParam->Cast<const FGsItemDetailOpenMessageParamEX>();
	if (nullptr == param || false == FGsItem::IsValidItem(param->_item))
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupItemDetailBox"));
	if (widget.IsValid())
	{
		if (UGsUIPopupItemDetailMain* popupDetail = Cast<UGsUIPopupItemDetailMain>(widget))
		{
			const FGsItemDetailOpenMessageParamEX::DetailOption& option = param->_option;

			popupDetail->SetItemData(param->_item, option._bHideButtons, param->_posDir, option._bHideBG, option._bHideButtonsBottom, option._bUseLock, option._bUseCollectionMarking);
		}
	}
}

void FGsItemDetailHandler::OpenItemDetailTID(const struct IGsMessageParam* InMessageParam)
{
	const FGsItemDetailOpenMessageParamTID* param = InMessageParam->Cast<const FGsItemDetailOpenMessageParamTID>();
	if (nullptr == param || 0 >= param->_itemTID)
		return;

	if (UGsUIManager* uiManager = GUI())
	{
		if (uiManager->IsActiveWidget(TEXT("PopupItemDetail")))
		{
			uiManager->CloseByKeyName(TEXT("PopupItemDetail"), true);
		}

		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupItemDetail"));
		if (widget.IsValid())
		{
			if (UGsUIPopupItemDetailMain* popupDetail = Cast<UGsUIPopupItemDetailMain>(widget))
			{
				popupDetail->SetItemData(param->_itemTID, param->_bHideButtons, param->_centerPosType, param->_bHideBG);
			}
		}
	}
}

void FGsItemDetailHandler::OpenItemDetail_CharDetail(const struct IGsMessageParam* InMessageParam)
{
	const FGsItemDetailOpenMessageParam* param = InMessageParam->Cast<const FGsItemDetailOpenMessageParam>();
	if (nullptr == param || false == FGsItem::IsValidItem(param->_item))
		return;

	if (UGsUIManager* uiManager = GUI())
	{
		/*
		if (uiManager->IsActiveWidget(TEXT("PopupItemDetail_CharDetail")))
		{
			uiManager->CloseByKeyName(TEXT("PopupItemDetail_CharDetail"), true);
		}
		*/

		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupItemDetail_CharDetail"));
		if (widget.IsValid())
		{
			if (UGsUIPopupItemDetailMain* popupDetail = Cast<UGsUIPopupItemDetailMain>(widget))
			{
				popupDetail->SetItemData(param->_item, param->_bHideButtons, param->_centerPosType, param->_bHideBG);
			}
		}
	}
}

void FGsItemDetailHandler::OpenCurrencyDetail(const struct IGsMessageParam* InMessageParam)
{
	const FGsCurrencyDetailOpenMessageParam* param = InMessageParam->Cast<const FGsCurrencyDetailOpenMessageParam>();
	if (nullptr == param || CurrencyType::NONE == param->_currencyType)
	{
		GSLOG(Warning, TEXT("nullptr == param || CurrencyType::NONE == param->_currencyType"));
		return;
	}

	if (UGsUIManager* uiManager = GUI())
	{
		if (uiManager->IsActiveWidget(TEXT("PopupItemDetail")))
		{
			uiManager->CloseByKeyName(TEXT("PopupItemDetail"), true);
		}

		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupItemDetail"));
		if (widget.IsValid())
		{
			if (UGsUIPopupItemDetailMain* popupDetail = Cast<UGsUIPopupItemDetailMain>(widget))
			{
				popupDetail->SetCurrencyData(param->_currencyType, param->_amount, param->_bHideButtons, param->_centerPosType, param->_bHideBG);
			}
		}
	}
}

void FGsItemDetailHandler::OpenItemDetailCompare(const struct IGsMessageParam* InMessageParam)
{
	const FGsItemDetailCompareOpenMsgParam* param = InMessageParam->Cast<const FGsItemDetailCompareOpenMsgParam>();
	if (nullptr == param || false == FGsItem::IsValidItem(param->_targetItem))
	{
		return;
	}

	if (false == param->_targetItem->IsEquipment())
	{
		return;
	}

	// 해당 부위에 착용중인 아이템을 가져옴
	UGsItemManager* itemMgr = GItem();
	FGsItemEquipment* equipData = param->_targetItem->GetCastItem<FGsItemEquipment>();
	uint8 slotIndex = equipData->GetCurrentPresetSlotIndex();
	EGsEquipSlotType slotType = itemMgr->GetEquipSlotType(equipData->GetSubCategory(), slotIndex);
	TWeakPtr<FGsItem> ownedItem = GItem()->GetPlayerEquipedItemBySlotType(slotType);
	if (false == ownedItem.IsValid())
	{
		// 양쪽 pair로 사용하는 악세사리 타입은 양쪽 둘다 체크 해야함
		if (true == itemMgr->IsMultiSlotItem(equipData->GetSubCategory()))
		{
			slotType = itemMgr->GetEquipSlotType(equipData->GetSubCategory(), 0 == slotIndex ? 1 : 0);
			ownedItem = GItem()->GetPlayerEquipedItemBySlotType(slotType);
		}
	}

	if (false == ownedItem.IsValid())
	{
		// 장착된 아이템이 없는 경우
		return;
	}

	if (UGsUIManager* uiManager = GUI())
	{
		if (uiManager->IsActiveWidget(TEXT("PopupItemDetailCompare")))
		{
			uiManager->CloseByKeyName(TEXT("PopupItemDetailCompare"), true);
		}
		if (uiManager->IsActiveWidget(TEXT("PopupItemDetail")))
		{
			TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupItemDetail"));
			if (widget.IsValid())
			{
				if (UGsUIPopupItemDetailMain* popupDetail = Cast<UGsUIPopupItemDetailMain>(widget))
				{
					popupDetail->SetKeepSelectionFlag(true);
				}
			}
		}

		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupItemDetailCompare"));
		if (widget.IsValid())
		{
			if (UGsUIPopupItemDetailCompare* popupDetail = Cast<UGsUIPopupItemDetailCompare>(widget))
			{
				popupDetail->SetItemData(param->_targetItem, ownedItem.Pin()->GetDBID(), param->_bHideButtons, param->_bCenterPopup, param->_bHideBG, param->_bHideButtonsBottom, param->_bUseLock);
				CloseDetail(nullptr);
			}
		}
	}
}

void FGsItemDetailHandler::UpdateItemDetail(struct FGsItemMessageParamBase& InParam)
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	FGsUpdateItem* paramData = static_cast<FGsUpdateItem*>(&InParam);
	if (nullptr == paramData)
		return;

	const FGsItem* itemData = UGsItemUtil::FindItemByStorageType(paramData->_itemDBID, paramData->_itemStorageType);
	if (nullptr == itemData)
		return;

	if (true == uiManager->IsActiveWidget(TEXT("PopupItemDetail")))
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("PopupItemDetail"));
		if (widget.IsValid())
		{
			if (UGsUIPopupItemDetailMain* popupDetail = Cast<UGsUIPopupItemDetailMain>(widget))
			{
				popupDetail->UpdateItemData(itemData->GetDBID());
			}
		}
	}

	if (true == uiManager->IsActiveWidget(TEXT("PopupItemDetail_CharDetail")))
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("PopupItemDetail_CharDetail"));
		if (widget.IsValid())
		{
			if (UGsUIPopupItemDetailMain* popupDetail = Cast<UGsUIPopupItemDetailMain>(widget))
			{
				popupDetail->UpdateItemData(itemData->GetDBID());
			}
		}
	}

	if (true == uiManager->IsActiveWidget(TEXT("PopupItemDetailCompare")))
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("PopupItemDetailCompare"));
		if (widget.IsValid())
		{
			if (UGsUIPopupItemDetailCompare* popupDetail = Cast<UGsUIPopupItemDetailCompare>(widget))
			{
				popupDetail->UpdateItemData(itemData->GetDBID(), paramData->_skipItemDetailInfo);
			}
		}
	}
}

void FGsItemDetailHandler::RemoveProc(const FName& InWidgetKey, const uint64 InItemDBID)
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	if (true == uiManager->IsActiveWidget(InWidgetKey))
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(InWidgetKey);
		if (widget.IsValid())
		{
			if (UGsUIPopupItemDetailMain* popupDetail = Cast<UGsUIPopupItemDetailMain>(widget))
			{
				popupDetail->RemoveItemData(InItemDBID);
			}
			if (UGsUIPopupItemDetailCompare* compareDetail = Cast<UGsUIPopupItemDetailCompare>(widget))
			{
				compareDetail->RemoveItemData(InItemDBID);
			}
		}
	}
}

void FGsItemDetailHandler::RemoveItemMessage(struct FGsItemMessageParamBase& InParam)
{
	FGsRemoveItem* paramData = static_cast<FGsRemoveItem*>(&InParam);
	if (nullptr == paramData)
		return;

	if (0 >= paramData->_itemDBID)
		return;

	RemoveProc(TEXT("PopupItemDetail"), paramData->_itemDBID);
	RemoveProc(TEXT("PopupItemDetail_CharDetail"), paramData->_itemDBID);
	RemoveProc(TEXT("PopupItemDetailCompare"), paramData->_itemDBID);
}

void FGsItemDetailHandler::CloseDetail(const struct IGsMessageParam* InMessageParam)
{
	if (UGsUIManager* uiManager = GUI())
	{
		if (true == uiManager->IsActiveWidget(TEXT("PopupItemDetail")))
		{
			uiManager->CloseByKeyName(TEXT("PopupItemDetail"));
		}
		if (true == uiManager->IsActiveWidget(TEXT("PopupItemDetail_CharDetail")))
		{
			uiManager->CloseByKeyName(TEXT("PopupItemDetail_CharDetail"));
		}
	}
}

void FGsItemDetailHandler::OpenMarket(const IGsMessageParam* InMessageParam)
{
	const FGsItemDetailOpenMarketParam* param = InMessageParam->Cast<const FGsItemDetailOpenMarketParam>();
	if (nullptr == param || 
		INVALID_ITEM_ID == param->_itemTId)
	{
		return;
	}

	bool bIsWorldMarket = false;
	if (FGsItemDetailOpenMarketParam::EGsMarketSearchOption::DEFAULT == param->_searchOption)
	{
		if (const FGsSchemaItemCommon* itemData =
			UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(param->_itemTId))
		{
			ItemGrade grade = FGsMarketHelper::GetWorldMarketShortcutItemGrade();
			if (grade <= itemData->grade)
			{
				bIsWorldMarket = true;
			}
		}
	}
	else if (FGsItemDetailOpenMarketParam::EGsMarketSearchOption::WORLD_SERVER == param->_searchOption)
	{
		bIsWorldMarket = true;
	}

	GSMarket()->SendSearchItem(bIsWorldMarket, param->_itemTId);

	// 상세창 및 비교창이 있다면 닫기
	if (GUI()->IsActiveWidget(TEXT("PopupItemDetailCompare")))
	{
		GUI()->CloseByKeyName(TEXT("PopupItemDetailCompare"), true);
	}

	CloseDetail(nullptr);
}

void FGsItemDetailHandler::OpenItemDetailByItemCollcetion(const struct IGsMessageParam* InMessageParam)
{
	const FGsItemDetailItemCollectionOpenMsgParam* param = InMessageParam->Cast<const FGsItemDetailItemCollectionOpenMsgParam>();
	if (nullptr == param || false == FGsItem::IsValidItem(param->_item))
		return;

	if (UGsUIManager* uiManager = GUI())
	{
		if (uiManager->IsActiveWidget(TEXT("PopupItemDetail")))
		{
			uiManager->CloseByKeyName(TEXT("PopupItemDetail"), true);
		}

		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupItemDetail"));
		if (widget.IsValid())
		{
			if (UGsUIPopupItemDetailMain* popupDetail = Cast<UGsUIPopupItemDetailMain>(widget))
			{
				popupDetail->SetItemDataItemCollectionConditionItemOnly(param->_item, param->_isShowBtn, param->_isEnchant, param->_bChanged, param->_bRegister, param->_btnRegistedrType);
			}
		}
	}
}

void FGsItemDetailHandler::SetPropertyText(const struct IGsMessageParam* InMessageParam)
{
	const FGsItemDetailTextPropertyParam* param = InMessageParam->Cast<const FGsItemDetailTextPropertyParam>();
	if (nullptr == param || nullptr == param->_textParam)
		return;

	if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("PopupItemDetail"));
		if (widget.IsValid())
		{
			if (UGsUIPopupItemDetailMain* detail = Cast<UGsUIPopupItemDetailMain>(widget))
			{
				detail->SetItemCollectionRegisterInfoText(*param->_textParam);
			}
		}
	}
}

void FGsItemDetailHandler::OpenSummonDetailPopup(const struct IGsMessageParam* InMessageParam)
{
	if (const auto param = InMessageParam->Cast<const FGsItemDetailSummonTickerParam>())
	{
		if (UGsUIManager* uiManager = GUI())
		{
			if (uiManager->IsActiveWidget(TEXT("PopupSummonDetail")))
			{
				uiManager->CloseByKeyName(TEXT("PopupSummonDetail"), true);
			}

			auto widget = uiManager->OpenAndGetWidget(TEXT("PopupSummonDetail"));
			if (widget.IsValid())
			{
				if (auto popupSummonDetail = Cast<UGsUIPopupSummonItemDetail>(widget.Get()))
				{
					UGsUIPopupSummonItemDetail::PopupInitData initArg{param->_detailId};
					popupSummonDetail->InitializeData(&initArg);
				}
			}
		}
	}
}

void FGsItemDetailHandler::InvenPopupClose(struct FGsInvenMessageParamBase&)
{
	CloseDetail(nullptr);
}