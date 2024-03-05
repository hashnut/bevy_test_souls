// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITicker.h"
#include "GsUITrayTickerInvasionNoti.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITrayTickerInvasionNoti : public UGsUITicker
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* _tickerAnimation;
	
private:
	FWidgetAnimationDynamicEvent _finishDelegate;

protected:
	UFUNCTION()
	void OnAnimationFinish();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	virtual TrayCondition GetTrayCondition() const override { return TrayCondition::TC_Priority; }
};
