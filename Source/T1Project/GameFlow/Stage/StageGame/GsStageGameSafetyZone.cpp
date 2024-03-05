// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStageGameSafetyZone.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "GsStageGameBase.h"
#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "Message/MessageParam/GsStageMessageParam.h"
#include "UTIL/GsGameObjectUtil.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

FGsStageGameSafetyZone::FGsStageGameSafetyZone() : FGsStageGameBase(FGsStageMode::Game::TOWN)
{
}

FGsStageGameSafetyZone::~FGsStageGameSafetyZone()
{
}

void FGsStageGameSafetyZone::Enter()
{
	FGsStageGameBase::Enter();

#if TERRITORY_DEBUG	
	GSLOG(Log, TEXT("territory - stage - enter safe zone"));
#endif		

	FGsFence* fence = nullptr;
	FGsFenceHandler* fenceHandler = GFence();
	if (fenceHandler
		&& fenceHandler->TryGetPlayerTopPriorityServerFence(fence))
	{
		if (TerritoryType::SAFE_ZONE == fence->GetTerritoryType())
		{
			FGsStageFenceMsgParam param(true, true, fence);
			GMessage()->GetStageParam().SendMessage(MessageStage::ENTER_SAFETY_CALLBACK, &param);
		}
		else
		{
			GSLOG(Error, TEXT("Fence type is wrong"));
		}
	}
}

void FGsStageGameSafetyZone::Exit()
{
	FGsStageFenceMsgParam param(false, false, nullptr);
	GMessage()->GetStageParam().SendMessage(MessageStage::LEAVE_SAFETY_CALLBACK, &param);

#if TERRITORY_DEBUG	
	GSLOG(Log, TEXT("territory - stage - exit safe zone"));
#endif		

	FGsStageGameBase::Exit();
}

void FGsStageGameSafetyZone::Update(float In_deltaTime)
{
	FGsStageGameBase::Update(In_deltaTime);
}
