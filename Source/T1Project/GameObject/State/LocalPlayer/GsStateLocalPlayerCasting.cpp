// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLocalPlayerCasting.h"
#include "GsStateLocalPlayerHeader.h"
#include "ActorEx/GsCharacterBase.h"
#include "Net/GsNetSendServiceWorld.h"
#include "Data/GsResourceManager.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "GameObject//ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject//Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "UTIL/GsTableUtil.h"

uint8 FGsStateLocalPlayerCasting::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Casting);
}
FName FGsStateLocalPlayerCasting::GetStateName() const
{
	return TEXT("StateLocalPlayerCasting");
}
FName FGsStateLocalPlayerCasting::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateLocalPlayerCasting::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		// GS_STATE_GROUP_ACTION:
		GS_STATE_GROUP_ACTION_END:
			return CanCancel(Owner);
		// GS_STATE_GROUP_MOVE:
		GS_STATE_GROUP_ABNORLITY:
		GS_STATE_GROUP_DIE:
		case EGsStateBase::Warp:
		// case EGsStateBase::ChangeIdle:
		case EGsStateBase::ChangeWeapon:
		case EGsStateBase::Attack:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateLocalPlayerCasting::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectLocalPlayer* local		= Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	AGsCharacterBase* characterActor	= Owner->GetCharacter();
	FGsSkillHandlerLocalPlayer* skillhandler = local->GetCastSkill<FGsSkillHandlerLocalPlayer>();
	const FGsSkillNetData* netSkillData = skillhandler->GetNetSkillData();
	if (nullptr == netSkillData)
	{
		FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::Idle);
		return;
	}
	const FGsSchemaSkillSet* skillData =
		UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(netSkillData->_skillId);
	if (nullptr == skillData)
	{
		FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::Idle);
		return;
	}
	
	if (UAnimMontage* res = GResource()->LoadSync<UAnimMontage>(skillData->aniPath))
	{
		float len = characterActor->PlayAnimMontage(res, skillhandler->GetCastSpeedRate());
	}
}

void FGsStateLocalPlayerCasting::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);	
}

void FGsStateLocalPlayerCasting::Update(UGsGameObjectBase* Owner, float Delta)
{
	Super::Update(Owner, Delta);
//  	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
// 	if (FGsSkillHandlerLocalPlayer* skillhandler = local->GetCastSkill<FGsSkillHandlerLocalPlayer>())
// 	{
// 		if (false == skillhandler->IsCasting())
// 		{
// 			FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::AttackEnd);
// 		}
// 	}
}

bool FGsStateLocalPlayerCasting::CanCancel(UGsGameObjectBase* Owner) const
{
	UGsGameObjectLocalPlayer* local	= Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	FGsSkillHandlerLocalPlayer* skillhandler = local->GetCastSkill<FGsSkillHandlerLocalPlayer>();
	return skillhandler->IsCasting() == false || skillhandler->IsCastingComplete();
}