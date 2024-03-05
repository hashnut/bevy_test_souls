#include "GsUIItemDropListContainer.h"

#include "Runtime/UMG/Public/Components/ListView.h"
#include "Runtime/UMG/Public/Components/ScrollBox.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "Classes/GsMessageHandlerOneParam.h"
#include "Classes/GsPoolUObject.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/WorldBoss/GsSchemaWorldBossData.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateBoss.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GsUICraftShortCutListEntry.h"
#include "Item/GsItem.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsCraftMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Popup/ItemDropNavigator/GsUIItemDropElement.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UTIL/GsGameObjectUtil.h"
#include "UTIL/GsLevelUtil.h"

void UGsUIItemDropListContainer::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FText::FindText(TEXT("WorldMapText"), TEXT("MonLevel"), _levelFormat);

	_listItemPools.Add(NewObject<UGsPoolUObject>(this));
	_listItemPools.Add(NewObject<UGsPoolUObject>(this));

	// 타입별 리스트뷰 할당
	_listViewMap.Emplace(EGsListItemType::Drop, _dropListView);
	_listViewMap.Emplace(EGsListItemType::Craft, _craftListView);
}

void UGsUIItemDropListContainer::SetParameters(FSimpleDelegate InOnClickListEntry, TArray<TPair<MapId, CreatureId>> InDropMapCreatures)
{
	_onClickListEntry = MoveTemp(InOnClickListEntry);

	constexpr EGsListItemType ListItemType = EGsListItemType::Drop;
	TArray<UObject*>& ListItems = _listItemMap.FindOrAdd(ListItemType);
	ListItems.Empty();

	const FGsWorldMapManager* WorldMapManager = GSWorldMap();
	if (nullptr == WorldMapManager)
	{
		GSLOG(Error, TEXT("nullptr == WorldMapManager"));
		return;
	}

	for (const TPair<MapId, CreatureId>& MapCreaturePair : InDropMapCreatures)
	{
		const MapId CurrentMapId = MapCreaturePair.Key;
		const CreatureId CurrentNpcId = MapCreaturePair.Value;

		UGsUIItemDropElementListItem* ListItem = ClaimListItem<UGsUIItemDropElementListItem>(ListItemType);
		ListItem->_onClickMoveBtn.BindUObject(this, &UGsUIItemDropListContainer::OnClickedDropElementMoveBtn);
		ListItem->_mapId = CurrentMapId;
		ListItem->_creatureId = CurrentNpcId;

		ListItem->_continentalName = WorldMapManager->GetContinentalName(WorldMapManager->FindContinentalIdBy(CurrentMapId));
		ListItem->_mapName = FGsWorldMapManager::GetMapName(CurrentMapId);
		ListItem->_creatureName = FText::GetEmpty();
		ListItem->_gradeText = FText::GetEmpty();

		ListItem->_offensiveText = FText::GetEmpty();
		ListItem->_levelText = FText::GetEmpty();

		ListItems.Add(ListItem);
	}

	if (0 >= ListItems.Num())
	{
		_widgetSwitcher->SetActiveWidgetIndex(StaticCast<uint8>(EGsListItemType::EmptyListItem));
	}
	else
	{
		_widgetSwitcher->SetActiveWidgetIndex(StaticCast<uint8>(ListItemType));
	}

	_listViewMap.FindRef(ListItemType)->SetListItems(ListItems);
}

