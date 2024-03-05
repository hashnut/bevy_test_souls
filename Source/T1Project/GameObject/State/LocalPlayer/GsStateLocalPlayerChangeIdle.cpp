// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateLocalPlayerChangeIdle.h"
#include "GsStateLocalPlayerHeader.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "Animation/GsAnimInstanceState.h"
#include "../GsGameObjectStateManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "../../Skill/GsSkillHandlerBase.h"


uint8 FGsStateLocalPlayerChangeIdle::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::ChangeIdle);
}

FName FGsStateLocalPlayerChangeIdle::GetStateName() const
{
	return TEXT("StateLocalPlayerChangeIdle");
}

FName FGsStateLocalPlayerChangeIdle::GetBPStateName() const
{
	return NAME_None;
}
bool FGsStateLocalPlayerChangeIdle::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		GS_STATE_GROUP_MOVE:
		GS_STATE_GROUP_ACTION_END:
		GS_STATE_GROUP_ACTION:
		GS_STATE_GROUP_ABNORLITY:
		GS_STATE_GROUP_DIE:
		case EGsStateBase::Warp:
		case EGsStateBase::ChangeIdle:
		case EGsStateBase::Social:
		case EGsStateBase::Spawn:
			return true;
		default:
			return CanCancel(Owner);
		}
	}
	return false;
}

void FGsStateLocalPlayerChangeIdle::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	FGsSkillHandlerBase* skillhandler = local->GetSkillHandler();
	FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();
	UGsAnimInstanceState* anim = local->GetAnimInstance();

	// effect 제거
	anim->ActiveMontageAnimNotifyDestroy();

	if (FGsGameObjectStateManager* fsm = Owner->GetBaseFSM())
	{
		// 평화 상태일때만 처리
		if (false == local->IsBattleMode())
		{
			if (const FGsSchemaSkillCommon* commonSkill = GSkill()->GetPlayerCommonActionData(Owner,
				CommonActionType::INPEACE))
			{
				skillRunner->StartRunner(Owner, commonSkill);
				anim->SetPhysicsAnimation(commonSkill->physicsAnimation);
				
				if (UAnimMontage* resAni = skillRunner->GetAni())
				{
					float len = anim->Montage_Play(resAni);
					fsm->DelayProcessEvent(len * 0.8f, EGsStateBase::ChangeIdle, EGsStateBase::CommonActionEnd);
					return;
				}
			}
			
		}

		fsm->ProcessEvent(EGsStateBase::CommonActionEnd);
	}
}

void FGsStateLocalPlayerChangeIdle::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);

	// 평화 상태 진행중 전투 상태가 될수 있다.
	Enter(Owner);
}

void FGsStateLocalPlayerChangeIdle::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

bool FGsStateLocalPlayerChangeIdle::CanCancel(UGsGameObjectBase* Owner) const
{
	// FGsSchemaSkillCommon 구조체는 캔슬 정보를 현재 포함 하고 있지 않으모로
	// 현재는 무의미하다.
	return false;

	//FGsSkillHandlerBase* skillMgr = Owner->GetSkillHandler();
	//FGsSkillRunnerBase* skillRunner = skillMgr->GetSkillRunner();

	//if (false == skillRunner->IsValid())
	//{
	//	return true;
	//}

	//return skillRunner->CanCancel();
}