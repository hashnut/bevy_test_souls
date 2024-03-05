// Fill out your copyright notice in the Description page of Project Settings.

#include "GsShortcutHandler.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GameFlow/GameContents/GsContentsMode.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "Message/GsMessageContentDungeon.h"
#include "Message/GsMessageUI.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "DataSchema/Map/GsSchemaMapData.h"

#include "UTIL/GsTableUtil.h"



void FGsShortcutHandler::Init()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		MUI& uiMsg = GMessage()->GetUI();

		_uiMsgList.Emplace(
			uiMsg.AddRaw(MessageUI::REQUEST_SHORTCUT_CONTENTS, this,
				&FGsShortcutHandler::OnShortcutContents));
	}
}

void FGsShortcutHandler::Close()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		MUI& uiMsg = GMessage()->GetUI();

		if (_uiMsgList.Num() != 0)
		{
			for (auto iter : _uiMsgList)
			{
				uiMsg.Remove(iter);
			}
			_uiMsgList.Empty();
		}
	}
}

void FGsShortcutHandler::OnShortcutContentsMapId(IN int32 inMapId)
{		
	const FGsSchemaMapData* schemaMapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(inMapId);
	if (nullptr == schemaMapData)
		return;
	
	switch (schemaMapData->mapType)
	{
		break;
	case MapType::PUBLIC_FIELD:
	case MapType::INTER_FIELD:
		break;
	case MapType::INSTANCE_SINGLE:
		if (schemaMapData->ContentsType == MapContentsType::SPACE_CRACK)
		{
			// SPACE_CRACK
		}
		else
		{
			// PUBLIC_FIELD
		}
		break;
	case MapType::INSTANCE_FIELD:
		break;
	case MapType::PUBLIC_DUNGEON:
	case MapType::PARTY_DUNGEON:
	case MapType::ELITE_DUNGEON_1:
		if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::DUNGEON, true))
		{
			GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_DUNGEON);
			
			FGsPrimitiveInt32 param(inMapId);
			GMessage()->GetDungeon().SendMessage(MessageContentDungeon::WINDOW_MAP_ID, &param);
		}
		break;		
	case MapType::NONE:
	default:
		break;
	}
}

void FGsShortcutHandler::OnShortcutContentsItemId(IN ItemId inMapId)
{

}

void FGsShortcutHandler::OnShortcutContentsCreatureId(IN CreatureId inCreatureId)
{

}


void FGsShortcutHandler::OnShortcutContents(const IGsMessageParam* inParam)
{
	const FGsUIMsgShortcutMap* paramMap = inParam->Cast<const FGsUIMsgShortcutMap>();
	if (nullptr != paramMap)
	{
		if (paramMap->_mapId > 0)
		{
			OnShortcutContentsMapId(paramMap->_mapId);
		}
	}

	const FGsUIMsgShortcutItem* paramItem = inParam->Cast<const FGsUIMsgShortcutItem>();
	if (nullptr != paramItem)
	{
		if (paramItem->_itemId > 0)
		{
			OnShortcutContentsItemId(paramItem->_itemId);
		}
	}

	const FGsUIMsgShortcutCreature* paramCreature = inParam->Cast<const FGsUIMsgShortcutCreature>();
	if (nullptr != paramCreature)
	{
		if (paramCreature->_creatureId > 0)
		{
			OnShortcutContentsCreatureId(paramCreature->_creatureId);
		}
	}
}