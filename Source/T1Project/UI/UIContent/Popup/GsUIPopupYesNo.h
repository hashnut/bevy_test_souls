// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupYesNo.generated.h"

class URichTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupYesNo : public UGsUIPopup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, MultiLine = true))
	class URichTextBlock*  _contentRichText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _yesButtonText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _noButtonText;

	// T: Yes, F: No
	TFunction<void(bool)> _buttonCallback;

	// destruct 호출 시점에 
	// 이벤트 종료 관련 호출처리(EGsEventProgressType::UI_POPUP_YES_NO_CHNNEL_MOVE)
	// T, F: _isCallEventProcessFinish
	TFunction<void(bool)> _destructCallback;

	// 이벤트 종료 호출 할지
	bool _isCallEventProcessFinish = true;

protected:
	virtual void NativeDestruct() override;

protected:
	virtual bool OnBack() override;
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	void SetData(const FText& InContent, const FText& InYesButton, const FText& InNoButton,
		TFunction<void(bool)> InCallback = nullptr,
		TFunction<void(bool)> InDestructCallback = nullptr);

	void SetData(const FText& InContent, TFunction<void(bool)> InCallback = nullptr,
		TFunction<void(bool)> InDestructCallback = nullptr);

	// UI를 따로 만들고 콜백 함수만 등록해서 쓸 경우
	void SetData(TFunction<void(bool)> InCallback = nullptr, 
		TFunction<void(bool)> InDestructCallback = nullptr);

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickYes();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickNo();
};
