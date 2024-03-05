// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLocalPlayerSocial.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "Animation/GsAnimInstanceState.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"
#include "Data/GsResourceManager.h"

uint8 FGsStateLocalPlayerSocial::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Social);
}
FName FGsStateLocalPlayerSocial::GetStateName() const
{
	return TEXT("StateLocalPlayerSocial");
}
FName FGsStateLocalPlayerSocial::GetBPStateName() const
{
	return TEXT("Idle");
}

bool FGsStateLocalPlayerSocial::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		GS_STATE_GROUP_ACTION:
		GS_STATE_GROUP_MOVE:
		GS_STATE_GROUP_ABNORLITY:
		GS_STATE_GROUP_DIE:
		case EGsStateBase::CommonActionEnd:
		case EGsStateBase::Warp:
		case EGsStateBase::ChangeIdle:
		case EGsStateBase::ChangeWeapon:
		case EGsStateBase::Social:
		case EGsStateBase::Idle:
		case EGsStateBase::Spawn:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateLocalPlayerSocial::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);

	Enter(Owner);
}

void FGsStateLocalPlayerSocial::Enter(UGsGameObjectBase* Owner)
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

void FGsStateLocalPlayerSocial::Exit(UGsGameObjectBase* Owner)
{
	// 일반 Idle 상태로 되돌림
	UGsGameObjectCreature* creature = Owner->CastGameObject<UGsGameObjectCreature>();
	creature->ChangeIdleType(creature->IsBattleMode() ? EGsAnimStateIdleType::Battle : EGsAnimStateIdleType::Normal);

	Super::Exit(Owner);
}

void FGsStateLocalPlayerSocial::Update(UGsGameObjectBase* Owner, float Delta)
{
	Super::Update(Owner, Delta);

	if (auto skillHandler = Owner->GetSkillHandler())
	{
		if (skillHandler->IsEndTimeIdleEmotionMotion())
		{
			skillHandler->StopSocial(true);
		}
	}
}
