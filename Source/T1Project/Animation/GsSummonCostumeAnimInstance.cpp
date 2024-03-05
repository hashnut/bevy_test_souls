#include "GsSummonCostumeAnimInstance.h"

#include "Engine/Classes/Animation/AnimNode_StateMachine.h"


void FGsSummonCostumeAnimInstanceStateProxy::PreUpdate(UAnimInstance* In_animInstance, float In_deltaSeconds)
{
	Super::PreUpdate(In_animInstance, In_deltaSeconds);

	if (UGsSummonCostumeAnimInstance* castAnim = Cast<UGsSummonCostumeAnimInstance>(In_animInstance))
	{
		if (StateType != castAnim->CurrentStateType)
		{
			StateType = castAnim->CurrentStateType;
			ChangeStateToBPAnimationState(StateType);
		}
	}

}

void FGsSummonCostumeAnimInstanceStateProxy::InitializeAnimationState()
{
	IsSpawn = false;
	IsIdle = false;
}

void FGsSummonCostumeAnimInstanceStateProxy::ChangeStateToBPAnimationState(EGsStateSummonCostume State)
{
	InitializeAnimationState();

	switch (State)
	{
	case EGsStateSummonCostume::Spawn:
		IsSpawn = true;
		break;
	case EGsStateSummonCostume::Idle:
		IsIdle = true;
		break;
	}
}

FAnimNode_StateMachine* FGsSummonCostumeAnimInstanceStateProxy::GetFirstMachine()
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

void UGsSummonCostumeAnimInstance::ChangeState(EGsStateSummonCostume State)
{
	if (CurrentStateType == State)
	{
		return;
	}
	CurrentStateType = State;


	_useAnimEnd = (EGsStateSummonCostume::Spawn == State) ? true : false;
	_playAnimTime = 0.0f;
}

FAnimInstanceProxy* UGsSummonCostumeAnimInstance::CreateAnimInstanceProxy()
{
	return &_proxy;
}


void UGsSummonCostumeAnimInstance::EndState(EGsStateSummonCostume State)
{
	_useAnimEnd = false;
	_playAnimTime = 0.0f;

	if (State != EGsStateSummonCostume::Idle)
		ChangeState(EGsStateSummonCostume::Idle);
}

void UGsSummonCostumeAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (_useAnimEnd)
	{
		if (_playAnimTime == 0.f && GetCurrentAnimLength() <= 0.f)
		{
			_playAnimTime = 0.2f;
		}

		_playAnimTime -= DeltaSeconds;

		if (IsCurrentAnimEnd())
			EndState(CurrentStateType);
	}
}

float UGsSummonCostumeAnimInstance::GetCurrentAnimLength()
{
	if (FAnimNode_StateMachine* stateMachine = _proxy.GetFirstMachine())
	{
		if (auto desc = GetMachineDescription(_proxy.GetAnimClassInterface(), stateMachine))
		{
			int stateIndex = stateMachine->GetCurrentState();
			return  GetRelevantAnimLength(GetStateMachineIndex(desc->MachineName), stateIndex);
		}
	}

	return 0.f;
}

bool UGsSummonCostumeAnimInstance::IsCurrentAnimEnd()
{
	if (_useAnimEnd)
	{
		if (FAnimNode_StateMachine* stateMachine = _proxy.GetFirstMachine())
		{
			if (auto desc = GetMachineDescription(_proxy.GetAnimClassInterface(), stateMachine))
			{
				int stateIndex = stateMachine->GetCurrentState();
				return  GetRelevantAnimTimeRemaining(GetStateMachineIndex(desc->MachineName), stateIndex) <= 0.f;
			}
		}
		return _playAnimTime <= 0.f;
	}

	return false;
}