#include "GsEffectComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Curves/CurveVector.h"
#include "Classes/Components/SkinnedMeshComponent.h"
#include "Classes/Components/SkeletalMeshComponent.h"
#include "Classes/GameFramework/Character.h"
#include "Classes/Particles/ParticleSystem.h"
#include "Classes/Particles/ParticleSystemComponent.h"
#include "ParticleEmitterInstances.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "ActorEx/GsCharacterBase.h"
#include "GsHommingProjectileComponent.h"
#include "DrawDebugHelpers.h"
#include "ForEach.h"

bool FGsBeamParticleEffectInfo::IsValid()
{
	if (_particle)
	{
		return _particle->IsValidLowLevel();
	}
	return false;
}

void FGsSoketParticleArray::Clear()
{
	for (int i = 0; i < ActiveParts.Num(); ++i)
	{
		if (ActiveParts[i].Active)
		{
			ActiveParts[i].Active->ReleaseToPool();			
			ActiveParts[i].Active = nullptr;
		}
	}
	ActiveParts.Empty();
}

void FGsSoketParticleArray::Activate()
{
	for (int i = 0; i < ActiveParts.Num(); ++i)
	{
		if(ActiveParts[i].Active)
		{
			ActiveParts[i].Active->Activate(true);
		}
	}
}

void FGsSoketParticleArray::Deactivate()
{
	for (int i = 0; i < ActiveParts.Num(); ++i)
	{
		if (ActiveParts[i].Active)
		{
			ActiveParts[i].Active->Deactivate();
		}
	}
}

UGsEffectComponent::UGsEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UGsEffectComponent::~UGsEffectComponent()
{
	// GarbageCollection.cpp
	// bool TickDestroyGameThreadObjects(bool bUseTimeLimit, float TimeLimit, double StartTime)
	// 가비지 컬랙션이 발생될 경우 EndPlay 호출이 안되는 상황이 있어 보인다.
	// ClearResource();
}

void UGsEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);
}

void UGsEffectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ClearResource();
	
	Super::EndPlay(EndPlayReason);
}

void UGsEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateBeamParticleEffect(DeltaTime);
	if (IsCognitionTargetEffectActive())
	{
		_attackCognitionTime += DeltaTime;
		if (_attackCognitionTime > _attackCognitionEndTime)
		{
			OnCognitionTargetEffectDeActive();
		}
	}

	// TickEnable을 제어 하는 정보들을 확인해야한다.
	if (false == _beamParticleInfo.IsValid() && nullptr == _attackCognitionEffect)
	{
		SetComponentTickEnabled(false);
	}
}

void UGsEffectComponent::BeginDestroy()
{
	// ClearResource();
	Super::BeginDestroy();
}

void UGsEffectComponent::ClearResource()
{
	OnDebuffDeactive();
	OnCognitionTargetEffectDeActive();
	OnBeamParticleEffectDeactive();
	OnVehicleEffectDeActive();

	int representAbnormalityEffNum = _mapAbnormalityEffectByParts.Num();
	if (representAbnormalityEffNum > 0)
	{
		for (auto& iter : _mapAbnormalityEffectByParts)
		{
			iter.Value.Clear();
		}
		_mapAbnormalityEffectByParts.Empty();
	}
}

UParticleSystemComponent* UGsEffectComponent::OnEffect(UParticleSystem* In_particle, bool In_isAutoDestroy,
	EPSCPoolMethod In_poolType, EAttachLocation::Type In_locationType, FVector In_location, FVector In_Scale,
	FName In_attachPointName, bool In_isMeshAttach)
{
	if (_enable == false || In_particle == nullptr) //|| GetOwner()->IsHidden()) //hidden 상태에서 다시 나타날때 restore 가능하도록 visible만 꺼둔다
	{
		return nullptr;
	}

	USceneComponent* targetcomponent = nullptr;

	if (In_isMeshAttach == true)
	{
		if (ACharacter* actor = Cast<ACharacter>(GetOwner()))
		{
			targetcomponent = actor->GetMesh();
		}
	}
	else
	{
		targetcomponent = _attachTarget;
	}
	

	UParticleSystemComponent* resVal = nullptr;
	if (targetcomponent != nullptr)
	{
		resVal =
			UGameplayStatics::SpawnEmitterAttached(
				In_particle,
				targetcomponent,
				In_attachPointName,
				In_location,
				FRotator::ZeroRotator,
				In_Scale,
				In_locationType,
				In_isAutoDestroy,
				In_poolType,
				true);

		if (resVal && GetOwner()->IsHidden())
		{
			resVal->SetHiddenInGame(true, true);
		}
	}	

	return resVal;
}


