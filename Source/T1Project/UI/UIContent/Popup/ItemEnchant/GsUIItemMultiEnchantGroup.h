// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsUIItemMultiEnchantGroup.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnEquipItemSlot);

class FGsItem;
class UGsButton;
class UTextBlock;
class UGsCheckBox;
class UPanelWidget;
class UVerticalBox;
class UWidgetSwitcher;
class UGsUIIconBase;
class UGsUITooltipDesc;
class UGsUICurrencyButton;
class UGsHorizontalBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UGsDynamicPanelSlotHelper;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIItemMultiEnchantGroup : public UUserWidget
{
	GENERATED_BODY()

private:
	const uint8 EXPECT_ENCHANT_NUM = 10;
	const uint8 EQUIPITEM_SLOT_NUM = 21;
	const uint8 EFFECT_SPEED = 2.f;

private:
	enum EnchantBtnType : uint8
	{
		BtnEnchantType = 0,
		BtnStopEnchantType,
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _ingredientItemIconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _ingredientIconSlotHelper;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	float _waitAnimSpeedValue = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	float _resultAnimSpeedValue = 1.f;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _expectEnchantCountText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _needIngredientCountText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDetailInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsCheckBox* _inputCheckbox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _enchantGuide;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _btnSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnStopEnchant;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnCurrency;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _enchantExpectCountListRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _equipItemListRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltipDescWidget;

protected:
	// expectEnchantCount panel
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _expectCountListEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _expectCountListPanelSlotHelper;

	// expectEnchantCount panel
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _equipItemListEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _equipItemListPanelSlotHelper;

	/*
protected:
	// Effect Anim Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	float _enchantAnimSpeedValue = 1.5f;
	*/


private:
	bool _isAutoEnchant = false;
	bool _isSendingPacket = false;
	ItemDBId _reserveRemoveItemDbid = 0;
	int32 _selectedExpectEnchantCount = -1;
	TArray<ItemDBId> _targetItemDbidList;
	TArray<bool> _expectEnchantStateList;
	TMap<ItemDBId, TPair<ItemId,ItemAmount>> _ingredientDataMap;

private:
	TArray<ItemDBId> _packetEquipItemDbidList;
	TArray<ItemDBIdAmountPair> _packetIngredientItemDbidList;
	Level _packetTargetLevel = 0;
	FTimerHandle _equipSlotWaitEffectTimer;


private:
	TMap<ItemDBId, TPair<int32,bool>> _equipSlotEffectInfoMap;

public:
	FOnEquipItemSlot OnEquipItemSlot;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;

private:
	void InvalidateAll();
	bool IsEnchantValidState();
	bool IsValidQuickEnchantNum();

private:
	void OnEndWaitingEffect();
	void ContinueEnchant();
	void OnFinishedEquipSlotEffect(const int32 InSlotIndex, const ItemDBId InItemDbid);

private:
	void OnClickExpectNum(const uint8 InIndex, const bool InState);
	void OnClickEquipItemSlot(const int32 InIndex, const ItemDBId InItemDbid);
	void OnConfirmResetBtn();
	void OnConfirmEnchant();

private:
	void UpdateExpectEnchantCount();
	void UpdateNeedIngredientCount();
	void UpdateCurrencyBtnInfo();
	void UpdateGuideText();
	void UpdateEquipItemList();

private:
	void AddAllIngredientItems(TWeakPtr<FGsItem> InInvenItem);
	void AddAllIngredientItems();
	void AddSameEquipItems();

public:
	void SetMultiEnchantInfo(const ItemDBId InTargetItemDbid, const ItemDBId InIngredientItemDBId);
	void OnClickInventory(TWeakPtr<FGsItem> InInvenItemData);
	void UpdateMultiEnchantInfo(const ItemDBId InItemDbid);
	void RemoveItemInfo(const ItemDBId InRemoveItemDbid);
	void InvalidateCurrencyInfo();
	void OnClickReset();
	void OnClickRegisterSameItem();
	void UpdateMultiEnchantResultInfo(TArray<ItemEnchantResultData> InEnchantResultList, TArray<ItemDBId> InUseIngredientItemDbisList);
	bool IsUpdateInfo(const ItemDBId InItemDbid);
	 

private:
	const bool IsRegisterCursed();
	const FGsItem* GetRegisterIngredientItemData();
	Currency GetNeedEnchantCost();


public:
	void CloseMultiEnchant();

public:
	const ItemAmount GetSelectedIngredientTotalAmount();
	const ItemId GetSelectedIngredientTID();
	TArray<ItemDBId> GetTargetItemDbidList() { return _targetItemDbidList;}
	TArray<ItemDBId> GetIngredientItemDbidList();
	const uint8 GetEquipItemSlotMaxCount() { return EQUIPITEM_SLOT_NUM; }
	const bool IsAutoMultiEnchant() { return _isAutoEnchant; }


protected:
	UFUNCTION()
	void OnclickDetailInfo();
	UFUNCTION()
	void OnclickEnchant();
	UFUNCTION()
	void OnclickStopEnchant();
	UFUNCTION()
	void OnChangeCheckbox(int32 InIndexGroup, bool InIsChecked);
	UFUNCTION()
	void OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void EnchantCountListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void EnchantEquipItemListRefresh(int32 InIndex, UWidget* InEntry);


};
