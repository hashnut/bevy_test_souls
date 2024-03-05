// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Quest/GsQuestData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIWindowQuestGuideItem.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowQuestGuideItem : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnClickSlot, const int32, const int32);

protected:
	UPROPERTY()
	TArray<UGsUIWindowQuestGuideItem*> _childList;
	
	int _slotIndex = 0;
	StoryId _storyId = INVALID_STORY_ID;

public:
	FOnClickSlot  _delegateClickHudUIOnOffBtn;	

public:
	void AddChild(UGsUIWindowQuestGuideItem* InItem);
	void removeChild(UGsUIWindowQuestGuideItem* InItem);
	void removeAllChildren();

	TArray<UGsUIWindowQuestGuideItem*> GetChildList() const { return _childList; }

	void OnGetChildren(UObject* InItem, TArray<UObject*>& InArray);

public:
	void SetStoryItem(int inSlotIndex, StoryId inStoryId);

	StoryId GetStoryId() { return _storyId; }
	int GetSlotIndex() { return _slotIndex; }
};
