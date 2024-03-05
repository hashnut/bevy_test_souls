// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateSandbagDying.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "ActorEx/GsCharacterBase.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "Animation/GsSandbagAnimInstance.h"

uint8 FGsStateSandbagDying::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Dying);
}

FName FGsStateSandbagDying::GetStateName() const
{
	return TEXT("StateSandbagDying");
}

FName FGsStateSandbagDying::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateSandbagDying::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Dead:
		case EGsStateBase::Spawn:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateSandbagDying::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	if (AGsCharacterBase* characterActor = Owner->GetCharacter())
	{
		// effect 제거
		characterActor->StopCurrentSkillEffectNSound();

		if (FGsSkillHandlerBase* skillhandler = Owner->GetSkillHandler())
		{
			// 스킬 종료
			skillhandler->StopSkill(0);

			FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();
			if (const FGsSchemaSkillCommon* commonData = GSkill()->GetNpcCommonActionData(Owner, CommonActionType::DYING))
			{
				skillRunner->StartRunner(Owner, commonData);
			}

			if (FGsGameObjectStateManager* fsm = Owner->GetBaseFSM())
			{
				if (UAnimMontage* resAni = skillRunner->GetAni())
				{
					float len = characterActor->PlayAnimMontage(resAni);
					fsm->DelayProcessEvent(len * 0.8f, EGsStateBase::Dying, EGsStateBase::Dead);
				}
				else
				{
					fsm->ProcessEvent(EGsStateBase::Dead);
				}
			}
		}
	}
}

void FGsStateSandbagDying::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}