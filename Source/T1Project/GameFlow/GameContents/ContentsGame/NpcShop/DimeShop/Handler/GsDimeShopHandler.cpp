// Fill out your copyright notice in the Description page of Project Settings.


#include "GsDimeShopHandler.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsNpcShopMessageParam.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsNpcShopManager.h"


bool FGsDimeShopHandler::TryBuyItem()
{
	if (false == Super::TryBuyItem())
	{
#if WITH_EDITOR
		if (nullptr == _npcSchemaData)
		{
			GSLOG(Warning, TEXT("FGsDimeShopHandler::TryBuyItem is EmptyData - _npcSchemaData"));
		}
		else if (NpcShopModeType::SellMode == _npcShopModeType)
		{
			GSLOG(Warning, TEXT("FGsDimeShopHandler::TryBuyItem is SellMode"));
		}
#endif
		return false;
	}

	return true;
}

bool FGsDimeShopHandler::TrySellItem()
{
	if (false == Super::TrySellItem())
	{
#if WITH_EDITOR
		if (nullptr == _npcSchemaData)
		{
			GSLOG(Warning, TEXT("FGsDimeShopHandler::TrySellItem is EmptyData - _npcSchemaData"));
		}
		else if (NpcShopModeType::BuyMode == _npcShopModeType)
		{
			GSLOG(Warning, TEXT("FGsDimeShopHandler::TrySellItem is BuyMode"));
		}
#endif
		return false;
	}

	return true;
}