// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GsGameObjectSkillPreviewPlayer.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsGameObjectSkillPreviewPlayer final : public UGsGameObjectCreature
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = GameObject, Meta = (AllowPrivateAccess = true))
	class AGsCharacterBase* _actor = nullptr;

	UPROPERTY()
	class UGsHitEffectData* HitEffectDataOverride = nullptr;

	const struct FGsSchemaSoundResData* HitSoundDataOverride = nullptr;

public:
	virtual void Initialize(const struct FGsSpawnParam* SpawnParam) override;
	virtual void InitializeActor(AActor* Owner) override;
	virtual void CreateSubClass() override;

protected:
	virtual TSharedPtr<FGsMovementHandlerBase> CreateMovement() override;
	virtual TSharedPtr<FGsPartsHandlerBase> CreateParts() override;
	virtual TSharedPtr<FGsSkillHandlerBase> CreateSkill() override;
	virtual TSharedPtr<FGsAbnormalityHandlerBase> CreateAbnormality() override;
	virtual class TSharedPtr<FGsStatBase> CreateCreatureStat() override;

	virtual void InitializeFSM() override;

public:
	virtual void NetSkillHealAck(PD::SC::PKT_SC_SKILL_HEALED_READ* Packet) override;
	virtual void NetSkillDamagedAck(PD::SC::PKT_SC_SKILL_DAMAGED_READ* Packet) override;

public:
	virtual class AActor* GetActor() const override;
	virtual class AGsCharacterBase* GetCharacter() const override;
	virtual void ClearActor() override;

	virtual const FGsSchemaShapeData* GetPolymorphShapeData() override;
};
