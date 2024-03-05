// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "BattlePass/GsBattlePassLevelData.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "GsUIBattlePassLevelRewardItem.generated.h"

class UGsHorizontalBoxIconSelector;
class UGsWrapBoxInventoryIconSelector;
class UGsDynamicIconSlotHelper;
class UGsUIIconBase;
class UImage;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBattlePassLevelRewardItem : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnReceiveReward, bool, TWeakPtr<FGsBattlePassLevelData>);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _redDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _dimmed;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _acceptable;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _levelText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _runLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _basicRewardIconSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsWrapBoxInventoryIconSelector* _addRewardIconSelector;

	UPROPERTY()
	UGsDynamicIconSlotHelper* _basicRewardIconSlotHelper;

	UPROPERTY()
	UGsDynamicIconSlotHelper* _addRewardIconSlotHelper;

protected:
	TWeakPtr<FGsBattlePassLevelData> _data;

public:
	FOnReceiveReward OnReceiveReward;

protected:
	UFUNCTION()
	void OnCreateBasicRewardIcon(UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnRefreshBasicRewardIcon(int32 InIndex, UGsUIIconBase* InIcon);

	UFUNCTION()
	void OnCreateAddRewardIcon(UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnRefreshAddRewardIcon(int32 InIndex, UGsUIIconBase* InIcon);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	void OnRecvBasicReward(UGsUIIconItem& icon);
	void OnRecvAdditionalReward(UGsUIIconItem& icon);
	void OnLongPressIcon(UGsUIIconItem& InIcon);

public:
	virtual void BeginDestroy() override;

	void SetData(TWeakPtr<FGsBattlePassLevelData> data) { _data = data; }
	void RefreshUI();
};
