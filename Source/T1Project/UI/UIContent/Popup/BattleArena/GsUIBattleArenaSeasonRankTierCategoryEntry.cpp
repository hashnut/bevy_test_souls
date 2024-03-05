#include "GsUIBattleArenaSeasonRankTierCategoryEntry.h"
#include "DataSchema/BattleArena/GsSchemaBattleArenaTierData.h"

#include "../UI/UIControlLib/ContentWidget/GsSwitcherButton.h"

#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"

#include "../../Helper/GsUIHelper.h"
#include "../Classes/GsDelegate.h"

#include "T1Project.h"


void UGsUIBattleArenaSeasonRankTierCategoryEntry::NativeConstruct()
{
	Super::NativeConstruct();

	_tierId = INVALID_GUILD_WAR_TIER_DATA_ID;
}

void UGsUIBattleArenaSeasonRankTierCategoryEntry::SetIsSelected(bool bInIsSelected)
{
	const static int32 UNSELECTED_INDEX = 0;
	const static int32 SELECTED_INDEX = 1;	
	_switcherToggle->SetActiveWidgetIndex(false == bInIsSelected ? UNSELECTED_INDEX : SELECTED_INDEX);
	GetToggleGroupEntry()->SetIsSelected(bInIsSelected);
}

bool UGsUIBattleArenaSeasonRankTierCategoryEntry::GetIsSelected() const
{
	const static int32 SELECTED_INDEX = 1;
	return (SELECTED_INDEX == _switcherToggle->GetActiveWidgetIndex());
}

IGsToggleGroupEntry* UGsUIBattleArenaSeasonRankTierCategoryEntry::GetToggleGroupEntry()
{
	return _toggleButton;
}

void UGsUIBattleArenaSeasonRankTierCategoryEntry::SetData(const FGsSchemaBattleArenaTierData* InData)
{
	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("nullptr == InData"));
		return;
	}

	_tierId = InData->id;

	_tierNameEnable->SetText(InData->name);
	_tierNameDisable->SetText(InData->name);

	ESlateVisibility visibility = ESlateVisibility::Collapsed;
	if (UPaperSprite* sprite = FGsUIHelper::LoadSpriteFromPath(InData->symbolPath))
	{
		_tierSymbolEnable->SetBrushFromAtlasInterface(sprite);
		_tierSymbolDisable->SetBrushFromAtlasInterface(sprite);

		visibility = ESlateVisibility::SelfHitTestInvisible;
	}

	_tierSymbolEnable->SetVisibility(visibility);
	_tierSymbolDisable->SetVisibility(visibility);
}