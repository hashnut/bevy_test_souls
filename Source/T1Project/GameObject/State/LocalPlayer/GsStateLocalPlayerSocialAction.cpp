// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLocalPlayerSocialAction.h"
#include "Animation/GsAnimInstanceState.h"
#include "Skill/GsSchemaSkillCommon.h"
#include "Data/GsResourceManager.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/ObjectClass/GsGameObjectPlayer.h"
#include "GameObject/Customize/GsCustomizeHandler.h"
#include "GameObject/Data/GsGameObjectDataPlayer.h"

uint8 FGsStateLocalPlayerSocialAction::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::SocialAction);
}
FName FGsStateLocalPlayerSocialAction::GetStateName() const
{
	return TEXT("StateLocalPlayerSocialAction");
}
FName FGsStateLocalPlayerSocialAction::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateLocalPlayerSocialAction::CanTransition(UGsGameObjectBase* Owner, int StateID) const
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
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateLocalPlayerSocialAction::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);

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

void FGsStateLocalPlayerSocialAction::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	// 소셜 Idle 상태 전달
	UGsGameObjectPlayer* player = Owner->CastGameObject<UGsGameObjectPlayer>();
	player->ChangeIdleType(EGsAnimStateIdleType::Social);

	// 타임라인 개념이 모호하기 때문에 SkillRunner를 타지 않는다.
	UGsAnimInstanceState* anim = Owner->GetAnimInstance();
	FGsSkillHandlerBase* skillhandler = Owner->GetSkillHandler();
	
	// 몽타주 강제 정지
	anim->StopMontagePlay();
	// 소켓 강제 변경
	FGsGameObjectDataPlayer* playerData = player->GetCastData<FGsGameObjectDataPlayer>();
	CreatureWeaponType currWeapon = playerData->GetCurrentWeaponType();
	const FGsSchemaUserData* pcTable = player->GetPcTable();

	if (auto* customizeHandler = player->GetCustomizeHandler())
	{
		customizeHandler->ChangeWeaponSocket(pcTable, currWeapon, false);
	}
	
	
	skillhandler->SetAutoIdleEmotionEnable(false);

	if (const FGsSchemaSkillCommon* socialAction = skillhandler->GetCurrentSocialData())
	{
		anim->SetPhysicsAnimation(socialAction->physicsAnimation);

		if (UAnimMontage* res = GResource()->LoadSync<UAnimMontage>(socialAction->aniPath))
		{
			float len = anim->Montage_Play(res);
		}
	}
}

void FGsStateLocalPlayerSocialAction::Exit(UGsGameObjectBase* Owner)
{
	// 일반 Idle 상태로 되돌림
	UGsGameObjectPlayer* player = Owner->CastGameObject<UGsGameObjectPlayer>();
	player->ChangeIdleType(player->IsBattleMode() ? EGsAnimStateIdleType::Battle : EGsAnimStateIdleType::Normal);

	if(auto skillhandler = Owner->GetSkillHandler())
		skillhandler->SetAutoIdleEmotionEnable(true);

	// 무기 소켓 정보 되돌림
	if (FGsCustomizeHandler* customizeHandler = player->GetCustomizeHandler())
	{
		FGsGameObjectDataPlayer* playerData = player->GetCastData<FGsGameObjectDataPlayer>();
		CreatureWeaponType currWeapon = playerData->GetCurrentWeaponType();
		const FGsSchemaUserData* pcTable = player->GetPcTable();
		customizeHandler->ChangeWeaponSocket(pcTable, currWeapon, player->IsBattleMode());
	}

	Super::Exit(Owner);
}