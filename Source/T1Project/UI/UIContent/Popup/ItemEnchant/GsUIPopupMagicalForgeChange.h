// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIPopupMagicalForgeChange.generated.h"


class UTextBlock;
class UGsButton;
/**
 * 마법 부여 변경 확인 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupMagicalForgeChange : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCurrent;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockExpect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

private:
	// T: Yes, F: No
	TFunction<void(bool)> _buttonCallback;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;
	virtual bool OnBack() override;

public:
	void SetCurrentText(const FText& InText, const FSlateColor& InColor);
	void SetExpectText(const FText& InText, const FSlateColor& InColor);
	void SetCallback(TFunction<void(bool)> InCallback);

private:
	UFUNCTION()
	void OnClickCancel();
	UFUNCTION()
	void OnClickOk();
};
