#include "GsAIStateAutoMoveStopWithClearReserveSkill.h"

#include "AI/GsAIStateManager.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsAIManager.h"

#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsAIMessageParam.h"

#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/AI/GsAIReserveHandler.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"

#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

void EGsAIStateAutoMoveStopWithClearReserveSkill::Enter()
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
	// ai reserve handler의 attack anyone action 다지우자
	// attack anyone이 루팅 위에 테스트에 있으면 움찔움찔 한다	
	FGsAIReserveParam aiReserveClearParam(EGsAIActionType::ATTACK_ANYONE);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_CLEAR_RESERVE_ALL_JOB_BY_TYPE, &aiReserveClearParam);

	// 반경 중심으로 이동할때 루팅할 아이템 있어도 이동
	// 예약된 자동 이동 정보를 클리어해보자
	// https://jira.com2us.com/jira/browse/CHR-24592

	// 이동 예약 데이터 삭제
	_targetPlayer->ClearReserveDataByReserveWork();


	FGsGameObjectStateManager* fsm = _targetPlayer->GetBaseFSM();
	fsm->ProcessEvent(EGsStateBase::AutoMoveStop);
}

void EGsAIStateAutoMoveStopWithClearReserveSkill::Update(float In_delta)
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
