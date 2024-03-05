// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRankingPageBase.h"

#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

#include "UI/UIContent/Window/Ranking/GsRankingListItem.h"
#include "UI/UIContent/Window/Ranking/GsUIRankingListEntryBase.h"

#include "Management/ScopeGame/GsRankingManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Shared/Client/SharedEnums/SharedRankEnum.h"


void UGsUIRankingPageBase::Clear()
{
	_subTabData = nullptr;
	_listView->ClearListItems();
	_switcherList->SetActiveWidgetIndex(1);
}

void UGsUIRankingPageBase::Show()
{
	_listView->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;

	_subTabData = rankingManager->GetCurrentSubTabData();
	if (nullptr == _subTabData)
		return;

	_worldId = INVALID_WORLD_ID;
	bool isAnotherServer = false;
	if (_subTabData->anotherServerList.Num() > 0)
	{
		if (const FGsSchemaWorldData* selectedServer = rankingManager->GetSelectedServer())
		{
			_worldId = selectedServer->worldId;
			isAnotherServer = true;
		}
	}

	if (_worldId == INVALID_WORLD_ID)
	{
		if (const FGsNetUserData* userData = GGameData()->GetUserData())
		{
			_worldId = userData->mHomeWorldId;
		}
	}

	FText text;
	// 단일, 전체 서버 구분
	if (RankServerRangeType::LOCAL == _subTabData->serverRangeType)
	{
		FText::FindText(TEXT("RankingUnitNameText"), TEXT("KnightsName"), text);
		_textBlockServerGuild->SetText(text);
	}
	else
	{
		FText::FindText(TEXT("RankingUnitNameText"), TEXT("ServerName"), text);
		_textBlockServerGuild->SetText(text);
	}

	// 랭킹 산정 항목 텍스트 설정
	_textBlockRankingUnit->SetText(_subTabData->rankUnitName);

	_minRank = 1;
	_maxRank = _subTabData->maxRankCount;
	const FGsSchemaRankingGroupCategory* groupTabData = rankingManager->GetGroupTabData(_subTabData->index);
	if (nullptr != groupTabData)
	{
		if (groupTabData->rankingGroupData.IsValidIndex(rankingManager->GetGroupTabIndex()))
		{
			_minRank = groupTabData->rankingGroupData[rankingManager->GetGroupTabIndex()].rankingGroupRangeMin;
			_maxRank = groupTabData->rankingGroupData[rankingManager->GetGroupTabIndex()].rankingGroupRangeMax;
		}
	}

	if (false == rankingManager->HasRankingRequest(_subTabData->rankingType, _worldId, _minRank))
	{
		if (false == isAnotherServer && false == rankingManager->HasMyRanking(_subTabData->rankingType) && nullptr != groupTabData)
		{
			rankingManager->SetIsFirstOpen(true);
		}

		if (rankingManager->IsFirstOpen())
		{
			rankingManager->RequestRankingData(_subTabData->rankingType, _worldId, 0);
		}
		else
		{
			rankingManager->RequestRankingData(_subTabData->rankingType, _worldId, _minRank);
		}
	}

	_listView->ScrollToTop();

	InvalidateList();
	InvalidateMyRanking();
	ScrollListViewDefault();
}

void UGsUIRankingPageBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUIRankingPageBase::ScrollListViewDefault()
{
	FGsRankingManager* rankingManager = GSRanking();
	if (nullptr == rankingManager)
		return;

	if (false == rankingManager->IsFirstOpen())
		return;

	if (nullptr == _subTabData)
		return;

	FGsRankingMyData* rankingMyData = rankingManager->GetMyRanking(_subTabData->rankingType);
	if (nullptr == rankingMyData)
		return;

	if (false == rankingMyData->_hasHighRanking)
		return;

	float ranking = rankingMyData->_highRanking - _minRank;

	float displayedCount = 0;
	UWidget* emptyPanel = _switcherList->GetWidgetAtIndex(1);
	if (nullptr != emptyPanel)
	{
		float panelHeight = emptyPanel->GetCachedGeometry().GetLocalSize().Y;
		float entryHeight = _myRanking->GetCachedGeometry().GetLocalSize().Y;
		displayedCount = panelHeight / entryHeight;
	}

	if (ranking < displayedCount)
	{
		_listView->ScrollToTop();
	}
	else
	{
		_listView->SetScrollOffset(ranking - displayedCount);
	}
}
