// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateRemotePlayerInteraction.h"
#include "GsStateRemotePlayerHeader.h"
#include "../../../ActorEx/GsCharacterBase.h"
#include "ActorEx/GsCharacterPlayer.h"
#include "Animation/GsAnimInstanceState.h"
#include "Data/GsResourceManager.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "../../Interaction/GsInteractionHandlerBase.h"
#include "../../ObjectClass/GsGameObjectRemotePlayer.h"
#include "../../ObjectClass/GsGameObjectPlayer.h"
#include "../../ObjectClass/GsGameObjectCamp.h"

uint8 FGsStateRemotePlayerInteraction::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Interaction);
}
FName FGsStateRemotePlayerInteraction::GetStateName() const
{
	return TEXT("StateRemotePlayerInteraction");
}
FName FGsStateRemotePlayerInteraction::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateRemotePlayerInteraction::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		//GS_STATE_GROUP_MOVE:
		GS_STATE_GROUP_ACTION_END:
		GS_STATE_GROUP_ACTION:
		GS_STATE_GROUP_ABNORLITY:
		GS_STATE_GROUP_DIE:
		case EGsStateBase::Warp:
			return true;
		default:
			return false;
		}
	}
	return false;
}
void FGsStateRemotePlayerInteraction::Enter(UGsGameObjectBase* inOwner)
{
	Super::Enter(inOwner);

	CommonActionType commonType = CommonActionType::TOUCHPROP;

	// 타임라인 개념이 모호하기 때문에 SkillRunner를 타지 않는다.
	UGsGameObjectRemotePlayer* remotePlayer = Cast<UGsGameObjectRemotePlayer>(inOwner);
	if (nullptr == remotePlayer)
		return;

	if (FGsInteractionHandlerBase* interactionHandler = remotePlayer->GetInteractionHandler())
	{
		if (UGsGameObjectBase* target = interactionHandler->GetInteractionTarget())
		{
			if (EGsGameObjectType::Camp == target->GetObjectType() ||
				EGsGameObjectType::Sanctum == target->GetObjectType())
			{
				commonType = CommonActionType::INTERACTION_CAMP;
			}
		}			
	}

	if (const FGsSchemaSkillCommon* commonSkill = GSkill()->GetPlayerCommonActionData(inOwner, commonType))
	{
		UGsAnimInstanceState* anim = inOwner->GetAnimInstance();
		anim->SetPhysicsAnimation(commonSkill->physicsAnimation);
		// 타임라인 개념이 모호하기 때문에 SkillRunner를 타지 않는다.
		//FGsSkillHandlerBase* skillhandler = _owner->GetSkillHandler();			
		//FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();
		//skillRunner->StartRunner(Owner, commonSkill);
		if (UAnimMontage* res = GResource()->LoadSync<UAnimMontage>(commonSkill->aniPath))
		{			
			if (AGsCharacterBase* caracterActor = inOwner->GetCharacter())
			{
				caracterActor->PlayAnimMontage(res, 1.f, TEXT("Start"));

				if (CommonActionType::INTERACTION_CAMP == commonType)
				{
					// 망치
					AGsCharacterPlayer* playerActor = remotePlayer->GetRemoteCharacter();
					if (nullptr != playerActor)
					{
						playerActor->SetToolWeaponStaticMeshVisible(true);
					}
				}
			}			
		}
	}
}

void FGsStateRemotePlayerInteraction::Exit(UGsGameObjectBase* Owner)
{
	UGsGameObjectRemotePlayer* remotePlayer = Cast<UGsGameObjectRemotePlayer>(Owner);
	if (nullptr == remotePlayer)
		return;

	if (FGsInteractionHandlerBase* interactionHandler = remotePlayer->GetInteractionHandler())
	{
		if (UGsGameObjectBase* target = interactionHandler->GetInteractionTarget())
		{
			if (target->IsA(UGsGameObjectCamp::StaticClass()))
			{
				// 망치
				AGsCharacterPlayer* playerActor = remotePlayer->GetRemoteCharacter();
				if (nullptr != playerActor)
				{
					playerActor->SetToolWeaponStaticMeshVisible(false);
				}
			}
		}
	}

	Super::Exit(Owner);
}