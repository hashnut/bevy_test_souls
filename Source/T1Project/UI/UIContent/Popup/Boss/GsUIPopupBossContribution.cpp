#include "GsUIPopupBossContribution.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/ListView.h"
#include "UMG/Public/Components/TextBlock.h"
#include "GsUIBossChannelPanel.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "GsBossContributionListViewEntry.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "T1Project/UI/UIControlLib/ContentWidget/GsButton.h"
#include "GameObject/Boss/GsFieldBossHandler.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GsGameFlowGame.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "UMG/Public/Components/WidgetSwitcher.h"

void UGsUIPopupBossContribution::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_closeButton->OnClicked.AddDynamic(this, &UGsUIPopupBossContribution::OnClose);
	_channelButton->OnClicked.AddDynamic(this, &UGsUIPopupBossContribution::OnOpenChannel);
	_contributionListView->SetScrollbarVisibility(ESlateVisibility::Hidden);
}

void UGsUIPopupBossContribution::NativeConstruct()
{
	Super::NativeConstruct();	

	FText::FindText(TEXT("UICommonText"), TEXT("NChannel"), _textChannelFormat);
	FText::FindText(TEXT("FieldBossText"), TEXT("UI_Title_Result"), _textTitleResultForamt);

	_currentChannel = 1;
	_channelPanel->Close();
	_bossSpawnTimeTextBlock->SetText(FText::GetEmpty());
}

void UGsUIPopupBossContribution::OnClose()
{
	GUI()->Close(this);
}

void UGsUIPopupBossContribution::OnOpenChannel()
{
	if (_channelPanel)
	{
		if (_channelPanel->IsOpen())
		{
			_channelPanel->Close();
		}
		else
		{
			_channelPanel->Open();
		}
	}
}

void UGsUIPopupBossContribution::InvalidBossContribution()
{		
	_contributionTitleTextBlock->SetText(FText::Format(_textTitleResultForamt, _bossNpcData->nameText));

	UGsBossContributionListViewEntryData* data = nullptr;
	const FGsSchemaFieldBossRewardData* reward = nullptr;
	TArray<UGsBossContributionListViewEntryData*> bossContributionArray;

	if (false == _bossDepeatData.IsValid())
	{
		return;
	}

	int32 num = _bossDepeatData.Get()->_channelDefeatArray.Num();
	TSharedPtr<FGsChannelDefeatList>* findChannel = _bossDepeatData->_channelDefeatArray.FindByPredicate([&](TSharedPtr<FGsChannelDefeatList> channel) {
		return channel->_channelId == _currentChannel;
		});

	if (nullptr == findChannel
	|| false == (*findChannel).IsValid())
	{
		return;				
	}

	TArray<TSharedPtr<FGsBossRankInfo>> rankInfoArray = (*findChannel).Get()->rankInfoArray;
	int32 rankNum = rankInfoArray.Num();
	for (int32 i = 0; i < rankNum; ++i)
	{
		data = NewObject<UGsBossContributionListViewEntryData>();		
		data->Initailize(i + 1, rankInfoArray[i].Get());		
		bossContributionArray.Add(data);
	}

	_contributionListView->SetListItems(bossContributionArray);
	_contributionListView->SetScrollbarVisibility(ESlateVisibility::Hidden);
}

void UGsUIPopupBossContribution::InitContribution(const FGsSchemaFieldBossData* inBossData, TSharedPtr<FGsResultDepeatFieldBoss> inData)
{	
	SetResultDepeatBoss(inData);
	SetBossData(inBossData, inBossData->bossNpcId.GetRow());
	InitChannel();
}

void UGsUIPopupBossContribution::SetResultDepeatBoss(TSharedPtr<FGsResultDepeatFieldBoss> inData)
{	
	_bossDepeatData = inData;

	_channelPanel->Clear();

	//Set first channel id
	if (_bossDepeatData->_channelDefeatArray.Num() > 0)
	{				
		_channelPanel->InitChannel(_bossDepeatData->_channelDefeatArray, this);
	}	
}

void UGsUIPopupBossContribution::SetChannel(int32 inChannel)
{
	_currentChannel = inChannel;	
	_channelTextBlock->SetText(FText::Format(_textChannelFormat, inChannel));	

	TSharedPtr<FGsChannelDefeatList>* findChannel = _bossDepeatData->_channelDefeatArray.FindByPredicate([&](TSharedPtr<FGsChannelDefeatList> channel){
		return channel->_channelId == inChannel;
	});

	if (findChannel)
	{
		if ((*findChannel).IsValid())
		{
			const FDateTime& defeatTime = (*findChannel).Get()->_logDateTime;
			_bossSpawnTimeTextBlock->SetText(FText::FromString(defeatTime.ToString(TEXT("%Y.%m.%d %H:%M"))));
		}
	}

	InvalidBossContribution();
}

void UGsUIPopupBossContribution::InitChannel()
{
	if (_bossDepeatData.IsValid())
	{
		if (_bossDepeatData->_channelDefeatArray.Num() > 0)
		{
			int32 channelId = _bossDepeatData->_channelDefeatArray[0].Get()->_channelId;
			SetChannel(channelId);

			_widgetSwitcher->SetActiveWidgetIndex(0);
			return;
		}		
	}

	_widgetSwitcher->SetActiveWidgetIndex(1);
	SetChannel(1);
}

void UGsUIPopupBossContribution::SetBossData(const FGsSchemaFieldBossData* inBossData, const FGsSchemaNpcData* inBossNpcData)
{
	_bossData = inBossData;
	_bossNpcData = inBossNpcData;
}

const FGsSchemaFieldBossRewardData* UGsUIPopupBossContribution::GetBossRewardData(const FGsSchemaFieldBossData* inBossData, int32 inRank)
{
	if (inBossData->bossRewardIdList.IsValidIndex(inRank))
	{
		return inBossData->bossRewardIdList[inRank].GetRow();
	}

	return nullptr;
}

void UGsUIPopupBossContribution::OnInputCancel()
{
	OnClose();
}