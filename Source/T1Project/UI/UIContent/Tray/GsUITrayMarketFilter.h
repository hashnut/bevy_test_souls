// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Market/GsMarketFilterData.h"
#include "DataSchema/GsSchemaEnums.h"
#include "GsUITrayMarketFilter.generated.h"

class UTextBlock;
class UPanelWidget;
class UGsButton;
class UScrollBox;
class UGsUICheckBoxCommonText;
class UGsUITwoSideSlider;
class UGsDynamicPanelSlotHelper;
class UWidget;
struct FGsMarketItemData;
struct FGsSchemaMarketFilterSetting;
struct FGsSchemaStatFormatingData;

/**
 * 거래소 검색 필터
 */
UCLASS()
class UGsUITrayMarketFilter : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITwoSideSlider* _sliderEnchat;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootEnchantNum;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITwoSideSlider* _sliderShield;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootShieldNum;
	
	// 등급
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICheckBoxCommonText* _checkBoxGradeAll;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelGradeListRoot;
	
	// 특성
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICheckBoxCommonText* _checkBoxEnchantPropAll;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEnchantPropListRoot;
	
	// 제련
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICheckBoxCommonText* _checkBoxRefineAll;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRefineListRoot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	
protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperItemGrade;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperEnchantProp;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperRefine;

protected:
	TArray<ItemGrade> _gradeList;
	TArray<EGsItemEnchantBonusType> _enchantPropList;	
	TArray<const FGsSchemaStatFormatingData*> _refineStatList;

	// 특성을 정의한 리스트는 따로 없어보이는 것 같기도. FGsSchemaItemEquipEnchantBonus 이것 같아
	FGsMarketFilterData _data; //<- 이거 복사해다가 쓰고, OK 누를 때 매니저에 Set 하도록 하자?

	bool _bIsWorldMarket = false;
	ItemGrade _worldMinimumGrade = ItemGrade::RARE;

	// 개선: 하드코딩
	const int32 MAX_ENCHANT_LEVEL = 20;
	const int32 MAX_SHIELD_COUNT = 4;
	
public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	// 윈도우처럼 취급. 시네마틱 등에서 꺼지지 않을까봐 변경
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_DEFAULT; }

public:
	void SetData(bool bIsWorldMarket);

protected:
	void InitRefineStatList();
	
protected:
	UFUNCTION()
	void OnCreateEntryItemGrade(UWidget* InEntry);
	UFUNCTION()
	void OnCreateEntryEnchantProp(UWidget* InEntry);
	UFUNCTION()
	void OnCreateEntryRefine(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshEntryItemGrade(int32 InIndex, UWidget* InEntry);
	UFUNCTION()	
	void OnRefreshEntryEnchantProp(int32 InIndex, UWidget* InEntry);	
	UFUNCTION()
	void OnRefreshEntryRefine(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnClickOk();
	UFUNCTION()
	void OnClickCancel();
	
protected:
	void OnSlideEnchant(float InCurrMinValue, float InCurrMaxValue);
	void OnSlideShield(float InCurrMinValue, float InCurrMaxValue);
	
	void OnCheckGradeAll(bool bIsChecked, int32 InIndex);
	void OnCheckGrade(bool bIsChecked, int32 InIndex);
	void OnCheckEnchantPropAll(bool bIsChecked, int32 InIndex);
	void OnCheckEnchantProp(bool bIsChecked, int32 InIndex);
	void OnCheckRefineAll(bool bIsChecked, int32 InIndex);
	void OnCheckRefine(bool bIsChecked, int32 InIndex);

private:
	void RecursiveFindTextBlock(UPanelWidget* InPanel, OUT TArray<UTextBlock*>& OutTextBlock);
};
