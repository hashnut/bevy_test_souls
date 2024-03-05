// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITicker.h"
#include "Containers/Queue.h"
#include "GsUITrayTickerMessage.generated.h"


UENUM(BlueprintType)
enum class EGsTickerAnimationType : uint8
{
	NONE = 0,
	FADE_IN_OUT
};

struct FGsTickerMessageData
{
	FText _text;
	float _playTime;
	EGsTickerAnimationType _animationType;

	float _delayTime{ 0.f };
};

/**
 * 메시지 티커 출력
 */
UCLASS()
class T1PROJECT_API UGsUITrayTickerMessage : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class URichTextBlock* _richTextBlock;

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

	virtual TrayCondition GetTrayCondition() const override { return _eCondition; }
	void SetTrayCondition(TrayCondition InCondition) { _eCondition = InCondition; }

protected:
	void StartPlay();
	void StartPlayInter(FGsTickerMessageData& InData);
	void OnEndPlayTime();
	bool IsPlaying() const;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "GsUI")
	void StartAnimation(EGsTickerAnimationType InAnimationType);	
};
