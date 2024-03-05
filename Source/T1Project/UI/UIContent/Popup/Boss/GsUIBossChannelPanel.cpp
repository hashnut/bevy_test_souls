#include "GsUIBossChannelPanel.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/ListView.h"
#include "GsUIBossChannelListViewEntry.h"

void UGsUIBossChannelPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUIBossChannelPanel::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(TEXT("UICommonText"), TEXT("NChannel"), _textChannelFormat);
}

void UGsUIBossChannelPanel::Open()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);	
}

void UGsUIBossChannelPanel::InitChannel(TArray<TSharedPtr<FGsChannelDefeatList>> inChannelDefeatArray, TWeakObjectPtr<UGsUIPopupBossContribution> inParents)
{	
	TArray<UGsUIBossChannelListViewEntryData*> dataArray;
	UGsUIBossChannelListViewEntryData* data = nullptr;	
	FString channelName;	
	int32 channelId;	
	int32 channelNum = inChannelDefeatArray.Num();
	for (int32 i = 0; i < channelNum; ++i)
	{
		channelId = inChannelDefeatArray[i].Get()->_channelId;
		data = NewObject<UGsUIBossChannelListViewEntryData>();		
		data->_channelName = FText::Format(_textChannelFormat, channelId).ToString();
		data->_channel = channelId;
		data->_parents = inParents;
		data->_channelPanel = this;
		dataArray.Add(data);
	}

	_channelListView->SetListItems(dataArray);
}

void UGsUIBossChannelPanel::Clear()
{
	_channelListView->ClearListItems();
}

void UGsUIBossChannelPanel::Close()
{
	SetVisibility(ESlateVisibility::Hidden);
}

bool UGsUIBossChannelPanel::IsOpen()
{
	return GetVisibility() == ESlateVisibility::SelfHitTestInvisible;
}
