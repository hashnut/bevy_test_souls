// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowRanking.h"

#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "Components/PanelWidget.h"

#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Ranking/GsSchemaRankingCategory.h"
#include "DataSchema/Ranking/GsSchemaRankingTypeObjective.h"
#include "DataSchema/Ranking/GsSchemaRankingGroupCategory.h"
#include "DataSchema/InterServer/World/GsSchemaWorldData.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsRankingManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"

#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/GsMessageContents.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "UI/UIContent/Common/GsUITabButtonCommon.h"
#include "UI/UIContent/Window/Ranking/GsUIRankingPageGuild.h"
#include "UI/UIContent/Window/Ranking/GsUIRankingPagePersonal.h"
#include "UI/UIContent/Window/Ranking/GsUIRankingPageGuildDungeon.h"
#include "UI/UIContent/Window/Ranking/GsUITabRankingGroup.h"
#include "UI/UIContent/Window/Ranking/GsUIRankingServerListEntry.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"

#include "T1Project.h"

void UGsUIWindowRanking::BeginDestroy()
{
	if (nullptr != _slotHelperMainTabBtn)
	{
		_slotHelperMainTabBtn->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowRanking::MainTabListRefresh);
		_slotHelperMainTabBtn = nullptr;
	}
	if (nullptr != _slotHelperSubTabBtn)
	{
		_slotHelperSubTabBtn->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowRanking::SubTabListRefresh);
		_slotHelperSubTabBtn = nullptr;
	}
	if (nullptr != _slotHelperGroupTabBtn)
	{
		_slotHelperGroupTabBtn->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowRanking::GroupTabListRefresh);
		_slotHelperGroupTabBtn = nullptr;
	}
	if (nullptr != _slotHelperServerList)
	{
		_slotHelperServerList->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowRanking::ServerListRefresh);
		_slotHelperServerList = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIWindowRanking::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowRanking::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowRanking::OnClickCloseAllStack);
	_btnRewardInfo->OnClicked.AddDynamic(this, &UGsUIWindowRanking::OnClickRewardInfo);
	_btnServer->OnClicked.AddDynamic(this, &UGsUIWindowRanking::OnClickServer);

	_toggleGroupMainTab.OnSelectChanged.BindUObject(this, &UGsUIWindowRanking::OnSelectChangedMainTab);
	_toggleGroupSubTab.OnSelectChanged.BindUObject(this, &UGsUIWindowRanking::OnSelectChangedSubTab);
	_toggleGroupGroupTab.OnSelectChanged.BindUObject(this, &UGsUIWindowRanking::OnSelectChangedGroupTab);

	if (nullptr == _slotHelperMainTabBtn)
	{
		_slotHelperMainTabBtn = NewObject<UGsDynamicPanelSlotHelper>(this);
		_slotHelperMainTabBtn->Initialize(_subClassOfMainTab, _rootPanelMainTab);
		_slotHelperMainTabBtn->OnRefreshEntry.AddDynamic(this, &UGsUIWindowRanking::MainTabListRefresh);
	}
	if (nullptr == _slotHelperSubTabBtn)
	{
		_slotHelperSubTabBtn = NewObject<UGsDynamicPanelSlotHelper>(this);
		_slotHelperSubTabBtn->Initialize(_subClassOfSubTab, _rootPanelSubTab);
		_slotHelperSubTabBtn->OnRefreshEntry.AddDynamic(this, &UGsUIWindowRanking::SubTabListRefresh);
	}
	if (nullptr == _slotHelperGroupTabBtn)
	{
		_slotHelperGroupTabBtn = NewObject<UGsDynamicPanelSlotHelper>(this);
		_slotHelperGroupTabBtn->Initialize(_subClassOfGroupTab, _rootPanelGroupTab);
		_slotHelperGroupTabBtn->OnRefreshEntry.AddDynamic(this, &UGsUIWindowRanking::GroupTabListRefresh);
	}
	if (nullptr == _slotHelperServerList)
	{
		_slotHelperServerList = NewObject<UGsDynamicPanelSlotHelper>(this);
		_slotHelperServerList->Initialize(_subClassOfServerList, _panelServerList);
		_slotHelperServerList->OnRefreshEntry.AddDynamic(this, &UGsUIWindowRanking::ServerListRefresh);
	}
}

