// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "GsUIRedDotNumber.generated.h"

/**
 * 레드닷 넘버 타입
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIRedDotNumber : public UGsUIRedDotBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockNumber;

public:
	virtual void SetRedDot(bool bIsRedDot, EGsIconRedDotType InType = EGsIconRedDotType::NORMAL, int32 InNumber = 0) override;
};
