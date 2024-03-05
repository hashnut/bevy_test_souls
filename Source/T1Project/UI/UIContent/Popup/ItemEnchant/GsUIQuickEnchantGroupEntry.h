// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "GsUIQuickEnchantGroupEntry.generated.h"


class UGsButton;
class UWidgetSwitcher;
class UGsUIQuickEnchantGroup;
class UGsUIQuickEnchantEffectGroup;

// OnClick Entry Slot Event - slotIndex
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectQuickEnchantDelegate, uint8);
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIQuickEnchantGroupEntry : public UUserWidget, public IGsEntryInterface
{

	GENERATED_BODY()

public:
	enum SlotStateType : uint8
	{
		State_Normal = 0,
		State_Selection ,
		State_Success,

		State_Max,
	};

protected:
	// 0 : normal  1 : selected  2 : enchantSuccess
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _widgetSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIQuickEnchantGroup* _quickEnchantNormal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIQuickEnchantGroup* _quickEnchantChecked;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIQuickEnchantEffectGroup* _quickEnchantEffect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSelect;

public:
	FOnSelectQuickEnchantDelegate OnSelectQuickEnchantDelegate;


private:
	uint8 _enchantNum = 0;
	bool _isSelected = false;
	SlotStateType _eSlotStateType = SlotStateType::State_Normal;
	

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	virtual void OnShow() override;
	virtual void OnHide() override;

private:
	UGsUIQuickEnchantGroup* GetCurrentActiveGroup();

public:
	void SetData(const uint8 InEnchantNum, const SlotStateType InSlotStateType, const EGsUIColorType InUIColorType);
	

public:
	void SetSelected(const bool InSelected) { _isSelected = InSelected; }

public:
	bool IsSelected() { return _isSelected; }
	uint8 GetEnchantNum() { return _enchantNum; }


public:
	UFUNCTION()
	void OnClickSelect();

	
};
