#include "GsUISummonMileageAddedEffect.h"

#include "UMG/Public/Animation/WidgetAnimation.h"

void UGsUISummonMileageAddedEffect::NativeConstruct()
{
	Super::NativeConstruct();

	float endTime = _animation->GetEndTime();
	PlayAnimation(_animation, endTime, 1, EUMGSequencePlayMode::Reverse);

}

// 애니 시작
void UGsUISummonMileageAddedEffect::StartAnimation()
{
	// 애니 맨 앞으로 돌리기
	float endTime = _animation->GetEndTime();
	PlayAnimation(_animation, endTime, 1, EUMGSequencePlayMode::Reverse);

	// 애니 시작
	PlayAnimation(_animation);
}