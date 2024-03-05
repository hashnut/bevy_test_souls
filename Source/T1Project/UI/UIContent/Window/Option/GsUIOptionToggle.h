// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsSchemaEnums.h"
#include "Shared/Client/SharedEnums/SharedGsOptionItemEnum.h"
#include "GsUIOptionToggle.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIOptionToggle : public UUserWidget
{
	GENERATED_BODY()
	
public:
	DECLARE_DELEGATE_TwoParams(FOnClickSlot, EGsOptionItem /*InType*/, bool /*InIsSelected*/);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnNormal;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSelected;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textOn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textOff;

	UPROPERTY()
	bool _bIsSelected = false;

	EGsOptionItem _optionType;

public:
	FOnClickSlot OnClickSlot;

public:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

public:
	void SetData(EGsOptionItem InType, bool InSelected = false, bool InIsEnabled = true);
	void SetButtonText(const FText& InOnText, const FText& InOffText);

protected:
	UFUNCTION()
	void OnClickNormal();

	UFUNCTION()
	void OnClickSelected();

public:
	UFUNCTION()
	void SetSelected(bool bInSelected, bool InIsDisable = false);

	UFUNCTION()
	bool GetIsSelected() const;
};
