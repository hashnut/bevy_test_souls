// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStageGameArenaReady.h"

#include "Message/MessageParam/GsStageMessageParam.h"

#include "Management/GsMessageHolder.h"

#include "UTIL/GsGameObjectUtil.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

FGsStageGameArenaReady::FGsStageGameArenaReady() : FGsStageGameBase(FGsStageMode::Game::ARENA_READY)
{

}

FGsStageGameArenaReady::~FGsStageGameArenaReady()
{

}

void FGsStageGameArenaReady::Enter()
{
	FGsStageGameBase::Enter();

#if INVASION_TOWN_BATTLE_DEBUG
	GSLOG(Log, TEXT("invasion town - stage - enter areana ready"));
#endif

#if TERRITORY_DEBUG	
	GSLOG(Log, TEXT("territory - stage - enter arena ready"));
#endif		

	FGsFence* fence = nullptr;
	FGsFenceHandler* fenceHandler = GFence();
	if (fenceHandler
		&& fenceHandler->TryGetPlayerTopPriorityServerFence(fence))
	{
		if (TerritoryType::ARENA_READY == fence->GetTerritoryType())
		{
			FGsStageFenceMsgParam param(true, true, fence);
			GMessage()->GetStageParam().SendMessage(MessageStage::ENTER_ARENA_READY_CALLBACK, &param);
		}
		else
		{
			GSLOG(Error, TEXT("Fence type is wrong"));
		}
	}
}

void FGsStageGameArenaReady::Exit()
{
	FGsStageFenceMsgParam param(false, false, nullptr);
	GMessage()->GetStageParam().SendMessage(MessageStage::LEAVE_ARENA_READY_CALLBACK, &param);

#if INVASION_TOWN_BATTLE_DEBUG
	GSLOG(Log, TEXT("invasion town - stage - leave areana ready"));
#endif

#if TERRITORY_DEBUG	
	GSLOG(Log, TEXT("territory - stage - exit arena ready"));
#endif		

	FGsStageGameBase::Exit();
}

void FGsStageGameArenaReady::Update(float In_deltaTime)
{
	FGsStageGameBase::Update(In_deltaTime);
}