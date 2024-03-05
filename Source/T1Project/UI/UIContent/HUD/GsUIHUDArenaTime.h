// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "GsUIHUDArenaTime.generated.h"

/**
 * HUD 대결 잔여 시간
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDArenaTime : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockTime;

protected:
	time_t _maxTime = 0;
	time_t _startTime = 0;
	time_t _remainTime = 0;

protected:
	virtual void InvalidateAllInternal() override;

public:
	virtual bool UpdateManagerTick(float InDeltaTime) override;

public:
	virtual void SetHUDMode(EGsUIHUDMode InMode) override;
	
	void InvalidateArena();

};
