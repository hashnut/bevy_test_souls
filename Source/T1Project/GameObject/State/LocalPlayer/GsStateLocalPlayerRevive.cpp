// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateLocalPlayerRevive.h"
#include "GsStateLocalPlayerHeader.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

uint8 FGsStateLocalPlayerRevive::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Revive);
}

FName FGsStateLocalPlayerRevive::GetStateName() const
{
	return TEXT("StateLocalPlayerRevive");
}

FName FGsStateLocalPlayerRevive::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateLocalPlayerRevive::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		// 현재는 부활관련 아무 처리가 없으므로 무조건 치환
		return true;

		/*switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Idle:
			return true;

		default:
			return false;
		}*/
	}
	return false;
}

void FGsStateLocalPlayerRevive::Enter(UGsGameObjectBase* Owner)
{
	//GSLOG(Log , TEXT("FGsStateLocalPlayerRevive::Enter() ~~~~~~~~~~~~~"));
	Super::Enter(Owner);

	// UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	/*UGsAnimInstanceState* anim = local->GetLocalCharacter()->GetAnim();
	FGsSkillHandlerBase* skillhandler = local->GetSkillHandler();
	FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();
	if (const FGsSchemaSkillCommon* commonData = GSkill()->GetCommonSkillData(Owner, ECommonActionType::REVIVE))
	{
		skillRunner->StartRunner(Owner, commonData);
	}

	if (FGsGameObjectStateManager* fsm = local->GetBaseFSM())
	{
		if (UAnimMontage* resAni = skillRunner->GetAni())
		{
			float len = anim->Montage_Play(resAni);
			fsm->DelayProcessEvent(len * 0.8f, EGsStateBase::Idle);
		}
		else
		{
			fsm->ProcessEvent(EGsStateBase::Idle);
		}
	}*/

	// Revive Anim 중에는 무적처리 함
	/*local->SetIsInvincible(true);

	if (FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>())
	{
		movement->SetMoveable(false);
	}*/

	// 현재는 부활 관련 연출/기타 처리가 없으므로 바로 Idle로 변경 요청)
	if (FGsGameObjectStateManager* fsm = Owner->GetBaseFSM())
	{
		fsm->ProcessEvent(EGsStateBase::Idle);
	}
}

void FGsStateLocalPlayerRevive::Exit(UGsGameObjectBase* Owner)
{
	//GSLOG(Log, TEXT("FGsStateLocalPlayerRevive::Exit() ~~~~~~~~~~~~~"));
	// check(Owner);
	// Owner->OnRevive();
	/*UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	if (FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>())
	{
		movement->SetMoveable(true);
	}*/

	Super::Exit(Owner);
}
