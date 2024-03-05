
#include "GsUITitleBattleLevelEntry.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "DataSchema/InterServer/GsSchemaInvadeBattleLevel.h"
#include "T1Project.h"


void UGsUITitleBattleLevelEntry::SetData(const FGsSchemaInvadeBattleLevel* InSchema, const int32 InCurrentLevel)
{
	if (nullptr == InSchema)
	{
		GSLOG(Error, TEXT("nullptr == InSchema"));
		return;
	}

	const static int32 SWITCHER_DEFAULT_INDEX = 0;
	const static int32 SWITCHER_SELECTED_INDEX = 1;
	_switcherSelected->SetActiveWidgetIndex(InSchema->battleLevel == InCurrentLevel ? SWITCHER_SELECTED_INDEX : SWITCHER_DEFAULT_INDEX);

	_textLevel->SetText(FText::AsNumber(InSchema->battleLevel));
	_textTitle->SetText(InSchema->LevelName);
	_textAmount->SetText(FText::AsNumber(InSchema->requireItemCount));

	_textSelectedLevel->SetText(FText::AsNumber(InSchema->battleLevel));
	_textSelectedTitle->SetText(InSchema->LevelName);
	_textSelectedAmount->SetText(FText::AsNumber(InSchema->requireItemCount));
}