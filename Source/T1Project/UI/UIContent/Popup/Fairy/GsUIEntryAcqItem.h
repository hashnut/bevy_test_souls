// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataSchema/Fairy/GsSchemaCeilingItemData.h"
#include "GsUIEntryAcqItem.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIEntryAcqItem : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textItemName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FLinearColor _colorItemName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textItemAmount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FLinearColor _colorItemAmount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textAcqRate;

public:
	void SetData(int index, FGsSchemaCeilingItemData& data, float rateTotal = 1.f); 
};
