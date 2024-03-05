// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/GsLobbyAnimInstanceState.h"
#include "T1Project.h"
#include "Animation/GsAnimInstanceState.h"
#include "Engine/Classes/Animation/AnimSequenceBase.h"
#include "Engine/Classes/Animation/AnimNode_AssetPlayerBase.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

void FGsLobbyAnimInstanceStateProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	Super::PreUpdate(InAnimInstance, DeltaSeconds);

	if (UGsLobbyAnimInstanceState* castAnim = Cast<UGsLobbyAnimInstanceState>(InAnimInstance))
	{
		if (StateType != castAnim->CurrentStateType)
		{
			StateType = castAnim->CurrentStateType;
			ChangeStateAnimation(StateType);			
		}
		RandomIndex = castAnim->AnimationBlendIndex;
	}
}

void FGsLobbyAnimInstanceStateProxy::PostUpdate(UAnimInstance* InAnimInstance) const
{
	Super::PostUpdate(InAnimInstance);	
}

void FGsLobbyAnimInstanceStateProxy::PostEvaluate(UAnimInstance* InAnimInstance)
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

	if (bCalcSkeletalMeshResize)
	{
		if (UGsLobbyAnimInstanceState* anim = Cast<UGsLobbyAnimInstanceState>(InAnimInstance))
		{
			anim->ReCalcSkeletalMeshRelativeLocation();
		}
		bCalcSkeletalMeshResize = false;
	}
}

void FGsLobbyAnimInstanceStateProxy::InitializeStateAnimation()
{
	IsSpawn = false;
	IsIdle = false;
	IsSelecSlot = false;
	IsGameEnter = false;

	RandomIndex = 0;
}

void FGsLobbyAnimInstanceStateProxy::CacheBones()
{
	if (bBoneCachesInvalidated)
	{
		bCalcSkeletalMeshResize = true;
	}

	Super::CacheBones();
}

void FGsLobbyAnimInstanceStateProxy::ChangeStateAnimation(EGsStateBase State)
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
		IsSelecSlot = true;
		break;
	case EGsStateBase::Warp:
		IsGameEnter = true;
		break;
	default:
		IsIdle = true;
		break;
	}
}

FAnimNode_StateMachine* FGsLobbyAnimInstanceStateProxy::GetFirstMachine()
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

void UGsLobbyAnimInstanceState::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UGsLobbyAnimInstanceState::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	_animOnly = CurrentStateType != EGsStateBase::Idle;

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

FAnimInstanceProxy* UGsLobbyAnimInstanceState::CreateAnimInstanceProxy()
{
	return &_useProxy;
}
FAnimInstanceProxy* UGsLobbyAnimInstanceState::GetAnimInstanceProxy()
{
	return &_useProxy;
}
void UGsLobbyAnimInstanceState::EndState()
{
	ChangeState(static_cast<uint8>(EGsStateBase::Idle), AnimationBlendIndex, false);
}

void UGsLobbyAnimInstanceState::ReCalcSkeletalMeshRelativeLocation()
{
	// Base 클래스에서 상태로 조건 구분 분기가 있어 이클래스에서 상위 호출을 하지 않고 따로 구현
	if (USkeletalMeshComponent* skeletalMeshComp = GetSkelMeshComponent())
	{
		if (skeletalMeshComp->ModifyBoneSpaceTransforms.Num() > 0)
		{
			const TArray<FTransform>& refEditableBoneTm = skeletalMeshComp->GetEditableComponentSpaceTransforms();
			// const TArray<FTransform>& refCurrentBoneTm = skeletalMeshComp->GetComponentSpaceTransforms();
			int32 BoneIndex = skeletalMeshComp->GetBoneIndex(FName("Bip001-L-Toe0"));
			if (BoneIndex == INDEX_NONE)
			{
				return;
			}

			if (ACharacter* characterActor = Cast<ACharacter>(skeletalMeshComp->GetOwner()))
			{
				UCapsuleComponent* capsule = characterActor->GetCapsuleComponent();
				float halfheight = capsule->GetScaledCapsuleHalfHeight();
				FVector vecSpaceBot = refEditableBoneTm[BoneIndex].GetLocation();

				skeletalMeshComp->SetRelativeLocation(FVector(0.f, 0.f, -(halfheight + vecSpaceBot.Z)));
			}
		}
	}
}

void UGsLobbyAnimInstanceState::OnGameEnterAction()
{
	ChangeState(static_cast<uint8>(EGsStateBase::Warp), AnimationBlendIndex, false);
}

void UGsLobbyAnimInstanceState::OnChangeSelectSlotAction()
{
	ChangeState(static_cast<uint8>(EGsStateBase::Social), AnimationBlendIndex, false);
}

UAnimSequenceBase* UGsLobbyAnimInstanceState::GetAnimSequence(FName StateName, FName In_assetName)
{
	UAnimSequenceBase* animSeqBase = nullptr;
	int idx = _useProxy.GetInstanceAssetPlayerIndex(StateMachineName, StateName, In_assetName);
	if (FAnimNode_AssetPlayerBase* playerNode = _useProxy.GetNodeFromIndex<FAnimNode_AssetPlayerBase>(idx))
	{
		animSeqBase = Cast<UAnimSequenceBase>(playerNode->GetAnimAsset());
	}
	return animSeqBase;
}