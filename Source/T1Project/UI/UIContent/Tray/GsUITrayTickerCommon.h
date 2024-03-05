// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Tray/GsUITrayTickerAnimation.h"
#include "GsUITrayTickerCommon.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITrayTickerCommon : public UGsUITrayTickerAnimation
{
	GENERATED_BODY()	

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textDesc;

	FText _originalTitle;
	FText _originalDesc;

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void SetData(TFunction<void()> InCallback = nullptr) override;
	virtual void SetData(const FText& InMainText, const FText& InSubText, TFunction<void()> InCallback = nullptr);
};
