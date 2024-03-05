// Fill out your copyright notice in the Description page of Project Settings.

#include "GsQuestPoolManager.h"
#include "GsQuestMainInstance.h"
#include "GsQuestSubInstance.h"
#include "GsQuestRepeatInstance.h"
#include "GsQuestRankInstance.h"
#include "GsQuestGuildInstance.h"
#include "GsQuestGuideInstance.h"
#include "T1Project.h"

void FGsQuestPoolManager::Initialize()
{
	_poolMap.Empty();
}

void FGsQuestPoolManager::Finalize()
{
	for (auto& iter : _poolMap)
	{
		FGsQuestPool* questPool = iter.Value;
		if (nullptr != questPool)
		{
			questPool->Flush();
			SAFE_DELETE(questPool);
		}		
	}

	_poolMap.Empty();
}

void FGsQuestPoolManager::ReleaseInstance(const QuestType inType, FGsQuestInstance* inData)
{
	FGsQuestPool* pool = _poolMap.FindRef(inType);
	if (nullptr != pool)
	{
		pool->Release(inData);
	}
}

FGsQuestInstance* FGsQuestPoolManager::ClaimInstance(const QuestType inType)
{
	FGsQuestPool* pool = FindPool(inType);
	if (nullptr == pool)
	{
		// ·Î±×»ðÀÔ
		return nullptr;
	}

	switch (inType)
	{
	case QuestType::MAIN:
		{
			return pool->Claim<FGsQuestMainInstance>();
		}
		break;
	case QuestType::SUB:
		{
			return pool->Claim<FGsQuestSubInstance>();
		}
		break;
	case QuestType::REPEAT:
		{
			return pool->Claim<FGsQuestRepeatInstance>();
		}
		break;
	case QuestType::MULTI_LEVEL_RANK:
		{
			return pool->Claim<FGsQuestRankInstance>();
		}break;
	case QuestType::GUILD:
		{
			return pool->Claim<FGsQuestGuildInstance>();
		}break;
	case QuestType::GUIDE:
		{
			return pool->Claim<FGsQuestGuideInstance>();
		}break;
	case QuestType::MAX:
		break;
	default:
		break;
	}

	return nullptr;
}

FGsQuestPool* FGsQuestPoolManager::FindPool(const QuestType inType)
{
	if (0 < _poolMap.Num())
	{
		if (FGsQuestPool* found = _poolMap.FindRef(inType))
		{
			return found;
		}
	}	

	return CreatePoolInternal(inType);
}

FGsQuestPool * FGsQuestPoolManager::CreatePoolInternal(const QuestType inType)
{
	FGsQuestPool* newPool = new FGsQuestPool();
	ensure(newPool);

	_poolMap.Emplace(inType, newPool);

	return newPool;
}
