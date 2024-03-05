// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "GsUITrayMockeryTicker.generated.h"

class UTextBlock;
class URichTextBlock;

UCLASS()
class T1PROJECT_API UGsUITrayMockeryTicker : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _textSystemMsg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textMockeryMsg;

	class UWidgetAnimation* _animationData = nullptr;

	FWidgetAnimationDynamicEvent _finishDelegate;

	TFunction<void(void)> _endCallback;
	
protected:
	virtual void NativeOnInitialized() override;

	void AnimationStart();

	UFUNCTION()
	void OnAnimationFinish();

public:
	virtual void BeginDestroy() override;

	void SetData(const FText& InSystemMsg, const FText& InMockery, TFunction<void(void)> InEndCallback = nullptr);
};
