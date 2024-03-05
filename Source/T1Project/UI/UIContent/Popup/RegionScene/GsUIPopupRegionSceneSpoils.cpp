#include "GsUIPopupRegionSceneSpoils.h"

#include "Map/Bookmark/GsBookmarkData.h"
#include "Map/Bookmark/GsBookmarkFunc.h"
#include "Map/WorldMap/GsRegionInfo.h"
#include "Map/WorldMap/GsWorldMapFunc.h"

#include "UI/UIContent/Window/RegionMap/GsUIRegionSceneImage.h"
#include "UI/UIContent/Window/RegionMap/GsUIRegionIconRoot.h"
#include "UI/UIContent/Window/RegionMap/GsUIRegionWarpListItem.h"
#include "UI/UIContent/Window/RegionMap/GsUIRegionIconFiltering.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconSelect.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconSelectMonster.h"

#include "UI/UIContent/Common/Icon/GsUIIconAbnormality.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipTitleDesc.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"

#include "UI/UILib/Manager/GsWidgetPoolManager.h"
#include "UI/UILib/Container/GsWidgetPool.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/GsSchemaMapTerritoryData.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"
#include "DataSchema/Map/WorldMap/GsSchemaRegionMapData.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/InterServer/GsSchemaInvadeMapData.h"

#include "Message/GsMessageContents.h"

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

#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Blueprint/UserWidget.h"

#include "Core/Public/Delegates/Delegate.h"

#include "T1Project.h"
#include "UI/UIContent/Window/RegionMap/GsUIRegionScene.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapSpoilsData.h"
#include "UI/UIContent/Window/RegionMap/Monster/GsUIRegionMonsterDropItem.h"
#include "UI/UIContent/Window/RegionMap/Monster/GsUIRegionSpoilsEntry.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "UI/UIContent/Window/RegionMap/RegionScene/GsUIRegionSceneItemDropEntry.h"

void UGsUIPopupRegionSceneSpoils::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelScrollRoot);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupRegionSceneSpoils::OnRefreshEntry);

	_closeButton->OnClicked.AddDynamic(this, &UGsUIPopupRegionSceneSpoils::OnClickClose);
}

void UGsUIPopupRegionSceneSpoils::NativeConstruct()
{
	Super::NativeConstruct();

	_scrollBox->SetScrollOffset(0);
}

void UGsUIPopupRegionSceneSpoils::NativeDestruct()
{
	_toggleGroup.Clear();

	Super::NativeDestruct();
}

void UGsUIPopupRegionSceneSpoils::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupRegionSceneSpoils::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	_toggleGroup.Clear();

	Super::BeginDestroy();
}

void UGsUIPopupRegionSceneSpoils::InvalidateAllInternal()
{
	_slotHelper->RefreshAll(_dataArray.Num());

	_toggleGroup.Clear();
	_toggleGroup.AddToggleWidgetByParentPanel(_panelScrollRoot);
	// 선택을 해제 시켜놓기 위함
	_toggleGroup.SetSelectedIndex(-1);
}

void UGsUIPopupRegionSceneSpoils::SetMonsterData(const TArray<FGsSchemaWorldMapSpoilsMonsterLevel>& inData)
{
	_dataArray = inData;
	// 드랍 리스트 채우기
	InvalidateAllInternal();

	if (_dataArray.Num() >= _numberOfWidgetForScroll)
	{
		_scrollBox->SetAlwaysShowScrollbar(true);
	}
	else
	{
		_scrollBox->SetAlwaysShowScrollbar(false);
	}

	_scrollBox->SetScrollOffset(0);	
}

void UGsUIPopupRegionSceneSpoils::OnInputCancel()
{
	Close();
}

void UGsUIPopupRegionSceneSpoils::OnClickClose()
{
	Close();
}

void UGsUIPopupRegionSceneSpoils::OnRefreshEntry(int32 InIndex, class UWidget* InEntry)
{
	if (false == _dataArray.IsValidIndex(InIndex))
	{
		return;
	}

	UGsUIRegionSceneItemDropEntry* entry = Cast<UGsUIRegionSceneItemDropEntry>(InEntry);
	if (nullptr == entry)
	{
		return;
	}

	entry->SetData(_dataArray[InIndex]);
}


bool UGsUIPopupRegionSceneSpoils::OnBack()
{
	GSLOG(Log, TEXT("WorldMap - UGsUIPopupRegionSceneSpoils - OnBack"));

	return Super::OnBack();
}
