// Fill out your copyright notice in the Description page of Project Settings.

#include "GsAnimInstanceState.h"
#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimNode_StateMachine.h"
#include "Animation/AnimNotifies/AnimNotify_PlayParticleEffect.h"
#include "Animation/AnimNotifies/AnimNotify_PlaySound.h"
#include "Animation/AnimNode_AssetPlayerBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "ActorEx/GsCharacterBase.h"
#include "ActorComponentEx/GsEffectComponent.h"
#include "AnimNotify/AnimNotify_PlayParticleManaged.h"
#include "AnimNotify/AnimNotifyState_MoveToTarget.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimCompositeBase.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Classes/Particles/WorldPSCPool.h"
#include "../T1Project.h"

void FGsAnimInstanceStateProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	Super::PreUpdate(InAnimInstance, DeltaSeconds);

	if (UGsAnimInstanceState* castAnim = Cast<UGsAnimInstanceState>(InAnimInstance))
	{
		IsBattleMode = castAnim->IsBattleMode;
		IsSocialMode = castAnim->IsSocialMode;
		IsAbnormalityMode = castAnim->IsAbnormalityMode;

		if (StateType != castAnim->CurrentStateType)
		{
			StateType = castAnim->CurrentStateType;
			ChnageStateToBPAnimationFastPath(StateType);
			RandomIndex = castAnim->AnimationBlendIndex;
		}
	}

	/*if (nullptr == _firstMachine)
	{
		_firstMachine = GetFirstMachine();
	}

	if (_firstMachine)
	{

	}*/
}

void FGsAnimInstanceStateProxy::Update(float DeltaSeconds)
{
}

void FGsAnimInstanceStateProxy::PostUpdate(UAnimInstance* InAnimInstance) const
{
	Super::PostUpdate(InAnimInstance);
}

void FGsAnimInstanceStateProxy::PostEvaluate(UAnimInstance* InAnimInstance)
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

void FGsAnimInstanceStateProxy::CacheBones()
{
	if (bBoneCachesInvalidated)
	{
		IsMaxLodIndex = false;
		if (USkeletalMeshComponent* skeletalMesh = GetSkelMeshComponent())
		{
			int maxLodIndex = skeletalMesh->GetNumLODs() - 1;
			IsMaxLodIndex = (GetLODLevel() == maxLodIndex);
		}
	}

	Super::CacheBones();
}

FAnimNode_StateMachine* FGsAnimInstanceStateProxy::GetFirstMachine()
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

void FGsAnimInstanceStateProxy::InitializeAnimationFastPath()
{	
	IsSpawn			= false;
	IsIdle			= false;
	IsBasicMoving	= false;
	IsMoving		= false;
	IsDie			= false;
	IsRoar			= false;
	IsRevive		= false;
	IsFreeze		= false;
	IsSocial		= false;
	IsHit			= false;
	IsRunAway		= false;

	RandomIndex		= 0;
}

void FGsAnimInstanceStateProxy::ChnageStateToBPAnimationFastPath(EGsStateBase State)
{
	InitializeAnimationFastPath();

	switch (State)
	{
	case EGsStateBase::Spawn:
		IsSpawn = true;
		break;
	case EGsStateBase::Idle:
		IsIdle = true;
		break;
	case EGsStateBase::ForwardWalk:
	case EGsStateBase::Run:
		IsBasicMoving = true;
	case EGsStateBase::AutoMove:
		IsMoving = true;
		break;
	case EGsStateBase::Dying:
		IsDie = true;
		break;
	case EGsStateBase::Roar:
		IsRoar = true;
		break;
	case  EGsStateBase::Revive:
		IsRevive = true;
		break;
	case EGsStateBase::Freeze:
		IsFreeze = true;
		break;
	case EGsStateBase::Social:
		IsSocial = true;
		break;
	case EGsStateBase::Hit:
		IsHit = true;
		break;
	case EGsStateBase::RunAway:
		IsRunAway = true;
		break;
	}
}

UGsAnimInstanceState::UGsAnimInstanceState()
{
	OnMontageStarted.AddDynamic(this, &UGsAnimInstanceState::CallbakMontageStarted);

	// 루트 모션 처리는 엔진 에서 계산하는 로직을 사용하지 않고 자체 처리
	// @see : void FGsMovementHandlerBase::Update(float inDelta)
	SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
}

UGsAnimInstanceState::~UGsAnimInstanceState()
{
}

