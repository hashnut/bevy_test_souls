// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GameStatistics/GsLineGraphData.h"
#include "Message/GsMessageUI.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIWindowGameStatistics.generated.h"

class UPanelWidget;
class UScrollBox;
class UTextBlock;
class UGsButton;
class UGsDynamicPanelSlotHelper;
class UGsUILineGraph;
class UWidgetSwitcher;
struct IGsMessageParam;
class FGsGameStatisticsData;

/**
 * 통계창
 */
UCLASS()
class T1PROJECT_API UGsUIWindowGameStatistics : public UGsUIWindow
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUIGameStatistics")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootTab;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDate;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scroll;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootGraph;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDataSubCategory;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDataName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUILineGraph* _uiLineGraph;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootEmptyGraph;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockEmptyGraph;

protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelper;

protected:
	FGsToggleGroup _toggleGroupTab;
	FGsToggleGroup _toggleGroupList;

	FGsLineGraphData _graphData;

	MsgUIHandleArray _msgHandleUIList;
	TArray<const FGsGameStatisticsData*> _dataList;

	int32 _selectedIndex = -1;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual bool SyncContentState() override;
	virtual void InvalidateAllInternal() override;

private:
	void BindMessages();
	void UnbindMessages();

private:
	void InvalidateList(int32 InSelectedIndex);
	void InvalidateGraph(int32 InSelectedIndex);

private:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

	void OnSelectTab(int32 InIndex);
	void OnClickList(int32 InIndex, bool bIsSelected);

private:
	void OnUpdateList(const IGsMessageParam* InParam);
	void OnUpdateGraph(const IGsMessageParam* InParam);

private:
	StatisticsTabId GetSelectedTab() const;
	const FGsGameStatisticsData* GetSelectedData() const;
};
