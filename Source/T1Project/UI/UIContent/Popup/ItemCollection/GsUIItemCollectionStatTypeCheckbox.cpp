#include "GsUIItemCollectionStatTypeCheckbox.h"

#include "Runtime/SlateCore/Public/Styling/SlateTypes.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

constexpr int32 CHECK_WIDGET_INDEX = 0;
constexpr int32 UNCHECK_WIDGET_INDEX = 1;

void UGsUIItemCollectionStatTypeCheckbox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btn->OnClicked.AddDynamic(this, &UGsUIItemCollectionStatTypeCheckbox::OnClickBtn);
}

void UGsUIItemCollectionStatTypeCheckbox::OnClickBtn()
{
	SetState((ECheckBoxState::Checked == GetState()) ? ECheckBoxState::Unchecked : ECheckBoxState::Checked);
	_onClickDelegate.ExecuteIfBound();
}

void UGsUIItemCollectionStatTypeCheckbox::SetTitle(const FText& InText)
{
	_textNameOn->SetText(InText);
	_textNameOff->SetText(InText);
}

void UGsUIItemCollectionStatTypeCheckbox::SetState(const ECheckBoxState InCheckState)
{
	_switcher->SetActiveWidgetIndex((ECheckBoxState::Checked == InCheckState) ? CHECK_WIDGET_INDEX : UNCHECK_WIDGET_INDEX);
}

ECheckBoxState UGsUIItemCollectionStatTypeCheckbox::GetState() const
{
	return (_switcher->GetActiveWidgetIndex() == CHECK_WIDGET_INDEX) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}