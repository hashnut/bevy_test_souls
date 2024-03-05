// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameObjectData.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Define/GsGameObjectStruct.h"

FGsGameObjectData::FGsGameObjectData(const FGsSpawnParam* SpawnParam)
{
	_gameId = SpawnParam->_gameId;
	_tId = SpawnParam->_tId;
	_spawnId = SpawnParam->_spawnId;

	// Iff정보 초기화
	_iffFaction.mGuildId = 0;
	_iffFaction.mPartyId = 0;
	_iffFaction.mIffFactionType = IffFactionType::NEUTRAL;

	for (int32 i = 0; i < MAX_DIPLOMACY_GUILDS_COUNT; ++i)
	{
		_iffFaction.mEnemyGuildArray[i] = 0;
		_iffFaction.mAllianceGuildArray[i] = 0;
	}
}

void FGsGameObjectData::Initialize()
{
}

void FGsGameObjectData::Finalize()
{
}

void FGsGameObjectData::SetLevel(uint16 InLevel)
{
	_level = InLevel;
	OnSend(EGsGameObjectDataMember::Level);
}

void FGsGameObjectData::OnSend(EGsGameObjectDataMember MemberType)
{
	if (_onChangeGameObjectDataDelegate.IsBound())
	{
		_onChangeGameObjectDataDelegate.Broadcast(MemberType);
	}
}
