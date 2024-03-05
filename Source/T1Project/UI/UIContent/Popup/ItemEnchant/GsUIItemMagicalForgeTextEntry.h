// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIItemMagicalForgeTextEntry.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIItemMagicalForgeTextEntry : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockDesc;

public:
	void SetDescText(const FText& InText);
	void SetDescColor(const FLinearColor& InColor);

};
