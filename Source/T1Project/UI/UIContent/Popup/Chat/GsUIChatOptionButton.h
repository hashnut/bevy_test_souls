// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIChatOptionButton.generated.h"

class UGsSwitcherButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIChatOptionButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnOption;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _optionName;

	uint64 _keyId{ 0 };
	TFunction<void(uint64, bool)> _toggleCallback;

public:
	UFUNCTION(BlueprintCallable, Category = "GsUIControl")
	virtual void SetIsSelected(bool bInIsSelected) ;

	UFUNCTION(BlueprintCallable, Category = "GsUIControl")
	virtual bool GetIsSelected() const ;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetButtonName(FText optionName) { _optionName = optionName; }
	void SetIndexInGroup(uint64 key);
	uint64 GetIndexInGroup() const { return _keyId; }
	void SetToggleCallback(TFunction<void(uint64, bool)> InToggleCallback);
};
