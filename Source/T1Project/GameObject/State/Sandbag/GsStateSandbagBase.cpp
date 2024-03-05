
#include "GsStateSandbagBase.h"
#include "GsStateSandbagHeader.h"
#include "Animation/GsAnimInstanceState.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GsStateSandbagIdle.h"
#include "GsStateSandbagDying.h"
#include "GsStateSandbagDead.h"
#include "Animation/GsSandbagAnimInstance.h"

IGsStateBase* FGsStateBaseSingleSandbag::GetState(uint8 StateID) const
{
	switch (static_cast<EGsStateBase>(StateID))
	{
	case EGsStateBase::Spawn:
		return FGsStateSandbagSpawn::GetInstance();
	case EGsStateBase::Idle:
		return FGsStateSandbagIdle::GetInstance();
	case EGsStateBase::Hit:
		return FGsStateSandbagHit::GetInstance();
	case EGsStateBase::Dying:
		return FGsStateSandbagDying::GetInstance();
	case EGsStateBase::Dead:
		return FGsStateSandbagDead::GetInstance();		
	default:
		GSLOG(Error, TEXT("State Type undefined %d"), StateID);
		return nullptr;
	}
	return nullptr;
}

void FGsStateBaseSingleSandbag::Enter(UGsGameObjectBase* Owner)
{
	if (UGsAnimInstanceState* anim = Owner->GetAnimInstance())
	{
		// 몽타주 플레이 종료 확인
		if (AllowMotionStop())
		{
			anim->StopMontagePlay();
		}

		anim->ChangeState(IsMontagePlayState() ? GetDefaultStateId(Owner) : GetStateID(),
			GetAniRandomCount());
	}
}

void FGsStateSandbagMoveBase::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectNonPlayerBase* nonPlayer = Owner->CastGameObject<UGsGameObjectNonPlayerBase>();
	if (nonPlayer)
	{
		// Movement 클래스에 이동 설정 알림
		if (FGsMovementHandlerBase* movement = nonPlayer->GetMovementHandler())
		{
			movement->SetMoveState(false);
		}
	}
}

void FGsStateSandbagMoveBase::Exit(UGsGameObjectBase* Owner)
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
	}
}

void FGsStateSandbagMoveBase::Update(UGsGameObjectBase* Owner, float Delta)
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