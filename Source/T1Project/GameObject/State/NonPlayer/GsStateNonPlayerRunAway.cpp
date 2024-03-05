#include "GsStateNonPlayerRunAway.h"
#include "GsStateNonPlayerHeader.h"
#include "../GsGameObjectStateManager.h"
#include "../ActorEx/GsCharacterBase.h"
#include "../Animation/GsAnimInstanceState.h"
#include "../../ObjectClass/GsGameObjectNonPlayerBase.h"
#include "../../Movement/GsMovementHandlerBase.h"

uint8 FGsStateNonPlayerRunAway::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::RunAway);
}

FName FGsStateNonPlayerRunAway::GetStateName() const
{
	return TEXT("StateTreasureMonsterRunAway");
}

FName FGsStateNonPlayerRunAway::GetBPStateName() const
{
	return TEXT("RunAway");
}

bool FGsStateNonPlayerRunAway::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		//case EGsStateBase::Dead:
		//case EGsStateBase::Dying:
		case EGsStateBase::Revive:
		case EGsStateBase::Spawn:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateNonPlayerRunAway::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	//position 이동이 오지 않을 때 제거 예정
	//UGsGameObjectNonPlayerBase* nonPlayer = Owner->CastGameObject<UGsGameObjectNonPlayerBase>();
	//if (nonPlayer)
	//{
	//	// Movement 클래스에 이동 설정 알림
	//	if (FGsMovementHandlerBase* movement = nonPlayer->GetMovementHandler())
	//	{
	//		movement->SetMoveState(false);
	//	}
	//}


	//if (FGsGameObjectStateManager* fsm = Owner->GetBaseFSM())
	//{
	//	if (AGsCharacterBase* characterActor = Owner->GetCharacter())
	//	{
	//		if (UGsAnimInstanceState* anim = characterActor->GetAnim())
	//		{
	//			float aniLen = anim->GetStateAnimationLength(fsm->GetBPStateName(), GetBPStateName());
	//			fsm->DelayProcessEvent(aniLen, EGsStateBase::RunAway, EGsStateBase::CommonActionEnd);
	//		}
	//	}
	//}
}

void FGsStateNonPlayerRunAway::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}