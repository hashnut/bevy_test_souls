#include "GsUIChatMacroEntry.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "EditableTextBox.h"
#include "TextBlock.h"

void UGsUIChatMacroEntry::NativeOnInitialized()
{
	_editEnterText->OnTextCommitted.AddDynamic(this, &UGsUIChatMacroEntry::OnCommitedText);
	_editEnterText->OnTextChanged.AddDynamic(this, &UGsUIChatMacroEntry::OnChangedText);

	_btnSend->OnClicked.AddDynamic(this, &UGsUIChatMacroEntry::OnClickSend);

	Super::NativeOnInitialized();
}

void UGsUIChatMacroEntry::OnChangedText(const FText& OriginalText)
{
	auto str = OriginalText.ToString();
	auto arr = str.GetCharArray();

	if (str.Len() > 40)
	{
		str = str.Left(40);
		_editEnterText->SetText(FText::FromString(str));
	}
}

void UGsUIChatMacroEntry::OnCommitedText(const FText& inputText, ETextCommit::Type InCommitType)
{
	if (InCommitType != ETextCommit::OnCleared && !inputText.IsEmpty())
	{
		OInputText.ExecuteIfBound(inputText);
	}
}

void UGsUIChatMacroEntry::OnClickSend()
{
	auto inputText = _editEnterText->GetText();
	OInputText.ExecuteIfBound(inputText);
	OnSendText.ExecuteIfBound();
}

void UGsUIChatMacroEntry::SetData(int8 index, const FText& preText)
{
	_index = index;
	_macroIndex->SetText(FText::AsNumber(index));
	_editEnterText->SetText(preText);

}