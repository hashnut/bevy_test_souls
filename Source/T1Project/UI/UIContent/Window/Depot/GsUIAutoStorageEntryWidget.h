// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIAutoStorageEntryWidget.generated.h"


class UTextBlock;
class UGsButton;
class UImage;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectAutoEntrySlot, UGsUIAutoStorageEntryWidget*);
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIAutoStorageEntryWidget : public UUserWidget, public IGsEntryInterface
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
	UImage* _selectedImg;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSelect;
	

public:
	FOnSelectAutoEntrySlot OnSelectAutoEntrySlot;

private:
	int32 _displayItemTID = 0;
	bool _isSelected = false;


public:
	void SetDisPlayItem(const int32 InItemTID , const bool InIsSelected);

public:
	bool IsSelected() { return _isSelected; }
	int32 GetDisPlayItemTID() { return _displayItemTID; }

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	virtual void OnShow() override;
	virtual void OnHide() override;

public:
	void OnClickIconSlot(class UGsUIIconItem& InIcon);

public:
	UFUNCTION()
	void OnClickSlot();
	UFUNCTION()
	void OnRefreshIconWrapBox(int32 InIndex, class UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnCreateIconWrapBox(class UGsUIIconBase* InIcon);

};
