// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateNpcShop.h"

#include "DimeShop/Handler/GsDimeShopHandler.h"
#include "EventShop/Handler/GsEventShopHandler.h"
#include "SkillShop/Handler/GsSkillShopHandler.h"
#include "EquipShop/Handler/GsEquipShopHandler.h"

#include "Net/GsNetSendServiceWorld.h"

#include "Management/ScopeGame/GsNpcShopManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "EventProgress/GsEventProgressDefine.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsNpcShopMessageParam.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/MessageParam/GsItemMessageParam.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"

#include "Input/GsInputEventMsgBase.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"

#include "Util/GsTableUtil.h"


FGsGameStateNpcShop::FGsGameStateNpcShop() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsNpcShop)
{
	_interactionMsgHandler = GMessage()->GetGameObject().AddRaw(MessageGameObject::INTERACTION_CONTENTS_BY_NPC_START,
		this, &FGsGameStateNpcShop::OnInteractionContents);

	_shopHandlerMap.Empty();
	_shopHandlerMap.Emplace(NpcFunctionType::SHOP, new FGsDimeShopHandler());
	_shopHandlerMap.Emplace(NpcFunctionType::EVENT_SHOP, new FGsEventShopHandler());
	_shopHandlerMap.Emplace(NpcFunctionType::SHOP_EQUIP, new FGsEquipShopHandler());
	_shopHandlerMap.Emplace(NpcFunctionType::SHOP_SKILL_BOOK, new FGsSkillShopHandler());
	
	for (auto& shopHandler : _shopHandlerMap)
	{
		if (nullptr != shopHandler.Value)
		{
			shopHandler.Value->Init();
		}
	}
}

void FGsGameStateNpcShop::RemoveMessage()
{
	FGsMessageHolder* message = GMessage();
	MGameObject& gameObjectMsg = message->GetGameObject();
	MNpcShop& npcShopMsg = message->GetNpcShop();
	MItem& itemMsg = message->GetItem();

	if (_despawnMsgHandler.Value.IsValid())
	{
		gameObjectMsg.Remove(_despawnMsgHandler);
	}	

	for (const TPair<MessageContentNpcShop, FDelegateHandle>& elemItem : _npcShopMsgHandler)
	{
		if (elemItem.Value.IsValid())
		{
			npcShopMsg.Remove(elemItem);
		}		
	}
	_npcShopMsgHandler.Empty();

	for (const TPair<MessageItem, FDelegateHandle>& elemItem : _listItemDelegate)
	{
		if (elemItem.Value.IsValid())
		{
			itemMsg.Remove(elemItem);
		}		
	}
	_listItemDelegate.Empty();
}

FGsGameStateNpcShop::~FGsGameStateNpcShop()
{
	if (GMessage())
	{
		GMessage()->GetGameObject().Remove(_interactionMsgHandler);
	}	
	
	for (auto& shopHandler : _shopHandlerMap)
	{
		delete(shopHandler.Value);
		shopHandler.Value = nullptr;
	}
	_shopHandlerMap.Empty();

	_npcSchemaData = nullptr;
}

// 해당 컨텐츠 포인터 remove 될때 호출(게임 종료할때?)
void FGsGameStateNpcShop::Close()
{
	for (auto& shopHandler : _shopHandlerMap)
	{
		if (nullptr != shopHandler.Value)
		{
			shopHandler.Value->Close();
		}
	}
	
	Super::Close();
}

// 해당 컨텐츠 out 할때 호출
void FGsGameStateNpcShop::Exit()
{
	_npcSchemaData = nullptr;

	RemoveMessage();

	for (auto& shopHandler : _shopHandlerMap)
	{
		if (nullptr != shopHandler.Value)
		{
			shopHandler.Value->Exit();
		}
	}

	if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
	{
		eventProgressManager->OnReqFinishEvent(EGsEventProgressType::NPC_SHOP);
	}

	BlockInput(false);

	Super::Exit();
}

