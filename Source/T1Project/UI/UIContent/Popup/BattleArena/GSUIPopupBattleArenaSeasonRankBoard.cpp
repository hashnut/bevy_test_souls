#include "GSUIPopupBattleArenaSeasonRankBoard.h"
#include "../BattleArenaRank/GsBattleArenaRankData.h"
#include "../Message/MessageParam/GsMessageParam.h"
#include "../Message/GsMessageBattleArena.h"
#include "../Message/MessageParam/GsBattleArenaMessageParam.h"
#include "../Management/GsMessageHolder.h"
#include "../Management/ScopeGame/GsBattleArenaSeasonRankManager.h"

#include "../Classes/GsPoolUObject.h"

#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/ListView.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/RichTextBlock.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/ScrollBox.h"

//#include "../UI/UIControlLib/ContentWidget/GsScrollBox.h"
#include "../UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "GsUIBattleArenaSeasonRankItemEntry.h"
#include "GsUIBattleArenaSeasonRankRewardItemEntry.h"
#include "GsUIBattleArenaSeasonTierRewardItemEntry.h"
#include "GsUIBattleArenaSeasonRankTierCategoryEntry.h"

#include "DataSchema/BattleArena/GsSchemaBattleArenaTierData.h"
#include "DataSchema/BattleArena/GsSchemaBattleArenaSeasonData.h"

#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaHelper.h"

#include "T1Project.h"




void UGSUIPopupBattleArenaSeasonRankBoard::BeginDestroy()
{	
	if (nullptr != _tierCategoryPanelSlotHelper)
	{
		_tierCategoryPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGSUIPopupBattleArenaSeasonRankBoard::OnRefreshTierCategory);
		_tierCategoryPanelSlotHelper = nullptr;
	}

	if (_poolRankItemData)
	{
		_poolRankItemData->RemovePool();
		_poolRankItemData = nullptr;
	}

	Super::BeginDestroy();
}

void UGSUIPopupBattleArenaSeasonRankBoard::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _tierCategoryPanelSlotHelper)
	{
		_tierCategoryPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_tierCategoryPanelSlotHelper->Initialize(_tierCategoryEntryWidgetClass, _tierCategoryRoot);
		_tierCategoryPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGSUIPopupBattleArenaSeasonRankBoard::OnRefreshTierCategory);
	}
	
	if (nullptr == _tierRewardPanelSlotHelper)
	{
		_tierRewardPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_tierRewardPanelSlotHelper->Initialize(_tierRewardEntryWidgetClass, _scrollBoxTierRewardListRoot);
		_tierRewardPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGSUIPopupBattleArenaSeasonRankBoard::OnRefreshTierReward);
	}

	if (nullptr == _rankRewardPanelSlotHelper)
	{
		_rankRewardPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_rankRewardPanelSlotHelper->Initialize(_rankRewardEntryWidgetClass, _scrollBoxRankRewardListRoot);
		_rankRewardPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGSUIPopupBattleArenaSeasonRankBoard::OnRefreshRankReward);
	}
	
	_toggleMainTabGroup.AddToggleWidget(_btnSeasonRank);
	_toggleMainTabGroup.AddToggleWidget(_btnSeasonReward);
	_toggleMainTabGroup.OnSelectChanged.BindUObject(this, &UGSUIPopupBattleArenaSeasonRankBoard::OnSelectedMainTab);

	_toggleTierCategoryGroup.OnSelectChanged.BindUObject(this, &UGSUIPopupBattleArenaSeasonRankBoard::OnSelectedTierCategory);

	_toggleRewardTabGroup.AddToggleWidget(_btnTeirReward);
	_toggleRewardTabGroup.AddToggleWidget(_btnRankReward);
	_toggleRewardTabGroup.OnSelectChanged.BindUObject(this, &UGSUIPopupBattleArenaSeasonRankBoard::OnSelectedRewardTab);

	_btnConfirm->OnClicked.AddDynamic(this, &UGSUIPopupBattleArenaSeasonRankBoard::OnClickedConfirm);

	// 2024/1/31 PKT - Pool( 넉넉히 잡자. )
	const static int32 POOL_WARM_UP_SIZE = 100;
	_poolRankItemData = NewObject<UGsPoolUObject>(this);
	_poolRankItemData->WarmUp<UGsBattleArenaSeasonRankItemEntryData>(POOL_WARM_UP_SIZE);
}

