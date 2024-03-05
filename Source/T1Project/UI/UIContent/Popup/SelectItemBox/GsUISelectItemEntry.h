// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "GsUISelectItemEntry.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectIndexDelegate, const uint8);

class UImage;
class UGsButton;
class UTextBlock;
class UGsUIIconBase;
class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUISelectItemEntry : public UUserWidget
	, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSelect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _selectedImage;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _itemiconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _iconSlotHelper;


public:
	FOnSelectIndexDelegate OnSelectIndexDelegate;

private:
	uint8 _index = 0;
	const FGsSchemaRewardItemBagData* _itemBagData = nullptr;

protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;

public:
	virtual void OnShow() override;
	virtual void OnHide() override;
	
public:
	void SetData(const uint8 InIndex, const FGsSchemaRewardItemBagData* InBagData);
	void SetSelectedImage(bool InState);

private:
	void OnLongPressIcon(class UGsUIIconItem& InIcon);



protected:
	UFUNCTION()
	void OnClick();
	UFUNCTION()
	void OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon);
};
