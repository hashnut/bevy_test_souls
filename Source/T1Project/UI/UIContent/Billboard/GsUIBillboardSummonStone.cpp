#include "GsUIBillboardSummonStone.h"

#include "UMG/Public/Components/WidgetSwitcher.h"

// 소환석 입력 타입 세팅
void UGsUIBillboardSummonStone::SetSummonStoneInputType(EGsUISummonStoneInputType In_type)
{
	_switcherInputType->SetActiveWidgetIndex((int)In_type);
}