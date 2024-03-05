// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Item/Data/GsItemStatData.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIItemRefineGroup.generated.h"

class FGsItem;
class FGsItemStat;

class UGsButton;
class UTextBlock;
class UPanelWidget;
class UVerticalBox;
class UWidgetSwitcher;
class UGsUIIconBase;
class UGsUICurrencyButton;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UGsDynamicPanelSlotHelper;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnClickRefineMainIconSlot, const bool, const ItemDBId);

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIItemRefineGroup : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsWrapBoxIconSelector* _targetItemIconSelector;
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsWrapBoxIconSelector* _materialItemIconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _targetIconSlotHelper;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _materialIconSlotHelper;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _refineStatRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _refineOptionTextAreaRootPanel;

protected:
	// refine stat
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _statListEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _statListPanelSlotHelper;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTargetItem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMaterialItem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _itemInfoAreaPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _guideText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _contentsSwitcher; // 0 : display stat 1 : guideText
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnCurrency;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRefineOptionInfo;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _mainSlotsafeEnchantEffect;


protected:
	// Effect Anim Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	float _refineOptionAnimSpeedValue = 1.f;

private:
	bool _isRefine = false;
	ItemDBId _targetItemDbid = 0;
	ItemDBId _ingredientItemDbid = 0;
	TMap<uint8, TPair<TPair<StatType,int32>, const FLinearColor>> _refineStatDisplayDataMap;

public:
	FOnClickRefineMainIconSlot OnClickRefineMainIconSlot;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;

private:
	bool UpdateRefineOptionList(TWeakPtr<FGsItem> InTargetItem);


private:
	void CloseExpectOptionPopup();
	void BindSafeEnchantEffectAnimFinishedEvent();

public:
	void UpdateRefineOptionResultInfo();
	bool UpdateItemInfo(const ItemDBId InTargetItemDbid);
	void RemoveItemInfo(const ItemDBId InRemoveItemDbid);

public:
	void InvalidateAll();
	void OnClickInventory(TWeakPtr<FGsItem> InItem);

public:
	void InvalidateCurrencyInfo();
	void CloseRefineOption();

public:
	void SetRefineInfo(const ItemDBId InTargetItemDbid);
	void SetInputItemDbid(const ItemDBId InTargetItemDbid, const ItemDBId InIngredientItemDbid);
	void SetTargetItemDbid(const ItemDBId InTagetItemDbid) { _targetItemDbid = InTagetItemDbid; }


public:
	bool IsPlayingAnim() { return _mainSlotsafeEnchantEffect->IsPlayingAnimation(); }
	ItemDBId GetTargetItemDbid() { return _targetItemDbid; }
	ItemDBId GetIngrdientItemDbid() { return _ingredientItemDbid; }

private:
	UFUNCTION()
	void OnclickRefine();
	UFUNCTION()
	void OnclickRefineOptionList();
	UFUNCTION()
	void OnclickTargetItemSlot();
	UFUNCTION()
	void OnClickMaterialItemSlot();
	UFUNCTION()
	void OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnRefreshIngredientIcon(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void RefineStatListRefresh(int32 InIndex, UWidget* InEntry);

};
