// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIDealScrollCurveImage.h"
#include "DataSchema/DealScroll/GsSchemaDealScrollData.h"


void UGsUIDealScrollCurveImage::OnCurveAnimationFinished(EGsUIAnimationTriggerType InTriggerType)
{
	// 주의: 별도 처리를 위해 일부로 막았음
	//Super::OnCurveAnimationFinished(InTriggerType);

	if (EGsUIAnimationTriggerType::TRIGGER == InTriggerType)
	{
		CloseInternal();
	}
}

void UGsUIDealScrollCurveImage::OnManagerTick(float InDeltaTime)
{
	for (FGsUICurveAnimation& curveAnim : _curveAnimationList)
	{
		if (curveAnim.IsPlaying())
		{
			curveAnim.Update(InDeltaTime * _playbackSpeed);
		}
	}
}

void UGsUIDealScrollCurveImage::SetCurrentPlaybackSpeed(float InPlaybackSpeed)
{
	_playbackSpeed = InPlaybackSpeed;
}

void UGsUIDealScrollCurveImage::StartAnimation(const FGsSchemaDealScrollData& InData)
{
	Super::StartAnimation(InData);

	_playbackSpeed = 1.f;

	PlayCurveAnimation(EGsUIAnimationTriggerType::TRIGGER);
}
