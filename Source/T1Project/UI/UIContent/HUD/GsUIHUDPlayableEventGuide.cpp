#include "GsUIHUDPlayableEventGuide.h"

#include "UMG/Public/Components/WidgetSwitcher.h"

#include "Management/ScopeLobby/GsLobbyPlayableEventManager.h"

#include "DataSchema/PlayableEvent/GsSchemaPlayableEventQuest.h"

#include "PlayableEvent/EGsPlayableEventQuestGoalType.h"

#include "PlayableEvent/GsPlayableEventDefine.h"
#include "Management/GsScopeHolder.h"
#include "Management/GsScopeLobby.h"

void UGsUIHUDPlayableEventGuide::InvalidateAllInternal()
{
	EGsPlayableEventQuestGoalType currentGoalType =
		LSLobbyPlayableEvent()->GetCurrentGoalType();

	// https://jira.com2us.com/jira/browse/CHR-17592
	// use move, kill type
	if (currentGoalType == EGsPlayableEventQuestGoalType::WaitSubDialogEnd ||
		currentGoalType == EGsPlayableEventQuestGoalType::WaitTimmer)
	{
		_switcherType->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		_switcherType->SetActiveWidgetIndex((int)currentGoalType);
		_switcherType->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}