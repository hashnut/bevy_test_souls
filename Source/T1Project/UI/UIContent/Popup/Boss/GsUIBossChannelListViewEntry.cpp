#include "GsUIBossChannelListViewEntry.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Button.h"
#include "GsUIPopupBossContribution.h"
#include "GsUIBossChannelPanel.h"
#include "T1Project/UI/UIControlLib/ContentWidget/GsButton.h"

void UGsUIBossChannelListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_button->OnClicked.AddDynamic(this, &UGsUIBossChannelListViewEntry::OnClickButton);
}

void UGsUIBossChannelListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	UGsUIBossChannelListViewEntryData* data = Cast<UGsUIBossChannelListViewEntryData>(inListItemObject);
	if (data)
	{
		_textBlock->SetText(FText::FromString(data->_channelName));
		_channel = data->_channel;
		_parents = data->_parents;
		_channelPanel = data->_channelPanel;
	}
}

void UGsUIBossChannelListViewEntry::OnClickButton()
{
	_channelPanel->Close();
	_parents->SetChannel(_channel);	
}
