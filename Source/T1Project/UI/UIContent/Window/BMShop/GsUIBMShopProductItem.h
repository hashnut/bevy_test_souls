// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIBMShopProductItem.generated.h"


class UTextBlock;
class UGsButton;
class UGsUIIconBase;
class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBMShopProductItem : public UUserWidget
	, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDetail;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemNameText;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _itemiconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _iconSlotHelper;


private:
	CurrencyPair _cashRebateInfo;
	const FGsSchemaRewardItemBagData* _itemBagData = nullptr;
	
public:
	virtual void OnShow() override;
	virtual void OnHide() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;

public:
	void SetData(CurrencyPair InCurrencyPairData);
	void SetData(const FGsSchemaRewardItemBagData* InBagData);
	void SetDetailBtnVisibility(const bool InFlag);

private:
	void OnLongPressIcon(class UGsUIIconItem& InIcon);

protected:
	UFUNCTION()
	void OnDetail();
	UFUNCTION()
	void OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon);

};
