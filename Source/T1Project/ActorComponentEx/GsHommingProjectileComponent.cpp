// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponentEx/GsHommingProjectileComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Curves/CurveVector.h"

#include "ActorEx/GsCharacterBase.h"

#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif

void FGsHomingProjectileEffectInfo::Clear()
{
	if (_particleSystemComponent)
	{
		_particleSystemComponent->KillParticlesForced();
		_particleSystemComponent->DeactivateImmediate();
		_particleSystemComponent = nullptr;
		_returnParticle = nullptr;
		_hitParticleTemplate = nullptr;
		_curveData = nullptr;
		_targetActor = nullptr;
	}
}

// Sets default values for this component's properties
UGsHommingProjectileComponent::UGsHommingProjectileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

UGsHommingProjectileComponent::~UGsHommingProjectileComponent()
{
	// GarbageCollection.cpp
	// bool TickDestroyGameThreadObjects(bool bUseTimeLimit, float TimeLimit, double StartTime)
	// 가비지 컬랙션이 발생될 경우 EndPlay 호출이 안되는 상황이 있어 보인다.
	// ClearResource();
}

// Called when the game starts
void UGsHommingProjectileComponent::BeginPlay()
{
	Super::BeginPlay();
	

	// ...
	SetComponentTickEnabled(false);
}

void UGsHommingProjectileComponent::BeginDestroy()
{
	// ClearResource();
	Super::BeginDestroy();
}

void UGsHommingProjectileComponent::EndPlay(EEndPlayReason::Type Result)
{
	ClearResource();
	Super::EndPlay(Result);
}

// Called every frame
void UGsHommingProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	UpdateHomingProjectileEffect(DeltaTime);

	if (_listHomingProjectile.Num() == 0 && _listTargetActor.Num() == 0)
	{
		SetComponentTickEnabled(false);
	}
}

void UGsHommingProjectileComponent::OnHomingProjectileHit(AActor* Caster, int EventId)
{
	OnHitHomingProjectile.Broadcast(Caster, EventId);
}

void UGsHommingProjectileComponent::SetHommingProjectileInfo(HitResultType inResultType, AGsCharacterBase* inTarget)
{
	if (HitResultType* findType = _mapHomingProjectileInfo.Find(inTarget))
	{
		*findType = inResultType;
	}
	else
	{
		_mapHomingProjectileInfo.Emplace(inTarget, inResultType);
	}
}

void UGsHommingProjectileComponent::ClearHommingProjectileInfo()
{
	_mapHomingProjectileInfo.Empty();

	if (_listTargetActor.Num() > 0)
	{
		for (AGsCharacterBase* el : _listTargetActor)
		{
			el->OnDestroyed.Remove(this, TEXT("OnTargetActorDestroyed"));
		}
		_listTargetActor.Empty();
	}
}

void UGsHommingProjectileComponent::ClearResource()
{
	for (FGsHomingProjectileEffectInfo& el : _listHomingProjectile)
	{
		el.Clear();
	}
	_listHomingProjectile.Empty();
}