void UGSUIPopupBattleArenaSeasonRankBoard::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessage();

	_rankSearchType = RankSearchType::MAX;

	_listViewTierRankItem->ClearListItems();
	_poolRankItemData->ReleaseAll();

	_widgetMySeasonRank->SetVisibility(ESlateVisibility::Collapsed);
}

void UGSUIPopupBattleArenaSeasonRankBoard::NativeDestruct()
{
	_listViewTierRankItem->ClearListItems();
	_poolRankItemData->ReleaseAll();

	UnBindMessage();

	if (FGsBattleArenaSeasonRankManager* manager = GsBattleArenaRank())
	{	// 2024/2/1 PKT - 모든 활동 중단.
		manager->ClearCurrentTask(_rankSearchType);
	}

	_rankSearchType = RankSearchType::MAX;

	Super::NativeDestruct();
}

void UGSUIPopupBattleArenaSeasonRankBoard::BindMessage()
{
	UnBindMessage();

	if (FGsMessageHolder* messageHolder = GMessage())
	{
		_battleArenaRankDelegates.Emplace(
			messageHolder->GetBattleArenaRank().AddUObject(
				MessageContentBattleRank::LOCAL_RANK_DATA, this, &UGSUIPopupBattleArenaSeasonRankBoard::OnDelegateInvalidateLocalData)
		);

		_battleArenaRankDelegates.Emplace(
			messageHolder->GetBattleArenaRank().AddUObject(
				MessageContentBattleRank::RESET_RANK_DATA, this, &UGSUIPopupBattleArenaSeasonRankBoard::OnDelegateResetRankData)
		);

		_battleArenaRankDelegates.Emplace(
			messageHolder->GetBattleArenaRank().AddUObject(
				MessageContentBattleRank::CREATE_RANK_DATA, this, &UGSUIPopupBattleArenaSeasonRankBoard::OnDelegateCreateRankList)
		);

		_battleArenaRankDelegates.Emplace(
			messageHolder->GetBattleArenaRank().AddUObject(
				MessageContentBattleRank::UPDATE_RANK_DATA, this, &UGSUIPopupBattleArenaSeasonRankBoard::OnDelegateInvalidateRankData)
		);
	}
}

void UGSUIPopupBattleArenaSeasonRankBoard::UnBindMessage()
{
	if (FGsMessageHolder* messageHolder = GMessage())
	{
		for (auto item : _battleArenaRankDelegates)
		{
			messageHolder->GetBattleArenaRank().Remove(item);
		}
		_battleArenaRankDelegates.Empty();
	}
}

void UGSUIPopupBattleArenaSeasonRankBoard::OnDelegateInvalidateLocalData(const IGsMessageParam* InParam)
{
	const FGsPrimitiveInt32* param = static_cast<const FGsPrimitiveInt32*>(InParam);
	if (nullptr == param)
	{
		GSLOG(Error, TEXT("nullptr == param"));
		return;
	}

	bool isNewData = static_cast<bool>(param->_data);

	// 2024/1/25 PKT - Main Tab 위치에 따라 달라 진다.
	const static int32 RANK_VIEW = 0;
	const static int32 REWARD_VIEW = 1;

	const int32 mainIndex = _toggleMainTabGroup.GetSelectedIndex();
	if (RANK_VIEW == mainIndex)
	{	// 2024/2/1 PKT - 내 정보 갱신
		BuildMySeasonTierData();
		
		if (isNewData)
		{
			BuildMainRankView(true);						
		}
	}
	else if (REWARD_VIEW == mainIndex)
	{
		if (isNewData)
		{
			const int32 index = _toggleRewardTabGroup.GetSelectedIndex();
			_toggleRewardTabGroup.SetSelectedIndex(index, true);
		}		
	}
}

void UGSUIPopupBattleArenaSeasonRankBoard::OnDelegateResetRankData(const IGsMessageParam* InParam)
{
	// 2024/1/26 PKT - 초기화 되었다. 딱히 Action 없음.
}

