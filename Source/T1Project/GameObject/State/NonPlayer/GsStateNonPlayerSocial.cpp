// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateNonPlayerSocial.h"
#include "GsStateNonPlayerHeader.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "Animation/GsAnimInstanceState.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"
#include "Data/GsResourceManager.h"

uint8 FGsStateNonPlayerSocial::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Social);
}

FName FGsStateNonPlayerSocial::GetStateName() const
{
	return TEXT("StateNonPlayerSocial");
}

FName FGsStateNonPlayerSocial::GetBPStateName() const
{
	return TEXT("Idle");
}

bool FGsStateNonPlayerSocial::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	return Super::CanTransition(Owner, StateID);
}

void FGsStateNonPlayerSocial::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	FGsSkillHandlerBase* skillhandler = Owner->GetSkillHandler();
	if (const FGsSchemaSkillCommon* action = skillhandler->GetCurrentSocialData())
	{
		if (UGsAnimInstanceState* anim = Owner->GetAnimInstance())
		{
			anim->SetPhysicsAnimation(action->physicsAnimation);

			if (UAnimMontage* res = GResource()->LoadSync<UAnimMontage>(action->aniPath))
			{
				anim->Montage_Play(res);
			}
		}
	}
}

void FGsStateNonPlayerSocial::Exit(UGsGameObjectBase* Owner)
{
	UGsAnimInstanceState* anim = Owner->GetAnimInstance();
	anim->StopMontagePlay();

	// 일반 Idle 상태로 되돌림
	UGsGameObjectCreature* creature = Owner->CastGameObject<UGsGameObjectCreature>();
	creature->ChangeIdleType(creature->IsBattleMode() ? EGsAnimStateIdleType::Battle : EGsAnimStateIdleType::Normal);

	Super::Exit(Owner);
}

void FGsStateNonPlayerSocial::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);

	Enter(Owner);
}

void FGsStateNonPlayerSocial::Update(UGsGameObjectBase* Owner, float Delta)
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