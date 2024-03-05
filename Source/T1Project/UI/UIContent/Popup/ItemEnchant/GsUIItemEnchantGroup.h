// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Item/Data/GsItemStatData.h"
#include "Management/ScopeGame/GsItemEnchantManager.h"
#include "GsUIItemEnchantGroup.generated.h"


class FGsItem;
class FGsItemEquipment;
class FGsItemIngredient;

class UImage;
class UGsButton;
class UTextBlock;
class UPanelWidget;
class UVerticalBox;
class UWidgetSwitcher;
class UGsUIIconBase;
class UGsUICurrencyButton;
class UGsUIChangeStatGroup;
class UGsUIQuickEnchantGroup;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UGsDynamicPanelSlotHelper;


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnClickMainIconSlot, const bool, const ItemDBId);

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIItemEnchantGroup : public UUserWidget
{
	GENERATED_BODY()

public:
	enum EnchantBtnType : uint8
	{
		BtnEnchantType = 0,
		BtnStopEnchantType,
	};
	enum DisableEnchantReasoneType : uint8
	{
		NoneType = 0,
		ItemLockType,
		ItemMaxLevelType,
		CantUseCursedType,
	};
	enum QuickEnchantSelectType : uint8
	{
		None = 0,	// 선택무
		LowSafe,	// 안전강화 수치 -1
		Safe,		// 안전강화
		Risk,		// 안전강화 수치 +1(파괴가능)
	};

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
	UPanelWidget* _defaultStatRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _defaultStatListRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _enchantPropStatRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _enchantPropStatListRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _enchantSkillRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _enchantSkillListRoot;

protected:
	// default stat
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _statListEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _statListPanelSlotHelper;

	// 특성
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _enchantPropTitleEntryWidget;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _enchantPropTitlePanelSlotHelper;

	// enchant Prop stat
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _enchantPropStatListEntryWidget;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _enchantPropStatListPanelSlotHelper;

	// enchant skill stat
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _enchantSkillListEntryWidget;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _enchantSkillListPanelSlotHelper;

	// expect rate Info List
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _expectRateListEntryWidget;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _expectRateListPanelSlotHelper;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTargetItem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMaterialItem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _itemInfoAreaPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _enchantShieldArea;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _enchantShieldCountText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _guideText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _contentsSwitcher; // 0 : display stat 1 : guideText
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _enchantGuideText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _confirmBtnSwitcher;  // 0 : currency  1 : stopEnchant
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnCurrency;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnStopEnchant;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _enchantDisplayStatSwitcher; // 0 : display stat  1 : hide stat

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRateInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _currentLevelText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _expectLevelText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _quickEnchantGroupRoot;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _quickEnchantBtn1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _quickEnchantBtnSwitcher1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIQuickEnchantGroup* _quickEnchantGroupCheckText1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIQuickEnchantGroup* _quickEnchantGroupNormalText1;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _quickEnchantBtn2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _quickEnchantBtnSwitcher2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIQuickEnchantGroup* _quickEnchantGroupCheckText2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIQuickEnchantGroup* _quickEnchantGroupNormalText2;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _quickEnchantBtn3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _quickEnchantBtnSwitcher3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIQuickEnchantGroup* _quickEnchantGroupCheckText3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIQuickEnchantGroup* _quickEnchantGroupNormalText3;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRateToolTip;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnToolTipClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _toolTipItemNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _expectRateListRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _toolTipGuideText;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _mainSlotsafeEnchantEffect;

protected:
	// SafeEnchant Effect Anim Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	float _safeEnchantAnimSpeedValue = 1.f;

private:
	bool _isAutoEnchant = false;
	int32 _expectEnchantLevel = 0;
	ItemDBId _targetItemDbid = 0;
	ItemDBId _ingredientItemDbid = 0;	
	TArray<FGsItemEnchantManager::DisplayDefaultStatData> _displayDefaultStatDataList;
	TArray<FGsItemEnchantManager::DisplayPropertyStatData> _displayPropertyStatDataList;
	TArray<FGsItemEnchantManager::DisplayRateInfo> _expectRateInfoList;

	// 각 레벨별로 가질 수 있는 모든 PassivitySet 에 대한 설명 텍스트
	TOptional<TMap<Level, TArray<FText>>> _displaySkillDataMap;
	TMap<QuickEnchantSelectType, uint8> _quickEnchantValueMap;
	TArray<TPair<FText, FLinearColor>> _propTitleInfoList;

public:
	FOnClickMainIconSlot OnClickMainIconSlot;


protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;

private:
	void InvalidateAll();
	void InvalidateByQuickEnchant();
	void InvalidateBtnState(const EnchantBtnType InEnchantBtnType);
	void InvalidateRateInfo(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData);
	void SetEquipItemInfo(TWeakPtr<FGsItem> InEquipData);
	void SetExpectInfo(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData, FGsItemEnchantManager* InEnchantMgr);
	void SetEnchantGuideText(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData);
	void SetEnchantBtnInfo();
	void SetQuickEnchantInfo(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData);
	void SetSelectQuinckEnchant(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData);
	void SetDefaultStatInfo(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData, FGsItemEnchantManager* InEnchantMgr);
	void SetEnchantPropertyStatInfo(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData, FGsItemEnchantManager* InEnchantMgr);
	void SetEnchantSkillInfo(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData, FGsItemEnchantManager* InEnchantMgr);

private:
	void OnConfirmEnchant();
	void AutoEnchantProc();

private:
	const DisableEnchantReasoneType CheckEnchantStateType(TWeakPtr<FGsItem> InEquipData, TWeakPtr<FGsItem> InIngredientData);

private:
	void BindSafeEnchantEffectAnimFinishedEvent();

private:
	QuickEnchantSelectType GetCurrenctQuickSelectType();
	Currency GetNeedEnchantCost();
	ItemDBId GetFirstIngredient(ItemDBId InTargetItemDBId);

public:
	void UpdateEnchantResultInfo(const ItemDBId IntargetItemDbid, const ItemDBId InIngredientItemDbid, const ItemEnchantResult InResultType);
	void RemoveItemInfo(const ItemDBId InRemoveItemDbid);
	bool UpdateItemInfo(const ItemDBId InTargetItemDbid);
	void InvalidateCurrencyInfo();
	void ResetQuickEnchantButton();

public:
	void CloseEnchant();

public:
	void SetEnchantInfo(const ItemDBId InTargetItemDbid, const EGsItemEnchantActionType InEnchantActionType);
	void SetEnchantItemDbid(const ItemDBId InTargetItemDbid);
	void SetInputItemDbid(const ItemDBId InTargetItemDbid, const ItemDBId InIngredientItemDbid);
	void SetTargetItemDbid(const ItemDBId InItemDbid) { _targetItemDbid = InItemDbid; }

public:
	ItemDBId GetTargetItemDbid() { return _targetItemDbid; }
	ItemDBId GetIngrdientItemDbid() { return _ingredientItemDbid; }
	bool IsPlayingAnim() { return _mainSlotsafeEnchantEffect->IsPlayingAnimation(); }
	bool IsAutoEnchant() { return _isAutoEnchant; }

private:
	UFUNCTION()
	void OnclickEnchant();
	UFUNCTION()
	void OnclickRateInfo();
	UFUNCTION()
	void OnclickCloseRateInfo();
	UFUNCTION()
	void OnclickStopEnchant();
	UFUNCTION()
	void OnclickQuickEnchant1();
	UFUNCTION()
	void OnclickQuickEnchant2();
	UFUNCTION()
	void OnclickQuickEnchant3();
	UFUNCTION()
	void OnclickTargetItemSlot();
	UFUNCTION()
	void OnClickMaterialItemSlot();
	UFUNCTION()
	void OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnRefreshMaterialIcon(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void DefaultStatListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void EnchantPropListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void EnchantSkillListRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void EnchantPropTitleRefresh(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void ExpectRateListRefresh(int32 InIndex, UWidget* InEntry);
};
