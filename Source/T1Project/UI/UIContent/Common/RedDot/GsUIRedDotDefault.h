// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "GsUIRedDotDefault.generated.h"

/**
 * 레드닷 일반, 체크타입(이미지만 있는 타입)
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIRedDotDefault : public UGsUIRedDotBase
{
	GENERATED_BODY()

public:
	virtual void SetRedDot(bool bIsRedDot, EGsIconRedDotType InType = EGsIconRedDotType::NORMAL, int32 InNumber = 0) override;
};
