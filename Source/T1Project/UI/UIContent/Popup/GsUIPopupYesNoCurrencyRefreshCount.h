// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsUIPopupYesNoCurrencyRefreshCount.generated.h"

class UTextBlock;
class URichTextBlock;
class UGsUICurrencyRefreshCountButton;
class UGsButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupYesNoCurrencyRefreshCount : public UGsUIPopup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	URichTextBlock* _richTextContent;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyRefreshCountButton* _btnYes;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNo;

	// T: Yes, F: No
	TFunction<void(bool)> _buttonCallback;

protected:
	CurrencyType _currencyType = CurrencyType::NONE;
	Currency _amount = 0;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	void SetData(const FText& InContent, int32 InRemainCount, CurrencyType InType, Currency InAmount,
		TFunction<void(bool)> InCallback = nullptr);

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickYes();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickNo();
};
