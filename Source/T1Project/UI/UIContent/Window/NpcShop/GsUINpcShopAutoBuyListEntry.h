// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Management/ScopeGame/GsNpcShopManager.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "GsUINpcShopAutoBuyListEntry.generated.h"


class USlider;
class UTextBlock;
class UEditableTextBox;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;

// OnClick Entry Slot Event
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectAutoBuyListDelegate, UGsUINpcShopAutoBuyListEntry*);
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUINpcShopAutoBuyListEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsWrapBoxIconSelector* _iconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _iconSlotHelper;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	USlider* _amountSlider;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UProgressBar* _progressBarAmount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _InputText;

private:
	int32 _displayItemTID = 0;
	const ItemAmount DEFAULT_COUNT = 0;
	ItemAmount _inputTempCount = DEFAULT_COUNT;

public:
	FOnSelectAutoBuyListDelegate OnSelectAutoBuyListDelegate;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;

public:
	virtual void OnHide() override;
	virtual void OnShow() override;

public:
	void SetDisplayItem(const int32 InItemTID, const ItemAmount InSettingCount, const ItemAmount InMaxStackCount);
	void SetAmount(const ItemAmount InSettingCount, bool InChangeSlider = true);
	
private:
	void OnClickIconSlot(class UGsUIIconItem& InIcon);


public:
	UFUNCTION()
	void OnRefreshIconWrapBox(int32 InIndex, class UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnChangeSliderValue(float InChangeValue);
	UFUNCTION()
	void OnTextConfirmEvent(const FText& InText, ETextCommit::Type InType);
	UFUNCTION()
	void OnTextChangeEvent(const FText& InText);

public:
	int32 GetSettingItemTID() { return _displayItemTID; }
	ItemAmount GetSettingItemAmount();

};