void UGsUIItemDropListContainer::SetParameters(FSimpleDelegate InOnClickListEntry, const TArray<FGsSchemaWorldMapMonsterElementDataRow>& InDropMapCreatures)
{
	_onClickListEntry = MoveTemp(InOnClickListEntry);

	constexpr EGsListItemType ListItemType = EGsListItemType::Drop;
	TArray<UObject*>& ListItems = _listItemMap.FindOrAdd(ListItemType);
	ListItems.Empty();

	const FGsWorldMapManager* WorldMapManager = GSWorldMap();
	if (nullptr == WorldMapManager)
	{
		GSLOG(Error, TEXT("nullptr == WorldMapManager"));
		return;
	}

	for (const FGsSchemaWorldMapMonsterElementDataRow& WorldMapMonsterElementDataRow : InDropMapCreatures)
	{
		const FGsSchemaWorldMapMonsterElementData* WorldMapMonsterElementData = WorldMapMonsterElementDataRow.GetRow();
		if (nullptr == WorldMapMonsterElementData)
		{
			GSLOG(Error, TEXT("nullptr == WorldMapMonsterElementData"));
			continue;
		}

		const FGsSchemaNpcData* NpcData = WorldMapMonsterElementData->id.GetRow();
		if (nullptr == NpcData)
		{
			GSLOG(Error, TEXT("nullptr == NpcData"));
			continue;
		}

		const MapId CurrentMapId = WorldMapMonsterElementData->mapId;

		UGsUIItemDropElementListItem* ListItem = ClaimListItem<UGsUIItemDropElementListItem>(ListItemType);
		ListItem->_onClickMoveBtn.BindUObject(this, &UGsUIItemDropListContainer::OnClickedDropElementMoveBtn);
		ListItem->_mapId = CurrentMapId;
		ListItem->_creatureId = NpcData->id;
		ListItem->_level = WorldMapMonsterElementData->level;
		
		ListItem->_continentalName = WorldMapManager->GetContinentalName(WorldMapManager->FindContinentalIdBy(CurrentMapId));
		ListItem->_mapName = FGsWorldMapManager::GetMapName(CurrentMapId);
		if (MapType::PUBLIC_FIELD == FGsWorldMapManager::GetMapType(CurrentMapId) 
			|| MapType::INTER_FIELD == FGsWorldMapManager::GetMapType(CurrentMapId))
		{
			ListItem->_mapNameColor = FGsUIColorHelper::GetColor(EGsUIColorType::MAPNAME_PUBLIC_FIELD);
		}
		else
		{
			ListItem->_mapNameColor = FGsUIColorHelper::GetColor(EGsUIColorType::MAPNAME_PUBLIC_DUNGEON);
		}

		ListItem->_creatureName = NpcData->nameText;
		ListItem->_gradeText = UGsGameObjectUtil::GetRankText(NpcData->grade);
		ListItem->_offensiveText = UGsGameObjectUtil::GetOffensiveText(WorldMapMonsterElementData->isOffensive);
		ListItem->_levelText = FText::Format(_levelFormat, WorldMapMonsterElementData->level);

		ListItems.Add(ListItem);
	}

	if (0 >= ListItems.Num())
	{
		_widgetSwitcher->SetActiveWidgetIndex(StaticCast<uint8>(EGsListItemType::EmptyListItem));
	}
	else
	{
		_widgetSwitcher->SetActiveWidgetIndex(StaticCast<uint8>(ListItemType));
	}

	_listViewMap.FindRef(ListItemType)->SetListItems(ListItems);
}

void UGsUIItemDropListContainer::SetParameters(FSimpleDelegate InOnClickListEntry, const TArray<CraftId>& InCraftIds)
{
	_onClickListEntry = MoveTemp(InOnClickListEntry);

	constexpr EGsListItemType ListItemType = EGsListItemType::Craft;

	TArray<UObject*>& ListItems = _listItemMap.FindOrAdd(ListItemType);

	const int32 ListCount = InCraftIds.Num();
	ListItems.Empty(ListCount);
	for (const CraftId& CurrentCraftId : InCraftIds)
	{
		UGsUICraftShortCutListItem* ListItem = ClaimListItem<UGsUICraftShortCutListItem>(ListItemType);
		ListItem->_onClickMoveBtn.BindUObject(this, &UGsUIItemDropListContainer::OnClickedCraftShortCutMoveBtn);
		ListItem->_craftId = CurrentCraftId;
		ListItems.Add(ListItem);
	}

	if (0 >= ListItems.Num())
	{
		_widgetSwitcher->SetActiveWidgetIndex(StaticCast<uint8>(EGsListItemType::EmptyListItem));
	}
	else
	{
		_widgetSwitcher->SetActiveWidgetIndex(StaticCast<uint8>(ListItemType));
	}

	_listViewMap.FindRef(ListItemType)->SetListItems(ListItems);
}

