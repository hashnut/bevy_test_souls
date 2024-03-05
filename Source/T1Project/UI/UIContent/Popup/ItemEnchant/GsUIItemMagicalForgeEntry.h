// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIItemMagicalForgeEntry.generated.h"

class UTextBlock;
class UPanelWidget;
class UGsDynamicPanelSlotHelper;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIItemMagicalForgeEntry : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelCurrent;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelExpect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	TSubclassOf<UUserWidget> _textEntryClassCurrent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	TSubclassOf<UUserWidget> _textEntryClassExpect;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperCurrent;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperExpect;

protected:
	bool _isRefreshColor = false;

	TArray<FString> _currentLines;
	TArray<FString> _expectLines;

	FLinearColor _currentColor;
	FLinearColor _expectColor;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetTitleText(const FText& InText);

	void SetCurrentText(const FText& InText);
	void SetCurrentColor(const FLinearColor& InColor);

	void SetExpectText(const FText& InText);
	void SetExpectColor(const FLinearColor& InColor);

protected:
	UFUNCTION()
	void OnRefreshCurrentEntry(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnRefreshExpectEntry(int32 InIndex, UWidget* InEntry);

};
