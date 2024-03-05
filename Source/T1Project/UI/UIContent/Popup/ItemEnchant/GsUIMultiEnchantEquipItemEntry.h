// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsUIMultiEnchantEquipItemEntry.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnClickEquipItem, const int32, const ItemDBId);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFinishedSlotEffect, const int32, const ItemDBId);

class FGsItem;

class UGsButton;
class UWidgetSwitcher;
class UGsUIIconBase;
class UGsHorizontalBoxIconSelector;
class UGsDynamicIconSlotHelper;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIMultiEnchantEquipItemEntry : public UUserWidget
	, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _itemIconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _IconSlotHelper;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOnClick;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _stateSwitcher; //  0 : empty  1 : expect  2 : result
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _enchantExpectEffectSwitcher;  //  0 :  succFx  1 : failFx
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _resultSwitcher;  // 0 : bigSucc  1 : cursed   2 : fail  3 : preserve  4 : succ  5 : shield

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _enchantProgressEffectRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _normalProgressEffect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _cursedProgressEffect;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _bigSuccessEffect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _cursedEffect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _destroyEffect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _preserveEffect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _successEffect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _shieldEffect;


private:
	bool _isLockItem = false;
	int32 _slotIndex = 0;
	ItemDBId _targetItemDbid = 0;
	int32 _targetLevel = 0;
	bool _isCursed = false;
	ItemId _equipItemTid = 0;
	Level _equipItemLevel = 0;	
	ItemEnchantResult _resultType = ItemEnchantResult::NONE;

public:
	FOnFinishedSlotEffect OnFinishedSlotEffect;
	FOnClickEquipItem OnClickEquipItem;
		

protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;

public:
	virtual void OnShow() override;
	virtual void OnHide() override;

private:
	void BindEnchantProcFinishedEvent();
	void BindEnchantResultFinishedEvent();

private:
	bool IsActiveEnchantProcEffect(TWeakPtr<FGsItem> InItemData, int32 InTargetLv, bool InIsCursed);

public:
	void AllOffHideEffect();

public:
	float SetEnchantWaitProcEffect(const bool InIsCursed, float InAnimSpeed);
	void SetData(const int32 InIndex, const ItemDBId InItemDbid, const int32 InTargetLevel, const bool InIsCursed);
	int32 OnEffect(const ItemEnchantResult InResultType, float InAnimSpeed);
	

public:
	int32 GetSlotIndex() { return _slotIndex;}
	ItemDBId GetItemDbid() { return _targetItemDbid; }

protected:
	UFUNCTION()
	void OnclickEqiupItem();
	UFUNCTION()
	void OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION(BlueprintCallable)
	void OnDissolveEffect();
};