void UGsUIItemDropListContainer::OnClickedCraftShortCutMoveBtn(const UGsUICraftShortCutListItem& InListItem)
{
	// 본 팝업은 아이템상세팝업을 통해 오픈 되는 경우가 있으므로, 본 창을 닫을 때, 아이템상세팝업도 같이 닫음.
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		MessageHolder->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, nullptr);
	}

	_onClickListEntry.ExecuteIfBound();

	FGsCraftMessageParamOpenCraftWith(InListItem._craftId).Send();
}

void UGsUIItemDropListContainer::OnClickedDropElementMoveBtn(const UGsUIItemDropElementListItem& InListItem)
{
	// 본 팝업은 아이템상세팝업을 통해 오픈 되는 경우가 있으므로, 본 창을 닫을 때, 아이템상세팝업도 같이 닫음.
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		MessageHolder->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, nullptr);
	}

	_onClickListEntry.ExecuteIfBound();

	UGsUIItemDropListContainer::TryToNavigate(InListItem._mapId, InListItem._creatureId, InListItem._level);
}

FGsGameStateRegionMap* UGsUIItemDropListContainer::GetGameStateRegionMap()
{
	FGsGameFlowGame* GameFlowGame = GMode()->GetGameFlow();
	if (nullptr == GameFlowGame)
	{
		GSLOG(Warning, TEXT("nullptr == GameFlowGame"));
		return nullptr;
	}

	FGsContentsManagerGame* ContentsManagerGame = GameFlowGame->GetContentsManagerGame();
	if (nullptr == ContentsManagerGame)
	{
		GSLOG(Warning, TEXT("nullptr == ContentsManagerGame"));
		return nullptr;
	}

	FGsGameStateRegionMap* GameStateRegionMap = ContentsManagerGame->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap);
	if (nullptr == GameStateRegionMap)
	{
		GSLOG(Warning, TEXT("nullptr == GameStateRegionMap"));
		return nullptr;
	}

	return GameStateRegionMap;
}

