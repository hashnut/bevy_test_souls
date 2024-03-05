// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Tray/GsUITrayTickerMessage.h"
#include "UI/UILib/Base/GsUITray.h"
#include "GsUITrayTickerLauncher.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITrayTickerLauncher : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlock;

	float _remainTime = 0.0f;
	TQueue<FGsTickerMessageData> _dataQueue;
	FText _lastText;

protected:
	virtual void NativeDestruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	// bInPriority가 true일 경우, 현재 출력 중인 메시지를 끊고 바로 출력한다
	void SetData(const FText& InText, bool bInPriority = false, float InPlayTime = 1.0f,
		EGsTickerAnimationType InAnimType = EGsTickerAnimationType::FADE_IN_OUT);

	virtual TrayCondition GetTrayCondition() const override { return TrayCondition::TC_Priority; }

protected:
	void StartPlay();
	void StartPlayInter(FGsTickerMessageData& InData);
	void OnEndPlayTime();
	bool IsPlaying() const;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "GsUI")
		void StartAnimation(EGsTickerAnimationType InAnimationType);
};
