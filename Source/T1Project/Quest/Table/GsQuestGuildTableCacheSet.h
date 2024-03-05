// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaQuestGuildStory;

struct T1PROJECT_API FGsQuestGuildStoryTableCache
{
	explicit FGsQuestGuildStoryTableCache(const FGsSchemaQuestGuildStory* schemaStoryItem)
		:_schemaStoryItem(schemaStoryItem) {}

public:
	const FGsSchemaQuestGuildStory* _schemaStoryItem;   // 스토리 스키마
	QuestId							_questId;			// 퀘스트Id

public:
	// Get 퀘스트Id 리스트
	QuestId GetQuestId() { return _questId; }
	// 스토리 스키마 정보
	const FGsSchemaQuestGuildStory* GetSchemaQuestStory() const { return _schemaStoryItem; }
};

/**
 * 
 */
class T1PROJECT_API FGsQuestGuildTableCacheSet
{
public:
	FGsQuestGuildTableCacheSet() = default;
	~FGsQuestGuildTableCacheSet() = default;

protected:
	TMap<StoryId, TSharedPtr<FGsQuestGuildStoryTableCache>>	_storyMap;

public:
	void Initialize();
	void Finalize();

private:
	const TArray<const FGsSchemaQuestGuildStory*> GetSchemaQuestGuildStoryList() const;

public:
	bool GetGuildStoryTableCache(StoryId inStoryId, OUT TSharedPtr<FGsQuestGuildStoryTableCache>& outStoryTableCache);
	// 퀘스트 id로 스토리id 찾기
	bool GetStoryId(QuestId inQuestId, OUT StoryId& outStoryId);
};