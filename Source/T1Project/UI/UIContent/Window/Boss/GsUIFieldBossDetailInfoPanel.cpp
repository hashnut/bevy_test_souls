#include "GsUIFieldBossDetailInfoPanel.h"
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
#include "UI/UIContent/Common/Icon/GsUIIconListViewEntry.h"
#include "UI/UIContent/Common/Entry/GsUIRewardItemIconListViewEntry.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Window/Boss/GsUIWindowBossInfo.h"
#include "UI/UIContent/Popup/Boss/GsUIPopupBossRankReward.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Image.h"
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
#include "UI/UIContent/Popup/Boss/GsBossInfoSpawnListViewEntry.h"
#include "Message/MessageParam/GsUIMessageParam.h"



void UGsUIFieldBossDetailInfoPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_worldMapButton->OnClicked.AddDynamic(this, &UGsUIFieldBossDetailInfoPanel::OnClickWorldMap);
	_mailRewardButton->OnClicked.AddDynamic(this, &UGsUIFieldBossDetailInfoPanel::OnClickMailReward);	
}

void UGsUIFieldBossDetailInfoPanel::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(FIELD_BOSS_TEXT_KEY, UI_TEXT_NPERCENTINBRACKETS, _ui_text_nPercentInBrackets);
	FText::FindText(FIELD_BOSS_TEXT_KEY, UI_TEXT_MONTH_DAY_HHMM, _ui_text_monthDayHHMM);
	FText::FindText(FIELD_BOSS_TEXT_KEY, UI_TEXT_NO_RESULT, _ui_text_noResult);

	//bind message
	if (FGsMessageHolder* msg = GMessage())
	{
		_uiDelegates.Emplace(msg->GetUI().AddUObject(MessageUI::INVALID_FEILD_BOSS_DEATH_TIME
			, this, &UGsUIFieldBossDetailInfoPanel::OnInvalidLastDeathTime));
	}
}

void UGsUIFieldBossDetailInfoPanel::NativeDestruct()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_uiDelegates.Num() > 0)
		{
			for (auto iter : _uiDelegates)
			{
				msg->GetUI().Remove(iter);
			}
			_uiDelegates.Empty();
		}
	}

	Super::NativeDestruct();
}

void UGsUIFieldBossDetailInfoPanel::SetFieldBossInfo(const FGsSchemaFieldBossData* inBossInfo
	, const FGsSchemaFieldBossData* inMutant
	, bool inShowMutant
	, time_t inTime)
{	
	Clear();

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

void UGsUIFieldBossDetailInfoPanel::OnChangeWeek(int32 inWeek)
{
	_weekIndex = (EDayOfWeek)inWeek;
}

void UGsUIFieldBossDetailInfoPanel::OnClickWorldMap()
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
		|| nullptr == _bossData->mapId.GetRow() 
		|| nullptr == _bossData->bossNpcId.GetRow())
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
				// 월드맵 개선
				// 필드보스에서 월드맵 이동시
				// 몬스터 탭에서 선택되어지게 처리
				// https://jira.com2us.com/jira/browse/CHR-23756
				regionMap->SetMapChangeType(EGsWorldMapChangeType::TO_REGIONMAP_DUNGEON_MONSTERINF0, _bossData->mapId.GetRow()->id, 0, 
					_bossData->bossNpcId.GetRow()->id);
			}
		}
	}

#if ACTIVE_COOPERATIVE_BOSS
	//필드 보스가 던전형 컨텐츠의 필드 보스라면 던전으로 이동
	if (MapType::PUBLIC_DUNGEON == _bossData->mapId.GetRow()->mapType)
	{
		FGsMessageHolder* MessageHolder = GMessage();
		if (nullptr == MessageHolder)
		{
			GSLOG(Warning, TEXT("nullptr == MessageHolder"));
			return;
		}

		FGsUIMsgShortcutMap Param(_bossData->mapId.GetRow()->id);
		MessageHolder->GetUI().SendMessage(MessageUI::REQUEST_SHORTCUT_CONTENTS, &Param);
	}
	//필드 보스면 월드맵에서 필드 보스 맵 보여준다.
	else
	{
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
	}	

	FGsGameStateBoss::GetInstance()->SaveSelectBoss();
