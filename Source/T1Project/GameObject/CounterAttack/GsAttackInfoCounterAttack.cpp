#include "GsAttackInfoCounterAttack.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/CounterAttack/GsCounterAttackHandler.h"

float FGsAttackInfoCounterAttack::GetWaitTime()
{
	return GData()->GetGlobalData()->_counterAttackButtonDeactivationSecond;
}


void FGsAttackInfoCounterAttack::EndTime()
{
	FGsAttackInfoBase::EndTime();
	UpdateUIHUD();
}
void FGsAttackInfoCounterAttack::StartCounterAttackShow(int64 In_gameId)
{
	_gameId = In_gameId;
	StartAttack();
}
void FGsAttackInfoCounterAttack::UpdateUIHUD()
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
	counterAttackHandler->UpdateCounterAttackButtonHUDMode();
}

void FGsAttackInfoCounterAttack::StopCountAttackShow()
{
	_isAttack = false;
	UpdateUIHUD();
}