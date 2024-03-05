#include "GsUISummonWidgetSwitcherEx.h"

void UGsUISummonWidgetSwitcherEx::SetProgressRate(float In_rate)
{
	// 0: on
	// 1: off
	int switchIndex = 1;
	// over value
	if (_progressRate <= In_rate)
	{
		switchIndex = 0;
	}

	SetActiveWidgetIndex(switchIndex);
}