// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "GsGrayscaleImage.generated.h"

class UPaperSprite;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsGrayscaleImage : public UImage
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UGsGrayscaleImage")
	UPaperSprite* _sprite;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UGsGrayscaleImage")
	float _grayDensity = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UGsGrayscaleImage")
	float _grayDarkness = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UGsGrayscaleImage")
	float _grayOpacity = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UGsGrayscaleImage")
	bool _bIsGray = false;

public:
	virtual void SynchronizeProperties() override;

public:
	UFUNCTION(BlueprintCallable, Category = "UGsGrayscaleImage")
	void SetPaperSprite(UPaperSprite* InSprite);

	UFUNCTION(BlueprintCallable, Category = "UGsGrayscaleImage")
	void SetGrayscale(bool bInIsGray);

	UFUNCTION(BlueprintCallable, Category = "UGsGrayscaleImage")
	bool IsGrayscale() const { return _bIsGray; }

	UFUNCTION(BlueprintCallable, Category = "UGsGrayscaleImage")
	void SetDissolveIntensity(float InValue);
};