void UGsAnimInstanceState::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 스테이트 머신 찾기
	if (const FBakedAnimationStateMachine* fsm = GetStateMachineInstanceDesc(StateMachineName))
	{
		// 각 스테이트 찾기
		for (const FBakedAnimationState& el : fsm->States)
		{
			// State Exit 델리게이트 등록
			AddNativeStateExitBinding(StateMachineName, el.StateName, 
				FOnGraphStateChanged::CreateUObject(this, &UGsAnimInstanceState::CallbakStateChangedExit));
		}
	}
}

void UGsAnimInstanceState::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// Proxy 에서 처리할경우 애니메이션 전환시 튀는 현상이 있다.
	// 여기서 처리했을때 멀티쓰레드 옵션에 Safe한지는 검증이 필요함
	// @see : GetCurrentActiveMontage()
	IsActiveMontage = GetCurrentActiveMontage() != nullptr;	
}

float UGsAnimInstanceState::GetStateAnimationLength(FName FsmName, FName StateName)
{
	int idx = GetInstanceAssetPlayerIndex(FsmName, StateName);
	return GetInstanceAssetPlayerLength(idx);
}

void UGsAnimInstanceState::InitSkeletalmeshComponent()
{
}

void UGsAnimInstanceState::InitMoveSpeedValue(int walkSpeed, int runSpeed, int vehicleSpeed)
{
	_walkSpeed = walkSpeed;
	_runSpeed = runSpeed;
	_vehicleSpeed = vehicleSpeed;
}

void UGsAnimInstanceState::ChangeState(uint8 State, int Index, bool IsRandom)
{
	CurrentStateType = static_cast<EGsStateBase>(State);

	SetPhysicsAnimation(true);

	// 기본슬롯으로 되돌림
	SlotMontagePlay = 0;	

	if (IsRandom)
	{
		AnimationBlendIndex = (Index > 1) ? FMath::RandRange(0, Index - 1) : 0;
	}
	else
	{
		AnimationBlendIndex = Index;
	}
}

void UGsAnimInstanceState::SetIdleType(EGsAnimStateIdleType Type)
{
	IsBattleMode = false;
	IsSocialMode = false;
	IsAbnormalityMode = false;
	switch (Type)
	{
	case EGsAnimStateIdleType::Battle:
		IsBattleMode = true;
		break;

	case EGsAnimStateIdleType::Social:		
		IsSocialMode = true;
		break;
	case EGsAnimStateIdleType::Abnormality:
		IsAbnormalityMode = true;
		break;
	}
}

void UGsAnimInstanceState::SetBattleIdleType(bool IsBattle)
{
	SetIdleType(IsBattle ? EGsAnimStateIdleType::Battle : EGsAnimStateIdleType::Normal);
}

FAnimInstanceProxy* UGsAnimInstanceState::CreateAnimInstanceProxy()
{
	return &_proxy;
	//return new FGsAnimInstanceStateProxy(this);
}

bool UGsAnimInstanceState::IsState(EGsStateBase State) const
{
	return CurrentStateType == State;
	//return StateType == State;
}

bool UGsAnimInstanceState::IsStates(const TArray<EGsStateBase>& States) const
{
	//개선 필요
	for (const EGsStateBase el : States)
	{
		if (el == CurrentStateType)
		{
			return true;
		}
	}
	return false;

	/*return States.Contains([this](const EGsStateBase& el)
	{
		return IsState(el);
	});
	return false;*/
}

float UGsAnimInstanceState::GetCurrentBlendSpaceAngle() const
{
	return BlendSpaceAngle;
}

//void UGsAnimInstanceState::ChangeState(uint8 State, int Min, int Max)
//{
//	EGsStateBase changeState = static_cast<EGsStateBase>(State);
//	FGsAnimInstanceStateProxy& proxy = GetProxyOnGameThread<FGsAnimInstanceStateProxy>();
//	if (proxy._changeStateType != changeState)
//	{
//		Timer = FApp::GetCurrentTime();
//	}
//
//	proxy._changeStateType = changeState;
//
//	//랜덤 인덱스 생성 (테스트 용)
//	if (Max - Min > 0)
//	{
//		RandomIndex = FMath::RandRange(Min, Max);
//	}
//	else
//	{
//		RandomIndex = 0;
//	}
//}
void UGsAnimInstanceState::SetMoveSpeed(MotionType motionType)
{
	//MoveSpeed = motionType;
	switch (motionType)
	{
	//case MotionType::STOP:
	//	MoveSpeed = 0;
	//	break;
	case MotionType::WALK:
		SetMoveSpeed(_walkSpeed);
		break;
	case MotionType::NORMAL:
		SetMoveSpeed(_runSpeed);
		break;	
	case MotionType::VEHICLE:
		SetMoveSpeed(_vehicleSpeed);
		break;
	default:
		SetMoveSpeed(500);
		break;
	}
}