// 위치에 이펙트 출력
UParticleSystemComponent* UGsEffectComponent::OnEffectPos(UParticleSystem* In_particle, const FVector& In_pos,
	const FVector& In_Scale, EPSCPoolMethod In_poolType)
{
	UParticleSystemComponent* resVal = nullptr;
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return resVal;
	}
	resVal = UGameplayStatics::SpawnEmitterAtLocation(
		world,
		In_particle,
		In_pos,
		FRotator::ZeroRotator,
		In_Scale,
		true,
		In_poolType,
		true);

	return resVal;
}

// 소켓 위치에 이펙트 출력
UParticleSystemComponent* UGsEffectComponent::OnEffectSocketPos(UParticleSystem* In_particle, FName In_socketName, float InScaleMultiplier, EPSCPoolMethod In_poolType)
{
	if (nullptr == _attachTarget)
		return nullptr;

	const FTransform ParentToWorld = _attachTarget->GetSocketTransform(In_socketName);	
	return  OnEffectPos(In_particle, ParentToWorld.GetLocation(), FVector::OneVector * InScaleMultiplier, In_poolType);
}

// 붙일 타겟 찾기
void UGsEffectComponent::FindAttachTarget()
{
	if (ACharacter* actor = Cast<ACharacter>(GetOwner()))
	{
		_attachTarget = actor->GetRootComponent();
		_enable = true;
	}
	else
	{
		_enable = false;
	}
}

void UGsEffectComponent::OnDebuffActive(UParticleSystem* In_particle)
{
	// 기존에 있다면 날리고
	if (_debuffParticle != nullptr)
	{
		OnDebuffDeactive();
	}

	UParticleSystem* setParticle = (In_particle == nullptr) ?
		GData()->GetGlobalData()->_debuffParticle : In_particle;

	_debuffParticle =
		OnEffect(setParticle, true,
			EPSCPoolMethod::ManualRelease, EAttachLocation::KeepRelativeOffset,
			GData()->GetGlobalData()->_debuffOffset, FVector::OneVector,
			GData()->GetGlobalData()->_debuffAttachPointName,
			true);
}

void UGsEffectComponent::OnDebuffDeactive()
{
	if (_debuffParticle != nullptr)
	{	
		_debuffParticle->ReleaseToPool();		
		_debuffParticle = nullptr;
	}	
}

void UGsEffectComponent::OnPresentAbnormalityEffectActive(int32 In_key, UParticleSystem* In_particle,
	EGsAbnormalityEffectParticleType attachType, FName In_socketName, FVector In_posOffset, 
	FVector In_Scale, EPSCPoolMethod In_PoolType)
{
	// 없는건데
	if (In_particle == nullptr)
	{
		return;
	}

	UParticleSystemComponent* findParticle = nullptr; 

	FName useName = NAME_None;
	if (attachType == EGsAbnormalityEffectParticleType::EmitterAtLocation)
	{
		if (nullptr == _attachTarget)
			return;

		const FTransform ParentToWorld = _attachTarget->GetSocketTransform(In_socketName);
		findParticle =
			OnEffectPos(In_particle, ParentToWorld.GetLocation() + In_posOffset, In_Scale, In_PoolType);
	}
	else
	{
		OnPresentAbnormalityEffectDeactive(In_key, In_socketName); //null 이어도 삭제

		useName = In_socketName;
		findParticle =
			OnEffect(In_particle, true,
				In_PoolType, EAttachLocation::KeepRelativeOffset,
				In_posOffset,
				In_Scale,
				In_socketName,
				true);

		if (findParticle && attachType == EGsAbnormalityEffectParticleType::EmitterAttachedOnlyLocation)
		{
			findParticle->SetUsingAbsoluteRotation(true);
		}
	}

	if (findParticle && In_PoolType == EPSCPoolMethod::ManualRelease)
	{
		FGsParticleByPart temp;
		temp.SocketName() = In_socketName;
		temp.Active = findParticle;
		_mapAbnormalityEffectByParts.FindOrAdd(In_key).ActiveParts.Add(temp);
	}
}

