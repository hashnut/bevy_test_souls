// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRegionSceneItemDropEntry.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Components/TextBlock.h"
#include "UTIL/GsText.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapSpoilsMonsterLevel.h"
#include "UI/UIContent/Popup/ItemDropNavigator/GsUIItemDropListContainer.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "UTIL/GsGameObjectUtil.h"

void UGsUIRegionSceneItemDropEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_itemDropButton->OnClicked.AddDynamic(this, &UGsUIRegionSceneItemDropEntry::OnClickGoTo);
}

void UGsUIRegionSceneItemDropEntry::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(TEXT("WorldMapText"), TEXT("MonLevel"), _levelFormat);
}

void UGsUIRegionSceneItemDropEntry::SetData(const struct FGsSchemaWorldMapSpoilsMonsterLevel& inData)
{
	_data = inData;
	_npcNameTextBlock->SetText(_data.npcId.GetRow()->nameText);		
	_gradeTextBlock->SetText(UGsGameObjectUtil::GetRankText(_data.npcId.GetRow()->grade));
	_isAgressiveTextBlock->SetText(UGsGameObjectUtil::GetOffensiveText(_data.isOffensive));
	_levelTextBlock->SetText(FText::Format(_levelFormat, _data.level));
}

void UGsUIRegionSceneItemDropEntry::SetIsSelected(bool bInIsSelected)
{
	_isSelect = bInIsSelected;
}

bool UGsUIRegionSceneItemDropEntry::GetIsSelected()  const
{
	return _isSelect;
}

void UGsUIRegionSceneItemDropEntry::OnClickGoTo()
{
	GMessage()->GetUI().SendMessage(MessageUI::HIDE_WORLD_MAP_ITEM_DROP, nullptr);

	int32 mapId = GSWorldMap()->GetShowRegionMapId();
	int32 npcId = (_data.npcId.GetRow() ? _data.npcId.GetRow()->id : INVALID_CREATURE_ID);
	Level level = _data.level;

	if (INVALID_CREATURE_ID == npcId)
	{
		UGsUIItemDropListContainer::TryToNavigate(mapId);
	}
	else
	{
		// [B1] | ejrrb10 | 일반 필드일 경우 RegionMap 을, 던전일 경우 Dungeon 을 보여준다.
		const FGsSchemaMapData* MapData = nullptr;
		const UGsTableMapData* MapDataTable = Cast<UGsTableMapData>(FGsSchemaMapData::GetStaticTable());
		if (nullptr == MapDataTable || !MapDataTable->FindRowById(mapId, MapData) || nullptr == MapData)
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
		case MapType::PUBLIC_DUNGEON:
		// 엘리트 던전에서 안나오는 현상 수정
		// https://jira.com2us.com/jira/browse/C2URWQ-7202
		case MapType::ELITE_DUNGEON_1:
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

			GameStateRegionMap->ShowDropItemMap(mapId, npcId, level, EGsToggleType::Active);
		}
		break;

		// 던전 페이지 열어 주기
		//case MapType::PUBLIC_DUNGEON:
		//{
		//	FGsMessageHolder* MessageHolder = GMessage();
		//	if (nullptr == MessageHolder)
		//	{
		//		GSLOG(Warning, TEXT("nullptr == MessageHolder"));
		//		return;
		//	}

		//	FGsUIMsgShortcutMap Param(mapId);
		//	MessageHolder->GetUI().SendMessage(MessageUI::REQUEST_SHORTCUT_CONTENTS, &Param);
		//}
		//break;

		default:
			GSLOG(Error, TEXT("Unknown mapType: %d, InMapId:%d"), MapData->mapType, mapId);
			break;
		}
	}
}