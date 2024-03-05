// Fill out your copyright notice in the Description page of Project Settings.
#include "Quest/Table/GsQuestGuildTableCacheSet.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Quest/Guild/GsSchemaQuestGuildStory.h"

#include "Management/ScopeGlobal/GsQuestManager.h"


void FGsQuestGuildTableCacheSet::Initialize()
{
	TArray<const FGsSchemaQuestGuildStory*> storyList;
	if (const UGsTable* table = FGsSchemaQuestGuildStory::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaQuestGuildStory>(storyList);
	}

	for (const FGsSchemaQuestGuildStory* story : storyList)
	{
		TSharedPtr<FGsQuestGuildStoryTableCache> storyTableCache = MakeShareable(new FGsQuestGuildStoryTableCache(story));
		if (const FGsSchemaQuest* questData = story->questId.GetRow())
		{
			storyTableCache->_questId = questData->id;
		}

		_storyMap.Add(story->id, storyTableCache);
	}
}

void FGsQuestGuildTableCacheSet::Finalize()
{
	_storyMap.Empty();
}

const TArray<const FGsSchemaQuestGuildStory*> FGsQuestGuildTableCacheSet::GetSchemaQuestGuildStoryList() const
{
	TArray<const FGsSchemaQuestGuildStory*> schemaQuestGuildStoryList;
	if (const UGsTable* table = FGsSchemaQuestGuildStory::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaQuestGuildStory>(schemaQuestGuildStoryList);
	}

	return schemaQuestGuildStoryList;
}

bool FGsQuestGuildTableCacheSet::GetGuildStoryTableCache(StoryId inStoryId, OUT TSharedPtr<FGsQuestGuildStoryTableCache>& outStoryTableCache)
{
	outStoryTableCache = _storyMap.FindRef(inStoryId);
	if (!outStoryTableCache.IsValid())
		return false;

	return true;
}

bool FGsQuestGuildTableCacheSet::GetStoryId(QuestId inQuestId, OUT StoryId& outStoryId)
{
	for (auto& iter : _storyMap)
	{
		TSharedPtr<FGsQuestGuildStoryTableCache> guildStoryTableCache = iter.Value;
		if (!guildStoryTableCache.IsValid())
			continue;

		QuestId questId = guildStoryTableCache->GetQuestId();
		if (questId == inQuestId)
		{
			const FGsSchemaQuestGuildStory* schemaQuestGuildStory = guildStoryTableCache->GetSchemaQuestStory();
			if (nullptr != schemaQuestGuildStory)
			{
				outStoryId = schemaQuestGuildStory->id;
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}
