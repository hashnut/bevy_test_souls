#include "GsUIRegionSceneSpoilsPanel.h"

#include "Map/Bookmark/GsBookmarkData.h"
#include "Map/Bookmark/GsBookmarkFunc.h"
#include "Map/WorldMap/GsRegionInfo.h"
#include "Map/WorldMap/GsWorldMapFunc.h"

#include "UI/UIContent/Window/RegionMap/GsUIRegionSceneImage.h"
#include "UI/UIContent/Window/RegionMap/GsUIRegionIconRoot.h"
#include "UI/UIContent/Window/RegionMap/GsUIRegionWarpListItem.h"
#include "UI/UIContent/Window/RegionMap/GsUIRegionIconFiltering.h"
#include "UI/UIContent/Window/RegionMap/GsUIRegionScene.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconSelect.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconSelectMonster.h"
#include "UI/UIContent/Window/RegionMap/Monster/GsUIRegionMonsterDropItem.h"
#include "UI/UIContent/Window/RegionMap/Monster/GsUIRegionSpoilsEntry.h"

#include "UI/UIContent/Popup/ItemDropNavigator/GsUIItemDropListContainer.h"

#include "UI/UIContent/Common/Icon/GsUIIconAbnormality.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipTitleDesc.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"

#include "UI/UILib/Define/GsUIDefine.h"
#include "UI/UILib/Manager/GsWidgetPoolManager.h"
#include "UI/UILib/Container/GsWidgetPool.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/GsSchemaMapTerritoryData.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"
#include "DataSchema/Map/WorldMap/GsSchemaRegionMapData.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapSpoilsData.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/InterServer/GsSchemaInvadeMapData.h"

#include "Message/GsMessageContents.h"

#include "Message/MessageParam/GsUIMessageParam.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsBookmarkManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"

#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Bookmark/GsBookmarkHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsUIUtil.h"
#include "UTIL/GsLevelUtil.h"

#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "UMG/Public/Blueprint/UserWidget.h"

#include "Core/Public/Delegates/Delegate.h"

#include "T1Project.h"


#define WOLRD_MAP_COLLECTION

void UGsUIRegionSceneSpoilsPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//equip spoil
	_equipScrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_equipScrollBoxHelper->Initialize(_entryWidgetClass, _equipSpoilVerticalBox);
#ifdef WOLRD_MAP_COLLECTION
	_equipScrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIRegionSceneSpoilsPanel::OnCreateItemEntry);
	_equipScrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIRegionSceneSpoilsPanel::OnRefreshItemEntry);
#else
	_equipScrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIRegionSceneSpoilsPanel::OnCreateEquipItemEntry);
	_equipScrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIRegionSceneSpoilsPanel::OnRefreshEquipItemEntry);
#endif
	
#ifndef WOLRD_MAP_COLLECTION
	//etc spoil
	_etcScrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_etcScrollBoxHelper->Initialize(_entryWidgetClass, _etcSpoilVerticalBox);
	_etcScrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIRegionSceneSpoilsPanel::OnCreateEtcItemEntry);
	_etcScrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIRegionSceneSpoilsPanel::OnRefreshEtcItemEntry);	
#endif

#ifdef WOLRD_MAP_COLLECTION
	_spoilTabToggleGroup.AddToggleWidgetByParentPanel(_panelTab);
	_spoilTabToggleGroup.OnSelectChanged.BindUObject(this, &UGsUIRegionSceneSpoilsPanel::OnClickTab);
#endif
}

void UGsUIRegionSceneSpoilsPanel::BeginDestroy()
{
	if (_equipScrollBoxHelper)
	{
		_equipScrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIRegionSceneSpoilsPanel::OnRefreshEquipItemEntry);
	}
#ifndef WOLRD_MAP_COLLECTION
	if (_etcScrollBoxHelper)
	{
		_etcScrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIRegionSceneSpoilsPanel::OnRefreshEtcItemEntry);
	}
#endif

	_equipScrollBoxHelper = nullptr;
#ifndef WOLRD_MAP_COLLECTION
	_etcScrollBoxHelper = nullptr;
	_equipSpoilToggleGroup.Clear();
	_etcSpoilToggleGroup.Clear();
#else
	_spoilToggleGroup.Clear();
#endif

	

	Super::BeginDestroy();
}

