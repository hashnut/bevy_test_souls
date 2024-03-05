// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRankingPageGuildDungeon.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIContent/Window/Ranking/GsUIRankingGuildListEntry.h"
#include "UI/UIContent/Window/Ranking/GsRankingListItem.h"
#include "Ranking/GsRankingData.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGame/GsRankingManager.h"


void UGsUIRankingPageGuildDungeon::NativeConstruct()
{
	Super::NativeConstruct();

	_textBlockRankingUnit2->SetText(FText::GetEmpty());
}

void UGsUIRankingPageGuildDungeon::Show()
{
	Super::Show();

	if (nullptr == _subTabData)
		return;

	_textBlockRankingUnit2->SetText(_subTabData->rankUnitName2);
}

void UGsUIRankingPageGuildDungeon::InvalidateList()
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
				rankingList.Emplace(item);
			}
		}

	} while (0);

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

void UGsUIRankingPageGuildDungeon::InvalidateMyRanking()
{
	if (nullptr == _subTabData)
		return;

	_myRanking->SetVisibility((_subTabData->anotherServerList.Num() > 0) ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	_myRanking->SetData(GSRanking()->GetMyRanking(_subTabData->rankingType));
}