void UGSUIPopupBattleArenaSeasonRankBoard::OnDelegateCreateRankList(const IGsMessageParam* InParam)
{
	const FGsBattleArenaRankCreateData* param = static_cast<const FGsBattleArenaRankCreateData*>(InParam);
	if (nullptr == param)
	{
		GSLOG(Error, TEXT("nullptr == param"));
		return;
	}

	const static int32 RANK_VIEW = 0;
	if (RANK_VIEW != _toggleMainTabGroup.GetSelectedIndex())
	{
		return;
	}

	if (const FGsSchemaBattleArenaTierData* tierData = GsBattleArenaRank()->FindTierData(_rankSearchType, _toggleTierCategoryGroup.GetSelectedIndex()))
	{
		BuildTierRankList(_rankSearchType, tierData->id);
	}
}

void UGSUIPopupBattleArenaSeasonRankBoard::OnDelegateInvalidateRankData(const IGsMessageParam* InParam)
{
	const FGsBattleArenaRankUpdateData* param = static_cast<const FGsBattleArenaRankUpdateData*>(InParam);
	if (nullptr == param)
	{
		GSLOG(Error, TEXT("nullptr == param"));
		return;
	}

	const static int32 RANK_VIEW = 0;
	if (RANK_VIEW != _toggleMainTabGroup.GetSelectedIndex())
	{
		return;
	}

	const FGsSchemaBattleArenaTierData* tierData = GsBattleArenaRank()->FindTierData(_rankSearchType, _toggleTierCategoryGroup.GetSelectedIndex());
	if (nullptr == tierData || param->_tierDataId != tierData->id)
	{
		return;
	}

	const TArray<UUserWidget*>& displayWidgets = _listViewTierRankItem->GetDisplayedEntryWidgets();
	for (UUserWidget* widget : displayWidgets)
	{
		TWeakObjectPtr<UGsUIBattleArenaSeasonRankItemEntry> item = Cast<UGsUIBattleArenaSeasonRankItemEntry>(widget);
		if (item.IsValid())
		{	// 2024/1/26 PKT - 내부 적으로 갱신이 필요한 애들만 갱신 하게 된다.
			item->Invalidate();
		}
	}
}

void UGSUIPopupBattleArenaSeasonRankBoard::BuildMainRankView(bool IsMyRankFocus /* = false */)
{
	// 2024/1/24 PKT - Make Tier Category
	const int32 maxCount = GsBattleArenaRank()->SeasonTierCount(_rankSearchType);
	_tierCategoryPanelSlotHelper->RefreshAll(maxCount);

	const int32 DEFAULT_TIER_CATEGORY_INDEX = 0;
	int32 categorySelectedIndex = DEFAULT_TIER_CATEGORY_INDEX;
	float listViewOffset = 0.f;

	/**
	 * Focus를 맞춰 줄때 내 정보가 있고, 순위가 존재 하면 내 Tier에 Focus..
	 * 위의 조건에 하나라도 맞지 않다면 전체 순위 중 가장 높은 등급의 순위를 우선 보여 준다.
	 */
	const FGsBattleArenaRankLocalData* localRankData = GsBattleArenaRank()->GetLocalSeasonRanklData(_rankSearchType);
	if (localRankData && localRankData->IsValidRankData())
	{	
		GuildWarTierDataId localTierDataId 
			= (localRankData->IsValidGuildData()) ? localRankData->_rankData._tierDataId : INVALID_GUILD_WAR_TIER_DATA_ID;

		if (IsMyRankFocus && INVALID_GUILD_WAR_TIER_DATA_ID != localTierDataId)
		{
			// 2024/2/22 PKT - 내 정보에 Tier정보가 있으면 내 Tier에 포커스
			for (int32 idx = 0; idx < maxCount; ++idx)
			{
				const FGsSchemaBattleArenaTierData* schemaTierData = GsBattleArenaRank()->FindTierData(_rankSearchType, idx);
				if (schemaTierData && (schemaTierData->id == localTierDataId))
				{
					categorySelectedIndex = idx;

					listViewOffset = StaticCast<float>(localRankData->_rankData._index);

					break;
				}
			}
		}
		else
		{
			// 2024/2/22 PKT - 현존하는 가장 높은 순위 Tier에 포커스
			for (int32 idx = 0; idx < maxCount; ++idx)
			{
				const FGsSchemaBattleArenaTierData* schemaTierData = GsBattleArenaRank()->FindTierData(_rankSearchType, idx);
				if (nullptr == schemaTierData)
				{
					continue;
				}

				const int32 size = GsBattleArenaRank()->BuildRankListByTier(_rankSearchType, schemaTierData->id);
				if (0 < size)
				{
					categorySelectedIndex = idx;

					listViewOffset = 0;

					break;
				}
			}
		}
	}
		
	// 2024/1/31 PKT - 카테고리 Selected
	_toggleTierCategoryGroup.SetSelectedIndex(categorySelectedIndex, true);
	
	if (UWidget* entryWidget = _tierCategoryPanelSlotHelper->GetEntry(categorySelectedIndex))
	{
		_scrollBoxTierCategory->ScrollWidgetIntoView(entryWidget, false,EDescendantScrollDestination::Center);
	}

	// 2024/1/31 PKT - List View Offset
	_listViewTierRankItem->SetScrollOffset(listViewOffset);
}

