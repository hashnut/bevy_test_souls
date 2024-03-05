// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateRemotePlayerInteractionEnd.h"
#include "GsStateRemotePlayerHeader.h"
#include "../../../ActorEx/GsCharacterBase.h"
#include "Animation/GsAnimInstanceState.h"
#include "Data/GsResourceManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"

uint8 FGsStateRemotePlayerInteractionEnd::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::InteractionEnd);
}
FName FGsStateRemotePlayerInteractionEnd::GetStateName() const
{
	return TEXT("StateRemotePlayerInteractionEnd");
}
FName FGsStateRemotePlayerInteractionEnd::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateRemotePlayerInteractionEnd::CanTransition(UGsGameObjectBase* Owner, int StateID) const
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
		case EGsStateBase::Idle:
		case EGsStateBase::Warp:
		case EGsStateBase::ChangeWeapon:
			return true;
		default:
			return false;
		}
	}
	return false;
}
void FGsStateRemotePlayerInteractionEnd::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	FGsGameObjectStateManager* fsm = Owner->GetBaseFSM();
	if (const FGsSchemaSkillCommon* commonSkill = GSkill()->GetPlayerCommonActionData(Owner,
		CommonActionType::TOUCHPROP))
	{
		UGsAnimInstanceState* anim = Owner->GetAnimInstance();
		anim->SetPhysicsAnimation(commonSkill->physicsAnimation);

		if (UAnimMontage* res = GResource()->LoadSync<UAnimMontage>(commonSkill->aniPath))
		{
			AGsCharacterBase* caracterActor = Owner->GetCharacter();
			float len = caracterActor->PlayAnimMontage(res, 1.f, TEXT("End"));

			fsm->DelayProcessEvent(len * 0.8f, EGsStateBase::InteractionEnd, EGsStateBase::Idle);
		}
	}
	else
	{
		fsm->ProcessEvent(EGsStateBase::Idle);
	}
}

void FGsStateRemotePlayerInteractionEnd::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}