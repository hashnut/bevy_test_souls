// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "GsUIRedDotBase.generated.h"

/**
 * 레드닷 부모
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class T1PROJECT_API UGsUIRedDotBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	virtual void SetRedDot(bool bIsRedDot, EGsIconRedDotType InType = EGsIconRedDotType::NORMAL, int32 InNumber = 0) {}
};
