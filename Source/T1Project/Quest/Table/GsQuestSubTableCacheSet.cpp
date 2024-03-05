// Fill out your copyright notice in the Description page of Project Settings.


#include "GsQuestSubTableCacheSet.h"

#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Quest/Sub/GsSchemaQuestSubChapter.h"
#include "Quest/Sub/GsSchemaQuestSubStory.h"
#include "Map/GsSchemaMapData.h"
#include "Dungeon/GsSchemaDungeonGroupData.h"
#include "Dungeon/GsSchemaDungeonData.h"
#include "GameObject/Npc/GsSchemaNpcData.h"

#include "Management/ScopeGlobal/GsQuestManager.h"

#include "UTIL/GsTableUtil.h"

void FGsQuestSubTableCacheSet::Initialize()
{
	const TArray<const FGsSchemaQuestSubChapter*> chapterList = GetSchemaQuestChapterList();
	int32 npcId;
	int32 mapId;
	const FGsSchemaMapData* mapData;
	for (const FGsSchemaQuestSubChapter* chapterItem : chapterList)
	{
		if (nullptr == chapterItem
		|| nullptr == chapterItem->npcId.GetRow())
		{
			continue;
		}

		npcId = chapterItem->npcId.GetRow()->id;
		mapId = chapterItem->npcPos.mapId;
		mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(mapId);
		if (mapData->mapType == MapType::PUBLIC_DUNGEON)
		{			
			mapId = GetDungeonGroupId(mapId);						
		}

		TSharedPtr<FGsQuestSubChapterTableCache> newData = MakeShareable(new FGsQuestSubChapterTableCache(chapterItem));

		for (FGsSchemaQuestSubStoryRow itemRow : chapterItem->questStoryList)
		{
			if (const FGsSchemaQuestSubStory* schemaStoryItem = itemRow.GetRow())
			{
				TSharedPtr<FGsQuestSubStoryTableCache> storyTableCache = MakeShareable(new FGsQuestSubStoryTableCache(schemaStoryItem));
				for (auto& schemaQuest : schemaStoryItem->questIdList)
				{
					if (const FGsSchemaQuest* questData = schemaQuest.GetRow())
					{
						storyTableCache->AddtQuestId((nullptr == questData) ? 0 : questData->id);
					}
				}

				newData->AddStoryTableCache(storyTableCache);
			}
		}

		_chapterMap.Emplace(npcId, newData);
		_chapterAreaMap.FindOrAdd(mapId).Add(newData);
		_chapterLevelMap.FindOrAdd(chapterItem->npcPos.mapId).Add(newData);
	}
}

void FGsQuestSubTableCacheSet::Finalize()
{
	_chapterMap.Empty();
	_chapterAreaMap.Empty();
	_chapterLevelMap.Empty();
}

const TArray<const FGsSchemaQuestSubChapter*> FGsQuestSubTableCacheSet::GetSchemaQuestChapterList() const
{
	TArray<const FGsSchemaQuestSubChapter*> questChapterList;
	if (const UGsTable* table = FGsSchemaQuestSubChapter::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaQuestSubChapter>(questChapterList);
	}

	return questChapterList;
}

bool FGsQuestSubTableCacheSet::GetChapterTableCache(int32 inNpcId, OUT TSharedPtr<FGsQuestSubChapterTableCache>& outChapterTableCache)
{
	outChapterTableCache = _chapterMap.FindRef(inNpcId);
	if (!outChapterTableCache.IsValid())
		return false;
	
	return true;
}

TArray<TSharedPtr<FGsQuestSubStoryTableCache>>& FGsQuestSubTableCacheSet::GetStoryTableCacheList(int32 inNpcId)
{
	TSharedPtr<FGsQuestSubChapterTableCache> chapterTableCache = _chapterMap.FindRef(inNpcId);

	return chapterTableCache->GetStoryTableCacheList();
}

bool FGsQuestSubTableCacheSet::GetStoryTableCache(QuestId inQuestId, OUT TSharedPtr<FGsQuestSubStoryTableCache>& outStoryTableCache, OUT int32& outNpcId)
{
	for (auto& iterChapter : _chapterMap)
	{
		TSharedPtr<FGsQuestSubChapterTableCache>& chapterTableCache = iterChapter.Value;
		if (!chapterTableCache.IsValid())
			continue;

		TArray<TSharedPtr<FGsQuestSubStoryTableCache>> &storyTableCacheList = chapterTableCache->GetStoryTableCacheList();
		for (TSharedPtr<FGsQuestSubStoryTableCache> storyTableCache : storyTableCacheList)
		{
			for (QuestId questId : storyTableCache->GetQuestIdList())
			{
				if (questId != inQuestId)
					continue;
				
				outStoryTableCache = storyTableCache;
				outNpcId = iterChapter.Key;
				return true;				
			}
		}
	}

	return false;
}

