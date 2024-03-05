// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GsUINpcShopAutoSellListEntry.generated.h"

class UTextBlock;
class UGsButton;
class UImage;
class UWidgetSwitcher;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUINpcShopAutoSellListEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsWrapBoxIconSelector* _iconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _iconSlotHelper;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSelect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _iconSwitcher; // 0 ~ 2 : armor n,m,r / 3 ~ 5 : weapon n,m,r
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _selectedImage;

private:
	int32 _displayItemTID = 0;
	bool _isSelected = false;
	bool _isEquipmentSlot = false;
	ItemCategoryMain _itemCategoryMain = ItemCategoryMain::NONE;
	ItemGrade _itemGrade = ItemGrade::NONE;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;

public:
	virtual void OnShow() override;

public:
	void SetDisplayItem(const int32 InItemTID, const bool InIsSelected);
	void SetDisplayItem(const NpcShopAutoSellEquipmentType InType, const bool InIsSelected);
	void SetSlotSelect(bool InState);

public:
	ItemCategoryMain GetSlotItemCategory();
	ItemGrade GetSlotItemGrade();

public:
	bool IsEquipmentSlot() { return _isEquipmentSlot; }
	bool IsSelectedSlot() { return _isSelected; }
	int32 GetDisplayItemTID() { return _displayItemTID; }

private:
	void OnClickIconSlot(class UGsUIIconItem& InIcon);


public:
	UFUNCTION()
	void OnClickSelect();
	UFUNCTION()
	void OnRefreshIconWrapBox(int32 InIndex, class UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnCreateIconWrapBox(class UGsUIIconBase* InIcon);
	
};
