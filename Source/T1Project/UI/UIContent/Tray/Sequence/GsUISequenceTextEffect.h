// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUISequenceTextEffect.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUISequenceTextEffect : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	UTextBlock* _text1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	UTextBlock* _text2;

public:
	void PlayEffect(const FText In_Text1, const FText In_Text2);
};
