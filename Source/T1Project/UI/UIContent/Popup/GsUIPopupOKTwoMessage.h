// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupOKTwoMessage.generated.h"

class UTextBlock;
class URichTextBlock;
/**
 * 버튼이 하나 있는 팝업. Msg 2개
 */
UCLASS()
class T1PROJECT_API UGsUIPopupOKTwoMessage : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	URichTextBlock* _richTextBlockFirstMsg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	URichTextBlock* _richTextBlockSecondMsg;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _buttonText;

	TFunction<void()> _okCallback;

protected:
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	virtual	void OnClickOk();

public:
	void SetData(const FText& InFirstMsg, const FText& InSecondMsg,
		TFunction<void()> InCallback = nullptr);
	void SetData(const FText& InFirstMsg, const FText& InSecondMsg, const FText& InButton,
		TFunction<void()> InOKCallback = nullptr);
};
