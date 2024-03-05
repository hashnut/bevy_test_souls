// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaQuest;
class FGsQuestObjectiveTableCache;

/**
 *  [퀘스트Table]->[오브젝티브Table]
 *  "퀘스트"에 필요한 스키마들을 캐싱
 */
class T1PROJECT_API FGsQuestTableCache
{
private:
	// 데이터 스키마
	const FGsSchemaQuest* _schemaQuest = nullptr;
	QuestId _questId;
	// 오브젝티브 리스트
	TArray<TSharedPtr<FGsQuestObjectiveTableCache>> _objectiveTableCacheList;

public:
	FGsQuestTableCache(const FGsSchemaQuest* inSchemaQuest);
	~FGsQuestTableCache();

public:
	void Initialize();
	void Finalize();

public:
	// 데이터 스키마
	const FGsSchemaQuest* GetSchemaQuest() const { return _schemaQuest; }
	// 오브젝티브 리스트
	TArray<TSharedPtr<FGsQuestObjectiveTableCache>> GetObjectiveTableCache() const { return _objectiveTableCacheList; }

	// 퀘스트 아이디
	const QuestId GetQuestId() const { return _questId; }
};
