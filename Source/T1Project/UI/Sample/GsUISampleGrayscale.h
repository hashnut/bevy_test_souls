// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUISampleGrayscale.generated.h"

class UGsGrayscaleImage;

/**
 * grayscaleTest
 */
UCLASS()
class T1PROJECT_API UGsUISampleGrayscale : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "GsUISample")
	FSoftObjectPath _path0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "GsUISample")
	FSoftObjectPath _path1;

	UPROPERTY(BlueprintReadOnly, Category = "GsUISample", meta = (BindWidget))
	UGsGrayscaleImage* _imgTest0;

	UPROPERTY(BlueprintReadOnly, Category = "GsUISample", meta = (BindWidget))
	UGsGrayscaleImage* _imgTest1;

public:
	UFUNCTION(BlueprintCallable, Category = "GsUISample")
	void OnClickSetImage();

	UFUNCTION(BlueprintCallable, Category = "GsUISample")
	void OnClickGrayscale0(bool bInIsGray);

	UFUNCTION(BlueprintCallable, Category = "GsUISample")
	void OnClickGrayscale1(bool bInIsGray);
};
