#include "GsEventActionDimmedOff.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

bool UGsEventActionDimmedOff::OnAfterPlay()
{
	// 딤드를 끔
	FGsUIHelper::TrayDimmed(false);
	return true;
}
