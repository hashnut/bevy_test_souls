// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupCommonChangeNumber.generated.h"

class UGsButton;
class UTextBlock;

/**
 * 길드 공지사항/소개글 변경창
 */
UCLASS()
class T1PROJECT_API UGsUIPopupCommonChangeNumber : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class URichTextBlock* _richTextMessage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UEditableTextBox* _inputBoxMsg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

protected:
	int32 _minNumber = 0;
	int32 _maxNumber = 0;
	TFunction<void(int32)> _callbackFunc;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	void SetData(int32 InMinNumber, int32 InMaxNumber, TFunction<void(int32)> InFunc);

	void SetTitleText(const FText& InText);
	void SetMessageText(const FText& InText);	
	
protected:
	UFUNCTION()
	void OnClickOk();

	UFUNCTION()
	void OnClickCancel();

	UFUNCTION()
	void OnTextChangedMsg(const FText& InText);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
