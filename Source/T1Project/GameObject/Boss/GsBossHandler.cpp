// Fill out your copyright notice in the Description page of Project Settings.


#include "GsBossHandler.h"
#include "GsWorldBossHandler.h"
#include "GsFieldBossHandler.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"

void FGsBossHandler::Init()
{
	_fieldBossHandler = TUniquePtr<FGsFieldBossHandler>(new FGsFieldBossHandler());
	if (_fieldBossHandler.IsValid())
	{
		_fieldBossHandler->Init();
	}

	_worldBossHandler = TUniquePtr<FGsWorldBossHandler>(new FGsWorldBossHandler());
	if (_worldBossHandler.IsValid())
	{
		_worldBossHandler->Init();
	}
}

void FGsBossHandler::Close()
{
	if (_fieldBossHandler.IsValid())
	{
		_fieldBossHandler->Close();
	}
	if (_worldBossHandler.IsValid())
	{
		_worldBossHandler->Close();
	}
}

bool FGsBossHandler::IsFieldBossSpawnGroup(MapId inMapId, int64 inSpawnGroup)
{
	TArray<const FGsSchemaFieldBossData*> fieldBossArray;
	FGsSchemaFieldBossData::GetStaticTable()->GetAllRows(fieldBossArray);

	const FGsSchemaFieldBossData** find = fieldBossArray.FindByPredicate([&](const FGsSchemaFieldBossData* bossData) {		
		if (nullptr == bossData->mapId.GetRow())
		{
			return false;
		}

		if (bossData->mapId.GetRow()->id != inMapId)
		{
			return false;
		}

		return bossData->spawnGroupIdList.Contains(inSpawnGroup);
		});

	return (nullptr != find);
}

FGsBossHandler* FGsBossHandler::GetInstance()
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return nullptr;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return nullptr;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return nullptr;
	}

	return  hud->GetBossHandler();
}