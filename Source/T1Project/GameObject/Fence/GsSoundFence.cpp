// Fill out your copyright notice in the Description page of Project Settings.

#include "GsSoundFence.h"
#include "UTIL/GsText.h"
#include "GsFenceHandler.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsSpaceCrackManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Sound/GsSoundPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Define/GsGameObjectDefine.h"

FGsSoundFence::~FGsSoundFence()
{
	
}

void FGsSoundFence::InitSoundFence(const FGsSchemaMapTerritorySound* inSound, const struct FGsSchemaFenceElement* inFence)
{
	_sound = inSound;
	_fence = inFence;
}

bool FGsSoundFence::Enter()
{
	GMessage()->GetSound().SendMessage(MessageSound::PLAY_BGM, nullptr);
	return true;
}

bool FGsSoundFence::Leave()
{
	GMessage()->GetSound().SendMessage(MessageSound::PLAY_BGM, nullptr);
	return true;
}

FString FGsSoundFence::GetFenceDescription()
{
	FString descriptionText;
	if (nullptr == _sound)
	{
		return descriptionText;
	}

	FString territoryTypeText = TEXT("Type : ") + FGsTextUtil::GetStringFromEnum(TEXT("TerritoryType"), TerritoryType::SOUND);	
	FString soundIdText;
	const TArray<FGsQuestSound>& questSoundArray = _sound->questSoundList;
	for (const FGsQuestSound& questSound : questSoundArray)
	{
		soundIdText = TEXT(" sound : ") + (questSound._sound.GetRow() != nullptr ? questSound._sound.GetRow()->path.ToString() : TEXT("none"));
		descriptionText += TEXT("\n") + FString::Printf(TEXT("start : %lld"), questSound._startQuestId) + FString::Printf(TEXT(" end : %lld"), questSound._endQuestId) + TEXT(" ") + soundIdText;
	}

	return descriptionText;
}

bool FGsSoundFence::HaveActiveCondition()
{
	return _sound->questSoundList.Num() > 0;
}

bool FGsSoundFence::IsActive() const
{
	bool isAnyQuestActive = false;
	for (const FGsQuestSound& condition : _sound->questSoundList)
	{
		if (0 != condition._startQuestId
			&& 0 == condition._endQuestId)
		{
			if (GSQuest()->IsProgress(condition._startQuestId))
			{
				isAnyQuestActive = true;
				break;
			}
		}
		else if (condition._startQuestId > 0
			&& condition._endQuestId > 0
			&& condition._startQuestId <= condition._endQuestId)
		{
			if (GSQuest()->IsProgress(condition._startQuestId, condition._endQuestId))
			{
				isAnyQuestActive = true;
				break;
			}
		}
	}

	return isAnyQuestActive;
}

bool FGsSoundFence::TryGetActiveSound(OUT FGsSchemaSoundResDataRow& outSound) const
{
	for (const FGsQuestSound& condition : _sound->questSoundList)
	{
		if (0 != condition._startQuestId
			&& 0 == condition._endQuestId)
		{
			if (GSQuest()->IsProgress(condition._startQuestId))
			{
				outSound = condition._sound;
#if TERRITORY_DEBUG				
				FString conditionText = FString::FromInt(outSound.GetRow()->id);
				GSLOG(Log, TEXT("sound : %s\ttstart quest id : %lld"), *conditionText, condition._startQuestId);
#endif
				return true;
			}

		}
		else if (condition._startQuestId > 0
			&& condition._endQuestId > 0
			&& condition._startQuestId <= condition._endQuestId)
		{
			if (GSQuest()->IsProgress(condition._startQuestId, condition._endQuestId))
			{
				outSound = condition._sound;
#if TERRITORY_DEBUG
				FString conditionText = FString::FromInt(outSound.GetRow()->id);
				GSLOG(Log, TEXT("sound id : %s\tstart quest id : %lld\tend quest id : %lld"), *conditionText, condition._startQuestId, condition._endQuestId);
#endif
				return true;
			}
		}
	}

	return false;
}
