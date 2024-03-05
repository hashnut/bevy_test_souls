// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "GsSchemaEnums.h"
#include "SharedInclude/SharedTypes.h"
#include "GsUIWindowQuestGuideEntry.generated.h"

class UGsUIRedDotBase;
class UGsUIWindowQuestGuideItem;
class UWidgetSwitcher;
class UImage;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowQuestGuideEntry :
	public UUserWidget,
	public IUserObjectListEntry
{
	GENERATED_BODY()

	static const int32 SLOT_STATE_COUNT = 3;

protected:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _selectImg;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	class UGsUIRedDotBase* _redDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherHudOnOff;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _unSwitcherHudOnOff;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherGuideIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _levelLimitText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _storyText;

	TWeakObjectPtr<class UGsUIWindowQuestGuideItem> _item;

	bool _bIsSelected = false;	
	bool _bIsHudUIOnOffType = false;
	bool _bIsHudUIOn = false;
	bool _bIsSelecting = false;
	bool _bIsRedDot = false;
	int32 _levelLimit = 0;
	bool _bIsLock = false;
	int _swichIndex = 0;
	
	StoryId _storyId = INVALID_STORY_ID;		// 스토리 아이디
	int _slotIndex = 0;	
	QuestWindowUISlotType _slotType;			// 슬롯타입 (완료, 진행중,...)
	int32 _requireLevel = 0;

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickHudUIOnOffBtn();				// hudUI를 등록/해지한다.			

public:
	void SetData();
	void SetDataStory(int inSlotIndex, StoryId inStoryId);
	void SetSelect(bool inIsSelect);
	void SetRedDot(bool bInIsOn);
	void UpdateRedDot();
	void UpdateHudOnOff();

	StoryId GetStoryId() { return _storyId; }
	int GetSlotIdex() { return _slotIndex; }
	bool GetIsSelected() const;
	QuestWindowUISlotType GetSlotType() { return _slotType; }

	TWeakObjectPtr<class UGsUIWindowQuestGuideItem> GetItem() { return _item; }
};
