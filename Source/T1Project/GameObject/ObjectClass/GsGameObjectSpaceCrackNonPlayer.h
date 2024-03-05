// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsGameObjectNonPlayer.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"
#include "../Interaction/GsGameObjectInteractionInterface.h"
#include "GsGameObjectSpaceCrackNonPlayer.generated.h"

/**
 * 
 */

class UCapsuleComponent;

UCLASS()
class T1PROJECT_API UGsGameObjectSpaceCrackNonPlayer final : public UGsGameObjectNonPlayer
{
	GENERATED_BODY()

public:
	virtual ~UGsGameObjectSpaceCrackNonPlayer() = default;

public:
	virtual void Initialize(const FGsSpawnParam* SpawnParam) override;
	virtual void InitializeActor(AActor* inOwner) override;	
	virtual void ActorDespawn() override;
	virtual EGsGameObjectType GetObjectType() const override { return EGsGameObjectType::SpaceCrackNonPlayer; }
	virtual bool OnHit(UGsGameObjectBase* Striker, UGsGameObjectBase* SubStriker, uint32 StriketSkillId, 
		uint32 StrikerNotifyId, HitResultType ResultType, DamageByType DamageType, DamageStatType DamageStat,
		HealByType HealType, int Damage, PassivityOwnerType PassivityOwner, bool IsNoDamage) override;

protected:
	virtual void InitializeFSM() override;

private:
	UFUNCTION()
		void OnBeginOverlapActor(UPrimitiveComponent* inHitComponent, AActor* inOtherActor, UPrimitiveComponent* inOtherComp, int32 inOtherBodyIndex, bool inbFromSweep, const FHitResult& inHit);
	void PlayOverlapSound();
};