void UGsHommingProjectileComponent::OnHomingProjectileEffectActive(EGsHomingHitParticleType In_type, UParticleSystem* In_Particle,
	const FGsHomingTargetInfo& In_TargetInfo, const FRotator& In_ParticleRot, float In_MoveSpeed, float In_TargetSize,
	UCurveVector* In_CurveVector, UParticleSystem* In_HitParticle, bool In_HitEffectAttackerLookDir,
	UParticleSystem* In_ReturnParticle)
{
	// FTransform relativeTm = In_TargetInfo.SpawnTm.GetRelativeTransform()
	FGsHomingProjectileEffectInfo newHomingProjectile;
	newHomingProjectile._particleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(), In_Particle, In_TargetInfo.SpawnTm, true, EPSCPoolMethod::AutoRelease);

	if (nullptr == newHomingProjectile._particleSystemComponent)
	{
		return;
	}

	if (false == In_ParticleRot.IsNearlyZero())
	{
		newHomingProjectile._particleSystemComponent->SetWorldRotation(In_ParticleRot);
	}

	newHomingProjectile._currPosition = In_TargetInfo.SpawnTm.GetLocation();
	newHomingProjectile._originPosition = In_TargetInfo.SpawnTm.GetLocation();
	newHomingProjectile._targetPosition = In_TargetInfo.GoalPos;
	newHomingProjectile._hitParticleTemplate = In_HitParticle;
	newHomingProjectile._hitEffectOffsetTm = In_TargetInfo.HitOffsetTm;
	newHomingProjectile._isHit = In_TargetInfo.IsHit;
	newHomingProjectile._size = In_TargetSize;
	newHomingProjectile._count = (In_type == EGsHomingHitParticleType::RETURN) ? 2 : 1;
	newHomingProjectile._moveSpeed = In_MoveSpeed;
	newHomingProjectile._targetActor = In_TargetInfo.GoalActor;
	newHomingProjectile._eventId = In_TargetInfo.EventId;

	FVector dir = (In_TargetInfo.GoalPos - newHomingProjectile._currPosition);
	newHomingProjectile._originDistance = dir.Size();
	newHomingProjectile._velocity = dir.GetSafeNormal() * In_MoveSpeed;
	newHomingProjectile._destTime = newHomingProjectile._originDistance / In_MoveSpeed;
	newHomingProjectile._destTimeInv = 1 / newHomingProjectile._destTime;
	newHomingProjectile._curveData = In_CurveVector;
	newHomingProjectile._isHitEffectAttackerLookDir = In_HitEffectAttackerLookDir;

	if (In_ReturnParticle)
	{
		newHomingProjectile._returnParticle = In_ReturnParticle;
	}

	_listHomingProjectile.Add(newHomingProjectile);

// 	DrawDebugDirectionalArrow(GetWorld(), newHomingProjectile._currPosition, 
// 		newHomingProjectile._targetPosition, 10.f, FColor::Green, false, 5.f);

	SetComponentTickEnabled(true);
}

void UGsHommingProjectileComponent::UpdateHomingProjectileEffect(float DeltaTime)
{
	FVector ownerLocation = GetOwner()->GetActorLocation();

	for (int i = 0; i < _listHomingProjectile.Num(); ++i)
	{
		FGsHomingProjectileEffectInfo& el = _listHomingProjectile[i];
		if (el._isFirst)
		{
			el._isFirst = false;
			continue;
		}
		FVector oldPosition = el._currPosition;

		UpdateHomingProjectilePosition(el, DeltaTime);

		// 발사체가 목적지 보다 뒤쪽에 갔을때를 판단후 렌덤위치에 히트 이펙트출력
		FVector targetDir = el._targetPosition - el._currPosition;
		FVector projectileDir = el._currPosition - oldPosition;
		float v = FVector::DotProduct(targetDir.GetSafeNormal(), projectileDir.GetSafeNormal());
		if (v < 0.f)
		{
			if (el._isHit)
			{
				FVector lookDir = el._targetPosition - ownerLocation;
				//if (el._isHitEffectAttackerLookDir == true)
				//{
				//	/*DrawDebugDirectionalArrow(GetWorld(), el._targetPosition, el._currPosition,
				//		20.f, FColor::Green, false, 5.f);*/

				//	DrawDebugDirectionalArrow(GetWorld(), el._targetPosition, ownerLocation,
				//			20.f, FColor::Green, false, 5.f);
				//}
				OnHitHomingProjectileEffect(el._hitParticleTemplate, el._targetActor, el._targetPosition,
					el._hitEffectOffsetTm, el._size, el._isHitEffectAttackerLookDir, lookDir, el._eventId);
			}

			if (el._count == 1)
			{
				el._isDelete = true;
				_isDeleteHomingProjectile = true;
				el.Clear();
			}
			else
			{
				ChangeHomingProjectileTarget(el);
			}
		}
	}

	if (_isDeleteHomingProjectile)
	{
		_listHomingProjectile.RemoveAll([](FGsHomingProjectileEffectInfo& el)
			{
				return el._isDelete;
			});

		_isDeleteHomingProjectile = false;
	}
}

