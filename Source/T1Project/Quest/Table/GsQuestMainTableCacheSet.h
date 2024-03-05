// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest/Main/GsSchemaQuestMainChapter.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
//#include "GsQuestMainTableCache.h"

struct FGsSchemaQuestMainChapter;
struct FGsSchemaQuestMainStory;

/**
 *  퀘스트 스토리 데이터
 */
struct T1PROJECT_API FGsQuestStoryTableCache
{
	explicit FGsQuestStoryTableCache(const FGsSchemaQuestMainStory* schemaStoryItem)
		:_schemaStoryItem(schemaStoryItem) {}

private:
	const FGsSchemaQuestMainStory*	_schemaStoryItem;   // 스토리 스키마
	TArray<QuestId>					_questIdList;		// 퀘스트Id 리스트
	int32							_storyIndex;

public:
	// Get 퀘스트Id 리스트
	TArray<QuestId>& GetQuestIdList() { return _questIdList; }
	// 스토리 스키마 정보
	const FGsSchemaQuestMainStory* GetSchemaQuestStory() const { return _schemaStoryItem; }
	// 배열에 첫번째 퀘스트
	QuestId GetFirstQuestId() { return _questIdList.Num() > 0 ? _questIdList[0] : 0; }
	// 스토리Id
	StoryId GetStoryId() { return (nullptr == _schemaStoryItem)?  0 : _schemaStoryItem->id;}
	// 서비스 항목
	bool IsExposeQuest() {	return (nullptr == _schemaStoryItem) ? false : _schemaStoryItem->exposeQuest;	}
	// 스토리index
	int32 GetStoryIndex() { return _storyIndex; }

public:
	void AddtQuestId(QuestId inQuestId) { _questIdList.Add(inQuestId); }
	void SetStoryIndex(int32 inIndex) { _storyIndex = inIndex; }
};

/**
 *  "챕터"에 필요한 "스토리 스키마"를 캐싱
 */
struct T1PROJECT_API FGsQuestChapterTableCache
{
	explicit FGsQuestChapterTableCache(const FGsSchemaQuestMainChapter* questChapterItem, int32 inChapterId)
		:_chapterItem(questChapterItem), _chapterId(inChapterId){}

private:
	const FGsSchemaQuestMainChapter*			_chapterItem;			// 챕터 스키마	
	TArray<TSharedPtr<FGsQuestStoryTableCache>>	_storyTableCacheList;	// 스토리 tableCache
	int32 _chapterId = 0;
	int32 _chapterIndex = 0;

public:
	// 챕터 스키마 Get
	const FGsSchemaQuestMainChapter* GetSchemaQuestChapter() const { return _chapterItem; }
	// 캐싱 된 스토리 List Get
	TArray<TSharedPtr<FGsQuestStoryTableCache>>& GetStoryTableCacheList()
	{
		return _storyTableCacheList;
	}

	int32 GetChapterId() { return _chapterId; }
	int32 GetChapterIndex() { return _chapterIndex; }
	void SetChapterIndex(int32 inChapterIndex) { _chapterIndex = inChapterIndex; }
	// 서비스 항목
	bool IsExposeQuest() { return (nullptr == _chapterItem) ? false : _chapterItem->exposeQuest; }

	// 챕터에 맞는 스토리캐시 추가
	void AddStoryTableCache(TSharedPtr<FGsQuestStoryTableCache> inStoryTableCache)
	{
		_storyTableCacheList.Add(inStoryTableCache);
	}

	TSharedPtr<FGsQuestStoryTableCache> GetStoryTableCache(int inIndex)
	{
		if (false == _storyTableCacheList.IsValidIndex(inIndex))
			return nullptr;

		return _storyTableCacheList[inIndex];
	}

	TSharedPtr<FGsQuestStoryTableCache> GetStoryTableCacheInId(StoryId inStoryId)
	{
		for (TSharedPtr<FGsQuestStoryTableCache> storyTableCache : _storyTableCacheList)
		{
			if (storyTableCache.IsValid())
			{
				if (storyTableCache->GetStoryId() == inStoryId)
				{
					return storyTableCache;
				}
			}
		}

		return nullptr;
	}

