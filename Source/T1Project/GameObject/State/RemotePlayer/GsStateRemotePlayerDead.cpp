// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateRemotePlayerDead.h"
#include "GsStateRemotePlayerHeader.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Data/GsResourceManager.h"
#include "Skill/GsSchemaSkillCommon.h"
#include "ActorEx/GsCharacterPlayer.h"
#include "Animation/GsAnimInstanceState.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "GameObject/Skill/GsSkillHandlerRemotePlayer.h"
#include "../../ObjectClass/GsGameObjectRemotePlayer.h"
#include "../../ObjectClass/GsGameObjectBase.h"
#include "Animation/AnimInstance.h"


uint8 FGsStateRemotePlayerDead::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Dead);
}
FName FGsStateRemotePlayerDead::GetStateName() const
{
	return TEXT("GsStateRemotePlayerDead");
}
FName FGsStateRemotePlayerDead::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateRemotePlayerDead::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Revive:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateRemotePlayerDead::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectRemotePlayer* remote = Owner->CastGameObject<UGsGameObjectRemotePlayer>();

	UGsAnimInstanceState* anim = remote->GetCharacter()->GetAnim();
	FGsSkillHandlerBase* skillhandler = remote->GetSkillHandler();
	if (const FGsSchemaSkillCommon* commonData = GSkill()->GetPlayerCommonActionData(Owner, CommonActionType::DYING))
	{
		anim->SetPhysicsAnimation(commonData->physicsAnimation);

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

	if (AGsCharacterPlayer* characterActor = remote->GetRemoteCharacter())
	{
		characterActor->SetEnableDeathActor(true);
	}
}

void FGsStateRemotePlayerDead::Exit(UGsGameObjectBase* Owner)
{
	// 이 곳에 들어오지 않음. Exit 되기 전에 Despawn패킷이 와서,
	// UGsGameObjectRemotePlayer::OnDespawn()에서 무덤을 디스폰시킨다.
	UGsGameObjectRemotePlayer* remote = Owner->CastGameObject<UGsGameObjectRemotePlayer>();
	if (nullptr != remote)
	{
		if (AGsCharacterPlayer* character = remote->GetRemoteCharacter())
		{
			character->SetEnableDeathActor(false);
		}
	}

	Super::Exit(Owner);
}
