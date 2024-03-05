// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIWindowBattlePass.generated.h"

struct FGsBattlePassMsgParamPass;
struct FGsBattlePassMsgParamLevelReward;
struct FGsBattlePassMsgParamLevel;
struct FGsBattlePassMsgParamMission;
struct FGsBattlePassMsgParamMissionTab;
struct FGsBattlePassMsgParamLevelRewardAll;

class UScrollBox;
class UPanelWidget;
class UImage;
class UGsButton;
class UGsUIProgressBar;

class UGsDynamicPanelSlotHelper;

class UGsUIBattlePassPage;
class UGsUIBattlePassTab;

class FGsBattlePassData;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowBattlePass : public UGsUIWindow
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _passNameText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _passDescText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _passPeriodText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FSlateBrush _passLevelIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _levelText;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _passBanner;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget)) 
	UGsUIProgressBar* _passProgress;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FSlateBrush _passExpIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _passExpText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnLevelUp;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _tabScrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _tabScrollPanel;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _tabScrollBoxHelper;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	TSubclassOf<UUserWidget> _tabBtnEntryClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIBattlePassPage* _battlePassPage;

	FGsToggleGroup _tabToggleGroup;

	int32 _selectTab;

	TArray<TWeakPtr<FGsBattlePassData>> _exposeBattlePasses;
	TWeakPtr<FGsBattlePassData> _selectedPass;
protected:
	UFUNCTION()
	void OnCreateTabButtonEntry(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshTabButtonEntry(int32 inIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnSelectTab(int32 index);

	UFUNCTION(BlueprintCallable)
	void OnClickLevelUp();

public:
	virtual void BeginDestroy() override;
	virtual bool SyncContentState() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void SetSelectedBattlePass(TWeakPtr<FGsBattlePassData> data);
	
public:
	void UpdateBattlePass(const FGsBattlePassMsgParamPass* param);
	void AckBattlePassLevelReward(const FGsBattlePassMsgParamLevelReward* param);
	void UpdateBattlePassLevel(const FGsBattlePassMsgParamLevel* param);
	void UpdateBattlePassMission(const FGsBattlePassMsgParamMission* param);

	void UpdateBattlePassMissionTab(const FGsBattlePassMsgParamMissionTab* param);

	void AckBattlePassLevelRewardAll(const FGsBattlePassMsgParamLevelRewardAll* param);

	void OnUpdateRedDot();
	void OnForcedClose();
};
