#include "GsAIBaseState.h"
#include "AI/GsAIStateManager.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"

void FGsAIBaseState::SetTarget(UGsGameObjectLocalPlayer* In_target)
{
	_targetPlayer = In_target;
}
// 캐릭터 삭제
void FGsAIBaseState::RemoveCharacter()
{
	_targetPlayer = nullptr;
}
void FGsAIBaseState::SetManager(FGsAIManager* In_aiMng, FGsAIStateManager* In_aiStateMng)
{
	_aiManager = In_aiMng;
	_aiStateManager = In_aiStateMng;
}

// 오브젝트 상태 idle로 만든다
void FGsAIBaseState::SetObjStateIdle()
{
	if (nullptr == _targetPlayer ||
		nullptr == _targetPlayer->GetBaseFSM())
	{
		return;
	}

	_targetPlayer->GetBaseFSM()->ProcessEvent(EGsStateBase::Idle);
}

// 이동 정지
void FGsAIBaseState::SetMovementStop()
{
	if (nullptr == _targetPlayer ||
		nullptr == _targetPlayer->GetMovementHandler())
	{
		return;
	}

	_targetPlayer->GetMovementHandler()->Stop();
}
// 플레이어가 이동 가능한가
bool FGsAIBaseState::GetMoveAble()
{
	if (FGsMovementHandlerLocalPlayer* movement = 
		static_cast<FGsMovementHandlerLocalPlayer*>(_targetPlayer->GetMovementHandler()))
	{
		return movement->GetMoveAble();
	}
	return false;
}