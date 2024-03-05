#include "GsUIRegionItem.h"

#include "DataSchema/Map/GsSchemaMapData.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Window/ContinentalMap/GsUIContinentalDungeonMenu.h"

#include "UTIL/GsTableUtil.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "Map/WorldMap/GsRegionInfo.h"
#include "Map/GsMapFunc.h"

#include "Message/GsMessageContents.h"

#include "UI/UIContent/Window/GsUIWindowContinentalMap.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"
#include "DataSchema/Map/Spot/GsSchemaSpotData.h"
#include "DataSchema/Map/GsSchemaMapSpotInfo.h"
#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "DataSchema/InterServer/GsSchemaInvadeMapData.h"

#include "Core/Public/Delegates/Delegate.h"

#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "Components/Image.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapLevelData.h"

void UGsUIRegionItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FText::FindText(TEXT("WorldMapText"), TEXT("RegionLevel"), _levelFormat);

	_btnRegion->OnClicked.AddDynamic(this, &UGsUIRegionItem::OnClickRegion);

	// 맵 타입 처리	
	
}
void UGsUIRegionItem::NativeConstruct()
{
	Super::NativeConstruct();

	ShowDungeonMenu(false);	

	UpdateMapType();
}

void UGsUIRegionItem::UpdateMapType()
{
	_mapType = GetMapType();

	_switcherMapType->SetActiveWidgetIndex((int32)_mapType);
	switch (_mapType)
	{
	case EGsMapType::Normal:
	{
		InvalidNormalMap();
	}
	break;
	case EGsMapType::Dungeon:
	{
		InvalidDungoen();
	}
	break;
	case EGsMapType::InvadePeace :
	{
		InvalidNormalMap(); 
	}
	break;
	case EGsMapType::InvadeOnAir :
	{
		InvalidNormalMap();
	}
	break;
	}
}
// 지역 버튼 클릭
void UGsUIRegionItem::OnClickRegion()
{
	if (_isDungeon == false)
	{
#ifndef TEST_MOVE
		// 현재 대륙-> 지역맵 갈땐 기존 스택 날리고 간다
		// 지역-대륙-지역 스택이면 백버튼시 지역이 남고 미니맵 동작안하는 이슈 때문에
		GUI()->CloseAllStack(nullptr, false);

		// 클릭한 지역맵으로 이동
		if (GMode())
		{
			if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
			{
				if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
				{
					if (FGsGameStateRegionMap* regionMap =
						contents->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap))
					{
						regionMap->SetMapChangeType(EGsWorldMapChangeType::TO_REGIONMAP_CLICK_ITEM, _regionMapId);
					}
				}
			}
		}

		// 지역맵으로 이동
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_REGIONMAP);
#else
		TWeakObjectPtr<UGsUIWidgetBase> wigdet = GUI()->GetWidgetByKey(TEXT("WindowContinentalMap"));
		TWeakObjectPtr<UGsUIWindowContinentalMap> continentalWindow = Cast<UGsUIWindowContinentalMap>(wigdet);
		if (continentalWindow.IsValid() == false)
		{
			return;
		}

		if (continentalWindow->IsCanMoveCenter(this) == true)
		{
			continentalWindow->MoveCenterRegionItem(this);
		}
		else
		{
			// 현재 대륙-> 지역맵 갈땐 기존 스택 날리고 간다
			// 지역-대륙-지역 스택이면 백버튼시 지역이 남고 미니맵 동작안하는 이슈 때문에
			GUI()->CloseAllStack(nullptr, false);

			// 클릭한 지역맵으로 이동
			if (GMode())
			{
				if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
				{
					if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
					{
						if (FGsGameStateRegionMap* regionMap =
							contents->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap))
						{
							regionMap->SetMapChangeType(EGsWorldMapChangeType::TO_REGIONMAP_CLICK_ITEM, _regionMapId);
						}
					}
				}
			}

			// 지역맵으로 이동
			GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_REGIONMAP);
		}
#endif
	}
	// 던전이면
	else
	{		
		FGsSchemaSpotInfo findSpotInfo;
		int mapId = 0;
		if (false == FGsMapFunc::GetDungeonSpotInfo(_regionMapId, findSpotInfo, mapId))
		{
			return;
		}

		GSWorldMap()->SetDungeonEntranceInfo(_regionMapId, mapId,
			findSpotInfo.pos, findSpotInfo.id);

		TWeakObjectPtr<UGsUIWidgetBase> wigdet = GUI()->GetWidgetByKey(TEXT("WindowContinentalMap"));

		if (wigdet.IsValid() == false)
		{
			return;
		}
		TWeakObjectPtr<UGsUIWindowContinentalMap> continentalWindow = Cast<UGsUIWindowContinentalMap>(wigdet);
		if (continentalWindow.IsValid() == false)
		{
			return;
		}
	

		if (false == IsShowDungeonMenu())
		{
			continentalWindow->MoveCenterRegionItem(this);
			continentalWindow->SetSelectDungeonItem(this);
		}
		else
		{
			ShowDungeonMenu(false);
		}

	}
}

