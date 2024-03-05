
#include "GsUIWindowDungeon.h"
#include "../Dungeon/GsDungeonData.h"
#include "../Dungeon/GsDungeonGroupBase.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"
#include "Dungeon/GsUISideTabDungeonGroupSlot.h"
#include "Dungeon/GsUIPageDungeonGroupBase.h"
#include "Dungeon/GsUIPageDungeonGroupArena.h"

#include "Image.h"
#include "ScrollBox.h"
#include "PanelWidget.h"
#include "WidgetSwitcher.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UMG/Public/Components/HorizontalBox.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "DataSchema/Dungeon/GsSchemaDungeonData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "T1Project.h"


void UGsUIWindowDungeon::BeginDestroy()
{
	if (_scrollBoxHelperSideTab)
	{
		_scrollBoxHelperSideTab->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowDungeon::OnRefreshSideTabEntry);
		_scrollBoxHelperSideTab = nullptr;
	}

	if (_btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIWindowDungeon::OnClickClose);
		_btnClose = nullptr;
	}

	if (_btnBack)
	{
		_btnBack->OnClicked.RemoveDynamic(this, &UGsUIWindowDungeon::OnClickBack);
		_btnBack = nullptr;
	}
	
	_toggleGroupTab.Clear();

	Super::BeginDestroy();
}

void UGsUIWindowDungeon::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_helpContentsKey = EGsHelpContents::Dungeon;

	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowDungeon::OnClickClose);
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowDungeon::OnClickBack);

	_scrollBoxHelperSideTab = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelperSideTab->Initialize(_entryWidgetSideTab, _scrollBoxSideTab);
	_scrollBoxHelperSideTab->OnRefreshEntry.AddDynamic(this, &UGsUIWindowDungeon::OnRefreshSideTabEntry);

	_toggleGroupTab.Clear();
	for (auto item : _panelTabGroup->GetAllChildren())
	{
		TWeakObjectPtr<UGsSwitcherButton> tapButton = Cast<UGsSwitcherButton>(item);
		if (tapButton.IsValid())
		{
			_toggleGroupTab.AddToggleWidget(tapButton.Get());
		}
	}
	// 2023/2/22 PKT - Main Tab
	_toggleGroupTab.OnSelectChanged.BindUObject(this, &UGsUIWindowDungeon::OnSelectedMainTab);
	// 2023/2/22 PKT - Side Sub Tab
	_toggleGroupSideSubTab.OnSelectChanged.BindUObject(this, &UGsUIWindowDungeon::OnSlectedSideSubTab);

	_captureData = FGsDungeonUICaptureData();
}

void UGsUIWindowDungeon::NativeConstruct()
{
	Super::NativeConstruct();

	bool isReset = false == IsStackRecoverFlag();

	for (int32 idx = 0; idx < StaticCast<int32>(DungeonCategory::MAX); ++idx)
	{
		if (UWidget* widget = Cast<UWidget>(_toggleGroupTab.GetEntry(idx)))
		{
			bool isHidden = GSDungeon()->IsHiddenWithMainTab(StaticCast<DungeonCategory>(idx));
			widget->SetVisibility(isHidden ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

			if (true == isHidden)
			{	// 2023/9/19 PKT - 숨겨야할 Tab이 보여줘야 할 Data라면 리셋한다.
				if (_captureData._toggleTabIndex == idx)
				{
					isReset = true;
				}
			}
		}
	}

	if (isReset)
	{		
		_captureData = FGsDungeonUICaptureData();
	}

	_activeDetailWidget.Reset();

	int32 currentTab = _captureData._toggleTabIndex;
	_toggleGroupTab.SetSelectedIndex(currentTab, true);

	// 2022/07/07 PKT - Clear
	_captureData = FGsDungeonUICaptureData();

	_contentsDelegate = GMessage()->GetContents().AddUObject(MessageContents::CLOSE_WINDOW_DUNGEON, this, &UGsUIWindowDungeon::OnCloseWindow);
}

void UGsUIWindowDungeon::NativeDestruct()
{	
	// 바로가기 던전아이디 삭제
	_shortcutMapId = 0;

	// 2023/2/22 PKT - 캡쳐
	_captureData = FGsDungeonUICaptureData();
	_captureData._toggleTabIndex = _toggleGroupTab.GetSelectedIndex();
	_captureData._toggleSlotIndex = _toggleGroupSideSubTab.GetSelectedIndex();
	_captureData._scrollBoxSlotOffset = _scrollBoxSideTab->GetScrollOffset();

	if (_activeDetailWidget.IsValid())
	{	
		_activeDetailWidget.Get()->Capture(&_captureData);		
	}

	GMessage()->GetContents().Remove(_contentsDelegate);

	Super::NativeDestruct();
}

bool UGsUIWindowDungeon::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsDungeon))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_DUNGEON);
		return true;
	}

	return false;
}

