#include "GsUIBattleArenaSeasonTierRewardItemEntry.h"
#include "../Classes/GsPoolUObject.h"
#include "../../Helper/GsUIHelper.h"
#include "../Management/ScopeGame/GsBattleArenaSeasonRankManager.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaHelper.h"
#include "../Reward/GsRewardHelper.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"
#include "../UI/UIContent/Common/Entry/GsUIRewardItemIconListViewEntry.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/ListView.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "DataSchema/BattleArena/GsSchemaBattleArenaSeasonData.h"
#include "DataSchema/BattleArena/GsSchemaBattleArenaTierSeasonReward.h"

#include "T1Project.h"



void UGsUIBattleArenaSeasonTierRewardItemEntry::BeginDestroy()
{
	if (_rewardEntryPool)
	{
		_rewardEntryPool->RemovePool();
		_rewardEntryPool = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIBattleArenaSeasonTierRewardItemEntry::NativeOnInitialized()
{
	const static int32 POOL_WARM_UP_SIZE = 30;
	_rewardEntryPool = NewObject<UGsPoolUObject>(this);
	_rewardEntryPool->WarmUp<UGsUIRewardIconListViewEntryData>(POOL_WARM_UP_SIZE);

	Super::NativeOnInitialized();
}

void UGsUIBattleArenaSeasonTierRewardItemEntry::NativeConstruct()
{
	Super::NativeConstruct();

	_listViewRewardSet->ClearListItems();
	_rewardEntryPool->ReleaseAll();
}

void UGsUIBattleArenaSeasonTierRewardItemEntry::NativeDestruct()
{
	_listViewRewardSet->ClearListItems();
	_rewardEntryPool->ReleaseAll();

	Super::NativeDestruct();
}

void UGsUIBattleArenaSeasonTierRewardItemEntry::SetData(const FGsSchemaBattleArenaTierSeasonReward* InData, GuildWarTierDataId InMyTierDataId)
{
	if (nullptr == InData || nullptr == InData->tierDataId.GetRow())
	{
		GSLOG(Error, TEXT("nullptr == InData || InData->tierDataId.GetRow()"));
		return;
	}

	if (const FGsSchemaBattleArenaTierData* schemaTierData = InData->tierDataId.GetRow())
	{
		_imgTierSymbol->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(schemaTierData->symbolPath));
		_textTierName->SetText(schemaTierData->name);

		FText textFormat;
		FGsBattleArenaHelper::FindText(TEXT("UI_Ranking_Tier"), textFormat);
		_textScore->SetText(FText::Format(textFormat, { schemaTierData->scroeRange.rankingGroupRangeMin }));
		_imgSelected->SetVisibility(schemaTierData->id == InMyTierDataId ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

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

bool UGsUIBattleArenaSeasonTierRewardItemEntry::IsMyTier() const
{
	return ESlateVisibility::Collapsed != _imgSelected->GetVisibility();
}