// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateLocalPlayerWarp.h"
#include "GsStateLocalPlayerHeader.h"

#include "ActorEx/GsCharacterLocalPlayer.h"
#include "Animation/GsAnimInstanceState.h"

#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerLocalPlayer.h"
#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsCameraModeManager.h"
#include "ManageMent/GsMessageHolder.h"

#include "Message/GsMessageSystem.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "UI/UIContent/Tray/GsUITrayFadeEffect.h"

uint8 FGsStateLocalPlayerWarp::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Warp);
}

FName FGsStateLocalPlayerWarp::GetStateName() const
{
	return TEXT("FGsStateLocalPlayerWarp");
}

FName FGsStateLocalPlayerWarp::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateLocalPlayerWarp::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		GS_STATE_GROUP_ABNORLITY:
		GS_STATE_GROUP_DIE:
		case EGsStateBase::Warp:
		// [B1] | ejrrb10 | 워프 시 카메라 연출을 보여줄 때까지 기다리게 되면서, Exit 이 아닌 ReEnter 가 불린다
		// 자동이동 중에 워프 연출이 무시되는 상황을 해결하기 위해 상태를 Warp 와 Spawn 으로만 제한한다.
		//case EGsStateBase::CommonActionEnd:
		//case EGsStateBase::Idle:
		case EGsStateBase::Spawn:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateLocalPlayerWarp::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	// TargetClear
	if (FGsTargetHandlerBase* targetHandler = Owner->GetTargetHandler())
	{
		targetHandler->ClearTarget(true);
	}


	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();

	if (nullptr == local)
	{
		GSLOG(Error, TEXT("nullptr == local"));
		return;
	}

	UGsAnimInstanceState* anim = local->GetLocalCharacter()->GetAnim();
	if (FGsSkillHandlerBase* skill = local->GetSkillHandler())
	{
		// 기존 스킬 종료
		skill->CompulsionCancelSkill(0);

		FGsSkillRunnerBase* skillRunner = skill->GetSkillRunner();
		if (const FGsSchemaSkillCommon* commonData = GSkill()->GetPlayerCommonActionData(Owner, CommonActionType::TELEPORT))
		{
			skillRunner->StartRunner(Owner, commonData);

			anim->SetPhysicsAnimation(commonData->physicsAnimation);
		}

		if (FGsGameObjectStateManager* fsm = local->GetBaseFSM())
		{
			if (UAnimMontage* resAni = skillRunner->GetAni())
			{
#ifdef OLD_WARP_ANI
				anim->Montage_Play(resAni);
#else
				float len = anim->Montage_Play(resAni);
				float warpAnimationCutRate = GData()->GetGlobalData()->_warpAnimationCutRate;
				fsm->DelayProcessEvent(len* warpAnimationCutRate , EGsStateBase::Warp, EGsStateBase::Warp);
				GMessage()->GetSystem().SendMessage(MessageSystem::LOAD_TOPO_REQUEST_WARP);
#endif
			}
			else
			{
				fsm->ProcessEvent(EGsStateBase::Idle);
			}
		}
	}
	
	// [B1] | ejrrb10 | Spawn 연출이 시작하면 움직임 제한 켜기 -> 서버 쪽으로 기능 이관
	//if (FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>())
	//{
	//	movement->SetMoveable(false);
	//}
}

void FGsStateLocalPlayerWarp::Update(UGsGameObjectBase* Owner, float Delta)
{
	Super::Update(Owner, Delta);
	FGsStateLocalPlayerCommonAction::Update(Owner, Delta);
}

void FGsStateLocalPlayerWarp::Exit(UGsGameObjectBase* Owner)
{
	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	local->SetLocalPlayerHidden();

	GMessage()->GetGameObject().SendMessage(MessageGameObject::COMMON_TELEPORT, nullptr);

	GMessage()->GetSystem().SendMessage(MessageSystem::LOAD_TOPO_END_WARP_EFFECT);

	FGsStateLocalPlayerCommonAction::Exit(Owner);

	Super::Exit(Owner);
}
void FGsStateLocalPlayerWarp::ReEnter(UGsGameObjectBase* Owner)
{
	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	local->SetLocalPlayerHidden();

	Super::ReEnter(Owner);

	GMessage()->GetSystem().SendMessage(MessageSystem::LOAD_TOPO_END_WARP_EFFECT);
}
