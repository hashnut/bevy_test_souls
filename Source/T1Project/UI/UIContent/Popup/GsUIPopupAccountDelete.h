// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupAccountDelete.generated.h"

class UEditableTextBox;
class UGsButton;
/**
 *  계정 삭제 확인 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupAccountDelete : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UEditableTextBox* _editText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	// T: Ok, F: Cancel
	TFunction<void(bool)> _buttonCallback;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

protected:
	virtual bool OnBack() override;
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickOk();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCancel();

public:
	void SetData(TFunction<void(bool)> InCallback);
};
