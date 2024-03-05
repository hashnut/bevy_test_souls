// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUICoolTimeEffect.generated.h"

/**
 * 쿨타임 흐르는 연출 UI
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUICoolTimeEffect : public UUserWidget
{
	GENERATED_BODY()
			
public:
	UFUNCTION(BlueprintCallable)
	virtual void SetProgress(float InRate) {}
};
