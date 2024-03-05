// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIPopupRankingRewardInfo.generated.h"

class UHorizontalBox;
class UGsButton;
class UListView;
class UWidget;
class UUserWidget;
class UPanelWidget;
class UGsDynamicPanelSlotHelper;
class UScrollBox;
struct FGsSchemaRankingReward;
struct FGsSchemaRankingCategory;
struct FGsSchemaRankingTypeObjective;

/**
 * ·©Å· º¸»ó Á¤º¸ ÆË¾÷
 */
UCLASS()
class T1PROJECT_API UGsUIPopupRankingRewardInfo : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	/************************************************************************/
	/* Button                                                               */
	/************************************************************************/
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	
	/************************************************************************/
	/* Tab, Reward Info                                                     */
	/************************************************************************/
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _rootPanelMainTab;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _rootPanelReward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfMainTab;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfReward;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperMainTabBtn;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperReward;

	FGsToggleGroup _toggleGroupMainTab;
	
	TArray<const FGsSchemaRankingCategory*> _mainTabDataList;
	TArray<const FGsSchemaRankingReward*> _rewardDataList;

	/************************************************************************/
	/* overrides                                                            */
	/************************************************************************/
public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/************************************************************************/
	/* Tab, Reward List                                                     */
	/************************************************************************/
private:
	void InvalidateMainTab();
	void InvalidateRewardList(int32 InMainTabIndex);

private:
	void OnSelectChangedMainTab(int32 InIndex);

private:
	UFUNCTION()
	void MainTabListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void RewardListRefresh(int32 InIndex, UWidget* InEntry);

	/************************************************************************/
	/* Etc..                                                                */
	/************************************************************************/
	UFUNCTION()
	void OnClickOk();
	
protected:
	virtual void OnInputCancel() override;
};
