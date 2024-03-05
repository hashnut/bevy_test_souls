// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotifyState_HomingProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveVector.h"

#include "ActorEx/GsCharacterBase.h"
#include "ActorComponentEx/GsHommingProjectileComponent.h"

#include "Shared/Client/SharedEnums/SharedSkillEnum.h"

#include "../GsAnimInstanceState.h"

#include "../T1Project.h"

#if WITH_EDITOR
#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "../ActorComponentEx/GsHitEffectComponent.h"
#include "../ActorComponentEx/GsMeshShaderControlComponent.h"
#include "../Data/GsDataContainManager.h"
#include "DrawDebugHelpers.h"
#include "ActorComponentEx/EGsMeshShaderEffect.h"
#endif


void UAnimNotifyState_HomingProjectile::NewHommingTargetInfo(OUT FGsHomingTargetInfo& outInfo, OUT float& outTargetSize, 
	AGsCharacterBase* OwnerActor, AGsCharacterBase* TargetActor, bool IsMiss)
{
	const FTransform& spawnTm = OwnerActor->GetMesh()->GetSocketTransform(SocketName);
	FVector spawnLocation = spawnTm.GetRotation().RotateVector(LocationOffset) + spawnTm.GetLocation();

	if (TargetActor && TargetActor->IsValidLowLevel())
	{
		UCapsuleComponent* targetCapsule = TargetActor->GetCapsuleComponent();
		outTargetSize = targetCapsule->GetScaledCapsuleRadius();
		FVector chapsuleSize(outTargetSize, 0.f, targetCapsule->GetScaledCapsuleHalfHeight());
		// Miss 결과값은 충돌 이벤트 바인딩 처리 하지 않음
		outInfo.IsHit = !IsMiss;

		// 타겟의 유도체 목표지점 설정
		// 소켓정보를 외부에서 얻어와야할수도 있다.			
		FVector targetPosition = TargetActor->GetTargetHitPoint(OwnerActor->GetActorLocation(),
			OwnerActor->GetActorRotation().Vector(), TEXT("HitEffectSocket"), IsMiss, chapsuleSize);
		
		FVector dir = targetPosition - spawnLocation;
		// 미스의 경우
		if (_type != EGsHomingHitParticleType::RETURN && IsMiss)
		{
			targetPosition = dir * 1.3f + spawnLocation;
		}

		outInfo.GoalActor = TargetActor;
		outInfo.GoalPos = targetPosition;
		outInfo.SpawnTm = FTransform(spawnTm.GetRotation() * FQuat(RotationOffset), spawnLocation);
	}
	else
	{
		// 인게임에선 타겟을못찾았을 경우를 확실히 인지 시켜주기 위해 디버깅용으로 처리하자
		FRotator ownerRot = OwnerActor->GetActorRotation();
		float randYaw = FMath::RandBool() ? -1.f : 1.f;
		ownerRot += FRotator(1.f, 1.f, 45.f * randYaw);
		FVector dir = ownerRot.Vector().GetSafeNormal();
		outInfo.SpawnTm = FTransform(spawnTm.GetRotation() * FQuat(RotationOffset), spawnLocation);
		outInfo.GoalPos = dir * _moveSpeed + spawnLocation;
	}
}

void UAnimNotifyState_HomingProjectile::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (nullptr == PSTemplate || Animation == nullptr)
	{
		return;
	}

	// Super::NotifyBegin(in_MeshComp, in_Animation, in_TotalDuration);	
	AActor* ownerActor = MeshComp->GetOwner();
	if (false == ownerActor->IsValidLowLevel())
	{
		return;
	}
	
	_effectComponent = ownerActor->FindComponentByClass<UGsHommingProjectileComponent>();
	if (nullptr == _effectComponent)
	{
		_effectComponent = NewObject<UGsHommingProjectileComponent>(MeshComp->GetOwner());
		ownerActor->CreateComponentFromTemplate(_effectComponent);
		_effectComponent->RegisterComponent();
	}

	FGsHomingTargetInfo newInfo;
	newInfo.HitOffsetTm = FTransform(_hitEffectData.RotationOffset, _hitEffectData.LocationOffset);
	newInfo.EventId = _skillNotifyId;
	
	_listTargetInfo.Empty();
	bool isMiss = false;
	float targetSize = 88.f;

	// 캐스터에게서 타겟 정보를 얻어와야한다.
	// Sequence View Editor에서는 다른곳에서 얻어와야한다.
	if (MeshComp->GetWorld()->IsGameWorld())
	{
		AGsCharacterBase* characterActor = Cast<AGsCharacterBase>(ownerActor);
		if (nullptr == characterActor)
		{
			return;
		}

		UGsHommingProjectileComponent* hommingComponent = characterActor->GetHommingProjectile();
		if (nullptr == hommingComponent)
		{
			return;
		}

		// 선판정 대상 정보가 있을때
		if (hommingComponent->_mapHomingProjectileInfo.Num() > 0)
		{
			for (TPair<AGsCharacterBase*, HitResultType> pair : hommingComponent->_mapHomingProjectileInfo)
			{
				NewHommingTargetInfo(newInfo, targetSize, characterActor, pair.Key, pair.Value == HitResultType::DODGE);
				_listTargetInfo.Emplace(newInfo);
			}
		}
		// 대상 결과에 대한 정보가 없다면 선판정 스킬이 아닌경우,
		// 무조건 대상을 목적지로 설정
		else 
		{
			for (AGsCharacterBase* el : hommingComponent->GetTargetActor())
			{
				NewHommingTargetInfo(newInfo, targetSize, characterActor, el, false);
				_listTargetInfo.Emplace(newInfo);
			}
		}
	}
