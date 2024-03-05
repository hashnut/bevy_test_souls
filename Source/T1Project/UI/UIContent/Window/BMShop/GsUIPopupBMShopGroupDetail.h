// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Reward/GsRewardHelper.h"
#include "GsUIPopupBMShopGroupDetail.generated.h"

class UGsButton;
class UVerticalBox;
class UGsDynamicPanelSlotHelper;
/**
 * 
 */
UCLASS()
class UGsUIPopupBMShopGroupDetail : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _itemListRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnConfirm;


protected:
	// 惑前 备己 酒捞袍 包府 府胶飘
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _itemListEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _itemListPanelSlotHelper;


private:
	TArray<FGsRewardUnboxingData> _rewardBoxDataList;

protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetProvideItem(TArray<FGsRewardUnboxingData> InDataList);


protected:
	UFUNCTION()
	void OnConfirm();
	UFUNCTION()
	void OnRefreshItem(int32 InIndex, UWidget* InEntry);

	void OnInputOk() final;
	void OnInputCancel() final;
};
