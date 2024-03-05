// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Classes/Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "GsCharacterBase.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifySkillGuide.h"
#include "Sound/GsSchemaSoundResData.h"
#include "GsActorProjectile.generated.h"

// 프로젝타일 임의 조작 파티클 정보
USTRUCT(BlueprintType)
struct T1PROJECT_API FGsProjectileParticleInfo
{
	GENERATED_BODY()
	
	// 파티클 리소스 정보
	UPROPERTY(EditAnywhere, Category = "ProjectileEffectData")
	class UParticleSystem* _particle = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "ProjectileEffectData")
	class UParticleSystemComponent* _playParticleComponent = nullptr;

	// 파티클 Delay 시간 설정 (초)
	UPROPERTY(EditAnywhere, Category = "ProjectileEffectData", Meta = (Tooltip = "스폰 딜레이 시간"))
	float _delay = 0.f;

	UPROPERTY(EditAnywhere, Category = "ProjectileEffectData", Meta = (Tooltip = "디스폰 딜레이 시간 (스폰이후 부터)"))
	float _despawnDelay = 0.f;

	// Offset TM
	UPROPERTY(EditAnywhere, Category = "ProjectileEffectData")
	FTransform _transform;

	FTimerHandle _timer;
};

// 발사체 효과
USTRUCT(BlueprintType)
struct T1PROJECT_API FGsProjectileEffectData
{
	GENERATED_BODY()
	
	// 기본 사운드
	UPROPERTY(EditAnywhere, Category = "ProjectileEffectData", Meta = (Tooltip = "예시선 정보를 설정할경우는 skillGuideSoundRes 설정으로 진행"))
	FGsSchemaSoundResDataRow _soundRes;

	// 예시선 사운드
	UPROPERTY(EditAnywhere, Category = "ProjectileEffectData", Meta = (Tooltip = "overrideCollisionInfo 설정이 되어있다면 기본 사운드를 무시하고 override"))
	FGsSchemaSoundResDataRow _skillGuideSoundRes;

	// 히트시 발생 사운드 정보
	UPROPERTY(EditAnywhere, Category = "ProjectileEffectData", Meta = (Tooltip = "프로젝타일 능력으로 히트 발생시 사운드"))
	FGsSchemaSoundResDataRow _hitSoundRes;

	// 사용자 정의 파티클 정보
	UPROPERTY(EditAnywhere, Category = "ProjectileEffectData")
	TArray<FGsProjectileParticleInfo> _particlePlayInfo;

	// 히트시 발생 파티클 정보
	UPROPERTY(EditAnywhere, Category = "ProjectileEffectData")
	TArray<class UParticleSystem*> _hitParticle;
};

/**
* 발사체 처리 AActor 클래스
* 현재는 (StaticMesh or SkeletalMesh), 내부 로직으로 생성하고 있지 않는다(BP에서 진행)
* 추후 관련 내용 협의 필요
*/
UCLASS()
class T1PROJECT_API AGsActorProjectile : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* _projectileMovement = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* _modelRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FGsProjectileEffectData _effectData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FGsSchemaSkillNotifySkillGuide _guideData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UGsSkillGuideEffectComponent* _guideEffectComponent = nullptr;
		
	TArray<class USceneComponent*> _listMesh;

	FTimerHandle		_delayTimer;

public:
	// 서버 응답으로 Despawn 인가 확인 플레그
	bool _isNetDespawn = false;

public:
	UPROPERTY(BlueprintAssignable, Category="Game")
	FActorDeleteSignature OnActorDelete;
	
public:	
	// Sets default values for this actor's properties
	AGsActorProjectile();

public:
	virtual void FinishDestroy() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void PostInitializeComponents() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void PlayHitEffect();

public:
	void SetProjectileSpeed(float Speed);

protected:
	void InitializeGuideData();

	void PlayParticle(FGsProjectileParticleInfo& ParticleInfo);
	void PlayGuideEffect();

	void DeactiveParticle(FGsProjectileParticleInfo& ParticleInfo);

protected:
	UFUNCTION()
	void OnGuideEffectTimer();

public:
	class UProjectileMovementComponent* GetMoveComponent() const { return _projectileMovement; }

//public:
//	USphereComponent* GetSphereComponent() const { return _sphereComponent; }
};
