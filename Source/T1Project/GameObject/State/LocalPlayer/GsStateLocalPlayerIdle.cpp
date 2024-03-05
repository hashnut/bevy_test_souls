// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateLocalPlayerIdle.h"
#include "GsStateLocalPlayerHeader.h"

#include "Input/GsInputBindingLocalPlayer.h"

#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/Looting/GsLootingHandlerLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"
#include "GameObject/Movement/LocalPlayer/Auto/GsWaypointWork.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "AI/Quest/GsAIQuestHandler.h"

#include "Management/ScopeGame/GsAIManager.h"


uint8 FGsStateLocalPlayerIdle::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Idle);
}

FName FGsStateLocalPlayerIdle::GetStateName() const
{
	return TEXT("StateLocalPlayerIdle");
}

FName FGsStateLocalPlayerIdle::GetBPStateName() const
{
	return TEXT("Idle");
}

bool FGsStateLocalPlayerIdle::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
			// 효과모션 플레이 하기
		//case EGsStateBase::Idle:
		//	ChangeIdleMotion(Owner);
		//	break;
		GS_STATE_GROUP_MOVE:
		GS_STATE_GROUP_ACTION_END:
		GS_STATE_GROUP_ACTION:
		GS_STATE_GROUP_ABNORLITY:
		GS_STATE_GROUP_DIE:		
		case EGsStateBase::Warp:
		case EGsStateBase::ChangeIdle:
		case EGsStateBase::ChangeWeapon:
		case EGsStateBase::SpaceCrackIdle:
		case EGsStateBase::Spawn:
			return true;
		case EGsStateBase::Social:
			return !CheckBattleIdle(Owner);
		case EGsStateBase::Hit:
			return Owner->LegacyHit;

		default:
			return false;
		}
	}
	return false;
}

void FGsStateLocalPlayerIdle::Exit(UGsGameObjectBase* Owner)
{
	UGsGameObjectLocalPlayer* castObj = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	if (FGsGameObjectStateManager* fsm = castObj->GetBaseFSM())
	{
		// 다음 상태들에서는 Idle로 간주
		if (false == fsm->IsState(EGsStateBase::Social))
		{
			castObj->SetForceDisconnectIdle(false);
		}
	}

	Super::Exit(Owner);
}

void FGsStateLocalPlayerIdle::ChangeIdleMotion(UGsGameObjectBase* Owner)
{
	//UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	//UGsAnimInstanceState* anim = local->GetLocalCharacter()->GetAnim();
	// Anim BP 상태와 비교한다.
	//if (!anim->IsAreaType(local->GetAreaType()))
	//{
	//	FGsSkillHandlerLocalPlayer* skillMgr = static_cast<FGsSkillHandlerLocalPlayer*>(local->GetSkill());
	//	check(skillMgr);

	//	const FSoftObjectPath& path = anim->IsAreaType(EAreaType::NonBattleArea) ?
	//		skillMgr->GetSummativeAnimation().BattleStart :
	//		skillMgr->GetSummativeAnimation().BattleEnd;

	//	UAnimMontage* ani = GData()->GetAnimationData()->GetMontage(path);
	//	anim->Montage_Play(ani);

	//	// Area 타입 연동
	//	anim->ChangeAreaType(local->GetAreaType());
	//}
}

void FGsStateLocalPlayerIdle::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectLocalPlayer* castObj = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	castObj->ChangeIdleType(castObj->IsBattleMode() ? EGsAnimStateIdleType::Battle : EGsAnimStateIdleType::Normal);

	castObj->SetForceDisconnectIdle(true);

	if (FGsTargetHandlerLocalPlayer* localTarget = castObj->GetCastTarget<FGsTargetHandlerLocalPlayer>())
	{
		if (localTarget->GetManualModeAutoTarget())
		{
			if (FGsSkillHandlerLocalPlayer* skillHandler = castObj->GetCastSkill<FGsSkillHandlerLocalPlayer>())
			{
				skillHandler->OnManualModeTargetAttackAction();
			}
		}
	}
}

void FGsStateLocalPlayerIdle::Update(UGsGameObjectBase* Owner, float Delta)
{	
	Super::Update(Owner, Delta);
}

bool FGsStateLocalPlayerIdle::CheckBattleIdle(UGsGameObjectBase* Owner) const
{
	if (auto creature = Owner->CastGameObject<UGsGameObjectCreature>())
		return creature->IsBattleMode();
	return false;
}

bool FGsStateLocalPlayerIdle::Ready(UGsGameObjectBase* Owner)
{
	if (Super::Ready(Owner))
	{
		// 예약된 항목 검사 실시

		UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
		FGsGameObjectStateManager* fsm = local->GetBaseFSM();
		if (FGsLootingHandlerLocalPlayer* lootingHandler = local->GetLootingHandler())
		{
			if (lootingHandler->IsReserveLootng())
			{
				FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::Looting);
				return false;
			}
		}
		if (FGsInteractionHandlerLocalPlayer* interactionHandler = local->GetCastInteraction<FGsInteractionHandlerLocalPlayer>())
		{
			if (interactionHandler->IsReserveInteraction())
			{
				FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::Interaction);
				return false;
			}
		}

		if (FGsMovementHandlerLocalPlayer* localMovement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>())
		{
			if (FGsWaypointWork* wayPointWork =
				localMovement->GetStateAutoMove().GetWaypointWork())
			{
				if (wayPointWork->IsReserveData())
				{
					wayPointWork->ProcessWaypoint();
					return false;
				}
			}
		}

		
		if (UGsInputBindingLocalPlayer* localInput = local->GetInputBinder())
		{
			if (localInput->IsReserveTouchMoveData())
			{
				localInput->ProcessReserveTouchMove();
				return false;
			}
		}

		if (FGsAIManager* aiMgr = GSAI())
		{
			if (FGsAIQuestHandler* aiQuestHandler = aiMgr->GetAIQuestHandler())
			{
				if (aiQuestHandler->IsReserveQuestKillMoveData())
				{
					aiQuestHandler->ProcessReserveQuestKillMove();
					return false;
				}
			}
		}

		if (FGsSkillHandlerLocalPlayer* skillHandler = local->GetCastSkill<FGsSkillHandlerLocalPlayer>())
		{
			if (skillHandler->IsReserveData())
			{
				skillHandler->OnReserveSkill();
				return false;
			}
		}
	}
	return true;
}