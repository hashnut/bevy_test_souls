

#include "GsUITreeViewSubElement.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

void UGsUITreeViewSubElement::NativeOnInitialized()
{
	UGsUITreeViewBaseElement::NativeOnInitialized();
}

void UGsUITreeViewSubElement::NativeConstruct()
{
	UGsUITreeViewBaseElement::NativeConstruct();

	_switcherState->SetActiveWidgetIndex(NORMAL_SWITCHER_INDEX);
}

bool UGsUITreeViewSubElement::IsSelected() const
{
	return (_switcherState->GetActiveWidgetIndex() == SELECTED_SWITCHER_INDEX);
}

void UGsUITreeViewSubElement::SetSelected(bool InIsSelected)
{
	// 2021/12/05 PKT - 상태 변경
	_switcherState->SetActiveWidgetIndex(InIsSelected ? SELECTED_SWITCHER_INDEX : NORMAL_SWITCHER_INDEX);

	UGsUITreeViewBaseElement::SetSelected(InIsSelected);
}