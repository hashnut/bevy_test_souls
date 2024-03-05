// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Tray/NotiMessage/GsUISectionMessageBase.h"
#include "GsUISectionMessageRolling.generated.h"

class URichTextBlock;
class UBorder;

/**
 * 위로 rolling 되는 ticker message 처리
 * 현재 추가된 메세지가 하나이면 static, 하나가 running 중에 메세지가 또 들어오면 메세지가 rolling
 * (delay enable) -> [fadeIn -> (output static message) -> fadeOut] -> (rolling)
 * rolling 시에는 (output static message) 생략
 * (output static message) 시에는 (rolling) 생략
 */
UCLASS()
class T1PROJECT_API UGsUISectionMessageRolling : public UGsUISectionMessageBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", Meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* _animationFade;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", Meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* _rollingFadeOut;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UBorder* _fadeBorder;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", Meta = (BindWidget))
	URichTextBlock* _fadeRollingText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	float _rollingSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	float _addSpeedByRolling{ 1.0f };

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	bool _isRollingFadeOut;

protected:
	int _workCount{ 0 };
	TWeakPtr<FGsTickerMessageDataEx> _lastData;
	TWeakPtr<FGsTickerMessageDataEx> _rollingData;

	FGsTaskManager _rollingTaskManager;

public:
	class FGsTaskTrayActCheckStay : public FGsTaskTrayAct
	{
	public:
		using Super = FGsTaskTrayAct;

	protected:
		TFunction<bool(float)> _onFnxUpdate{ nullptr };

	public:
		FGsTaskTrayActCheckStay(FString taskName, float endTime,
			TFunction<void()> onFnxStarted = nullptr, 
			TFunction<void()> onFnxFinished = nullptr, 
			TFunction<bool(float)> onFnxUpdate = nullptr)
			: FGsTaskTrayAct(taskName, endTime, onFnxStarted, onFnxFinished)
			, _onFnxUpdate(onFnxUpdate) {}

		virtual void update(float delta) override
		{
			Super::update(delta);
			if (_onFnxUpdate != nullptr && _onFnxUpdate(delta))
			{
				finish();
			}
		}
	};

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void OnInitWork(TWeakPtr<FGsTickerMessageDataEx> data) override;
	virtual void OnClearWork() override;

	bool IsWorkOverroad();
	void FinishCurrentTask(FString taskName);

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "GsUI")
	void StartAnimation(EGsTickerSectionAnimationType InAnimationType);

	virtual void InitSection() override;

	friend class UGsUISectionMessageBase::FGsTaskTrayAct;
	
};
