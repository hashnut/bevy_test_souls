#include "GsAIStateAttackAnyone.h"

#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Target/AutoTarget/GsTargetRule.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/Looting/GsLootingHandlerLocalPlayer.h"
#include "GameObject/Reserve/GsReserveWork.h"
#include "GameObject/AI/GsAIReserveHandler.h"

#include "AI/GsAIStateManager.h"
#include "AI/Function/GsAIFunc.h"
#include "AI/ContentsCondition/GsAIContentsConditionAttOrder.h"
#include "AI/Data/GsAIReserveJob.h"

#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

#include "T1Project.h"

#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillSet.h"

// 입장
void FGsAIStateAttackAnyone::Enter()
{
	// 현재 world가 유효한지 체크해본다
	// https://com2us-raon.sentry.io/issues/4276369261/events/622ca4c4d94b427c750d77791c63be93/?project=4504115468566528
	if (_aiStateManager == nullptr)
	{
		return;
	}
	UGsGameObjectManager* gameObjectManager =  GSGameObject();
	if (gameObjectManager == nullptr)
	{
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
		return;
	}
	UWorld* world = gameObjectManager->GetWorld();
	if (world == nullptr)
	{
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);	
		return;
	}
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

	
	FGsSkillHandlerLocalPlayer* localSkill = _targetPlayer->GetCastSkill< FGsSkillHandlerLocalPlayer>();
	if (nullptr == localSkill)
	{
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
		return;
	}

	// 사용할 skill id 가져오기
	const FGsSchemaSkillSet * skillData = FGsAIFunc::FindNowUseSkillID(_targetPlayer);
	if (nullptr != skillData)
	{
		// 타겟 선정
		FGsTargetHandlerLocalPlayer* castTargetHandler = _targetPlayer->GetCastTarget<FGsTargetHandlerLocalPlayer>();
		
		FGsAIReserveHandler* aiReserveHandler = _targetPlayer->GetAIReserve();
		if (aiReserveHandler == nullptr)
		{
			_aiStateManager->ChangeState(EGsAIActionType::WAIT);
			return;
		}

		FGsAIReserveJob* reserveJob = aiReserveHandler->GetFirstJob();
		if (reserveJob == nullptr || reserveJob->_targetObj == nullptr||
			reserveJob->_actionType != EGsAIActionType::ATTACK_ANYONE)
		{
			_aiStateManager->ChangeState(EGsAIActionType::WAIT);
			return;
		}

		// 나 자신에게 사용할 스킬 조건 확인
		if (FGsSkillHandlerLocalPlayer::IsMySelfSkill(skillData))
		{
			castTargetHandler->SetSkillSelectTarget(_targetPlayer);
		}
		else
		{
			castTargetHandler->SetTarget(reserveJob->_targetObj, false, FGsTargetHandlerBase::Auto);
		}

		localSkill->OnSkill(skillData->id);

#ifdef TEST_AI_RETURN
		if(aiReserveHandler->IsFirstJobAction(EGsAIActionType::MOVE_TO_AUTO_START_POS))
		{ 
			GSLOG(Error, TEXT("ai attack :MOVE_TO_AUTO_START_POS fist reserve job !!"));
		}
#endif
	}
}
// 갱신
void FGsAIStateAttackAnyone::Update(float In_deltaTime)
{
	if (nullptr == _targetPlayer ||
		nullptr == _targetPlayer->GetBaseFSM())
	{
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
		return;
	}

	// 타겟이 우선순위 높은게 아니고
	// 시야안에 우선순위 높은게 있으면
	// 기존 타겟 클리어하고
	// 새로 타겟팅한다
	//if (true == _aiManager->GetContentsConditionAttOrder()->IsNeedTargetChange())
	//{
	//	FGsTargetHandlerBase* targetMng = _targetPlayer->GetTargetHandler();
	//	if (nullptr == targetMng)
	//	{
	//		// 대기로
	//		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
	//		return;
	//	}

	//	// 타겟 초기화
	//	targetMng->ClearTarget();
	//	// obj fsm idle 로
	//	SetObjStateIdle();
	//	// 대기로
	//	_aiStateManager->ChangeState(EGsAIActionType::WAIT);

	//	return;
	//}

	// 아이들이면 
	if (true == _targetPlayer->GetBaseFSM()->IsState(EGsStateBase::Idle))
	{
		// 대기로 변경
		_aiStateManager->ChangeState(EGsAIActionType::WAIT);
	}
	else if (true == _targetPlayer->GetBaseFSM()->IsState(EGsStateBase::Attack))
	{
		// 켄슬 가능 상황이면
		if (FGsSkillHandlerLocalPlayer* skillHandler = _targetPlayer->GetCastSkill<FGsSkillHandlerLocalPlayer>())
		{
			if (skillHandler->CanCancel())
			{
				// 대기로 변경
				_aiStateManager->ChangeState(EGsAIActionType::WAIT);
			}
		}
	}
}

