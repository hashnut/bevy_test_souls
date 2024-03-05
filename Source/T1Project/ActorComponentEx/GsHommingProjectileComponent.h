// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Animation/AnimNotify/AnimNotifyState_HomingProjectile.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "GsHommingProjectileComponent.generated.h"

// 유도체 관리 구조체
USTRUCT()
struct FGsHomingProjectileEffectInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	class UParticleSystemComponent* _particleSystemComponent = nullptr;
	UPROPERTY()
	class UParticleSystem* _returnParticle = nullptr;
	UPROPERTY()
	class UParticleSystem* _hitParticleTemplate = nullptr;
	UPROPERTY()
	class UCurveVector* _curveData = nullptr;
	UPROPERTY()
	class AActor* _targetActor = nullptr;

	FVector _originPosition;
	FVector _currPosition;
	FVector _targetPosition;
	FVector _velocity;

	float _moveSpeed = 0.f;
	// 시도 횟수
	int _count = 1;
	// NotifyId
	int _eventId = 0;

	// 충돌 영역 설정
	// 타겟의 경우 chapsuleSize
	float _size = 88.f;
	float _currentTime = 0.f;
	float _destTime = 0.f;
	float _destTimeInv = 0.f;
	float _originDistance = 0.f;

	// 적중 또는 미스
	bool _isHit = false;

	bool _isDelete = false;
	bool _isFirst = true;

	FTransform _hitEffectOffsetTm;

	bool _isHitEffectAttackerLookDir;

	void Clear();
};

/**
* 유도체 연출 처리 담당 컴퍼넌트
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class T1PROJECT_API UGsHommingProjectileComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// 원거리 발사체 처리 캐시 정보
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<class AGsCharacterBase*, HitResultType> _mapHomingProjectileInfo;

	// 원거리 발사체에 히트시 발생 이벤트 정보
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHitHomingProjectile, class AActor*, int32)
	FOnHitHomingProjectile OnHitHomingProjectile;

	// Homming Projectile Effect관리
	UPROPERTY()
	TArray<FGsHomingProjectileEffectInfo> _listHomingProjectile;
	bool _isDeleteHomingProjectile = false;

protected:
	// _mapHomingProjectileInfo 대상에 들어갈수 없는 현재 타겟 대상 설정
	TArray<class AGsCharacterBase*> _listTargetActor;
	FScriptDelegate _targetActorDelegate;

public:	
	// Sets default values for this component's properties
	UGsHommingProjectileComponent();
	virtual ~UGsHommingProjectileComponent();

public:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void EndPlay(EEndPlayReason::Type Result) override;

	void ClearResource();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// Homing Projectile 이펙트 등록
	void OnHomingProjectileEffectActive(EGsHomingHitParticleType In_type, class UParticleSystem* In_Particle,
		const struct FGsHomingTargetInfo& In_TargetInfo, const FRotator& In_ParticleRot, float In_MoveSpeed, float In_TargetSize,
		class UCurveVector* In_CurveVector, class UParticleSystem* In_HitParticle, bool In_HitEffectAttackerLookDir,
		class UParticleSystem* In_ReturnParticle = nullptr);

	// 유도체 (AnimNotify HomingProjectile) 적중 이벤트
	virtual void OnHomingProjectileHit(class AActor* Caster, int EventId);

	void ClearHommingProjectileInfo();

	// 유도체 정보 설정 (캐스터의 발사체 정보 설정)
	void SetHommingProjectileInfo(HitResultType inResultType, class AGsCharacterBase* inTarget);
	void SetTargetActor(class AGsCharacterBase* inTarget);
	TArray<class AGsCharacterBase*> GetTargetActor() const;

private:
	void UpdateHomingProjectileEffect(float DeltaTime);
	void UpdateHomingProjectilePosition(OUT FGsHomingProjectileEffectInfo& In_Info, float DeltaTime);
	void ChangeHomingProjectileTarget(OUT FGsHomingProjectileEffectInfo& Out_Info);

	// 고정 위치로 이동: 초기 방향
	void UpdateHomingProjectilePositionFixed(OUT FGsHomingProjectileEffectInfo& In_Info, float DeltaTime);
	// 동적 대상 위치로 이동 : (현재 대상 - 현재 파티클) 방향
	void UpdateHomingProjectilePositionDynamic(OUT FGsHomingProjectileEffectInfo& In_Info, float DeltaTime);
	// 동적 대상 위치로 이동 : (2차 베지어 (현재 파티클, 이전 대상, 현재 대상 위치 사용) - 현재 파티클) 방향
	void UpdateHomingProjectilePositionDynamicBezier(OUT FGsHomingProjectileEffectInfo& In_Info, float DeltaTime);

	void OnHitHomingProjectileEffect(class UParticleSystem* In_Particle, AActor* In_TargetActor,
		const FVector& In_SpawnLocation, const FTransform& In_Offset, float In_Size, bool In_isAttackerLookDir,
		FVector& In_targetDir, int32 In_Id);

	UFUNCTION()
	void OnTargetActorDestroyed(AActor* DestroyedActor);

#if WITH_EDITOR
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnEditorViewHitHomingProjectile, FVector)
	FOnEditorViewHitHomingProjectile OnEditorViewHitHomingProjectile;
#endif
};
