#include "GsUIBossDetailInfoPanel.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/ListView.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "Components/VerticalBox.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "GsBossInfoWeekListViewEntry.h"
#include "UI/UIContent/Common/Icon/GsUIIconListViewEntry.h"
#include "UI/UIContent/Common/Entry/GsUIRewardItemIconListViewEntry.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Window/Boss/GsUIWindowBossInfo.h"
#include "UI/UIContent/Popup/Boss/GsUIPopupBossRankReward.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Image.h"
#include "GsBossInfoSpawnListViewEntry.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateBoss.h"
#include "Map/WorldMap/GsWorldMapDefine.h"
#include "Map/WorldMap/GsWorldMapFunc.h"
#include "Message/GsMessageContents.h"

#include "UTIL/GsGameObjectUtil.h"
#include "UTIL/GsCronUtil.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "UTIL/GsUIUtil.h"

#include "Management/GsMessageHolder.h"

void UGsUIBossDetailInfoPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_worldMapButton->OnClicked.AddDynamic(this, &UGsUIBossDetailInfoPanel::OnClickWorldMap);
	_mailRewardButton->OnClicked.AddDynamic(this, &UGsUIBossDetailInfoPanel::OnClickMailReward);
	_dropRewardButton->OnClicked.AddDynamic(this, &UGsUIBossDetailInfoPanel::OnClickDropReward);
}

void UGsUIBossDetailInfoPanel::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(FIELD_BOSS_TEXT_KEY, UI_TEXT_NPERCENTINBRACKETS, _ui_text_nPercentInBrackets);
	FText::FindText(FIELD_BOSS_TEXT_KEY, UI_TEXT_MONTH_DAY_HHMM, _ui_text_monthDayHHMM);
	FText::FindText(FIELD_BOSS_TEXT_KEY, UI_TEXT_NO_RESULT, _ui_text_noResult);
}

void UGsUIBossDetailInfoPanel::SetBossInfo(EGsBossTab inBossTab
	, const FGsSchemaFieldBossData* inBossInfo
	, const FGsSchemaFieldBossData* inMutant
	, bool inShowMutant
	, time_t inTime)
{
	_bossTab = inBossTab;
	Clear(inBossTab);

	if (nullptr == inBossInfo)
	{
		return;
	}

	_bossData = inBossInfo;
	_bossNpcData = inBossInfo->bossNpcId.GetRow();
	_bossMutant = inMutant;
	_weekIndex = EDayOfWeek::Monday;
	_lastDeadTime = inTime;
	_isShowMutant = inShowMutant;

	InvalidBossDetailInfo();
}

void UGsUIBossDetailInfoPanel::OnChangeWeek(int32 inWeek)
{
	_weekIndex = (EDayOfWeek)inWeek;
}

void UGsUIBossDetailInfoPanel::OnClickWorldMap()
{
	//if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::WORLDMAP))
	//{
	//	GSUnlock()->ShowLockTicker(EGsUnlockContentsType::WORLDMAP);
	//	return;
	//}

	// 언락 컨텐츠와 가능 타입 맵, 관전 모드 체크 한번에 하자
	if (FGsWorldMapFunc::IsRegionMapOpenPossible() == false)
	{
		return;
	}

	if (nullptr == _bossData
		|| nullptr == _bossData->mapId.GetRow())
	{
		return;
	}
	
	if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
	{
		if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
		{
			if (FGsGameStateRegionMap* regionMap =
				contents->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap))
			{
				regionMap->SetMapChangeType(EGsWorldMapChangeType::TO_REGIONMAP_CLICK_ITEM, _bossData->mapId.GetRow()->id);
			}
		}
	}

	if (GUI()->IsActiveWidget(TEXT("WindowRegionMap")))
	{
		// 지역맵이 켜져있다면 변경
		GMessage()->GetUI().SendMessage(MessageUI::CHANGE_REGION_MAP_SCENE, nullptr);
	}
	else
	{
		// 꺼져 있다면 지역맵 열기
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_REGIONMAP);
	}

	FGsGameStateBoss::GetInstance()->SaveSelectBoss();
}

