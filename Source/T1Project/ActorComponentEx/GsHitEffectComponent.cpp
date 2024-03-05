// Fill out your copyright notice in the Description page of Project Settings.
#include "GsHitEffectComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "Sound/GsSchemaSoundResData.h"
#include "Sound/GsSoundPlayer.h"
#include "Management/ScopeGlobal/GsSoundManager.h"

#include "GameObject/Define/HitEffectAttachType.h"
#include "GameObject/Define/HitEffectVisibleType.h"
#include "UTIL/GsGameObjectUtil.h"
// #include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGsHitEffectComponent::UGsHitEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UGsHitEffectComponent::OverrideData(UGsHitEffectData* inData, const FGsSchemaSoundResData* inSoundData,
	float inOverrideEffectSizeRate, float inOverrideModelRadius)
{
	_enable = ValidateData(inData);
	_soundData = inSoundData;
	if (_enable)
	{
		if (FMath::IsNearlyZero(inOverrideEffectSizeRate))
		{
			_overrideEffectScale = 1.f;
		}
		else
		{
			_overrideEffectScale = inOverrideEffectSizeRate;
		}

		if (FMath::IsNearlyZero(inOverrideModelRadius))
		{
			_modelRadius = ExtrectModelRadius();
		}
		else
		{
			_modelRadius = inOverrideModelRadius;
		}

		_enable = FindAttachTarget();		
	}
}

void UGsHitEffectComponent::OnHitSound(const struct FGsSchemaSoundResData* inSoundData, bool inIsCritical, 
	CreatureWeaponType inWeaponType)
{
	const FGsSchemaSoundResData* soundData = (inSoundData) ? inSoundData : _soundData;

	// 사운드 재생
	if (soundData)
	{
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			TArray<TPair<EGsAudioParameterType, float>> playSounds;
			playSounds.Emplace(TPair<EGsAudioParameterType, float>(EGsAudioParameterType::CreatureWeapon, (float)inWeaponType));
			playSounds.Emplace(TPair<EGsAudioParameterType, float>(EGsAudioParameterType::HitCritical,
				(inIsCritical) ? 1.f : 0.f));
			
			soundPlayer->PlaySoundTarget(soundData, GetOwner()->GetRootComponent(),playSounds);
		}
	}
}

void UGsHitEffectComponent::OnHitEffect(const FVector& attackerLocation, UGsHitEffectData* inData, bool inAttackerHeightFixed)
{
	if (false == _enable || GetOwner()->IsHidden())
	{
		return;
	}

	UGsHitEffectData* effectData = (inData) ? inData : _data;
	_effectScale = FVector(effectData->ScaleFactor);

	FVector targetSizeScale = _effectScale * _overrideEffectScale;
// 	if (false == FMath::IsNearlyZero(_overrideEffectScale))
// 	{
// 		targetSizeScale = FVector(_overrideEffectScale);
// 	}
// 	else
// 	{
// 		// _effectScale은 기존 리소스 스케일 정보이며
// 		// 아래는 대상 사이즈에 따라 비율을 다시한번 계산해준다. (https://jira.com2us.com/jira/browse/CHR-11816)
// 		targetSizeScale = _effectScale * UGsGameObjectUtil::GetParticleEffectScale(GetOwner());
// 	}

	// 소켓 정보가 있다면 Tm얻기
	const FTransform ParentToWorld = _attachTarget->GetSocketTransform(*effectData->SocketName);

	FVector dirAttacker = attackerLocation - ParentToWorld.GetLocation();
	dirAttacker = dirAttacker.GetSafeNormal();

	// DrawDebugDirectionalArrow(GetWorld(), ParentToWorld.GetLocation(), attackerLocation, 10.f, FColor::Yellow, false, 5.f);

	FVector relateBondingPos = (dirAttacker * (_modelRadius *
		effectData->BoundMultipleFactor) + ParentToWorld.GetLocation()) + effectData->Offset;
	FRotator relateBondingRot = effectData->SpawnRotator;
	relateBondingRot.Add(0.f, dirAttacker.Rotation().Yaw, 0.f);

	// PC 타격자 기준에서만 PC높이에 맞춘다.
	// https://jira.com2us.com/jira/browse/CHR-18917
	if (inAttackerHeightFixed)
	{
		relateBondingPos.Z = attackerLocation.Z;
	}
	// 최종 Offset Pos 반영
	relateBondingPos += effectData->Offset;

	// 임시 위치정보 찍기
	// DrawDebugSphere(GetWorld(), relateBondingPos, 10.f, 100, FColor::Blue, false, 5.f);

	if (effectData->Attach == HitEffectAttachType::LOCATION)
	{				
		if (UWorld* world = GetWorld())
		{
			TArray<UParticleSystem*> list;
			GetPlayList(list, effectData);
			for (UParticleSystem* particle : list)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					world,
					particle,
					relateBondingPos,
					effectData->SpawnRotator,
					targetSizeScale,
					true,
					EPSCPoolMethod::AutoRelease);
			}
		}		
	}
	else if(effectData->Attach == HitEffectAttachType::SOCKET)
	{
		TArray<UParticleSystem*> list;
		GetPlayList(list, effectData);
		for (UParticleSystem* particle : list)
		{
			UGameplayStatics::SpawnEmitterAttached(
				particle,
				_attachTarget,
				*effectData->SocketName,
				effectData->Offset,
				effectData->SpawnRotator,
				targetSizeScale,
				EAttachLocation::Type::KeepRelativeOffset,
				true,
				EPSCPoolMethod::AutoRelease);
		}
		
	}
