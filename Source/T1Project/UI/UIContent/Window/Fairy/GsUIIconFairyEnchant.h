// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIIconFairyEnchant.generated.h"

class UImage;
class UGsButton;

DECLARE_MULTICAST_DELEGATE(FOnClickIcon);
DECLARE_MULTICAST_DELEGATE(FOnPressedLong);
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIIconFairyEnchant : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UGsButton* _btnIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _imgBG;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _imgIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textItemName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FLinearColor _colorItemName;


public:
	FOnClickIcon OnClickIcon;
	FOnPressedLong OnPressedLong;

private:
	void OnFinishedLongPress();

protected:
	UFUNCTION()
	virtual void OnClickItemIcon();

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void SetEmptyIcon();
	virtual void SetIconImage(const FSoftObjectPath& InPath);
	virtual void SetItemNameColor(FText nameText, const FLinearColor& InColor);


};