#if WITH_EDITOR
	//  Sequence View Editor 
	else
	{
		if (false == _delegatehandle.IsValid())
		{
			_delegatehandle = _effectComponent->OnEditorViewHitHomingProjectile.AddLambda([this, Animation, ownerActor](FVector HitLocation)
				{
					OnParticleCollide(Animation, ownerActor, HitLocation);
				});
		}

		if (nullptr == _globalConstant)
		{
			// 기본 피격 정보 불러오기
			_globalConstant = LoadObject<UGsGlobalConstant>(
				nullptr, TEXT("/Game/Data/GlobalConstant/GlobalConstant.GlobalConstant"));
		}

		if (nullptr == _curveFloat)
		{
			// 기본 RimLight효과 커브 데이터 불러오기
			_curveFloat = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/Data/CH/RimLightCurve.RimLightCurve"));
		}

		const FTransform& spawnTm = MeshComp->GetSocketTransform(SocketName);
		FVector spawnLocation = spawnTm.GetRotation().RotateVector(LocationOffset) + spawnTm.GetLocation();
		isMiss = false;

		// 대상 찾음 타겟 여러개일경우 추가 작업이 필요할수 있음..
		if (Animation->SpawnBPData.Num() > 0 && Animation->SpawnBPDataRendering)
		{
			for (int i = 0; i < Animation->SpawnBPData.Num(); ++i)
			{
				if (false == Animation->SpawnBPData[i].SpawnActor->IsValidLowLevel())
				{
					continue;
				}

				// 모든 스폰 액터 기본 Idle 모션으로 리셋
				if (AGsCharacterBase* castActor = Cast<AGsCharacterBase>(Animation->SpawnBPData[i].SpawnActor))
				{
					if (UGsAnimInstanceState* anim = castActor->GetAnim())
					{
						anim->ChangeState((uint8)EGsStateBase::Idle);
					}
				}

				// 가장 첫번째 요소의 Hit 정보 설정
				if (i == 0)
				{
					UGsHitEffectData* effectInfo = _globalConstant->HitEffectData;
					const FGsSchemaSoundResData* soundInfo = _globalConstant->HitSoundData.GetRow();
					AGsCharacterBase* castActor = Cast<AGsCharacterBase>(Animation->SpawnBPData[i].SpawnActor);
					UGsHitEffectComponent* hitComponent = castActor->GetHitEffect();
					hitComponent->OverrideData(effectInfo, soundInfo, 0.f, 0.f);
				}
				
				const FVector& targetPos = Animation->SpawnBPData[i].SpawnActor->GetActorLocation();
				FVector dir = (targetPos - spawnLocation);
				newInfo.SpawnTm = FTransform(spawnTm.GetRotation() * FQuat(RotationOffset), spawnLocation);
				newInfo.GoalActor = Animation->SpawnBPData[i].SpawnActor;
				newInfo.GoalPos = targetPos;
			}
		}
		else
		{
			// Sequence View Editor 전방 Y축으로 설정
			FVector dir = FVector::YAxisVector;
			newInfo.SpawnTm = FTransform(spawnTm.GetRotation() * FQuat(RotationOffset), spawnLocation);
			newInfo.GoalPos = dir.GetSafeNormal() * _moveSpeed + spawnLocation;
		}

		newInfo.IsHit = !isMiss;
		_listTargetInfo.Emplace(newInfo);
	}
