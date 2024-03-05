// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupChangeNickName.generated.h"

class UEditableTextBox;
class UGsButton;

/**
 * 닉네임 변경 확인 창(기존 닉네임, 변경 닉네임 입력)
 */
UCLASS()
class T1PROJECT_API UGsUIPopupChangeNickName : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _editNickName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// Back키 대응 필요여부 확인

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	void CallOkCallback(const FString& InStr);

protected:
	UFUNCTION()
	void OnTextChangedNickName(const FText& InText);

	UFUNCTION()
	void OnClickOK();

	UFUNCTION()
	void OnClickCancel();
};
