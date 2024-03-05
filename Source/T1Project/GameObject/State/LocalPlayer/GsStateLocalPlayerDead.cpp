// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLocalPlayerDead.h"
#include "GsStateLocalPlayerHeader.h"

#include "Animation/GsAnimInstanceState.h"
#include "ActorEx/GsCharacterLocalPlayer.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/ScopeGame/GsObserverManager.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"

#include "Data/GsResourceManager.h"
#include "Data/GsDataContainManager.h"

#include "Skill/GsSchemaSkillCommon.h"

uint8 FGsStateLocalPlayerDead::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Dead);
}

FName FGsStateLocalPlayerDead::GetStateName() const
{
	return TEXT("StateLocalPlayerDead");
}

FName FGsStateLocalPlayerDead::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateLocalPlayerDead::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Revive:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateLocalPlayerDead::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	if (FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>())
	{
		movement->SetMoveable(false);
	}
	//// 이동 동기화 Stop처리
	//FGsMovementHandlerBase* movement = local->GetMovementHandler();
	//movement->Stop(false);	
	
	// 애니메이션 마지막 모션으로 고정 처리(임시)
	UGsAnimInstanceState* anim = local->GetLocalCharacter()->GetAnim();
	if (const FGsSchemaSkillCommon* commonSkillData = GSkill()->GetPlayerCommonActionData(Owner, CommonActionType::DYING))
	{
		anim->SetPhysicsAnimation(commonSkillData->physicsAnimation);

		if (UAnimMontage* resAni = GResource()->LoadSync<UAnimMontage>(commonSkillData->aniPath))
		{
			UAnimMontage* playAni = anim->GetCurrentActiveMontage();

			if (resAni != playAni)
			{
				anim->Montage_Play(resAni);
			}

			anim->Montage_SetPosition(resAni, resAni->SequenceLength);
			anim->Montage_Pause(resAni);
		}
	}

	if (AGsCharacterLocalPlayer* characterActor = local->GetLocalCharacter())
	{
		characterActor->PlayDeathEffect();
		//character->StopCurrentSkillEffectNSound();
		characterActor->SetEnableDeathActor(true);
	}

	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_DEAD, nullptr);

	// 시공의 틈새 저장 안함
	// 관전 모드 저장 안함
	if (GLevel()->IsSpaceCrackMapContents() == false &&
		GSObserver()->GetIsObserveStart() == false)
	{
		// 카메라 백업
		GSCameraMode()->SaveBackUpTransform();
	}
	
	_maxTime = GData()->GetGlobalData()->_revivePopupShowDelayTime;
	_isShowRevive = false;
}

void FGsStateLocalPlayerDead::Exit(UGsGameObjectBase* Owner)
{
	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	if (FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>())
	{
		movement->SetMoveable(true);
	}


	UGsAnimInstanceState* anim = local->GetLocalCharacter()->GetAnim();
	anim->StopMontagePlay();
	   
	if (AGsCharacterLocalPlayer* characterActor = local->GetLocalCharacter())
	{
		characterActor->StopDeathEffect();
		characterActor->SetEnableDeathActor(false);
	}

	Super::Exit(Owner);
}

void FGsStateLocalPlayerDead::Update(class UGsGameObjectBase* inOwner, float inDelta)
{
	Super::Update(inOwner, inDelta);

	if (false == _isShowRevive)
	{		
		if (_staticLocalStateTimer > _maxTime)
		{
			_isShowRevive = true;
			GMessage()->GetUI().SendMessage(MessageUI::SHOW_REVIVE, nullptr);
		}
	}	
}
