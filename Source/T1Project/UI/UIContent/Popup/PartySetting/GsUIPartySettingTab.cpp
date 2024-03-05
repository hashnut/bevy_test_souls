#include "GsUIPartySettingTab.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UMG/Public/Components/WidgetSwitcher.h"

#include "Core/Public/Delegates/Delegate.h"

void UGsUIPartySettingTab::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnTab->OnClicked.AddDynamic(this, &UGsUIPartySettingTab::OnClickTab);
}
// 클릭 탭
void UGsUIPartySettingTab::OnClickTab()
{
	if (_isUseable == false)
	{
		return;
	}

	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}

	OnToggleButtonClicked.ExecuteIfBound();
}
void UGsUIPartySettingTab::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;
	// 0: 선택
	// 1: 비선택
	// 2: 비활성화
	int tabType = 2;
	if (_isUseable == true)
	{
		tabType = (bInIsSelected == true) ? 0 : 1;
	}
	_switcherType->SetActiveWidgetIndex(tabType);
}

// 텍스트 세팅
void UGsUIPartySettingTab::SetText(const FText& In_text)
{
	_textSelected = In_text;
	_textUnSelected = In_text;
	_textDisable = In_text;
}
// 세팅 텍스트가 다를 때
void UGsUIPartySettingTab::SetText(const FText& In_selectText, const FText& In_unselectText)
{
	_textSelected = In_selectText;
	_textUnSelected = In_unselectText;
}

bool UGsUIPartySettingTab::GetIsSelected() const
{
	return _bIsSelected;
}