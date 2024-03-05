#include "GsUIHUDPartyTargetButton.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"

#include "Management/ScopeGame/GsPartyManager.h"

#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Party/GsPartyHandler.h"

#include "UMG/Public/Components/WidgetSwitcher.h"

void UGsUIHUDPartyTargetButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnTarget->OnClicked.AddDynamic(this, &UGsUIHUDPartyTargetButton::OnClickTarget);
}


void UGsUIHUDPartyTargetButton::OnClickTarget()
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsPartyHandler* partyHandler = hud->GetPartyHandler();
	if (partyHandler == nullptr)
	{
		return;
	}

	partyHandler->OnClickPartyTargetButton(_isActive);

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}
void UGsUIHUDPartyTargetButton::SetTargetName(const FString& In_name)
{
	_textTarget = FText::FromString(In_name);
}

void UGsUIHUDPartyTargetButton::SetIsActive(bool In_isActive)
{
	int switchIndex = (In_isActive == true) ? 0 : 1;
	_switcherType->SetActiveWidgetIndex(switchIndex);

	_isActive = In_isActive;
}
void UGsUIHUDPartyTargetButton::SetHUDMode(EGsUIHUDMode InMode)
{
	// in party check
	bool isInParty = GSParty()->GetIsInParty();

	if (IsHideMode(InMode) ||
		isInParty == false)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UpdateSharedIdStateHandler();
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}
void UGsUIHUDPartyTargetButton::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateSharedIdStateHandler();
}

void UGsUIHUDPartyTargetButton::UpdateSharedIdStateHandler()
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsPartyHandler* partyHandler = hud->GetPartyHandler();
	if (partyHandler == nullptr)
	{
		return;
	}

	partyHandler->UpdateSharedIdState();
}