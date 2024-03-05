// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIMultiLevelRankIcon.generated.h"


class UImage;
class UPanelWidget;
/**
 * ½Â±Þ ¾ÆÀÌÄÜ
 */
UCLASS()
class T1PROJECT_API UGsUIMultiLevelRankIcon : public UUserWidget
{
	GENERATED_BODY()

protected:
	const int32 MAX_SUB_ICON_COUNT = 5;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageMainIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSubIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageSubIcon1;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageSubIcon2;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageSubIcon3;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageSubIcon4;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageSubIcon5;

protected:
	UPROPERTY()
	TArray<UImage*> _subIconList;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetMainImage(const FSoftObjectPath& InImagePath);
	void SetSubImage(const FSoftObjectPath& InImagePath);
	void SetSubImageCount(int32 InCount);

};
