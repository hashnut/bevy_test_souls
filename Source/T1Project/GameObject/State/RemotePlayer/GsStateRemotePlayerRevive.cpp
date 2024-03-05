// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateRemotePlayerRevive.h"
#include "GsStateRemotePlayerHeader.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "GameObject/Skill/GsSkillHandlerRemotePlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerCreature.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"

uint8 FGsStateRemotePlayerRevive::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Revive);
}

FName FGsStateRemotePlayerRevive::GetStateName() const
{
	return TEXT("GsStateRemotePlayerRevive");
}

FName FGsStateRemotePlayerRevive::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateRemotePlayerRevive::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		// 현재는 부활관련 아무 처리가 없으므로 무조건 치환
		return true;

		/*switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Idle:
		case EGsStateBase::ForwardWalk:
		case EGsStateBase::Run:
		case EGsStateBase::Attack:
		case EGsStateBase::Dying:
		case EGsStateBase::ChangeWeapon:
			return true;
		default:
			return false;
		}*/
	}
	return false;
}

void FGsStateRemotePlayerRevive::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	//UGsGameObjectRemotePlayer* remote = Owner->CastGameObject<UGsGameObjectRemotePlayer>();
	//UGsAnimInstanceState* anim = remote->GetCharacter()->GetAnim();
	//FGsSkillHandlerBase* skillhandler = remote->GetSkillHandler();
	//FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();
	//if (const FGsSchemaSkillCommon* commonData = GSkill()->GetPlayerCommonActionData(Owner, CommonActionType::DYING))
	//{
	//	skillRunner->StartRunner(Owner, commonData);
	//}
	//if (FGsGameObjectStateManager* fsm = Owner->GetBaseFSM())
	//{
	//	if (UAnimMontage* resAni = skillRunner->GetAni())
	//	{
	//		float len = anim->Montage_Play(resAni);
	//		fsm->DelayProcessEvent(len, EGsStateBase::Idle);
	//	}
	//}

	// 현재는 부활 관련 연출/기타 처리가 없으므로 바로 Idle로 변경 요청)
	if (FGsGameObjectStateManager* fsm = Owner->GetBaseFSM())
	{
		fsm->ProcessEvent(EGsStateBase::Idle);
	}
}

void FGsStateRemotePlayerRevive::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}
