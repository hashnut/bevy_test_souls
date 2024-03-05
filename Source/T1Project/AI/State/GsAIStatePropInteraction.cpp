// Fill out your copyright notice in the Description page of Project Settings.


#include "GsAIStatePropInteraction.h"

#include "AI/GsAIStateManager.h"
#include "AI/Data/GsAIReserveJob.h"

#include "GameObject/ObjectClass/GsGameObjectPropInteraction.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Reserve/GsReserveWork.h"
#include "GameObject/AI/GsAIReserveHandler.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"

#include "Message/MessageParam/GsInteractionMessageParam.h"

#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

void FGsAIStatePropInteraction::Enter()
{
	TArray<IGsReserveWork*> resrveWorks = _targetPlayer->GetReserveWorks();
	for (IGsReserveWork* iter : resrveWorks)
	{
		if (iter == nullptr)
		{
			continue;
		}

		if (iter->IsReserveData() == true)
		{
			iter->DoRetry();
			return;
		}
	}

	FGsAIReserveHandler* aiReserveHandler = _targetPlayer->GetAIReserve();
	if (aiReserveHandler == nullptr)
	{
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
		return;
	}

	FGsAIReserveJob* reserveJob = aiReserveHandler->GetFirstJob();
	if (reserveJob == nullptr || reserveJob->_targetObj == nullptr ||
		reserveJob->_actionType != EGsAIActionType::INTERACTION)
	{
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
		return;
	}
	UGsGameObjectPropInteraction* prop = Cast<UGsGameObjectPropInteraction>(reserveJob->_targetObj);
	if (prop == nullptr)
	{
		return;
	}


	//if (UGsGameObjectPropInteraction* prop = GSGameObject()->FindPropObjectNearest(true))
	{
		// 버튼 클릭 메시지에서는 ai를 끄는 처리가 있어서
		// 바로 호출한다
		GSInteraction()->TryInteractionGameObject(prop);
	}
}

void  FGsAIStatePropInteraction::Update(float In_deltaTime)
{
	if (nullptr == _targetPlayer ||
		nullptr == _targetPlayer->GetBaseFSM())
	{
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
		return;
	}

	// 아이들이면 
	if (true == _targetPlayer->GetBaseFSM()->IsState(EGsStateBase::Idle))
	{
		// 대기로 변경
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
	}
}