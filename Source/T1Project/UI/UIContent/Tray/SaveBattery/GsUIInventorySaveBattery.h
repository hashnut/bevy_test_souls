// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIInventorySaveBattery.generated.h"

class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UGsUIIconBase;
/**
 * 절전 모드 획득 아이템
 */
UCLASS()
class T1PROJECT_API UGsUIInventorySaveBattery : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsWrapBoxIconSelector* _iconSelector;

	UPROPERTY()
	UGsDynamicIconSlotHelper* _iconSlotHelper;

	/************************************************************************/
	/* overrides 및 갱신                                                     */
	/************************************************************************/
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void BeginDestroy() override;

public:
	void InvalidateAll();

	/************************************************************************/
	/* 획득 아이템                                                           */
	/************************************************************************/
public:
	void AddItem(const uint64 InItemDBID);
	void UpdateItem(const uint64 InItemDBID);
	void RemoveItem(const uint64 InItemDBID, const int32 InItemTID);

	/************************************************************************/
	/* 아이콘                                                                */
	/************************************************************************/
public:
	void RefreshIconByIndex(int32 InIndex);	

protected:
	UFUNCTION()
	void OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon);
};

