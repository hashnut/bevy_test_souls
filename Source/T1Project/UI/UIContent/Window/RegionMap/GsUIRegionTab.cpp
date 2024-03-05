#include "GsUIRegionTab.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UMG/Public/Components/WidgetSwitcher.h"

#include "Core/Public/Delegates/Delegate.h"

void UGsUIRegionTab::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnTab->OnClicked.AddDynamic(this, &UGsUIRegionTab::OnClickTab);
}
// 클릭 탭
void UGsUIRegionTab::OnClickTab()
{

	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}
void UGsUIRegionTab::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;
	// 0: 선택
	// 1: 비선택
	int tabType = (bInIsSelected == true) ? 0 : 1;
	
	_switcherType->SetActiveWidgetIndex(tabType);
}

// 텍스트 세팅
void UGsUIRegionTab::SetText(const FText& In_text)
{
	_textTab = In_text;
}

bool UGsUIRegionTab::GetIsSelected() const
{
	return _bIsSelected;
}