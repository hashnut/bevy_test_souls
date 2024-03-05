// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIOneTextBoxListEntry.generated.h"

class UTextBlock;

/**
 * 텍스트 하나 출력하는 ListEntry
 */
UCLASS()
class T1PROJECT_API UGsUIOneTextBoxListEntry : public UUserWidget, 
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDesc;

public:
	void SetData(const FText& InText);
};
