// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateRemotePlayerSocialAction.h"
#include "Animation/GsAnimInstanceState.h"
#include "Skill/GsSchemaSkillCommon.h"
#include "Data/GsResourceManager.h"
#include "../../Skill/GsSkillHandlerBase.h"
#include "../../ObjectClass/GsGameObjectCreature.h"

uint8 FGsStateRemotePlayerSocialAction::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::SocialAction);
}
FName FGsStateRemotePlayerSocialAction::GetStateName() const
{
	return TEXT("StateRemotePlayerSocialAction");
}
FName FGsStateRemotePlayerSocialAction::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateRemotePlayerSocialAction::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	return Super::CanTransition(Owner, StateID);
}

void FGsStateRemotePlayerSocialAction::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);

	Enter(Owner);
}

void FGsStateRemotePlayerSocialAction::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectCreature* creature = Owner->CastGameObject<UGsGameObjectCreature>();
	creature->ChangeIdleType(EGsAnimStateIdleType::Social);

	// 타임라인 개념이 모호하기 때문에 SkillRunner를 타지 않는다.
	UGsAnimInstanceState* anim = Owner->GetAnimInstance();
	FGsSkillHandlerBase* skillhandler = Owner->GetSkillHandler();
	if (const FGsSchemaSkillCommon* socialAction = skillhandler->GetCurrentSocialData())
	{
		anim->SetPhysicsAnimation(socialAction->physicsAnimation);

		if (UAnimMontage* res = GResource()->LoadSync<UAnimMontage>(socialAction->aniPath))
		{
			float len = anim->Montage_Play(res);
		}
	}
}

void FGsStateRemotePlayerSocialAction::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}