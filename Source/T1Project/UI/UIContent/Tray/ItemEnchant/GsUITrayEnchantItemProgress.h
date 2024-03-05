// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUITrayEnchantItemProgress.generated.h"

class UGsButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITrayEnchantItemProgress : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnConfirm;

private:
	TFunction<void()> _confirmCallback;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetData(TFunction<void()> InConfirmCallback = nullptr);


public:
	UFUNCTION(BlueprintCallable)
	void FinishedAnim();
	UFUNCTION()
	void OnClickBlock();
	
};
