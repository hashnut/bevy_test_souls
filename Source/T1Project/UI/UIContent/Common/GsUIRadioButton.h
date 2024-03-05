// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIRadioButton.generated.h"


class UGsSwitcherButton;
class IGsToggleGroupEntry;
/**
 * 라디오 버튼
 */
UCLASS()
class T1PROJECT_API UGsUIRadioButton : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioButtonSelected, bool, bIsSelected);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _switcherButton;

public:
	FOnRadioButtonSelected OnRadioButtonSelected;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	void SetIsSelected(bool bInIsSelected);
	bool GetIsSelected() const;

	IGsToggleGroupEntry* GetToggleGroupEntry();

protected:
	UFUNCTION()
	void OnClickButton();
};
