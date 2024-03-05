// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest/Main/GsSchemaQuestMainStory.h"
#include "Quest/Sub/GsSchemaQuestSubStory.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/Quest/Sub/GsSchemaQuestSubChapter.h"

struct FGsSchemaQuestMainChapter;
struct FGsSchemaQuestMainStory;
struct FGsSchemaBase;
struct FGsSchemaQuestSubStory;
struct FGsSchemaQuestSubChapter;

/**
 *  퀘스트 스토리 데이터
 */
struct T1PROJECT_API FGsQuestSubStoryTableCache
{
	explicit FGsQuestSubStoryTableCache(const FGsSchemaQuestSubStory* schemaStoryItem)
		:_schemaStoryItem(schemaStoryItem) {}

private:
	const FGsSchemaQuestSubStory*	_schemaStoryItem;   // 스토리 스키마
	TArray<QuestId>					_questIdList;		// 퀘스트Id 리스트

public:
	// Get 퀘스트Id 리스트
	TArray<QuestId>& GetQuestIdList() { return _questIdList; }
	// 스토리 스키마 정보
	const FGsSchemaQuestSubStory* GetSchemaQuestStory() const { return _schemaStoryItem; }
	// 퀘스트 아이디 추가
	void AddtQuestId(QuestId inQuestId) { _questIdList.Add(inQuestId); }
	//// 현재 퀘스트 인덱스
	//void SetCurrentQuestIndex(int32 inIndex) { _currentQuestIndex = inIndex; }
	// 배열에 첫번째 퀘스트
	QuestId GetFirstQuestId() { return 0 < _questIdList.Num() ? _questIdList[0] : 0; }
	StoryId GetStoryId() { return _schemaStoryItem ? _schemaStoryItem->id : 0; }

	bool CheckMainStroy(StoryId inStoryid) {
		return  (nullptr == _schemaStoryItem) ? false : ((inStoryid > (StoryId)_schemaStoryItem->requireMainStoryId) ? true : false);
	}
	// 서비스 항목
	bool IsExposeQuest() { return (nullptr == _schemaStoryItem) ? false : _schemaStoryItem->exposeQuest; }
};

/**
 *  "챕터"에 필요한 "스토리 스키마"를 캐싱
 */
struct T1PROJECT_API FGsQuestSubChapterTableCache
{
	explicit FGsQuestSubChapterTableCache(const FGsSchemaQuestSubChapter* questChapterItem)
		:_chapterItem(questChapterItem) {}

private:	
	const FGsSchemaQuestSubChapter*					_chapterItem;				// 챕터 스키마	
	TArray<TSharedPtr<FGsQuestSubStoryTableCache>>	_storyTableCacheList;		// 스토리 tableCache

public:
	// 챕터에 맞는 스토리캐시 추가
	void AddStoryTableCache(TSharedPtr<FGsQuestSubStoryTableCache> inStoryTableCache) { _storyTableCacheList.Add(inStoryTableCache); }

public:
	// 챕터 스키마 Get
	const FGsSchemaQuestSubChapter* GetSchemaQuestChapter() const { return _chapterItem; }
	// 캐싱 된 스토리 List Get
	TArray<TSharedPtr<FGsQuestSubStoryTableCache>>& GetStoryTableCacheList() { return _storyTableCacheList; }
	int32 GetNpcId() {		
		return (nullptr == _chapterItem) ? 0 : ((nullptr == _chapterItem->npcId.GetRow()) ? 0 : _chapterItem->npcId.GetRow()->id);
	}
	// 배열에 첫번째 스토리 퀘스트아이디
	QuestId GetFirstStroyQuestId() { return (0 < _storyTableCacheList.Num())? _storyTableCacheList[0]->GetFirstQuestId() : 0; }
	StoryId GetFirstStroyId() { return (0 < _storyTableCacheList.Num()) ? _storyTableCacheList[0]->GetStoryId() : 0; }
	QuestContentsType GetQuestContentsType() { return (nullptr == _chapterItem) ? QuestContentsType::NORMAL : _chapterItem->questContentsType; }
	TSharedPtr<FGsQuestSubStoryTableCache> GetFirstStroyTableCache() { return  _storyTableCacheList[0]; }
	TSharedPtr<FGsQuestSubStoryTableCache> GetStroyTableCache(StoryId inStoryId)
	{
		for (TSharedPtr<FGsQuestSubStoryTableCache> iter: _storyTableCacheList)
		{
			if (inStoryId == iter->GetStoryId())
				return iter;
		}

		return _storyTableCacheList[0];
	}
	
