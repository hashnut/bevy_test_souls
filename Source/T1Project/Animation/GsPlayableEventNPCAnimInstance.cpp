#include "GsPlayableEventNPCAnimInstance.h"

#include "Engine/Classes/Animation/AnimNode_StateMachine.h"

void FGsPlayableEventNPCAnimInstanceStateProxy::PreUpdate(UAnimInstance* In_animInstance, float In_deltaSeconds)
{
	Super::PreUpdate(In_animInstance, In_deltaSeconds);

	if (UGsPlayableEventNPCAnimInstance* castAnim = Cast<UGsPlayableEventNPCAnimInstance>(In_animInstance))
	{
		if (StateType != castAnim->CurrentStateType)
		{
			StateType = castAnim->CurrentStateType;
			ChangeStateToBPAnimationState(StateType);
		}
	}

}

void FGsPlayableEventNPCAnimInstanceStateProxy::InitializeAnimationState()
{
	IsIdle = false;
	IsSpawn = false;	
	IsCastingLooping = false;
	IsAttackLooping = false;
	IsAction = false;
	IsSkill = false;
	IsDamaged = false;
}

void FGsPlayableEventNPCAnimInstanceStateProxy::ChangeStateToBPAnimationState(EGsStatePlayableEventNPC State)
{
	InitializeAnimationState();

	switch (State)
	{
	case EGsStatePlayableEventNPC::Idle:
		IsIdle = true;
		break;
	case EGsStatePlayableEventNPC::Spawn:
		IsSpawn = true;
		break;
	case EGsStatePlayableEventNPC::CastingLooping:
		IsCastingLooping = true;
		break;
	case EGsStatePlayableEventNPC::AttackLooping:
		IsAttackLooping = true;
		break;
	case EGsStatePlayableEventNPC::Action:
		IsAction = true;
		break;
	case EGsStatePlayableEventNPC::Skill:
		IsSkill = true;
		break;
	case EGsStatePlayableEventNPC::Damaged:
		IsDamaged = true;
		break;
	}
}

FAnimNode_StateMachine* FGsPlayableEventNPCAnimInstanceStateProxy::GetFirstMachine()
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

void UGsPlayableEventNPCAnimInstance::ChangeState(EGsStatePlayableEventNPC State)
{
	if (CurrentStateType == State)
	{
		return;
	}
	CurrentStateType = State;

	_useAnimEnd = false;

	switch (State)
	{
	case EGsStatePlayableEventNPC::Skill:
	case EGsStatePlayableEventNPC::Spawn:
	case EGsStatePlayableEventNPC::Action:
	{
		_useAnimEnd = true;
	}
	break;
	}

	_playAnimTime = 0.0f;
}

FAnimInstanceProxy* UGsPlayableEventNPCAnimInstance::CreateAnimInstanceProxy()
{
	return &_proxy;
}

void UGsPlayableEventNPCAnimInstance::EndState(EGsStatePlayableEventNPC State)
{
	_useAnimEnd = false;
	_playAnimTime = 0.0f;


	EGsStatePlayableEventNPC newState = EGsStatePlayableEventNPC::None;
	switch (State)
	{
	case EGsStatePlayableEventNPC::Skill:
		newState = EGsStatePlayableEventNPC::Idle;
		break;
	case EGsStatePlayableEventNPC::Spawn:
		newState = EGsStatePlayableEventNPC::AttackLooping;
		break;
	case EGsStatePlayableEventNPC::Action:
		newState = EGsStatePlayableEventNPC::AttackLooping;
		break;
	}

	if (newState != EGsStatePlayableEventNPC::None)
	{
		ChangeState(newState);
	}
}

void UGsPlayableEventNPCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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

float UGsPlayableEventNPCAnimInstance::GetCurrentAnimLength()
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

bool UGsPlayableEventNPCAnimInstance::IsCurrentAnimEnd()
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