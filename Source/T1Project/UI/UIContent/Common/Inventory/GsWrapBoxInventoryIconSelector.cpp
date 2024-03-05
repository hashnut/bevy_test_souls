
#include "GsWrapBoxInventoryIconSelector.h"
#include "UI/UIControlLib/Define/EGsIconType.h"

void UGsWrapBoxInventoryIconSelector::SetEIconType(EGsIconType InType /* = EGsIconType::NONE */)
{
	_iconType = InType;
}

void UGsWrapBoxInventoryIconSelector::SetIconKey(uint8 InIconKey /* = 0 */)
{
	_iconKey = InIconKey;
}

void UGsWrapBoxInventoryIconSelector::SetpreviewCount(int32 InPreviewCount /* = 0 */)
{
#if WITH_EDITORONLY_DATA
	_previewCount = InPreviewCount;
#endif
}