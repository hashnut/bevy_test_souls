// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/BattlePass/GsUIBattlePassMission.h"
#include "UI/UIContent/Window/BattlePass/GsUIBattlePassMissionItem.h"
#include "UI/UIContent/Window/BattlePass/GsUIBattlePassTab.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "WidgetSwitcher.h"
#include "PanelWidget.h"
#include "ScrollBox.h"

#include "BattlePass/GsBattlePassData.h"
#include "BattlePass/GsBattlePassMissionData.h"

void UGsUIBattlePassMission::BeginDestroy()
{
	if (nullptr != _tabScrollBoxHelper)
	{
		_tabScrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIBattlePassMission::OnCreateMissionTabEntry);
		_tabScrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIBattlePassMission::OnRefreshMissionTabEntry);
		_tabScrollBoxHelper = nullptr;
	}

	if (nullptr != _missionScrollBoxHelper)
	{
		_missionScrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIBattlePassMission::OnRefreshMissionItemEntry);
		_missionScrollBoxHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIBattlePassMission::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> childList = _missionScrollPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUIBattlePassMissionItem>())
		{
			child->RemoveFromParent();
		}
	}

	_tabScrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_tabScrollBoxHelper->Initialize(_tabWidgetClass, _missionTabPanel);
	_tabScrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIBattlePassMission::OnCreateMissionTabEntry);
	_tabScrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIBattlePassMission::OnRefreshMissionTabEntry);

	_missionScrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_missionScrollBoxHelper->Initialize(_listEntryWidgetClass, _missionScrollPanel);
	_missionScrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIBattlePassMission::OnRefreshMissionItemEntry);

	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIBattlePassMission::OnSelectedMissionTab);
}

void UGsUIBattlePassMission::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIBattlePassMission::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIBattlePassMission::SetData(TWeakPtr<FGsBattlePassData> passData)
{
	_battlePassData = passData;

	if (auto battlePass = _battlePassData.Pin())
	{	
		_missionTabList.Reset();
		battlePass->GetMissionTabList(_missionTabList);
	}
}

void UGsUIBattlePassMission::RefreshUI()
{
	_toggleGroup.Clear();

	TArray<UWidget*> childList = _missionTabPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUIBattlePassTab>())
		{
			child->RemoveFromParent();
		}
	}
	if (auto battlePass = _battlePassData.Pin())
	{
		_tabScrollBoxHelper->RefreshAll(_missionTabList.Num());

		if(_missionTabList.IsValidIndex(0))
		 _toggleGroup.SetSelectedIndex(0, true);
	}
}

void UGsUIBattlePassMission::OnCreateMissionTabEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIBattlePassTab>(InEntry))
	{
		_toggleGroup.AddToggleWidget(entry);
		entry->SetToggleInfo(entry->GetIndexInGroup());
	}
}

void UGsUIBattlePassMission::OnRefreshMissionTabEntry(int32 index, UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIBattlePassTab>(InEntry))
	{
		if (auto iter = _missionTabList.FindByPredicate([index](auto e) {
			return e.IsValid() && e.Pin()->GetTabIndex() == index; }))
		{
			entry->SetTitle((*iter).Pin()->GetMissionTabName() );
		}
	}
}

void UGsUIBattlePassMission::OnSelectedMissionTab(int tabIndex)
{
	auto tabOrder = _missionTabList.IndexOfByPredicate([tabIndex](auto e) { return e.IsValid() && e.Pin()->GetTabIndex() == tabIndex; });
	if(tabOrder != INDEX_NONE)
	{	
		_selectTabIndex = tabOrder;
		_openTimeDesc = _missionTabList[tabOrder].Pin()->GetMissionTabDesc();

		_missionScrollView->ScrollToStart();
		_missionScrollBoxHelper->RefreshAll(_missionTabList[tabOrder].Pin()->_missions.Num());
	}
}
void UGsUIBattlePassMission::UpdateMissionInfo(BattlePassMissionTabId tabId, BattlePassMissionId missionId)
{
	if (_missionTabList.IsValidIndex(_selectTabIndex) && _missionTabList[_selectTabIndex].Pin()->GetTabId() == tabId)
	{
		if (auto tab = _missionTabList[_selectTabIndex].Pin())
		{
			auto missionIndex = tab->_missions.IndexOfByPredicate([missionId](auto e) {
				if (e.IsValid()) return e.Pin()->GetMissionId() == missionId; return false; });

			if (missionIndex != INDEX_NONE)
			{
				if (auto entry = Cast<UGsUIBattlePassMissionItem>(_missionScrollBoxHelper->GetEntry(missionIndex)))
				{
					entry->RefreshUI();
				}
			}
		}
	}
}

void UGsUIBattlePassMission::OnRefreshMissionItemEntry(int32 index, UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIBattlePassMissionItem>(InEntry))
	{
		if (auto tab = _missionTabList[_selectTabIndex].Pin())
		{
			if (tab->_missions.IsValidIndex(index))
			{
				entry->SetData(tab->_missions[index]);
				entry->RefreshUI();
			}
		}
	}
}
