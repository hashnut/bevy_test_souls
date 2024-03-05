// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNpcShopUIMsgReceiver.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsNpcShopMessageParam.h"
#include "Message/MessageParam/GsTutorialMessageParam.h"
#include "UI/UIContent/Window/NpcShop/GsUIWindowNpcShop.h"


FGsNpcShopUIMsgReceiver::~FGsNpcShopUIMsgReceiver()
{
	_npcShopUI.Reset();
}

void FGsNpcShopUIMsgReceiver::RegisterMessage()
{
	FGsMessageHolder* msgManager = GMessage();
	if (nullptr == msgManager)
		return;

	_listNpcShopDelegate.Emplace(msgManager->GetNpcShop().AddRaw(MessageContentNpcShop::INVALIDATE_SHOP,
		this, &FGsNpcShopUIMsgReceiver::InvalidateShopItemList));
	_listNpcShopDelegate.Emplace(msgManager->GetNpcShop().AddRaw(MessageContentNpcShop::INVALIDATE_BASKET,
		this, &FGsNpcShopUIMsgReceiver::InvalidateShoppingBasketList));
	_listNpcShopDelegate.Emplace(msgManager->GetNpcShop().AddRaw(MessageContentNpcShop::CHANGE_MODE,
		this, &FGsNpcShopUIMsgReceiver::OnChangeNpcShopMode));
	_listNpcShopDelegate.Emplace(msgManager->GetNpcShop().AddRaw(MessageContentNpcShop::OPEN,
		this, &FGsNpcShopUIMsgReceiver::OnEnterNpcShop));
	_listNpcShopDelegate.Emplace(msgManager->GetNpcShop().AddRaw(MessageContentNpcShop::INVALIDATE_PRODUCTS,
		this, &FGsNpcShopUIMsgReceiver::OnUpdateShopItemList));
	_listNpcShopDelegate.Emplace(msgManager->GetNpcShop().AddRaw(MessageContentNpcShop::INVALIDATE_WEIGHT,
		this, &FGsNpcShopUIMsgReceiver::OnUpdateShopItemList));

}

void FGsNpcShopUIMsgReceiver::RemoveMsg()
{
	if (nullptr != GMessage() && 0 < _listNpcShopDelegate.Num())
	{
		FGsMessageHolder* msg = GMessage();
		for (TPair<MessageContentNpcShop, FDelegateHandle>& elem : _listNpcShopDelegate)
		{
			msg->GetNpcShop().Remove(elem);
		}
		_listNpcShopDelegate.Empty();
	}
}

void FGsNpcShopUIMsgReceiver::InvalidateShopItemList(const struct IGsMessageParam* InParamData)
{
	const FGsInvalidateNpcShopItemList* param = InParamData->Cast<const FGsInvalidateNpcShopItemList>();
	if (nullptr == param || false == _npcShopUI.IsValid())
		return;

	_npcShopUI.Get()->InvalidateProductItemList(param->_npcFunctionType, param->_npcShopGroupIDList, param->_shopProductTabType, param->_isBuyMode);
}

void FGsNpcShopUIMsgReceiver::InvalidateShoppingBasketList(const struct IGsMessageParam* InParamData)
{
	const FGsInvalidateShoppingBasket* param = InParamData->Cast<const FGsInvalidateShoppingBasket>();
	if (nullptr == param || false == _npcShopUI.IsValid())
		return;

	_npcShopUI.Get()->InvalidateShoppingBasketScroll(param->_changedSlotIndex);
	_npcShopUI.Get()->InvalidateAllShoppingBasketUI();
}

void FGsNpcShopUIMsgReceiver::OnChangeNpcShopMode(const struct IGsMessageParam* InParamData)
{
	const FGsOnChangeNpcShopMode* param = InParamData->Cast<const FGsOnChangeNpcShopMode>();
	if (nullptr == param || false == _npcShopUI.IsValid())
		return;

	_npcShopUI.Get()->InvalidateShopMode(param->_isBuyMode);
}

void FGsNpcShopUIMsgReceiver::OnEnterNpcShop(const struct IGsMessageParam* InParamData)
{
	const FGsEnterNpcShop* param = InParamData->Cast<const FGsEnterNpcShop>();
	if (nullptr == param || false == _npcShopUI.IsValid())
		return;

	_npcShopUI.Get()->InvalidateShopType(param->_npcFunctionType);
	_npcShopUI.Get()->SetShopName(param->_npcFunctionType, param->_npcData);
	_npcShopUI.Get()->InvalidateProductItemList(param->_npcFunctionType, param->_npcShopGroupIDList, param->_shopProductTabType, param->_isBuyMode);

	// 튜토리얼 발동 체크를 위한 메시지 전송
	FString typeName = NpcFunctionTypeEnumToString(param->_npcFunctionType);
	FGsTutorialMsgParamOpenUI tutorialParam(_npcShopUI.Get()->GetTableKey(), typeName);
	GMessage()->GetTutorial().SendMessage(MessageContentTutorial::OPEN_UI_CHECK_PARAM, &tutorialParam);
}

void FGsNpcShopUIMsgReceiver::InvalidateWeight(const struct IGsMessageParam* InParamData)
{
	if (false == _npcShopUI.IsValid())
		return;

	_npcShopUI.Get()->InvalidateText();
}

void FGsNpcShopUIMsgReceiver::SetUI(UGsUIWindowNpcShop* InUI)
{
	_npcShopUI = InUI;
}

void FGsNpcShopUIMsgReceiver::OnUpdateShopItemList(const struct IGsMessageParam* InParamData)
{
	_npcShopUI.Get()->UpdateShopItemList();
}