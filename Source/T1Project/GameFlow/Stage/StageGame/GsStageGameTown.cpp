// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStageGameTown.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "GsStageGameBase.h"
#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsStageMessageParam.h"
#include "UTIL/GsGameObjectUtil.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

FGsStageGameTown::FGsStageGameTown() : FGsStageGameBase(FGsStageMode::Game::TOWN)
{
}

FGsStageGameTown::~FGsStageGameTown()
{
}

void FGsStageGameTown::Enter()
{
	FGsStageGameBase::Enter();	

#if INVASION_TOWN_BATTLE_DEBUG
	GSLOG(Log, TEXT("invasion town - enter town stage"));
#endif

#if TERRITORY_DEBUG	
	GSLOG(Log, TEXT("territory - stage - enter town"));
#endif		

	FGsFence* fence = nullptr;
	FGsFenceHandler* fenceHandler = GFence();
	if (fenceHandler
		&& fenceHandler->TryGetPlayerTopPriorityServerFence(fence))
	{
		if (TerritoryType::TOWN == fence->GetTerritoryType()
			|| TerritoryType::TOWN_CHAOS == fence->GetTerritoryType())
		{
			FGsStageFenceMsgParam param(true, true, fence);
			GMessage()->GetStageParam().SendMessage(MessageStage::ENTER_TOWN_CALLBACK, &param);
		}
		else
		{
			GSLOG(Error, TEXT("Fence type is wrong"));
		}
	}
}

void FGsStageGameTown::Exit()
{	
	FGsStageFenceMsgParam param(false, false, nullptr);
	GMessage()->GetStageParam().SendMessage(MessageStage::LEAVE_TOWN_CALLBACK, &param);

#if INVASION_TOWN_BATTLE_DEBUG
	GSLOG(Log, TEXT("invasion town - leave town stage"));
#endif

#if TERRITORY_DEBUG	
	GSLOG(Log, TEXT("territory - stage - exit town"));
#endif		

	FGsStageGameBase::Exit();
}

void FGsStageGameTown::Update(float In_deltaTime)
{
	FGsStageGameBase::Update(In_deltaTime);
}
