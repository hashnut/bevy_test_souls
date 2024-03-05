// Fill out your copyright notice in the Description page of Project Settings.


#include "GsFairyAnimInstance.h"
#include "Public/Animation/AnimInstanceProxy.h"
#include "Engine/Classes/Animation/AnimNode_StateMachine.h"
#include "Data/GsResourceManager.h"

void FGsFairyAnimInstanceStateProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	Super::PreUpdate(InAnimInstance, DeltaSeconds);

	if (UGsFairyAnimInstance* castAnim = Cast<UGsFairyAnimInstance>(InAnimInstance))
	{
		if (StateType != castAnim->CurrentStateType)
		{
			StateType = castAnim->CurrentStateType;
			IdleIndex = castAnim->IsWaitingState() ? 1 : 0;
			ChangeStateToBPAnimationState(StateType);
		}
	}
}

void FGsFairyAnimInstanceStateProxy::PostEvaluate(UAnimInstance* InAnimInstance)
{
	Super::PostEvaluate(InAnimInstance);
}

void FGsFairyAnimInstanceStateProxy::InitializeAnimationState()
{
	IsSpawn = false;
	IsUseSkill = false;
	IsIdle = false;
	IsDespawn = false;
	IsSpecial = false;
	IsMove = false;
}

void FGsFairyAnimInstanceStateProxy::ChangeStateToBPAnimationState(EGsStateFairy State)
{
	InitializeAnimationState();

	switch (State)
	{
	case EGsStateFairy::Spawn:
		IsSpawn = true;
		break;
	case EGsStateFairy::Idle:
		IsIdle = true;
		break;
	case EGsStateFairy::Despawn:
		IsDespawn = true;
		break;
	case EGsStateFairy::Skill:
		IsUseSkill = true;
		break;
	case EGsStateFairy::Move:
		IsMove = true;
		break;
	case EGsStateFairy::Special:
		IsSpecial = true;
		break;
	case EGsStateFairy::Destroy:
		IsDestroy = true;
		break;
	}
}

FAnimNode_StateMachine* FGsFairyAnimInstanceStateProxy::GetFirstMachine()
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

UGsFairyAnimInstance::UGsFairyAnimInstance()
{
}

UGsFairyAnimInstance::~UGsFairyAnimInstance()
{
}

void UGsFairyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UGsFairyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (_restartState != EGsStateFairy::None)
	{
		FGsFairyAnimInstanceStateProxy& proxy = GetProxyOnGameThread<FGsFairyAnimInstanceStateProxy>();
		if (proxy.StateType == CurrentStateType)
		{
			ChangeState(_restartState);
			_restartState = EGsStateFairy::None;
		}
	}

	if (_useAnimEnd)
	{
		if (_playAnimTime == 0.f && GetCurrentAnimLength() <= 0.f)
		{	
			_playAnimTime = 0.02f;
		}
		
		_playAnimTime -= DeltaSeconds;

		if(IsCurrentAnimEnd())
			EndState(CurrentStateType);
	}
}


FAnimInstanceProxy* UGsFairyAnimInstance::CreateAnimInstanceProxy()
{
	return &_proxy;
}

void UGsFairyAnimInstance::ResetCurrentState(FName FsmName)
{
	FGsFairyAnimInstanceStateProxy& proxy = GetProxyOnGameThread<FGsFairyAnimInstanceStateProxy>();
	if (FAnimNode_StateMachine* machine = proxy.GetStateMachineInstanceFromName(FsmName))
	{
		FAnimationInitializeContext InitializationContext(&proxy);
		machine->Initialize_AnyThread(InitializationContext);
	}
}

void UGsFairyAnimInstance::ChangeState(EGsStateFairy State, bool loopState, TFunction<void(EGsStateFairy)> cbAnimEnd)
{
	if (CurrentStateType == EGsStateFairy::Destroy)
		return;

	_useAnimEnd = loopState ? false :
		State != EGsStateFairy::Idle && State != EGsStateFairy::Move;
	_playAnimTime = 0.f;
	_cbAnimEnd = cbAnimEnd;

	if (_useAnimEnd && CurrentStateType == State)
	{
		_restartState = State;
		EndState(CurrentStateType);
		return;
	}

	CurrentStateType = State;
}

void UGsFairyAnimInstance::EndState(EGsStateFairy State)
{
	if (_cbAnimEnd) _cbAnimEnd(State);

	_cbAnimEnd = nullptr;
	_useAnimEnd = false;
	_playAnimTime = 0.f;

	if (State != EGsStateFairy::Destroy && State != EGsStateFairy::Despawn)
	{
		if(_isMoveState)
			 ChangeState(EGsStateFairy::Move);
		else ChangeState(EGsStateFairy::Idle);
	}
}

float UGsFairyAnimInstance::GetCurrentAnimLength()
{
	if (FAnimNode_StateMachine* stateMachine = _proxy.GetFirstMachine())
	{
		if (auto desc = GetMachineDescription(_proxy.GetAnimClassInterface(), stateMachine))
		{
			int stateIndex = stateMachine->GetCurrentState();
			float animLength = GetRelevantAnimLength(GetStateMachineIndex(desc->MachineName), stateIndex);

			if (animLength != MAX_flt) return animLength;
		}
	}

	return 0.f;
}

bool UGsFairyAnimInstance::IsCurrentAnimEnd()
{
	if (_useAnimEnd)
	{
		if (FAnimNode_StateMachine* stateMachine = _proxy.GetFirstMachine())
		{
			if (auto desc = GetMachineDescription(_proxy.GetAnimClassInterface(), stateMachine))
			{
				int stateIndex = stateMachine->GetCurrentState();

				float animLength = GetRelevantAnimTimeRemaining(GetStateMachineIndex(desc->MachineName), stateIndex);
				if(animLength != MAX_flt) return animLength <= 0.f;
			}
		}
		return _playAnimTime <= 0.f;
	}

	return false;
}

void UGsFairyAnimInstance::SetMoveState(bool isMove) 
{ 
	if (_isMoveState != isMove)
	{
		_isMoveState = isMove;
		if (!_useAnimEnd)
		{
			ChangeState(_isMoveState ? EGsStateFairy::Move : EGsStateFairy::Idle);
		}
	}
}

void UGsFairyAnimInstance::SetWaitingState(bool isWaiting)
{
	if (_isWaitingState != isWaiting)
	{
		_isWaitingState = isWaiting;
		if (!_useAnimEnd)
		{
			ChangeState(EGsStateFairy::Idle);
		}
	}
}

void UGsFairyAnimInstance::SetVehicleState(bool isVehicle)
{
	if (_isVehicleState != isVehicle)
	{
		_isVehicleState = isVehicle;
	}
}
	