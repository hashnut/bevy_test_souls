// Fill out your copyright notice in the Description page of Project Settings.


#include "GsPropAnimInstance.h"
#include "Public/Animation/AnimInstanceProxy.h"
#include "Engine/Classes/Animation/AnimNode_StateMachine.h"
#include "UTIL/GsText.h"
#include "T1Project.h"

void FGsPropAnimInstanceStateProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	Super::PreUpdate(InAnimInstance, DeltaSeconds);

	if (UGsPropAnimInstance* castAnim = Cast<UGsPropAnimInstance>(InAnimInstance))
	{
		if (StateType != castAnim->CurrentStateType)
		{
			StateType = castAnim->CurrentStateType;
			ChangeStateToBPAnimationState(StateType);
		}
	}
}

void FGsPropAnimInstanceStateProxy::PostEvaluate(UAnimInstance* InAnimInstance)
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

void FGsPropAnimInstanceStateProxy::InitializeAnimationState()
{
	IsSpawn = false;
	IsAction = false;
	IsIdle = false;
	IsFinish = false;
}

void FGsPropAnimInstanceStateProxy::ChangeStateToBPAnimationState(EGsStateProp inState)
{
	InitializeAnimationState();

#if false
	FString stateName = FGsTextUtil::GetStringFromEnum(TEXT("EGsStateProp"), inState);
	GSLOG(Log, TEXT("FGsPropAnimInstanceStateProxy - ChangeStateToBPAnimationState : %s"), *stateName);
#endif

	switch (inState)
	{
	case EGsStateProp::Spawn:
		IsSpawn = true;
		break;
	case EGsStateProp::Idle:
		IsIdle = true;
		break;
	case EGsStateProp::Finish:
		IsFinish = true;
		break;
	case EGsStateProp::Action:
		IsAction = true;
		break;
	}
}


UGsPropAnimInstance::UGsPropAnimInstance()
{
}

UGsPropAnimInstance::~UGsPropAnimInstance()
{
}

void UGsPropAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UGsPropAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	IsActiveMontage = GetCurrentActiveMontage() != nullptr;

	//if (CurrentStateType != EGsStateFairy::None && !IsAnyMontagePlaying())
	//	EndState(CurrentStateType);
}


FAnimInstanceProxy* UGsPropAnimInstance::CreateAnimInstanceProxy()
{
	return &_proxy;
}

void UGsPropAnimInstance::ResetCurrentState(FName FsmName)
{
	FGsPropAnimInstanceStateProxy& proxy = GetProxyOnGameThread<FGsPropAnimInstanceStateProxy>();
	if (FAnimNode_StateMachine* machine = proxy.GetStateMachineInstanceFromName(FsmName))
	{
		FAnimationInitializeContext InitializationContext(&proxy);
		machine->Initialize_AnyThread(InitializationContext);
	}
}

void UGsPropAnimInstance::ChangeState(EGsStateProp State)
{
	if (CurrentStateType == State) return;
	CurrentStateType = State;
}

void UGsPropAnimInstance::EndState(EGsStateProp State)
{
	if (State != EGsStateProp::Idle && State != EGsStateProp::Finish)
		ChangeState(EGsStateProp::Idle);
}