bool FGsQuestSubTableCacheSet::GetStoryIndexAndNpcID(QuestId inQuestId, OUT int32& outStoryIndex, OUT int32& outNpcId)
{
	for (auto& iterChapter : _chapterMap)
	{
		TSharedPtr<FGsQuestSubChapterTableCache>& chapterTableCache = iterChapter.Value;
		if (!chapterTableCache.IsValid())
			continue;

		TArray<TSharedPtr<FGsQuestSubStoryTableCache>> &storyTableCacheList = chapterTableCache->GetStoryTableCacheList();
		for (int i = 0; i < storyTableCacheList.Num(); ++i)
		{
			TSharedPtr<FGsQuestSubStoryTableCache> storyTableCache = storyTableCacheList[i];
			if(!storyTableCache.IsValid())
				continue;

			for (QuestId questId : storyTableCache->GetQuestIdList())
			{
				if (questId != inQuestId)
					continue;

				outStoryIndex = i;
				outNpcId = iterChapter.Key;
				return true;
			}
		}
	}

	return false;
}

TSharedPtr<FGsQuestSubStoryTableCache> FGsQuestSubTableCacheSet::GetNextSubStoryTableCache(int32 inNpcId, StoryId inStoryId)
{
	TArray<TSharedPtr<FGsQuestSubStoryTableCache>>& subStoryTableCacheList = GetStoryTableCacheList(inNpcId);
	int num = subStoryTableCacheList.Num();
	for (int i = 0; i < num; ++i)
	{
		TSharedPtr<FGsQuestSubStoryTableCache> subStoryTableCache = subStoryTableCacheList[i];
		if(!subStoryTableCache.IsValid())
			continue;

		const FGsSchemaQuestSubStory* SchemaSubStory = subStoryTableCache->GetSchemaQuestStory();
		if (nullptr == SchemaSubStory)
			continue;

		if (SchemaSubStory->id != inStoryId)
			continue;

		if (i + 1 < num)
		{
			return subStoryTableCacheList[i + 1];
		}		
	}

	return nullptr;
}

TSharedPtr<FGsQuestSubStoryTableCache> FGsQuestSubTableCacheSet::GetSubStoryTableCache(int32 inNpcId, StoryId inStoryId)
{
	TArray<TSharedPtr<FGsQuestSubStoryTableCache>>& subStoryTableCacheList = GetStoryTableCacheList(inNpcId);
	int num = subStoryTableCacheList.Num();
	for (int i = 0; i < num; ++i)
	{
		TSharedPtr<FGsQuestSubStoryTableCache> subStoryTableCache = subStoryTableCacheList[i];
		if (!subStoryTableCache.IsValid())
			continue;

		const FGsSchemaQuestSubStory* SchemaSubStory = subStoryTableCache->GetSchemaQuestStory();
		if (nullptr == SchemaSubStory)
			continue;

		if (SchemaSubStory->id != inStoryId)
			continue;

		return subStoryTableCacheList[i];		
	}

	return nullptr;
}

// 퀘스트 클라에서 자동 습득 하므로 다음 퀘스트아이디를 넘겨준다.
bool FGsQuestSubTableCacheSet::GetNextQuestId(StoryId inStoryId, QuestId inQuestId, int32 inNpcId, QuestId& outQuestId)
{
	TArray<TSharedPtr<FGsQuestSubStoryTableCache>>& subStoryTableCacheList = GetStoryTableCacheList(inNpcId);
	int num = subStoryTableCacheList.Num();
	for (int i = 0; i < num; ++i)
	{
		TSharedPtr<FGsQuestSubStoryTableCache> subStoryTableCache = subStoryTableCacheList[i];
		if (!subStoryTableCache.IsValid())
			continue;

		const FGsSchemaQuestSubStory* SchemaSubStory = subStoryTableCache->GetSchemaQuestStory();
		if (nullptr == SchemaSubStory)
			continue;

		if (SchemaSubStory->id != inStoryId)
			continue;

		TArray<QuestId>& questIdList = subStoryTableCache->GetQuestIdList();
		int32 questListNum = questIdList.Num();
		for (int j = 0; j < questListNum; ++j)
		{
			if (questIdList[j] != inQuestId)
				continue;

			if (j + 1 < questListNum)
			{
				outQuestId = questIdList[j + 1];
				return true;
			}
		}		
	}

	return false;
}