	StoryId GetInChapterFirstStoryId()
	{
		if (false == _storyTableCacheList.IsValidIndex(0))
			return 0;

		return _storyTableCacheList[0]->GetStoryId();
	}
};

/**
 * 
 */
class T1PROJECT_API FGsQuestMainTableCacheSet
{
private:
	// 현재 진행 중인 챕터	
	int32 _currentChapterIndex = 0;
	// 현재 진행 중인 스토리	
	int32 _currentStoryIndex = 0;
	// 첫 챕터 첫퀘스트ID
	QuestId _firstChapterQuestId = 0;

	TArray<TSharedPtr<FGsQuestChapterTableCache>> _chapterTableCacheList;	// 퀘스트메인에 속한 챕터테이블 리스트 [챕터스키마]

public:
	FGsQuestMainTableCacheSet() = default;
	~FGsQuestMainTableCacheSet() = default;

public:
	void Initialize();
	void Finalize();

	// 퀘스트 습득 시 테이블 로드
	void InitializeTableCache(QuestId inQuestId);
	//// 스토리 오픈 시 테이블 로드
	//void InitializeNextTableCache(QuestId inQuestId);

private:
	// 처음부터 모든 스토리를 들고 있는게 아니고 챕터가 오픈하면 그때 생성해서 들고 있는다.
	void CreateStoryTableCache(TSharedPtr<FGsQuestChapterTableCache> inQuestChapterTableCache) const;

private:
	// 데이터 센터에서 챕터 스키마 Get 
	const TArray<const FGsSchemaQuestMainChapter*> GetSchemaQuestChapterList() const;

public:
	// 퀘스트메인 테이블캐시 리스트 (챕터 -> 스토리 -> 퀘스트)
	const TArray<TSharedPtr<FGsQuestChapterTableCache>>& GetQuestChapterTableCacheList() const { return _chapterTableCacheList; }

	// 지정 챕터 테이블 캐시 리턴
	TSharedPtr<FGsQuestChapterTableCache> GetchapterTableCache(int32 inIndex);

	// 지정 챕터 테이블 캐시 리턴
	TSharedPtr<FGsQuestChapterTableCache> GetchapterTableCacheInIndex(int32 inChapterIndex);

	// 핸재 챕터의 테이블캐시 리턴
	TSharedPtr<FGsQuestChapterTableCache> GetCurrentQuestChapterTableCache() const;

	// 현재 챕터의 스토리의 테이블캐시 리턴
	TSharedPtr<FGsQuestStoryTableCache> GetCurrentQuestStoryTableCache() const;

	// 원하는 index의 스토리의 테이블캐시 리턴
	TSharedPtr<FGsQuestStoryTableCache> GetQuestStoryTableCache(int32 inIndex) const;

	// 퀘스트 클라에서 자동 습득 하므로 다음 퀘스트아이디를 넘겨준다.
	bool GetNextQuestId(QuestId inQuestId, QuestId &outNextQuestId);

	bool GetPreQuestId(QuestId inQuestId, QuestId &outPreQuestId);

	int32 GetCurrentQuestChapterIndex() const;

	// 현재 진행 중인 챕터 인덱스
	int32 GetChapterIndex() const { return _currentChapterIndex; }

	// 현재 진행 중인 스토리	인덱스
	int32 GetStoryIndex() const { return _currentStoryIndex; }

	// 현재 진행 중인 스토리	인덱스
	StoryId GetCurrentStoryId() const;

	// 현재 진행 중인 스토리	인덱스
	int32 GetCurrentStoryIndex() const;

	// 첫 퀘스트는 클라이언트에서 시작
	bool GetStartQuestId(QuestId &outQuestId);

	// 스토리가 열렸을 때 현재 인덱스 증가
	bool ActiveNextIndex();

	// ClearQuest 치트 명령 수행
	void ClearIndex();

	// 첫챕터 첫퀘스트 셋팅
	void SetFirstChapterQuestId();
	QuestId GetFirstChapterQuestId() const { return _firstChapterQuestId; }
	bool IsFirstChapterQuestId(QuestId inQuestId) const { return _firstChapterQuestId == inQuestId;}

	// 현재 진행 중인 챕터 또는 스토리가 노출이 금지되어 있는 상태인지 확인
	bool IsCurrentExposeQuest();
}; 