void UGSUIPopupBattleArenaSeasonRankBoard::BuildMainRewardView()
{
	const int32 DEFAULT_TIER_REWARD_INDEX = 0;
	_toggleRewardTabGroup.SetSelectedIndex(DEFAULT_TIER_REWARD_INDEX, true);
}

void UGSUIPopupBattleArenaSeasonRankBoard::BuildTierRankList(RankSearchType InRankSearchType, GuildWarTierDataId InTierDataId)
{
	const int32 size = GsBattleArenaRank()->BuildRankListByTier(InRankSearchType, InTierDataId);

	const static int32 FILL_INDEX = 0;
	const static int32 EMPTY_INDEX = 1;
	if (0 > size)
	{	// 2024/1/24 PKT - 아직 서버로 부터 어떠한 Data도 전달 받지 못했다.
		_switcherTierRankList->SetActiveWidgetIndex(EMPTY_INDEX);

		FText message;
		FGsBattleArenaHelper::FindText(TEXT("UI_Ranking_LoadingRank"), message);		
		_textEmptyReason->SetText(message);		
	}
	else if (0 == size)
	{	// 2024/1/24 PKT - 서버로 부터 전달은 받았지만, 리스트가 없다.(해당 티어 존재하는 Data가 없다.)
		_switcherTierRankList->SetActiveWidgetIndex(EMPTY_INDEX);
		
		FText message;
		FGsBattleArenaHelper::FindText(TEXT("UI_Ranking_NoTier"), message);
		_textEmptyReason->SetText(message);
	}
	else
	{
		_switcherTierRankList->SetActiveWidgetIndex(FILL_INDEX);

		_poolRankItemData->ReleaseAll();
		_listViewTierRankItem->ClearListItems();
		
		TArray<UGsBattleArenaSeasonRankItemEntryData*> entryDataSet;
		for (int32 idx=0; idx < size; ++idx)
		{
			UGsBattleArenaSeasonRankItemEntryData* data = _poolRankItemData->Claim<UGsBattleArenaSeasonRankItemEntryData>();
			data->SetData(InRankSearchType, InTierDataId, idx);

			entryDataSet.Emplace(data);
		}

		_listViewTierRankItem->SetListItems(entryDataSet);
		_listViewTierRankItem->RegenerateAllEntries();
		_listViewTierRankItem->SetScrollOffset(0.f);
		_listViewTierRankItem->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	}
}

void UGSUIPopupBattleArenaSeasonRankBoard::BuildMySeasonTierData()
{
	const FGsSchemaBattleArenaSeasonData* shcemaSeasonData = GsBattleArenaRank()->GetSeasonData(_rankSearchType);
	if (nullptr == shcemaSeasonData)
	{
		GSLOG(Error, TEXT("nullptr == shcemaSeasonData"));
		return;
	}	

	int32 playCount = 0;
	const FGsBattleArenaRankLocalData* localSeasonRankData = GsBattleArenaRank()->GetLocalSeasonRanklData(_rankSearchType);	
	if (nullptr == localSeasonRankData || false == localSeasonRankData->IsValidGuildData())
	{
		_widgetMySeasonRank->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_widgetMySeasonRank->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_widgetMyRankData->SetData(_rankSearchType, &(localSeasonRankData->_rankData));
		playCount = localSeasonRankData->_playCount;
	}

	FText textFormat;
	// 2024/1/31 PKT - "(현재 참여 횟수: {0}회)"
	FGsBattleArenaHelper::FindText(TEXT("Notice_Ranking_Reward_MyCondition"), textFormat);	
	_ritchTextRicPlayCount->SetText(FText::Format(textFormat, { playCount }));
}

