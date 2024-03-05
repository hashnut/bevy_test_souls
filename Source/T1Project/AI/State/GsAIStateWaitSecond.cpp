#include "GsAIStateWaitSecond.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "GameObject/Reserve/GsReserveWork.h"

#include "GameObject/AI/GsAIReserveHandler.h"

#include "GameObject/State/GsGameObjectStateManager.h"

#include "GameObject/Define/GsGameObjectDefine.h"

#include "AI/GsAIStateManager.h"
#include "AI/Data/GsAIReserveJob.h"

#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

#include "Message/MessageParam/GsAIMessageParam.h"
#include "Message/GsMessageGameObject.h"

#include "Management/GsMessageHolder.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Core/Public/Misc/DateTime.h"

void FGsAIStateWaitSecond::Enter()
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
		reserveJob->_actionType != EGsAIActionType::WAIT_SECOND)
	{
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
		return;
	}


	_waitStartTime = FDateTime::UtcNow().GetTicks();

	_maxAIWaitSecond = GData()->GetGlobalData()->_maxAIWaitSedond * ETimespan::TicksPerSecond;
}

void  FGsAIStateWaitSecond::Update(float In_deltaTime)
{

	int64 nowTick = FDateTime::UtcNow().GetTicks();
	int64 diffTick = nowTick - _waitStartTime;

	bool isTimeOver = (diffTick >= _maxAIWaitSecond)? true: false;


	if (nullptr == _targetPlayer ||
		nullptr == _targetPlayer->GetBaseFSM())
	{
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
		return;
	}



	// 시간이 지났으면
	// 아이들이면 
	if (true == _targetPlayer->GetBaseFSM()->IsState(EGsStateBase::Idle) &&
		isTimeOver == true)
	{
		
		FGsAIReserveParam aiReserveClearParam(EGsAIActionType::WAIT_SECOND);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_CLEAR_RESERVE_ALL_JOB_BY_TYPE, &aiReserveClearParam);

		// 대기로 변경
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
	}
}