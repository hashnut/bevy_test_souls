#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "UI/UILib/Base/GsUITicker.h"
#include "FairyEnchant/GsUITrayEnchantFairyBase.h"
#include "GsUITrayTickerCollectionMulti.generated.h"

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class T1PROJECT_API UGsUITrayTickerCollectionMulti : public UGsUITicker
{
	GENERATED_BODY()

protected:
	FTimerHandle _timerOnAnimationEnd;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", Meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* _animationStart;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UTextBlock* _textName;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UTextBlock* _textDesc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	FText _name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	FText _desc;

public:
	struct FCollectionTickerInitParam
	{
		FText _name;
		FText _desc;
	};

protected:
	FGsTaskManager _taskManager;

public:
	void SetData(const FCollectionTickerInitParam& InInitData);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void OnInitWork(const FCollectionTickerInitParam& data);

public:
	class FGsTaskTrayAct : public FGsTask
	{
	public:
		using Super = FGsTask;

	protected:

		float _deltaTime{ 0.f };
		TFunction<void()> _onFnxStarted{ nullptr };
		TFunction<void()> _onFnxFinished{ nullptr };
	public:
		FGsTaskTrayAct(FString taskName, float deltaTime = 1.f, TFunction<void()> onFnxStarted = nullptr, TFunction<void()> onFnxFinished = nullptr)
			: FGsTask(taskName), _deltaTime(deltaTime), _onFnxStarted(onFnxStarted), _onFnxFinished(onFnxFinished)
		{}

		virtual void onStart() override
		{
			if (_onFnxStarted) _onFnxStarted();
		}

		virtual void onFinish() override
		{
			if (_onFnxFinished) _onFnxFinished();
		}

		virtual bool cond() override
		{
			return _deltaTime <= 0.f;
		}

		virtual void update(float delta)
		{
			_deltaTime -= delta;
		}
	};

};