void UGsHommingProjectileComponent::ChangeHomingProjectileTarget(OUT FGsHomingProjectileEffectInfo& Out_Info)
{
	if (Out_Info._count > 1)
	{
		Out_Info._count--;
		// 다단 히트를 구현하게 되면 이부분에 대한 처리 수정
		if (Out_Info._count == 1)
		{
			Out_Info._targetPosition = Out_Info._originPosition;
			Out_Info._targetActor = nullptr;
			Out_Info._isHit = false;
		}

		FVector dir = (Out_Info._targetPosition - Out_Info._currPosition);
		Out_Info._originDistance = dir.Size();
		Out_Info._velocity = dir.GetSafeNormal() * Out_Info._moveSpeed;
		Out_Info._currentTime = 0.f;
		Out_Info._destTime = dir.Size() / Out_Info._moveSpeed;
		Out_Info._destTimeInv = 1 / Out_Info._destTime;

		if (Out_Info._returnParticle)
		{
			Out_Info._particleSystemComponent->SetTemplate(Out_Info._returnParticle);
		}

		// 한번더 계산해줌
		UpdateHomingProjectilePosition(Out_Info, 0.033f);
	}
}

void UGsHommingProjectileComponent::UpdateHomingProjectilePositionFixed(OUT FGsHomingProjectileEffectInfo& In_Info, float DeltaTime)
{
	/* 
	* 정적 목표 위치 연산
	* 고정값 : 목표 위치, 속도, 도달 시간(Inverse)
	* 1. 변위 = 속도 * 시간 변화량
	* 2. 위치 갱신 = 이전 위치 + 변위
	* 3. 시간 갱신 = 이전 시간 + 시간 변화량
	* 4. 시간별 파티클 변화 적용
	* 5. 최종 위치 적용
	*/

	if (In_Info._particleSystemComponent &&
		In_Info._particleSystemComponent->IsActive())
	{
		FVector moveDelta = In_Info._velocity * DeltaTime;
		In_Info._currPosition += moveDelta;
		In_Info._currentTime += DeltaTime;
		FVector lastPosition = In_Info._currPosition;
		if (In_Info._curveData)
		{
			FRotator particleRot = In_Info._particleSystemComponent->GetComponentRotation();
			float rate = In_Info._currentTime * In_Info._destTimeInv;
			lastPosition += particleRot.RotateVector(In_Info._curveData->GetVectorValue(rate));
		}

		In_Info._particleSystemComponent->SetWorldLocation(lastPosition);
	}
}

void UGsHommingProjectileComponent::UpdateHomingProjectilePositionDynamic(OUT FGsHomingProjectileEffectInfo& In_Info, float DeltaTime)
{
	if (nullptr == In_Info._targetActor)
		return;

	/*
	* 동적 목표 위치 연산 : 틱마다 상대 위치로 방향 변경
	* 1. 상대 목표 위치 갱신
	* 2. 속력 = 기존 or 거리 증가 시 속력 증가
	* 3. 새로운 방향 = 타겟 위치 - 유도체 위치
	* 4. 변위 = 속력 * 방향 * 시간 변화량
	* 5. 위치 갱신 = 이전 위치 + 변위
	* 6. 시간 갱신 = 이전 시간 + 시간 변화량
	* 7. 시간별 파티클 변화 적용
	* 8. 최종 위치 적용
	*/

	In_Info._targetPosition = In_Info._targetActor->GetActorLocation();

	float moveSpeed = In_Info._moveSpeed;
	float newDistance = (In_Info._originPosition - In_Info._targetPosition).Size();
	if (newDistance > In_Info._originDistance)
	{
		moveSpeed = moveSpeed * newDistance / In_Info._originDistance;
	}

	FVector newDirection = In_Info._targetPosition - In_Info._currPosition;
	FVector moveDelta = moveSpeed * newDirection.GetSafeNormal() * DeltaTime;
	In_Info._currPosition += moveDelta;
	In_Info._currentTime += DeltaTime;

	FVector lastPosition = In_Info._currPosition;
	if (In_Info._curveData)
	{
		FRotator particleRot = In_Info._particleSystemComponent->GetComponentRotation();
		float rate = In_Info._currentTime * In_Info._destTimeInv;
		lastPosition += particleRot.RotateVector(In_Info._curveData->GetVectorValue(rate));
	}

	In_Info._particleSystemComponent->SetWorldLocation(lastPosition);
}

