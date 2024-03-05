#include "GsEventActionDimmedOn.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

bool UGsEventActionDimmedOn::OnAfterPlay()
{
	// 딤드를 켬
	FGsUIHelper::TrayDimmed(true, FLinearColor::Black);
	return true;
}
