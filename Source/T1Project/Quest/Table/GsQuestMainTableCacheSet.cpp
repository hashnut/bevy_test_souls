// Fill out your copyright notice in the Description page of Project Settings.


#include "GsQuestMainTableCacheSet.h"

#include "T1Project.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Quest/Main/GsSchemaQuestMainChapter.h"
#include "Quest/Main/GsSchemaQuestMainStory.h"

#include "Management/ScopeGlobal/GsQuestManager.h"


// 초기에 첫챕터, 첫스토리를 로드 해 들고있는다.
void FGsQuestMainTableCacheSet::Initialize()
{
	const TArray<const FGsSchemaQuestMainChapter*> chapterList = GetSchemaQuestChapterList();

	int32 num = chapterList.Num();
	if (0 >= num) return;

	int index = 0;
	for (int i = 0; i < num; ++i)
	{
		if (const FGsSchemaQuestMainChapter* chapter = chapterList[i])
		{
			int32 chapterId = i;
			TSharedPtr<FGsQuestChapterTableCache> newData = MakeShareable(new FGsQuestChapterTableCache(chapter, chapterId));
			if (newData.IsValid())
			{
				if (false == newData->IsExposeQuest())
				{
					newData->SetChapterIndex(-1);
				}
				else
				{
					newData->SetChapterIndex(index);
					++index;
				}
			}

			if (i == 0)
			{
				CreateStoryTableCache(newData);
			}

			_chapterTableCacheList.Add(newData);
		}
	}

	// 첫챕터 첫퀘스트 셋팅
	SetFirstChapterQuestId();
}

void FGsQuestMainTableCacheSet::Finalize()
{
	_chapterTableCacheList.Empty();
}

// 습득한 퀘스트의 챕터, 스토리의 테이블 로드 및 익덱스 저장
void FGsQuestMainTableCacheSet::InitializeTableCache(QuestId inQuestId)
{
	int32 num = _chapterTableCacheList.Num();
	if (0 > _currentChapterIndex || num <= _currentChapterIndex) return;

	TSharedPtr<FGsQuestChapterTableCache> chapterTableCache = GetCurrentQuestChapterTableCache();
	if (nullptr == chapterTableCache)
		return;

	TArray<TSharedPtr<FGsQuestStoryTableCache>> storyTableCacheList = chapterTableCache->GetStoryTableCacheList();
	int32 stroyNume = storyTableCacheList.Num();
	if (0 >= stroyNume)
	{
		CreateStoryTableCache(chapterTableCache);
		storyTableCacheList = chapterTableCache->GetStoryTableCacheList();
		stroyNume = storyTableCacheList.Num();
	}

	TSharedPtr<FGsQuestStoryTableCache> storyTableCache = storyTableCacheList[_currentStoryIndex];
	if (nullptr == storyTableCache)
		return;

	TArray<QuestId> questIdList = storyTableCache->GetQuestIdList();
	bool isFind = questIdList.Contains(inQuestId);
	if (true == isFind)
		return;

	// 현재 진행 중이 스토리에 해당 퀘스트Id가 없다. 
	if (stroyNume > _currentStoryIndex + 1)
	{
		// 넥스트 스토리가 있으면 스토리인덱스를 증가한다.
		++_currentStoryIndex;
	}
	else
	{
		_currentStoryIndex = 0;
		// 넥스트 스토리가 없으면 스토리인텍스는 0으로 하고 챕터인덱스를 증가한다.
		if (num > _currentChapterIndex + 1)
		{
			++_currentChapterIndex;
		}
		else
		{
			return;
		}
	}

	InitializeTableCache(inQuestId);
}


// 현재 스토리에 다음 스토리인덱스를 계산한다.
bool FGsQuestMainTableCacheSet::ActiveNextIndex()
{
	int32 chapterNum = _chapterTableCacheList.Num();
	if (0 > _currentChapterIndex || chapterNum <= _currentChapterIndex)
		return false;

	TSharedPtr<FGsQuestChapterTableCache> chapterTableCache = GetCurrentQuestChapterTableCache();
	if (nullptr == chapterTableCache)
		return false;

	TArray<TSharedPtr<FGsQuestStoryTableCache>> storyTableCacheList = chapterTableCache->GetStoryTableCacheList();
	int32 stroyNume = storyTableCacheList.Num();
	if (0 >= stroyNume)
		return false;

	int nextStroyIndex = _currentStoryIndex + 1;
	if (stroyNume > nextStroyIndex)
	{
		for (int i = nextStroyIndex; i < stroyNume; ++i)
		{
			if (TSharedPtr<FGsQuestStoryTableCache> storyTableCache = storyTableCacheList[i])
			{
				if (true == storyTableCache->IsExposeQuest())
				{
					_currentStoryIndex = i;
					return true;
				}
			}
		}
	}
	else
	{
		int nextChapterIndex = _currentChapterIndex + 1;
		if (chapterNum > nextChapterIndex)
		{
			for (int i = nextChapterIndex; i < chapterNum; ++i)
			{
				if (TSharedPtr<FGsQuestChapterTableCache> nextChapterTableCache = _chapterTableCacheList[i])
				{
					if (true == nextChapterTableCache->IsExposeQuest())
					{
						_currentChapterIndex = i;
						_currentStoryIndex = 0;
						return true;
					}
				}
			}
		}
	}

	return false;
}

