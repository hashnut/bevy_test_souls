// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Tray/GsUITrayTickerAnimation.h"
#include "GsUITrayTickerArenaCount.generated.h"

/**
 * 대결 카운트 다운 UI
 */
UCLASS()
class T1PROJECT_API UGsUITrayTickerArenaCount : public UGsUITrayTickerAnimation
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockTime;

protected:
	int32 _remainCount = 0;
	int32 _maxCount = 0;

	time_t _startTime = 0;
	time_t _endTime = 0;
	time_t _remainTime = 0;

	double _startAppTime = 0;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayCountAnimation();
	
	virtual void OnExitAnimation() override;

public:
	void SetCount(time_t InStartTime, int32 InCount);
};
