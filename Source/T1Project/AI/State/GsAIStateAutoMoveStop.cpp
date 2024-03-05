#include "GsAIStateAutoMoveStop.h"

#include "AI/GsAIStateManager.h"

#include "Management/ScopeGame/GsAIManager.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Define/GsGameObjectDefine.h"

#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

void EGsAIStateAutoMoveStop::Enter()
{
	// 매니저 없으면 리턴		
	if (nullptr == _aiStateManager || nullptr == _aiManager)
	{
		return;
	}

	// 플레이어 없어도 리턴
	if (nullptr == _targetPlayer)
	{
		// 대기로
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
		return;
	}

	// 이동 예약 데이터 삭제
	_targetPlayer->ClearReserveDataByReserveWork();

	FGsGameObjectStateManager* fsm = _targetPlayer->GetBaseFSM();
	fsm->ProcessEvent(EGsStateBase::AutoMoveStop);
}

void EGsAIStateAutoMoveStop::Update(float In_delta)
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
