// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "GsSchemaEnums.h"
#include "SharedInclude/SharedTypes.h"
#include "Quest/GsQuestData.h"
#include "GsUIWindowQuestRepeatEntry.generated.h"

class UImage;
class UTextBlock;
class UGsUIRedDotBase;
class UWidgetSwitcher;
class UGsUIWindowQuestRepeatItem;
class UGsSwitcherButton;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowQuestRepeatEntry :
	public UUserWidget,
	public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	class UGsUIRedDotBase* _redDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _selectImg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _storyTypeImg;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _unStoryTypeImg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _storyText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _unStoryText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _typeText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _unTypeText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherHudOnOff;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _unSwitcherHudOnOff;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidget* _updownWidget;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidget* _updownSelectWidget;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnUp;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnDown;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnSelectUp;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnSelectDown;

	TWeakObjectPtr<class UGsUIWindowQuestRepeatItem> _item;

	bool _bIsSelected = false;
	bool _bIsHudUIOn = false;
	bool _bIsSelecting = false;
	bool _bIsRedDot = false;
	
	StoryId _storyId;		// 스토리 아이디
	QuestKey _questKey;
	RepeatStoryType _repeatStoryType = RepeatStoryType::BOARD;
	QuestState _questState = QuestState::ACCEPTED;

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

public:
	virtual void NativeOnInitialized() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickHudUIOnOffBtn();		// hudUI를 등록/해지한다.	

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickUp();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickDown();

public:
	bool GetIsSelected() const;

	StoryId GetStoryId() { return _storyId; }
	QuestKey GetQuestKey() { return _questKey; }

	void SetRedDot(bool bInIsOn);
	void UpdateRedDot();
	bool GetIsRedDot() { return _bIsRedDot; }

	TWeakObjectPtr<class UGsUIWindowQuestRepeatItem> GetItem() { return _item; }
	int8 GetSlotIndex();

public:
	void ReSetDataStory(StoryId inStoryId, const QuestKey& inQuestKey);
	void SetDataStory(StoryId inStoryId, const QuestKey& inQuestKey);
	void SetSelect(bool inIsSelect);
	void SetViewUpdDownButtons();
};
