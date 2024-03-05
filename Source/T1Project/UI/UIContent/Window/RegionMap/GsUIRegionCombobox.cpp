#include "GsUIRegionCombobox.h"

#include "UI/UIContent/Window/RegionMap/GsUIRegionComboboxItem.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Popup/ItemDropNavigator/GsUIItemDropListContainer.h"

#include "Map/WorldMap/GsContinentalInfo.h"
#include "Map/WorldMap/GsRegionGroupInfoBase.h"
#include "Map/WorldMap/GsRegionInfo.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Management/ScopeGame/GsWorldMapManager.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapLevelData.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "UTIL/GsTableUtil.h"

#include "UMG/Public/Components/VerticalBox.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/TextBlock.h"

#include "Components/Image.h"

#include "Core/Public/Delegates/Delegate.h"


void UGsUIRegionCombobox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _verticalBox);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIRegionCombobox::OnRefreshEntry);

	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIRegionCombobox::OnClickComboboxItem);
	_toggleGroup.OnNotChangeEvent.BindUObject(this, &UGsUIRegionCombobox::OnSelectNotChange);

	_btnShowList->OnClicked.AddDynamic(this, &UGsUIRegionCombobox::OnClickShowList);
}

void UGsUIRegionCombobox::NativeConstruct()
{
	FText::FindText(TEXT("WorldMapText"), TEXT("RegionLevel"), _levelFormat);
	
	InitData();
	_panelList->SetVisibility(ESlateVisibility::Hidden);
	_btnShowList->SetIsEnabled(GGameData()->IsInvadeWorld() == false);
	Super::NativeConstruct();
}

void UGsUIRegionCombobox::NativeDestruct()
{
	_panelList->SetVisibility(ESlateVisibility::Hidden);
	Super::NativeDestruct();
}

void UGsUIRegionCombobox::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	FGsRegionGroupInfoBase* findInfo = GSWorldMap()->FindGroupInfoByCurrentRegionId();
	if (findInfo == nullptr)
	{
		return;
	}

	if (findInfo->_arrayRegionId.Num() <= InIndex)
	{
		return;
	}

	int regionMapId = findInfo->_arrayRegionId[InIndex];

	int showRegionMapId = GSWorldMap()->GetShowRegionMapId();
	bool isSameMapItem = (showRegionMapId == regionMapId) ? true : false;

	UGsUIRegionComboboxItem* item = Cast<UGsUIRegionComboboxItem>(InEntry);
	if (item == nullptr)
	{
		return;
	}
	FText mapText = FGsWorldMapManager::GetMapName(regionMapId);
	item->SetText(mapText);
	item->SetCurrentMapItem(isSameMapItem);
	
	bool isThereATown = false;
	if (FGsRegionInfo* finRegiondInfo = FindRegionInfo(regionMapId))
	{
		isThereATown = finRegiondInfo->_isThereATown;
	}
	item->SetIsInTown(isThereATown);
}

// 아이템 클릭
void UGsUIRegionCombobox::OnClickComboboxItem(int32 In_index)
{
	if (-1 == In_index)
	{
		return;
	}

	// 해당 id로 지역 변경
	FGsRegionGroupInfoBase* findInfo = GSWorldMap()->FindGroupInfoByCurrentRegionId();
	if (nullptr == findInfo)
	{
		return;
	}

	if (false == findInfo->_arrayRegionId.IsValidIndex(In_index))
	{
		return;
	}

	int regionMapId = findInfo->_arrayRegionId[In_index];

	FGsGameStateRegionMap* GameStateRegionMap = UGsUIItemDropListContainer::GetGameStateRegionMap();
	if (GameStateRegionMap)
	{
		//같은 맵을 선택하면 리턴
		if (regionMapId == GSWorldMap()->GetShowRegionMapId())
		{
			_panelList->SetVisibility(ESlateVisibility::Hidden);
			return;
		}		

		//다른 맵을 선택하면 전리품 정보 초기화
		GameStateRegionMap->ClearSpoils();
		GameStateRegionMap->ClearSpoilsMonster();
	}

	if (GMode())
	{
		if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				if (FGsGameStateRegionMap* regionMap =
					contents->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap))
				{
					regionMap->SetMapChangeType(EGsWorldMapChangeType::TO_REGIONMAP_CLICK_ITEM, regionMapId);
				}
			}
		}
	}

	GMessage()->GetUI().SendMessage(MessageUI::CHANGE_REGION_MAP, nullptr);

	_panelList->SetVisibility(ESlateVisibility::Hidden);
}

void UGsUIRegionCombobox::OnSelectNotChange()
{
	_panelList->SetVisibility(ESlateVisibility::Hidden);
}

// 리스트 보여주기 클릭
void UGsUIRegionCombobox::OnClickShowList()
{
	ESlateVisibility currentVisibility = _panelList->GetVisibility();
	ESlateVisibility newVisibility = (currentVisibility == ESlateVisibility::Hidden) ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;

	_panelList->SetVisibility(newVisibility);
}

void UGsUIRegionCombobox::BeginDestroy()
{
	if (_scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIRegionCombobox::OnRefreshEntry);
	}
	_scrollBoxHelper = nullptr;

	_toggleGroup.Clear();
	Super::BeginDestroy();
}

// 리스트 초기화
void UGsUIRegionCombobox::InitList()
{
	FGsRegionGroupInfoBase* findInfo = GSWorldMap()->FindGroupInfoByCurrentRegionId();
	if (findInfo == nullptr)
	{
		return;
	}

	_scrollBoxHelper->RefreshAll(findInfo->_arrayRegionId.Num());

	// 그룹초기화
	_toggleGroup.Clear();
	_toggleGroup.AddToggleWidgetByParentPanel(_verticalBox);

	// 선택 안됨처리
	_toggleGroup.SetSelectedIndex(-1);
}

// 최신 데이터로 세팅
void UGsUIRegionCombobox::InitData()
{
	InitList();

	int showRegionMapId = GSWorldMap()->GetShowRegionMapId();
	_textCurrentSelMapName = FGsWorldMapManager::GetMapName(showRegionMapId);

	InvalidLevelInfo(showRegionMapId);

	bool isThereATown = false;
	if (FGsRegionInfo* findInfo = FindRegionInfo(showRegionMapId))
	{
		isThereATown = findInfo->_isThereATown;
	}

	SetIsInTown(isThereATown);
}

void UGsUIRegionCombobox::SetIsInTown(bool In_isInTown)
{
	_imgInTown->SetVisibility((In_isInTown == true) ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIRegionCombobox::InvalidLevelInfo(int32 inLevelId)
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
	}
	else
	{
		_levelTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
		_levelTextBlock->SetText(FText::Format(_levelFormat, levelData->levelRange.Min, levelData->levelRange.Max));
	}	
}

FGsRegionInfo* UGsUIRegionCombobox::FindRegionInfo(int In_regionId)
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