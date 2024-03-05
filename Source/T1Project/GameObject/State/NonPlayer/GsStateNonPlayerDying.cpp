// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateNonPlayerDying.h"
#include "GsStateNonPlayerHeader.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "../../ObjectClass/GsGameObjectBase.h"
#include "../ActorEx/GsCharacterBase.h"

uint8 FGsStateNonPlayerDying::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Dying);
}

FName FGsStateNonPlayerDying::GetStateName() const
{
	return TEXT("StateNonPlayerDying");
}

FName FGsStateNonPlayerDying::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateNonPlayerDying::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Dead:
		case EGsStateBase::Revive:
		case EGsStateBase::Spawn:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateNonPlayerDying::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	if (AGsCharacterBase* characterActor = Owner->GetCharacter())
	{
		characterActor->DyingEvent();

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

void FGsStateNonPlayerDying::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}