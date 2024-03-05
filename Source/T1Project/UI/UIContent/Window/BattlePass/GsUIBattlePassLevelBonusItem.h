// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "GsUIBattlePassLevelBonusItem.generated.h"

class UUserWidget;
class UImage;

class UGsUIIconBase;
class UGsHorizontalBoxIconSelector;
class UGsDynamicIconSlotHelper;
struct FGsBattlePassLevelBonusData;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBattlePassLevelBonusItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
	DECLARE_DELEGATE_OneParam(FOnReceiveBonus, TWeakPtr<FGsBattlePassLevelBonusData>);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _redDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _dimmed;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _acceptable;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textAmount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _rewardIconSelector;

	UPROPERTY()
	UGsDynamicIconSlotHelper* _rewardIconSlotHelper;

protected:
	TWeakPtr<FGsBattlePassLevelBonusData> _data;

public:
	FOnReceiveBonus OnReceiveReward;

protected:
	UFUNCTION()
	void OnCreateBonusRewardIcon(UGsUIIconBase* InIcon);
	
	UFUNCTION()
	void OnRefreshBonusRewardIcon(int32 InIndex, UGsUIIconBase* InIcon);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	void OnRecvBonusReward(UGsUIIconItem& icon);
	void OnLongPressIcon(UGsUIIconItem& InIcon);

public:
	virtual void BeginDestroy() override;

	void SetData(TWeakPtr<FGsBattlePassLevelBonusData> data) { _data = data; }
	void RefreshUI();
};
