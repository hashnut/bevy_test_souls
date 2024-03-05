// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIDealScrollCurve.h"
#include "Components/TextBlock.h"
#include "DataSchema/DealScroll/GsSchemaDealScrollData.h"


void UGsUIDealScrollCurve::OnCurveAnimationFinished(EGsUIAnimationTriggerType InTriggerType)
{
	// 주의: 별도 처리를 위해 일부로 막았음
	//Super::OnCurveAnimationFinished(InTriggerType);

	if (EGsUIAnimationTriggerType::TRIGGER == InTriggerType)
	{
		CloseInternal();
	}
}

void UGsUIDealScrollCurve::OnManagerTick(float InDeltaTime)
{
	for (FGsUICurveAnimation& curveAnim : _curveAnimationList)
	{
		if (curveAnim.IsPlaying())
		{
			curveAnim.Update(InDeltaTime * _playbackSpeed);
		}
	}
}

void UGsUIDealScrollCurve::SetCurrentPlaybackSpeed(float InPlaybackSpeed)
{
	_playbackSpeed = InPlaybackSpeed;
}

void UGsUIDealScrollCurve::StartAnimation(const FGsSchemaDealScrollData& InData)
{
	Super::StartAnimation(InData);

	_playbackSpeed = 1.f;

	PlayCurveAnimation(EGsUIAnimationTriggerType::TRIGGER);
}

void UGsUIDealScrollCurve::SetValueText(const FText& InText)
{
	_textBlockValue->SetText(InText);
}

void UGsUIDealScrollCurve::SetValueString(const FString& InValue, bool bIsNumber)
{
	_textBlockValue->SetText(FText::FromString(InValue));
}
