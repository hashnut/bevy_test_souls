// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIItemSetItemListEntry.generated.h"

class UTextBlock;
class UWidgetSwitcher;

/**
 * 세트 아이템 정보
 */
UCLASS()
class T1PROJECT_API UGsUIItemSetItemListEntry: public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNameOn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNameOff;

public:
	void SetTextName(const FText& InText);
	void SetOnOff(bool bIsOn);
};
