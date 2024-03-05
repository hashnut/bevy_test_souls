// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUISaveBatteryMode.generated.h"

class UWidgetSwitcher;
/**
 * 절전모드 캐릭터 행동 표시
 */
UCLASS()
class T1PROJECT_API UGsUISaveBatteryMode : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherMode;

public:
	void SetMode(int32 InIndex);

};