// Fill out your copyright notice in the Description page of Project Settings.


#include "GsQuestRepeatTableCacheSet.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Quest/Repeat/GsSchemaQuestRepeatStory.h"
#include "Quest/Repeat/GsSchemaQuestRepeatScroll.h"

#include "Management/ScopeGlobal/GsQuestManager.h"


void FGsQuestRepeatTableCacheSet::Initialize()
{
	const TArray<const FGsSchemaQuestRepeatScroll*> schemaQuestRepeatScrollList = GetSchemaQuestRepeatScrollList();
	for (const FGsSchemaQuestRepeatScroll* schemaQuestRepeatScroll : schemaQuestRepeatScrollList)
	{
		for (FGsSchemaQuestRepeatStoryRow itemRow : schemaQuestRepeatScroll->questStoryList)
		{
			if (const FGsSchemaQuestRepeatStory* schemaStoryItem = itemRow.GetRow())
			{
				TSharedPtr<FGsQuestRepeatStoryTableCache> storyTableCache = MakeShareable(new FGsQuestRepeatStoryTableCache(schemaStoryItem));
				for (auto& schemaQuest : schemaStoryItem->questIdList)
				{
					if (const FGsSchemaQuest* questData = schemaQuest.GetRow())
					{
						storyTableCache->AddtQuestId((nullptr == questData) ? 0 : questData->id);
					}
				}

				_storyMap.Add(schemaStoryItem->id, storyTableCache);
			}
		}
	}
}

void FGsQuestRepeatTableCacheSet::Finalize()
{
	_storyMap.Empty();
}

const TArray<const FGsSchemaQuestRepeatScroll*> FGsQuestRepeatTableCacheSet::GetSchemaQuestRepeatScrollList() const
{
	TArray<const FGsSchemaQuestRepeatScroll*> schemaQuestRepeatScrollList;
	if (const UGsTable* table = FGsSchemaQuestRepeatScroll::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaQuestRepeatScroll>(schemaQuestRepeatScrollList);
	}

	return schemaQuestRepeatScrollList;
}

const TArray<const FGsSchemaQuestRepeatStory*> FGsQuestRepeatTableCacheSet::GetSchemaQuestRepeatStoryList() const
{
	TArray<const FGsSchemaQuestRepeatStory*> schemaQuestRepeatStoryList;
	if (const UGsTable* table = FGsSchemaQuestRepeatStory::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaQuestRepeatStory>(schemaQuestRepeatStoryList);
	}

	return schemaQuestRepeatStoryList;
}

bool FGsQuestRepeatTableCacheSet::GetRepeatStoryTableCache(StoryId inStoryId, OUT TSharedPtr<FGsQuestRepeatStoryTableCache>& outStoryTableCache)
{
	outStoryTableCache = _storyMap.FindRef(inStoryId);
	if (!outStoryTableCache.IsValid())
		return false;

	return true;
}

bool FGsQuestRepeatTableCacheSet::GetNextQuestId(StoryId inStoryId, QuestId inQuestId, OUT QuestId &outNextQuestId)
{
	TSharedPtr<FGsQuestRepeatStoryTableCache> repeatStoryTableCache = _storyMap.FindRef(inStoryId);

	if (!repeatStoryTableCache.IsValid())
		return false;

	const FGsSchemaQuestRepeatStory* SchemaRepeatStory = repeatStoryTableCache->GetSchemaQuestStory();
	if (nullptr == SchemaRepeatStory)
		return false;

	TArray<QuestId>& questIdList = repeatStoryTableCache->GetQuestIdList();
	int32 questListNum = questIdList.Num();
	for (int i = 0; i < questListNum; ++i)
	{
		if (questIdList[i] != inQuestId)
			continue;

		if (i + 1 < questListNum)
		{
			outNextQuestId = questIdList[i + 1];
			return true;
		}
	}
	return false;
}

bool FGsQuestRepeatTableCacheSet::GetStoryId(QuestId inQuestId, OUT StoryId& outStoryId)
{
	for (auto& iter : _storyMap)
	{
		TSharedPtr<FGsQuestRepeatStoryTableCache> repeatStoryTableCache = iter.Value;
		if (!repeatStoryTableCache.IsValid())
			continue;
		
		TArray<QuestId>& questIdList = repeatStoryTableCache->GetQuestIdList();
		int32 questListNum = questIdList.Num();
		for (int i = 0; i < questListNum; ++i)
		{
			if (questIdList[i] == inQuestId)
			{
				const FGsSchemaQuestRepeatStory* schemaQuestRepeatStory = repeatStoryTableCache->GetSchemaQuestStory();
				if (nullptr != schemaQuestRepeatStory)
				{
					outStoryId = schemaQuestRepeatStory->id;
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