void UGSUIPopupBattleArenaSeasonRankBoard::OnSelectedMainTab(int32 index)
{
	_switcherMainBoard->SetActiveWidgetIndex(index);
	_switcherSeasonDesc->SetActiveWidgetIndex(index);

	const static int32 RANK_VIEW = 0;
	const static int32 REWARD_VIEW = 1;
	if (RANK_VIEW == index)
	{
		BuildMainRankView(false);
	}
	else if (REWARD_VIEW == index)
	{
		BuildMainRewardView();
	}
}

void UGSUIPopupBattleArenaSeasonRankBoard::OnSelectedTierCategory(int32 index)
{
	const FGsSchemaBattleArenaTierData* tierData = GsBattleArenaRank()->FindTierData(_rankSearchType, index);
	if (nullptr == tierData)
	{
		GSLOG(Error, TEXT("nullptr == tierData"));
		return;
	}

	BuildTierRankList(_rankSearchType, tierData->id);
}

void UGSUIPopupBattleArenaSeasonRankBoard::OnSelectedRewardTab(int32 index)
{
	_switcherRewardList->SetActiveWidgetIndex(index);

	const static int32 TIER_REWARD_VIEW = 0;
	const static int32 RANK_REWARD_VIEW = 1;
	if (TIER_REWARD_VIEW == index)
	{
		const int32 maxCount = GsBattleArenaRank()->SeasonTierCount(_rankSearchType);
		_tierRewardPanelSlotHelper->RefreshAll(maxCount);

		const UGsUIBattleArenaSeasonTierRewardItemEntry* focusEntry = nullptr;
		for (int32 idx = 0; idx < maxCount; ++idx)
		{
			if (const UGsUIBattleArenaSeasonTierRewardItemEntry* entry 
				= Cast<UGsUIBattleArenaSeasonTierRewardItemEntry>(_tierRewardPanelSlotHelper->GetEntry(idx))
				)
			{
				if (entry->IsMyTier())
				{
					focusEntry = entry;
					break;
				}
			}
		}
		// 2024/2/1 PKT - 내 정보에 포커싱
		_scrollBoxTierRewardListRoot->ScrollWidgetIntoView(const_cast<UGsUIBattleArenaSeasonTierRewardItemEntry*>(focusEntry), false, EDescendantScrollDestination::Center);
	}
	else if (RANK_REWARD_VIEW == index)
	{
		const int32 maxCount = GsBattleArenaRank()->SeasonRankCount(_rankSearchType);
		_rankRewardPanelSlotHelper->RefreshAll(maxCount);

		const UGsUIBattleArenaSeasonRankRewardItemEntry* focusEntry = nullptr;
		for (int32 idx = 0; idx < maxCount; ++idx)
		{
			if (const UGsUIBattleArenaSeasonRankRewardItemEntry* entry
				= Cast<UGsUIBattleArenaSeasonRankRewardItemEntry>(_rankRewardPanelSlotHelper->GetEntry(idx))
				)
			{
				if (entry->IsMyRank())
				{
					focusEntry = entry;
					break;
				}
			}
		}
		// 2024/2/1 PKT - 내 정보에 포커싱
		_scrollBoxRankRewardListRoot->ScrollWidgetIntoView(const_cast<UGsUIBattleArenaSeasonRankRewardItemEntry*>(focusEntry), false, EDescendantScrollDestination::Center);
	}
}

void UGSUIPopupBattleArenaSeasonRankBoard::OnClickedConfirm()
{
	Close();
}

void UGSUIPopupBattleArenaSeasonRankBoard::OnRefreshTierCategory(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIBattleArenaSeasonRankTierCategoryEntry> entry = Cast<UGsUIBattleArenaSeasonRankTierCategoryEntry>(InEntry);
	if (false == entry.IsValid())
	{
		return;
	}

	const FGsSchemaBattleArenaTierData* tierData = GsBattleArenaRank()->FindTierData(_rankSearchType, InIndex);
	if (nullptr == tierData)
	{
		entry->SetVisibility(ESlateVisibility::Collapsed);

		GSLOG(Error, TEXT("nullptr == tierData"));
		return;
	}

	entry->SetData(tierData);
	entry->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 2024/1/24 PKT - Add Toggle Group
	_toggleTierCategoryGroup.AddToggleWidget(entry->GetToggleGroupEntry());
}

