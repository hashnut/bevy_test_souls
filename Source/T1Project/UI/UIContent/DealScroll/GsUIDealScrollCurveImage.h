// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/DealScroll/GsUIDealScrollBase.h"
#include "GsUIDealScrollCurveImage.generated.h"

/**
 * 커브를 사용하는 딜스크롤(화면에 자주 출력되는 경우 이것을 사용)
 */
UCLASS()
class T1PROJECT_API UGsUIDealScrollCurveImage : public UGsUIDealScrollBase
{
	GENERATED_BODY()

protected:
	float _playbackSpeed = 1.f;

	// UGsUIWidgetBase override
public:
	virtual void OnCurveAnimationFinished(EGsUIAnimationTriggerType InTriggerType) override;

protected:
	virtual bool IsEnableManagerTick() const override { return true; }
	virtual void OnManagerTick(float InDeltaTime) override;

	// UGsUIDealScrollBase override
public:
	virtual void SetCurrentPlaybackSpeed(float InPlaybackSpeed) override;

protected:
	virtual void StartAnimation(const FGsSchemaDealScrollData& InData) override;
};
