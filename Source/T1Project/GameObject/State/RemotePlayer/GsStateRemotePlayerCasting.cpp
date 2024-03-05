// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateRemotePlayerCasting.h"
#include "GsStateRemotePlayerHeader.h"
#include "ActorEx/GsCharacterBase.h"
#include "Data/GsResourceManager.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"
#include "GameObject/Skill/GsSkillHandlerRemotePlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"

uint8 FGsStateRemotePlayerCasting::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Casting);
}
FName FGsStateRemotePlayerCasting::GetStateName() const
{
	return TEXT("StateRemotePlayerCasting");
}
FName FGsStateRemotePlayerCasting::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateRemotePlayerCasting::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	// 이동 모션보다는 캐스팅 모션의 우선순위가 높다.
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Casting:
		case EGsStateBase::Attack:
		case EGsStateBase::AttackEnd:
		case EGsStateBase::Dying:
		case EGsStateBase::Dead:
		case EGsStateBase::Reaction:
		case EGsStateBase::ChangeWeapon:
		case EGsStateBase::Warp:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateRemotePlayerCasting::ReEnter(UGsGameObjectBase* Owner)
{
	Enter(Owner);
}

void FGsStateRemotePlayerCasting::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectCreature* creature	= Owner->CastGameObject<UGsGameObjectCreature>();
	FGsSkillHandlerRemotePlayer* skillhandler = creature->GetCastSkill<FGsSkillHandlerRemotePlayer>();
	if (const FGsSkillNetData* netSkillData = skillhandler->GetNetSkillData())
	{
		FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();
		const FGsSchemaSkillSet* skillTableData =
			UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(netSkillData->_skillId);
		float fSpeedRate = skillTableData->isNormal ? skillhandler->GetAttackSpeedRate() :
			skillhandler->GetCastSpeedRate();
		skillRunner->StartRunner(Owner, skillTableData, fSpeedRate, netSkillData);

		if (AGsCharacterBase* characterActor = Owner->GetCharacter())
		{
			// 캐스팅 애니메이션
			if (UAnimMontage* res = GResource()->LoadSync<UAnimMontage>(skillTableData->aniPath))
			{
				float len = characterActor->PlayAnimMontage(res, fSpeedRate);
			}
		}
	}
}

void FGsStateRemotePlayerCasting::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}