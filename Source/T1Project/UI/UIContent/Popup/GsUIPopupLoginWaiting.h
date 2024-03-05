// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupLoginWaiting.generated.h"

class UTextBlock;
class UGsButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupLoginWaiting : public UGsUIPopup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	UTextBlock* _loginWaitingText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	TFunction<void()> _cancelCallback;

	int _loginWaitingCount;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;	

	virtual bool OnBack() override;

public:
	void SetData(int InWaitingCount,
		TFunction<void()> InCancelCallback = nullptr);

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCancel();

	// 2022/10/20 PKT - KeyBoard Mapping
	virtual void OnInputCancel();
};
