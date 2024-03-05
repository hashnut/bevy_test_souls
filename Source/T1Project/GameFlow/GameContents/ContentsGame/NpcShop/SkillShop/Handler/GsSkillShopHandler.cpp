// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillShopHandler.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsNpcShopMessageParam.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsNpcShopManager.h"


bool FGsSkillShopHandler::TryBuyItem()
{
	if (false == Super::TryBuyItem())
	{
#if WITH_EDITOR
		if (nullptr == _npcSchemaData)
		{
			GSLOG(Warning, TEXT("FGsSkillShopHandler::TryBuyItem is EmptyData - _npcSchemaData"));
		}
		else if (NpcShopModeType::SellMode == _npcShopModeType)
		{
			GSLOG(Warning, TEXT("FGsSkillShopHandler::TryBuyItem is SellMode"));
		}
#endif
		return false;
	}

	return true;
}

bool FGsSkillShopHandler::TrySellItem()
{
	// 판매 기능이 없다.
	return false;
}