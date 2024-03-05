// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupYesNoTwoMessage.generated.h"

class UTextBlock;
class UGsButton;
class URichTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupYesNoTwoMessage : public UGsUIPopup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	URichTextBlock* _richTextBlockFirstMsg;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	URichTextBlock* _richTextBlockSecondMsg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnYes;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockYesButton;

	// T: Yes, F: No
	TFunction<void(bool)> _buttonCallback;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	void SetData(const FText& InFirstMsg, const FText& InSecondMsg,
		TFunction<void(bool)> InCallback = nullptr);

	void SetData(const FText& InFirstMsg, const FText& InYesButton, const FText& InSecondMsg,
		TFunction<void(bool)> InCallback = nullptr);

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickYes();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickNo();
};
