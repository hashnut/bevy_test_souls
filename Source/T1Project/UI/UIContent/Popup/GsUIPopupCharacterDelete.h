// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupCharacterDelete.generated.h"

class UEditableTextBox;
class UGsButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupCharacterDelete : public UGsUIPopup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UEditableTextBox* _editName;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY()
	FString _name;

	// T: Ok, F: Cancel
	TFunction<void(bool)> _buttonCallback;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetData(const FString& InName, TFunction<void(bool)> InCallback = nullptr);

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickOk();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCancel();
};
