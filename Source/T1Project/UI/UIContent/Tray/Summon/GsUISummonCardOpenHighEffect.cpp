#include "GsUISummonCardOpenHighEffect.h"

#include "UMG/Public/Animation/WidgetAnimation.h"


void UGsUISummonCardOpenHighEffect::SetAnimationFirstPos()
{
	float endTime = _animation->GetEndTime();
	PlayAnimation(_animation, endTime, 1, EUMGSequencePlayMode::Reverse);
}
void UGsUISummonCardOpenHighEffect::PlayEffectAnimation()
{

	PlayAnimation(_animation);
}