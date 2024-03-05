// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/GsCompleteCampAnimInstance.h"
#include "Public/Animation/AnimInstanceProxy.h"
#include "Engine/Classes/Animation/AnimNode_StateMachine.h"
#include "UTIL/GsText.h"
#include "T1Project.h"


void FGsCompleteCampAnimInstanceStateProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	Super::PreUpdate(InAnimInstance, DeltaSeconds);

	if (UGsCompleteCampAnimInstance* castAnim = Cast<UGsCompleteCampAnimInstance>(InAnimInstance))
	{
		if (CompleteCampStateType != castAnim->CurrentStateType)
		{
			CompleteCampStateType = castAnim->CurrentStateType;
			SpawnTypeIndex = castAnim->_spawnTypeIndex;
			ChangeStateToBPAnimationState(CompleteCampStateType);
		}
	}
}

void FGsCompleteCampAnimInstanceStateProxy::PostEvaluate(UAnimInstance* InAnimInstance)
{
	Super::PostEvaluate(InAnimInstance);
}

void FGsCompleteCampAnimInstanceStateProxy::InitializeAnimationState()
{
	IsPreSpawn = false;
	IsSpawn = false;
	IsIdle = false;
	IsDamage = false;
	IsDestroy = false;
}

void FGsCompleteCampAnimInstanceStateProxy::ChangeStateToBPAnimationState(EGsStateCampComplete inState)
{
	InitializeAnimationState();

	//GSLOG(Warning, TEXT("[FGsCampAnimInstanceStateProxy::ChangeStateToBPAnimationState] - CurrentStep : %d"), CurrentStep);

	switch (inState)
	{
	case EGsStateCampComplete::PreSpawn_CompleteCamp:
		IsPreSpawn = true;
		break;
	case EGsStateCampComplete::Spawn_CompleteCamp:
		IsSpawn = true;
		break;
	case EGsStateCampComplete::Idle_CompleteCamp:
		IsIdle = true;
		break;
	case EGsStateCampComplete::Hit_CompleteCamp:
		IsDamage = true;
		break;
	case EGsStateCampComplete::Destroy_CompleteCamp:
		IsDestroy = true;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////


UGsCompleteCampAnimInstance::UGsCompleteCampAnimInstance()
{
}

UGsCompleteCampAnimInstance::~UGsCompleteCampAnimInstance()
{
}

void UGsCompleteCampAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

}

void UGsCompleteCampAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

FAnimInstanceProxy* UGsCompleteCampAnimInstance::CreateAnimInstanceProxy()
{
	return &_completeCampAnimStateProxy;
}

void UGsCompleteCampAnimInstance::ResetCurrentState(FName FsmName)
{
	FGsCompleteCampAnimInstanceStateProxy& proxy = GetProxyOnGameThread<FGsCompleteCampAnimInstanceStateProxy>();
	if (FAnimNode_StateMachine* machine = proxy.GetStateMachineInstanceFromName(FsmName))
	{
		FAnimationInitializeContext InitializationContext(&proxy);
		machine->Initialize_AnyThread(InitializationContext);
	}
}

void UGsCompleteCampAnimInstance::ChangeState(EGsStateCampComplete State)
{
	if (CurrentStateType == State)
		return;

	CurrentStateType = State;
}

void UGsCompleteCampAnimInstance::SetSpawnTypeIndex(int InNum)
{
	_spawnTypeIndex = InNum;
}

void UGsCompleteCampAnimInstance::EndState(EGsStateCampComplete State)
{
	
}
