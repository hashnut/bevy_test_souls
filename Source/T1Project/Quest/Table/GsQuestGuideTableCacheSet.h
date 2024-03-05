// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest/Guide/GsSchemaQuestGuideStory.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaQuestMainStory;
struct FGsSchemaBase;

/**
 *  퀘스트 스토리 데이터
 */
struct T1PROJECT_API FGsQuestGuideStoryTableCache
{
	explicit FGsQuestGuideStoryTableCache(const FGsSchemaQuestGuideStory* schemaStoryItem)
		:_schemaStoryItem(schemaStoryItem) {}

private:
	const FGsSchemaQuestGuideStory*	_schemaStoryItem;   // 스토리 스키마
	QuestId							_questId;			// 퀘스트Id 

public:
	// 스토리 스키마 정보
	const FGsSchemaQuestGuideStory* GetSchemaQuestStory() const { return _schemaStoryItem; }
	// 퀘스트 아이디 추가
	void SetQuestId(QuestId inQuestId) { _questId = inQuestId; }
	QuestId GetQuestId() { return _questId; }
	StoryId GetStoryId() { return _schemaStoryItem? _schemaStoryItem->id : 0; }
	int32 GetRequireLevel() { return _schemaStoryItem? _schemaStoryItem->requireLevel : 0; }
	StoryId GetRequireMainStoryId() {
		return ((nullptr == _schemaStoryItem) ? 0 : (_schemaStoryItem->requireMainStoryId.GetRow() ?
			_schemaStoryItem->requireMainStoryId.GetRow()->id : 0));
	}

	//bool CheckMainStroy(StoryId inStoryid) { return (inStoryid > (StoryId)_schemaStoryItem->requireMainStoryId)? true : false; }
};

/**
 * 
 */
class T1PROJECT_API FGsQuestGuideTableCacheSet
{
private:
	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>> _storyList;

public:
	FGsQuestGuideTableCacheSet() = default;
	~FGsQuestGuideTableCacheSet() = default;

public:
	void Initialize();
	void Finalize();

private:
	// 데이터 센터에서 스키마 Get 
	const TArray<const FGsSchemaQuestGuideStory*> GetSchemaQuestStoryList() const;

public:	
	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>>& GetGuideStoryTableCache() { return _storyList; }
	// 스토리아이디로 스토리캐시 데이터 찾기
	bool GetStoryTableCache(StoryId inStoryId, OUT TSharedPtr<FGsQuestGuideStoryTableCache>& outStoryTableCache);
	// 퀘스트아이디로 스토리캐시 데이터 찾기
	bool GetQuestStoryTableCache(QuestId inQuestId, OUT TSharedPtr<FGsQuestGuideStoryTableCache>& outStoryTableCache);

	// 퀘스트 id로 스토리id 찾기
	bool GetStoryId(QuestId inQuestId, OUT StoryId& outStoryId);
};