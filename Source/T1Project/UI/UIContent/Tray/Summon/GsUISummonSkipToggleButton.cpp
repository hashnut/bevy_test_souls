#include "GsUISummonSkipToggleButton.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UMG/Public/Components/WidgetSwitcher.h"

#include "Core/Public/Delegates/Delegate.h"

void UGsUISummonSkipToggleButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnTab->OnClicked.AddDynamic(this, &UGsUISummonSkipToggleButton::OnClickTab);
}
// 클릭 탭
void UGsUISummonSkipToggleButton::OnClickTab()
{
	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}
void UGsUISummonSkipToggleButton::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;
	// 0: 선택
	// 1: 비선택
	int tabType = (bInIsSelected == true) ? 0 : 1;
	
	_switcherSelect->SetActiveWidgetIndex(tabType);
}

// 세팅 텍스트가 다를 때
void UGsUISummonSkipToggleButton::SetText(const FText& In_Text)
{
	_textSkip = In_Text;
}

bool UGsUISummonSkipToggleButton::GetIsSelected() const
{
	return _bIsSelected;
}