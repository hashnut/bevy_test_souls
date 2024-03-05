// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Quest/GsQuestData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../GsUIWindowQuestTitleItem.h"
#include "GsUIWindowQuestMainItem.generated.h"

struct FGsQuestChapterTableCache;
struct FGsQuestStoryTableCache;
struct FGsSchemaQuestSubStory;
class UTreeView;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowQuestMainItem : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TArray<UGsUIWindowQuestMainItem*> _childList;

public:
	bool _isChapter = false;
	int32 _chapterIndex = 0;
	int32 _storyIndex = 0;
	StoryId _storyId = INVALID_STORY_ID;

	void SetChapterItem(int32 inChapterIndex);
	void SetStoryItem(int32 inChapterIndex, int32 inStoryIndex, StoryId inStoryId);

	bool GetIsChapter() { return _isChapter; }
	int32 GetChapterIndex() { return _chapterIndex; }

	StoryId GetStoryId() { return _storyId; }
	int32 GetStoryIndex() { return _storyIndex; }

public:
	void AddChild(UGsUIWindowQuestMainItem* InItem);
	void removeChild(UGsUIWindowQuestMainItem* InItem);
	void removeAllChildren();

	TArray<UGsUIWindowQuestMainItem*> GetChildList() const { return _childList; }

	void OnGetChildren(UObject* InItem, TArray<UObject*>& InArray);
};
