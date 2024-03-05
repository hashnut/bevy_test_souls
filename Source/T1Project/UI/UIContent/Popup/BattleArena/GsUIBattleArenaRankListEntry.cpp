#include "GsUIBattleArenaRankListEntry.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/TextBlock.h"
#include "../../Helper/GsUIStringHelper.h"

void UGsUIBattleArenaRankListEntry::SetData(int32 InRank, const FGsBattleArenaGuildWarMemberInfo& InData)
{	
	auto AmountTextWithComma = [](int32 num) -> FText
	{
		FText outText;
		FGsUIStringHelper::GetAmountText(num, MAX_uint64, outText);
		return outText;
	};
	
	/**
	 * Rank
	 */
	const static int32 SHOW_TEXT_RANK_INDEX = 3;
	if (SHOW_TEXT_RANK_INDEX >= InRank)
	{
		const int32 rankIndex = FMath::Max(InRank - 1, 0);
		_switcherRanking->SetActiveWidgetIndex(rankIndex);
	}
	else
	{		
		_switcherRanking->SetActiveWidgetIndex(SHOW_TEXT_RANK_INDEX);
		_textRanking->SetText(AmountTextWithComma(InRank));
	}
	 
	 // 2023/11/24 PKT - Name
	_textName->SetText(InData._name);	
	// 2023/11/24 PKT - Point
	_textPoint->SetText(AmountTextWithComma(InData._point));
	// 2023/11/24 PKT - Kill
	_textKill->SetText(AmountTextWithComma(InData._killCount));
	// 2023/11/24 PKT - Assist
	_textAssist->SetText(AmountTextWithComma(InData._assistCount));
	// 2023/11/24 PKT - Deat
	_textDeath->SetText(AmountTextWithComma(InData._deathCount));
}