void UGsUIWindowDungeon::CheckRedDot()
{
	const FGsDungeonManager* dungeonManager = GSDungeon();
	if (dungeonManager)
	{	// 2023/2/22 PKT - 던전 레드닷은 파티 던전만 존재.
		bool isPartyDungeonRedDot = dungeonManager->CheckRedDot(DungeonCategory::PARTY);
		_redDotPartyDungeon->SetVisibility(isPartyDungeonRedDot ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UGsUIWindowDungeon::OnCloseWindow()
{
	Close();
}

void UGsUIWindowDungeon::OnRefreshSideTabEntry(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUISideTabDungeonGroupSlot> item = Cast<UGsUISideTabDungeonGroupSlot>(InEntry);
	if (false == item.IsValid())
	{
		return;
	}

	const FGsDungeonGroupBase* data 
		= GSDungeon()->ViewGroupSet().IsValidIndex(InIndex) ? GSDungeon()->ViewGroupSet()[InIndex] : nullptr;
	if (nullptr == data)
	{
		return;
	}

	item->SetData(data);
	if (false == item->_OnClickedSlotDelegate.IsBoundToObject(this))
	{
		item->_OnClickedSlotDelegate.BindUObject(this, &UGsUIWindowDungeon::OnSelectedSideTab);
	}	
}

void UGsUIWindowDungeon::OnSelectedMainTab(int32 inIndex)
{
	const int32 maxCount = GSDungeon()->BuildViewGroup(StaticCast<DungeonCategory>(inIndex));

	// 2023/2/21 PKT - Set Data
	_scrollBoxHelperSideTab->RefreshAll(maxCount);
	
	_toggleGroupSideSubTab.Clear();
	_toggleGroupSideSubTab.AddToggleWidgetByParentPanel(_scrollBoxSideTab, false);

	UWidget* focusWidget = nullptr;
	int32 sideTabIndex = _captureData._toggleSlotIndex;
	if (0 != _captureData._mapId)
	{
		const FGsDungeonGroupBase* groupData = GSDungeon()->FindGroupDataByMapId(_captureData._mapId);
		for (int32 idx = 0; idx < _scrollBoxHelperSideTab->GetItemCount(); ++idx)
		{
			TWeakObjectPtr<UGsUISideTabDungeonGroupSlot> item = Cast<UGsUISideTabDungeonGroupSlot>(_scrollBoxHelperSideTab->GetEntry(idx));
			if (groupData && item.IsValid() && groupData->GetSchemaData()->id == item.Get()->GetData()->GetSchemaData()->id)
			{
				sideTabIndex = idx;
				
				focusWidget = item.Get();

				break;
			}
		}
	}
	_toggleGroupSideSubTab.SetSelectedIndex(sideTabIndex, true);
		
	// 2023/2/21 PKT - Scoll Offset
	if (focusWidget)
	{
		_scrollBoxSideTab->ScrollWidgetIntoView(focusWidget, false, EDescendantScrollDestination::IntoView);
	}
	else
	{
		_scrollBoxSideTab->SetScrollOffset(_captureData._scrollBoxSlotOffset);
	}
	

	// 2022/06/03 PKT - 파티 던전에서 부터 레드닷 생겼음.
	CheckRedDot();
}

void UGsUIWindowDungeon::OnSlectedSideSubTab(int InIndex)
{
	const auto& groupData = GSDungeon()->ViewGroupSet().IsValidIndex(InIndex) ? GSDungeon()->ViewGroupSet()[InIndex] : nullptr;
	if (nullptr == groupData)
	{
		return;
	}
	
	if (_activeDetailWidget.IsValid())
	{
		_activeDetailWidget.Get()->Leave();
		_activeDetailWidget.Reset();
	}

	switch (groupData->GetCategory())
	{
	case DungeonCategory::NORMAL:
	case DungeonCategory::SPECIAL:
	case DungeonCategory::ELITE:
		_switcherDungeonDetail->SetActiveWidgetIndex(0);
		break;
	case DungeonCategory::PARTY:
		_switcherDungeonDetail->SetActiveWidgetIndex(1);
		break;
	case DungeonCategory::ARENA:
		_switcherDungeonDetail->SetActiveWidgetIndex(2);
		break;
	};

	_activeDetailWidget = Cast<UGsUIPageDungeonGroupBase>(_switcherDungeonDetail->GetActiveWidget());
	if (_activeDetailWidget.IsValid())
	{
		_activeDetailWidget->Enter(groupData, _captureData);
	}	
}

void UGsUIWindowDungeon::OnSelectedSideTab(const FGsDungeonGroupBase* InSelectedData)
{
	if (nullptr == InSelectedData)
	{
		return;
	}

	int32 selectedIndex 
		= GSDungeon()->ViewGroupSet().IndexOfByPredicate([InSelectedData](const FGsDungeonGroupBase* InData)
		{
			return InSelectedData->GetSchemaData()->id == InData->GetSchemaData()->id;
		}
	);

	if (INDEX_NONE != selectedIndex)
	{
		_toggleGroupSideSubTab.SetSelectedIndex(selectedIndex);
	}
}

void UGsUIWindowDungeon::OnClickClose()
{
	Close();
}

void UGsUIWindowDungeon::Invalidate(DungeonGroupId InGroupId)
{
	// 2022/06/03 PKT - 파티 던전에서 부터 레드닷 생겼음.
	CheckRedDot();

	const int32 maxCount = _scrollBoxHelperSideTab->GetItemCount();
	for (int32 idx = 0; idx < maxCount; ++idx)
	{
		TWeakObjectPtr<UGsUISideTabDungeonGroupSlot> item = Cast<UGsUISideTabDungeonGroupSlot>(_scrollBoxHelperSideTab->GetEntry(idx));
		if (item.IsValid())
		{
			
			if (INVALID_DUNGEON_GROUP_ID == InGroupId		// 2023/2/21 PKT - GroupId가 없으면 항상 갱신
				|| (item.Get()->GetData() && item.Get()->GetData()->GetGroupId() == InGroupId))	// 2023/2/21 PKT - GroupId가 있으면 해당 Data만..갱신
			{
				item.Get()->Invalidate();

				if (INVALID_DUNGEON_GROUP_ID != InGroupId)
				{
					break;
				}
			}			
		}
	}

	if (_activeDetailWidget.IsValid())
	{
		if (INVALID_DUNGEON_GROUP_ID == InGroupId	// 2023/2/21 PKT - GroupId가 없으면 항상 갱신
			|| (_activeDetailWidget.Get()->GetData() && _activeDetailWidget.Get()->GetData()->GetGroupId() == InGroupId))	// 2023/2/21 PKT - GroupId가 있으면 해당 Data만..갱신
		{
			_activeDetailWidget.Get()->Invalidate();
		}
	}
}

void UGsUIWindowDungeon::GoSelectedDungeonGroup(int32 inMapId)
{
	const FGsDungeonGroupBase* groupData = GSDungeon()->FindGroupDataByMapId(inMapId);
	const FGsDungeonData* dungeonData = GSDungeon()->FindDungeonDataByMapId(inMapId);
	if (groupData && dungeonData && false == groupData->IsUIHidden())
	{
		_captureData._mapId = inMapId;
		int32 mainTabIndex = StaticCast<int32>(groupData->GetCategory());

		_toggleGroupTab.SetSelectedIndex(mainTabIndex, true);
		
		_captureData._mapId = 0;
	}
}

void UGsUIWindowDungeon::InvalidatePartyList(const TArray<FGsPartyDungeonPartyStatusList>* InPartyStatusList)
{
	if (nullptr == InPartyStatusList || false == _activeDetailWidget.IsValid())
	{	// 2022/06/02 PKT - 처리 필요 없음.
		return;
	}

	_activeDetailWidget->InvalidatePartyInfoSet(InPartyStatusList);
}

void UGsUIWindowDungeon::InvalidateBattleArena()
{
	if (_activeDetailWidget.IsValid() && _activeDetailWidget.Get()->GetData())
	{
		if (DungeonCategory::ARENA == _activeDetailWidget.Get()->GetData()->GetCategory())
		{
			TWeakObjectPtr<UGsUIPageDungeonGroupArena> widget = Cast<UGsUIPageDungeonGroupArena>(_activeDetailWidget.Get());
			if (widget.IsValid())
			{
				widget.Get()->Invalidate();
			}			
		}
	}
}