// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsGameBase.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageItem.h"


class FGsNpcShopBase;
struct FGsSchemaNpcData;
struct FGsItemMessageParamBase;
struct IGsMessageParam;
/**
 * NpcShop 상위 State - 모두 통일된 인터페이스 상점이
   아닐수도 있으므로, 다른 인터페이스(ui,기능) 상점 고려
 */
class T1PROJECT_API FGsGameStateNpcShop : public FGsContentsGameBase
{
	using Super = FGsContentsGameBase;

private:
	TMap<NpcFunctionType, FGsNpcShopBase*> _shopHandlerMap;

private:
	const FGsSchemaNpcData* _npcSchemaData;
	NpcFunctionType _npcFunctionType;
	TArray<TPair<MessageContentNpcShop, FDelegateHandle>> _npcShopMsgHandler;
	MsgGameObjHandle _interactionMsgHandler;
	MsgGameObjHandle _despawnMsgHandler;
	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemDelegate;

private:
	int64 _targetNpcGameId = 0;

public:
	FGsGameStateNpcShop();
	virtual ~FGsGameStateNpcShop();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Close() override;
	virtual void OnReconectionEnd() override;

protected:
	virtual bool IsChangeSoundMixer() const override { return false; }

public:
	void OnInteractionContents(const IGsMessageParam* In_data);
	void OnNpcDespawn(const IGsMessageParam* In_data);

private:
	void RegisterMessage();
	void RemoveMessage();

private:
	void TryBuyItem(const IGsMessageParam* InParam);
	void TrySellItem(const IGsMessageParam* InParam);
	void TryInputShoppingBasket(const IGsMessageParam* InParam);

private:
	void UpdateItemMessage(struct FGsItemMessageParamBase& InParam);
	void OnRemoveItems(struct FGsItemMessageParamBase& InParam);

private:
	bool IsShop(NpcFunctionType InType);

private:
	void BlockInput(bool InIsBlock);
	
};
