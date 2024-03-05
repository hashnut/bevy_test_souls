// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStageGameChaos.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "GsStageGameBase.h"
#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsStageMessageParam.h"
#include "UTIL/GsGameObjectUtil.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

FGsStageGameChaos::FGsStageGameChaos() : FGsStageGameBase(FGsStageMode::Game::CHAOS)
{
}

FGsStageGameChaos::~FGsStageGameChaos()
{
}

void FGsStageGameChaos::Enter()
{
	FGsStageGameBase::Enter();	

#if TERRITORY_DEBUG	
	GSLOG(Log, TEXT("territory - stage - enter chaos"));
#endif			

	FGsFence* fence = nullptr;
	FGsFenceHandler* fenceHandler = GFence();
	if (fenceHandler
		&& fenceHandler->TryGetPlayerTopPriorityServerFence(fence))
	{
		if (TerritoryType::TOWN_CHAOS == fence->GetTerritoryType())
		{
			FGsStageFenceMsgParam param(false, true, fence);
			GMessage()->GetStageParam().SendMessage(MessageStage::ENTER_CHAOS_CALLBACK, &param);
		}
		else
		{
			GSLOG(Error, TEXT("Fence type is wrong"));
		}
	}

#if INVASION_TOWN_BATTLE_DEBUG
	GSLOG(Log, TEXT("invasion town - Enter chaos stage"));
#endif
}

void FGsStageGameChaos::Exit()
{		
	FGsStageFenceMsgParam param(false, false, nullptr);
	GMessage()->GetStageParam().SendMessage(MessageStage::LEAVE_CHAOS_CALLBACK, &param);

#if INVASION_TOWN_BATTLE_DEBUG
	GSLOG(Log, TEXT("invasion town - Leave chaos stage"));
#endif

#if TERRITORY_DEBUG	
	GSLOG(Log, TEXT("territory - stage - exit chaos"));
#endif		

	FGsStageGameBase::Exit();
}

void FGsStageGameChaos::Update(float In_deltaTime)
{
	FGsStageGameBase::Update(In_deltaTime);
}
