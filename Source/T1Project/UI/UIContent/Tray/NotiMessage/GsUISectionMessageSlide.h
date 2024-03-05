#pragma once
#include "CoreMinimal.h"
#include "GsUISectionMessageBase.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUISectionMessageSlide.generated.h"

using FGsTaskTrayAct = UGsUISectionMessageBase::FGsTaskTrayAct;

class UWidgetAnimation;
/**
 * (delay enable) -> fadeIn -> slide slot positioning -> output message & slide -> fadeOut
 * slide message. single line output
 */
UCLASS()
class T1PROJECT_API UGsUISectionMessageSlide : public UGsUISectionMessageBase, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", Meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* _animationFade;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	float _slideAnimSpeed = 1.f;

	UPROPERTY()
	UCanvasPanelSlot* _panelSlot{ nullptr };

protected:
	float _maxRange{ 0.f };
	FVector2D _orgPos;

public:

	class FGsTaskTrayActSlide : public FGsTaskTrayAct
	{
	public:
		using Super = FGsTaskTrayAct;
	protected:

		TWeakObjectPtr<UGsUISectionMessageSlide> _widget;
		float _animEndTime{ 0.f };
		float _xMoveLength{ 0.f };
		float _speed{ 0.f };
		FVector2D _orgPos;
	public:

		FGsTaskTrayActSlide(FString taskName, float endTime,
			TFunction<void()> onFnxStarted = nullptr, 
			TFunction<void()> onFnxFinished = nullptr, 
			TWeakObjectPtr<UGsUISectionMessageSlide> widget = nullptr)
			: FGsTaskTrayAct(taskName, endTime, onFnxStarted, onFnxFinished)
			, _widget(widget)
		{}
		void onStart() override;
		void update(float delta) override;
		bool cond() override
		{
			return Super::cond();
		}

		void onFinish() override
		{
			Super::onFinish();
		}
	};

protected:
	virtual void NativeConstruct() override;
	virtual void OnInitWork(TWeakPtr<FGsTickerMessageDataEx> data) override;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "GsUI")
	void StartAnimation(EGsTickerSectionAnimationType InAnimationType);

	virtual void InitSection() override;
	virtual void InitSection(float maxRange, UCanvasPanelSlot* panelSlot) override;

};

