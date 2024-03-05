// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "GsUIPopupPKBookTryToActionBase.generated.h"

class UGsButton;
class URichTextBlock;
class UGsUICurrencySlot;

UCLASS()
class T1PROJECT_API UGsUIPopupPKBookTryToActionBase : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencySlot* _currencySlotPay;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencySlot* _currencySlotHave;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOK;

	PKRecordId _recordId = INVALID_PK_RECORD_ID;
	CurrencyType _currencyType = CurrencyType::NONE;
	Currency _amountCost = 0;

protected:
	virtual void NativeOnInitialized() override;
public:
	virtual void BeginDestroy() override;

protected:
	virtual void TryToAction(PKRecordId InRecordId);

protected:
	UFUNCTION()
	void OnClickedClose();

	UFUNCTION()
	void OnClickedOK();

public:
	void SetData(PKRecordId InRecordId, const struct FGsPKRecordingContributorData* InData);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
