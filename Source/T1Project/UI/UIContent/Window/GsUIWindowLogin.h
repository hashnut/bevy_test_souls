// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "GsUIWindowLogin.generated.h"

class UGsCheckBox;
/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUIWindowLogin : public UGsUIWindow
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClickButton, const FString&);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	class UEditableTextBox* _editVidID;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsCheckBox* _checkBoxAuto;

	UPROPERTY()
	class UGsButton* _btnConnect;

public:
	FOnClickButton OnClickButton;

private:
	FText _prevVidText;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual bool OnBack() override;

protected:
	UFUNCTION()
	void OnClickConnect();
	// 오토 상태 변경 클릭
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickAutoLogin(int32 In_indexGroup, bool In_isChecked);
	// VID 입력 이벤트
	UFUNCTION()
	void OnTextChangeEvent(const FText& InText);

public:
	void SetData(const FString& InVidID);

protected:
	bool IsInvalidateVID(const FText& InText, OUT FText& outText);

public:
	// UUserWidget override.
	virtual void RemoveFromParent() override;
};
