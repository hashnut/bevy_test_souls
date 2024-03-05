#include "GsCostumeAnimInstanceState.h"
#include "T1Project.h"

void FGsCostumeAnimInstanceStateProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	Super::PreUpdate(InAnimInstance, DeltaSeconds);

	if (UGsCostumeAnimInstanceState* castAnim = Cast<UGsCostumeAnimInstanceState>(InAnimInstance))
	{
		if (StateType != castAnim->CurrentStateType)
		{
			StateType = castAnim->CurrentStateType;
			ChangeStateAnimation(StateType);
		}
	}
}

void FGsCostumeAnimInstanceStateProxy::PostUpdate(UAnimInstance* InAnimInstance) const
{
	Super::PostUpdate(InAnimInstance);
}

void FGsCostumeAnimInstanceStateProxy::PostEvaluate(UAnimInstance* InAnimInstance)
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

void FGsCostumeAnimInstanceStateProxy::InitializeStateAnimation()
{
	IsSpawn = false;
	IsIdle = false;
	IsWearOn = false;
	IsWearChange = false;
}

void FGsCostumeAnimInstanceStateProxy::ChangeStateAnimation(EGsStateBase State)
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
		IsWearOn = true;
		break;
	case EGsStateBase::Freeze:
		IsWearChange = true;
		break;
	default:
		IsIdle = true;
		break;
	}
}

FAnimNode_StateMachine* FGsCostumeAnimInstanceStateProxy::GetFirstMachine()
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

void UGsCostumeAnimInstanceState::NativeInitializeAnimation()
{
	UAnimInstance::NativeInitializeAnimation();
}

void UGsCostumeAnimInstanceState::NativeUpdateAnimation(float DeltaSeconds)
{
	UAnimInstance::NativeUpdateAnimation(DeltaSeconds);

	if (_animOnly)
	{
		if (FAnimNode_StateMachine* stateMachine = _useProxy.GetFirstMachine())
		{
			if (auto desc = GetMachineDescription(_useProxy.GetAnimClassInterface(), stateMachine))
			{
				int stateIndex = stateMachine->GetCurrentState();
				float remainTime = GetRelevantAnimTimeRemaining(GetStateMachineIndex(desc->MachineName), stateIndex);

				if (remainTime <= 0.f)
					EndState();
			}
		}
	}

	IsActiveMontage = GetCurrentActiveMontage() != nullptr;
}

FAnimInstanceProxy* UGsCostumeAnimInstanceState::CreateAnimInstanceProxy()
{
	return &_useProxy;
}

void UGsCostumeAnimInstanceState::StartState()
{
	if (CurrentStateType == EGsStateBase::Social ||
		CurrentStateType == EGsStateBase::Freeze )
		_animOnly = true;
}

void UGsCostumeAnimInstanceState::EndState()
{
	_animOnly = false;
	ChangeState(static_cast<uint8>(EGsStateBase::Idle), 1, false);
}

void UGsCostumeAnimInstanceState::ChangeCostumeState()
{
	_animOnly = true;
	if (CurrentStateType == EGsStateBase::Social)
	{
		ChangeState(static_cast<uint8>(EGsStateBase::Freeze), 1, false);
	}
	else
	{
		ChangeState(static_cast<uint8>(EGsStateBase::Social), 1, false);
	}
}