// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/GsCampAnimInstance.h"
#include "Public/Animation/AnimInstanceProxy.h"
#include "Engine/Classes/Animation/AnimNode_StateMachine.h"
#include "UTIL/GsText.h"
#include "T1Project.h"


void FGsCampAnimInstanceStateProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	Super::PreUpdate(InAnimInstance, DeltaSeconds);

	if (UGsCampAnimInstance* castAnim = Cast<UGsCampAnimInstance>(InAnimInstance))
	{
		if (CampStateType != castAnim->CurrentStateType)
		{
			CampStateType = castAnim->CurrentStateType;
			CurrentStep = castAnim->_currentStep;
			ChangeStateToBPAnimationState(CampStateType);
		}
	}
}

void FGsCampAnimInstanceStateProxy::PostEvaluate(UAnimInstance* InAnimInstance)
{
	Super::PostEvaluate(InAnimInstance);
}

void FGsCampAnimInstanceStateProxy::InitializeAnimationState()
{
	IsIdle = false;
	IsDamage = false;
	IsDestroy = false;
	IsBuilding = false;
	IsCompleteBuild = false;
}

void FGsCampAnimInstanceStateProxy::ChangeStateToBPAnimationState(EGsStateCamp inState)
{
	InitializeAnimationState();

	//GSLOG(Warning, TEXT("[FGsCampAnimInstanceStateProxy::ChangeStateToBPAnimationState] - CurrentStep : %d"), CurrentStep);

	switch (inState)
	{
	case EGsStateCamp::Idle_Camp:
		IsIdle = true;
		break;
	case EGsStateCamp::Hit_Camp:
		IsDamage = true;
		break;
	case EGsStateCamp::Destroy_Camp:
		IsDestroy = true;
		break;
	case EGsStateCamp::Building_Camp:
		IsBuilding = true;
		break;
	case EGsStateCamp::BuildingComplete:
		IsCompleteBuild = true;
		break;
	}
}

UGsCampAnimInstance::UGsCampAnimInstance()
{
}

UGsCampAnimInstance::~UGsCampAnimInstance()
{
}

void UGsCampAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	_isAllowedBuildCampFlag = false;
}

void UGsCampAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

FAnimInstanceProxy* UGsCampAnimInstance::CreateAnimInstanceProxy()
{
	return &_campAnimStateProxy;
}

void UGsCampAnimInstance::ResetCurrentState(FName FsmName)
{
	FGsCampAnimInstanceStateProxy& proxy = GetProxyOnGameThread<FGsCampAnimInstanceStateProxy>();
	if (FAnimNode_StateMachine* machine = proxy.GetStateMachineInstanceFromName(FsmName))
	{
		FAnimationInitializeContext InitializationContext(&proxy);
		machine->Initialize_AnyThread(InitializationContext);
	}
}

void UGsCampAnimInstance::ChangeState(EGsStateCamp State)
{
	if (CurrentStateType == State)
		return;

	CurrentStateType = State;
}

void UGsCampAnimInstance::SetCampCurrentStep(int InStepNum)
{
	_currentStep = InStepNum;
}

void UGsCampAnimInstance::EndState(EGsStateCamp State)
{
	//GSLOG(Warning, TEXT("[UGsCampAnimInstance::EndState] !!!"));
}

void UGsCampAnimInstance::ImpossibleBuildCamp()
{
	_isAllowedBuildCampFlag = true;
	//GSLOG(Warning, TEXT("[UGsCampAnimInstance::ImpossibleBuildCamp] !!!!!!! "));
}

void UGsCampAnimInstance::PossibleBuildCamp()
{
	_isAllowedBuildCampFlag = false;
	//GSLOG(Warning, TEXT("[UGsCampAnimInstance::PossibleBuildCamp] ~~~~~~~~ "));
}

bool UGsCampAnimInstance::IsAllowedBuildCamp()
{
	return _isAllowedBuildCampFlag;
}