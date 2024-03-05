// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupOK.generated.h"

class URichTextBlock;
class UTextBlock;
/**
 * 버튼이 하나 있는 팝업. Msg 1개
 */
UCLASS()
class T1PROJECT_API UGsUIPopupOK : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	URichTextBlock* _contentRichText;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _buttonText;

	TFunction<void()> _okCallback;

protected:
	virtual void NativeDestruct() override;

protected:
	virtual bool OnBack() override;
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	virtual	void OnClickOk();

public:
	void SetData(const FText& InContent, const FText& InButton, TFunction<void()> InOKCallback = nullptr);
	void SetData(const FText& InContent, TFunction<void()> InOKCallback = nullptr);
};
