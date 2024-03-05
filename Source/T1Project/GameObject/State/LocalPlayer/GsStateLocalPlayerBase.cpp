// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateLocalPlayerBase.h"

#include "GsStateLocalPlayerHeader.h"
#include "Animation/GsAnimInstanceState.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerLocalPlayer.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "../../Movement/GsMovementHandlerBase.h"
#include "../../Camera/GsCameraModeHandler.h"
#include "../Management/GsMessageHolder.h"

float FGsStateBaseSingleLocalPlayer::_staticLocalStateTimer = 0.f;

IGsStateBase* FGsStateBaseSingleLocalPlayer::GetState(uint8 StateID) const
{
	switch (static_cast<EGsStateBase>(StateID))
	{
	case EGsStateBase::Spawn:
		return FGsStateLocalPlayerSpawn::GetInstance();
	case EGsStateBase::Idle:
		return FGsStateLocalPlayerIdle::GetInstance();
	case EGsStateBase::ForwardWalk:
		return FGsStateLocalPlayerWalk::GetInstance();
	case EGsStateBase::Run:
		return FGsStateLocalPlayerRun::GetInstance();
	case EGsStateBase::AutoMove:
		return FGsStateLocalPlayerAutoMove::GetInstance();
	case EGsStateBase::AutoMoveStop:
		return FGsStateLocalPlayerAutoMoveStop::GetInstance();
	case EGsStateBase::Casting:
		return FGsStateLocalPlayerCasting::GetInstance();
	case EGsStateBase::Attack:
		return FGsStateLocalPlayerAttack::GetInstance();
	case EGsStateBase::AttackEnd:
		return FGsStateLocalPlayerActionEnd::GetInstance();
	case EGsStateBase::Hit:
		return FGsStateLocalPlayerHit::GetInstance();
	case EGsStateBase::Reaction:
		return FGsStateLocalPlayerReaction::GetInstance();
	case EGsStateBase::ReactionEnd:
		return FGsStateLocalPlayerActionEnd::GetInstance();
	//case EGsStateBase::CommonAction:
	//	return FGsStateLocalPlayerCommonAction::GetInstance();
	case EGsStateBase::CommonActionEnd:
		return FGsStateLocalPlayerActionEnd::GetInstance();
	case EGsStateBase::ChangeIdle:
		return FGsStateLocalPlayerChangeIdle::GetInstance();
	case EGsStateBase::Dying:
		return FGsStateLocalPlayerDying::GetInstance();
	case EGsStateBase::Dead:
		return FGsStateLocalPlayerDead::GetInstance();
	case EGsStateBase::Ride:
		return FGsStateLocalPlayerRide::GetInstance();
	case EGsStateBase::Roar:
	case EGsStateBase::Warp:
		return FGsStateLocalPlayerWarp::GetInstance();
	//case EGsStateBase::Social:
	//	return FGsStateLocalPlayerCommonAction::GetInstance();
	case EGsStateBase::Revive:
		return FGsStateLocalPlayerRevive::GetInstance();
	case EGsStateBase::Freeze:
		return FGsStateLocalPlayerFreezeState::GetInstance();
	case EGsStateBase::UnFreeze:
		return FGsStateLocalPlayerActionEnd::GetInstance();
	case EGsStateBase::Interaction:
		return FGsStateLocalPlayerInteraction::GetInstance();
	case EGsStateBase::InteractionEnd:
		return FGsStateLocalPlayerInteractionEnd::GetInstance();
	case EGsStateBase::Looting:
		return FGsStateLocalPlayerLooting::GetInstance();
	case EGsStateBase::ChangeWeapon:
		return FGsStateLocalPlayerChangeWeapon::GetInstance();
	case EGsStateBase::ChangeWeaponEnd:
		return FGsStateLocalPlayerChangeWeaponEnd::GetInstance();
	case EGsStateBase::Social:
		return FGsStateLocalPlayerSocial::GetInstance();
	case EGsStateBase::SocialAction:
		return FGsStateLocalPlayerSocialAction::GetInstance();
	default:
		GSLOG(Error, TEXT("State Type undefined %d"), StateID);
		return nullptr;
	}
	return nullptr;
}

void FGsStateBaseSingleLocalPlayer::Enter(UGsGameObjectBase* Owner)
{
	if (UGsAnimInstanceState* anim  = Owner->GetAnimInstance())
	{
		// 몽타주 플레이 종료 확인
		if (AllowMotionStop())
		{
			anim->StopMontagePlay();
		}

		// AnimBP 스테이트와 연동된 상태는 AnimBP 에 전달한다.
		// 아닐 경우 디폴트 상태로 연동
		anim->ChangeState(IsMontagePlayState() ? GetDefaultStateId(Owner) : GetStateID(),
			GetAniRandomCount());
	}

	_staticLocalStateTimer = 0.f;
}

void FGsStateBaseSingleLocalPlayer::Update(UGsGameObjectBase* Owner, float Delta)
{
	if (CheckDeadlock())
	{
		_staticLocalStateTimer += Delta;
		if (_staticLocalStateTimer > GetCheckDelayTime())
		{
			// 이전 상태도 확인
			FGsGameObjectStateManager* fsm = Owner->GetBaseFSM();

			GSLOG(Error, TEXT("LocalPlayer State DeadLock %s, PrevState : %s"), 
				*GetStateName().ToString(), *fsm->PrevState()->GetStateName().ToString());

			// 복구 실행
			IGsStateBase* state = fsm->CurrentState();
			FGsStateBaseSingleLocalPlayer* castState = FGsClassUtil::FCheckStaticCast<FGsStateBaseSingleLocalPlayer>(state);
			castState->Restore(Owner);

			// 10초에 한번씩으로 재검사
			_staticLocalStateTimer = GetCheckDelayTime() - 10.f;

			// 현재는 개발중이므로 강제 종료
			// checkNoEntry();
		}
	}
}