#endif	
	for (const FGsHomingTargetInfo& el : _listTargetInfo)
	{
		FRotator targetRot = FRotator::ZeroRotator;
		if (RotationOffset.IsNearlyZero())
		{
			FVector dir = (el.GoalPos - el.SpawnTm.GetLocation());
			targetRot = dir.Rotation();
			// 파티클 경우 Y축 전방을 기준으로 제작되어 있어, 내부적으로 X축 전방으로 회전 처리 변경
			// FRotator에서 Roll 회전 처리를 할수 없기 때문에 Pitch, Roll 회전을 바꿔서 처리해준다.
			targetRot -= FRotator(targetRot.Pitch, 90.f, targetRot.Pitch);
		}

		_effectComponent->OnHomingProjectileEffectActive(_type, PSTemplate, el, targetRot,
			_moveSpeed, targetSize, _curveData, _hitEffectData.PSTemplate,
			_hitEffectData.UserCharacterOffset, _PSReturnTemplate);
	}

	Received_NotifyBegin(MeshComp, Animation, TotalDuration);
}
		
void UAnimNotifyState_HomingProjectile::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

#if WITH_EDITOR
	// DrawDebugPoint(MeshComp->GetWorld(), _targetPosition, 20.f, FColor::Yellow, false, 1.f);
	// DrawDebugPoint(MeshComp->GetWorld(), _currPosition, 20.f, FColor::Red, false, 2.f);		
	//  Sequence View Editor 
	if (false == MeshComp->GetWorld()->IsGameWorld())
	{
		if (_hitTargetActor)
		{
			_animIdelStateTimer -= FrameDeltaTime;
			if (_animIdelStateTimer <= 0.f)
			{
				AGsCharacterBase* castActor = Cast<AGsCharacterBase>(_hitTargetActor);
				if (UGsAnimInstanceState* anim = castActor->GetAnim())
				{
					anim->ChangeState((uint8)EGsStateBase::Idle);
				}
				_hitTargetActor = nullptr;
			}
		}
	}
#endif
}

void UAnimNotifyState_HomingProjectile::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	_listTargetInfo.Empty();
#if WITH_EDITOR
	if (_delegatehandle.IsValid())
	{
		_effectComponent->OnEditorViewHitHomingProjectile.Remove(_delegatehandle);
		_delegatehandle.Reset();
	}
#endif

	Received_NotifyEnd(MeshComp, Animation);
}

void UAnimNotifyState_HomingProjectile::OnParticleCollide(UAnimSequenceBase* Animation, AActor* CasterActor, const FVector& HitLocation)
{
	if (_hitEffectData.PSTemplate)
	{
		// 발사체 히트 이펙트 출력
		UGameplayStatics::SpawnEmitterAtLocation(
			CasterActor->GetWorld(),
			_hitEffectData.PSTemplate,
			HitLocation + _hitEffectData.LocationOffset,
			_hitEffectData.RotationOffset,
			true, EPSCPoolMethod::AutoRelease);

#if WITH_EDITOR
		// Sequence View Editor
		if (false == CasterActor->GetWorld()->IsGameWorld())
		{
			if (Animation->SpawnBPDataRendering)
			{
				float distance = 0.f;
				for (FSpawnBP& spawnBp : Animation->SpawnBPData)
				{
					AActor* spawnActor = spawnBp.SpawnActor;
					if (nullptr == spawnActor || false == spawnActor->IsValidLowLevel())
					{
						continue;
					}
					
					if (nullptr == _hitTargetActor || 
						distance > (spawnActor->GetActorLocation() - HitLocation).SizeSquared())
					{
						_hitTargetActor = spawnActor;
					}
					distance = (spawnActor->GetActorLocation() - HitLocation).SizeSquared();
				}
			}

			if (_hitTargetActor)
			{
				AGsCharacterBase* targetCastActor = Cast<AGsCharacterBase>(_hitTargetActor);
				UGsMeshShaderControlComponent* meshShaderComponent = targetCastActor->GetMeshShaderComponent();
				meshShaderComponent->Add(EGsMeshShaderEffect::Fresnel, targetCastActor->GetMesh(), _curveFloat);
				meshShaderComponent->Start();

				targetCastActor->OnHit(HitResultType::NORMAL, EGsStateBase::Idle, nullptr);
				targetCastActor->OnHitEffect(CasterActor->GetActorLocation(), HitResultType::NORMAL,
					nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

				UGsHitEffectComponent* hitComponent = targetCastActor->GetHitEffect();
				hitComponent->OnHitEffect(CasterActor->GetActorLocation());
			}
		}
#endif
	}
}