void UGsAnimInstanceState::SetMoveSpeed(float Speed)
{
	MoveSpeed = Speed;
}

void UGsAnimInstanceState::SetBlendSpaceAngle(float Angle)
{
	IsBlendSpaceAngleStart = true;
	BlendSpaceAngle = Angle;
	BlendSpaceAngle = FMath::Max(FMath::Min(180.f, BlendSpaceAngle), -180.f);
}

void UGsAnimInstanceState::StopBlendSpaceAngle()
{
	IsBlendSpaceAngleStart = false;
	BlendSpaceAngle = 0.f;
}

void UGsAnimInstanceState::StopMontagePlay()
{
	if (IsActiveMontage)
	{
		// 현재 설정된 BlendTime에 맞춰 몽타주 플레이 Stop 
		StopAllMontages(MontageToSequenceBlendTime);
	}
}

void UGsAnimInstanceState::SetPhysicsAnimation(bool IsPhysics)
{
	IsPhysicsAnimation = IsPhysics;
}

void UGsAnimInstanceState::SetRootmotionAnimation(bool IsEnable)
{
	IsRootmotionAnimation = IsEnable;
}

void UGsAnimInstanceState::SetSlotMontagePlay(EGsAnimSlotType AnimSlotType)
{
	SlotMontagePlay = (uint8)AnimSlotType;
}

void UGsAnimInstanceState::Pose()
{
	if (false == IsPoseAnimation)
	{
		SavePoseSnapshot(_cacheSnapshotName);
	}
	IsPoseAnimation = true;
}

void UGsAnimInstanceState::Resume()
{
	IsPoseAnimation = false;
}

void UGsAnimInstanceState::ResetCurrentState(FName FsmName)
{
	FAnimInstanceProxy& proxy = GetProxyOnGameThread<FAnimInstanceProxy>();
	if (FAnimNode_StateMachine* machine = proxy.GetStateMachineInstanceFromName(FsmName))
	{
		FAnimationInitializeContext InitializationContext(&proxy);
		machine->Initialize_AnyThread(InitializationContext);
	}
}

void UGsAnimInstanceState::AnimNotifyDestroy(UAnimSequenceBase* AnimSeqBase)
{
	for (FAnimNotifyEvent& el : AnimSeqBase->Notifies)
	{
		AnimNotifyDestroy(el.Notify);
	}
}

void UGsAnimInstanceState::AnimNotifyDestroy(FName FsmName, FName StateName)
{
	int idx = _proxy.GetInstanceAssetPlayerIndex(FsmName, StateName);
	if (FAnimNode_AssetPlayerBase* playerNode = _proxy.GetNodeFromIndex<FAnimNode_AssetPlayerBase>(idx))
	{
		if (UAnimSequenceBase* animSeqBase = Cast<UAnimSequenceBase>(playerNode->GetAnimAsset()))
		{
			AnimNotifyDestroy(animSeqBase);
		}
		//else if (UBlendSpaceBase* blendSpace = Cast<UBlendSpaceBase>(playerNode->GetAnimAsset()))
		//{
		//	// 구현 필요
		//}
	}
}

void UGsAnimInstanceState::ActiveMontageAnimNotifyDestroy()
{
	if (UAnimMontage* activeMontage = GetCurrentActiveMontage())
	{
		AnimNotifyDestroy(activeMontage);
	}
}

void UGsAnimInstanceState::AnimNotifyQueueDestroy()
{
	USkeletalMeshComponent* skeletalCoponent = _proxy.GetSkelMeshComponent();
	for (FAnimNotifyEventReference& el : NotifyQueue.AnimNotifies)
	{
		const FAnimNotifyEvent* notifyEvnet = el.GetNotify();
		AnimNotifyDestroy(notifyEvnet->Notify);
	}
}

