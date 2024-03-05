// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "GsUIWindowAccountLogin.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWindowAccountLogin : public UGsUIWindow
{
	GENERATED_BODY()	
public:
	DECLARE_DELEGATE_OneParam(FOnClickButton, const FString&);
		
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	class UEditableTextBox* _editAccountID;
	UPROPERTY()
	class UGsButton* _btnLogin;

public:
	FOnClickButton OnClickButton;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual bool OnBack() override;

public:
	UFUNCTION()
	void OnClickLogin();

public:
	void SetData(const FString& InAccountID);
	
};
