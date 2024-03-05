// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "UI/UILib/Base/GsUITicker.h"
#include "GsUITrayTickerAnimation.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITrayTickerAnimation : public UGsUITicker
{
	GENERATED_BODY()
	
private:
	TFunction<void()> _exitAnimationCallback;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnCommandPlayAnimation();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	virtual void OnExitAnimation();

protected:
	virtual void NativeConstruct() override;
	virtual void RestartAllAnimations() override;

public:
	virtual void SetData(TFunction<void()> InCallback = nullptr);
};