void UGsAnimInstanceState::AnimNotifyDestroy(UAnimNotify* Notify)
{
	// 파티클 확인
	if (Notify->IsA(UAnimNotify_PlayParticleEffect::StaticClass()))
	{
		USkeletalMeshComponent* skeletalCoponent = _proxy.GetSkelMeshComponent();
		UAnimNotify_PlayParticleEffect* particle = Cast<UAnimNotify_PlayParticleEffect>(Notify);
		TArray<USceneComponent*> childrenCom =  skeletalCoponent->GetAttachChildren();
		for (USceneComponent* el : childrenCom)
		{
			if (UParticleSystemComponent* particleComp = Cast<UParticleSystemComponent>(el))
			{
				if (particleComp && particleComp->Template == particle->PSTemplate)
				{
					particleComp->DeactivateImmediate();					
				}
			}
		}
	}

	// 사운드 확인
	if (Notify->IsA(UAnimNotify_PlaySound::StaticClass()))
	{
		UAudioComponent* soundComponent = GetOwningActor()->FindComponentByClass<UAudioComponent>();
		UAnimNotify_PlaySound* sound = Cast<UAnimNotify_PlaySound>(Notify);
		if (soundComponent && soundComponent->Sound == sound->Sound)
		{
			soundComponent->Deactivate();
		}
	}
}

void UGsAnimInstanceState::AddAnimNotifyParticle(EPlayParticleManagedType In_Type, 
	TWeakObjectPtr<UParticleSystemComponent> In_Particle)
{
	if (In_Particle.IsValid())
	{
		if (AnimNotifyParticle* findInfo = _mapAnimNotifyParticle.Find(In_Type))
		{
			findInfo->Add(In_Particle);
		}
		else
		{
			AnimNotifyParticle newInfo;
			newInfo.Add(In_Particle);
			_mapAnimNotifyParticle.Emplace(In_Type, newInfo);
		}

		// 삭제 콜백 등록
		In_Particle.Get()->OnSystemFinished.AddDynamic(this, &UGsAnimInstanceState::CallbackAnimNotifyParticleDestroy);
	}
}

void UGsAnimInstanceState::RemoveAnimNotifyParticle(EPlayParticleManagedType In_Type)
{
	if (_mapAnimNotifyParticle.Num() > 0)
	{
		if (AnimNotifyParticle* findInfo = _mapAnimNotifyParticle.Find(In_Type))
		{
			for (int i = 0; i < findInfo->_listParticle.Num(); ++i)
			{
				if (UParticleSystemComponent* el = findInfo->_listParticle[i].Get())
				{
					el->Deactivate();
				}
			}
		}
		_mapAnimNotifyParticle.Remove(In_Type);
	}
}

void UGsAnimInstanceState::CallbackAnimNotifyParticleDestroy(UParticleSystemComponent* In_PSystem)
{
	for (auto it = _mapAnimNotifyParticle.CreateIterator(); it; ++it)
	{
		it.Value().RemoveAll(In_PSystem);

		if (it.Value().IsEmpty())
		{
			it.RemoveCurrent();
		}
	}
	In_PSystem->OnSystemFinished.RemoveDynamic(this, &UGsAnimInstanceState::CallbackAnimNotifyParticleDestroy);
}

void UGsAnimInstanceState::CallbakMontageStarted(UAnimMontage* Montage)
{
	RemoveAnimNotifyParticle(EPlayParticleManagedType::ImmediateDestroy);
	SequenceToMontageBlendTime = Montage->BlendIn.GetBlendTime();
	MontageToSequenceBlendTime = Montage->BlendOut.GetBlendTime();

	// IsRootmotionAnimation 비활성 처리는 외부에서 진행한다. (서버와 위치 동기화 때문에)
	SetRootmotionAnimation(Montage->HasRootMotion());
}

void UGsAnimInstanceState::CallbakStateChangedExit(const FAnimNode_StateMachine& Machine,
	int32 PrevStateIndex, int32 NextStateIndex)
{
	RemoveAnimNotifyParticle(EPlayParticleManagedType::ImmediateDestroy);
}