void UGsEffectComponent::OnPresentAbnormalityEffectDeactive(int32 In_key, FName In_socketName)
{
	if (_mapAbnormalityEffectByParts.Contains(In_key))
	{
		for (auto& ParticleByPart : _mapAbnormalityEffectByParts[In_key].ActiveParts)
		{
			if (ParticleByPart.Active && ParticleByPart._socketName == In_socketName)
			{
				ParticleByPart.Active->ReleaseToPool();
				ParticleByPart.Active = nullptr;
			}
		}

		_mapAbnormalityEffectByParts[In_key].ActiveParts.RemoveAll([In_socketName](auto e) { return e._socketName == In_socketName || e.Active == nullptr; });

		if (0 >= _mapAbnormalityEffectByParts[In_key].ActiveParts.Num())
		{
			_mapAbnormalityEffectByParts[In_key].Clear();
			_mapAbnormalityEffectByParts.Remove(In_key);
		}
	}
}

void UGsEffectComponent::OnPresentAbnormalityEffectDeactive(int32 In_key, UParticleSystemComponent* deactiveComponent)
{
	if (deactiveComponent && _mapAbnormalityEffectByParts.Contains(In_key))
	{
		auto findEffectByPart = _mapAbnormalityEffectByParts[In_key].ActiveParts.FindByPredicate([deactiveComponent](const FGsParticleByPart& InData)
			{
				return InData.Active == deactiveComponent;
			}
		);

		if (findEffectByPart )
		{
			OnPresentAbnormalityEffectDeactive(In_key, findEffectByPart->_socketName);
		}
	}
}

void UGsEffectComponent::OnAllPresentAbnormalityEffectsDeactive(int32 In_key)
{
	if (auto iter = _mapAbnormalityEffectByParts.Find(In_key))
	{
		iter->Clear();
	}
	_mapAbnormalityEffectByParts.Remove(In_key);
}


void UGsEffectComponent::OnCognitionTargetEffectDeActive()
{
	if (IsCognitionTargetEffectActive())
	{
		_attackCognitionEffect->ReleaseToPool();
		// nullptr 처리를 해주지 않으면, 가비지 되기전까지 UParticleSystemComponent* 변수는 쓰레기 값으로 남아
		// !nullptr 조건에 오작동
		_attackCognitionEffect = nullptr;
		_attackCognitionTime = 0.f;
	}
}

UParticleSystemComponent* UGsEffectComponent::OnCognitionTargetEffectActive(UParticleSystem* In_Particle, 
	const FName& In_TargetSocketName, float In_DestoryTime, const FVector& In_posOffset)
{
	OnCognitionTargetEffectDeActive();

	if (UParticleSystemComponent* paricleSystem = OnEffect(In_Particle, true, EPSCPoolMethod::ManualRelease,
		EAttachLocation::KeepRelativeOffset, In_posOffset, FVector::OneVector, In_TargetSocketName,true))
	{
		_attackCognitionEffect = paricleSystem;
		_attackCognitionTime = 0.f;
		_attackCognitionEndTime = In_DestoryTime;
		SetComponentTickEnabled(true);
		
		return paricleSystem;
	}

	return nullptr;
}

bool UGsEffectComponent::IsCognitionTargetEffectActive() const
{
	return _attackCognitionEffect != nullptr;
}

UParticleSystemComponent* UGsEffectComponent::OnVehicleEffectActive(UParticleSystem* In_Particle)
{
	if (nullptr == In_Particle)
	{
		return nullptr;
	}

	if (IsVehicleEffectActive())
	{
		OnVehicleEffectDeActive();
	}

	if (UParticleSystemComponent* paricleSystem = OnEffect(In_Particle, true, EPSCPoolMethod::ManualRelease,
		EAttachLocation::KeepRelativeOffset, FVector::ZeroVector, FVector::OneVector, NAME_None, true))
	{
		_vehicleEffect = paricleSystem;

		return paricleSystem;
	}

	return nullptr;
}

void UGsEffectComponent::OnVehicleEffectDeActive()
{
	if (IsVehicleEffectActive())
	{
		// nullptr 처리를 해주지 않으면, 가비지 되기전까지 UParticleSystemComponent* 변수는 쓰레기 값으로 남아
		_vehicleEffect->ReleaseToPool();
		// !nullptr 조건에 오작동
		_vehicleEffect = nullptr;
	}
}

bool UGsEffectComponent::IsVehicleEffectActive()
{
	return _vehicleEffect != nullptr;
}

