// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Billboard/GsUIBillboardFairyTalkBalloon.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "ActorEx//GsActorFieldFairy.h"

void UGsUIBillboardFairyTalkBalloon::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIBillboardFairyTalkBalloon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsVisible())
	{
		if (_endTime > 0.f && _endTime - FApp::GetCurrentTime() <= 0.f)
		{
			// 외부에서 visible을 껐다가 켜게되는 경우, 애니 연출중이었다면 fadeout 시켜준다
			PlayAnimation(_animationPopup, _animationPopup->GetEndTime() - 0.5f);
			_endTime = 0.f;
		}
	}
}

void UGsUIBillboardFairyTalkBalloon::SetName(FText fairyName, FairyGrade grade)
{
	_nameColor = FGsUIColorHelper::GetColorFairyGrade(grade);
	_fairyName = fairyName;

	SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIBillboardFairyTalkBalloon::SetTalk(FText talk)
{
	_fairyTalk = talk;

	static float animTime = _animationPopup->GetEndTime() + 0.5f;
	_endTime =  FApp::GetCurrentTime() + animTime;
	
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	PlayAnimation(_animationPopup, 0.f, 1, EUMGSequencePlayMode::Forward);

	//StartAnimation();
}
