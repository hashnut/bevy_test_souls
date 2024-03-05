// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIItemDescListEntry.generated.h"

class UTextBlock;
/**
 * 아이템 설명글 리스트 출력 - 스킬북 설명에 사용함
 */
UCLASS()
class T1PROJECT_API UGsUIItemDescListEntry: public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitle;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDesc;
	
public:
	void SetTextTitle(const FText& InText);
	void SetTextDesc(const FText& InText);
	void SetColorTitle(const FLinearColor& InColor);
};