void FGsGameStateNpcShop::Enter()
{
	Super::Enter();

	RegisterMessage();
	// 재화 바를 열어 놓기만 한다. 
	// UGsUIWindowNpcShop::InvalidateCurrencyIcon(재화 설정)에서 설정한다.
	OpenNavigationBar();

	BlockInput(true);

	if (true == _shopHandlerMap.Contains(_npcFunctionType))
	{
		if (nullptr != _shopHandlerMap[_npcFunctionType])
		{
			_shopHandlerMap[_npcFunctionType]->EnterShop(_npcSchemaData, _targetNpcGameId);
		}
	}
}

void FGsGameStateNpcShop::RegisterMessage()
{
	FGsMessageHolder* message = GMessage();

	_despawnMsgHandler = message->GetGameObject().AddRaw(MessageGameObject::COMMON_DESPAWN,
		this, &FGsGameStateNpcShop::OnNpcDespawn);

	MNpcShop& npcShopMsg = message->GetNpcShop();
	_npcShopMsgHandler.Emplace(npcShopMsg.AddRaw(MessageContentNpcShop::PURCHASE,
		this, &FGsGameStateNpcShop::TryBuyItem));
	_npcShopMsgHandler.Emplace(npcShopMsg.AddRaw(MessageContentNpcShop::SALE,
		this, &FGsGameStateNpcShop::TrySellItem));
	_npcShopMsgHandler.Emplace(npcShopMsg.AddRaw(MessageContentNpcShop::INSERT_BASKET,
		this, &FGsGameStateNpcShop::TryInputShoppingBasket));

	_listItemDelegate.Emplace(message->GetItem().AddRaw(MessageItem::UPDATEITEMLIST,
		this, &FGsGameStateNpcShop::UpdateItemMessage));
	_listItemDelegate.Emplace(message->GetItem().AddRaw(MessageItem::REMOVEITEMS,
		this, &FGsGameStateNpcShop::OnRemoveItems));
}

void FGsGameStateNpcShop::OnInteractionContents(const IGsMessageParam* In_data)
{
	const FGsInteractionMessageParamContents* interactionParam = In_data->Cast<const FGsInteractionMessageParamContents>();
	if (nullptr == interactionParam)
		return;

	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(interactionParam->_npcTblId);
	if (nullptr == npcData)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("FGsSchemaNpcData::FindRowById is EmptyData - _npcTblId : %d"), interactionParam->_npcTblId);
#endif
		return;
	}
	
	if (false == IsShop(npcData->npcFunctionType))
		return;

	_npcSchemaData = npcData;
	_npcFunctionType = npcData->npcFunctionType;
	_targetNpcGameId = interactionParam->_gameId;

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[FGsGameStateNpcShop::OnInteractionContents] - gameID : %d"), _targetNpcGameId);
	GSLOG(Warning, TEXT("[FGsGameStateNpcShop::OnInteractionContents] - npcTbID : %d"), interactionParam->_npcTblId);
	GSLOG(Warning, TEXT("[FGsGameStateNpcShop::OnInteractionContents] - spawnID : %d"), interactionParam->_spawnId);
#endif

	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_NPC_SHOP);
}

void FGsGameStateNpcShop::OnNpcDespawn(const IGsMessageParam* In_data)
{
	const FGsGameObjectMessageParam* despawnParam = In_data->Cast<const FGsGameObjectMessageParam>();
	if (nullptr == despawnParam)
		return;

	UGsGameObjectBase* despawnedObject = despawnParam->_paramOwner;
	if (nullptr == despawnedObject || _targetNpcGameId != despawnedObject->GetGameId())
		return;

	if (nullptr != _shopHandlerMap[_npcFunctionType])
	{
		_shopHandlerMap[_npcFunctionType]->CloseNpcShop();
	}
}

void FGsGameStateNpcShop::TryBuyItem(const IGsMessageParam* InParam)
{
	if (true == _shopHandlerMap.Contains(_npcFunctionType))
	{
		if (nullptr != _shopHandlerMap[_npcFunctionType])
		{
			_shopHandlerMap[_npcFunctionType]->TryBuyItem();
		}
	}
}