void UGsUIWindowRanking::NativeConstruct()
{
	Super::NativeConstruct();

	_currentWorldList.Empty();
	if (FGsNetManager* netManager = GNet())
	{
		const TArray<ServerElem>& serverList = netManager->GetSeverList();
		for (const ServerElem& serverInfo : serverList)
		{
			PlanetWorldId planetWorldId(serverInfo.mPlanetWorldId);
			WorldId worldId = planetWorldId.st.worldId;
			_currentWorldList.Emplace(worldId);
		}
	}

	_worldList.Empty();
	_selectedServer = nullptr;

	InvalidateMainTab();

	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;

	if (rankingManager->IsSyncing())
	{
		rankingManager->SetIsFirstOpen(false);
	}
	else
	{
		rankingManager->SetIsFirstOpen(true);
		rankingManager->SetCurrentTabData(0, 0, 0, nullptr);
	}

	// 복구된 랭킹화면인가 ? 기존 탭 : 첫번째 탭
	int32 mainTabIndex = rankingManager->IsSyncing() ? rankingManager->GetMainTabIndex() : 0;
	_toggleGroupMainTab.SetSelectedIndex(mainTabIndex, true);
}

void UGsUIWindowRanking::NativeDestruct()
{
	_toggleGroupMainTab.Clear();
	_toggleGroupSubTab.Clear();
	_toggleGroupGroupTab.Clear();

	_pagePersonal->Clear();
	_pageGuild->Clear();
	_pageGuildDungeon->Clear();

	_currentWorldList.Empty();
	_worldList.Empty();
	_selectedServer = nullptr;

	Super::NativeDestruct();
}

bool UGsUIWindowRanking::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsRanking))
	{
		if (FGsRankingManager* rankingManager = GSRanking())
		{
			rankingManager->SetIsSyncing(true);
		}
		
		if (FGsMessageHolder* messageHolder = GMessage())
		{
			messageHolder->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_RANKING);
		}
		return true;
	}

	return false;
}

void UGsUIWindowRanking::InvalidateMainTab()
{
	_toggleGroupMainTab.Clear();

	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;

	int32 numMainTabs = rankingManager->GetNumMainTabs();
	if (0 >= numMainTabs)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[UGsUIWindowRanking::InvalidateMainTab] mainTabDataList is Empty"));
#endif
	}

	_slotHelperMainTabBtn->RefreshAll(numMainTabs);
}

void UGsUIWindowRanking::InvalidateSubTab()
{
	_toggleGroupSubTab.Clear();

	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;

	int32 selectedIndex = _toggleGroupMainTab.GetSelectedIndex();
	int32 numSubTabs = rankingManager->GetNumSubTabs(selectedIndex);

#if WITH_EDITOR
	if (0 >= numSubTabs)
	{
		GSLOG(Warning, TEXT("[UGsUIWindowRanking::InvalidateSubTab] subTabDataList is Empty"));
	}
#endif

	_slotHelperSubTabBtn->RefreshAll(numSubTabs);
}

