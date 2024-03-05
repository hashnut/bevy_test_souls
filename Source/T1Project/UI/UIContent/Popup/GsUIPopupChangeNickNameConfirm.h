// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupChangeNickNameConfirm.generated.h"

class UTextBlock;
class UGsButton;

/**
 * 닉네임 변경 확인 창(기존 닉네임, 변경 닉네임 입력)
 */
UCLASS()
class T1PROJECT_API UGsUIPopupChangeNickNameConfirm : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbCurrNickName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbChangeNickName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

protected:
	bool _bIsOk = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickOK();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCancel();

public:
	void SetData(const FString& InCurrNickName, const FString& InChangeNickName);
};
