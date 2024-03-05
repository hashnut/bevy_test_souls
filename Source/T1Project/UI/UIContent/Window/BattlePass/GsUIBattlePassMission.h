// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIBattlePassMission.generated.h"

class UScrollBox;
class UPanelWidget;
class UWidgetSwitcher;
class UGsDynamicPanelSlotHelper;

struct FGsBattlePassMissionTab;

class FGsBattlePassData;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBattlePassMission : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _missionTab;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _missionTabPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _missionScrollView;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _missionScrollPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _missionSwitcher; // open no open

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _openTimeDesc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (DisplayName = "TabButtonWidget"))
	TSubclassOf<UUserWidget> _tabWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (DisplayName = "ListEntryWidget"))
	TSubclassOf<UUserWidget> _listEntryWidgetClass;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _tabScrollBoxHelper;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _missionScrollBoxHelper;

	FGsToggleGroup _toggleGroup;

protected:
	//TWeakPtr<FGsBattlePassMissionTab> _passMissionTab;
	
	int32 _selectTabIndex{ INDEX_NONE };
	TWeakPtr<FGsBattlePassData>  _battlePassData;
	TArray<TWeakPtr<FGsBattlePassMissionTab>> _missionTabList;

protected:
	UFUNCTION()
	void OnSelectedMissionTab(int tabIndex);
	
	UFUNCTION()
	void OnRefreshMissionItemEntry(int32 index, UWidget* InEntry);

	UFUNCTION()
	void OnCreateMissionTabEntry(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshMissionTabEntry(int32 index, UWidget* InEntry);

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void BeginDestroy() override;

public:
	//void SetData(TWeakPtr<FGsBattlePassMissionTab> missionTab);
	void SetData(TWeakPtr<FGsBattlePassData> passData);
	void RefreshUI();

	void UpdateMissionInfo(BattlePassMissionTabId tabId, BattlePassMissionId missionId);
};