void FGsGameStateNpcShop::TrySellItem(const IGsMessageParam* InParam)
{
	if (true == _shopHandlerMap.Contains(_npcFunctionType))
	{
		if (nullptr != _shopHandlerMap[_npcFunctionType])
		{
			_shopHandlerMap[_npcFunctionType]->TrySellItem();
		}
	}
}

// 구매/판매 목록에 아이템 추가
void FGsGameStateNpcShop::TryInputShoppingBasket(const IGsMessageParam* InParam)
{
	const FGsInputShoppingBasket* paramData = InParam->Cast<const FGsInputShoppingBasket>();
	if (nullptr == paramData)
		return;

	if (true == _shopHandlerMap.Contains(_npcFunctionType))
	{
		if (nullptr != _shopHandlerMap[_npcFunctionType])
		{
			_shopHandlerMap[_npcFunctionType]->InputShoppingBasket(paramData->_itemList, paramData->_isBuyMode, paramData->_isAdded);
		}
	}
}

void FGsGameStateNpcShop::OnReconectionEnd()
{
	if (true == _shopHandlerMap.Contains(_npcFunctionType))
	{
		if (nullptr != _shopHandlerMap[_npcFunctionType])
		{
			_shopHandlerMap[_npcFunctionType]->OnReconectionEnd();
		}
	}

	Super::OnReconectionEnd();
}

void FGsGameStateNpcShop::OnRemoveItems(struct FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItemList* paramData = static_cast<const FGsRemoveItemList*>(&InParam);
	if (nullptr == paramData)
		return;

	UGsNpcShopManager* shopManager = GSNpcShop();
	if (nullptr == shopManager)
		return;

	for (ItemDBId removedItemDBId : paramData->_itemDBIdList)
	{
		shopManager->RemoveBasketItemByItemDBId(removedItemDBId);
	}

	FGsInvalidateShoppingBasket invalidateParam(-1);
	GMessage()->GetNpcShop().SendMessage(MessageContentNpcShop::INVALIDATE_BASKET, &invalidateParam);
}

void FGsGameStateNpcShop::UpdateItemMessage(struct FGsItemMessageParamBase& InParam)
{
	FGsUpdateItemList* paramData = static_cast<FGsUpdateItemList*>(&InParam);
	if (nullptr == paramData)
		return;

	UGsNpcShopManager* shopManager = GSNpcShop();
	if (nullptr == shopManager)
		return;

	if (ItemStorageType::INVENTORY == paramData->_itemStorageType)
	{
		for (ItemDBId updatedItemDBId : paramData->_itemDbidList)
		{
			shopManager->UpdateBasketItem(updatedItemDBId);
		}

		FGsInvalidateShoppingBasket invalidateParam(-1);
		GMessage()->GetNpcShop().SendMessage(MessageContentNpcShop::INVALIDATE_BASKET, &invalidateParam);
		GMessage()->GetNpcShop().SendMessage(MessageContentNpcShop::INVALIDATE_WEIGHT, nullptr);
	}
}

bool FGsGameStateNpcShop::IsShop(NpcFunctionType InType)
{
	if (InType == NpcFunctionType::SHOP ||
		InType == NpcFunctionType::EVENT_SHOP ||
		InType == NpcFunctionType::SHOP_EQUIP ||
		InType == NpcFunctionType::SHOP_SKILL_BOOK)
	{
		return true;
	}
	return false;
}

void FGsGameStateNpcShop::BlockInput(bool InIsBlock)
{
	// 이동 On/Off
	if (FGsMessageHolder* message = GMessage())
	{
		//FGsInputEventMsgBase msg;
		//msg.Id = static_cast<int32>(InIsBlock);
		FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_NPC, InIsBlock);
		message->GetInput().SendMessage(MessageInput::BlockInput, msg);
	}
}