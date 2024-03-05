// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupCommonPayIncrease.generated.h"

class UTextBlock;
class UGsButton;
class UGsUICurrencySlot;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupCommonPayIncrease : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _contentText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMin;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPlus;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencySlot* _currencyCommonPay;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencySlot* _currencyLocalPay;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _amountText;

	TFunction<void(bool, bool, Level)> _buttonCallback;

	// destruct 호출 시점에 
	// 이벤트 종료 관련 호출처리(EGsEventProgressType::UI_POPUP_YES_NO_CHNNEL_MOVE)
	// T, F: _isCallEventProcessFinish
	TFunction<void(bool)> _destructCallback;

	// 이벤트 종료 호출 할지
	bool _isCallEventProcessFinish = true;

	Level _pastLevel{ 0 };
	Level _curLevel{ 0 };
	Level _minLevel{ 0 };
	Level _maxLevel{ 0 };

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	virtual	void OnClickYes();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	virtual	void OnClickNo();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	virtual void OnClickMinus();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	virtual	void OnClickPlus();
	
	virtual void NativeDestruct() override;
	virtual void NativeOnInitialized() override;

	virtual void OnInputCancel();

public:
	virtual void RefreshUI() {}
};
