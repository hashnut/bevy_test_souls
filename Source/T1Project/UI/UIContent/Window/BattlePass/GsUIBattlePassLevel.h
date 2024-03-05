// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "GsUIBattlePassLevel.generated.h"

class UScrollBox;
class UPanelWidget;
class UGsButton;
class UGsUICurrencyButton;
class UGsUIBattlePassLevelHelper;
class UGsUIBattlePassLevelRewardItem;
class UGsUIBattlePassLevelBonusItem;

class FGsBattlePassData;
struct FGsBattlePassLevelData;
struct FGsBattlePassLevelBonusData;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBattlePassLevel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _levelScrollView;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _dpLevelPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIBattlePassLevelRewardItem* _dpReward;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _addRewardDesc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnRewardEnable;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRecvRewardAll;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIBattlePassLevelBonusItem * _bonusItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (DisplayName = "rewardWidget"))
	TSubclassOf<UUserWidget> _entryReward;

	UPROPERTY()
	UGsUIBattlePassLevelHelper* _levelScrollBoxHelper;

	static bool isBlockClick;
	static FTimerHandle clickTimer;
private:
	TWeakPtr<FGsBattlePassData> _battlePassData;

	TArray<TWeakPtr<FGsBattlePassLevelData>> _passLevelData;
	TWeakPtr<FGsBattlePassLevelBonusData> _passBonusData;

	bool _isScrollEndPosition{ false };
	float _endTopOffset{ 0.f };

	TArray<FString> _inWidgets;

protected:
	UFUNCTION()
	void OnScrollEvent(float Offset);

	UFUNCTION(BlueprintCallable)
	void OnClickOpenAdditionalReward();

	UFUNCTION(BlueprintCallable)
	void OnReceiveRewardAll();
	
	UFUNCTION()
	void CreateLevelWidgets();
	
	UFUNCTION()
	void RefreshLevelWidgets();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	void OnReceiveReward(bool isAdditionalReward, TWeakPtr<FGsBattlePassLevelData> levelData);
	void OnReceiveRewardBonus(TWeakPtr<FGsBattlePassLevelBonusData> weaklevelData);

private:
	void SetDisplayItem(TWeakPtr<FGsBattlePassLevelData> dpItem);
	void SetScrollToWidget(TWeakPtr<FGsBattlePassLevelData> level);

	FORCEINLINE int32 GetCountLevelItems()
	{
		auto count = _passLevelData.Num();
		count += _passBonusData.IsValid() ? 1 : 0;
		return count;
	}

	bool IsScrollEndPosition();

public:
	virtual void BeginDestroy() override;

	void SetData(TWeakPtr<FGsBattlePassData> passData);
	void RefreshUI(bool init = true);

	void SetActiveAddReward();
	void SetRecvReward(Level level, bool isAddReward = false);
	void OnAckReceiveBattlePassLevelRewardAll(const TArray<Level>& receivedLevel);

	void OnUpdateRedDot();
};
