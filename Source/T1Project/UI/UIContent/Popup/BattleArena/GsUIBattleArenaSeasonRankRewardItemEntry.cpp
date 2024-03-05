#include "GsUIBattleArenaSeasonRankRewardItemEntry.h"
#include "../Classes/GsPoolUObject.h"
#include "../Reward/GsRewardHelper.h"
#include "../UI/UIContent/Common/Entry/GsUIRewardItemIconListViewEntry.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaHelper.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/ListView.h"
#include "UMG/Public/Components/TextBlock.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "DataSchema/BattleArena/GsSchemaBattleArenaRankingSeasonReward.h"
#include "T1Project.h"



void UGsUIBattleArenaSeasonRankRewardItemEntry::BeginDestroy()
{
	Super::BeginDestroy();

	if (_rewardEntryPool)
	{
		_rewardEntryPool->RemovePool();
		_rewardEntryPool = nullptr;
	}
}

void UGsUIBattleArenaSeasonRankRewardItemEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const static int32 POOL_WARM_UP_SIZE = 30;
	_rewardEntryPool = NewObject<UGsPoolUObject>(this);
	_rewardEntryPool->WarmUp<UGsUIRewardIconListViewEntryData>(POOL_WARM_UP_SIZE);
}

void UGsUIBattleArenaSeasonRankRewardItemEntry::NativeConstruct()
{
	Super::NativeConstruct();

	_listViewRewardSet->ClearListItems();
	_rewardEntryPool->ReleaseAll();
}

void UGsUIBattleArenaSeasonRankRewardItemEntry::NativeDestruct()
{
	_listViewRewardSet->ClearListItems();
	_rewardEntryPool->ReleaseAll();

	Super::NativeDestruct();
}

void UGsUIBattleArenaSeasonRankRewardItemEntry::SetData(const FGsSchemaBattleArenaRankingSeasonReward* InData, int32 InMyRank)
{
	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("nullptr == InData"));
		return;
	}

	// 2024/2/1 PKT - 높은 순위가 Min에 속함.(1등이 2등보다 큼.)
	int32 minRank = InData->rankingGroupData.rankingGroupRangeMin;
	int32 maxRank = InData->rankingGroupData.rankingGroupRangeMax;
	
	FText textFormat;
	FGsBattleArenaHelper::FindText(TEXT("UI_Ranking_RankMinMax"), textFormat);

	FText textRank;
	if (minRank == maxRank)
	{
		textRank = FText::Format(textFormat, { minRank });
	}
	else
	{
		FString strFormat = FString::Format(TEXT("{0} - {1}"), {minRank, maxRank});
		textRank = FText::Format(textFormat, { FText::FromString(strFormat) });
	}
	_textRankRange->SetText(textRank);

	_imgSelected->SetVisibility((minRank <= InMyRank && InMyRank <= maxRank) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	/**
	 * Reward
	 */
	_rewardEntryPool->ReleaseAll();
	TArray<UGsUIRewardIconListViewEntryData*> entryDataSet;
	for (const FGsSchemaRewardDataRow& row : InData->rewardIdSetShowOnly)
	{
		FGsRewardUnboxingData unboxingData;
		if (row.GetRow() && true == FGsRewardHelper::UnboxingRewardData(row.GetRow()->id, unboxingData))
		{
			for (const auto& item : unboxingData._itemDataList)
			{
				FGsUIRewardIconData rewardItem;
				if (CurrencyType::NONE == item->currencyType)
				{
					rewardItem.Init(item, item->itemCountMin, item->itemCountMax);
				}
				else
				{
					rewardItem.Init(item, item->currencyAmountMin, item->currencyAmountMax);
				}

				UGsUIRewardIconListViewEntryData* entryData = _rewardEntryPool->Claim<UGsUIRewardIconListViewEntryData>();
				entryData->_rewardItemDataArray.Add(rewardItem);

				// 2024/2/1 PKT - Add..
				entryDataSet.Emplace(entryData);
			}
		}
	}

	_listViewRewardSet->SetListItems(entryDataSet);
	_listViewRewardSet->ScrollIndexIntoView(0);
	_listViewRewardSet->SetScrollbarVisibility(ESlateVisibility::Collapsed);
}

bool UGsUIBattleArenaSeasonRankRewardItemEntry::IsMyRank() const
{
	return ESlateVisibility::Collapsed != _imgSelected->GetVisibility();
}