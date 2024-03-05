// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRankingPagePersonal.h"

#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsRankingManager.h"

#include "UI/UIContent/Window/Ranking/GsUIRankingPersonalListEntry.h"
#include "UI/UIContent/Window/Ranking/GsRankingListItem.h"

#include "Ranking/GsRankingHelper.h"
#include "Ranking/GsRankingData.h"

#include "Templates/Less.h"


void UGsUIRankingPagePersonal::InvalidateList()
{
	_listView->ClearListItems();

	TArray<UGsRankingListItem*> rankingList;
	do
	{
		FGsRankingManager* rankingManager = GSRanking();
		if (nullptr == rankingManager)
			break;

		if (nullptr == _subTabData)
			break;

		if (false == rankingManager->HasRankingRequest(_subTabData->rankingType, _worldId, _minRank))
			break;

		TArray<const FGsRankingData*> dataList = rankingManager->GetRankingList(_subTabData->rankingType, _worldId, _minRank, _maxRank);
		int32 maxIndex = (_subTabData->maxRankCount < dataList.Num()) ? _subTabData->maxRankCount : dataList.Num();
		for (int32 i = 0; i < maxIndex; ++i)
		{
			if (dataList.IsValidIndex(i))
			{
				const FGsRankingData* rankingData = dataList[i];
				UGsRankingListItem* item = NewObject<UGsRankingListItem>(this);
				item->SetData(rankingData);
				item->SetClickListItemCallback([this](UWidget* InWidget)
					{
						if (UGsUIRankingPersonalListEntry* entry = Cast<UGsUIRankingPersonalListEntry>(InWidget))
						{
							OnOpenUserContext(Cast<UGsRankingListItem>(entry->GetListItem()));
						}
					});
				rankingList.Emplace(item);
			}
		}

	} while (0);

	const FSoftObjectPath* tempBadge = FGsRankingHelper::GetBadgeIconPath(1, _subTabData->rankingType);
	ESlateVisibility visibility = (nullptr != tempBadge && tempBadge->IsValid()) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
	_spacerBadge->SetVisibility(visibility);
	_boxBadge->SetVisibility(visibility);

	const int32 DATA_EXIST = 0;
	const int32 DATA_NO_EXIST = 1;
	if (rankingList.Num() > 0)
	{
		_switcherList->SetActiveWidgetIndex(DATA_EXIST);
		_listView->SetListItems(rankingList);
	}
	else
	{
		_switcherList->SetActiveWidgetIndex(DATA_NO_EXIST);
	}
}

void UGsUIRankingPagePersonal::InvalidateMyRanking()
{
	if (nullptr == _subTabData)
		return;

	_myRanking->SetVisibility((_subTabData->anotherServerList.Num() > 0) ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	_myRanking->SetData(GSRanking()->GetMyRanking(_subTabData->rankingType));
}

void UGsUIRankingPagePersonal::OnOpenUserContext(UGsRankingListItem* InListItem)
{
	if (nullptr == InListItem->_data)
		return;

	const TArray<UUserWidget*>& listEntrys = _listView->GetDisplayedEntryWidgets();
	if (listEntrys.Num() <= 0)
		return;

	TArray<int32> rankingList;
	for (int32 i = 0; i < listEntrys.Num(); ++i)
	{
		if (UGsUIRankingPersonalListEntry* displayedFirstData = Cast<UGsUIRankingPersonalListEntry>(listEntrys[i]))
		{
			rankingList.Emplace(Cast<UGsRankingListItem>(displayedFirstData->GetListItem())->_data->_ranking);
		}
	}
	rankingList.Sort(TLess<int32>());

	if (rankingList.Num() > 0)
	{
		if (rankingList[0] == InListItem->_data->_ranking)
		{
			_listView->SetScrollOffset(InListItem->_data->_ranking - _minRank);
		}
		if (rankingList.Last() == InListItem->_data->_ranking)
		{
			float listViewHeight = _listView->GetCachedGeometry().GetLocalSize().Y;
			float entryHeight = _listView->GetDisplayedEntryWidgets()[0]->GetCachedGeometry().GetLocalSize().Y;
			float displayedCount = listViewHeight / entryHeight;

			_listView->SetScrollOffset(InListItem->_data->_ranking - _minRank + 1 - displayedCount);
		}
	}
}