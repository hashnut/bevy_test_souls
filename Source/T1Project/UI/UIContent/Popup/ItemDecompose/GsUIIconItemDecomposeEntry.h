// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsUIIconItemDecomposeEntry.generated.h"

class UTextBlock;
class UGsButton;
class UPanelWidget;
class UWidgetSwitcher;
class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;

// Entry class Event
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnChangeSelectDelegate, const uint64, const bool, const int32);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnChangeUserSelectDelegate, const uint64, const bool, const int32);
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIIconItemDecomposeEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _iconHelper;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSelect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _selectSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _displayTypeSwitcher;		// 0 장착용 아이템 , 1 스킬북 아이템
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemSpecialStatNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _skillbookTypeText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _skillbookAmountText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemShieldCountText;
	

private:
	bool _isSelected = false;
	uint64 _itemDBID = 0;
	int32 _widgetIndex = 0;
	int32 _currentInputCount = 0;
	int32 _maxStackCount = 0;
	ItemGrade _itemGrade;

public:
	FOnChangeSelectDelegate OnChangeSelectDelegate;
	FOnChangeUserSelectDelegate OnChangeUserSelectDelegate;

public:
	virtual void BeginDestroy() override;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	virtual void OnShow() override;
	virtual void OnHide() override;

public:
	void SetItemInfo(const uint64 InItemDBID, const int32 InIndex);
	void SetSelectionInfo(const bool InFlag, const bool InExcuteEvent = true);
	bool GetSelectionInfo() { return _isSelected; }
	void SetStackCount(const int32 InStackCount);
	int32 GetStackCount();

public:
	uint64 GetSlotItemDBID() { return _itemDBID; }
	ItemGrade GetSlotItemGrade() { return _itemGrade; }


private:
	void SetItemTextColor(ItemGrade InGrade, UTextBlock& InTextBlock);
	void InvalidateCurrentInputCount();


public:
	UFUNCTION()
	void OnClickSelect();
	UFUNCTION()
	void OnRefreshIconWrapBox(int32 InIndex, class UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnCreateIconWrapBox(class UGsUIIconBase* InIcon);

	void OnLongPressIcon(class UGsUIIconItem& InIcon);

};
