// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UIContent/Tray/GsUITrayTickerAnimation.h"

#include "GsUITrayTickerArenaResult.generated.h"

/**
 * 대결 결과 UI
 */
UCLASS()
class T1PROJECT_API UGsUITrayTickerArenaResult : public UGsUITrayTickerAnimation
{
	GENERATED_BODY()	

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
