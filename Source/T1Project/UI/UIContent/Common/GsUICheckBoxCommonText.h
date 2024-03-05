// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/GsUICheckBoxCommon.h"
#include "GsUICheckBoxCommonText.generated.h"

class UTextBlock;

/**
 * WBP_CheckBoxCommon 체크박스에 이름 붙어있는 형태. WBP_CheckBoxCommonText
 */
UCLASS()
class T1PROJECT_API UGsUICheckBoxCommonText : public UGsUICheckBoxCommon
{
	GENERATED_BODY()

protected:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNameOff;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNameOn;
	
public:
	void SetText(const FText& InText);
};