// 	else if(effectData->Attach == HitEffectAttachType::SOCKET_BOUNDING_DIRECTION)
// 	{
// 		const FTransform ParentToWorld = _attachTarget->GetSocketTransform(*effectData->SocketName);
// 		FVector dirAttacker = attackerLocation - ParentToWorld.GetLocation();
// 		dirAttacker = dirAttacker.GetSafeNormal();
// 		FVector relateBondingPos = (dirAttacker * (_modelRadius * 
// 			effectData->BoundMultipleFactor) + ParentToWorld.GetLocation()) + effectData->Offset;
// 		FRotator relateBondingRot = effectData->SpawnRotator;
// 		relateBondingRot.Add(0.f, dirAttacker.Rotation().Yaw, 0.f);
// 
// 		TArray<UParticleSystem*> list;
// 		GetPlayList(list, effectData);
// 		for (UParticleSystem* particle : list)
// 		{
// 			UGameplayStatics::SpawnEmitterAtLocation(
// 				GetWorld(),
// 				particle,
// 				relateBondingPos,
// 				relateBondingRot,
// 				targetSizeScale,
// 				true,
// 				EPSCPoolMethod::AutoRelease);
// 		}
// 	}
	else if (effectData->Attach == HitEffectAttachType::TARGET_ROTATION)
	{
		TArray<UParticleSystem*> list;
		GetPlayList(list, effectData);
		
		// Yaw 회전만 적용
		FRotator rot = effectData->SpawnRotator;
		rot.Add(0.f, dirAttacker.Rotation().Yaw, 0.f);
		//FQuat finalQut =/* billboardRot.Quaternion() * */dirAttacker.ToOrientationQuat() * effectData->SpawnRotator.Quaternion();
		//FRotator offsetRot = dirAttacker.Rotation() += effectData->SpawnRotator;
		for (UParticleSystem* particle : list)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				particle,
				relateBondingPos,
				rot,
				targetSizeScale,
				true,
				EPSCPoolMethod::AutoRelease);
		}
	}
	else if (effectData->Attach == HitEffectAttachType::SOCKET_RANDOM_BOUNDING)
	{
		float randX = FMath::FRandRange(-effectData->RandomRadius, effectData->RandomRadius);
		float randZ = FMath::FRandRange(-effectData->RandomRadius, effectData->RandomRadius);
		FVector randPos(randX, 0.f, randZ);

		dirAttacker = attackerLocation - (relateBondingPos + randPos);
		dirAttacker = dirAttacker.GetSafeNormal();
		relateBondingPos = (dirAttacker * _modelRadius + relateBondingPos);

		TArray<UParticleSystem*> list;
		GetPlayList(list, effectData);
		for (UParticleSystem* particle : list)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				particle,
				relateBondingPos,
				effectData->SpawnRotator,
				targetSizeScale,
				true,
				EPSCPoolMethod::AutoRelease);
		}

	}
}

// Called when the game starts
void UGsHitEffectComponent::BeginPlay()
{
	Super::BeginPlay();	
}


bool UGsHitEffectComponent::ValidateData(UGsHitEffectData* inData)
{
	check(inData);
	_data = inData;

	if (_data->ParticleSystems.Num() != 0)
	{
		return true;
	}
	return false;
}

float UGsHitEffectComponent::ExtrectModelRadius()
{
	if (ACharacter* actor = Cast<ACharacter>(GetOwner()))
	{
		if (UCapsuleComponent* capsule = actor->GetCapsuleComponent())
		{
			return (capsule->GetUnscaledCapsuleRadius() * actor->GetRootComponent()->GetComponentScale().X);
		}
	}
	return 1.0f;
}

void UGsHitEffectComponent::GetPlayList(TArray<UParticleSystem*>& outList, UGsHitEffectData* inData) const
{
	if (inData)
	{
		if (inData->Visible == HitEffectVisibleType::ALL)
		{
			outList = inData->ParticleSystems;
		}
		else if (inData->Visible == HitEffectVisibleType::RANDOM)
		{
			int select = FMath::RandRange(0, inData->ParticleSystems.Num() - 1);
			outList.Add(inData->ParticleSystems[select]);
		}
	}
}


bool UGsHitEffectComponent::FindAttachTarget()
{
	if (ACharacter* actor = Cast<ACharacter>(GetOwner()))
	{
		_attachTarget = actor->GetMesh();
		return true;
	}
	return false;
}


// Called every frame
void UGsHitEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

