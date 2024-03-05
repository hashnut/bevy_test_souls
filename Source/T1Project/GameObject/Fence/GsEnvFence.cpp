// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEnvFence.h"
#include "UTIL/GsText.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "GsFenceHandler.h"
#include "Management/ScopeGame/GsSpaceCrackManager.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Management/GsMessageHolder.h"

void FGsEnvFence::InitEnvFence(const FGsSchemaMapTerritoryEnv* inEnv, const struct FGsSchemaFenceElement* inFence)
{
	_env = inEnv;
	_fence = inFence;
}

bool FGsEnvFence::Enter()
{
	EGsEnvEvent envEvent;
	if (TryGetActiveEnv(envEvent))
	{	
		FGsGameObjectFenceParam param = FGsGameObjectFenceParam(this, envEvent);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_TERRITORY_ENTERED, &param);
		return true;
	}

	return false;
}

bool FGsEnvFence::Leave()
{
	FGsGameObjectFenceParam param = FGsGameObjectFenceParam(this, EGsEnvEvent::NONE);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_TERRITORY_LEFT, &param);

	return true;
}


FString FGsEnvFence::GetFenceDescription()
{
	FString descriptionText;
	if (nullptr == _env)
	{
		return descriptionText;
	}

	FString territoryTypeText = TEXT("Type : ") + FGsTextUtil::GetStringFromEnum(TEXT("TerritoryType"), TerritoryType::Env);		
	FString envEventText;
	const TArray<FGsEnvCondition>& envConditionArray = _env->envConditionList;
	for (const FGsEnvCondition& envCondition : envConditionArray)
	{
		envEventText = FGsTextUtil::GetStringFromEnum(TEXT("EGsEnvEvent"), envCondition._envEvent);
		descriptionText += TEXT("\n") + FString::Printf(TEXT("start : %lld"), envCondition._startQuestId) + FString::Printf(TEXT("\tend : %lld"), envCondition._endQuestId) + TEXT(" ") + envEventText;
	}

	return descriptionText;
}

bool FGsEnvFence::IsInFence(const FVector& inLocation) const
{
	return ((FVector2D(_fence->Center) - FVector2D(inLocation)).Size() <= _fence->Radius);
}

bool FGsEnvFence::HaveActiveCondition()
{
	return _env->envConditionList.Num() > 0;
}

bool FGsEnvFence::IsActive() const
{
	bool isAnyQuestActive = false;
	for (const FGsEnvCondition& condition : _env->envConditionList)
	{
		if ((0 != condition._startQuestId && 0 == condition._endQuestId)
			|| (0 != condition._startQuestId && condition._startQuestId == condition._endQuestId))
		{
			if (GSQuest()->IsProgress(condition._startQuestId))
			{
				isAnyQuestActive = true;
				break;
			}
		}
		else if (condition._startQuestId > 0
			&& condition._endQuestId > 0
			&& condition._startQuestId < condition._endQuestId)
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

bool FGsEnvFence::TryGetActiveEnv(OUT EGsEnvEvent& inEvent) const
{
	for (const FGsEnvCondition& condition : _env->envConditionList)
	{
		if ((0 != condition._startQuestId && 0 == condition._endQuestId)
			|| (0 != condition._startQuestId && condition._startQuestId == condition._endQuestId))
		{
			if (GSQuest()->IsProgress(condition._startQuestId))
			{
				inEvent = condition._envEvent;
#if TERRITORY_DEBUG
				FString conditionText = FGsTextUtil::GetStringFromEnum(TEXT("EGsEnvEvent"), condition._envEvent);
				GSLOG(Log, TEXT("env type : %s\tstart quest id : %lld"), *conditionText, condition._startQuestId);
#endif
				return true;
			}

		}
		else if (condition._startQuestId > 0
			&& condition._endQuestId > 0
			&& condition._startQuestId < condition._endQuestId)
		{
			if (GSQuest()->IsProgress(condition._startQuestId, condition._endQuestId))
			{
				inEvent = condition._envEvent;
#if TERRITORY_DEBUG
				FString conditionText = FGsTextUtil::GetStringFromEnum(TEXT("EGsEnvEvent"), condition._envEvent);
				GSLOG(Log, TEXT("env type : %s\tstart quest id : %lld\tend quest id : %lld"), *conditionText, condition._startQuestId, condition._endQuestId);
#endif
				return true;
			}
		}
	}

	return false;
}
