// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIRefineOptionStatEntry.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIRefineOptionStatEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _statNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _statValueText;

public:
	void SetStatDisplayInfo(const FText& InStatNameText, const FText& InFirstValueText, const FText& InSecondValueText, FString InBonusValueString, const bool InIsBonusStat);
		
};