	// 현재 퀘스트의 다음 퀘스트아이디
	QuestId NextQuestId(StoryId inStoryId)
	{
		int32 num = _storyTableCacheList.Num();
		for (int i = 0; i < num; ++i)
		{
			TSharedPtr<FGsQuestSubStoryTableCache> storyTableCache = _storyTableCacheList[i];	
			if (!storyTableCache.IsValid())
				continue;

			if (storyTableCache->GetSchemaQuestStory()->id != inStoryId)
				continue;
			
			if (i + 1 < num)
			{
				TSharedPtr<FGsQuestSubStoryTableCache> nextStoryTableCache = _storyTableCacheList[i + 1];

				return nextStoryTableCache->GetFirstQuestId();
			}
			else
				return 0;
		}
		return 0;
	}
	// 서비스 항목
	bool IsExposeQuest() { return (nullptr == _chapterItem) ? false : _chapterItem->exposeQuest; }
};

/**
 * 
 */
class T1PROJECT_API FGsQuestSubTableCacheSet
{
private:
	TMap<int32, TSharedPtr<FGsQuestSubChapterTableCache>>	_chapterMap;
	TMap<int32, TArray<TSharedPtr<FGsQuestSubChapterTableCache>>>	_chapterAreaMap;
	TMap<int32, TArray<TSharedPtr<FGsQuestSubChapterTableCache>>>	_chapterLevelMap;

public:
	FGsQuestSubTableCacheSet() = default;
	~FGsQuestSubTableCacheSet() = default;

public:
	void Initialize();
	void Finalize();

private:
	// 데이터 센터에서 챕터 스키마 Get 
	const TArray<const FGsSchemaQuestSubChapter*> GetSchemaQuestChapterList() const;

public:
	// 퀘스트 서브 챕터 npc별 캐시 리스트
	TMap<int32, TSharedPtr<FGsQuestSubChapterTableCache>>& GetChapterTableCacheList() { return _chapterMap; }
	// 퀘스트 서브 챕터 Area별 캐시 리스트
	TMap<int32, TArray<TSharedPtr<FGsQuestSubChapterTableCache>>>& GetChapterAreaTableCacheList() { return _chapterAreaMap; }
	// 퀘스트 서브 챕터 Level별 캐시 리스트
	TMap<int32, TArray<TSharedPtr<FGsQuestSubChapterTableCache>>>& GetChapterLevelTableCacheList() { return _chapterLevelMap; }
	// 퀘스트 서브 챕터 캐시
	TSharedPtr<FGsQuestSubChapterTableCache> GetChapterTableCache(int32 inNpcId) { return _chapterMap.FindRef(inNpcId); }
	// 퀘스트 서브 챕터 캐시
	TArray<TSharedPtr<FGsQuestSubChapterTableCache>> GetLevelChapterTableCache(int32 inMapId) { return _chapterLevelMap.FindRef(inMapId); }
	// 퀘스트 서브 챕터 캐시
	bool GetChapterTableCache(int32 inNpcId, OUT TSharedPtr<FGsQuestSubChapterTableCache>& outChapterTableCache);
	// 챕터 별 스토리 캐시 리스트
	TArray<TSharedPtr<FGsQuestSubStoryTableCache>>& GetStoryTableCacheList(int32 inNpcId);
	// 퀘스트아이디로 스토리캐시 데이터와 npcId 찾기
	bool GetStoryTableCache(QuestId inQuestId, OUT TSharedPtr<FGsQuestSubStoryTableCache>& outStoryTableCache, OUT int32& outNpcId);
	// 스토리아이디로 npcId 찾기
	bool GetNpcId(StoryId inStoryId, OUT int32& outNpcId);
	// 퀘스트아이디로 스토리 Index 랑 NpcId 찾기
	bool GetStoryIndexAndNpcID(QuestId inQuestId, OUT int32& outStoryIndex, OUT int32& outNpcId);
	// 다음 스토리 캐시 리스트
	TSharedPtr<FGsQuestSubStoryTableCache> GetNextSubStoryTableCache(int32 inNpcId, StoryId inStoryId);
	// 현재 스토리 캐시 리스트
	TSharedPtr<FGsQuestSubStoryTableCache> GetSubStoryTableCache(int32 inNpcId, StoryId inStoryId);
	// next 퀘스트아이디
	bool GetNextQuestId(StoryId inStoryId, QuestId inQuestId, int32 inNpcId, QuestId& outQuestId);
	// 스토리의 첫번째 퀘스트아이디
	QuestId GetFirstQuestId(int32 inNpcId, StoryId inStoryId);

	int32 GetDungeonGroupId(IN int32 inMapId);
	bool IsDungeonMap(IN int32 inDungeonGroupId, IN int32 inUserMapId);
	bool GetDungeonGroupName(DungeonGroupId inDungeonGroupId, OUT FText& outDungeonGroupName);
};