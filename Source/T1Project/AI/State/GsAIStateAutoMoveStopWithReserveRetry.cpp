#include "GsAIStateAutoMoveStopWithReserveRetry.h"

#include "AI/GsAIStateManager.h"

#include "Management/ScopeGame/GsAIManager.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/AI/GsAIReserveHandler.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"

#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

void EGsAIStateAutoMoveStopWithReserveRetry::Enter()
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

	if (FGsAIReserveHandler* aiReserveHandler = _targetPlayer->GetAIReserve())
	{
		aiReserveHandler->RetryByAutoPause();
	}

	// clear reserve skill data
	// before: 
	// 1. auto move stop
	// 2. idle ready
	// 3. skillHandler->OnReserveSkill();(GsStateLocalPlayerIdle.cpp 195)
	if (FGsSkillHandlerLocalPlayer* skillHandler = _targetPlayer->GetCastSkill<FGsSkillHandlerLocalPlayer>())
	{
		skillHandler->ClearReserveSkill();
	}
	// clear target(by auto)
	if (FGsTargetHandlerLocalPlayer* targetHandler = _targetPlayer->GetCastTarget<FGsTargetHandlerLocalPlayer>())
	{
		if (targetHandler->GetTarget() != nullptr &&
			targetHandler->IsTargetByAuto() == true)
		{
			targetHandler->ClearTarget();
		}
	}


	FGsGameObjectStateManager* fsm = _targetPlayer->GetBaseFSM();
	fsm->ProcessEvent(EGsStateBase::AutoMoveStop);
}

void EGsAIStateAutoMoveStopWithReserveRetry::Update(float In_delta)
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