// 챕터가 시작되면 해당 스토리테이블을 캐싱한다.
void FGsQuestMainTableCacheSet::CreateStoryTableCache(TSharedPtr<FGsQuestChapterTableCache> inQuestChapterTableCache) const
{
	if (nullptr == inQuestChapterTableCache)
		return;

	const FGsSchemaQuestMainChapter* chapterItem = inQuestChapterTableCache->GetSchemaQuestChapter();
	if (nullptr == chapterItem)
		return;

	int8 index = 0;
	for (FGsSchemaQuestMainStoryRow itemRow : chapterItem->questStoryList)
	{
		if (const FGsSchemaQuestMainStory* schemaStoryItem = itemRow.GetRow())
		{
			TSharedPtr<FGsQuestStoryTableCache> storyTableCache = MakeShareable(new FGsQuestStoryTableCache(schemaStoryItem));
			for (auto& schemaQuest : schemaStoryItem->questIdList)
			{
				const FGsSchemaQuest* questData = schemaQuest.GetRow();
				storyTableCache->AddtQuestId((nullptr == questData) ? 0 : questData->id);
				
			}
			
			inQuestChapterTableCache->AddStoryTableCache(storyTableCache);

			if (false == storyTableCache->IsExposeQuest())
			{
				storyTableCache->SetStoryIndex(-1);
			}
			else
			{
				storyTableCache->SetStoryIndex(index);
				++index;
			}
		}
	}
}

// 퀘스트 클라에서 자동 습득 하므로 다음 퀘스트아이디를 넘겨준다.
bool FGsQuestMainTableCacheSet::GetNextQuestId(QuestId inQuestId, QuestId &outNextQuestId)
{
	int32 currIndex = 0;
	int32 nextIndex = 0;
	int32 questListNum = 0;

	TSharedPtr<FGsQuestStoryTableCache> storyTableCache = GetCurrentQuestStoryTableCache();
	if (nullptr == storyTableCache)
		return false;

	TArray<QuestId> questIdList = storyTableCache->GetQuestIdList();
	questListNum = questIdList.Num();
	if (questIdList.Find(inQuestId, currIndex))
	{
		nextIndex = currIndex + 1;
		if (questListNum > nextIndex)
		{
			// 다음퀘스트가 있다.
			outNextQuestId = questIdList[nextIndex];
			return true;
		}
	}

	return false;
}

bool FGsQuestMainTableCacheSet::GetPreQuestId(QuestId inQuestId, QuestId &outPreQuestId)
{
	int32 currIndex = 0;
	int32 nextIndex = 0;
	int32 questListNum = 0;

	TSharedPtr<FGsQuestStoryTableCache> storyTableCache = GetCurrentQuestStoryTableCache();
	if (nullptr == storyTableCache)
		return false;

	TArray<QuestId> questIdList = storyTableCache->GetQuestIdList();
	questListNum = questIdList.Num();
	if (questIdList.Find(inQuestId, currIndex))
	{
		nextIndex = currIndex - 1;
		if (questListNum > nextIndex)
		{
			// 다음퀘스트가 있다.
			if (questIdList.IsValidIndex(nextIndex))
			{
				outPreQuestId = questIdList[nextIndex];
				return true;
			}
		}
	}

	return false;
}

bool FGsQuestMainTableCacheSet::GetStartQuestId(QuestId &outQuestId)
{
	TSharedPtr<FGsQuestStoryTableCache> storyTableCache = GetCurrentQuestStoryTableCache();
	if (nullptr == storyTableCache)
		return false;

	TArray<QuestId> questIdList = storyTableCache->GetQuestIdList();
	int32 questListNum = questIdList.Num();
	if (0 < questListNum)
	{
		outQuestId = questIdList[0];
		return true;
	}

	return false;
}

TSharedPtr<FGsQuestChapterTableCache> FGsQuestMainTableCacheSet::GetCurrentQuestChapterTableCache() const
{
	return _chapterTableCacheList[_currentChapterIndex];
}

int32 FGsQuestMainTableCacheSet::GetCurrentQuestChapterIndex() const
{
	TSharedPtr<FGsQuestChapterTableCache> chapterTableCache = GetCurrentQuestChapterTableCache();
	if (!chapterTableCache.IsValid())
		return 0;

	return chapterTableCache->GetChapterIndex();
}

