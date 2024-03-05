

#include "GsUITreeViewMainElement.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

void UGsUITreeViewMainElement::NativeOnInitialized()
{
	UGsUITreeViewBaseElement::NativeOnInitialized();
}	

void UGsUITreeViewMainElement::NativeConstruct()
{
	UGsUITreeViewBaseElement::NativeConstruct();

	_switcherState->SetActiveWidgetIndex(CLOSE_SWITCHER_INDEX);
}

bool UGsUITreeViewMainElement::IsSelected() const
{
	return (_switcherState->GetActiveWidgetIndex() == OPEN_SWITCHER_INDEX);
}

void UGsUITreeViewMainElement::SetSelected(bool InIsSelected)
{
	// 2021/12/05 PKT - 상태 변경
	_switcherState->SetActiveWidgetIndex(InIsSelected ? OPEN_SWITCHER_INDEX : CLOSE_SWITCHER_INDEX);

	UGsUITreeViewBaseElement::SetSelected(InIsSelected);
}