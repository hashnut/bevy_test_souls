// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupContribution.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIContent/Popup/Contribution/GsContributionListViewEntry.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "GameFlow/Stage/GsStageMode.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/Stage/StageGame/GsStageGameGuildDungeon.h"
#include "Guild/GsGuildContributionData.h"
#include "Guild/GsAgitDungeonRaidData.h"


void UGsUIPopupContribution::BeginDestroy()
{
	if (_poolListItem)
	{
		_poolListItem->RemovePool();
	}
	_poolListItem = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupContribution::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_poolListItem = NewObject<UGsPoolUObject>(this); //UGsContributionListViewEntryData
}

void UGsUIPopupContribution::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();

	if (FGsGameDataManager* gameDataMgr = GGameData())
	{
		if (const FGsNetUserData* userData = gameDataMgr->GetUserData())
		{
			_strMyName = userData->mUserName;
		}	
	}

	_playerContribution->SetContribution(0, _strMyName, 0);
	_listView->ClearListItems();		
}

void UGsUIPopupContribution::NativeDestruct()
{
	UnbindMessages();

	Super::NativeDestruct();
}

void UGsUIPopupContribution::BindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	MGuildDungeon& msgGuildDungeon = msgMgr->GetGuildDungeon();
	_listGuildDungeonDelegate.Emplace(msgGuildDungeon.AddUObject(MessageContentGuildDungeon::UPDATE_GUILD_DUNGEON_RANKING,
		this, &UGsUIPopupContribution::OnUpdateGuildDungeonRanking));
	_listGuildDungeonDelegate.Emplace(msgGuildDungeon.AddUObject(MessageContentGuildDungeon::UPDATE_AGIT_RAID_RANKING,
		this, &UGsUIPopupContribution::OnUpdateAgitRaidRanking));
}

void UGsUIPopupContribution::UnbindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	MGuildDungeon& msgGuildDungeon = msgMgr->GetGuildDungeon();
	for (MsgGuildDungeonHandle& iter : _listGuildDungeonDelegate)
	{
		msgGuildDungeon.Remove(iter);
	}
	_listGuildDungeonDelegate.Empty();
}

void UGsUIPopupContribution::OnUpdateGuildDungeonRanking(const IGsMessageParam*)
{
	_rankingList.Empty();

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsStageManagerGame* stageMgr = gameFlow->GetStageManagerGame();
	if (nullptr == stageMgr)
	{
		return;
	}

	TSharedPtr<FGsStageGameBase> stage = stageMgr->Find(FGsStageMode::Game::GUILD_DUNGEON);
	FGsStageGameGuildDungeon* guildDungeon = static_cast<FGsStageGameGuildDungeon*>(stage.Get());
	if (nullptr == guildDungeon)
	{
		return;
	}

	guildDungeon->GetSortedRankingList(_rankingList);

	InvalidateList();
}

void UGsUIPopupContribution::OnUpdateAgitRaidRanking(const IGsMessageParam*)
{
	_rankingList.Empty();

	if (FGsAgitDataController* agitController = GGuild()->GetAgitDataController())
	{
		if (FGsAgitDungeonRaidData* raidData = agitController->GetRaidData())
		{
			raidData->GetSortedRankingList(_rankingList);
		}
	}

	InvalidateList();
}

void UGsUIPopupContribution::InvalidateList()
{
	_poolListItem->ReleaseAll();
	_listView->ClearListItems();

	UGsContributionListViewEntryData* myListItem = nullptr;
	TArray<UGsContributionListViewEntryData*> rankingListItems;

	for (TWeakPtr<FGsContributionData> iter : _rankingList)
	{
		if (false == iter.IsValid())
		{
			continue;
		}

		UGsContributionListViewEntryData* listItem = _poolListItem->Claim<UGsContributionListViewEntryData>();
		listItem->_rank = iter.Pin()->_rank;
		listItem->_userName = iter.Pin()->_userName;
		listItem->_contribution = iter.Pin()->_contribution;

		rankingListItems.Emplace(listItem);

		if (_strMyName == iter.Pin()->_userName)
		{
			myListItem = listItem;
		}
	}

	// 리스트표시
	_listView->SetListItems(rankingListItems);

	// 빈 리스트일 경우에 대한 표시
	_switcherList->SetActiveWidgetIndex((0 < rankingListItems.Num()) ? 1 : 0);

	// 내 랭킹 표시
	if (myListItem)
	{
		_playerContribution->InvalidateAll(myListItem);
	}
	else
	{
		_playerContribution->SetContribution(0, _strMyName, 0);
	}

	// 강제 갱신 호출
	const TArray<UUserWidget*> entryList = _listView->GetDisplayedEntryWidgets();
	for (UUserWidget* widget : entryList)
	{
		if (UGsContributionListViewEntry* listEntry = Cast<UGsContributionListViewEntry>(widget))
		{
			listEntry->RefreshAll();
		}
	}
}
