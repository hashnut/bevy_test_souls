// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateRemotePlayerChangeIdle.h"
#include "GsStateRemotePlayerHeader.h"
#include "Animation/GsAnimInstanceState.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"

uint8 FGsStateRemotePlayerChangeIdle::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::ChangeIdle);
}

FName FGsStateRemotePlayerChangeIdle::GetStateName() const
{
	return TEXT("GsStateRemotePlayerChangeIdle");
}

FName FGsStateRemotePlayerChangeIdle::GetBPStateName() const
{
	return NAME_None;
}
bool FGsStateRemotePlayerChangeIdle::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Idle:		
		case EGsStateBase::Dying:
		case EGsStateBase::Dead:
		case EGsStateBase::ForwardWalk:
		case EGsStateBase::Run:
		case EGsStateBase::Attack:
		case EGsStateBase::Hit:
		case EGsStateBase::Reaction:
		case EGsStateBase::ChangeWeapon:
		case EGsStateBase::CommonActionEnd:
		case EGsStateBase::Social:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateRemotePlayerChangeIdle::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	if (FGsSkillHandlerBase* skillhandler = Owner->GetSkillHandler())
	{
		FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();
		
		if (FGsGameObjectStateManager* fsm = Owner->GetBaseFSM())
		{
			UGsGameObjectCreature* creature = Owner->CastGameObject<UGsGameObjectCreature>();
			if (false == creature->IsBattleMode())
			{
				if (const FGsSchemaSkillCommon* commonData = GSkill()->GetPlayerCommonActionData(Owner, CommonActionType::INPEACE))
				{
					skillRunner->StartRunner(Owner, commonData);

					UGsAnimInstanceState* anim = Owner->GetAnimInstance();
					anim->SetPhysicsAnimation(commonData->physicsAnimation);

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
}

void FGsStateRemotePlayerChangeIdle::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}