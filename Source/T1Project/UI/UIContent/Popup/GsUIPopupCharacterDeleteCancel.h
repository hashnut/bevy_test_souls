// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupCharacterDeleteCancel.generated.h"

class UTextBlock;
class UGsButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupCharacterDeleteCancel : public UGsUIPopup
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

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
