// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIBMShopStorageEntry.generated.h"


class UGsButton;
class UTextBlock;
class UGsUIIconBase;
class FGsBMShopStorageItemData;
class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBMShopStorageEntry : public UUserWidget
	, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAccept;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _withdrawDurationText;


protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _itemiconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _iconSlotHelper;


private:
	int32 _currentDay = 999;
	bool _isRunningTime = false;
	FGsBMShopStorageItemData* _storageItemData = nullptr;


protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void OnShow() override;
	virtual void OnHide() override;

public:
	void SetData(const uint64 InStorageItemDBID);

private:
	void OnLongPressIcon(class UGsUIIconItem& InIcon);
	

protected:
	UFUNCTION()
	void OnClickAccept();
	UFUNCTION()
	void OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon);
};
