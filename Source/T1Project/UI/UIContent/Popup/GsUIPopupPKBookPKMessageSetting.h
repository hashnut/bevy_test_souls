// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "GsUIPopupPKBookPKMessageSetting.generated.h"



UCLASS()
class T1PROJECT_API UGsUIPopupPKBookPKMessageSetting : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _btnCheck;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UEditableTextBox* _editorTextBoxKillMessage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UEditableTextBox* _editorTextBoxDieMessage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnOK;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	bool LimitTextLength(const FText& InOrgText, int32 InLimitLength, FText& OutText);
public:
	virtual void BeginDestroy() override;

protected:
	UFUNCTION()
	void OnClickedCancel();

	UFUNCTION()
	void OnClickedOK();

	UFUNCTION()
	void OnSelectedBtnCheck(bool InIsSelected);

	UFUNCTION()
	void OnClickedBtnCheck();

	UFUNCTION()
	void OnTextChangedKillMessage(const FText& InText);

	UFUNCTION()
	void OnTextChangedDieMessage(const FText& InText);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
