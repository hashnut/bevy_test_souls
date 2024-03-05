// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLocalPlayerDying.h"
#include "GsStateLocalPlayerHeader.h"
#include "Animation/GsAnimInstanceState.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "UI/UILib/Define/GsUIDefine.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"



uint8 FGsStateLocalPlayerDying::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Dying);
}

FName FGsStateLocalPlayerDying::GetStateName() const
{
	return TEXT("StateLocalPlayerDying");
}

FName FGsStateLocalPlayerDying::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateLocalPlayerDying::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Spawn:
		case EGsStateBase::Revive:
		case EGsStateBase::Dead:
			return true;

		default:
			return false;
		}
	}
	return false;
}

void FGsStateLocalPlayerDying::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	if (FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>())
	{
		movement->SetMoveable(false);

		// Stop처리 필요없는지?
		//movement->Stop();
	}

	if (AGsCharacterLocalPlayer* actor = local->GetLocalCharacter())
	{
		actor->PlayDeathEffect();
		actor->StopCurrentSkillEffectNSound();
	}

	// TargetClear
	if (FGsTargetHandlerBase* targetHandler = Owner->GetTargetHandler())
	{
		targetHandler->ClearTarget(true);
	}

	UGsAnimInstanceState* anim = local->GetLocalCharacter()->GetAnim();
	if (FGsSkillHandlerBase* skillhandler = local->GetSkillHandler())
	{
		// 기존 스킬 종료
		skillhandler->CompulsionCancelSkill(0);

		FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();
		if (const FGsSchemaSkillCommon* commonData = GSkill()->GetPlayerCommonActionData(Owner, CommonActionType::DYING))
		{
			skillRunner->StartRunner(Owner, commonData);
			anim->SetPhysicsAnimation(commonData->physicsAnimation);
		}

		if (FGsGameObjectStateManager* fsm = local->GetBaseFSM())
		{
			if (UAnimMontage* resAni = skillRunner->GetAni())
			{
				float len = anim->Montage_Play(resAni);
				fsm->DelayProcessEvent(len * 0.8f, EGsStateBase::Dying, EGsStateBase::Dead);
			}
			else
			{
				fsm->ProcessEvent(EGsStateBase::Dead);
			}
		}

	}

	//사망시 모든 UI 꺼짐( 파티 던전에서는 예외 )
	if (MapType::PARTY_DUNGEON != GLevel()->GetCurrentMapType())
	{
		GUI()->CloseAllStack();
		GUI()->SetHUDMode(EGsUIHUDMode::NORMAL);
		GUI()->ClearHideFlags(EGsUIHideFlags::UI_HIDE_ALL);
		GUI()->SetHideFlags(EGsUIHideFlags::STATE_DIE);
	}	
}

void FGsStateLocalPlayerDying::Exit(UGsGameObjectBase* Owner)
{
	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	if (FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>())
	{
		movement->SetMoveable(true);
	}

	Super::Exit(Owner);
}
