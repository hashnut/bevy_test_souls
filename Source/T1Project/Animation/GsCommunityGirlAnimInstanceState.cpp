// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/GsCommunityGirlAnimInstanceState.h"
#include "T1Project.h"

void FGsCommunityGirlAnimInstanceStateProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	Super::PreUpdate(InAnimInstance, DeltaSeconds);

	if (UGsCommunityGirlAnimInstanceState* castAnim = Cast<UGsCommunityGirlAnimInstanceState>(InAnimInstance))
	{
		if (StateType != castAnim->CurrentStateType)
		{
			StateType = castAnim->CurrentStateType;
			ChangeStateAnimation(StateType);
			RandomIndex = castAnim->AnimationBlendIndex;
		}
	}
}

void FGsCommunityGirlAnimInstanceStateProxy::PostUpdate(UAnimInstance* InAnimInstance) const
{
	Super::PostUpdate(InAnimInstance);
}

void FGsCommunityGirlAnimInstanceStateProxy::PostEvaluate(UAnimInstance* InAnimInstance)
{
	Super::PostEvaluate(InAnimInstance);

	if (const FMontageEvaluationState* montage = GetActiveMontageEvaluationState())
	{
		IsActiveMontage = montage->bIsActive;
	}
	else
	{
		IsActiveMontage = false;
	}
}

void FGsCommunityGirlAnimInstanceStateProxy::InitializeStateAnimation()
{
	IsSpawn = false;
	IsIdle = false;
	IsIdleBreak = false;
	IsSelectab = false;
	IsTouchAct = false;
	RandomIndex = 0;
}

void FGsCommunityGirlAnimInstanceStateProxy::ChangeStateAnimation(EGsStateBase State)
{
	InitializeStateAnimation();

	GSLOG(Log, TEXT("ChangeStateAnimation %d"), State);

	switch (State)
	{
	case EGsStateBase::Spawn:
		IsSpawn = true;
		break;
	case EGsStateBase::Idle:
		IsIdle = true;
		break;
	case EGsStateBase::Social:
		IsSelectab = true;
		break;
	case EGsStateBase::SocialAction:
		IsTouchAct = true;
		break;
	case EGsStateBase::Freeze:
		IsIdleBreak = true;
		break;
	default:
		IsIdle = true;
		break;
	}
}

FAnimNode_StateMachine* FGsCommunityGirlAnimInstanceStateProxy::GetFirstMachine()
{
	if (GetAnimClassInterface())
	{
		for (FStructProperty* Property : GetAnimClassInterface()->GetAnimNodeProperties())
		{
			if (Property && Property->Struct->IsChildOf(FAnimNode_StateMachine::StaticStruct()))
			{
				return Property->ContainerPtrToValuePtr<FAnimNode_StateMachine>(GetAnimInstanceObject());
			}
		}
	}

	return nullptr;
}

void UGsCommunityGirlAnimInstanceState::NativeInitializeAnimation()
{
	UAnimInstance::NativeInitializeAnimation();
}

void UGsCommunityGirlAnimInstanceState::NativeUpdateAnimation(float DeltaSeconds)
{
	UAnimInstance::NativeUpdateAnimation(DeltaSeconds);

	_animOnly = CurrentStateType != EGsStateBase::Idle;

	if (_animOnly)
	{
		if (FAnimNode_StateMachine* stateMachine = _useProxy.GetFirstMachine())
		{
			if (auto desc = GetMachineDescription(_useProxy.GetAnimClassInterface(), stateMachine))
			{
				int stateIndex = stateMachine->GetCurrentState();
				if (stateIndex >= 0)
				{
					float remainTime = GetRelevantAnimTimeRemaining(GetStateMachineIndex(desc->MachineName), stateIndex);

					if (remainTime <= 0.f)
						EndState();
				}
				else EndState();
			}
		}
	}
	else
	{
		_specialState -= DeltaSeconds;
		if (_specialState <= 0)
		{
			SetIdleBreak();
		}
	}

	IsActiveMontage = GetCurrentActiveMontage() != nullptr;
}

FAnimInstanceProxy* UGsCommunityGirlAnimInstanceState::CreateAnimInstanceProxy()
{
	return &_useProxy;
}

void UGsCommunityGirlAnimInstanceState::EndState()
{
	ChangeState(static_cast<uint8>(EGsStateBase::Idle), 1, false);
	_specialState = 10.f;

	if (_cbStartAction) _cbStartAction(EGsStateBase::Idle);
}

void UGsCommunityGirlAnimInstanceState::SelectTabAction()
{
	ChangeState(static_cast<uint8>(EGsStateBase::Social), 1, false);

	if (_cbStartAction) _cbStartAction(EGsStateBase::Social);
}

void UGsCommunityGirlAnimInstanceState::TouchAction(int touchIndex, bool isRandom)
{
	ChangeState(static_cast<uint8>(EGsStateBase::SocialAction), touchIndex, isRandom);

	if (_cbStartAction) _cbStartAction(EGsStateBase::SocialAction);
}

void UGsCommunityGirlAnimInstanceState::SetIdleBreak()
{
	ChangeState(static_cast<uint8>(EGsStateBase::Freeze), 1, false);

	if (_cbStartAction) _cbStartAction(EGsStateBase::Freeze);
}