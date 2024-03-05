// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateLocalPlayerSpawn.h"
#include "GsStateLocalPlayerHeader.h"

#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Skill/Runner/GsSkillRunnerLocalPlayer.h"
#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "Animation/GsAnimInstanceState.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsCameraModeManager.h"
#include "ActorEx/GsCharacterBase.h"

#include "UTIL/GsGameObjectUtil.h"

#include "Net/GsNetSendServiceWorld.h"

uint8 FGsStateLocalPlayerSpawn::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Spawn);
}

FName FGsStateLocalPlayerSpawn::GetStateName() const
{
	return TEXT("StateLocalPlayerSpawn");
}

FName FGsStateLocalPlayerSpawn::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateLocalPlayerSpawn::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		// [B1] | ejrrb10 | 전투 중에 스폰 처리될 경우 스폰이 무시되는 예외
		// 예외적으로, Idle 만 Next 로 들어왔을 때 처리가 가능하도록 해야 함
		// 추가적으로 기존에 예약된 상태를 체크한다
		case EGsStateBase::Idle:
		case EGsStateBase::Looting:
		case EGsStateBase::Interaction:
		case EGsStateBase::AutoMove:
		case EGsStateBase::Warp:
		case EGsStateBase::Spawn:
			return true;
		default:
			return false;
		}
	}

	return false;
}

void FGsStateLocalPlayerSpawn::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);
	Enter(Owner);
}

void FGsStateLocalPlayerSpawn::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();

	if (FGsGameObjectStateManager* fsm = local->GetBaseFSM())
	{
		EGsLoadTopoEffectType effectType = GLevel()->GetLoadTopoEffectType();
		if (EGsLoadTopoEffectType::SAME_MAP_SAME_POS == effectType)
		{
			fsm->ProcessEvent(EGsStateBase::Idle);
		}
		else
		{
			// GameObject 클래스의 Initialize 타이밍에 들어온 거라면 UGsAnimInstanceState 생성 전일수 있다.
			if (UGsAnimInstanceState* anim = local->GetLocalCharacter()->GetAnim())
			{
				if (FGsSkillHandlerBase* skill = local->GetSkillHandler())
				{
					FGsSkillRunnerBase* skillRunner = skill->GetSkillRunner();
					if (const FGsSchemaSkillCommon* commonData = GSkill()->GetPlayerCommonActionData(Owner, CommonActionType::SPAWN))
					{
						skillRunner->StartRunner(Owner, commonData);

						anim->SetPhysicsAnimation(commonData->physicsAnimation);
					}

					if (UAnimMontage* resAni = skillRunner->GetAni())
					{
						float len = anim->Montage_Play(resAni);
						fsm->DelayProcessEvent(len, EGsStateBase::Spawn, EGsStateBase::Idle);
					}
					else
					{
						fsm->ProcessEvent(EGsStateBase::Idle);
					}
				}
			}
		}
	}

	// [B1] | ejrrb10 | 스폰 상태에 진입하면, 스폰 연출할 준비가 완료되었다는 신호를 보낸다
	// 동일 맵 로드 등의 상황으로 이미 CS_REQ_SPAWN_ME_ACTION 가 보내졌다면 패스될 것이다
	FGsNetSendServiceWorld::SendReqSpawnMeAction();
}

void FGsStateLocalPlayerSpawn::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
	if (FGsSkillHandlerBase* skill = Owner->GetSkillHandler())
	{
		FGsSkillRunnerBase* skillRunner = skill->GetSkillRunner();
		skillRunner->EndRunner();
	}

	// [B1] | ejrrb10 | Spawn 연출이 끝나면 움직임의 제한을 풀어 주어야 함 -> 서버 쪽으로 기능 이관
	//UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	//if (FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>())
	//{
	//	movement->SetMoveable(true);
	//}
}