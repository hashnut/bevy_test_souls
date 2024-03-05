// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "GsUITraySafeEnchantItemResult.generated.h"

class UWidgetSwitcher;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFinishedAnim, const bool);
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITraySafeEnchantItemResult : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _tickerSwitcher;

public:
	FOnFinishedAnim _onFinishedAnim;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetEffectData(int32 InSwitcherIndex);

public:
	UFUNCTION(BlueprintCallable, Category = "Anim")
	void OnFinishedAnimation();
	
};