void UGsUIWindowRanking::InvalidateSubTabSide()
{
	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;

	const FGsSchemaRankingTypeObjective* subTabData = rankingManager->GetSubTabData(_toggleGroupMainTab.GetSelectedIndex(), _toggleGroupSubTab.GetSelectedIndex());
	if (nullptr == subTabData)
		return;

	int32 numGroupTabs = rankingManager->GetNumGroupTabs(_toggleGroupMainTab.GetSelectedIndex(), _toggleGroupSubTab.GetSelectedIndex());

	if (subTabData->anotherServerList.Num() > 0)
	{
		_rootPanelSubTabSide->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_btnServer->SetVisibility(ESlateVisibility::Visible);

		if (0 >= numGroupTabs)
		{
			_panelGroupTab->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			if (nullptr == _selectedServer)
			{
				_textBlockServer->SetText(FText::FromString(TEXT("-")));
				_panelGroupTab->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				_textBlockServer->SetText(_selectedServer->worldName);
				_panelGroupTab->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}
	}
	else
	{
		_btnServer->SetVisibility(ESlateVisibility::Collapsed);
		
		if (0 >= numGroupTabs)
		{
			_rootPanelSubTabSide->SetVisibility(ESlateVisibility::Collapsed);
			_panelGroupTab->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			_rootPanelSubTabSide->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			_panelGroupTab->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}

	_slotHelperGroupTabBtn->RefreshAll(numGroupTabs);
}

void UGsUIWindowRanking::InvalidateServerList()
{
	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;

	const FGsSchemaRankingTypeObjective* subTabData = rankingManager->GetSubTabData(_toggleGroupMainTab.GetSelectedIndex(), _toggleGroupSubTab.GetSelectedIndex());
	if (nullptr == subTabData)
		return;

	_worldList.Empty();
	for (const FGsSchemaWorldDataRow& row : subTabData->anotherServerList)
	{
		const FGsSchemaWorldData* rowData = row.GetRow();
		if (nullptr == rowData)
			continue;

		if (false == _currentWorldList.Contains(rowData->worldId))
			continue;

		_worldList.Emplace(rowData);
	}
	_slotHelperServerList->RefreshAll(_worldList.Num());
}

void UGsUIWindowRanking::OnSelectChangedMainTab(int32 InIndex)
{
	InvalidateSubTab();

	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;

	// 복구된 랭킹화면인가 ? 기존 탭 : 첫번째 탭
	int32 subTabIndex = rankingManager->IsSyncing() ? rankingManager->GetSubTabIndex() : 0;
	_toggleGroupSubTab.SetSelectedIndex(subTabIndex, true);
}

void UGsUIWindowRanking::OnSelectChangedSubTab(int32 InIndex)
{
	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;
	
	_selectedServer = rankingManager->IsSyncing() ? rankingManager->GetSelectedServer() : _selectedServer;

	InvalidateSubTabSide();

	const FGsSchemaRankingTypeObjective* subTabData = rankingManager->GetSubTabData(_toggleGroupMainTab.GetSelectedIndex(), InIndex);
	if (nullptr == subTabData)
		return;
	
	if (subTabData->anotherServerList.Num() > 0)
	{
		if (nullptr == _selectedServer)
		{
			InvalidateServerList();
			_switcherPage->SetActiveWidgetIndex(EGsRankingPage::SERVER_LIST);
			return;
		}
	}

	const FGsSchemaRankingGroupCategory* groupTabData = rankingManager->GetGroupTabData(subTabData->index);
	if (nullptr == groupTabData)
	{
		// 별도의 분할 그룹 정보가 없는 서브탭
		rankingManager->SetIsSyncing(false);
		SetRankingPage(_toggleGroupMainTab.GetSelectedIndex(), InIndex);
	}
	else
	{
		// 별도의 분할 그룹 정보가 있는 서브탭
		int32 groupTabIndex = (rankingManager->IsSyncing() ? rankingManager->GetGroupTabIndex() : 0);
		if (rankingManager->IsFirstOpen())
		{
			if (FGsRankingMyData* myData = rankingManager->GetMyRanking(subTabData->rankingType))
			{
				if (myData->_hasHighRanking)
				{
					int32 index = 0;
					int32 myHighRank = myData->_highRanking;
					for (const FGsSchemaRankingGroupData& minMaxData : groupTabData->rankingGroupData)
					{
						if (myHighRank <= minMaxData.rankingGroupRangeMax)
						{
							groupTabIndex = index;
							break;
						}
						++index;
					}
				}
			}
			if (UWidget* targetTabWidget = _rootPanelGroupTab->GetChildAt(groupTabIndex))
			{
				_rootPanelGroupTab->ScrollWidgetIntoView(targetTabWidget, true, EDescendantScrollDestination::Center);
			}
		}
		_toggleGroupGroupTab.SetSelectedIndex(groupTabIndex, true);
	}
}

void UGsUIWindowRanking::OnSelectChangedGroupTab(int32 InIndex)
{
	// 그룹 탭이 선택되었을 때 IsFirstOpen 이면 
	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;

	rankingManager->SetIsSyncing(false);
	SetRankingPage(_toggleGroupMainTab.GetSelectedIndex(), _toggleGroupSubTab.GetSelectedIndex(), InIndex);
}

void UGsUIWindowRanking::MainTabListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUITabButtonCommon* entry = Cast<UGsUITabButtonCommon>(InEntry);
	if (nullptr == entry)
		return;

	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;

	const FGsSchemaRankingCategory* mainTabData = rankingManager->GetMainTabData(InIndex);
	if (nullptr == mainTabData)
		return;

	entry->SetTitleText(mainTabData->categoryName);
	_toggleGroupMainTab.AddToggleWidget(entry->GetToggleGroupEntry());
}

void UGsUIWindowRanking::SubTabListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUITabButtonCommon* entry = Cast<UGsUITabButtonCommon>(InEntry);
	if (nullptr == entry)
		return;

	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;

	const FGsSchemaRankingTypeObjective* subTabData = rankingManager->GetSubTabData(_toggleGroupMainTab.GetSelectedIndex(), InIndex);
	if (nullptr == subTabData)
		return;

	entry->SetTitleText(subTabData->rankingTabName);
	_toggleGroupSubTab.AddToggleWidget(entry->GetToggleGroupEntry());
}