void UGsUIRegionSceneSpoilsPanel::InitializeRegionSceneSpoilsPanelWidget()
{
	//던전에서는 전리품을 보여주지 않는다. 
	// 월드맵 개선
	// 던전에서도 전리품 탭 사용 가능하게 변경
	// https://jira.com2us.com/jira/browse/CHR-23756
	
	//일반 필드라면 정상적으로 전리품을 보여준다.
	_switcher->SetActiveWidgetIndex(0);
	InitSpoil();

	// 기존 코드인데 빼도 되지 않을까
#ifndef WOLRD_MAP_COLLECTION
	PreSelectIndex();
#endif
}

void UGsUIRegionSceneSpoilsPanel::InitSpoil()
{
	int currentMapId = GSWorldMap()->GetShowRegionMapId();
	_spoilsData = UGsTableUtil::FindRowById<UGsTableWorldMapSpoilsData, FGsSchemaWorldMapSpoilsData>(currentMapId);

#ifdef WOLRD_MAP_COLLECTION
	// 초기화
	int listCount = 0;
	_equipItemNum = 0;
	_etcItemNum = 0;
	if (_spoilsData != nullptr)
	{
		_equipItemNum = _spoilsData->equipSpoils.Num();
		_etcItemNum = _spoilsData->etcSpoils.Num();		
	}

	// 장비탭 액티브 처리
	_spoilTabToggleGroup.SetSelectedIndex(0, true);

	_spoilToggleGroup.Clear();
	_spoilToggleGroup.AddToggleWidgetByParentPanel(_equipSpoilVerticalBox);
	_spoilToggleGroup.SetSelectedIndex(-1);
#else
	InitEquipSpoil();
	InitEtcSpoil();	
#endif

}

void UGsUIRegionSceneSpoilsPanel::InitEquipSpoil()
{
#ifndef WOLRD_MAP_COLLECTION
	if (nullptr == _spoilsData)
	{
		_equipSpoilSwitcher->SetActiveWidgetIndex(1);
		return;
	}

	_equipItemNum = _spoilsData->equipSpoils.Num();
	if (0 == _equipItemNum)
	{
		_equipSpoilSwitcher->SetActiveWidgetIndex(1);
		return;
	}
	
	_equipSpoilSwitcher->SetActiveWidgetIndex(0);	

	_equipScrollBoxHelper->RefreshAll(_equipItemNum);
	_equipSpoilToggleGroup.Clear();
	_equipSpoilToggleGroup.AddToggleWidgetByParentPanel(_equipSpoilVerticalBox);
	_equipSpoilToggleGroup.SetSelectedIndex(-1);
#endif
}

void UGsUIRegionSceneSpoilsPanel::InitEtcSpoil()
{
#ifndef WOLRD_MAP_COLLECTION
	if (nullptr == _spoilsData)
	{
		_etcSpoilSwitcher->SetActiveWidgetIndex(1);
		return;
	}

	_etcItemNum = _spoilsData->etcSpoils.Num();
	if (0 == _etcItemNum)
	{
		_etcSpoilSwitcher->SetActiveWidgetIndex(1);
		return;
	}
	
	_etcSpoilSwitcher->SetActiveWidgetIndex(0);	

	_etcScrollBoxHelper->RefreshAll(_etcItemNum);
	_etcSpoilToggleGroup.Clear();
	_etcSpoilToggleGroup.AddToggleWidgetByParentPanel(_etcSpoilVerticalBox);
	_etcSpoilToggleGroup.SetSelectedIndex(-1);
#endif
}

void UGsUIRegionSceneSpoilsPanel::OnCreateEquipItemEntry(class UWidget* inWidget)
{
#ifndef WOLRD_MAP_COLLECTION
	UGsUIRegionSpoilsEntry* widget = Cast<UGsUIRegionSpoilsEntry>(inWidget);
	if (nullptr == widget)
	{
		return;
	}

	widget->_clickEvent = [this](int32 index) {
		_equipSpoilToggleGroup.SetSelectedIndex(index);
		_etcSpoilToggleGroup.SetSelectedIndex(-1);	

		if (_spoilsData->equipSpoils.IsValidIndex(index))
		{
			_selectItem = _spoilsData->equipSpoils[index];
			ShowItemDrop(_selectItem);
		}		

		//전리품 선택시 전리품 몬스터 정보 클리어
		FGsGameStateRegionMap* GameStateRegionMap = UGsUIItemDropListContainer::GetGameStateRegionMap();
		if (GameStateRegionMap)
		{
			GameStateRegionMap->SelectSpoils(_selectItem);
			GameStateRegionMap->ClearSpoilsMonster();
		}
	};
#endif
}