void UGsUIBossDetailInfoPanel::OnClickMailReward()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(POPUP_BOSS_REWARD);
	if (false == widget.IsValid())
	{
		return;
	}

	UGsUIPopupBossRankReward* rankRewardWidget = Cast<UGsUIPopupBossRankReward>(widget.Get());
	if (rankRewardWidget)
	{
		rankRewardWidget->InitReward(_bossData, _bossNpcData, BossRewardGiveType::MAIL);
	}
}

void UGsUIBossDetailInfoPanel::OnClickDropReward()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(POPUP_BOSS_REWARD);
	if (false == widget.IsValid())
	{
		return;
	}

	UGsUIPopupBossRankReward* rankRewardWidget = Cast<UGsUIPopupBossRankReward>(widget.Get());
	if (rankRewardWidget)
	{
		rankRewardWidget->InitReward(_bossData, _bossNpcData, BossRewardGiveType::FILED_DROP);
	}
}

void UGsUIBossDetailInfoPanel::InvalidBossDetailInfo()
{
	const FGsSchemaFieldBossData* bossData = _bossData;
	if (_isShowMutant)
	{
		bossData = _bossMutant;
	}

	if (nullptr == bossData)
	{
		return;
	}

	//get boss npc id
	{
		_bossNameTextBlock->SetText(bossData->bossNpcId.GetRow()->nameText);

		//set boss icon
		UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(_bossNpcData->shapeId.GetRow()->imageRegionMap);
		_bossIcon->SetBrushFromAtlasInterface(spriteImg);
	}

	//get area name
	{
		FText areaName = _bossData->mapId.GetRow()->MapStringId;
		_areaNameTextBlock->SetText(areaName);
	}

	InvalidSpawn();
	InvalidDropReward(bossData);
	InvalidMailReward(bossData);
	InvalidNpcInfo();
	InvalidChannel();
}

void UGsUIBossDetailInfoPanel::InvalidSpawn()
{	
	_spawnTimeWidgetSwitcher->SetActiveWidgetIndex((int32)EGsBossTab::WorldBoss);

	UGsUIUtil::SetFieldBossCompetitionSpawnInfo(_bossData, _spawnIntervalTextBlock);

	if (0 == _lastDeadTime)
	{
		_lastKillTimeTextBlock->SetText(_ui_text_noResult);
	}
	else
	{
		// 2022/10/21 PKT - 서버 시 -> 클라이언트(로컬) 시
		FDateTime lastKillTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_lastDeadTime);

		FString hour = FString::Printf(TEXT("%02d"), lastKillTime.GetHour());
		FString minut = FString::Printf(TEXT("%02d"), lastKillTime.GetMinute());
		_lastKillTimeTextBlock->SetText(FText::Format(_ui_text_monthDayHHMM
			, FText::FromString(FString::FromInt(lastKillTime.GetMonth()))
			, FText::FromString(FString::FromInt(lastKillTime.GetDay()))
			, FText::FromString(hour)
			, FText::FromString(minut)));

#if FIELD_BOSS_DEBUG
		GSLOG(Error, TEXT("%s"), *lastKillTime.ToString());
#endif
	}
}