void UGsEffectComponent::OnBeamParticleEffectActive(UParticleSystem* In_particle, AActor* In_Target, 
	const FName& In_TargetSocketName, float In_LifeTime)
{
	if (In_particle)
	{
		// 아직 존재 하면 제거
		if (_beamParticleInfo.IsValid())
		{
			OnBeamParticleEffectDeactive();
		}

		AActor* owner = GetOwner();
		if (nullptr == owner)
		{
			return;
		}

		_beamParticleInfo._particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), In_particle,
			owner->GetActorLocation(), FRotator::ZeroRotator, FVector::OneVector,
			true, EPSCPoolMethod::AutoRelease);

		if (_beamParticleInfo._particle)
		{
			// 기존에 존재 하던 _attachTarget 변수를 활용
			if (ACharacter* actor = Cast<ACharacter>(owner))
			{
				_attachTarget = actor->GetMesh();
			}

			if (In_Target)
			{
				_beamParticleInfo._targetActor = In_Target;
				_beamParticleInfo._targetActor->OnDestroyed.AddDynamic(this, &UGsEffectComponent::OnTargetActorDestroyed);
				_beamParticleInfo._lastTargetPos = In_Target->GetActorLocation();
				_beamParticleInfo._targetSocket = In_TargetSocketName;
				_beamParticleInfo._lifeTime = In_LifeTime;
			}

			// BeamData 모듈을 찾는다.
			_beamParticleInfo._listEmitterIndex.Empty();
			UParticleSystemComponent* particleComponent = _beamParticleInfo._particle;
			for (int idx = 0 ; idx < particleComponent->EmitterInstances.Num(); ++idx)
			{
				if (particleComponent->EmitterInstances[idx])
				{
					if (particleComponent->EmitterInstances[idx]->bIsBeam)
					{
						_beamParticleInfo._listEmitterIndex.Emplace(idx);
					}
				}
			}

			SetComponentTickEnabled(true);
		}
	}
}

void UGsEffectComponent::OnTargetActorDestroyed(AActor* DestroyedActor)
{
	if (_beamParticleInfo._targetActor && _beamParticleInfo._targetActor == DestroyedActor)
	{
		_beamParticleInfo._targetActor->OnDestroyed.Remove(this, TEXT("OnTargetActorDestroyed"));
		_beamParticleInfo._lastTargetPos = _beamParticleInfo._targetActor->GetActorLocation();
		_beamParticleInfo._targetActor = nullptr;
	}
}

void UGsEffectComponent::OnBeamParticleEffectDeactive()
{
	if (_beamParticleInfo._particle)
	{
		_beamParticleInfo._particle->DeactivateImmediate();
		if (_beamParticleInfo._targetActor)
		{
			_beamParticleInfo._targetActor->OnDestroyed.Remove(this, TEXT("OnTargetActorDestroyed"));
		}
		_beamParticleInfo._particle = nullptr;
		_beamParticleInfo._targetActor = nullptr;
	}
}

void UGsEffectComponent::UpdateBeamParticleEffect(float DeltaTime)
{
	if (_beamParticleInfo.IsValid())
	{
		// 소켓 위치 얻어오기
		if (_attachTarget)
		{
			FVector srcPos = _attachTarget->GetSocketLocation(_beamParticleInfo._sourceSocket);
			for (int32 idx : _beamParticleInfo._listEmitterIndex)
			{
				_beamParticleInfo._particle->SetBeamSourcePoint(idx, srcPos, 0);
			}
		}

		FVector targetPos = FVector::ZeroVector;
		if (_beamParticleInfo._targetActor->IsValidLowLevel())
		{
			if (AGsCharacterBase* castActor = Cast<AGsCharacterBase>(_beamParticleInfo._targetActor))
			{
				targetPos = castActor->GetMesh()->GetSocketLocation(
					_beamParticleInfo._targetSocket);
			}
			else
			{
				targetPos = _beamParticleInfo._targetActor->GetActorLocation();
			}
		}
		else
		{
			targetPos = _beamParticleInfo._lastTargetPos;
		}

		if (false == targetPos.IsNearlyZero())
		{
			for (int32 idx : _beamParticleInfo._listEmitterIndex)
			{
				_beamParticleInfo._particle->SetBeamTargetPoint(idx, targetPos, 0);
			}
		}

		if (_beamParticleInfo._lifeTime > 0.f)
		{
			_beamParticleInfo._lifeTime -= DeltaTime;
			if (_beamParticleInfo._lifeTime < 0.f)
			{
				OnBeamParticleEffectDeactive();
			}
		}
	}
}

// represent abnormality effect visible
void UGsEffectComponent::SetRepresentAbnormalityEffectVisible(bool In_isVisible)
{
	int representAbnormalityEffNum = _mapAbnormalityEffectByParts.Num();
	if (representAbnormalityEffNum > 0)
	{
		for (auto& iter : _mapAbnormalityEffectByParts)
		{
			In_isVisible ? iter.Value.Activate() : iter.Value.Deactivate();
		}
	}
}