void UGsUIRegionSceneSpoilsPanel::OnCreateEtcItemEntry(class UWidget* inWidget)
{
#ifndef WOLRD_MAP_COLLECTION
	UGsUIRegionSpoilsEntry* widget = Cast<UGsUIRegionSpoilsEntry>(inWidget);
	if (nullptr == widget)
	{
		return;
	}

	widget->_clickEvent = [this](int32 index) {
		_etcSpoilToggleGroup.SetSelectedIndex(index);
		_equipSpoilToggleGroup.SetSelectedIndex(-1);

		if (_spoilsData->etcSpoils.IsValidIndex(index))
		{
			_selectItem = _spoilsData->etcSpoils[index];
			ShowItemDrop(_selectItem);
		}

		//전리품 선택시 전리품 몬스터 정보 클리어
		FGsGameStateRegionMap* GameStateRegionMap = UGsUIItemDropListContainer::GetGameStateRegionMap();
		if (GameStateRegionMap)
		{
			GameStateRegionMap->SelectSpoils(_selectItem);
			GameStateRegionMap->ClearSpoilsMonster();
		}
	};
#endif
}

void UGsUIRegionSceneSpoilsPanel::OnRefreshEquipItemEntry(int32 InIndex, class UWidget* InEntry)
{
#ifndef WOLRD_MAP_COLLECTION
	if (nullptr == _spoilsData)
	{
		return;
	}

	if (_spoilsData->equipSpoils.Num() <= InIndex)
	{
		return;
	}

	const FGsSchemaItemCommon* itemTbl = _spoilsData->equipSpoils[InIndex].item.GetRow();
	if (itemTbl == nullptr)
	{
		return;
	}

	UGsUIRegionSpoilsEntry* item = Cast<UGsUIRegionSpoilsEntry>(InEntry);
	if (item == nullptr)
	{
		return;
	}

	item->SetItemTblId(itemTbl->id, itemTbl->name, itemTbl->grade);	
#endif
}

void UGsUIRegionSceneSpoilsPanel::OnRefreshEtcItemEntry(int32 InIndex, class UWidget* InEntry)
{
#ifndef WOLRD_MAP_COLLECTION
	if (nullptr == _spoilsData)
	{
		return;
	}

	if (_spoilsData->etcSpoils.Num() <= InIndex)
	{
		return;
	}

	const FGsSchemaItemCommon* itemTbl = _spoilsData->etcSpoils[InIndex].item.GetRow();
	if (itemTbl == nullptr)
	{
		return;
	}

	UGsUIRegionSpoilsEntry* item = Cast<UGsUIRegionSpoilsEntry>(InEntry);
	if (item == nullptr)
	{
		return;
	}

	item->SetItemTblId(itemTbl->id, itemTbl->name, itemTbl->grade);
#endif
}

void UGsUIRegionSceneSpoilsPanel::ShowItemDrop(const FGsSchemaWorldMapSpoilsItem& inItem)
{
	FGsUIMsgWorldMapItemDrop param;
	param._monsterArray = inItem.monsterLevelArray;
	GMessage()->GetUI().SendMessage(MessageUI::SHOW_WORLD_MAP_ITEM_DROP, &param);
}

//이전에 선택한 전리품이 있다면 초기화시 전리품 선택
void UGsUIRegionSceneSpoilsPanel::PreSelectIndex()
{
	FGsGameStateRegionMap* gameStateRegionMap = UGsUIItemDropListContainer::GetGameStateRegionMap();
	if (nullptr == gameStateRegionMap)
	{
		return;
	}

	if (gameStateRegionMap->IsEmptySpoils()
		|| gameStateRegionMap->IsEmptySpoilsMonster())
	{
		return;
	}

	if (nullptr == _spoilsData)
	{
		return;
	}

	for (int32 i = 0; i < _spoilsData->equipSpoils.Num(); ++i)
	{	
		if (gameStateRegionMap->IsEqualsSpoils(_spoilsData->equipSpoils[i].item.GetRow()))
		{		
			_equipSpoilToggleGroup.SetSelectedIndex(i);
			return;
		}
	}

	for (int32 i = 0; i < _spoilsData->etcSpoils.Num(); ++i)
	{
		if (gameStateRegionMap->IsEqualsSpoils(_spoilsData->etcSpoils[i].item.GetRow()))
		{
			_etcSpoilToggleGroup.SetSelectedIndex(i);
			return;
		}
	}
}

