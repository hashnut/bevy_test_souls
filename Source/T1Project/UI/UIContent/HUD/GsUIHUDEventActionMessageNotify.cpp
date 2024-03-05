#include "GsUIHUDEventActionMessageNotify.h"
#include "UMG/Public/Components/TextBlock.h"

void UGsUIHUDEventActionMessageNotify::SetHUDMode(EGsUIHUDMode InMode)
{
	// 2023/2/27 PKT - HUDMode 에서 핸들링 되어지는것이 아니라 서버에서 핸들링 되어야 한다.
	//Super::SetHUDMode(InMode);
}

void UGsUIHUDEventActionMessageNotify::SetData(const FText& InMessage)
{
	_message->SetText(InMessage);
}