void UGsUIItemDropListContainer::TryToNavigate(const MapId InMapId, const CreatureId InCreatureId, const Level InLevel)
{
	if (INVALID_CREATURE_ID == InCreatureId)
	{
		UGsUIItemDropListContainer::TryToNavigate(InMapId);
	}
	else
	{
		// [B1] | ejrrb10 | 일반 필드일 경우 RegionMap 을, 던전일 경우 Dungeon 을 보여준다.
		const FGsSchemaMapData* MapData = nullptr;
		const UGsTableMapData* MapDataTable = Cast<UGsTableMapData>(FGsSchemaMapData::GetStaticTable());
		if (nullptr == MapDataTable || !MapDataTable->FindRowById(InMapId, MapData) || nullptr == MapData)
		{
			return;
		}

		switch (MapData->mapType)
		{
		case MapType::PUBLIC_FIELD:
		case MapType::INTER_FIELD:
		// 월드맵개선
		// 던전에서 드랍 아이템 획득처 보여주게 처리
		// https://jira.com2us.com/jira/browse/CHR-23756
		case MapType::ELITE_DUNGEON_1:
		case MapType::PUBLIC_DUNGEON:
		// 그림자 전장도 던전처럼 표시
		//https://jira.com2us.com/jira/browse/C2URWQ-7778
		case MapType::GUILD_WAR:
		{
			FGsGameStateRegionMap* GameStateRegionMap = UGsUIItemDropListContainer::GetGameStateRegionMap();
			if (nullptr == GameStateRegionMap)
			{
				GSLOG(Warning, TEXT("nullptr == GameStateRegionMap"));
				return;
			}

			GameStateRegionMap->ShowDropItemMap(InMapId, InCreatureId, InLevel, EGsToggleType::Active);
			UGsUIManager* uiMgr = GUI();
			if (uiMgr == nullptr)
			{
				return;
			}
			// 몬스터 정보 팝업을 닫자
			uiMgr->CloseByKeyName(TEXT("PopupMonsterInfo"));
		}
		break;

		// 던전 페이지 열어 주기
		/*case MapType::ELITE_DUNGEON_1:
		case MapType::PUBLIC_DUNGEON:
		{
			FGsMessageHolder* MessageHolder = GMessage();
			if (nullptr == MessageHolder)
			{
				GSLOG(Warning, TEXT("nullptr == MessageHolder"));
				return;
			}

			FGsUIMsgShortcutMap Param(InMapId);
			MessageHolder->GetUI().SendMessage(MessageUI::REQUEST_SHORTCUT_CONTENTS, &Param);
		}
		break;*/

		// 월드 보스 열어 주기
		case MapType::WORLD_BOSS:
		{
			FGsGameStateBoss::GetInstance()->OpenSelectedWorldBoss(InMapId, InCreatureId);
		}
		break;

		default:
			GSLOG(Error, TEXT("Unknown mapType: %d, InMapId:%d"), MapData->mapType, InMapId);
			break;
		}		
	}
}

bool UGsUIItemDropListContainer::TryToNavigate(const MapId InMapId)
{
	const FGsSchemaMapData* MapData = nullptr;
	const UGsTableMapData* MapDataTable = Cast<UGsTableMapData>(FGsSchemaMapData::GetStaticTable());
	if (nullptr == MapDataTable || !MapDataTable->FindRowById(InMapId, MapData) || nullptr == MapData)
	{
		return false;
	}

	switch (MapData->mapType)
	{
	case MapType::PUBLIC_FIELD:
	{
		FGsGameStateRegionMap* GameStateRegionMap = UGsUIItemDropListContainer::GetGameStateRegionMap();
		if (nullptr == GameStateRegionMap)
		{
			GSLOG(Warning, TEXT("nullptr == GameStateRegionMap"));
			return false;
		}

		if (!GameStateRegionMap->ShowRegionMapAndMovePopup(InMapId))
		{
			GSLOG(Warning, TEXT("!GameStateRegionMap->ShowRegionMapAndMovePopup(%d)"), InMapId);
			return false;
		}
	}
	break;

	case MapType::SINGLE_FIELD:
		GSLOG(Warning, TEXT("check MapType::SINGLE_FIELD %d"), InMapId);
		break;

	case MapType::INSTANCE_SINGLE:
		GSLOG(Warning, TEXT("check MapType::INSTANCE_SINGLE %d"), InMapId);
		break;

	case MapType::INSTANCE_FIELD:
		GSLOG(Warning, TEXT("check MapType::INSTANCE_FIELD %d"), InMapId);
		break;

	case MapType::ELITE_DUNGEON_1:
	case MapType::PUBLIC_DUNGEON:
	{
		FGsMessageHolder* MessageHolder = GMessage();
		if (nullptr == MessageHolder)
		{
			GSLOG(Warning, TEXT("nullptr == MessageHolder"));
			return false;
		}

		FGsUIMsgShortcutMap Param(InMapId);
		MessageHolder->GetUI().SendMessage(MessageUI::REQUEST_SHORTCUT_CONTENTS, &Param);
	}
	break;

	default:
		GSLOG(Error, TEXT("Unknown mapType: %d, InMapId:%d"), MapData->mapType, InMapId);
		break;
	}

	return true;
}
