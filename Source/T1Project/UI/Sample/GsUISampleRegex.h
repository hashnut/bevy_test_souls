// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUISampleRegex.generated.h"

class UMultiLineEditableText;
class UEditableTextBox;
class UTextBlock;

/**
 * 정규식 테스트 편의를 위한 클래스
 */
UCLASS()
class T1PROJECT_API UGsUISampleRegex : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UMultiLineEditableText* _inputTarget;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _inputRegex;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockResult;

public:
	UFUNCTION(BlueprintCallable)
	void OnClickApply();
};
