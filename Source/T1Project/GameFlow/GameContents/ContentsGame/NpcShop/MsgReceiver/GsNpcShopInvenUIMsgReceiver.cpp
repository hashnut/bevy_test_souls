// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNpcShopInvenUIMsgReceiver.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsNpcShopMessageParam.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "UI/UIContent/Window/NpcShop/GsUIPopupNpcShopInven.h"


FGsNpcShopInvenUIMsgReceiver::~FGsNpcShopInvenUIMsgReceiver()
{
}

void FGsNpcShopInvenUIMsgReceiver::RegisterMessage()
{
	FGsMessageHolder* msgManager = GMessage();
	if (nullptr == msgManager)
		return;
	
	_listNpcShopDelegate.Emplace(msgManager->GetNpcShop().AddRaw(MessageContentNpcShop::CHANGE_MODE,
		this, &FGsNpcShopInvenUIMsgReceiver::OnChangeNpcShopMode));
	_listNpcShopDelegate.Emplace(msgManager->GetNpcShop().AddRaw(MessageContentNpcShop::INVALIDATE_BASKET,
		this, &FGsNpcShopInvenUIMsgReceiver::InvalidateShoppingBasketList));
	_listNpcShopDelegate.Emplace(msgManager->GetNpcShop().AddRaw(MessageContentNpcShop::OPEN,
		this, &FGsNpcShopInvenUIMsgReceiver::OnEnterNpcShop));
	

	//_listItemDelegate.Emplace(msgManager->GetItem().AddRaw(MessageItem::ADDITEM,
	//	this, &FGsNpcShopInvenUIMsgReceiver::AddItemMessage));
	_listItemDelegate.Emplace(msgManager->GetItem().AddRaw(MessageItem::ADDITEMLIST,
		this, &FGsNpcShopInvenUIMsgReceiver::OnAddItemList));
	_listItemDelegate.Emplace(msgManager->GetItem().AddRaw(MessageItem::UPDATEITEM,
		this, &FGsNpcShopInvenUIMsgReceiver::UpdateItemMessage));
	_listItemDelegate.Emplace(msgManager->GetItem().AddRaw(MessageItem::REMOVEITEM,
		this, &FGsNpcShopInvenUIMsgReceiver::RemoveItemMessage));
	_listItemDelegate.Emplace(msgManager->GetItem().AddRaw(MessageItem::REMOVEITEMS,
		this, &FGsNpcShopInvenUIMsgReceiver::OnRemoveItems));
}

void FGsNpcShopInvenUIMsgReceiver::RemoveMsg()
{
	if (nullptr != GMessage())
	{
		FGsMessageHolder* msg = GMessage();
		for (TPair<MessageContentNpcShop, FDelegateHandle>& elem : _listNpcShopDelegate)
		{
			msg->GetNpcShop().Remove(elem);
		}
		_listNpcShopDelegate.Empty();

		for (TPair<MessageItem, FDelegateHandle>& el : _listItemDelegate)
		{
			msg->GetItem().Remove(el);
		}
		_listItemDelegate.Empty();
	}
}

// New Item
//void FGsNpcShopInvenUIMsgReceiver::AddItemMessage(struct FGsItemMessageParamBase& InParam)
//{	
//	FGsAddItem* paramData = static_cast<FGsAddItem*>(&InParam);
//	if (nullptr == paramData || false == _npcShopInvenUI.IsValid())
//		return;
//
//	_npcShopInvenUI.Get()->AddItem(paramData->_itemDBID);
//}

void FGsNpcShopInvenUIMsgReceiver::OnAddItemList(struct FGsItemMessageParamBase& InParam)
{
	FGsAddItemList* paramData = static_cast<FGsAddItemList*>(&InParam);
	if (nullptr == paramData || false == _npcShopInvenUI.IsValid())
		return;

	for (ItemDBId itemDBId : paramData->_itemDbidList)
	{
		_npcShopInvenUI.Get()->AddItem(itemDBId);
	}

	_npcShopInvenUI.Get()->InvalidateAll();
}

// Remove Item
void FGsNpcShopInvenUIMsgReceiver::RemoveItemMessage(struct FGsItemMessageParamBase& InParam)
{
	FGsRemoveItem* paramData = static_cast<FGsRemoveItem*>(&InParam);
	if (nullptr == paramData || false == _npcShopInvenUI.IsValid())
		return;

	_npcShopInvenUI.Get()->RemoveItem(paramData->_itemDBID);
}

void FGsNpcShopInvenUIMsgReceiver::OnRemoveItems(struct FGsItemMessageParamBase& InParam)
{
	// REMOVEITEM 을 통해 아이템 정보를 갱신하고 REMOVEITEMS 를 통해 UI를 갱신한다.
	_npcShopInvenUI.Get()->InvalidateInventorySimple();
}

// Update Item
// 특정 아이템에 한하여서 update 처리 필요
void FGsNpcShopInvenUIMsgReceiver::UpdateItemMessage(struct FGsItemMessageParamBase& InParam)
{
	FGsUpdateItem* paramData = static_cast<FGsUpdateItem*>(&InParam);
	if (nullptr == paramData || false == _npcShopInvenUI.IsValid())
		return;

	_npcShopInvenUI.Get()->UpdateItem(paramData->_itemDBID);
}

void FGsNpcShopInvenUIMsgReceiver::InvalidateShoppingBasketList(const struct IGsMessageParam* InParamData)
{
	const FGsInvalidateShoppingBasket* param = InParamData->Cast<const FGsInvalidateShoppingBasket>();
	if (nullptr == param || false == _npcShopInvenUI.IsValid())
		return;

	_npcShopInvenUI.Get()->InvalidateInventorySimple();
}

void FGsNpcShopInvenUIMsgReceiver::OnChangeNpcShopMode(const struct IGsMessageParam* InParamData)
{
	const FGsOnChangeNpcShopMode* param = InParamData->Cast<const FGsOnChangeNpcShopMode>();
	if (nullptr == param || false == _npcShopInvenUI.IsValid())
		return;

	_npcShopInvenUI.Get()->ChangeShopMode(param->_isBuyMode);
}

void FGsNpcShopInvenUIMsgReceiver::SetUI(UGsUIPopupNpcShopInven* InUI)
{
	_npcShopInvenUI = InUI;
}

void FGsNpcShopInvenUIMsgReceiver::OnEnterNpcShop(const struct IGsMessageParam* InParamData)
{
	const FGsEnterNpcShop* param = InParamData->Cast<const FGsEnterNpcShop>();
	if (nullptr == param || false == _npcShopInvenUI.IsValid())
		return;

	_npcShopInvenUI.Get()->ChangeShopType(param->_npcFunctionType);
}