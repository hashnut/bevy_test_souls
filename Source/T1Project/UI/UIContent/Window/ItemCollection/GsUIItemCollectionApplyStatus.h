// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "GsUIItemCollectionApplyStatus.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIItemCollectionApplyStatus : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textApplyState;

public:
	void SetData(const FText& InText);
	void SetData(const StatType& InStatType);
};