void UGsUIWindowRanking::GroupTabListRefresh(int32 InIndex, UWidget* InEntry)
{
	UGsUITabRankingGroup* entry = Cast<UGsUITabRankingGroup>(InEntry);
	if (nullptr == entry)
		return;

	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;

	const FGsSchemaRankingTypeObjective* subTabData = rankingManager->GetSubTabData(_toggleGroupMainTab.GetSelectedIndex(), _toggleGroupSubTab.GetSelectedIndex());
	if (nullptr == subTabData)
		return;
	
	const FGsSchemaRankingGroupCategory* groupTabData = rankingManager->GetGroupTabData(subTabData->index);
	if (nullptr == groupTabData)
		return;

	if (false == groupTabData->rankingGroupData.IsValidIndex(InIndex))
		return;

	FText findText;
	FText::FindText(TEXT("RankingCommonUIText"), TEXT("RankingGroupCategory"), findText);
	FText groupText = FText::Format(findText, FText::AsNumber(groupTabData->rankingGroupData[InIndex].rankingGroupRangeMin), FText::AsNumber(groupTabData->rankingGroupData[InIndex].rankingGroupRangeMax));

	entry->SetTabText(groupText);
	_toggleGroupGroupTab.AddToggleWidget(entry->GetToggleGroupEntry());
}

