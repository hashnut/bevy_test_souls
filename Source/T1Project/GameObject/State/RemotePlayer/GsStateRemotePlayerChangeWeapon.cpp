// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateRemotePlayerChangeWeapon.h"
#include "Animation/GsAnimInstanceState.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "../../ObjectClass/GsGameObjectBase.h"
#include "Management/GsScopeHolder.h"

uint8 FGsStateRemotePlayerChangeWeapon::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::ChangeWeapon);
}
FName FGsStateRemotePlayerChangeWeapon::GetStateName() const
{
	return TEXT("StateRemotePlayerChangeWeapon");
}
FName FGsStateRemotePlayerChangeWeapon::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateRemotePlayerChangeWeapon::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Idle:
		case EGsStateBase::CommonActionEnd:
		case EGsStateBase::ForwardWalk:
		case EGsStateBase::Run:
		case EGsStateBase::Ride:
		case EGsStateBase::AutoMove:
		case EGsStateBase::Dying:
		case EGsStateBase::Dead:
		case EGsStateBase::Freeze:
		case EGsStateBase::Reaction:
		case EGsStateBase::Warp:
		case EGsStateBase::Hit:
		case EGsStateBase::ChangeIdle:
			return true;

		default:
			return false;
		}
	}
	return false;
}

void FGsStateRemotePlayerChangeWeapon::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	if (FGsGameObjectStateManager* fsm = Owner->GetBaseFSM())
	{
		FGsSkillHandlerBase* skillhandler = Owner->GetSkillHandler();
		FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();

		if (const FGsSchemaSkillCommon* commonSkill = GSkill()->GetPlayerCommonActionData(Owner,
			CommonActionType::WEAPON_CHANGE))
		{
			skillRunner->StartRunner(Owner, commonSkill);

			UGsAnimInstanceState* anim = Owner->GetAnimInstance();
			anim->SetPhysicsAnimation(commonSkill->physicsAnimation);

			if (UAnimMontage* resAni = skillRunner->GetAni())
			{
				float len = anim->Montage_Play(resAni);
			}
		}
	}
}

void FGsStateRemotePlayerChangeWeapon::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}