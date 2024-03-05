// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "GsUIWindowIDPassword.generated.h"

class UEditableTextBox;
class UGsButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowIDPassword : public UGsUIWindow
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnClickButton, const FString&, const FString&);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UEditableTextBox* _editID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UEditableTextBox* _editPassword;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnIDPassword;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockNotice;

public:
	FOnClickButton OnClickButton;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnInitialized() override;
	virtual bool OnBack() override;

protected:
	UFUNCTION()
	void OnClickIDPassword();
};