void UGsHommingProjectileComponent::UpdateHomingProjectilePositionDynamicBezier(OUT FGsHomingProjectileEffectInfo& In_Info, float DeltaTime)
{
	if (nullptr == In_Info._targetActor)
		return;

	AGsCharacterBase* characterActor = Cast<AGsCharacterBase>(In_Info._targetActor);
	if (nullptr == characterActor)
		return;

	if (nullptr == In_Info._particleSystemComponent ||
		false == In_Info._particleSystemComponent->IsActive())
		return;

	/*
	* 동적 목표 위치 연산 : 2차 베지어 곡선 차용 (목표 시간과 가까워질수록 더 많이 회전한다.)
	* 1. 시간 갱신 = 이전 시간 + 시간 변화량 상대 목표 위치 갱신
	* 2. 속력 = 기존 or 거리 증가 시 속력 증가
	* 3. 새로운 방향 = 현재 시간에 대한 2차 베지어 위치 - 유도체 위치
	* 4. 변위 = 속력 * 방향 * 시간 변화량
	* 5. 위치 갱신 = 이전 위치 + 변위
	* 6. 시간 갱신 = 이전 시간 + 시간 변화량
	* 7. 시간별 파티클 변화 적용
	* 8. 최종 위치 적용
	*/

	const FVector& targetPrevLocation = In_Info._targetPosition;
	FVector targetCurrLocation = characterActor->GetTargetHitPoint(In_Info._currPosition, FVector::ForwardVector, TEXT("HitEffectSocket"), false, FVector::ZeroVector); // Forward는 의미 없는 부분

	In_Info._currentTime += DeltaTime;

	float moveSpeed = In_Info._moveSpeed;
	float newDistance = (In_Info._originPosition - targetCurrLocation).Size();
	if (newDistance > In_Info._originDistance)
	{
		moveSpeed = moveSpeed * newDistance / In_Info._originDistance;
	}

	FVector dir = FVector::ZeroVector;
	float rate = In_Info._currentTime * In_Info._destTimeInv;
	if (In_Info._currentTime < In_Info._destTime)
	{
		// 새로운 방향 계산 : 2차 베지어 곡선의 특정 위치 계산
		float u = 1 - rate;
		FVector nextEffectPosition = u * u * In_Info._currPosition + 2 * u * rate * targetPrevLocation + rate * rate * targetCurrLocation;
		dir = nextEffectPosition - In_Info._currPosition;
	}
	else
	{
		dir = targetCurrLocation - In_Info._currPosition;
	}

	FVector moveDelta = moveSpeed * dir.GetSafeNormal() * DeltaTime;
	In_Info._currPosition += moveDelta;
	In_Info._targetPosition = targetCurrLocation;

	FVector nextPosition = In_Info._currPosition;
	if (In_Info._curveData)
	{
		FRotator particleRot = In_Info._particleSystemComponent->GetComponentRotation();
		nextPosition += particleRot.RotateVector(In_Info._curveData->GetVectorValue(rate));
	}

	In_Info._particleSystemComponent->SetWorldLocation(nextPosition);
}

