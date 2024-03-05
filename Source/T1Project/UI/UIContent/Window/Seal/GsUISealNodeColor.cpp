#include "GsUISealNodeColor.h"

void UGsUISealNodeColor::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	InvalidateColor();
}

void UGsUISealNodeColor::InvalidateColor()
{
	UWidgetAnimation* TargetAni = nullptr;
	switch (_color)
	{
	case SealSlotColor::NONE: TargetAni = _color0; break;
	case SealSlotColor::RARE: TargetAni = _color1; break;
	case SealSlotColor::EPIC: TargetAni = _color2; break;
	case SealSlotColor::UNIQUE: TargetAni = _color3; break;
	}

	if (nullptr != TargetAni)
	{
		PlayAnimation(TargetAni, 0.0f, 1);
	}
}