void FGsStateBaseSingleLocalPlayer::Exit(UGsGameObjectBase* Owner)
{
}

bool FGsStateBaseSingleLocalPlayer::Ready(UGsGameObjectBase* Owner)
{
	return Owner->CastGameObject<UGsGameObjectLocalPlayer>()->GetLocalCharacter() != nullptr ? true : false;
}

void FGsStateLocalPlayerMoveBase::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	if (FGsMovementHandlerBase* movement = Owner->GetMovementHandler())
	{
		// Movement 클래스에 이동 설정 알림
		movement->SetMoveState(true);
	}

	if (auto skillHandler = Owner->GetSkillHandler())
	{
		skillHandler->SetAutoIdleEmotionEnable(false);
	}

	// 페어리 버프 이펙트 확인
	if (UGsGameObjectCreature* castObject = Owner->CastGameObject<UGsGameObjectCreature>())
	{
		castObject->OnVehicleEffect(false);
	}

	// 자동 사용 스킬 갱신 요청
	FGsGameObjectMessageParamBuffSkill messageParam(Owner, 0, FGsGameObjectMessageParamBuffSkill::BuffSkillAuto::Enable);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_AUTOSKILL_UPDATE, &messageParam);
}

void FGsStateLocalPlayerMoveBase::Update(UGsGameObjectBase* Owner, float Delta)
{
	Super::Update(Owner, Delta);

	//이동 Update 활성
	if (FGsMovementHandlerBase* movement = Owner->GetMovementHandler())
	{
		FGsMovementDirtyMark mark(movement);

		movement->Update(Delta);

		// 블렌딩 처리를 위한 이동 속도값 알림
		if (UGsAnimInstanceState* anim = Owner->GetAnimInstance())
		{
			anim->SetMoveSpeed(movement->GetMotionType());
		}
	}

	// 카메라 모드 이동 갱신
	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	if (FGsCameraModeHandler* cameraModehandler = local->GetCameraModeHandler())
	{
		cameraModehandler->GetState()->UpdateMove(false);
	}
}

void FGsStateLocalPlayerMoveBase::Exit(UGsGameObjectBase* Owner)
{
	if (FGsMovementHandlerBase* movement = Owner->GetMovementHandler())
	{
		// Movement 클래스에 이동 설정 알림
		movement->SetMoveState(false);
	}

	if (auto skillHandler = Owner->GetSkillHandler())
	{
		skillHandler->SetAutoIdleEmotionEnable(true);
	}

	// 페어리 버프 이펙트 제거
	if (UGsGameObjectCreature* castObject = Owner->CastGameObject<UGsGameObjectCreature>())
	{
		castObject->OnRestoreVehicleEffect();
	}

	FGsGameObjectMessageParamBuffSkill messageParam(Owner, 0, FGsGameObjectMessageParamBuffSkill::BuffSkillAuto::Disable);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_AUTOSKILL_UPDATE, &messageParam);

	Super::Exit(Owner);
}
//직접으로 Run->Run CanTransition false, Fear + polymorph / Fear + polymorph 해소 시에 reactionEnd -> 다시 Run
//automove 시에 polymorph 도 다시 Run
void FGsStateLocalPlayerMoveBase::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);

	if (IsLock(Owner) || IsShapeChangedLock(Owner)) Enter(Owner);
}

bool FGsStateLocalPlayerMoveBase::IsLock(UGsGameObjectBase* Owner) const
{
	if (FGsAbnormalityHandlerBase* abnormalityHandler = Owner->GetAbnormalityHandler())
	{
		return abnormalityHandler->IsAbnormalityEffectType(AbnormalityEffectType::CC_FEAR);
	}
	return false;
}

bool FGsStateLocalPlayerMoveBase::IsShapeChangedLock(class UGsGameObjectBase* Owner) const
{
	if (FGsAbnormalityHandlerBase* abnormalityHandler = Owner->GetAbnormalityHandler())
	{
		return abnormalityHandler->IsAbnormalityEffectType(AbnormalityEffectType::CC_POLYMORP);
	}
	return false;
}


//----------------------------------------------------------------------------------------
// FGsStateLocalPlayerCommonAction
//----------------------------------------------------------------------------------------
void FGsStateLocalPlayerCommonAction::Enter(UGsGameObjectBase* Owner)
{
	/*UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	FGsSkillHandlerBase* skillhandler = local->GetSkillHandler();
	FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();*/

	/*skillRunner->StartRunner();
	if (UAnimMontage* resAni = skillMgr->GetSkillRunner()->GetAni())
	{
		UGsAnimInstanceState* anim = local->GetLocalCharacter()->GetAnim();
		anim->Montage_Play(resAni);
	}
	else
	{
		GSLOG(Warning, TEXT("Animation Resource is null"));
	}*/
}

void FGsStateLocalPlayerCommonAction::Update(UGsGameObjectBase* Owner, float Delta)
{
	// 이동 스킬일 경우 이동 Update 활성
	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	FGsSkillHandlerBase* skillhandler = local->GetSkillHandler();
	FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();

	if (skillRunner->CanMove())
	{
		// 이동값만 반영
		// 나중에 서버 동기화용 로직이 필요
		local->GetCastMovement<FGsMovementHandlerLocalPlayer>()->Update(Delta);
	}
}

void FGsStateLocalPlayerCommonAction::Exit(UGsGameObjectBase* Owner)
{
}
