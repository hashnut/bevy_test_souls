// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "GsUIPopupWatchMovieReward.generated.h"

class UGsButton;
class UGsUICurrencySlot;
class UTextBlock;
class UWidgetSwitcher;
class UGsCheckBox;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupWatchMovieReward : public UGsUIPopup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _expTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencySlot* _currencySlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherRewardType;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsCheckBox* _checkBoxEditable;

	// T: Ok, F: Cancel
	TFunction<void(bool)> _buttonCallback;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	virtual int32 GetManagedDefaultZOrder() const override { return UI_DEPTH_SYSTEM_POPUP; }

	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_SYSTEM_POPUP; }

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

protected:
	UFUNCTION()
	void OnClickOk();
	UFUNCTION()
	void OnClickCancel();
	UFUNCTION()
	void OnClickClose();
	UFUNCTION()
	void OnChangeCheckBox(int32 IndexInGroup, bool bIsChecked);
public:
	void SetData(int32 inSequenceResId, TFunction<void(bool)> InCallback);
};