void UGsHommingProjectileComponent::UpdateHomingProjectilePosition(OUT FGsHomingProjectileEffectInfo& In_Info, float DeltaTime)
{
	if (nullptr != In_Info._targetActor)
	{
		UpdateHomingProjectilePositionDynamicBezier(In_Info, DeltaTime);
		//UpdateHomingProjectilePositionDynamic(In_Info, DeltaTime);
	}
	else
	{
		UpdateHomingProjectilePositionFixed(In_Info, DeltaTime);
	}
	

// #if WITH_EDITOR
// // 		// 디버깅 확인용
//  		DrawDebugPoint(GetWorld(), lastPosition, 10.f, FColor::Red, false, DeltaTime * 10.f);
// #endif
}

void UGsHommingProjectileComponent::OnHitHomingProjectileEffect(UParticleSystem* In_Particle, AActor* In_TargetActor,
	const FVector& In_SpawnLocation, const FTransform& In_Offset, float In_Size, bool In_isAttackerLookDir,
	FVector& In_targetDir, int32 In_Id)
{
	float randX = FMath::RandRange(0.f, In_Size);
	randX *= FMath::RandBool() ? -1.f : 1.f;

	float randZ = FMath::RandRange(0.f, In_Size);
	FVector spawnPos = In_SpawnLocation;
	spawnPos += FVector(randX, 0.f, randZ);
	FRotator useRot = FRotator::ZeroRotator;
	if (In_isAttackerLookDir == true)
	{
		useRot = In_targetDir.Rotation() + FRotator(0.0f, -90.0f, 0.0f);
	}
	else
	{
		useRot = GetOwner()->GetActorRotation().GetInverse();
	}
	FTransform tm(useRot, spawnPos);
	tm *= In_Offset;

	if (false == tm.IsValid())
	{
		return;
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), In_Particle, tm, true, EPSCPoolMethod::AutoRelease);

	if (In_TargetActor)
	{
		// 타겟의 컴퍼넌트에 연결
		if (AGsCharacterBase* castCharacter = Cast<AGsCharacterBase>(In_TargetActor))
		{
			if (UGsHommingProjectileComponent* hommingComponent = castCharacter->GetHommingProjectile())
			{
				hommingComponent->OnHomingProjectileHit(GetOwner(), In_Id);
			}
		}
	}

#if WITH_EDITOR
	OnEditorViewHitHomingProjectile.Broadcast(tm.GetLocation());
#endif

	// #if WITH_EDITOR
	// 		// 디버깅 확인용
	// 		DrawDebugDirectionalArrow(GetWorld(), In_SpawnLocation, tm.GetLocation(),
	// 			20.f, FColor::Green, false, 5.f);
	// #endif

		//if (In_isAttackerLookDir == false)
		//{
		//	FVector newPos = In_SpawnLocation + GetOwner()->GetActorRotation().GetInverse().Vector() * 1000.0f;
		//	DrawDebugDirectionalArrow(GetWorld(), In_SpawnLocation, newPos,
		//		20.f, FColor::Green, false, 5.f);
		//}

}

void UGsHommingProjectileComponent::SetTargetActor(AGsCharacterBase* inTarget)
{
	if (nullptr == inTarget)
	{
		return;
	}

	if (false == _listTargetActor.Contains(inTarget))
	{
		_listTargetActor.Emplace(inTarget);

		FScriptDelegate destroyDelegate;
		destroyDelegate.BindUFunction(this, TEXT("OnTargetActorDestroyed"));
		inTarget->OnDestroyed.Add(destroyDelegate);
	}
}

TArray<AGsCharacterBase*> UGsHommingProjectileComponent::GetTargetActor() const
{
	return _listTargetActor;
}

void UGsHommingProjectileComponent::OnTargetActorDestroyed(AActor* DestroyedActor)
{
	if (AGsCharacterBase* castActor = Cast<AGsCharacterBase>(DestroyedActor))
	{
		_listTargetActor.Remove(castActor);
	}
}