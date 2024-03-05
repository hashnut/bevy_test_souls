// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/UILib/Base/GsUITray.h"
#include "GsUITrayTickerArenaCount.h"
#include "GSUITrayTickerCountEX.generated.h"

/**
 * Tray Condition ¸¸ ¹Ù²Þ
 */
UCLASS()
class T1PROJECT_API UGSUITrayTickerCountEX : public UGsUITrayTickerAnimation
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockTime;

	FTimerHandle _timerHandler;

	time_t _timerClearTime = 0;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void PlayCount(int32 InCount);

	void ClearTimer();
public:
	virtual TrayCondition GetTrayCondition() const override { return TrayCondition::TC_AlwaysShow; }

	void SetCountData(int32 InCount, time_t InEndTime, TFunction<void()> InCallback = nullptr);
};
