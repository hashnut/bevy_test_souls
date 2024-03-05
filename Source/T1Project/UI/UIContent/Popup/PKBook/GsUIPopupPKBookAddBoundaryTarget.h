// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupPKBookAddBoundaryTarget.generated.h"

class UGsButton;
class UEditableTextBox;
class FGsCustomizeHandler;

UCLASS()
class T1PROJECT_API UGsUIPopupPKBookAddBoundaryTarget : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UEditableTextBox* _editNickName;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnApply;


protected:
	TFunction<void(WorldId, const FString&)> _okCallback;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	void SetOkCallback(TFunction<void(WorldId InWorldId, const FString&)> InOkCallback = nullptr);
	void CallOkCallback(WorldId InWorldId, const FText& InUserName);

protected:
	UFUNCTION()
	void OnTextChangedNickName(const FText& InText);

public:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickOK();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCancel();
};
