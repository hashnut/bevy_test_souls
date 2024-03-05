// Fill out your copyright notice in the Description page of Project Settings.


#include "GsServerFence.h"
#include "UTIL/GsText.h"
#include "Sound/GsSoundPlayer.h"
#include "GsFenceHandler.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsSpaceCrackManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/GsMessageHolder.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Fence/GsFenceHandler.h"

FGsServerFence::~FGsServerFence()
{
}

TerritoryType FGsServerFence::GetTerritoryType() const
{
	if (_territory)
	{
		return _territory->territoryType;
	}

	return TerritoryType::NONE;
}

bool FGsServerFence::TryGetActiveSound(OUT FGsSchemaSoundResDataRow& outSound) const
{
	if (GetTerritoryData()->sound.GetRow())
	{
		outSound = GetTerritoryData()->sound;
		return true;
	}

	return false;
}

bool FGsServerFence::IsActiveNpcList() const
{
	if (_territory)
	{
		return (_territory->territoryType == TerritoryType::TOWN);
	}

	return false;
}

bool FGsServerFence::IsSafetyZone() const
{
	if (_territory)
	{
		switch (GetTerritoryType())
		{
		case TerritoryType::TOWN:
		case TerritoryType::SAFE_ZONE:
		{
			return true;
		}
			break;
		}
	}

	return false;
}

bool FGsServerFence::IsArenaReady() const
{
	if (_territory)
	{
		if (_territory->territoryType == TerritoryType::ARENA_READY)
		{
			return true;
		}
	}

	return false;
}

FString FGsServerFence::GetFenceDescription()
{
	FString descriptionText;
	if (nullptr == _territory)
	{
		return descriptionText;
	}

	descriptionText = TEXT("Type : ") + FGsTextUtil::GetStringFromEnum(TEXT("TerritoryType"), _territory->territoryType);	
	return descriptionText;
}

void FGsServerFence::Initialize(const struct FGsSchemaMapTerritoryInfo* inMapTerritoryInfo, const struct FGsSchemaFenceElement* inFence)
{
	_territory = inMapTerritoryInfo;
	_fence = inFence;
}

void FGsServerFence::Clear()
{
	
}

bool FGsServerFence::Enter()
{
	GMessage()->GetSound().SendMessage(MessageSound::PLAY_BGM, nullptr);
	return true;
}

bool FGsServerFence::ReEnter()
{
	GMessage()->GetSound().SendMessage(MessageSound::PLAY_BGM, nullptr);
	return true;
}

bool FGsServerFence::Leave()
{
	GMessage()->GetSound().SendMessage(MessageSound::PLAY_BGM, nullptr);
	return true;
}