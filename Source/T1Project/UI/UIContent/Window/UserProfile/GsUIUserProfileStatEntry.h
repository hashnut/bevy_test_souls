// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIUserProfileStatEntry.generated.h"


class UImage;
class UTextBlock;
/**
 * 유저 프로필 스탯 정보 슬롯
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUIUserProfileStatEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageStatIcon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockStatName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockStatValue;

public:
	void SetData(const FSoftObjectPath& InIconPath, const FText& InStatName, const FText& InStatValue);
private:
	void LoadComplete(TSharedRef<struct FStreamableHandle> inRes);
};