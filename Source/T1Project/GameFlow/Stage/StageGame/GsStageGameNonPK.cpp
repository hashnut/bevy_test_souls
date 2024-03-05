// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStageGameNonPK.h"

#include "Message/MessageParam/GsStageMessageParam.h"

#include "Management/GsMessageHolder.h"

#include "UTIL/GsGameObjectUtil.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

FGsStageGameNonPK::FGsStageGameNonPK() : FGsStageGameBase(FGsStageMode::Game::NON_PK)
{

}

FGsStageGameNonPK::~FGsStageGameNonPK()
{

}

void FGsStageGameNonPK::Enter()
{
	FGsStageGameBase::Enter();

#if TERRITORY_DEBUG	
	GSLOG(Log, TEXT("territory - stage - enter non pk"));
#endif		

	FGsFence* fence = nullptr;
	FGsFenceHandler* fenceHandler = GFence();
	if (fenceHandler
		&& fenceHandler->TryGetPlayerTopPriorityServerFence(fence))
	{
		if (TerritoryType::NON_PK == fence->GetTerritoryType())
		{
			FGsStageFenceMsgParam param(false, false, fence);
			GMessage()->GetStageParam().SendMessage(MessageStage::ENTER_NON_PK_CALLBACK, &param);
		}
		else
		{
			GSLOG(Error, TEXT("Fence type is wrong"));
		}
	}
}

void FGsStageGameNonPK::Exit()
{
	FGsStageFenceMsgParam param(false, false, nullptr);
	GMessage()->GetStageParam().SendMessage(MessageStage::LEAVE_NON_PK_CALLBACK, &param);

#if TERRITORY_DEBUG	
	GSLOG(Log, TEXT("territory - stage - exit non pk"));
#endif		

	FGsStageGameBase::Exit();
}

void FGsStageGameNonPK::Update(float In_deltaTime)
{
	FGsStageGameBase::Update(In_deltaTime);
}