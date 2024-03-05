// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIBattlePassPage.h"

#include "Management/ScopeGame/GsBattlePassManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "UI/UIContent/Window/BattlePass/GsUIBattlePassLevel.h"
#include "UI/UIContent/Window/BattlePass/GsUIBattlePassMission.h"
#include "UI/UIContent/Common/GsUIProgressBar.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Window/BattlePass/GsUIBattlePassTab.h"

#include "Engine/AssetManager.h"

#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Algo/ForEach.h"

void UGsUIBattlePassPage::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIBattlePassPage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_tabToggleGroup.OnSelectChanged.BindUObject(this, &UGsUIBattlePassPage::OnSelectedTab);
	_tabToggleGroup.AddToggleWidget(_levelTab);
	_tabToggleGroup.AddToggleWidget(_missionTab);
	_levelTab->SetToggleInfo(_levelTab->GetIndexInGroup());
	_missionTab->SetToggleInfo(_missionTab->GetIndexInGroup());

	FText levelTitle;
	FText::FindText(TEXT("BattlePassText"), TEXT("UI_PassNormal_Reward"), levelTitle);

	_levelTab->SetTitle(levelTitle);

	FText missionTitle;
	FText::FindText(TEXT("BattlePassText"), TEXT("UI_PassNormal_Mission"), missionTitle);

	_missionTab->SetTitle(missionTitle);
}

void UGsUIBattlePassPage::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIBattlePassPage::OnSelectedTab(int tabIndex)
{
	_selectedTabType = tabIndex == 0 ? PASSTAB::LEVEL : PASSTAB::MISSON;
	_switcherLevelMission->SetActiveWidgetIndex(tabIndex > 0 ? 1: 0);

	if(_selectedTabType == PASSTAB::LEVEL)
		 _contentLevel->RefreshUI();
	else _contentMission->RefreshUI();

}

void UGsUIBattlePassPage::SetSelectedBattlePass(TWeakPtr<FGsBattlePassData> battlePassData)
{
	_battlePassData = battlePassData;
	_contentLevel->SetData(battlePassData);
	_contentMission->SetData(battlePassData);

	if (auto passData = _battlePassData.Pin())
	{
		_tabToggleGroup.SetSelectedIndex(0, true);
	}
}

void UGsUIBattlePassPage::Reset()
{
	_battlePassData.Reset();

	_contentLevel->SetData(nullptr);
	_contentMission->SetData(nullptr);

	_contentLevel->RefreshUI();
	_contentMission->RefreshUI();
}

void UGsUIBattlePassPage::SetActiveAddReward()
{
	_contentLevel->SetActiveAddReward();
}

void UGsUIBattlePassPage::AckReceiveReward(Level level, bool isAddReward)
{
	_contentLevel->SetRecvReward(level, isAddReward);
}

void UGsUIBattlePassPage::AckReceiveRewardAll(const TArray<Level>& level)
{
	_contentLevel->OnAckReceiveBattlePassLevelRewardAll(level);
}

void UGsUIBattlePassPage::UpdateBattlePassLevel()
{
	_contentLevel->RefreshUI(false);
}

void UGsUIBattlePassPage::UpdateBattlePassMission(BattlePassMissionTabId tabId, BattlePassMissionId missionId)
{
	if (_battlePassData.IsValid())
	{
		_contentMission->UpdateMissionInfo(tabId, missionId);
	}
}

void UGsUIBattlePassPage::OnUpdateRedDot()
{
	_contentLevel->OnUpdateRedDot();
}
