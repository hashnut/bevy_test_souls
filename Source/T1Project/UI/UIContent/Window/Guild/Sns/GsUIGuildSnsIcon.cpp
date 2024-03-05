#include "GsUIGuildSnsIcon.h"

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/PanelWidget.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "T1Project.h"

static UWidgetSwitcher* RecursiveFindSwitcher(UPanelWidget* InPanel)
{
	if (nullptr == InPanel)
	{
		return nullptr;
	}

	if (InPanel->IsA<UWidgetSwitcher>())
	{
		return Cast<UWidgetSwitcher>(InPanel);
	}

	const int32 ChildrenCount = InPanel->GetChildrenCount();
	for (int32 i = 0; i < ChildrenCount; ++i)
	{
		UWidget* ChildWidget = InPanel->GetChildAt(i);
		if (ChildWidget->IsA<UPanelWidget>())
		{
			RecursiveFindSwitcher(Cast<UPanelWidget>(ChildWidget));
		}
	}

	return nullptr;
}

void UGsUIGuildSnsIcon::SetParameter(const Parameters& InParameters)
{
	_parameters = InParameters;

	_switcher->SetActiveWidgetIndex(_parameters._iconIndex);

	UWidgetSwitcher* WidgetSwitcher = RecursiveFindSwitcher(Cast<UPanelWidget>(_switcher->GetWidgetAtIndex(_parameters._iconIndex)));
	if (nullptr != WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(_parameters._isOn ? 1 : 0);
	}
}
