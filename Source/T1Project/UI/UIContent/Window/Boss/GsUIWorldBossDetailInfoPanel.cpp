#include "GsUIWorldBossDetailInfoPanel.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UTIL/GsUIUtil.h"
#include "UTIL/GsGameObjectUtil.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UI/UIContent/Popup/Boss/GsBossInfoSpawnListViewEntry.h"
#include "UI/UIContent/Common/Entry/GsUIRewardItemIconListViewEntry.h"
#include "Reward/GsRewardHelper.h"
#include "Components/ListView.h"
#include "DataSchema/Item/GsSchemaItemData.h"
#include "UTIL/GsTableUtil.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"

void UGsUIWorldBossDetailInfoPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_toolTipButton->OnPressed.AddDynamic(this, &UGsUIWorldBossDetailInfoPanel::OnClickToolTip);
	
}

void UGsUIWorldBossDetailInfoPanel::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(FIELD_BOSS_TEXT_KEY, UI_TEXT_NPERCENTINBRACKETS, _ui_text_nPercentInBrackets);
	FText::FindText(FIELD_BOSS_TEXT_KEY, UI_TEXT_MONTH_DAY_HHMM, _ui_text_monthDayHHMM);
	FText::FindText(FIELD_BOSS_TEXT_KEY, UI_TEXT_NO_RESULT, _ui_text_noResult);
	FText::FindText(TEXT("WorldBossText"), TEXT("Worldboss_EarnableRewardsCount"), _ui_text_leftRewardCount);
	FText::FindText(TEXT("WorldBossText"), TEXT("Worldboss_EarnRewardsTooltip"), _ui_text_toolTipText);

	_toolTipPanel->SetDescText(_ui_text_toolTipText);
	_toolTipPanel->Close();
}

void UGsUIWorldBossDetailInfoPanel::NativeDestruct()
{
	_toolTipPanel->Close();
}

void UGsUIWorldBossDetailInfoPanel::SetWorldBossInfo(const struct FGsSchemaWorldBossData* inWorldBossData, int32 inLeftRewardCount)
{
	if (nullptr == inWorldBossData)
	{
		return;
	}

	_toolTipPanel->Close();

	_worldBossData = inWorldBossData;
	_bossNpcData = inWorldBossData->bossNpcId.GetRow();
	_weekIndex = EDayOfWeek::Monday;	
	_leftRewardCount = inLeftRewardCount;

	InvalidWorldBossDetailInfo();
}

void UGsUIWorldBossDetailInfoPanel::OnChangeWeek(int32 inWeek)
{
	_weekIndex = (EDayOfWeek)inWeek;
}

void UGsUIWorldBossDetailInfoPanel::OnClickToolTip()
{
	_toolTipPanel->ToggleOpenClose();
}

void UGsUIWorldBossDetailInfoPanel::InvalidWorldBossDetailInfo()
{
	InvalidNpcInfo();
	InvalidSpawn(_worldBossData);
	InvalidParticipantReward(_worldBossData);
	InvalidRankReward(_worldBossData);
	InvalidLastHitReward(_worldBossData);
	InvalidLeftRewardCount();
}

void UGsUIWorldBossDetailInfoPanel::InvalidSpawn(const struct FGsSchemaWorldBossData* inBossData)
{
	//월드 보스	
	if (nullptr == inBossData)
	{
		return;
	}

	TArray<UGsBossInfoSpawnListViewEntryData*> spawnListViewEntryDataArray;
	UGsBossInfoSpawnListViewEntryData* spawnListViewEntryData;

	spawnListViewEntryData = NewObject<UGsBossInfoSpawnListViewEntryData>();
	spawnListViewEntryData->_crone = inBossData->spawnTimeCron;
	spawnListViewEntryDataArray.Add(spawnListViewEntryData);

	_spawnTimeListView->SetListItems(spawnListViewEntryDataArray);
	_spawnTimeListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
}

void UGsUIWorldBossDetailInfoPanel::InvalidParticipantReward(const FGsSchemaWorldBossData* inBossData)
{
	InvalidRewardData(inBossData->basicRewardIdList, _participantRewardListView);
}

