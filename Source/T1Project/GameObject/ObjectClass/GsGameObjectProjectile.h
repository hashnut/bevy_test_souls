// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsGameObjectBase.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "../Trigger/GsTriggerEventSkillCollision.h"
#include "GsGameObjectProjectile.generated.h"

class FGsMovementHandlerBase;
/**
 * 발사체 클래스
 */
UCLASS()
class T1PROJECT_API UGsGameObjectProjectile : public UGsGameObjectBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = GameObject, Meta = (AllowPrivateAccess = true))
	class AGsActorProjectile* _actor;

	int64 _casterGameId = 0;

	FVector _spawnPos;

	const struct FGsSchemaProjectileSet* _projectileData = nullptr;
	TSharedPtr<FGsTriggerEventSkillCollision> _triggerEvent;

public:
	virtual ~UGsGameObjectProjectile();

	virtual void Initialize(const struct FGsSpawnParam* SpawnParam) override;
	virtual void CreateSubClass() override;

public:
	virtual void ActorDespawn() override;

protected:
	virtual void InitializeActor(AActor* Owner) override;

protected:
	virtual TSharedPtr<FGsMovementHandlerBase> CreateMovement();

protected:
	void SetCasterActor(UGsGameObjectBase* Caster = nullptr);

public:
	// 프로젝타일 발사 주체자 설정
	void SetCaster(UGsGameObjectBase* Caster);

	// 네트워크 응답 처리
public:
	void NetSkillDamagedAck(PD::SC::PKT_SC_SKILL_DAMAGED_READ* Packet);

	// 프로젝타일 충돌 Delegate
	UFUNCTION()
	void ComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);

public:
    virtual EGsGameObjectType GetObjectType() const override;

public:
	virtual class AActor* GetActor() const override;
	virtual void ClearActor() override;
	class AGsActorProjectile* GetProjectile() const;
	int64 GetCasterId() const;
	//virtual FActorDeleteSignature* GetDeleteSignature() const override;

public:
	const struct FGsSchemaProjectileSet* GetProjectileData() const { return _projectileData; }

public:
	const int64 GetDmgValue();
};
