// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Shared/Client/SharedEnums/SharedGsOptionItemEnum.h"
#include "GsUIOptionButton.generated.h"

class UGsButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIOptionButton : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClickSlot, EGsOptionItem);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btn;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textNameTag;

	EGsOptionItem _optionType;

public:
	FOnClickSlot OnClickSlot;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

public:
	void SetData(EGsOptionItem InType, const FText& InNameTag);

protected:
	UFUNCTION()
	void OnClick();
};