void UGsUIWorldBossDetailInfoPanel::InvalidRankReward(const FGsSchemaWorldBossData* inBossData)
{
	TArray<FGsSchemaRewardDataRow> rewardDataRowSet;
	for (const auto& rewardByRankRow : inBossData->rankRewardIdList)
	{
		if (const FGsSchemaRewardByRankData* rewardRankData = rewardByRankRow.GetRow())
		{
			rewardDataRowSet.Emplace(rewardRankData->rewardDataRow);
		}
	}
	
	InvalidRewardData(rewardDataRowSet, _rankRewardListView);
}

void UGsUIWorldBossDetailInfoPanel::InvalidLastHitReward(const struct FGsSchemaWorldBossData* inBossData)
{
	InvalidRewardData(inBossData->lastHitRewardIdList, _lastHitRewardListView);
}

void UGsUIWorldBossDetailInfoPanel::InvalidRewardData(const TArray<FGsSchemaRewardDataRow>& inRewardList, class UListView* inRewardListView)
{
	FGsRewardUnboxingData totalUnboxingData;
	for (const FGsSchemaRewardDataRow& rewardData : inRewardList)
	{
		if (nullptr == rewardData.GetRow())
		{
			continue;
		}

		FGsRewardUnboxingData unboxingData;
		FGsRewardHelper::UnboxingRewardData(rewardData.GetRow(), unboxingData);

		totalUnboxingData.AddData(unboxingData);
	}

	totalUnboxingData.ItemSort();

	UGsUIRewardIconListViewEntryData* listViewEntryData = nullptr;
	TArray<UGsUIRewardIconListViewEntryData*> iconListViewEntryDataArray;
	FGsUIRewardIconData rewardData;
	for (const FGsSchemaRewardItemBagData* itemBag : totalUnboxingData._itemDataList)
	{
		listViewEntryData = NewObject<UGsUIRewardIconListViewEntryData>();

		if (CurrencyType::NONE == itemBag->currencyType)
		{
			rewardData.Init(itemBag, itemBag->itemCountMin, itemBag->itemCountMax);
		}
		else
		{
			rewardData.Init(itemBag, itemBag->currencyAmountMin, itemBag->currencyAmountMax);
		}

		listViewEntryData->_rewardItemDataArray.Add(rewardData);

		iconListViewEntryDataArray.Add(listViewEntryData);
	}

	inRewardListView->SetListItems(iconListViewEntryDataArray);
	inRewardListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
}

void UGsUIWorldBossDetailInfoPanel::InvalidNpcInfo()
{
	if (_bossNpcData == nullptr)
	{
		return;
	}

	FText materialText = UGsGameObjectUtil::GetMaterialText(_bossNpcData->defenseMaterial);
	_materialTextBlock->SetText(materialText);
	FText raceText = UGsGameObjectUtil::GetRaceText(_bossNpcData->race);
	_raceTextBlock->SetText(raceText);
	FText elementText = UGsGameObjectUtil::GetMonsterElementalText(_bossNpcData->element);
	_traitTextBlock->SetText(elementText);
}

void UGsUIWorldBossDetailInfoPanel::InvalidLeftRewardCount()
{	
	_leftRewardCountTextBlock->SetText(FText::Format(_ui_text_leftRewardCount, _leftRewardCount));
	if (0 == _leftRewardCount)
	{
		_rewardCompletePanel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		_rewardCompletePanel->SetVisibility(ESlateVisibility::Collapsed);
	}	
}

void UGsUIWorldBossDetailInfoPanel::Clear(EGsBossTab inTab)
{
	_raceTextBlock->SetText(FText::GetEmpty());
	_materialTextBlock->SetText(FText::GetEmpty());
	_traitTextBlock->SetText(FText::GetEmpty());

	_spawnIntervalTextBlock->SetText(FText::GetEmpty());
	_lastKillTimeTextBlock->SetText(FText::GetEmpty());
}

void UGsUIWorldBossDetailInfoPanel::SetWeek(int32 inWeek)
{
	_weekIndex = (EDayOfWeek)inWeek;

	InvalidSpawn(_worldBossData);
}

const FGsSchemaWorldBossData* UGsUIWorldBossDetailInfoPanel::GetWorldBossData()
{
	return _worldBossData;
}

const FGsSchemaNpcData* UGsUIWorldBossDetailInfoPanel::GetBossNpcData()
{
	return _bossNpcData;
}

void UGsUIWorldBossDetailInfoPanel::SetParents(TWeakObjectPtr<UGsUIWindowBossInfo> inParents)
{
	_parent = inParents;
}

