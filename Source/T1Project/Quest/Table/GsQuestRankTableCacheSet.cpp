// Fill out your copyright notice in the Description page of Project Settings.


#include "GsQuestRankTableCacheSet.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Quest/Rank/GsSchemaQuestMultiLevelRankStory.h"


void FGsQuestRankTableCacheSet::Initialize()
{
	const TArray<const FGsSchemaQuestMultiLevelRankStory*> schemaQuestMultiLevelRankStoryList = GetSchemaQuestMultiLevelRankStoryList();
	for (const FGsSchemaQuestMultiLevelRankStory* schemaQuestMultiLevelRankStory : schemaQuestMultiLevelRankStoryList)
	{		
		TSharedPtr<FGsQuestMultiLevelRankStoryTableCache> storyTableCache = MakeShareable(new FGsQuestMultiLevelRankStoryTableCache(schemaQuestMultiLevelRankStory));
		for (auto& schemaQuest : schemaQuestMultiLevelRankStory->questIdList)
		{
			if (const FGsSchemaQuest* questData = schemaQuest.GetRow())
			{
				storyTableCache->AddtQuestId((nullptr == questData) ? 0 : questData->id);
			}
		}

		_storyMap.Add(schemaQuestMultiLevelRankStory->id, storyTableCache);
	}
}

void FGsQuestRankTableCacheSet::Finalize()
{
	_storyMap.Empty();
}

const TArray<const FGsSchemaQuestMultiLevelRankStory*> FGsQuestRankTableCacheSet::GetSchemaQuestMultiLevelRankStoryList() const
{
	TArray<const FGsSchemaQuestMultiLevelRankStory*> schemaQuestMultiLevelRankStoryList;
	if (const UGsTable* table = FGsSchemaQuestMultiLevelRankStory::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaQuestMultiLevelRankStory>(schemaQuestMultiLevelRankStoryList);
	}

	return schemaQuestMultiLevelRankStoryList;
}

bool FGsQuestRankTableCacheSet::GetRankStoryTableCache(StoryId inStoryId, OUT TSharedPtr<FGsQuestMultiLevelRankStoryTableCache>& outStoryTableCache)
{
	outStoryTableCache = _storyMap.FindRef(inStoryId);
	if (!outStoryTableCache.IsValid())
		return false;

	return true;
}

bool FGsQuestRankTableCacheSet::GetStoryId(QuestId inQuestId, OUT StoryId& outStoryId)
{
	for (auto& iter : _storyMap)
	{
		TSharedPtr<FGsQuestMultiLevelRankStoryTableCache> multiLevelRankStoryTableCache = iter.Value;
		if (!multiLevelRankStoryTableCache.IsValid())
			continue;

		TArray<QuestId>& questIdList = multiLevelRankStoryTableCache->GetQuestIdList();
		int32 questListNum = questIdList.Num();
		for (int i = 0; i < questListNum; ++i)
		{
			if (questIdList[i] == inQuestId)
			{
				const FGsSchemaQuestMultiLevelRankStory* schemaQuestRankStory = multiLevelRankStoryTableCache->GetSchemaQuestStory();
				if (nullptr != schemaQuestRankStory)
				{
					outStoryId = schemaQuestRankStory->id;
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}

	return false;
}