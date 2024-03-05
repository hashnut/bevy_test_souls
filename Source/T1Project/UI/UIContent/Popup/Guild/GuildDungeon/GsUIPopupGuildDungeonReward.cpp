#include "GsUIPopupGuildDungeonReward.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "UI/UIContent/Common/Entry/GsUIRewardItemIconListViewEntry.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "DataSchema/Guild/GuildDungeon/GsSchemaGuildDungeon.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "GsGuildDungeonRewardListViewEntry.h"
#include "T1Project/UI/UIControlLib/ContentWidget/GsButton.h"

void UGsUIPopupGuildDungeonReward::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_closeButton->OnClicked.AddDynamic(this, &UGsUIPopupGuildDungeonReward::OnClickClose);
}

void UGsUIPopupGuildDungeonReward::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIPopupGuildDungeonReward::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIPopupGuildDungeonReward::InitRewardList()
{
	TArray<UGsGuildDungeonRewardListViewEntryData*> iconListViewEntryDataArray;
	UGsGuildDungeonRewardListViewEntryData* listViewEntryData = nullptr;

	for (const TPair<CurrencyType, TPair<Currency, Currency>>& currencyPair : _unboxingData._tempCurrencyMap)
	{
		listViewEntryData = NewObject<UGsGuildDungeonRewardListViewEntryData>();
		FGsUIRewardIconData rewardIconData;
		rewardIconData.Init(currencyPair.Key, currencyPair.Value.Key, currencyPair.Value.Value);
		listViewEntryData->_rewardData = rewardIconData;

		iconListViewEntryDataArray.Add(listViewEntryData);
	}

	//reward box	
	for (const FGsSchemaRewardItemBagData* bagData : _rewardItemBagDataList)
	{
		listViewEntryData = NewObject<UGsGuildDungeonRewardListViewEntryData>();
		FGsUIRewardIconData rewardIconData;
		if (CurrencyType::NONE == bagData->currencyType)
		{
			rewardIconData.Init(bagData, bagData->itemCountMin, bagData->itemCountMax);
		}
		else
		{
			rewardIconData.Init(bagData, bagData->currencyAmountMin, bagData->currencyAmountMax);
		}
		
		listViewEntryData->_rewardData = rewardIconData;

		UGsGuildDungeonRewardListViewEntryData** findReward = iconListViewEntryDataArray.FindByPredicate([&](UGsGuildDungeonRewardListViewEntryData* bag)->bool
			{				
				if (bag->_rewardData._rewardItem
					&& bag->_rewardData._rewardItem->itemId == bagData->itemId)
				{
					return true;
				}
				
				return false;
			});
		if (nullptr != findReward)
		{
			continue;
		}

		iconListViewEntryDataArray.Add(listViewEntryData);
	}

	_rewardListView->SetListItems(iconListViewEntryDataArray);
	_rewardListView->ScrollIndexIntoView(0);
}

void UGsUIPopupGuildDungeonReward::SetData(const FText& inTitle, const TArray<const FGsSchemaRewardItemBagData*>& inRewardBagList)
{
	_unboxingData.Clear();
	_rewardItemBagDataList = inRewardBagList;
	_titleTextBlock->SetText(inTitle);

	InitRewardList();
}

void UGsUIPopupGuildDungeonReward::SetData(const FText& inTitle, const TArray<const FGsSchemaRewardItemBagData*>& inRewardBagList, const FGsRewardUnboxingData& inRewardUnboxingData)
{
	_unboxingData = inRewardUnboxingData;
	_rewardItemBagDataList = inRewardBagList;
	_titleTextBlock->SetText(inTitle);

	InitRewardList();
}

void UGsUIPopupGuildDungeonReward::OnClickClose()
{
	Close();
}

void UGsUIPopupGuildDungeonReward::OnInputCancel()
{
	Close();
}