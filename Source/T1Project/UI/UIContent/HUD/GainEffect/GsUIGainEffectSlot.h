// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UILib/Base/WidgetBase/GsUICurveAnimation.h"
#include "GsUIGainEffectSlot.generated.h"

class UTextBlock;

/**
 * UGsUIHUDGainEffect 에 연결된 슬롯. EXP, Gold 획득 연출.
 */
UCLASS()
class T1PROJECT_API UGsUIGainEffectSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIGainEffect")
	float _lifeTimeSec = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIGainEffect")
	float _fadeOutStartSec = 1.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIGainEffect")
	float _upTimeSec = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIGainEffect")
	FGsUICurveAnimationList _animationList;

protected:
	float _remainSec = 0.f;
	float _fadeSec = 0.f;
	float _upRemainSec = 0.f;

	FVector2D _currPos;
	FVector2D _targetPos;

public:
	void Reset();
	void UpdateTick(float InDeltaTime);
	
	void SetTextValue(const FText& InValue);	
	void SetStartEffect(float InFirstScale, float InPosX);
	void SetUpEffect(float InCurrPosY, float InNextPosY);	
	
	bool IsEndLifeTime() const;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayShowAnimation();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayUpAnimation();

protected:
	UFUNCTION(BlueprintCallable)
	void SetCurveAnimationTarget(EGsUIAnimationTriggerType InTriggerType, class UWidget* InTarget);
};
