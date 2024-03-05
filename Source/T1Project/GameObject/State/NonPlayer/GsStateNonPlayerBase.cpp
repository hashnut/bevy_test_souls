
#include "GsStateNonPlayerBase.h"
#include "GsStateNonPlayerHeader.h"
#include "Animation/GsAnimInstanceState.h"
#include "ActorEx/GsCharacterNonPlayer.h"

#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GameObject/Skill/GsSkillHandlerNonPlayer.h"

#include "Management/GsMessageHolder.h"

IGsStateBase* FGsStateBaseSingleNonPlayer::GetState(uint8 StateID) const
{
	switch (static_cast<EGsStateBase>(StateID))
	{
	case EGsStateBase::Spawn:
		return FGsStateNonPlayerSpawn::GetInstance();
	case EGsStateBase::Idle:
		return FGsStateNonPlayerIdle::GetInstance();
	case EGsStateBase::ForwardWalk:
		return FGsStateNonPlayerWalk::GetInstance();
	case EGsStateBase::Run:
		return FGsStateNonPlayerRun::GetInstance();
	case EGsStateBase::Attack:
		return FGsStateNonPlayerAttack::GetInstance();
	case EGsStateBase::AttackEnd:
		return FGsStateNonPlayerIdle::GetInstance();
	case EGsStateBase::Hit:
		return FGsStateNonPlayerHit::GetInstance();
	case EGsStateBase::Reaction:
		return FGsStateNonPlayerReaction::GetInstance();
	case EGsStateBase::ReactionEnd:
		return FGsStateNonPlayerIdle::GetInstance();
	case EGsStateBase::Dying:
		return FGsStateNonPlayerDying::GetInstance();
	case EGsStateBase::Dead:
		return FGsStateNonPlayerDead::GetInstance();
	case EGsStateBase::Roar:
		return FGsStateNonPlayerRoar::GetInstance();
	//case EGsStateBase::AutoMove:
	//	return FGsStateNonPlayerAutoMove::GetInstance();
	//case EGsStateBase::CommonAction:
	//	return FGsStateNonPlayerCommonAction::GetInstance();
	//case EGsStateBase::CommonActionEnd:
	//	return FGsStateNonPlayerIdle::GetInstance();
	//case EGsStateBase::ChangeIdle:
	//	return FGsStateNonPlayerChangeIdle::GetInstance();
	//case EGsStateBase::Teleport:
	//	return FGsStateNonPlayerTeleport::GetInstance();
	case EGsStateBase::Social:
		return FGsStateNonPlayerSocial::GetInstance();
	case EGsStateBase::SocialAction:
		return FGsStateNonPlayerSocialAction::GetInstance();
	//case EGsStateBase::Revive:
	//	return FGsStateNonPlayerRevive::GetInstance();
	//case EGsStateBase::Freeze:
	//	return FGsStateNonPlayerFreezeState::GetInstance();
	//case EGsStateBase::UnFreeze:
	//	return FGsStateNonPlayerIdle::GetInstance();
	//case EGsStateBase::Ride:
	//	return FGsStateNonPlayerRide::GetInstance();
	case EGsStateBase::SpaceCrackIdle:
		return FGsStateNonPlayerSpaceCrackIdle::GetInstance();
	case EGsStateBase::RunAway:
		return FGsStateNonPlayerRunAway::GetInstance();
	default:
		GSLOG(Error, TEXT("State Type undefined %d"), StateID);
		return nullptr;
	}
	return nullptr;
}

bool FGsStateBaseSingleNonPlayer::Ready(UGsGameObjectBase* Owner)
{
	return Owner->CastGameObject<UGsGameObjectCreature>()->GetActor() != nullptr ? true : false;
}

void FGsStateBaseSingleNonPlayer::Enter(UGsGameObjectBase* Owner)
{
	if (UGsAnimInstanceState* anim = Owner->GetAnimInstance())
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

	// 상태 변화에 따른 말풍선 출력
	if (AGsCharacterBase* characterBase = Owner->GetCharacter())
	{
		if (AGsCharacterNonPlayer* nonPlayer = Cast<AGsCharacterNonPlayer>(characterBase))
		{
			nonPlayer->ShowTalkBalloonOnChangedState(GetStateID());
		}
	}
}

void FGsStateBaseSingleNonPlayer::Exit(UGsGameObjectBase* Owner)
{
}

void FGsStateNonPlayerMoveBase::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectNonPlayerBase* nonPlayer = Owner->CastGameObject<UGsGameObjectNonPlayerBase>();
	if (nonPlayer)
	{
		// Movement 클래스에 이동 설정 알림
		if (FGsMovementHandlerBase* movement = nonPlayer->GetMovementHandler())
		{
			movement->SetMoveState(true);
		}
		if (auto skillHandler = nonPlayer->GetSkillHandler())
		{
			skillHandler->SetAutoIdleEmotionEnable(false);
		}
	}
}

void FGsStateNonPlayerMoveBase::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);

	UGsGameObjectNonPlayerBase* nonPlayer = Owner->CastGameObject<UGsGameObjectNonPlayerBase>();
	if (nonPlayer)
	{
		if (FGsMovementHandlerBase* movement = nonPlayer->GetMovementHandler())
		{
			// Movement 클래스에 이동 설정 알림
			movement->SetMoveState(false);
		}

		if (auto skillHandler = nonPlayer->GetSkillHandler())
		{
			skillHandler->SetAutoIdleEmotionEnable(true);
		}
	}
}

void FGsStateNonPlayerMoveBase::Update(UGsGameObjectBase* Owner, float Delta)
{
	Super::Update(Owner, Delta);

	// 블렌딩 처리를 위한 이동 속도값 알림
	// NonPlayer는 직접 속도값을 설정하자...
	// FGsMovementHandlerRemote 클래스에서 Motion타입을 얻어오기가 조금 애매
	if (FGsMovementHandlerBase* movement = Owner->GetMovementHandler())
	{
		if (UGsAnimInstanceState* anim = Owner->GetAnimInstance())
		{
			anim->SetMoveSpeed(movement->GetMoveSpeed());
		}
	}
}
