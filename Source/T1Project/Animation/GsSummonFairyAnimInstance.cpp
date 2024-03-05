#include "GsSummonFairyAnimInstance.h"

#include "Engine/Classes/Animation/AnimNode_StateMachine.h"


void FGsSummonFairyAnimInstanceStateProxy::PreUpdate(UAnimInstance* In_animInstance, float In_deltaSeconds)
{
	Super::PreUpdate(In_animInstance, In_deltaSeconds);

	if (UGsSummonFairyAnimInstance* castAnim = Cast<UGsSummonFairyAnimInstance>(In_animInstance))
	{
		if (StateType != castAnim->CurrentStateType)
		{
			StateType = castAnim->CurrentStateType;
			ChangeStateToBPAnimationState(StateType);
		}
	}

}

void FGsSummonFairyAnimInstanceStateProxy::InitializeAnimationState()
{
	IsSpawn = false;
	IsIdle = false;
	IsAppear = false;
}

void FGsSummonFairyAnimInstanceStateProxy::ChangeStateToBPAnimationState(EGsStateSummonFairy State)
{
	InitializeAnimationState();

	switch (State)
	{
	case EGsStateSummonFairy::Spawn:
		IsSpawn = true;
		break;
	case EGsStateSummonFairy::Idle:
		IsIdle = true;
		break;
	case EGsStateSummonFairy::Appear:
		IsAppear = true;
		break;
	}
}

FAnimNode_StateMachine* FGsSummonFairyAnimInstanceStateProxy::GetFirstMachine()
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

void UGsSummonFairyAnimInstance::ChangeState(EGsStateSummonFairy State)
{
	if (CurrentStateType == State)
	{
		return;
	}
	CurrentStateType = State;


	_useAnimEnd = (EGsStateSummonFairy::Spawn == State)? true: false;
	_playAnimTime = 0.0f;
}

FAnimInstanceProxy* UGsSummonFairyAnimInstance::CreateAnimInstanceProxy()
{
	return &_proxy;
}


void UGsSummonFairyAnimInstance::EndState(EGsStateSummonFairy State)
{
	_useAnimEnd = false;
	_playAnimTime = 0.0f;

	if (State != EGsStateSummonFairy::Idle)
		ChangeState(EGsStateSummonFairy::Idle);
}

void UGsSummonFairyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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

float UGsSummonFairyAnimInstance::GetCurrentAnimLength()
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

bool UGsSummonFairyAnimInstance::IsCurrentAnimEnd()
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