// Fill out your copyright notice in the Description page of Project Settings.


#include "GsQuestTableCache.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "T1Project.h"

#include "GsQuestObjectiveTableCache.h"

#include "Quest/GsSchemaQuest.h"
#include "Quest/GsSchemaQuestObjective.h"

#include "Management/ScopeGlobal/GsQuestManager.h"


FGsQuestTableCache::FGsQuestTableCache(const FGsSchemaQuest * inSchemaQuest)
{
	_schemaQuest = inSchemaQuest;	
	_questId = _schemaQuest->id;

	if (0 >= _questId)
	{
		GSLOG(Error, TEXT("[Quest] error questId : %llu"), _questId);
		//check(false);
	}
}

FGsQuestTableCache::~FGsQuestTableCache()
{
	Finalize();
}

void FGsQuestTableCache::Finalize()
{
	_objectiveTableCacheList.Empty();
}

void FGsQuestTableCache::Initialize()
{
	if (nullptr == _schemaQuest)
		return;
	
	for (const FGsSchemaQuestObjectiveRow& objectiveRow : _schemaQuest->objectiveList)
	{
		if (const FGsSchemaQuestObjective* objective = objectiveRow.GetRow())
		{
			TSharedPtr<FGsQuestObjectiveTableCache> newData = 
				MakeShareable(new FGsQuestObjectiveTableCache(objective->id, objective));
			newData->Initialize();
			_objectiveTableCacheList.Add(newData);
		}
	}
}
