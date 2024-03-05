// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Reward/GsRewardHelper.h"
#include "GsUIBMShopProvideGroup.generated.h"


class UVerticalBox;
class UTextBlock;
class UGsUIBMShopProductItem;
class UGsDynamicPanelSlotHelper;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBMShopProvideGroup : public UUserWidget
	, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _provideTitleText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _itemListRootPanel;
	


protected:
	// 惑前 备己 酒捞袍 包府 府胶飘
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _itemListEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _itemListPanelSlotHelper;


private:
	FGsRewardUnboxingData _data;
	CurrencyPair _cashRebateInfo;

private:
	bool _hideDetailBtn = false;

private:
	void SetTitleText(const RewardType InType);

public:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;
	virtual void OnShow() override;
	virtual void OnHide() override;

public:
	void SetProvideItem(FGsRewardUnboxingData InData,bool InIsUseLastData = false, bool InHideDetailBtn = false);
	void SetCashRebateInfo(CurrencyPair InInfo) { _cashRebateInfo = InInfo; }

protected:
	UFUNCTION()
	void OnRefreshItem(int32 InIndex, UWidget* InEntry);
};
