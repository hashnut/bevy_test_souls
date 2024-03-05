// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Quest/GsQuestData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../GsUIWindowQuestTitleItem.h"
#include "GsUIWindowQuestRepeatItem.generated.h"


/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowQuestRepeatItem : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnClickSlot, const int32, const int32);

protected:
	UPROPERTY()
	TArray<UGsUIWindowQuestRepeatItem*> _childList;

public:
	FOnClickSlot  _delegateClickHudUIOnOffBtn;
	FOnClickSlot  _delegateClickHudUIUpBtn;
	FOnClickSlot  _delegateClickHudUIDownBtn;

public:
	void AddChild(UGsUIWindowQuestRepeatItem* InItem);
	void removeChild(UGsUIWindowQuestRepeatItem* InItem);
	void removeAllChildren();

	TArray<UGsUIWindowQuestRepeatItem*> GetChildList() const { return _childList; }
	void OnGetChildren(UObject* InItem, TArray<UObject*>& InArray);

public:
	StoryId _storyId = INVALID_STORY_ID;
	QuestKey _questKey = QuestKey::KeyNone();
	int _slotIndex = 0;
	bool _isFirst = false;
	bool _isLast = false;

	void SetQuestItem(StoryId inStoryId, QuestKey inQuestKey, int inSlotIndex);
	void SetQuestItem(StoryId inStoryId, QuestKey inQuestKey, int inSlotIndex, bool inIsFirst, bool inIsLast);
	void ReSetQuestItem(StoryId inStoryId, QuestKey inQuestKey);

	StoryId GetStoryId() { return _storyId; }
	QuestKey& GetQuestKey() { return _questKey; }
	
	int GetSlotIndex() { return _slotIndex; }
	void SetSlotIndex(int8 inSlotIndex) { _slotIndex = inSlotIndex; }

	bool GetIsFirst() { return _isFirst; }
	bool GetIsLast() { return _isLast; }
	bool SetFirst(bool inIsOn) { return _isFirst = inIsOn; }
	bool SetLast(bool inIsOff) { return _isLast = inIsOff; }
	bool IsFirst() { return _isFirst; }
	bool IsLast() { return _isLast; }
};
