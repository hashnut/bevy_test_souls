#include "GsUISimpleTextViewEntry.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Button.h"
#include "T1Project/UI/UIControlLib/ContentWidget/GsButton.h"

void UGsUISimpleTextViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_button->OnClicked.AddDynamic(this, &UGsUISimpleTextViewEntry::OnClickButton);
}

void UGsUISimpleTextViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	UGsUISimpleTextViewEntryData* data = Cast<UGsUISimpleTextViewEntryData>(inListItemObject);
	if (data)
	{
		_textBlock->SetText(FText::FromString(data->_text));
	}
}

void UGsUISimpleTextViewEntry::OnClickButton()
{
	
}
