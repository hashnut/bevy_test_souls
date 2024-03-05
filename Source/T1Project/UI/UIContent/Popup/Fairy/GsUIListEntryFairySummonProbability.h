// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "GsUIListEntryFairySummonProbability.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIListEntryFairySummonProbability : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _fairyNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FSlateColor _fairyNameColor;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _probabilityText;

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
