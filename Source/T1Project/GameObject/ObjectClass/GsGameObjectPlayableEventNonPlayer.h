#pragma once

#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"

#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Client/SharedEnums/SharedCombatEnum.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"

#include "GsGameObjectPlayableEventNonPlayer.generated.h"

class AActor;

class FGsSkillHandlerBase;
class FGsPlayableEventHandler;
class UGsGameObjectBase;
class AGsActorPlayableEventMonsterSpawn;

struct DamageStatInfo;

UCLASS()
class UGsGameObjectPlayableEventNonPlayer final : public UGsGameObjectNonPlayer
{
	GENERATED_BODY()

private:
	AGsActorPlayableEventMonsterSpawn* _spawnActor;

	FTimerHandle _despawnHandle;

	int _hitCount = 0;
	int _deathHitCount = 0;

public:
	UGsGameObjectPlayableEventNonPlayer() = default;
	virtual ~UGsGameObjectPlayableEventNonPlayer() = default;

public:
	virtual void CreateSubClass() override;

	virtual void Initialize(const struct FGsSpawnParam* SpawnParam) override;
	virtual void ActorDespawn() override;
protected:
	virtual void InitializeActor(AActor* Owner) override;	
	virtual TSharedPtr<FGsSkillHandlerBase> CreateSkill() override;
	virtual void InitializeMovement() override;
	virtual void RemoveSubClass() override;
public:
	virtual void Update(float Delta) override;

	virtual bool OnHit(UGsGameObjectBase* Striker, UGsGameObjectBase* SubStriker, uint32 StrikerSkillId, uint32 StrikerNotifyId,
		HitResultType ResultType = HitResultType::NORMAL, DamageByType DamageType = DamageByType::SKILL,
		DamageStatType DamageStat = DamageStatType::MAX, HealByType HealType = HealByType::MAX,  int Damage = 1,
		PassivityOwnerType PassivityOwner = PassivityOwnerType::MAX,
		bool IsNoDamage = false) override;

	// 딜스크롤 안뜨게 처리
	virtual void SetDealScroll(const FName& InDealScrollType, const FString& InValue, const FVector& InDir) override {}
	virtual void FindTargetHandler() override{}
	/*virtual void InitNpcDebugWidget() override {}*/

	// logic func
public:
	// 퀘스트 킬카우트 추가 처리
	void AddQuestKillCount(int In_tblId);
	void RequestRespawn();
	void DespawnHadleReset();

	void OnDeath();
	void OnDespawn();

	void MakeSpawnEffect();

	// set, get
public:
	void SetSpawnActor(AGsActorPlayableEventMonsterSpawn* In_actor)
	{
		_spawnActor = In_actor;
	}
	FGsPlayableEventHandler* GetPlayableEventHandler();
};