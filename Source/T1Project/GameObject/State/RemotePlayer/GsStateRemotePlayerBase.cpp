
#include "GsStateRemotePlayerBase.h"
#include "GsStateRemotePlayerHeader.h"
#include "Animation/GsAnimInstanceState.h"
#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"
#include "GameObject/Movement/GsMovementHandlerRemote.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"

IGsStateBase* FGsStateBaseRemotePlayer::GetState(uint8 StateID) const
{
	switch (static_cast<EGsStateBase>(StateID))
	{
	case EGsStateBase::Spawn:
		return FGsStateRemotePlayerSpawn::GetInstance();
	case EGsStateBase::Idle:
		return FGsStateRemotePlayerIdle::GetInstance();
	// 타유저는 걷기가 없음
	case EGsStateBase::ForwardWalk:
		return FGsStateRemotePlayerWalk::GetInstance();
	case EGsStateBase::Run:
		return FGsStateRemotePlayerRun::GetInstance();
	case EGsStateBase::Casting:
		return FGsStateRemotePlayerCasting::GetInstance();
	case EGsStateBase::Attack:
		return FGsStateRemotePlayerAttack::GetInstance();
	case EGsStateBase::AttackEnd:
		return FGsStateRemotePlayerIdle::GetInstance();
	case EGsStateBase::Hit:
		return FGsStateRemotePlayerHit::GetInstance();
	case EGsStateBase::Reaction:
		return FGsStateRemotePlayerReaction::GetInstance();
	case EGsStateBase::ReactionEnd:
		return FGsStateRemotePlayerIdle::GetInstance();
	case EGsStateBase::Dying:
		return FGsStateRemotePlayerDying::GetInstance();
	case EGsStateBase::Dead:
		return FGsStateRemotePlayerDead::GetInstance();
	case EGsStateBase::Revive:
		return FGsStateRemotePlayerRevive::GetInstance();
	case EGsStateBase::CommonActionEnd:
		return FGsStateRemotePlayerIdle::GetInstance();
	case EGsStateBase::ChangeIdle:
		return FGsStateRemotePlayerChangeIdle::GetInstance();
	case EGsStateBase::Interaction:
		return FGsStateRemotePlayerInteraction::GetInstance();
	case EGsStateBase::InteractionEnd:
		return FGsStateRemotePlayerInteractionEnd::GetInstance();
	case EGsStateBase::ChangeWeapon:
		return FGsStateRemotePlayerChangeWeapon::GetInstance();
	case EGsStateBase::Warp:
		return FGsStateRemotePlayerWarp::GetInstance();
	case EGsStateBase::Social:
		return FGsStateRemotePlayerSocial::GetInstance();
	case EGsStateBase::SocialAction:
		return FGsStateRemotePlayerSocialAction::GetInstance();
	//case EGsStateBase::AutoMove:
	//	return FGsStateRemotePlayerAutoMove::GetInstance();
	//case EGsStateBase::Roar:
	//case EGsStateBase::CommonAction:
	//	return FGsStateRemotePlayerCommonAction::GetInstance();
	//case EGsStateBase::CommonActionEnd:
	//	return FGsStateRemotePlayerIdle::GetInstance();
	//case EGsStateBase::ChangeIdle:
	//	return FGsStateRemotePlayerChangeIdle::GetInstance();
	//case EGsStateBase::Ride:
	//	return FGsStateRemotePlayerRide::GetInstance();
	//case EGsStateBase::Teleport:
	//	return FGsStateRemotePlayerTeleport::GetInstance();
	//case EGsStateBase::Social:
	//	return FGsStateRemotePlayerCommonAction::GetInstance();
	//case EGsStateBase::Freeze:
	//	return FGsStateRemotePlayerFreezeState::GetInstance();
	//case EGsStateBase::UnFreeze:
	//	return FGsStateRemotePlayerIdle::GetInstance();
	default:
		GSLOG(Error, TEXT("State Type undefined %d"), StateID);
		return nullptr;
	}
	return nullptr;
}

void FGsStateBaseRemotePlayer::Enter(UGsGameObjectBase* Owner)
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
		if (IsMontagePlayState())
		{
			// 몽타주 플레이중 공격시에는 목적지에 로컬모션처리를 IDle로 변경하지 않는다. 
			// 타유저의 경우에는 딜레이시 미끄러짐 현상등을 볼수 있는데 분절을 통해서 애니메이션 처리를 하기위함
			anim->ChangeState(EGsStateBase::Attack == (EGsStateBase)GetStateID() ? GetStateID() : GetDefaultStateId(Owner),
				GetAniRandomCount());
		}
		else
		{
			anim->ChangeState(GetStateID(), GetAniRandomCount());
		}
	}	
}

void FGsStateBaseRemotePlayer::Exit(UGsGameObjectBase* Owner)
{	
}

void FGsStateRemotePlayerMoveBase::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	if (FGsMovementHandlerBase* movement = Owner->GetMovementHandler())
	{
		// Movement 클래스에 이동 설정 알림
		movement->SetMoveState(true);
	}

	// 페어리 버프 이펙트 확인
	if (UGsGameObjectCreature* castObject = Owner->CastGameObject<UGsGameObjectCreature>())
	{
		castObject->OnVehicleEffect(false);
	}
}

void FGsStateRemotePlayerMoveBase::Update(UGsGameObjectBase* Owner, float Delta)
{
	Super::Update(Owner, Delta);

	// 블렌딩 처리를 위한 이동 속도값 알림
	if (FGsMovementHandlerBase* movement = Owner->GetMovementHandler())
	{
		if (UGsAnimInstanceState* anim = Owner->GetAnimInstance())
		{
			anim->SetMoveSpeed(movement->GetMotionType());
		}
	}
}

void FGsStateRemotePlayerMoveBase::Exit(UGsGameObjectBase* Owner)
{
	if (FGsMovementHandlerBase* movement = Owner->GetMovementHandler())
	{
		// Movement 클래스에 이동 설정 알림
		movement->SetMoveState(false);
	}

	// 페어리 버프 이펙트 제거
	if (UGsGameObjectCreature* castObject = Owner->CastGameObject<UGsGameObjectCreature>())
	{
		castObject->OnRestoreVehicleEffect();
	}

	Super::Exit(Owner);
}

void FGsStateRemotePlayerMoveBase::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);

	if (IsLock(Owner) || IsShapeChangedLock(Owner)) 
		Enter(Owner);
}

bool FGsStateRemotePlayerMoveBase::IsLock(UGsGameObjectBase* Owner) const
{
	if (FGsAbnormalityHandlerBase* abnormalityHandler = Owner->GetAbnormalityHandler())
	{
		return abnormalityHandler->IsAbnormalityEffectType(AbnormalityEffectType::CC_FEAR);
	}
	return false;
}

bool FGsStateRemotePlayerMoveBase::IsShapeChangedLock(class UGsGameObjectBase* Owner) const
{
	if (FGsAbnormalityHandlerBase* abnormalityHandler = Owner->GetAbnormalityHandler())
	{
		return abnormalityHandler->IsAbnormalityEffectType(AbnormalityEffectType::CC_POLYMORP);
	}
	return false;
}
