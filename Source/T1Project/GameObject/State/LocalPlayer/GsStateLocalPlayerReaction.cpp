// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateLocalPlayerReaction.h"
#include "GsStateLocalPlayerHeader.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject//ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Abnormality/CrowdControl/GsCrowdControlBase.h"
#include "DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityCrowdControlSet.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "Animation/GsAnimInstanceState.h"
#include "DataSchema/Skill/GsSchemaSkillReaction.h"

#if UE_EDITOR
#include "DrawDebugHelpers.h"
#endif

//----------------------------------------------------------------------------------------
// FGsStateLocalPlayerReaction
//----------------------------------------------------------------------------------------
uint8 FGsStateLocalPlayerReaction::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Reaction);
}

FName FGsStateLocalPlayerReaction::GetStateName() const
{
	return TEXT("StateLocalPlayerReaction");
}

FName FGsStateLocalPlayerReaction::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateLocalPlayerReaction::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		GS_STATE_GROUP_DIE:
		case EGsStateBase::Reaction:
		case EGsStateBase::ReactionEnd:
		case EGsStateBase::Revive:
		case EGsStateBase::Warp:
			return true;
		case EGsStateBase::Run: //공포
			return !IsLock(Owner);
		default:
			return false;
		}
	}
	return false;
}

void FGsStateLocalPlayerReaction::IgnoreState(UGsGameObjectBase* Owner, IGsStateBase* ChangeState)
{
	Super::IgnoreState(Owner, ChangeState);

	Exit(Owner);
}

void FGsStateLocalPlayerReaction::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);

	Enter(Owner);
}

void FGsStateLocalPlayerReaction::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	_isJumped = false;
	// 리액션 데이터를 통한 몽타주 플레이
	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();		

#if UE_BUILD_DEBUG
	DrawDebugSphere(Owner->GetWorld(), Owner->GetActor()->GetActorLocation() + FVector(0, 0, 50.0f), 10.f, 50.f, FColor::Red, false, 30.f);
#endif
	
	if (FGsSkillHandlerLocalPlayer* skillhandler = local->GetCastSkill<FGsSkillHandlerLocalPlayer>())
	{
		skillhandler->CompulsionCancelSkill(0);
		skillhandler->SetAutoIdleEmotionEnable(false);
	}

	if (UGsAnimInstanceState* anim = Owner->GetAnimInstance())
	{
		FGsAbnormalityHandlerBase* abnormalityHandler = Owner->GetAbnormalityHandler();
		FGsCrowdControlBase* crowdControl = abnormalityHandler->GetCrowdControl();
		// Animation Pose 설정
		if (crowdControl->IsPoseAnimation())
		{
			anim->Pose();
		}
		else
		{
			auto reaction = crowdControl->GetCurrentReaction();
			if (const FGsSchemaSkillReaction* reactionTable = crowdControl->GetCurrentReactionData())
			{
				// Loop 구간이 있는 리액션 데이터 설정
				if (crowdControl->IsVariableLengthReactionType(reactionTable->category))
				{
					AbnormalityEffectType subType = reaction.IsValid() ?
						reaction.Pin()->_data->type : AbnormalityEffectType::NONE;
					if (auto longerstData = abnormalityHandler->GetLongerstAbnormalityData(subType).Pin())
					{
						crowdControl->SetVariableLengthReaction(longerstData->GetRemainTime(), LoopReactionAnimationEndName);
					}
				}

				if (UAnimMontage* resAni = crowdControl->GetPlayAni())
				{
					anim->Resume();
					anim->Montage_Play(resAni);
					anim->SetPhysicsAnimation(reactionTable->physicsAnimation);
				}
				else
				{
					GSLOG(Warning, TEXT("Animation Resource is null"));
				}
			}
			else
			{
				GSLOG(Error, TEXT("	not valid ReactionData "));
			}
		}
	}
}

void FGsStateLocalPlayerReaction::Update(UGsGameObjectBase* Owner, float Delta)
{
	Super::Update(Owner, Delta);

	if (FGsAbnormalityHandlerBase* abnormalityHandler = Owner->GetAbnormalityHandler())
	{
		if (abnormalityHandler->IsStateChangedByAbnormality() == false)
		{
			FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::ReactionEnd);
		}

		FGsCrowdControlBase* crowdControl = abnormalityHandler->GetCrowdControl();
		const auto& reactionVariable = crowdControl->GetReactionVariableLength();
		if (reactionVariable.IsJumpToSection())
		{
			if (UAnimMontage* resAni = crowdControl->GetPlayAni())
			{
				UGsAnimInstanceState* anim = Owner->GetAnimInstance();
				anim->PlayAnimMontageJumpToSection(resAni, 1.f, reactionVariable._endSectionName);
			}

			crowdControl->ClearVariableLengthReaction();
			_isJumped = true;
		}
		else if (_isJumped && reactionVariable._endSectionName != NAME_None) //jump되고 reaction이 끝나기도 전에 같은 reaction이 실행
		{	
			ReEnter(Owner);
		}
	}
}

bool FGsStateLocalPlayerReaction::IsLock(class UGsGameObjectBase* Owner) const
{
	if (Owner->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT))
	{
		return true;
	}

	return false;
}

void FGsStateLocalPlayerReaction::Exit(UGsGameObjectBase* Owner)
{
	if (FGsAbnormalityHandlerBase* abnormalityHandler = Owner->GetAbnormalityHandler())
	{
		FGsCrowdControlBase* crowdControl = abnormalityHandler->GetCrowdControl();
		crowdControl->ClearVariableLengthReaction();
	}

	UGsAnimInstanceState* anim = Owner->GetAnimInstance();
	if (anim->IsPoseAnimation)
		anim->Resume();

	// 루트 모션 동기화
	if (anim->IsRootmotionAnimation)
	{
		UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
		local->SyncRootmotionPosition();
		anim->SetRootmotionAnimation(false);
	}

	if (auto skillHandler = Owner->GetSkillHandler())
	{
		skillHandler->SetAutoIdleEmotionEnable(true);
	}

	Super::Exit(Owner);
}

//----------------------------------------------------------------------------------------
// FGsStateLocalPlayerReactionEnd
//----------------------------------------------------------------------------------------
uint8 FGsStateLocalPlayerReactionEnd::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::ReactionEnd);
}

FName FGsStateLocalPlayerReactionEnd::GetStateName() const
{
	return TEXT("StateLocalPlayerReactionEnd");
}

FName FGsStateLocalPlayerReactionEnd::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateLocalPlayerReactionEnd::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Dying:
		case EGsStateBase::Idle:
		case EGsStateBase::ForwardWalk:
		case EGsStateBase::Run:
		case EGsStateBase::Reaction:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateLocalPlayerReactionEnd::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);
}

void FGsStateLocalPlayerReactionEnd::Update(UGsGameObjectBase* Owner, float Delta)
{
	// 다음 틱에도 다른 메세지가 오지 않으면 Idle 상태 전환을 요청한다.
	if (FGsGameObjectStateManager* stateMgr = Owner->GetBaseFSM())
	{
		stateMgr->ProcessEvent(EGsStateBase::Idle);
	}
}

void FGsStateLocalPlayerReactionEnd::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}