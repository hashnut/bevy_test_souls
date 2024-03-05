// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupChangeNameMigration.generated.h"

class UEditableTextBox;
class UGsButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupChangeNameMigration : public UGsUIPopup
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnChangedName, FText);
	DECLARE_DELEGATE(FOnClosedPopup);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _editName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY(BlueprintReadOnly)
	FText _textTitle;

	UPROPERTY(BlueprintReadOnly)
	FText _textDesc;

	UPROPERTY(BlueprintReadOnly)
	FText _textHint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GSUI")
	float _btnBlockTime{ 0.2f };

	float _endCheckTime{ 0.f };

public:
	FOnChangedName OnChangedName;
	FOnClosedPopup OnClosedPopup;

public:
	virtual void BeginDestroy() override;
	virtual void SynchronizeProperties() override;

protected:
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

protected:
	UFUNCTION()
	void OnTextChangedName(const FText& InText);

	UFUNCTION()
	void OnClickOK();

	UFUNCTION()
	void OnClickCancel();

public:
	struct InitParam
	{
		FText titleText;
		FText descText;
		FText hintText;
	};

	void SetInit(InitParam* param = nullptr);
};
