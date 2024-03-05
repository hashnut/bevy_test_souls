#pragma once

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Client/SharedEnums/SharedCombatEnum.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "GsGameObjectPlayableEventLocalPlayer.generated.h"

/*
* 플레이에이블 이벤트 내 플레이어
*/

class AActor;
class AController;

class FGsSkillHandlerBase;
class FGsMovementHandlerBase;
class FGsTargetHandlerBase;
class UGsGameObjectPlayableEventNonPlayer;
class UGsGameObjectBase;
class AGsActorPlayableEventMonsterSpawn;

UCLASS()
class UGsGameObjectPlayableEventLocalPlayer final : public UGsGameObjectLocalPlayer
{
	GENERATED_BODY()


public:
	UGsGameObjectPlayableEventLocalPlayer() = default;
	virtual ~UGsGameObjectPlayableEventLocalPlayer() = default;

public:
	virtual void CreateSubClass() override;

protected:	
	virtual void InitializeActor(AActor* Owner) override;
	virtual TSharedPtr<FGsTargetHandlerBase> CreateTarget() override;
	virtual TSharedPtr<FGsSkillHandlerBase> CreateSkill() override;
	virtual TSharedPtr<FGsMovementHandlerBase> CreateMovement() override;

public:
	virtual void UpdatePartyWeaponChange(CreatureWeaponType CurrWeaponType) override {}
	virtual void ActorSpawned(AActor* Spawn) override;
	virtual void Finalize() override;
	virtual void Update(float Delta) override;
	// 딜스크롤 안뜨게 처리
	virtual void SetDealScroll(const FName& InDealScrollType, const FString& InValue, const FVector& InDir) override {}
	// error log : FGsStatBase::GetStatInfo 198 line
	virtual void ApplyMoveSpeedFromStatInfo() override;

	// 전투 평화 상태
	virtual int UpdateCombatStatus(bool InImmediately = false) override;

	virtual bool CanTarget(UGsGameObjectBase* Target) override;

	virtual bool OnHit(UGsGameObjectBase* Striker, UGsGameObjectBase* SubStriker, uint32 StriketSkillId,
		uint32 StrikerNotifyId, HitResultType ResultType = HitResultType::NORMAL,
		DamageByType DamageType = DamageByType::SKILL, DamageStatType DamageStat = DamageStatType::MAX, 
		HealByType HealType = HealByType::MAX, int Damage = 1, 
		PassivityOwnerType PassivityOwner = PassivityOwnerType::MAX,
		bool IsNoDamage = false) override;
};