QuestId FGsQuestSubTableCacheSet::GetFirstQuestId(int32 inNpcId, StoryId inStoryId)
{
	TArray<TSharedPtr<FGsQuestSubStoryTableCache>>& subStoryTableCacheList = GetStoryTableCacheList(inNpcId);
	int num = subStoryTableCacheList.Num();
	for (int i = 0; i < num; ++i)
	{
		TSharedPtr<FGsQuestSubStoryTableCache> subStoryTableCache = subStoryTableCacheList[i];
		if (!subStoryTableCache.IsValid())
			continue;

		const FGsSchemaQuestSubStory* SchemaSubStory = subStoryTableCache->GetSchemaQuestStory();
		if (nullptr == SchemaSubStory)
			continue;

		if (SchemaSubStory->id != inStoryId)
			continue;

		TArray<QuestId>& questIdList = subStoryTableCache->GetQuestIdList();
		int32 questListNum = questIdList.Num();
		if (0 < questListNum)
		{
			return questIdList[0];
		}
		
		break;
	}

	return 0;
}

// 스토리아이디로 npcId 찾기
bool FGsQuestSubTableCacheSet::GetNpcId(StoryId inStoryId, OUT int32& outNpcId)
{
	for (auto& iterChapter : _chapterMap)
	{
		TSharedPtr<FGsQuestSubChapterTableCache>& chapterTableCache = iterChapter.Value;
		if (!chapterTableCache.IsValid())
			continue;

		TArray<TSharedPtr<FGsQuestSubStoryTableCache>>& storyTableCacheList = chapterTableCache->GetStoryTableCacheList();
		for (TSharedPtr<FGsQuestSubStoryTableCache> storyTableCache : storyTableCacheList)
		{
			if (storyTableCache->GetStoryId() == inStoryId)
			{
				outNpcId = chapterTableCache->GetNpcId();
				return true;
			}
		}
	}
	
	return false;
}

int32 FGsQuestSubTableCacheSet::GetDungeonGroupId(IN int32 inMapId)
{
	TArray<const FGsSchemaDungeonGroupData*> dungeonGroupDataList;
	if (const UGsTable* table = FGsSchemaDungeonGroupData::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaDungeonGroupData>(dungeonGroupDataList);
	}

	for (const FGsSchemaDungeonGroupData* dungeonGroupData : dungeonGroupDataList)
	{
		if (nullptr == dungeonGroupData)
			continue;

		for (auto& iterDungeon : dungeonGroupData->dungeonList)
		{
			if (const FGsSchemaDungeonData* schemaDungeonData = iterDungeon.GetRow())
			{
				if (const FGsSchemaMapData* schemaMapData = schemaDungeonData->mapId.GetRow())
				{
					if (schemaMapData->id == inMapId)
					{
						return dungeonGroupData->id;
					}
				}
			}
		}
	}

	return 0;
}

bool FGsQuestSubTableCacheSet::IsDungeonMap(IN int32 inDungeonGroupId, IN int32 inUserMapId)
{
	TArray<const FGsSchemaDungeonGroupData*> dungeonGroupDataList;
	if (const UGsTable* table = FGsSchemaDungeonGroupData::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaDungeonGroupData>(dungeonGroupDataList);
	}

	for (const FGsSchemaDungeonGroupData* dungeonGroupData : dungeonGroupDataList)
	{
		if (nullptr == dungeonGroupData)
			continue;

		if (dungeonGroupData->id == inDungeonGroupId)
		{			
			for (auto& iterDungeon : dungeonGroupData->dungeonList)
			{
				if (const FGsSchemaDungeonData* schemaDungeonData = iterDungeon.GetRow())
				{
					if (const FGsSchemaMapData* schemaMapData = schemaDungeonData->mapId.GetRow())
					{
						if (schemaMapData->id == inUserMapId)
						{
							return true;
						}
					}
				}
			}
		}		
	}

	return false;
}

bool FGsQuestSubTableCacheSet::GetDungeonGroupName(DungeonGroupId inDungeonGroupId, OUT FText& outDungeonGroupName)
{
	TArray<const FGsSchemaDungeonGroupData*> dungeonGroupDataList;
	if (const UGsTable* table = FGsSchemaDungeonGroupData::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaDungeonGroupData>(dungeonGroupDataList);
	}

	for (const FGsSchemaDungeonGroupData* dungeonGroupData : dungeonGroupDataList)
	{
		if (nullptr == dungeonGroupData)
			continue;

		if (dungeonGroupData->id != inDungeonGroupId)
			continue;

		outDungeonGroupName = dungeonGroupData->dungeonGroupName;
		return true;
	}

	return false;
}