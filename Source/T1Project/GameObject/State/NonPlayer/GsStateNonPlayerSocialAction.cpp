// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateNonPlayerSocialAction.h"
#include "GsStateNonPlayerHeader.h"
#include "Skill/GsSchemaSkillCommon.h"
#include "Data/GsResourceManager.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"

uint8 FGsStateNonPlayerSocialAction::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::SocialAction);
}

FName FGsStateNonPlayerSocialAction::GetStateName() const
{
	return TEXT("StateNonPlayerSocialAction");
}

FName FGsStateNonPlayerSocialAction::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateNonPlayerSocialAction::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (static_cast<EGsStateBase>(StateID) == EGsStateBase::Social)
	{
		return false;
	}

	return Super::CanTransition(Owner, StateID);
}

void FGsStateNonPlayerSocialAction::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);

	Enter(Owner);
}

void FGsStateNonPlayerSocialAction::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectCreature* creature = Owner->CastGameObject<UGsGameObjectCreature>();
	creature->ChangeIdleType(EGsAnimStateIdleType::Social);

	// 타임라인 개념이 모호하기 때문에 SkillRunner를 타지 않는다.
	if (UGsAnimInstanceState* anim = Owner->GetAnimInstance())
	{
		FGsSkillHandlerBase* skillhandler = Owner->GetSkillHandler();
		skillhandler->SetAutoIdleEmotionEnable(false);

		if (const FGsSchemaSkillCommon* socialAction = skillhandler->GetCurrentSocialData())
		{
			if (UAnimMontage* res = GResource()->LoadSync<UAnimMontage>(socialAction->aniPath))
			{
				float len = anim->Montage_Play(res);
			}
		}
	}
}

void FGsStateNonPlayerSocialAction::Exit(UGsGameObjectBase* Owner)
{
	if (auto skillhandler = Owner->GetSkillHandler())
		skillhandler->SetAutoIdleEmotionEnable(true);

	Super::Exit(Owner);
}
