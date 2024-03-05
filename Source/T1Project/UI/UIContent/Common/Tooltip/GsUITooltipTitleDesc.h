// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITooltip.h"
#include "GsUITooltipTitleDesc.generated.h"

class UTextBlock;

/**
 * 제목과 내용을 출력하는 툴팁
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUITooltipTitleDesc : public UGsUITooltip
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDesc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUITooltip")
	FLinearColor _colorDefaultTitle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUITooltip")
	FLinearColor _colorDefaultDesc;

protected:
	virtual void NativeConstruct();

public:
	void SetTitleText(const FText& InText);
	void SetDescText(const FText& InText);
	void SetTitleColor(const FLinearColor& InColor);
	void SetDescColor(const FLinearColor& InColor);

	void SetDefaultColor();
};