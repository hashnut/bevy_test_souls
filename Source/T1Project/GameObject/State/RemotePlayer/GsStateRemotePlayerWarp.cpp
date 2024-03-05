#include "GsStateRemotePlayerWarp.h"
#include "GsStateRemotePlayerHeader.h"

#include "ActorEx/GsCharacterBase.h"

#include "Animation/GsAnimInstanceState.h"
#include "../../ObjectClass/GsGameObjectCreature.h"
#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"
#include "GameObject/Skill/GsSkillHandlerRemotePlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerCreature.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Fairy/GsFairyHandlerPlayer.h"
#include "GameObject/Movement/GsMovementHandlerRemote.h"


#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Engine/Classes/Engine/World.h"
#include "Engine/Public/TimerManager.h"

uint8 FGsStateRemotePlayerWarp::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Warp);
}

FName FGsStateRemotePlayerWarp::GetStateName() const
{
	return TEXT("FGsStateRemotePlayerWarp");
}

FName FGsStateRemotePlayerWarp::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateRemotePlayerWarp::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::CommonActionEnd:
		case EGsStateBase::Dying:
		case EGsStateBase::Reaction:
		case EGsStateBase::Idle:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateRemotePlayerWarp::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	// TargetClear
	if (FGsTargetHandlerBase* targetHandler = Owner->GetTargetHandler())
	{
		targetHandler->ClearTarget();
	}

	UGsGameObjectRemotePlayer* remote = Owner->CastGameObject<UGsGameObjectRemotePlayer>();
	if (remote == nullptr)
	{
		return;
	}

	// [U1] | ejrrb10 | 타 유저 입장에서 움직이면서 워프하는 상황을 막기 위한 처리.
	// LocalPlayerAutoMove 가 Exit 할 때의 기능을 이 녀석도 해 주어야 한다.
	// Movement 클래스에 이동 설정 알림


	// [U1] | ejrrb10 | 타 유저 입장에서 움직이면서 워프하는 상황을 막기 위한 처리. 
	remote->StopActive();

	if (FGsMovementHandlerRemote* movementHandler = remote->GetCastMovement<FGsMovementHandlerRemote>())
	{
		// [U2] | ejrrb10 | 40% 확률로 타 플레이어가 사라지는 현상의 원인은 StopRunnerMovement 임 (C2URWQ-6284)
		//movementHandler->ResetNetLocation();
		//movementHandler->StopRunnerMovement();
	 	//movementHandler->SetMoveState(false);

		movementHandler->ReceveNetMotionStopReason(MotionStopReason::WARP);
//#if WITH_EDITOR
//		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Purple,
//			*FString::Printf(TEXT("FGsMovementHandlerRemote : Stop Remote Player")));
//#endif
	}

	// sentry null 참조 예외 처리
	// https://com2us-raon.sentry.io/issues/4276237033/?alert_rule_id=12817173&alert_timestamp=1687797947161&alert_type=email&environment=Release&project=4504115468566528&referrer=alert_email
	if (remote->GetCharacter() == nullptr)
	{
		return;
	}

	UGsAnimInstanceState* anim = remote->GetCharacter()->GetAnim();
	if (anim == nullptr)
	{
		return;
	}

	if (FGsSkillHandlerBase* skill = remote->GetSkillHandler())
	{
		// 기존 스킬 종료
		skill->CompulsionCancelSkill(0);

		FGsSkillRunnerBase* skillRunner = skill->GetSkillRunner();
		if (nullptr == skillRunner)
		{
			return;
		}

		if (const FGsSchemaSkillCommon* commonData = GSkill()->GetPlayerCommonActionData(Owner, CommonActionType::TELEPORT))
		{
			skillRunner->StartRunner(Owner, commonData);

			anim->SetPhysicsAnimation(commonData->physicsAnimation);
		}

		if (FGsGameObjectStateManager* fsm = remote->GetBaseFSM())
		{
			if (UAnimMontage* resAni = skillRunner->GetAni())
			{
				float time = anim->Montage_Play(resAni);
				float warpAnimationCutRate = GData()->GetGlobalData()->_warpAnimationCutRate;
				fsm->DelayProcessEvent(time * warpAnimationCutRate, EGsStateBase::Warp, EGsStateBase::Idle);
			}
			else
			{
				fsm->ProcessEvent(EGsStateBase::Idle);
			}
		}
	}
}

void FGsStateRemotePlayerWarp::Exit(UGsGameObjectBase* Owner)
{
	// [B1] | ejrrb10 | 타 유저 입장에서도 워프 연출이 끝나가는 시점에서 액터를 숨겨주어
	// 워프 몽타주가 끝날 때 다시 Idle 로 전환되는 어색한 장면을 숨긴다.
	UGsGameObjectRemotePlayer* remote = Owner->CastGameObject<UGsGameObjectRemotePlayer>();

	remote->SetRemotePlayerHidden();
	if (FGsMovementHandlerRemote* movementHandler = remote->GetCastMovement<FGsMovementHandlerRemote>())
	{
		movementHandler->SetMoveState(true);
	}
	Super::Exit(Owner);
}
