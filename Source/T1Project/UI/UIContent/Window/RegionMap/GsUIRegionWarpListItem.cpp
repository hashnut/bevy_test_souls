#include "GsUIRegionWarpListItem.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Popup/GsUIPopupWarp.h"

#include "Message/GsMessageContents.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsBookmarkManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"

#include "Map/Bookmark/GsBookmarkDefine.h"
#include "Map/Bookmark/GsBookmarkFunc.h"
#include "Map/GsMapFunc.h"

#include "Map/WorldMap/GsWorldMapDefine.h"
#include "Map/WorldMap/GsWorldMapFunc.h"

#include "DataSchema/Map/GsSchemaMapData.h"

#include "UTIL/GsTableUtil.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "Core/Public/Delegates/Delegate.h"

#include "Components/Image.h"

#include "UMG/Public/Components/WidgetSwitcher.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Bookmark/GsBookmarkHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "EventProgress/GsEventProgressDefine.h"

void UGsUIRegionWarpListItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnAutoMove->OnClicked.AddDynamic(this, &UGsUIRegionWarpListItem::OnClickAutoMove);
	_btnTeleport->OnClicked.AddDynamic(this, &UGsUIRegionWarpListItem::OnClickTeleport);
	_btnItem->OnClicked.AddDynamic(this, &UGsUIRegionWarpListItem::OnClickItem);
}


void UGsUIRegionWarpListItem::OnClickAutoMove()
{
	bool isPortal = (_itemType == EGsRegionWarpListItemType::Portal) ? true : false;
	FGsWorldMapFunc::ClickAutoMoveRegionMap(isPortal, _realSelUseIndex);
}

void UGsUIRegionWarpListItem::OnClickTeleport()
{
	bool isPortal = (_itemType == EGsRegionWarpListItemType::Portal) ? true : false;
	FGsWorldMapFunc::ClickTeleportRegionMap(isPortal, _realSelUseIndex, _textName.ToString());
}
// 텍스트 세팅
void UGsUIRegionWarpListItem::SetText(const FText& In_name)
{
	_textName = In_name;
}

void UGsUIRegionWarpListItem::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;
	if (_imgSelected == nullptr)
	{
		return;
	}

	_imgSelected->SetVisibility((bInIsSelected) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

bool UGsUIRegionWarpListItem::GetIsSelected() const
{
	return _bIsSelected;
}

// 아이템 클릭
void UGsUIRegionWarpListItem::OnClickItem()
{
	// 설명이면 클릭 처리 안함
	if (_itemType == EGsRegionWarpListItemType::PortalTypeDesc ||
		_itemType == EGsRegionWarpListItemType::BookmarkTypeDesc ||
		_itemType == EGsRegionWarpListItemType::EmptyDesc)
	{
		return;
	}

	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	if (_toggleCallback)
	{
		// 이미 선택이 되어있으면
		if (_bIsSelected == true)
		{
			// 해제(-1이 선택되게 처리)
			_toggleCallback(-1, true);
		}
		else
		{
			_toggleCallback(_indexInGroup, !_bIsSelected);
		}
	}
}

// 데이터 세팅
void UGsUIRegionWarpListItem::SetData(EGsRegionWarpListItemType In_type, 
	const FText& In_name,
	int In_realSelUseIndex, bool In_isInTown)
{
	int switchIndex = 0;

	_itemType = In_type;

	if (In_type == EGsRegionWarpListItemType::BookmarkTypeDesc ||
		In_type == EGsRegionWarpListItemType::PortalTypeDesc)
	{
		switchIndex = 0;
		_textTypeDesc = In_name;
	}
	else if(In_type == EGsRegionWarpListItemType::Bookmark ||
		In_type == EGsRegionWarpListItemType::Portal)
	{
		switchIndex = 1;
		_textName = In_name;

		// portal and not in town 0
		// bookmark  1
		// portal and in town 2
		int switchIconIndex = 0;
		if (In_type == EGsRegionWarpListItemType::Portal)
		{
			switchIconIndex = (In_isInTown == true) ? 2 : 0;
		}
		else
		{
			switchIconIndex = 1;
		}
		_switcherIcon->SetActiveWidgetIndex(switchIconIndex);
	}
	else
	{
		switchIndex = 2;
		_textEmptyDesc = In_name;
	}
	_switcherType->SetActiveWidgetIndex(switchIndex);


	_realSelUseIndex = In_realSelUseIndex;

}