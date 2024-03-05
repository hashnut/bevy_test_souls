// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIReuseItemIconSlotBase.generated.h"

class FGsItem;

class UTextBlock;
class UGsButton;
class UGsUIIconBase;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;
/**
 * 
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIReuseItemIconSlotBase : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsWrapBoxIconSelector* _slotIconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _iconSlotHelper;

protected:
	uint64 _itemDBID = 0;
	int32 _itemTID = 0;
	int64 _itemAmount = 0;
	uint8 _slotIndex = 0;
	uint8 _groupIndex = 0;

protected:
	virtual void NativeOnInitialized() override;
	//virtual void NativeConstruct() override;
	//virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;

public:
	virtual void OnShow() override;
	virtual void OnHide() override;

protected:
	virtual void OnClickIconSlot(class UGsUIIconItem& InIcon);

private:
	void OnLongPressIcon(class UGsUIIconItem& InIcon);

private:
	void ResetInfo();

public:
	virtual void SetSlotItemIcon(const uint64 InItemDBID, const int32 InItemTID, const int64 InAmount, const uint8 InSlotIndex = 0);

public:
	void Init();

public:
	void SetSlotIndex(uint8 InSlotIndex) { _slotIndex = InSlotIndex; }

public:
	uint8 GetSlotIndex() { return _slotIndex; }
	int32 GetItemTID() { return _itemTID; }
	const FGsItem* GetIconItemData();

protected:
	UFUNCTION()
	virtual void OnRefreshIconWrapBox(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnCreateIconWrapBox(class UGsUIIconBase* InIcon);
	
};
