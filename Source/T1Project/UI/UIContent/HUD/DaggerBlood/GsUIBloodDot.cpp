#include "GsUIBloodDot.h"
#include "Image.h"

void UGsUIBloodDot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIBloodDot::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIBloodDot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIBloodDot::FinishEffectAnimation()
{
	_OnCompleteAnimation.Broadcast();
}

void UGsUIBloodDot::UpdateWidget()
{
	StartEffectAnimation();
}

