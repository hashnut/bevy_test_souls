// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaBase;
struct FGsSchemaQuestRepeatScroll;
struct FGsSchemaQuestRepeatStory;

struct T1PROJECT_API FGsQuestRepeatStoryTableCache
{
	explicit FGsQuestRepeatStoryTableCache(const FGsSchemaQuestRepeatStory* schemaStoryItem)
		:_schemaStoryItem(schemaStoryItem) {}

public:
	const FGsSchemaQuestRepeatStory*	_schemaStoryItem;   // 스토리 스키마
	TArray<QuestId>						_questIdList;		// 퀘스트Id 리스트

public:
	// Get 퀘스트Id 리스트
	TArray<QuestId>& GetQuestIdList() { return _questIdList; }
	// 퀘스트 아이디 추가
	void AddtQuestId(QuestId inQuestId) { _questIdList.Add(inQuestId); }
	// 스토리 스키마 정보
	const FGsSchemaQuestRepeatStory* GetSchemaQuestStory() const { return _schemaStoryItem; }
};

struct T1PROJECT_API FGsQuestRepeatScrollTableCache
{
	explicit FGsQuestRepeatScrollTableCache(const FGsSchemaQuestRepeatScroll* questRepeatScroll)
		:_scrollItem(questRepeatScroll) {}

private:	
	const FGsSchemaQuestRepeatScroll*					_scrollItem;				// 스크롤
	TArray<TSharedPtr<FGsQuestRepeatStoryTableCache>>	_storyTableCacheList;		// 스토리 tableCache

};

/**
 * 
 */
class T1PROJECT_API FGsQuestRepeatTableCacheSet
{
public:
	FGsQuestRepeatTableCacheSet() = default;
	~FGsQuestRepeatTableCacheSet() = default;

protected:
	TMap<StoryId, TSharedPtr<FGsQuestRepeatStoryTableCache>>	_storyMap;

public:
	void Initialize();
	void Finalize();

private:
	const TArray<const FGsSchemaQuestRepeatScroll*> GetSchemaQuestRepeatScrollList() const;
	const TArray<const FGsSchemaQuestRepeatStory*> GetSchemaQuestRepeatStoryList() const;

public:
	bool GetRepeatStoryTableCache(StoryId inStoryId, OUT TSharedPtr<FGsQuestRepeatStoryTableCache>& outStoryTableCache);
	// 퀘스트 클라에서 자동 습득 하므로 다음 퀘스트아이디를 넘겨준다.
	bool GetNextQuestId(StoryId inStoryId, QuestId inQuestId, OUT QuestId& outNextQuestId);
	// 퀘스트 id로 스토리id 찾기
	bool GetStoryId(QuestId inQuestId, OUT StoryId& outStoryId);
};