// 리스트 하나로 변경
// https://jira.com2us.com/jira/browse/CHR-25993
void UGsUIRegionSceneSpoilsPanel::OnCreateItemEntry(class UWidget* inWidget)
{
	UGsUIRegionSpoilsEntry* widget = Cast<UGsUIRegionSpoilsEntry>(inWidget);
	if (nullptr == widget)
	{
		return;
	}

	widget->_clickEvent = [this](int32 index) {
		_spoilToggleGroup.SetSelectedIndex(index);

		if (_spoilTabType == EGsWorldMapSpoilTabType::Equip)
		{
			if (_spoilsData->equipSpoils.IsValidIndex(index))
			{
				_selectItem = _spoilsData->equipSpoils[index];
				ShowItemDrop(_selectItem);
			}
		}
		else
		{
			if (_spoilsData->etcSpoils.IsValidIndex(index))
			{
				_selectItem = _spoilsData->etcSpoils[index];
				ShowItemDrop(_selectItem);
			}
		}

		//전리품 선택시 전리품 몬스터 정보 클리어
		FGsGameStateRegionMap* GameStateRegionMap = UGsUIItemDropListContainer::GetGameStateRegionMap();
		if (GameStateRegionMap)
		{
			GameStateRegionMap->SelectSpoils(_selectItem);
			GameStateRegionMap->ClearSpoilsMonster();
		}
		};
}


void UGsUIRegionSceneSpoilsPanel::OnRefreshItemEntry(int32 InIndex, class UWidget* InEntry)
{	
	const FGsSchemaItemCommon* itemTbl = GetItemCommon(InIndex);
	if (itemTbl == nullptr)
	{
		return;
	}

	UGsUIRegionSpoilsEntry* item = Cast<UGsUIRegionSpoilsEntry>(InEntry);
	if (item == nullptr)
	{
		return;
	}

	item->SetItemTblId(itemTbl->id, itemTbl->name, itemTbl->grade);
}

// 탭 클릭	
void UGsUIRegionSceneSpoilsPanel::OnClickTab(int32 In_index)
{
	EGsWorldMapSpoilTabType tabType = (EGsWorldMapSpoilTabType)In_index;
	_spoilTabType = tabType;

	int listCount = (_spoilTabType == EGsWorldMapSpoilTabType::Equip)? _equipItemNum : _etcItemNum;

	if(listCount == 0)
	{
		_equipSpoilSwitcher->SetActiveWidgetIndex(1);
		return;
	}
	_equipSpoilSwitcher->SetActiveWidgetIndex(0);

	_equipScrollBoxHelper->RefreshAll(listCount);

	// 스크롤 위치 초기화
	_equipSpoilScrollBox->SetScrollOffset(0.0f);

	// 셀렉트 해제
	_spoilToggleGroup.SetSelectedIndex(-1);
}

// 현재 인덱스로 아이템 테이블 접근
const FGsSchemaItemCommon* UGsUIRegionSceneSpoilsPanel::GetItemCommon(int In_listIndex)
{
	if (nullptr == _spoilsData)
	{
		return nullptr;
	}
	const FGsSchemaItemCommon* findTbl = nullptr;
	if(_spoilTabType == EGsWorldMapSpoilTabType::Equip)
	{ 
		if (_spoilsData->equipSpoils.Num() <= In_listIndex)
		{
			return nullptr;
		}
		findTbl = _spoilsData->equipSpoils[In_listIndex].item.GetRow();
	}		
	else
	{
		if (_spoilsData->etcSpoils.Num() <= In_listIndex)
		{
			return nullptr;
		}
		findTbl = _spoilsData->etcSpoils[In_listIndex].item.GetRow();
	}
	return findTbl;
}