void UGsUIWindowRanking::SetRankingPage(int32 InMainTabIndex, int32 InSubTabIndex, int32 InGroupTabIndex)
{
	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;

	const FGsSchemaRankingTypeObjective* subTabData = rankingManager->GetSubTabData(InMainTabIndex, InSubTabIndex);
	if (nullptr == subTabData)
		return;

	const FGsSchemaWorldData* selectedServer = _selectedServer;
	if (subTabData->anotherServerList.Num() == 0)
	{
		selectedServer = nullptr;
	}

	// 보고있는 탭 위치 및 데이터 저장
	rankingManager->SetCurrentTabData(InMainTabIndex, InSubTabIndex, InGroupTabIndex, selectedServer);

	_pagePersonal->Clear();
	_pageGuild->Clear();
	_pageGuildDungeon->Clear();

	// 보여져야 할 페이지 설정
	if (RankSearchType::PERSONAL == subTabData->rankingSearchType)
	{
		_pagePersonal->Show();
		_switcherPage->SetActiveWidgetIndex(EGsRankingPage::PERSONAL);
	}
	else
	{
		if (RankType::KNIGHTS_DUNGEON_CLEAR_RANK == subTabData->rankingType)
		{
			_pageGuildDungeon->Show();
			_switcherPage->SetActiveWidgetIndex(EGsRankingPage::GUILD_DUNGEON_CLEAR);
		}
		else
		{
			_pageGuild->Show();
			_switcherPage->SetActiveWidgetIndex(EGsRankingPage::GUILD);
		}
	}

	// 랭킹 주기 문장 설정
	SetRankingCheckCycle(subTabData->rankingCheckCycle, subTabData->rankingRewardCycle);
}

void UGsUIWindowRanking::SetRankingCheckCycle(const FString& InCheckCycle, const FString& InRewardCycle)
{
	FText fullGuideText;
	do 
	{
		if (InCheckCycle.IsEmpty())
			break;

		if (InRewardCycle.IsEmpty())
			break;

		// 랭킹 갱신 주기 전체 문장
		FText guideText;
		FText::FindText(TEXT("RankingCommonUIText"), TEXT("RankingGuide"), guideText);

		// 산정 주기
		TArray<FString> cronItemList;
		InCheckCycle.ParseIntoArray(cronItemList, TEXT(" "));

		FText nMinutesText;
		FText::FindText(TEXT("SystemText"), TEXT("NMinutes"), nMinutesText);
		FText firstText = FText::Format(nMinutesText, FText::FromString(cronItemList[1]));

		// 보상 주기
		cronItemList.Empty();
		InRewardCycle.ParseIntoArray(cronItemList, TEXT(" "));

		FText nHoursText;
		FText::FindText(TEXT("SystemText"), TEXT("NHours"), nHoursText);
		FText secondText = FText::Format(nHoursText, FText::FromString(cronItemList[2]));

		fullGuideText = FText::Format(guideText, firstText, secondText);
	} while (0);
	

	// 랭킹 갱신 주기 문장 설정
	_textBlockGuide->SetText(fullGuideText);
}

void UGsUIWindowRanking::OnUpdateRankingPage()
{
	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;

	if (rankingManager->IsFirstOpen())
	{
		_toggleGroupMainTab.SetSelectedIndex(rankingManager->GetMainTabIndex(), true);
		rankingManager->SetIsFirstOpen(false);
	}
	else
	{
		SetRankingPage(_toggleGroupMainTab.GetSelectedIndex(), _toggleGroupSubTab.GetSelectedIndex(), _toggleGroupGroupTab.GetSelectedIndex());
	}
}

void UGsUIWindowRanking::OnClickRewardInfo()
{
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->Open(TEXT("PopupRankingReward"));
	}
}

void UGsUIWindowRanking::ServerListRefresh(int32 InIndex, UWidget* InEntry)
{
	if (false == _worldList.IsValidIndex(InIndex))
		return;

	UGsUIRankingServerListEntry* entry = Cast<UGsUIRankingServerListEntry>(InEntry);
	if (nullptr == entry)
		return;

	if (entry->OnClickedServer.IsBound())
	{
		entry->OnClickedServer.Unbind();
	}
	entry->OnClickedServer.BindUObject(this, &UGsUIWindowRanking::OnSelectedServer);
	entry->SetData(_worldList[InIndex]);
}

void UGsUIWindowRanking::OnClickServer()
{
	InvalidateServerList();
	_switcherPage->SetActiveWidgetIndex(EGsRankingPage::SERVER_LIST);
}

void UGsUIWindowRanking::OnSelectedServer(const FGsSchemaWorldData* InWorldData)
{
	_selectedServer = InWorldData;

	OnSelectChangedSubTab(_toggleGroupSubTab.GetSelectedIndex());
}
