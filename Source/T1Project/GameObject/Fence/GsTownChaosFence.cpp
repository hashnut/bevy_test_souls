// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTownChaosFence.h"
#include "UTIL/GsText.h"
#include "GsFenceHandler.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsSpaceCrackManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Message/MessageParam/GsInvasionMessageParam.h"
#include "Message/GsMessageInvasion.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Fence/GsFenceHandler.h"

FGsTownChaosFence::FGsTownChaosFence()
{
	FGsMessageHolder* msg = GMessage();
	if (msg)
	{
		MInvasion& invasionMessage = msg->GetInvasion();
		_messageInvasionList.Empty();
		_messageInvasionList.Emplace(invasionMessage.AddRaw(MessageInvasion::INVAION_START, this, &FGsTownChaosFence::OnActiveChaosFence));
		_messageInvasionList.Emplace(invasionMessage.AddRaw(MessageInvasion::INVAION_FINISH, this, &FGsTownChaosFence::OnDeactiveChaosFence));
		_messageInvasionList.Emplace(invasionMessage.AddRaw(MessageInvasion::INVALID_TOWN_CHAOS_FENCE, this, &FGsTownChaosFence::OnChangeChaosFence));
	}
}

FGsTownChaosFence::~FGsTownChaosFence()
{
	FGsMessageHolder* msg = GMessage();
	if (msg)
	{
		MInvasion& invasionMessage = msg->GetInvasion();
		for (const MsgInvasionHandle& contentQuestPair : _messageInvasionList)
		{
			invasionMessage.Remove(contentQuestPair);
		}
		_messageInvasionList.Empty();
	}	
}

void FGsTownChaosFence::OnActiveChaosFence(const struct IGsMessageParam* InParam)
{
	SetChaos(true);
}

void FGsTownChaosFence::OnDeactiveChaosFence(const struct IGsMessageParam* InParam)
{
	SetChaos(false);
}

void FGsTownChaosFence::OnChangeChaosFence(const struct IGsMessageParam* InParam)
{
	const FGsInvasionChaosParam* param = InParam->Cast<const FGsInvasionChaosParam>();
	if (nullptr == param)
	{
		return;
	}

	SetChaos(param->_isChaos);
}

void FGsTownChaosFence::SetChaos(bool inChaos)
{
	_isChaos = inChaos;

#if INVASION_TOWN_BATTLE_DEBUG
	if (inChaos)
	{
		GSLOG(Log, TEXT("Invasion town - TownChaos is chaos state"));
	}
	else
	{
		GSLOG(Log, TEXT("Invasion town - TownChaos is town state"));
	}
#endif
}

void FGsTownChaosFence::Initialize(const struct FGsSchemaMapTerritoryInfo* inMapTerritoryInfo, const struct FGsSchemaFenceElement* inFence)
{
	FGsServerFence::Initialize(inMapTerritoryInfo, inFence);

	bool isInvade = GGameData()->IsInvadeWorld() || GGameData()->GetInvasionDefenseState();
	SetChaos(isInvade);
}

bool FGsTownChaosFence::Enter()
{
	GMessage()->GetSound().SendMessage(MessageSound::PLAY_BGM, nullptr);
	return true;
}

bool FGsTownChaosFence::Leave()
{
	GMessage()->GetSound().SendMessage(MessageSound::PLAY_BGM, nullptr);

	return true;
}