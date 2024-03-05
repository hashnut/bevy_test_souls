// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupCommonInput.generated.h"


class UTextBlock;
class UEditableTextBox;
class UGsButton;
/**
 * 텍스트 입력 공용 팝업
 */

UCLASS()
class T1PROJECT_API UGsUIPopupCommonInput : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _titleText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDesc;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockWarning;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _editableTextBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

protected:
	TFunction<void(const FString&)> _callbackFunc;

	int32 _maxTextLength; // 최대 바이트 길이
	bool _bLengthCheckNameType = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	void SetTitleText(const FText& InTitleText);
	void SetDescText(const FText& InDescText);
	void SetWarningText(const FText& InWarningText);
	void SetHintText(const FText& InHintText);
	void SetText(const FText& InText);
	void SetCallbackFunc(TFunction<void(const FString&)> InFunc);
	void SetMaxTextLength(int32 InMaxLength, bool bLengthCheckNameType);

	void ProcessCallback();

protected:
	UFUNCTION()
	void OnClickOk();
	UFUNCTION()
	void OnClickCancel();
	UFUNCTION()
	void OnTextChanged(const FText& InText);
};