bool UGsUIRegionItem::IsShowDungeonMenu()
{
	ESlateVisibility nowVisible = _dungeonMenu->GetVisibility();
	return (nowVisible == ESlateVisibility::Hidden) ? false : true;
}
void UGsUIRegionItem::ShowDungeonMenu(bool In_val)
{
	if (In_val == true)
	{
		_dungeonMenu->PlayAniMenu();
	}

	_dungeonMenu->SetVisibility((In_val == true) ? 
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIRegionItem::InvalidLevelInfo(int32 inLevelId)
{
	const FGsSchemaWorldMapLevelData* levelData = UGsTableUtil::FindRowById<UGsTableWorldMapLevelData, FGsSchemaWorldMapLevelData>(inLevelId);
	if (nullptr == levelData)
	{
		return;
	}

	if (0 == levelData->levelRange.Min
		&& 0 == levelData->levelRange.Max)
	{
		_levelTextBlock->SetVisibility(ESlateVisibility::Collapsed);
		_textMapLevel = FText::GetEmpty();
	}
	else
	{
		_levelTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
		_levelTextBlock->SetText(FText::Format(_levelFormat, levelData->levelRange.Min, levelData->levelRange.Max));

		_textMapLevel = FText::Format(_levelFormat, levelData->levelRange.Min, levelData->levelRange.Max);
	}	
}

void UGsUIRegionItem::InvalidDungoenLevelInfo(int32 inLevelId)
{
	const FGsSchemaDungeonGroupData* dungeonGroupTbl = UGsTableUtil::FindRowById<UGsTableDungeonGroupData, FGsSchemaDungeonGroupData>(inLevelId);
	if (nullptr == dungeonGroupTbl)
	{
		return;
	}

	TArray<int32> levelArray;
	for (const FGsSchemaDungeonDataRow& dungeonData : dungeonGroupTbl->dungeonList)
	{
		const FGsSchemaWorldMapLevelData* levelData = UGsTableUtil::FindRowById<UGsTableWorldMapLevelData, FGsSchemaWorldMapLevelData>(dungeonData.GetRow()->mapId.GetRow()->id);
		if (nullptr == levelData)
		{
			continue;
		}

		if (false == levelArray.Contains(levelData->levelRange.Min))
		{
			levelArray.Add(levelData->levelRange.Min);
		}
		
		if (false == levelArray.Contains(levelData->levelRange.Max))
		{
			levelArray.Add(levelData->levelRange.Max);
		}		
	}

	float min = FMath::Min(levelArray);
	float max = FMath::Max(levelArray);

	_dungeonLevelTextBlock->SetText(FText::Format(_levelFormat, min, max));
}

EGsMapType UGsUIRegionItem::GetMapType()
{
#if WORLD_MAP_INVASION_DEBUG
	return EGsMapType::Invade;
#endif

	if (_isDungeon)
	{
		return EGsMapType::Dungeon;
	}
	else
	{
		auto isInvasionMap = GSInterServer()->GetInvadeMapData(_regionMapId).IsValid();

		if (isInvasionMap)
		{
			if (GSInterServer()->IsValidInvasionTime() && GSInterServer()->IsMapInInvasionTime(_regionMapId))
			{
				 return EGsMapType::InvadeOnAir;
			}
			else
			{
				return EGsMapType::InvadePeace;
			}
		}
	}

	return EGsMapType::Normal;
}

void UGsUIRegionItem::InvalidNormalMap()
{
	// 이름 세팅
	const FGsSchemaMapData* mapTable = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(_regionMapId);
	if (mapTable)
	{
		_textMapName = mapTable->MapStringId;

		InvalidLevelInfo(_regionMapId);

		bool isThereATown = false;
		if (FGsRegionInfo* findInfo = FindRegionInfo(_regionMapId))
		{
			isThereATown = findInfo->_isThereATown;
		}

		SetIsInTown(isThereATown);
	}
}

void UGsUIRegionItem::InvalidDungoen()
{
	FGsSchemaSpotInfo findSpotInfo;
	int mapId = 0;
	if (FGsMapFunc::GetDungeonSpotInfo(_regionMapId, findSpotInfo, mapId))
	{
		_textMapName = findSpotInfo.name;
		InvalidDungoenLevelInfo(_regionMapId);

		_dungeonMenu->SetSpotName(findSpotInfo.name.ToString());
	}
}

void UGsUIRegionItem::UpdateInvaderData()
{
	_invadeInfoPanel->SetVisibility(ESlateVisibility::Collapsed);
	_effInvadeRegion->HideVFX();

#if WORLD_MAP_INVASION_DEBUG
	{
		_effInvadeRegion->ShowVFX();

		_invadeInfoPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		FText InvadeLevelRange;
		FText InvadeUserCount;
		FText::FindText(TEXT("InvadeText"), TEXT("InvadeLevelRange"), InvadeLevelRange);
		FText::FindText(TEXT("InvadeText"), TEXT("InvadeUserCount"), InvadeUserCount);

		auto data = GSInterServer()->GetInvadeMapData(_regionMapId).Pin();
		if (data)
		{
			_textInvadeLevel = FText::FromString(FString::Format(*InvadeLevelRange.ToString(), { data->GetValidLowLevel(), data->GetValidHighLevel() }));
			_textInvadeCount = FText::FromString(FString::Format(*InvadeUserCount.ToString(), { data->GetInvaderCount() })); //data->GetMaxInvaderCount()
		}		
		return;
	}
#endif

	if (auto data = GSInterServer()->GetInvadeMapData(_regionMapId).Pin())
	{
		if (GSInterServer()->IsValidInvasionTime() && GSInterServer()->IsMapInInvasionTime(_regionMapId))
		{
			if (_mapType != EGsMapType::InvadeOnAir)
				UpdateMapType();

			_effInvadeRegion->ShowVFX();

			_invadeInfoPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			FText InvadeLevelRange;
			FText InvadeUserCount;
			FText::FindText(TEXT("InvadeText"), TEXT("InvadeLevelRange"), InvadeLevelRange);
			FText::FindText(TEXT("InvadeText"), TEXT("InvadeUserCount"), InvadeUserCount);


			_textInvadeLevel = FText::FromString(FString::Format(*InvadeLevelRange.ToString(), { data->GetValidLowLevel(), data->GetValidHighLevel() }));
			_textInvadeCount = FText::FromString(FString::Format(*InvadeUserCount.ToString(), { data->GetInvaderCount() })); //data->GetMaxInvaderCount()

		}
		else if (_mapType == EGsMapType::InvadeOnAir)
		{
			UpdateMapType();
		}
	}
	else
	{
		UpdateMapType();
	}
}

void UGsUIRegionItem::SetIsInTown(bool In_isInTown)
{
	UImage* targetTownImg = nullptr;

	// 침공 가능 or 침공전중인 맵에선 마을 표시가 안나오는 이슈 수정
	// img는 있는데 코드처리가 안되어 있었음
	if (_mapType == EGsMapType::Normal)
	{
		targetTownImg = _imgInTown;
	}
	else if(_mapType == EGsMapType::InvadeOnAir)
	{
		targetTownImg = _imgInTown_1;
	}
	else if (_mapType == EGsMapType::InvadePeace)
	{
		targetTownImg = _imgInTown_2;
	}

	if (targetTownImg == nullptr)
	{
		return;
	}

	targetTownImg->SetVisibility((In_isInTown == true) ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

FGsRegionInfo* UGsUIRegionItem::FindRegionInfo(int In_regionId)
{
	FGsGameFlowManager* flowManager = GMode();
	if (flowManager == nullptr)
	{
		return nullptr;
	}
	FGsGameFlowGame* gameFlow = flowManager->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return nullptr;
	}
	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();

	if (nullptr == contents)
	{
		return nullptr;
	}
	FGsGameStateRegionMap* regionMap =
		contents->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap);
	if (nullptr == regionMap)
	{
		return nullptr;
	}

	FGsRegionInfo* findInfo = regionMap->FindRegionInfo(In_regionId);

	if (nullptr == findInfo)
	{
		return nullptr;
	}

	return findInfo;
}

//bool UGsUIRegionItem::GetDungeonSpotInfo(int In_dungeonId, FGsSchemaSpotInfo& Out_spotInfo, int& Out_mapId)
//{
//	const FGsSchemaDungeonGroupData* dungeonGroupTbl =
//		UGsTableUtil::FindRowById<UGsTableDungeonGroupData, FGsSchemaDungeonGroupData>(In_dungeonId);
//
//	if (dungeonGroupTbl == nullptr)
//	{
//		return false;
//	}
//	int mapId = dungeonGroupTbl->UIContinentalMapSpotTable.mapId.GetRow()->mapId;
//	Out_mapId = mapId;
//
//	FGsClientSpawnManager* spawnManager = GSClientSpawn();
//	if (spawnManager == nullptr)
//	{
//		return false;
//	}
//
//	if (false == spawnManager->TryGetSpot(
//		mapId,
//		dungeonGroupTbl->UIContinentalMapSpotTable.spotIndex, Out_spotInfo))
//	{
//		return false;
//	}
//
//	return true;
//}