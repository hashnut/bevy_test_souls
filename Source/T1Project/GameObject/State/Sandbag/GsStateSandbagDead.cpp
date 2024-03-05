// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateSandbagDead.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Data/GsResourceManager.h"
#include "Animation/AnimInstance.h"
#include "Animation/GsSandbagAnimInstance.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"

uint8 FGsStateSandbagDead::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Dead);
}

FName FGsStateSandbagDead::GetStateName() const
{
	return TEXT("StateSandbagDead");
}

FName FGsStateSandbagDead::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateSandbagDead::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{		
		case EGsStateBase::Spawn:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateSandbagDead::Enter(UGsGameObjectBase* Owner)
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
}

void FGsStateSandbagDead::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}