void UGsUIBossDetailInfoPanel::InvalidMailReward(const FGsSchemaFieldBossData* inBossData)
{
	if (nullptr == inBossData)
	{
		return;
	}

	int32 num = inBossData->bossRewardIdList.Num();
	TArray<FGsSchemaFieldBossData> rewardBoxList;
	TArray<const FGsSchemaRewardItemBagData*> rewardItemBagDataArray;
	const FGsSchemaFieldBossRewardData* fieldBossData = nullptr;
	const FGsSchemaRewardData* rewardData = nullptr;
	for (int32 i = 0; i < num; ++i)
	{
		fieldBossData = inBossData->bossRewardIdList[i].GetRow();
		if (nullptr == fieldBossData)
		{
			continue;
		}

		if (fieldBossData->giveType != BossRewardGiveType::MAIL)
		{
			continue;
		}

		rewardData = fieldBossData->rewardId.GetRow();
		if (nullptr == rewardData)
		{
			continue;
		}

		for (const FGsSchemaRewardBox& rewardBox : rewardData->rewardBoxList)
		{
			const FGsSchemaRewardBoxData* rewardBoxData = rewardBox.rewardBoxDataRow.GetRow();
			if (nullptr == rewardBoxData)
			{
				continue;
			}

			for (const FGsSchemaRewardItemBagData& itemBagData : rewardBoxData->itemBagList)
			{
				const FGsSchemaRewardItemBagData** find = rewardItemBagDataArray.FindByPredicate([&](const FGsSchemaRewardItemBagData* source)
					{
						return (source->itemId == itemBagData.itemId);
					});

				if (nullptr != find)
				{
					continue;
				}

				rewardItemBagDataArray.Add(&itemBagData);
			}
		}
	}

	if (rewardItemBagDataArray.Num() == 0)
	{
		_mailRewardPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_mailRewardPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	const UGsTableItemCommon* itemTable = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	const FGsSchemaItemCommon* sourceItem = nullptr;
	const FGsSchemaItemCommon* destItem = nullptr;
	rewardItemBagDataArray.Sort([&](const FGsSchemaRewardItemBagData& source, const FGsSchemaRewardItemBagData& dest)->bool {
		if (itemTable)
		{

			itemTable->FindRowById(source.itemId, sourceItem);
			itemTable->FindRowById(dest.itemId, destItem);

			if ((int32)sourceItem->grade > (int32)destItem->grade)
			{
				return true;
			}
			else if ((int32)sourceItem->grade == (int32)destItem->grade)
			{
				return source.itemId > dest.itemId;
			}
		}

		return false;
		});

	TArray<UGsUIRewardIconListViewEntryData*> iconListViewEntryDataArray;
	UGsUIRewardIconListViewEntryData* listViewEntryData = nullptr;
	FGsUIRewardIconData rewardIconData;
	for (const FGsSchemaRewardItemBagData* itemBag : rewardItemBagDataArray)
	{
		listViewEntryData = NewObject<UGsUIRewardIconListViewEntryData>();

		rewardIconData.Init(itemBag);
		listViewEntryData->_rewardItemDataArray.Add(rewardIconData);

		iconListViewEntryDataArray.Add(listViewEntryData);
	}

	_mailRewardListView->SetListItems(iconListViewEntryDataArray);
	_mailRewardListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_mailRewardListView->ScrollIndexIntoView(0);
}

void UGsUIBossDetailInfoPanel::InvalidDropReward(const FGsSchemaFieldBossData* inBossData)
{
	if (nullptr == inBossData)
	{
		return;
	}

	int32 num = inBossData->bossRewardIdList.Num();
	TArray<FGsSchemaFieldBossData> rewardBoxList;
	TArray<const FGsSchemaRewardItemBagData*> rewardItemBagDataArray;
	const FGsSchemaFieldBossRewardData* fieldBossData = nullptr;
	const FGsSchemaRewardData* rewardData = nullptr;
	for (int32 i = 0; i < num; ++i)
	{
		fieldBossData = inBossData->bossRewardIdList[i].GetRow();
		if (nullptr == fieldBossData)
		{
			continue;
		}

		if (fieldBossData->giveType == BossRewardGiveType::MAIL)
		{
			continue;
		}

		rewardData = fieldBossData->rewardId.GetRow();
		if (nullptr == rewardData)
		{
			continue;
		}

		for (const FGsSchemaRewardBox& rewardBox : rewardData->rewardBoxList)
		{
			const FGsSchemaRewardBoxData* rewardBoxData = rewardBox.rewardBoxDataRow.GetRow();
			if (nullptr == rewardBoxData)
			{
				continue;
			}

			for (const FGsSchemaRewardItemBagData& itemBagData : rewardBoxData->itemBagList)
			{
				const FGsSchemaRewardItemBagData** find = rewardItemBagDataArray.FindByPredicate([&](const FGsSchemaRewardItemBagData* source)
					{
						return (source->itemId == itemBagData.itemId);
					});

				if (nullptr != find)
				{
					continue;
				}

				rewardItemBagDataArray.Add(&itemBagData);
			}
		}
	}

	if (rewardItemBagDataArray.Num() == 0)
	{
		_dropRewardPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_dropRewardPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	const UGsTableItemCommon* itemTable = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	const FGsSchemaItemCommon* sourceItem = nullptr;
	const FGsSchemaItemCommon* destItem = nullptr;
	rewardItemBagDataArray.Sort([&](const FGsSchemaRewardItemBagData& source, const FGsSchemaRewardItemBagData& dest)->bool {
		if (itemTable)
		{
			itemTable->FindRowById(source.itemId, sourceItem);
			itemTable->FindRowById(dest.itemId, destItem);

			if ((int32)sourceItem->grade > (int32)destItem->grade)
			{
				return true;
			}
			else if ((int32)sourceItem->grade == (int32)destItem->grade)
			{
				return source.itemId > dest.itemId;
			}
		}

		return false;
		});

	TArray<UGsUIRewardIconListViewEntryData*> iconListViewEntryDataArray;
	UGsUIRewardIconListViewEntryData* listViewEntryData = nullptr;
	FGsUIRewardIconData rewardIconData;
	for (const FGsSchemaRewardItemBagData* itemBag : rewardItemBagDataArray)
	{
		listViewEntryData = NewObject<UGsUIRewardIconListViewEntryData>();

		rewardIconData.Init(itemBag);
		listViewEntryData->_rewardItemDataArray.Add(rewardIconData);

		iconListViewEntryDataArray.Add(listViewEntryData);
	}

	_dropRewardListView->SetListItems(iconListViewEntryDataArray);
	_dropRewardListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_dropRewardListView->ScrollIndexIntoView(0);
}

void UGsUIBossDetailInfoPanel::InvalidNpcInfo()
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

void UGsUIBossDetailInfoPanel::InvalidChannel()
{
	if (_bossNpcData == nullptr)
	{
		return;
	}

	_channelTextBlock->SetText(UGsGameObjectUtil::GetChannelText(_bossData->spawnChannelType));
}

void UGsUIBossDetailInfoPanel::Clear(EGsBossTab inTab)
{
	_isShowMutant = false;
	_bossIcon->SetBrushFromAtlasInterface(nullptr);
	_bossNameTextBlock->SetText(FText::GetEmpty());
	_areaNameTextBlock->SetText(FText::GetEmpty());
	_raceTextBlock->SetText(FText::GetEmpty());
	_materialTextBlock->SetText(FText::GetEmpty());
	_traitTextBlock->SetText(FText::GetEmpty());
	_channelTextBlock->SetText(FText::GetEmpty());

	TArray<UGsBossInfoSpawnListViewEntryData*> spawnListViewEntryDataArray;
	_spawnTimeListView->SetListItems(spawnListViewEntryDataArray);
	_mailRewardPanel->SetVisibility(ESlateVisibility::Hidden);
	TArray<UGsUIRewardIconListViewEntryData*> iconListViewEntryDataArray;
	_dropRewardListView->SetListItems(iconListViewEntryDataArray);

	if (inTab == EGsBossTab::FieldBoss)
	{
		_spawnTimeWidgetSwitcher->SetActiveWidgetIndex((int32)EGsBossTab::FieldBoss);
	}
	else
	{
		_spawnTimeWidgetSwitcher->SetActiveWidgetIndex((int32)EGsBossTab::WorldBoss);
	}

	_spawnIntervalTextBlock->SetText(FText::GetEmpty());
	_lastKillTimeTextBlock->SetText(FText::GetEmpty());

	_spawnTimeListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_mailRewardListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_dropRewardListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
}

void UGsUIBossDetailInfoPanel::SetWeek(int32 inWeek)
{
	_weekIndex = (EDayOfWeek)inWeek;

	InvalidSpawn();
}

const FGsSchemaFieldBossData* UGsUIBossDetailInfoPanel::GetBossData()
{
	return _bossData;
}

const FGsSchemaNpcData* UGsUIBossDetailInfoPanel::GetBossNpcData()
{
	return _bossNpcData;
}

void UGsUIBossDetailInfoPanel::SetParents(TWeakObjectPtr<UGsUIWindowBossInfo> inParents)
{
	_parent = inParents;
}

void UGsUIBossDetailInfoPanel::SetShowMutant(bool inIsShowMutant)
{
	_isShowMutant = inIsShowMutant;

	InvalidBossDetailInfo();
}
