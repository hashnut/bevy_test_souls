// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Message/GsMessageContents.h"

class UGsUIWindowNpcShop;
/**
 * 
 */
class T1PROJECT_API FGsNpcShopUIMsgReceiver
{
private:
	TWeakObjectPtr<UGsUIWindowNpcShop> _npcShopUI;
	TArray<TPair<MessageContentNpcShop, FDelegateHandle>>	_listNpcShopDelegate;

public:
	FGsNpcShopUIMsgReceiver() = default;
	~FGsNpcShopUIMsgReceiver();

public:
	void RegisterMessage();
	void RemoveMsg();

private:
	void InvalidateShopItemList(const struct IGsMessageParam* InParamData);
	void InvalidateShoppingBasketList(const struct IGsMessageParam* InParamData);
	void OnChangeNpcShopMode(const struct IGsMessageParam* InParamData);
	void OnUpdateShopItemList(const struct IGsMessageParam* InParamData);
	void OnEnterNpcShop(const struct IGsMessageParam* InParamData);
	void InvalidateWeight(const struct IGsMessageParam* InParamData);

public:
	void SetUI(UGsUIWindowNpcShop* InUI);

public:
	bool IsValidUI() { return _npcShopUI.IsValid(); }
};
