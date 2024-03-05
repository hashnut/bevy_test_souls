// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateRemotePlayerSocial.h"
#include "Animation/GsAnimInstanceState.h"
#include "Skill/GsSchemaSkillCommon.h"
#include "../../Skill/GsSkillHandlerBase.h"
#include "../../Skill/Runner/GsSkillRunnerBase.h"
#include "../../ObjectClass/GsGameObjectCreature.h"

uint8 FGsStateRemotePlayerSocial::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Social);
}
FName FGsStateRemotePlayerSocial::GetStateName() const
{
	return TEXT("StateRemotePlayerSocial");
}
FName FGsStateRemotePlayerSocial::GetBPStateName() const
{
	return TEXT("Idle");
}

bool FGsStateRemotePlayerSocial::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	return Super::CanTransition(Owner, StateID);
}

void FGsStateRemotePlayerSocial::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);

	Enter(Owner);
}

void FGsStateRemotePlayerSocial::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	//UGsGameObjectCreature* creature = Owner->CastGameObject<UGsGameObjectCreature>();
	//creature->ChangeIdleType(EGsAnimStateIdleType::Social);

	FGsGameObjectStateManager* fsm = Owner->GetBaseFSM();
	FGsSkillHandlerBase* skillhandler = Owner->GetSkillHandler();
	if (const FGsSchemaSkillCommon* socialAction = skillhandler->GetCurrentSocialData())
	{
		FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();
		skillRunner->StartRunner(Owner, socialAction);

		UGsAnimInstanceState* anim = Owner->GetAnimInstance();
		anim->SetPhysicsAnimation(socialAction->physicsAnimation);

		if (UAnimMontage* resAni = skillRunner->GetAni())
		{			
			float len = anim->Montage_Play(resAni);
		}
	}
}

void FGsStateRemotePlayerSocial::Exit(UGsGameObjectBase* Owner)
{
	UGsAnimInstanceState* anim = Owner->GetAnimInstance();
	anim->StopMontagePlay();

	UGsGameObjectCreature* creature = Owner->CastGameObject<UGsGameObjectCreature>();
	creature->ChangeIdleType(creature->IsBattleMode() ? EGsAnimStateIdleType::Battle : EGsAnimStateIdleType::Normal);

	Super::Exit(Owner);
}

void FGsStateRemotePlayerSocial::Update(UGsGameObjectBase* Owner, float Delta)
{
	Super::Update(Owner, Delta);

	if (auto skillHandler = Owner->GetSkillHandler())
	{
		if (skillHandler->IsEndTimeIdleEmotionMotion())
		{
			FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::Idle);
		}
	}
}