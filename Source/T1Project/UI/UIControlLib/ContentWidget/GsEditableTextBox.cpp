
#include "GsEditableTextBox.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Font.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Slate/SlateBrushAsset.h"
#include "SlateComponent/SEditableTextBoxEx.h"

UGsEditableTextBox::UGsEditableTextBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TextColor = WidgetStyle.ForegroundColor;
}

TSharedRef<SWidget> UGsEditableTextBox::RebuildWidget()
{
	VirtualKeyboardDismissAction = EVirtualKeyboardDismissAction::TextCommitOnDismiss;

	MyEditableTextBlock = SNew(SEditableTextBoxEx)
		.Style(&WidgetStyle)
		.MinDesiredWidth(MinimumDesiredWidth)
		.IsCaretMovedWhenGainFocus(IsCaretMovedWhenGainFocus)
		.SelectAllTextWhenFocused(SelectAllTextWhenFocused)
		.RevertTextOnEscape(RevertTextOnEscape)
		.SelectAllTextOnCommit(SelectAllTextOnCommit)
		.ClearKeyboardFocusOnCommit(ClearKeyboardFocusOnCommit)
		.AllowContextMenu(AllowContextMenu)
		.OnTextChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnTextChanged))
		.OnTextCommitted(BIND_UOBJECT_DELEGATE(FOnTextCommitted, HandleOnTextCommitted))
		.VirtualKeyboardType(EVirtualKeyboardType::AsKeyboardType(KeyboardType.GetValue()))
		.VirtualKeyboardOptions(VirtualKeyboardOptions)
		.VirtualKeyboardTrigger(VirtualKeyboardTrigger)
		.VirtualKeyboardDismissAction(VirtualKeyboardDismissAction)
		.Justification(Justification);

	return MyEditableTextBlock.ToSharedRef();
}

void UGsEditableTextBox::SynchronizeProperties()
{
	if(auto block = StaticCastSharedPtr<SEditableTextBoxEx>(MyEditableTextBlock))
		block->SetColorAndOpacity(TextColor);
	Super::SynchronizeProperties();
}

void UGsEditableTextBox::SetColorAndOpacity(FSlateColor color)
{
	TextColor = color;
	if (auto block = StaticCastSharedPtr<SEditableTextBoxEx>(MyEditableTextBlock))
		block->SetColorAndOpacity(TextColor);
}

FSlateColor UGsEditableTextBox::GetColorAndOpacity()
{
	return TextColor;
}

void UGsEditableTextBox::SetMaximumLength(int maxByteLength)
{
	_maxLength = maxByteLength;
}

void UGsEditableTextBox::EnableInput()
{
	if (!HasKeyboardFocus())
	{
		SetFocus();
	}
}

void UGsEditableTextBox::HandleOnTextChanged(const FText& InText)
{
//#if PLATFORM_IOS
//	GSLOG(Log, TEXT("*********** UGsEditableTextBox::HandleOnTextChanged [%s]"), *InText.ToString());
//#endif
	Super::HandleOnTextChanged(InText);
}

void UGsEditableTextBox::HandleOnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod)
{
//#if PLATFORM_IOS
//	GSLOG(Log, TEXT("*********** UGsEditableTextBox::HandleOnTextCommitted [%s] [%d]"), *InText.ToString(), CommitMethod);
//#endif
	Super::HandleOnTextCommitted(InText, CommitMethod);
}
