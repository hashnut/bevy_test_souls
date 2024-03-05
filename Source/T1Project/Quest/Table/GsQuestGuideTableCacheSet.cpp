// Fill out your copyright notice in the Description page of Project Settings.


#include "GsQuestGuideTableCacheSet.h"
#include "Quest/Guide/GsSchemaQuestGuideStory.h"

#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Management/ScopeGlobal/GsQuestManager.h"

#include "Quest/GsSchemaQuest.h"

void FGsQuestGuideTableCacheSet::Initialize()
{
	const TArray<const FGsSchemaQuestGuideStory*> storyList = GetSchemaQuestStoryList();

	for (const FGsSchemaQuestGuideStory* storyItem : storyList)
	{
		TSharedPtr<FGsQuestGuideStoryTableCache> newData = MakeShareable(new FGsQuestGuideStoryTableCache(storyItem));
		if (const FGsSchemaQuest* questData = storyItem->questId.GetRow())
		{
			newData->SetQuestId((nullptr == questData) ? 0 : questData->id);
		}

		_storyList.Emplace(newData);
	}
}

void FGsQuestGuideTableCacheSet::Finalize()
{
	_storyList.Empty();
}

const TArray<const FGsSchemaQuestGuideStory*> FGsQuestGuideTableCacheSet::GetSchemaQuestStoryList() const
{
	TArray<const FGsSchemaQuestGuideStory*> questStoryList;
	if (const UGsTable* table = FGsSchemaQuestGuideStory::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaQuestGuideStory>(questStoryList);
	}

	return questStoryList;
}

bool FGsQuestGuideTableCacheSet::GetStoryTableCache(StoryId inStoryId, OUT TSharedPtr<FGsQuestGuideStoryTableCache>& outStoryTableCache)
{
	for (TSharedPtr<FGsQuestGuideStoryTableCache>& tableCache : _storyList)
	{
		if (!tableCache.IsValid())
			continue;
		
		if (tableCache->GetStoryId() != inStoryId)
			continue;

		outStoryTableCache = tableCache;
		return true;
	}

	return false;
}

bool FGsQuestGuideTableCacheSet::GetQuestStoryTableCache(QuestId inQuestId, OUT TSharedPtr<FGsQuestGuideStoryTableCache>& outStoryTableCache)
{
	for (TSharedPtr<FGsQuestGuideStoryTableCache>& tableCache : _storyList)
	{		
		if (!tableCache.IsValid())
			continue;

		if (tableCache->GetQuestId() != inQuestId)
			continue;

		outStoryTableCache = tableCache;
		return true;
	}

	return false;
}

bool FGsQuestGuideTableCacheSet::GetStoryId(QuestId inQuestId, OUT StoryId& outStoryId)
{
	for (auto& iter : _storyList)
	{
		TSharedPtr<FGsQuestGuideStoryTableCache>& tableCache = iter;
		if (!tableCache.IsValid())
			continue;

		if (tableCache->GetQuestId() != inQuestId)
			continue;

		outStoryId = tableCache->GetStoryId();
		return true;
	}

	return false;
}
