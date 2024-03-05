#include "GsUIIconRegionSelect.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Animation/WidgetAnimation.h"

// 선택 처리
void UGsUIIconRegionSelect::SetSelect(bool In_select)
{
	int index = (In_select == true) ? 1 : 0;
	_switcherSelect->SetActiveWidgetIndex(index);

	PlaySelectAnimation(In_select);
}

// 선택 애니 플레이
void UGsUIIconRegionSelect::PlaySelectAnimation(bool In_select)
{
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);

	if (In_select == true)
	{
		PlayAnimation(_ani_blink, 0.0f, 0);
	}
}

void UGsUIIconRegionSelect::SetSpotName(FText In_name)
{
	_textSpotName = In_name;
}

void UGsUIIconRegionSelect::NativeDestruct()
{
	// clear now
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	Super::NativeDestruct();
}