#else
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
#endif	
}

void UGsUIFieldBossDetailInfoPanel::OnClickMailReward()
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

void UGsUIFieldBossDetailInfoPanel::InvalidBossDetailInfo()
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

void UGsUIFieldBossDetailInfoPanel::InvalidSpawn()
{
	_spawnTimeWidgetSwitcher->SetActiveWidgetIndex((int32)EGsBossTab::WorldBoss);

	UGsUIUtil::SetFieldBossCompetitionSpawnInfo(_bossData, _spawnIntervalTextBlock);

	if (_bossData == nullptr)
	{
		return;
	}
	// 1.고정 시간이면 등장 시간
	// 2.아니면 등장 간격
	// https://jira.com2us.com/jira/browse/CHR-23655

	FText findText;
	if (_bossData->makeCron.IsEmpty() == true)
	{
		// 등장 간격
		FText::FindText(TEXT("FieldBossText"), TEXT("UI_Text_SpawnInterval"), findText);
	}
	else
	{
		// 등장 시간
		FText::FindText(TEXT("FieldBossText"), TEXT("UI_Text_SpawnCron"), findText);
	}

	_textSpawnTimeType = findText;


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

void UGsUIFieldBossDetailInfoPanel::InvalidMailReward(const FGsSchemaFieldBossData* inBossData)
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

void UGsUIFieldBossDetailInfoPanel::InvalidDropReward(const FGsSchemaFieldBossData* inBossData)
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

			if (nullptr == sourceItem)
			{
				return true;
			}

			if (nullptr == destItem)
			{
				return false;
			}

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

		// 필드 보스 아이템 갯수 표현 추가
		// https://jira.com2us.com/jira/browse/C2URWQ-8150
		if(itemBag != nullptr)
		{ 
			rewardIconData.Init(itemBag, itemBag->itemCountMin, itemBag->itemCountMax);
		}
		else
		{
			rewardIconData.Init(itemBag);
		}
		listViewEntryData->_rewardItemDataArray.Add(rewardIconData);

		iconListViewEntryDataArray.Add(listViewEntryData);
	}

	_dropRewardListView->SetListItems(iconListViewEntryDataArray);
	_dropRewardListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_dropRewardListView->ScrollIndexIntoView(0);
}

void UGsUIFieldBossDetailInfoPanel::InvalidNpcInfo()
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

void UGsUIFieldBossDetailInfoPanel::InvalidChannel()
{
	if (_bossNpcData == nullptr)
	{
		return;
	}

	_channelTextBlock->SetText(UGsGameObjectUtil::GetChannelText(_bossData->spawnChannelType));
}

void UGsUIFieldBossDetailInfoPanel::Clear()
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

	_spawnIntervalTextBlock->SetText(FText::GetEmpty());
	_lastKillTimeTextBlock->SetText(FText::GetEmpty());

	_spawnTimeListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_mailRewardListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_dropRewardListView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
}

void UGsUIFieldBossDetailInfoPanel::OnInvalidLastDeathTime(const struct IGsMessageParam* inParam)
{
	const FGsUIMsgParamFieldBossDeathTime* param = inParam->Cast<const FGsUIMsgParamFieldBossDeathTime>();
	if (nullptr == param)
	{
		return;
	}

	_lastDeadTime = param->_fieldBossLastDeadTime;

	InvalidSpawn();
}

void UGsUIFieldBossDetailInfoPanel::SetWeek(int32 inWeek)
{
	_weekIndex = (EDayOfWeek)inWeek;

	InvalidSpawn();
}

const FGsSchemaFieldBossData* UGsUIFieldBossDetailInfoPanel::GetBossData()
{
	return _bossData;
}

const FGsSchemaNpcData* UGsUIFieldBossDetailInfoPanel::GetBossNpcData()
{
	return _bossNpcData;
}

void UGsUIFieldBossDetailInfoPanel::SetShowMutant(bool inIsShowMutant)
{
	_isShowMutant = inIsShowMutant;

	InvalidBossDetailInfo();
}
