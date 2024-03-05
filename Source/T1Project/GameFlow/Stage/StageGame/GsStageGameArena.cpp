// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStageGameArena.h"

#include "Message/MessageParam/GsStageMessageParam.h"

#include "Management/GsMessageHolder.h"

#include "UTIL/GsGameObjectUtil.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

FGsStageGameArena::FGsStageGameArena() : FGsStageGameBase(FGsStageMode::Game::ARENA)
{

}

FGsStageGameArena::~FGsStageGameArena()
{

}

void FGsStageGameArena::Enter()
{
	FGsStageGameBase::Enter();

#if TERRITORY_DEBUG	
	GSLOG(Log, TEXT("territory - stage - enter arena"));
#endif	

	FGsFence* fence = nullptr;
	FGsFenceHandler* fenceHandler = GFence();
	if (fenceHandler
		&& fenceHandler->TryGetPlayerTopPriorityServerFence(fence))
	{
		if (TerritoryType::ARENA == fence->GetTerritoryType())
		{
			FGsStageFenceMsgParam param(false, false, fence);
			GMessage()->GetStageParam().SendMessage(MessageStage::ENTER_ARENA_CALLBACK, &param);
		}
		else
		{
			GSLOG(Error, TEXT("Fence type is wrong"));
		}		
	}	

	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_ARENA);
}

void FGsStageGameArena::Exit()
{
	FGsStageFenceMsgParam param(false, false, nullptr);
	GMessage()->GetStageParam().SendMessage(MessageStage::LEAVE_ARENA_CALLBACK, &param);

#if TERRITORY_DEBUG	
	GSLOG(Log, TEXT("territory - stage - exit arena"));
#endif	

	FGsStageGameBase::Exit();
}

void FGsStageGameArena::Update(float In_deltaTime)
{
	FGsStageGameBase::Update(In_deltaTime);
}