void UGsAnimInstanceState::OnInitialize(FName TopBoneName, FName BottomBonName)
{
// 	AActor* actor = GetOwningActor();
// 	if (ACharacter* characterActor = Cast<ACharacter>(actor))
// 	{
// 		if (USkeletalMeshComponent* CustomMeshComponent = characterActor->GetMesh())
// 		{
// 			FVector vecSpaceConstant = CustomMeshComponent->GetBoneLocation(FName("Root"));
// 			FVector vecSpaceTop = CustomMeshComponent->GetBoneLocation(TopBoneName);
// 			FVector vecSpaceBot = CustomMeshComponent->GetBoneLocation(BottomBonName);
// 			// 캡슐 사이즈
// 			float halfHeight = (vecSpaceTop.Z - vecSpaceBot.Z) * 0.5f;
// 
// 			// bone Tm 정보가 갱신되지 않았는데 처리되는 경우를 파악하기 위한 로그 
// 			if (halfHeight < 50.f)
// 			{
// 				GSLOG(Error, TEXT("---------------------- CapsuleSize Update Error!!"));
// 				GSLOG(Error, TEXT("halfHeight : %f"), halfHeight);
// 				GSLOG(Error, TEXT("SkeletalMesh : %s"), *CustomMeshComponent->SkeletalMesh->GetFullName());
// 				const TArray<FTransform>& listBoneTm = CustomMeshComponent->GetComponentSpaceTransforms();
// 				FVector BonePos = listBoneTm[1].GetLocation();
// 				GSLOG(Error, TEXT("Bone Transform %f, %f , %f"), BonePos.X, BonePos.Y, BonePos.Z);
// 				if (UAnimMontage* montage = GetCurrentActiveMontage())
// 				{
// 					GSLOG(Error, TEXT("Current Activemontage %s"), *montage->GetFullName());
// 				}
// 			}
// 			else
// 			{
// 				UCapsuleComponent* CapsuleComponent = characterActor->GetCapsuleComponent();
// 				if (CapsuleComponent)
// 				{
// 					CapsuleComponent->SetCapsuleHalfHeight(halfHeight);
// 
// 					// 메시 위치 설정
// 					float offset = vecSpaceConstant.Z - vecSpaceBot.Z;
// 					CustomMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -(halfHeight - offset)));
// 				}
// 			}
// 			OnRecalcSizeDelegate.ExecuteIfBound();
// 		}
// 	}
// 
// 	// 스테이트 머신 초기화
// 	_proxy.IsInitilaize = false;
// 	ResetCurrentState(StateMachineName);
}

float UGsAnimInstanceState::PlayAnimMontageJumpToSection(UAnimMontage* AnimMontage, float InPlayRate,
	FName SectionName, bool MissingAnimNotifyExcute)
{
	if (MissingAnimNotifyExcute)
	{
		float currPosion = 0.f;
		int sectionIndex = AnimMontage->GetSectionIndex(SectionName);
		if (-1 == sectionIndex)
		{
			return 0.f;
		}
		float sectionStart = AnimMontage->GetAnimCompositeSection(sectionIndex).GetTime();

		// 현재 재생중인 몽타주가 있는가 확인
		FAnimMontageInstance* montageInstance = GetActiveMontageInstance();
		if (montageInstance)
		{
			if (AnimMontage == montageInstance->Montage)
			{
				currPosion = montageInstance->GetPosition();
			}
		}

		// 미실행된 노티파이 조사
		for (FSlotAnimationTrack& el : AnimMontage->SlotAnimTracks)
		{
			for (FAnimSegment& seg : el.AnimTrack.AnimSegments)
			{
				UAnimSequenceBase* sequence = seg.AnimReference;

				TArray<FAnimNotifyEventReference> NotifyRefs;
				sequence->GetAnimNotifiesFromDeltaPositions(currPosion, sectionStart, NotifyRefs);

				// 실행
				for (FAnimNotifyEventReference& notify : NotifyRefs)
				{
					const FAnimNotifyEvent* Event = notify.GetNotify();
					if (Event->Notify)
					{
						Event->Notify->Notify(GetSkelMeshComponent(), AnimMontage);
					}
				}
			}
		}
	}

	float const Duration = Montage_Play(AnimMontage, InPlayRate);
	if (Duration > 0.f)
	{
		// Start at a given Section.
		if (SectionName != NAME_None)
		{
			Montage_JumpToSection(SectionName, AnimMontage);
		}

		return Duration;
	}

	return 0.f;
}

float UGsAnimInstanceState::GetCapsuleRelativeHeight(UCapsuleComponent* SrcCapsuleCompoent,
	USkeletalMeshComponent* DestMeshComponent, const FName& DestBoneName)
{
	if (SrcCapsuleCompoent && DestMeshComponent)
	{
		int32 BoneIndex = DestMeshComponent->GetBoneIndex(DestBoneName);
		if (BoneIndex == INDEX_NONE)
		{
			return 0.f;
		}

		FTransform srcTm = SrcCapsuleCompoent->GetComponentToWorld();
		FTransform destTm = DestMeshComponent->GetBoneTransform(BoneIndex);
		float halfheight = SrcCapsuleCompoent->GetScaledCapsuleHalfHeight();
		float lowWorldZ = srcTm.GetLocation().Z - halfheight;

		return lowWorldZ - destTm.GetLocation().Z;
	}
	return 0.f;
}