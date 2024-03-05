// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIBattlePassPage.generated.h"

class UImage;
class UScrollBox;
class UPanelWidget;
class UGsUIProgressBar;
class UGsButton;
class UGsSwitcherButton;
class UWidgetSwitcher;
class UGsUIBattlePassLevel;
class UGsUIBattlePassMission;
class UGsUIBattlePassTab;

class UGsDynamicPanelSlotHelper;

class FGsBattlePassData;
struct FGsBattlePassMissionTab;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBattlePassPage : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIBattlePassTab* _levelTab;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIBattlePassTab* _missionTab;

	FGsToggleGroup _tabToggleGroup;
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherLevelMission;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIBattlePassLevel* _contentLevel;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIBattlePassMission* _contentMission;

protected:
	enum class PASSTAB : int8 { INVALID = -1, LEVEL = 0, MISSON };

	PASSTAB _selectedTabType{ PASSTAB::INVALID };

private:
	TWeakPtr<FGsBattlePassData> _battlePassData;

protected:
	UFUNCTION()
	void OnSelectedTab(int tabIndex);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	virtual void BeginDestroy() override;

public:
	void SetSelectedBattlePass(TWeakPtr<FGsBattlePassData> battlePassData);
	void Reset();

public:
	void SetActiveAddReward();
	void AckReceiveReward(Level level, bool isAddReward);
	void AckReceiveRewardAll(const TArray<Level>& level);
	void UpdateBattlePassLevel();
	void UpdateBattlePassMission(BattlePassMissionTabId tabId, BattlePassMissionId missionId);

	void OnUpdateRedDot();
};
