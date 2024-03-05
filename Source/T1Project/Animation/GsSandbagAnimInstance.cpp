// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSandbagAnimInstance.h"
#include "Public/Animation/AnimInstanceProxy.h"
#include "Engine/Classes/Animation/AnimNode_StateMachine.h"
#include "UTIL/GsText.h"
#include "T1Project.h"
#include "GameObject/Define/GsGameObjectDefine.h"

void FGsSandbagAnimInstanceStateProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	Super::PreUpdate(InAnimInstance, DeltaSeconds);

	if (UGsSandbagAnimInstance* castAnim = Cast<UGsSandbagAnimInstance>(InAnimInstance))
	{
		if (StateType != castAnim->CurrentStateType)
		{
			StateType = castAnim->CurrentStateType;
			ChangeStateToBPAnimationState(StateType);
		}
	}
}

void FGsSandbagAnimInstanceStateProxy::PostEvaluate(UAnimInstance* InAnimInstance)
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

void FGsSandbagAnimInstanceStateProxy::InitializeAnimationState()
{
	IsSpawn = false;	
	IsIdle = false;
	IsHit = false;
	IsDying = false;
	IsDead = false;
}

void FGsSandbagAnimInstanceStateProxy::ChangeStateToBPAnimationState(EGsStateBase inState)
{
	InitializeAnimationState();

#if false
	FString stateName = FGsTextUtil::GetStringFromEnum(TEXT("EGsStateProp"), inState);
	GSLOG(Log, TEXT("FGsPropAnimInstanceStateProxy - ChangeStateToBPAnimationState : %s"), *stateName);
#endif

	switch (inState)
	{
	case EGsStateBase::Spawn:
		IsSpawn = true;
		break;
	case EGsStateBase::Idle:
		IsIdle = true;
		break;
	case EGsStateBase::Hit:
		IsHit = true;
		break;
	case EGsStateBase::Dying:
		IsDying = true;
		break;
	case EGsStateBase::Dead:
		IsDead = true;
		break;
	}
}


UGsSandbagAnimInstance::UGsSandbagAnimInstance()
{
}

UGsSandbagAnimInstance::~UGsSandbagAnimInstance()
{
}

void UGsSandbagAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UGsSandbagAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	IsActiveMontage = GetCurrentActiveMontage() != nullptr;

	//if (CurrentStateType != EGsStateFairy::None && !IsAnyMontagePlaying())
	//	EndState(CurrentStateType);
}


FAnimInstanceProxy* UGsSandbagAnimInstance::CreateAnimInstanceProxy()
{
	return &_sandbagProxy;
}

void UGsSandbagAnimInstance::ResetCurrentState(FName FsmName)
{
	FGsSandbagAnimInstanceStateProxy& proxy = GetProxyOnGameThread<FGsSandbagAnimInstanceStateProxy>();
	if (FAnimNode_StateMachine* machine = proxy.GetStateMachineInstanceFromName(FsmName))
	{
		FAnimationInitializeContext InitializationContext(&proxy);
		machine->Initialize_AnyThread(InitializationContext);
	}
}

void UGsSandbagAnimInstance::ChangeState(EGsStateBase State)
{
	if (CurrentStateType == State)
		return;

	CurrentStateType = State;
}

void UGsSandbagAnimInstance::EndState(EGsStateBase State)
{
	if (State != EGsStateBase::Idle
		&& State != EGsStateBase::Dead
		&& State != EGsStateBase::Hit)
		ChangeState(EGsStateBase::Idle);
}