// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateNonPlayerReaction.h"
#include "GsStateNonPlayerHeader.h"

#include "Animation/GsAnimInstanceState.h"

#include "DataSchema/Skill/GsSchemaSkillReaction.h"
#include "DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityCrowdControlSet.h"

#include "GameObject/Abnormality/CrowdControl/GsCrowdControlBase.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "T1Project.h"

//----------------------------------------------------------------------------------------
// GsStateNonPlayerReaction
//----------------------------------------------------------------------------------------
uint8 FGsStateNonPlayerReaction::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Reaction);
}

FName FGsStateNonPlayerReaction::GetStateName() const
{
	return TEXT("StateNonPlayerReaction");
}

FName FGsStateNonPlayerReaction::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateNonPlayerReaction::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Dying:
		case EGsStateBase::Reaction:
		// case EGsStateBase::Idle:
		case EGsStateBase::ForwardWalk:
		case EGsStateBase::Run:
		case EGsStateBase::Attack:
		case EGsStateBase::ReactionEnd:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateNonPlayerReaction::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);

	Enter(Owner);
}

void FGsStateNonPlayerReaction::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	_isJumped = false;
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
					anim->Montage_Play(resAni);
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

void FGsStateNonPlayerReaction::Exit(UGsGameObjectBase* Owner)
{
	if (UGsAnimInstanceState* anim = Owner->GetAnimInstance())
	{
		if (anim->IsPoseAnimation)
			anim->Resume();

		anim->StopMontagePlay();

		// 루트 모션 동기화
		if (anim->IsRootmotionAnimation)
		{
			anim->SetRootmotionAnimation(false);
		}
	}

	if (FGsAbnormalityHandlerBase* abnormalityHandler = Owner->GetAbnormalityHandler())
	{
		FGsCrowdControlBase* crowdControl = abnormalityHandler->GetCrowdControl();
		crowdControl->ClearVariableLengthReaction();
	}

	Super::Exit(Owner);
}

void FGsStateNonPlayerReaction::Update(UGsGameObjectBase* Owner, float Delta)
{
	Super::Update(Owner, Delta);

	if (FGsAbnormalityHandlerBase* abnormalityHandler = Owner->GetAbnormalityHandler())
	{
		if (UGsAnimInstanceState* anim = Owner->GetAnimInstance())
		{
			FGsCrowdControlBase* crowdControl = abnormalityHandler->GetCrowdControl();
			const auto& reactionVariable = crowdControl->GetReactionVariableLength();
			if (reactionVariable.IsJumpToSection())
			{
				if (UAnimMontage* resAni = crowdControl->GetPlayAni())
				{
					anim->PlayAnimMontageJumpToSection(resAni, 1.f, reactionVariable._endSectionName);
				}

				crowdControl->ClearVariableLengthReaction();
				_isJumped = true;
			}
			else if (_isJumped && reactionVariable._endSectionName != NAME_None) //jump되고 reaction이 끝나기도 전에 같은 reaction이 실행
			{
				ReEnter(Owner);
			}

			// 리액션 상황이 끝났는지 검증
			if (abnormalityHandler->IsStateChangedByAbnormality() == false)
			{
				FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::ReactionEnd);
			}
		}
	}

	// 리액션 상황이 끝났는지 검증
	//if (false == Owner->IsCrowdControlLock(EGsCrowdControlType::StunType))
	//{
	//	FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::Idle);
	//}
}