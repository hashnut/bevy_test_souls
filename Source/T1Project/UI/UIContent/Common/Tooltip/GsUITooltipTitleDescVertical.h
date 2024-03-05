// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsUITooltipDesc.h"
#include "GsUITooltipTitleDescVertical.generated.h"

class UTextBlock;

/**
 * 제목과 내용을 출력하는 툴팁
 */
UCLASS()
class T1PROJECT_API UGsUITooltipTitleDescVertical : public UGsUITooltipDesc
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDescSecond;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUITooltip")
	FLinearColor _colorDescSecond;

public:
	virtual void SetDefaultColor();

	void SetDescText(const FText& InFirstText, const FText& InSecondText);
	void SetDescColor(const FLinearColor& InFirstColor, const FLinearColor& InSecondColor);
};