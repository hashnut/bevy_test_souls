#include "GsUIPartyOwnTab.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UMG/Public/Components/WidgetSwitcher.h"

#include "Core/Public/Delegates/Delegate.h"

void UGsUIPartyOwnTab::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnTab->OnClicked.AddDynamic(this, &UGsUIPartyOwnTab::OnClickTab);
}

// 클릭 탭
void UGsUIPartyOwnTab::OnClickTab()
{
	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}

void UGsUIPartyOwnTab::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	int tabType = (bInIsSelected == true) ? 0 : 1;
	_switcherType->SetActiveWidgetIndex(tabType);
}

// 텍스트 세팅
void UGsUIPartyOwnTab::SetText(const FText& In_text)
{
	_textSelected = In_text;
	_textUnSelected = In_text;
}

bool UGsUIPartyOwnTab::GetIsSelected() const
{
	return _bIsSelected;
}

// 버튼 활성화 할지
void UGsUIPartyOwnTab::SetButtonEnable(bool In_enable)
{
	_btnTab->SetIsEnabled(In_enable);
}