TSharedPtr<FGsQuestStoryTableCache> FGsQuestMainTableCacheSet::GetCurrentQuestStoryTableCache() const
{
	if (TSharedPtr<FGsQuestChapterTableCache> chapterTableCache = _chapterTableCacheList[_currentChapterIndex])
	{
		TArray<TSharedPtr<FGsQuestStoryTableCache>> storyTableCacheList = chapterTableCache->GetStoryTableCacheList();

		int32 stroyNume = storyTableCacheList.Num();
		if (0 >= stroyNume)
		{
			CreateStoryTableCache(chapterTableCache);
			storyTableCacheList = chapterTableCache->GetStoryTableCacheList();
			stroyNume = storyTableCacheList.Num();
		}

		if (0 < stroyNume && _currentStoryIndex < stroyNume)
		{
			return storyTableCacheList[_currentStoryIndex];
		}
	}

	return nullptr;
}

TSharedPtr<FGsQuestChapterTableCache> FGsQuestMainTableCacheSet::GetchapterTableCache(int32 inIndex)
{
	if (false == _chapterTableCacheList.IsValidIndex(inIndex))
		return nullptr;

	return _chapterTableCacheList[inIndex];
}

// 지정 챕터 테이블 캐시 리턴
TSharedPtr<FGsQuestChapterTableCache> FGsQuestMainTableCacheSet::GetchapterTableCacheInIndex(int32 inChapterIndex)
{
	for (TSharedPtr<FGsQuestChapterTableCache> chapterTableCache : _chapterTableCacheList)
	{
		if (nullptr == chapterTableCache)
			continue;

		if (false == chapterTableCache->IsExposeQuest())
			continue;

		if (chapterTableCache->GetChapterIndex() == inChapterIndex)
		{
			return chapterTableCache;
		}
	}

	return nullptr;
}

TSharedPtr<FGsQuestStoryTableCache> FGsQuestMainTableCacheSet::GetQuestStoryTableCache(int32 inIndex) const
{
	if (TSharedPtr<FGsQuestChapterTableCache> chapterTableCache = _chapterTableCacheList[_currentChapterIndex])
	{
		TArray<TSharedPtr<FGsQuestStoryTableCache>> storyTableCacheList = chapterTableCache->GetStoryTableCacheList();
		return storyTableCacheList[inIndex];
	}

	return nullptr;
}

const TArray<const FGsSchemaQuestMainChapter*> FGsQuestMainTableCacheSet::GetSchemaQuestChapterList() const
{
	TArray<const FGsSchemaQuestMainChapter*> questChapterList;
	if (const UGsTable* table = FGsSchemaQuestMainChapter::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaQuestMainChapter>(questChapterList);
	}

	return questChapterList;
}

void  FGsQuestMainTableCacheSet::ClearIndex()
{
	_currentChapterIndex = 0;
	_currentStoryIndex = 0;
}

void FGsQuestMainTableCacheSet::SetFirstChapterQuestId()
{
	if (0 >= _chapterTableCacheList.Num())
		return;

	if (TSharedPtr<FGsQuestChapterTableCache> chapterTableCache = _chapterTableCacheList[0])
	{
		TArray<TSharedPtr<FGsQuestStoryTableCache>> storyTableCacheList = chapterTableCache->GetStoryTableCacheList();

		int32 stroyNume = storyTableCacheList.Num();
		if (0 < stroyNume)
		{
			TSharedPtr<FGsQuestStoryTableCache> storyTableCache = storyTableCacheList[0];
			if (storyTableCache.IsValid())
			{
				TArray<QuestId> questIdList = storyTableCache->GetQuestIdList();
				int32 questListNum = questIdList.Num();
				if (0 < questListNum)
				{
					_firstChapterQuestId = questIdList[0];
					return;
				}
			}
		}
	}
}

StoryId FGsQuestMainTableCacheSet::GetCurrentStoryId() const
{
	TSharedPtr<FGsQuestStoryTableCache> storyTableCache = GetCurrentQuestStoryTableCache();
	if (!storyTableCache.IsValid())
		return INVALID_STORY_ID;

	return storyTableCache->GetStoryId();
}

int32 FGsQuestMainTableCacheSet::GetCurrentStoryIndex() const
{
	TSharedPtr<FGsQuestStoryTableCache> storyTableCache = GetCurrentQuestStoryTableCache();
	if (!storyTableCache.IsValid())
		return INVALID_STORY_ID;

	return storyTableCache->GetStoryIndex();
}

bool FGsQuestMainTableCacheSet::IsCurrentExposeQuest()
{
	TSharedPtr<FGsQuestChapterTableCache> chapterTableCache = GetCurrentQuestChapterTableCache();
	if (!chapterTableCache.IsValid())
		return false;

	if (true == chapterTableCache->IsExposeQuest())
	{
		TSharedPtr<FGsQuestStoryTableCache> storyTableCache = GetCurrentQuestStoryTableCache();
		if (!storyTableCache.IsValid())
			return false;

		return storyTableCache->IsExposeQuest();
	}

	return false;
}