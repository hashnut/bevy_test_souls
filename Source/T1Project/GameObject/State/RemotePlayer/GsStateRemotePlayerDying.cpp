// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateRemotePlayerDying.h"
#include "GsStateRemotePlayerHeader.h"
#include "Animation/GsAnimInstanceState.h"
#include "ActorEx/GsCharacterBase.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "GameObject/Skill/GsSkillHandlerRemotePlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerCreature.h"
#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"

uint8 FGsStateRemotePlayerDying::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Dying);
}

FName FGsStateRemotePlayerDying::GetStateName() const
{
	return TEXT("StateRemotePlayerDying");
}

FName FGsStateRemotePlayerDying::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateRemotePlayerDying::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Revive:
		case EGsStateBase::Dead:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateRemotePlayerDying::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectRemotePlayer* remote = Owner->CastGameObject<UGsGameObjectRemotePlayer>();
	AGsCharacterBase* charactorActor = remote->GetCharacter();
	if (nullptr == charactorActor)
	{
		return;
	}

	charactorActor->StopCurrentSkillEffectNSound();

	if (FGsSkillHandlerBase* skillhandler = remote->GetSkillHandler())
	{
		// 스킬 종료
		skillhandler->StopSkill(0);		

		FGsGameObjectStateManager* fsm = Owner->GetBaseFSM();
		UGsAnimInstanceState* anim = charactorActor->GetAnim();
		if (fsm && anim)
		{
			FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();
			if (const FGsSchemaSkillCommon* commonData = GSkill()->GetPlayerCommonActionData(Owner, CommonActionType::DYING))
			{
				skillRunner->StartRunner(Owner, commonData);
				anim->SetPhysicsAnimation(commonData->physicsAnimation);
			}

			if (UAnimMontage* resAni = skillRunner->GetAni())
			{				
				float len = anim->Montage_Play(resAni);
				fsm->DelayProcessEvent(len * 0.8f, EGsStateBase::Dying, EGsStateBase::Dead);
			}
			else
			{
				fsm->ProcessEvent(EGsStateBase::Dead);
			}
		}
	}
}

void FGsStateRemotePlayerDying::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}