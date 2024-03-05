// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Shared/Client/SharedEnums/SharedGsOptionItemEnum.h"
#include "GsUIOptionCheckButton.generated.h"

class UGsSwitcherButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIOptionCheckButton : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnClickCheckBox, EGsOptionItem /*bIsChecked*/, bool /*InIndexInGroup*/);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherButton;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockOn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockOff;

	bool _bIsSelected = false;
	EGsOptionItem _itemType = EGsOptionItem::None;

public:
	FOnClickCheckBox _onClickButton;

public:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

public:
	void SetIsSelected(bool bInIsSelected);
	bool GetIsSelected() const;

public:
	// FIX: 원하는 자료형으로 넣어주자
	void SetData(const FText& InNameTag, bool InIsSelect, bool InIsEnabled = true, bool InisVisible = true);
	void SetData(EGsOptionItem InType, const FText& InNameTag, bool InIsSelect, bool InIsEnabled, bool InisVisible = true);

protected:
	UFUNCTION()
	void OnClickButton();
};
