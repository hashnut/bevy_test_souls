// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DPICustomScalingRule.h"
#include "GsUIScaleRule.generated.h"

/**
 * UserInterfaceSettings 에서 사용하는 Custom UI 스케일 룰
 */
UCLASS()
class T1PROJECT_API UGsUIScaleRule : public UDPICustomScalingRule
{
	GENERATED_BODY()

private:
	FIntPoint DesignScreenSize = FIntPoint(1920, 1080);

public:
	static void SetUIScaleValue(float InValue);
	static float GetUIScaleValue();

public:
	// UDPICustomScalingRule override
	virtual float GetDPIScaleBasedOnSize(FIntPoint Size) const override;
};
