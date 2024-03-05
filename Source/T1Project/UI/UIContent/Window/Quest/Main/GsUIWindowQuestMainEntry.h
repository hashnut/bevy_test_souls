// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "GsSchemaEnums.h"
#include "SharedInclude/SharedTypes.h"
#include "GsUIWindowQuestMainEntry.generated.h"

struct FGsQuestChapterData;
struct FGsQuestStoryData;
class UGsUIWindowQuestTitleItem;
class UImage;
class UCanvasPanel;
struct FSlateBrush;
class UTextBlock;
class UGsUIRedDotBase;
struct FGsSchemaQuestMainChapter;
struct FGsQuestChapterTableCache;
class UGsUIWindowQuestMainItem;
class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowQuestMainEntry :
	public UUserWidget,
	public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _selectImg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetOptional))
	class UGsUIRedDotBase* _redDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _nameText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _numText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	class UImage* _stateImg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherStorySlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherChapterSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherUnSelectSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherSelectSlot;

	TWeakObjectPtr<class UGsUIWindowQuestMainItem> _item;

public:
	bool _isChapter = false;
	bool _bIsSelected = false;
	bool _bIsChapterSlot = true;

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

	virtual void NativeDestruct() override;

public:
	void SetIsSelected(bool bInIsSelected);
	bool GetIsSelected() const;
	void SetRedDot(bool bInIsOn);

	TWeakObjectPtr<class UGsUIWindowQuestMainItem> GetItem() { return _item; }

	void SetDataChapter(int32 inChapterIndex);
	void SetDataStory(int32 inChapterIndex, StoryId inStoryId);

	void SetSelect(bool inIsSelect);
	bool GetIsChapter() { return _isChapter; }

	TSharedPtr<FGsQuestChapterTableCache> GetchapterTableCacheInIndex(int32 inChapterIndex);
	const FGsSchemaQuestMainChapter* GetSchemaQuestChapterInIndex(int32 inChapterIndex);
};
