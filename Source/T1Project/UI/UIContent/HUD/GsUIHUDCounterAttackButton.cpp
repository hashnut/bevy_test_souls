#include "GsUIHUDCounterAttackButton.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/CounterAttack/GsCounterAttackHandler.h"

#include "GameObject/CounterAttack/GsAttackInfoCounterAttack.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

void UGsUIHUDCounterAttackButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnCounterAttack->OnClicked.AddDynamic(this, &UGsUIHUDCounterAttackButton::OnClickCounterAttack);
}

void UGsUIHUDCounterAttackButton::OnClickCounterAttack()
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

	FGsCounterAttackHandler* counterAttackHandler = hud->GetCounterAttackHandler();
	if (nullptr == counterAttackHandler)
	{
		return;
	}
	counterAttackHandler->OnClickCounterAttackButton();

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDCounterAttackButton::SetHUDMode(EGsUIHUDMode InMode)
{
	// check counter attack show 
	FGsAttackInfoCounterAttack* attackInfoCounterAttack = GSGameObject()->GetAttackInfoCounterAttack();
	if (attackInfoCounterAttack == nullptr)
	{
		return;
	}
	bool isCounterAttackShow = attackInfoCounterAttack->GetIsCounterAttackShow();

	if (IsHideMode(InMode) ||
		isCounterAttackShow == false)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UGsUIHUDCounterAttackButton::UpdateVisible()
{
	// check counter attack show 
	FGsAttackInfoCounterAttack* attackInfoCounterAttack = GSGameObject()->GetAttackInfoCounterAttack();
	if (attackInfoCounterAttack == nullptr)
	{
		return;
	}
	bool isCounterAttackShow = attackInfoCounterAttack->GetIsCounterAttackShow();

	if (isCounterAttackShow == false)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}
void UGsUIHUDCounterAttackButton::UpdateHudMode()
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
	{
		return;
	}

	SetHUDMode(uiManager->GetHUDMode());
}