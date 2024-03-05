// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaBase;
struct FGsSchemaQuestMultiLevelRankStory;

struct T1PROJECT_API FGsQuestMultiLevelRankStoryTableCache
{
	explicit FGsQuestMultiLevelRankStoryTableCache(const FGsSchemaQuestMultiLevelRankStory* storyItem)
		:_schemaStoryItem(storyItem) {}

public:
	const FGsSchemaQuestMultiLevelRankStory*	_schemaStoryItem;   // 스토리 스키마
	TArray<QuestId>								_questIdList;		// 퀘스트Id 리스트

public:
	// Get 퀘스트Id 리스트
	TArray<QuestId>& GetQuestIdList() { return _questIdList; }
	// 퀘스트 아이디 추가
	void AddtQuestId(QuestId inQuestId) { _questIdList.Add(inQuestId); }
	// 스토리 스키마 정보
	const FGsSchemaQuestMultiLevelRankStory* GetSchemaQuestStory() const { return _schemaStoryItem; }
};

/**
 *  다단계 랭크 미션 스토리테이블 CacheSet
 */
class T1PROJECT_API FGsQuestRankTableCacheSet
{
public:
	FGsQuestRankTableCacheSet() = default;
	~FGsQuestRankTableCacheSet() = default;

protected:
	TMap<StoryId, TSharedPtr<FGsQuestMultiLevelRankStoryTableCache>>	_storyMap;

public:
	void Initialize();
	void Finalize();

private:
	const TArray<const FGsSchemaQuestMultiLevelRankStory*> GetSchemaQuestMultiLevelRankStoryList() const;

public:
	bool GetRankStoryTableCache(StoryId inStoryId, OUT TSharedPtr<FGsQuestMultiLevelRankStoryTableCache>& outStoryTableCache);
	// 퀘스트 id로 스토리id 찾기
	bool GetStoryId(QuestId inQuestId, OUT StoryId& outStoryId);
};
