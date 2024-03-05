#include "GsUIBattleArenaSeasonRankItemEntry.h"
#include "../BattleArenaRank/GsBattleArenaRankData.h"
#include "../Management/GsScopeHolder.h"
#include "../Management/ScopeGame/GsBattleArenaSeasonRankManager.h"

#include "../../Helper/GsUIHelper.h"
#include "../Guild/GsGuildHelper.h"
#include "../ContentsServer/GsContentsServerHelper.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaHelper.h"

#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "DataSchema/BattleArena/GsSchemaBattleArenaTierData.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"

#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/WidgetSwitcher.h"

#include "T1Project.h"


/**
 * UGsBattleArenaSeasonRankItemEntryData
 */
void UGsBattleArenaSeasonRankItemEntryData::SetData(RankSearchType InRankSearchType, GuildWarTierDataId InTierDataId, int32 InIndex)
{
	_rankSerarchType = InRankSearchType;
	_tierDataId = InTierDataId;
	_dataIndex = InIndex;
}

/**
 * UGsUIBattleArenaSeasonRankItemEntry
 */
void UGsUIBattleArenaSeasonRankItemEntry::NativeOnListItemObjectSet(UObject* InEntryData)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(InEntryData);

	UGsBattleArenaSeasonRankItemEntryData* object = Cast<UGsBattleArenaSeasonRankItemEntryData>(InEntryData);
	if (object)
	{
		_rankSerarchType = object->GetRankSearchType();
		_tierDataId = object->GetTierDataId();
		_dataIndex = object->GetDataIndex();
		_state = ValidRankDataState::Invalidate;
		_panelWidgetRoot->SetVisibility(ESlateVisibility::Hidden);

		Invalidate();
	}
}

void UGsUIBattleArenaSeasonRankItemEntry::SetData(RankSearchType InRankSearchType, const FGsBattleArenaRankListItem* InData)
{
	if (nullptr == InData || ValidRankDataState::Validate != InData->_isValidate)
	{	// 2024/2/1 PKT - Data 검증
		return;
	}

	// 2024/2/1 PKT - Rank
	if (0 >= InData->_rank)
	{
		const static int32 UNKNOWN_RANK_INDEX = 4;
		_switcherRanking->SetActiveWidgetIndex(UNKNOWN_RANK_INDEX);
	}
	else if (3 >= InData->_rank)
	{	// 2024/2/1 PKT - Rank 1 ~ 3위까지(index순서는 -1씩 함.)
		_switcherRanking->SetActiveWidgetIndex(InData->_rank - 1);
	}
	else
	{	// 2024/2/1 PKT - Text로 표기
		const static int32 TEXT_RANK_INDEX = 3;
		_switcherRanking->SetActiveWidgetIndex(TEXT_RANK_INDEX);
	}
	// 2024/2/1 PKT - Text Rank
	_textRanking->SetText(FText::AsNumber(InData->_rank));

	ESlateVisibility visibility = ESlateVisibility::Collapsed;
	const FGsSchemaBattleArenaTierData* schemaTierData = GsBattleArenaRank()->FindTierData(InRankSearchType, InData->_tierDataId);
	if (schemaTierData)
	{
		if (UPaperSprite* sprite = FGsUIHelper::LoadSpriteFromPath(schemaTierData->symbolPath))
		{
			_imgTier->SetBrushFromAtlasInterface(sprite);

			visibility = ESlateVisibility::SelfHitTestInvisible;
		}
	}
	_imgTier->SetVisibility(visibility);

	// 2024/2/1 PKT - Guild Emblem
	if (const FGsSchemaGuildEmblem* schemaGuildEmblem = FGsGuildHelper::GetGuildEmblem(InData->_guildEmblemId))
	{
		_iconGuildEmblem->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(schemaGuildEmblem->iconPathSmall));
		_iconGuildEmblem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_iconGuildEmblem->SetVisibility(ESlateVisibility::Collapsed);
	}

	static const int32 NEW_RANK_DATA = 0;
	if (NEW_RANK_DATA == InData->_prevRank)
	{
		const static int32 RANK_NEW_INDEX = 2;
		_switcherRankRate->SetActiveWidgetIndex(RANK_NEW_INDEX);
	}
	else if (InData->_rank != InData->_prevRank)
	{
		const static int32 RANK_ARROW_INDEX = 0;
		_switcherRankRate->SetActiveWidgetIndex(RANK_ARROW_INDEX);
		if (InData->_rank < InData->_prevRank)
		{	// 2024/2/21 PKT - 현재 랭크가 작을 수록 상승
			const static int32 RANK_ARROW_UP = 1;
			_switcherRankRateArrow->SetActiveWidgetIndex(RANK_ARROW_UP);
			_textRankRate->SetText( FText::AsNumber(InData->_prevRank - InData->_rank));
		}
		else
		{
			const static int32 RANK_ARROW_DOWN = 0;
			_switcherRankRateArrow->SetActiveWidgetIndex(RANK_ARROW_DOWN);
			_textRankRate->SetText(FText::AsNumber(InData->_rank - InData->_prevRank));
		}
	}
	else
	{
		const static int32 RANK_SAME_INDEX = 1;
		_switcherRankRate->SetActiveWidgetIndex(RANK_SAME_INDEX);
	}

	// 2024/2/1 PKT - Guild Name
	_textGuildName->SetText(InData->_guildName);

	// 2024/2/1 PKT - Server Name
	_textServerName->SetText(FGsContentsServerHelper::GetWorldIPrefixdWithWorldId(InData->_worldId));

	// 2024/2/1 PKT - Score
	_textPoint->SetText(FText::AsNumber(InData->_score));

	FText textFormat;
	// 2024/2/1 PKT - "{0}승 {1}무 {2}패"
	FGsBattleArenaHelper::FindText(TEXT("UI_Ranking_RecordDetail"), textFormat);
	_textRecord->SetText(FText::Format(textFormat, { InData->_winCount, InData->_tieCount, InData->_loseCount }));

	_panelWidgetRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIBattleArenaSeasonRankItemEntry::Invalidate()
{
	if (ValidRankDataState::Validate == _state)
	{	// 2024/2/1 PKT - 이미 완료 한 Data라면 갱신 하지 않는다.
		return;
	}
	
	const FGsBattleArenaRankListItem* findItem = GsBattleArenaRank()->FindRankData(_rankSerarchType, _tierDataId, _dataIndex);
	if (findItem && ValidRankDataState::Validate == findItem->_isValidate)
	{
		SetData(_rankSerarchType, findItem);
	}
}