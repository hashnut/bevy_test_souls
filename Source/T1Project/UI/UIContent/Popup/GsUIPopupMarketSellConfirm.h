// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupMarketSellConfirm.generated.h"

class UTextBlock;
class UPanelWidget;
class UWidgetSwitcher;
class UGsButton;
class UGsUIItemDefaultInfo;

/**
 * 거래소 판매 등록 확인 팝업
 */
UCLASS()
class UGsUIPopupMarketSellConfirm : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIItemDefaultInfo* _defaultInfoUI;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPrice;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelPricePerUnit;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPricePerUnit;	

	// 0: 일반(골드) / 1: 월드(젠)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherRegistrationFeeType;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRegistrationFee;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

protected:
	// T: Ok, F: Cancel
	TFunction<void(bool, ItemDBId)> _callbackOkCancel;
	bool _bIsOk = false;
	ItemDBId _itemDBId = INVALID_ITEM_DB_ID;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual bool OnBack() override;
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

protected:
	UFUNCTION()
	void OnClickOk();
	UFUNCTION()
	void OnClickCancel();

public:
	void SetData(bool bIsWorldMarket, ItemDBId InItemDBId, int64 InAmount, int32 InPrice, int32 InRegistrationFee);
	void SetCallback(TFunction<void(bool, ItemDBId)> InCallback);
};
