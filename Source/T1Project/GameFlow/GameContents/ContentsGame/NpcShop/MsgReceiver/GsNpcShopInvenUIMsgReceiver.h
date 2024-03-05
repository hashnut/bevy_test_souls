// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageItem.h"


class UGsUIPopupNpcShopInven;
/**
 * 
 */
class T1PROJECT_API FGsNpcShopInvenUIMsgReceiver
{
private:
	TWeakObjectPtr<UGsUIPopupNpcShopInven> _npcShopInvenUI;
	TArray<TPair<MessageContentNpcShop, FDelegateHandle>>	_listNpcShopDelegate;
	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemDelegate;

public:
	FGsNpcShopInvenUIMsgReceiver() = default;
	~FGsNpcShopInvenUIMsgReceiver();

public:
	void RegisterMessage();
	void RemoveMsg();

private:
	void OnChangeNpcShopMode(const struct IGsMessageParam* InParamData);
	void InvalidateShoppingBasketList(const struct IGsMessageParam* InParamData);
	void OnEnterNpcShop(const struct IGsMessageParam* InParamData);

private:
	//void AddItemMessage(struct FGsItemMessageParamBase& InParam);
	void OnAddItemList(struct FGsItemMessageParamBase& InParam);
	void UpdateItemMessage(struct FGsItemMessageParamBase& InParam);
	void RemoveItemMessage(struct FGsItemMessageParamBase& InParam);
	void OnRemoveItems(struct FGsItemMessageParamBase& InParam);

public:
	void SetUI(UGsUIPopupNpcShopInven* InUI);

public:
	bool IsValidUI() { return _npcShopInvenUI.IsValid(); }
};