void UGSUIPopupBattleArenaSeasonRankBoard::OnRefreshTierReward(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIBattleArenaSeasonTierRewardItemEntry> entry = Cast<UGsUIBattleArenaSeasonTierRewardItemEntry>(InEntry);
	if (false == entry.IsValid())
	{
		return;
	}

	const FGsSchemaBattleArenaTierSeasonReward* tierRewardData = GsBattleArenaRank()->FindTierRewardData(_rankSearchType, InIndex);
	if (nullptr == tierRewardData)
	{
		entry->SetVisibility(ESlateVisibility::Collapsed);

		GSLOG(Error, TEXT("nullptr == tierRewardData"));
		return;
	}

	const FGsBattleArenaRankLocalData* localRankData = GsBattleArenaRank()->GetLocalSeasonRanklData(_rankSearchType);
	entry->SetData(tierRewardData, (localRankData && localRankData->IsValidRankData()) ? localRankData->_rankData._tierDataId : INVALID_GUILD_WAR_TIER_DATA_ID);
}

void UGSUIPopupBattleArenaSeasonRankBoard::OnRefreshRankReward(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIBattleArenaSeasonRankRewardItemEntry> entry = Cast<UGsUIBattleArenaSeasonRankRewardItemEntry>(InEntry);
	if (false == entry.IsValid())
	{
		return;
	}

	const FGsSchemaBattleArenaRankingSeasonReward* rankRewardData = GsBattleArenaRank()->FindRankRewardData(_rankSearchType, InIndex);
	if (nullptr == rankRewardData)
	{
		entry->SetVisibility(ESlateVisibility::Collapsed);

		GSLOG(Error, TEXT("nullptr == rankRewardData"));
		return;
	}

	const FGsBattleArenaRankLocalData* localRankData = GsBattleArenaRank()->GetLocalSeasonRanklData(_rankSearchType);
	entry->SetData(rankRewardData, (localRankData && localRankData->IsValidRankData()) ? localRankData->_rankData._rank : INVALID_RANK);
}

void UGSUIPopupBattleArenaSeasonRankBoard::OnInputCancel()
{
	Close();
}

void UGSUIPopupBattleArenaSeasonRankBoard::SetData(RankSearchType InRankSearchType)
{
	// 2024/1/24 PKT - RankSearchType Type이 반듯이 필요하다.
	_rankSearchType = InRankSearchType;

	// 2024/1/30 PKT - 시즌 정보
	FText textFormat;
	// 2024/1/30 PKT - "시즌 {0}"
	FGsBattleArenaHelper::FindText(TEXT("UI_Ranking_SeasonNumber"), textFormat);
	_textSeasonName->SetText(FText::Format(textFormat, { GsBattleArenaRank()->SeasonNum(_rankSearchType) }));
	// 2024/1/30 PKT - "{0} 까지"
	FText textSeasonEndDate = GsBattleArenaRank()->SeasonDateText(_rankSearchType);
	if (textSeasonEndDate.IsEmpty())
	{
		_textSeasonEndDate->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		FGsBattleArenaHelper::FindText(TEXT("UI_Ranking_SeasonEndTime"), textFormat);
		_textSeasonEndDate->SetText(FText::Format(textFormat, { textSeasonEndDate }));
		_textSeasonEndDate->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}	
	// 2024/1/30 PKT - "시즌 참여 횟수가 {0}회 미만인 기사단원은 시즌 보상이 지급되지 않습니다"
	if (const FGsSchemaBattleArenaSeasonData* schemaSeasonData = GsBattleArenaRank()->GetSeasonData(_rankSearchType))
	{
		FGsBattleArenaHelper::FindText(TEXT("Notice_Ranking_Reward_Condition"), textFormat);
		_ritchTextSeasonRewardNotice->SetText(FText::Format(textFormat, { schemaSeasonData->playRewardLimitCount }));
	}

	// 2024/1/24 PKT - 기본 랭킹 정보
	const int32 DEFAULT_MAIN_TAP_INDEX = 0;
	_toggleMainTabGroup.SetSelectedIndex(DEFAULT_MAIN_TAP_INDEX, true);

	// 2024/1/31 PKT - 내 정보를 채워준다.
	if (GsBattleArenaRank()->BuildLocalRankData(_rankSearchType))
	{
		BuildMySeasonTierData();
	}	
}