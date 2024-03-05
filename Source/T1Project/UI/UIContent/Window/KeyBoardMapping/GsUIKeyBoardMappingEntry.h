// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Option/GsOptionEnumType.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "GsUIKeyBoardMappingEntry.generated.h"

class UTextBlock;
class UInputKeySelector;
class UImage;
class UGsUIVFX;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIKeyBoardMappingEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnKeySelected, EGsKeyMappingType, FInputChord);
	DECLARE_DELEGATE_TwoParams(FOnSelectingKeyChanged, bool, EGsKeyMappingType);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UInputKeySelector* _keySelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _selectKeyName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _selectImg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIVFX* _touchEffect;

	EGsKeyMappingType _keyMapptingType;

public:
	FOnKeySelected OnSelected;
	FOnSelectingKeyChanged OnSelectingKeyChanged;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

public:
	void SetData(EGsKeyMappingType InType, const FInputChord& InInputChord);
	void SetDataEmpty();
	void SetTextKeyName(const FText& InKeyName);

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnKeySelect(FInputChord inInputChord);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnIsSelectingKeyChanged();

public:
	const FInputChord& GetSelectedKey();
};
