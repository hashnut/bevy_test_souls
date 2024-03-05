// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateNonPlayerDead.h"
#include "GsStateNonPlayerHeader.h"
#include "Animation/GsAnimInstanceState.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Data/GsResourceManager.h"
#include "Animation/AnimInstance.h"
#include "Animation/GsAnimInstanceState.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"
#include "../../ObjectClass/GsGameObjectBase.h"
#include "../../ObjectClass/GsGameObjectNonPlayer.h"


uint8 FGsStateNonPlayerDead::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Dead);
}

FName FGsStateNonPlayerDead::GetStateName() const
{
	return TEXT("StateNonPlayerDead");
}

FName FGsStateNonPlayerDead::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateNonPlayerDead::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Revive:
		case EGsStateBase::Spawn:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateNonPlayerDead::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	if (UGsAnimInstanceState* anim = Owner->GetAnimInstance())
	{
		if (const FGsSchemaSkillCommon* commonData = GSkill()->GetNpcCommonActionData(Owner, CommonActionType::DYING))
		{
			if (UAnimMontage* resAni = GResource()->LoadSync<UAnimMontage>(commonData->aniPath))
			{
				UAnimMontage* playAni = anim->GetCurrentActiveMontage();

				if (resAni != playAni)
				{
					anim->Montage_Play(resAni);
				}

				anim->Montage_SetPosition(resAni, resAni->SequenceLength);
				anim->Montage_Pause(resAni);
			}
		}
	}

	if (UGsGameObjectNonPlayer* pNonePlayer = Owner->CastGameObject<UGsGameObjectNonPlayer>())
	{
		pNonePlayer->ChangeCollisionPresetToPawn();
	}
}

void FGsStateNonPlayerDead::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}