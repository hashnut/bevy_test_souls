// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsGameObjectCreature.h"

#include "Quest/GsGameObjectQuestTargetInterface.h"

#include "GsGameObjectNonPlayerBase.generated.h"

/**
 *
 */
UCLASS()
class T1PROJECT_API UGsGameObjectNonPlayerBase : public UGsGameObjectCreature, public IGsGameObjectQuestTargetInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = GameObject, Meta = (AllowPrivateAccess = true))
	class AGsCharacterBase* _actor;
	// 스폰 위치(ai에서 사용할 값)
	FVector _spawnPos;

	//npc
protected:
	const struct FGsSchemaNpcData* _npcData;
	const struct FGsSchemaNpcBalance* _npcBalance;
	float _sightRadius;
	TWeakPtr<FGsSchemaSpawnElement> _spawnInfo;
	/*TWeakObjectPtr<AGsCharacterNonPlayer> _npcActor;*/
	class FGsTargetHandlerLocalPlayer* _targetHandler;

	//quest
protected:
	// 퀘스트 타겟 액티브 되었나
	bool _isQuestTargetActive = false;
	bool _isTargetedByLocalPlayer = false;

	//stat
protected:
	// 스텟변경 핸들러
	FDelegateHandle		_changeStatHandle;

public:
	NpcSpecialType _specialType = NpcSpecialType::NONE;

	bool _isOffensive;

	//state
protected:
	//Death
	FTimerHandle _deathHandle;	

	// 생성자, 소멸자

public:
	virtual ~UGsGameObjectNonPlayerBase() = default;

	// 각종 가상함수
public:
	virtual void Initialize(const struct FGsSpawnParam* SpawnParam) override;
	virtual void InitializeActor(AActor* Owner) override;
	virtual void Update(float inDelta) override;

public:
	virtual void CreateSubClass() override;

	virtual void BindMessage() override;
	virtual void ActorDespawn() override;

	virtual void FindTargetHandler();

protected:	
	virtual void RemoveSubClass() override;

protected:
	virtual void InitializeFSM() override;
	virtual void InitializeMovement();
	void InitializeNameWidget();
	void InitializeHitEffect();
	void InitializeUI();

public:
	void InvalidateQuestMark();
	void InvalidateMiniMap();

protected:
	virtual TSharedPtr<FGsTargetHandlerBase> CreateTarget() override;
	virtual TSharedPtr<FGsSkillHandlerBase> CreateSkill() override;
	virtual TSharedPtr<FGsMovementHandlerBase> CreateMovement() override;
	virtual TSharedPtr<class FGsStatBase> CreateCreatureStat() override;

public:
	virtual bool OnHit(UGsGameObjectBase* Striker, UGsGameObjectBase* SubStriker, uint32 StriketSkillId,
		uint32 StrikerNotifyId, HitResultType ResultType, DamageByType DamageType, DamageStatType DamageStat,
		HealByType HealType, int Damage, PassivityOwnerType PassivityOwner, bool IsNoDamage) override;
	virtual void OnDie() override;

	virtual void UpdateTargetMark(bool inVisibleTargetMaker, bool inIsFirst = true) override;
	virtual void OnTargetClear(const struct IGsMessageParam* inParam) override;

public:
	void WaitUntilDie(FName in_anim);

	// 로직 함수
public:
	// 스폰 위치 저장
	void SaveSpawnPos();
	// 외부값을 저장
	void SaveSpawnPos(const FVector& In_pos) { _spawnPos = In_pos; }

	void DeathHandleReset();

public:
	virtual void AsyncLoadClassComplete(class UObject* inRes) override;

private:
	// void DrawDebugSightRadius();
	//void InvalidDebugWidget();

public:
	/*virtual void InitNpcDebugWidget();*/

	// get, set 함수
public:
	virtual EGsGameObjectType GetObjectType() const override { return EGsGameObjectType::NonPlayerBase; }

public:
	virtual AActor* GetActor() const override;
	virtual void	ClearActor() override { _actor = nullptr; }
	UFUNCTION(BlueprintCallable, Category = "GameObject")
	class AGsCharacterBase* GetCharacterBase() const { return _actor; }

	const struct FGsSchemaNpcData* GetNpcData() const { return _npcData; }
	// npc tbl id 가져오기
	int GetNpcTblId();
	const struct FGsSchemaNpcBalance* GetNpcBalance() const { return _npcBalance; }

public:
	FVector GetSpawnPos() const { return _spawnPos; }
	virtual FVector GetLootTargetPoint(UGsGameObjectBase* LootTarget) const override;

	virtual void LookAt(const FRotator& LookRotator, bool IsSmoothRotation = true) override;

private:
	void OnCheckIsTargetedByLocalPlayer(const struct IGsMessageParam* inParam);
	UFUNCTION()
	void CallbackCognitionTargetEffectFinish(class UParticleSystemComponent* inParticleComp);

	//quest 
public:
	// 클라이언트에서 생성한 오브젝트 정보
	bool IsClientObject() const;

	// 퀘스트 타겟 액티브인지
	virtual void SetQuestTargetActive(bool In_isActive, int In_Index = 0) override;
	virtual bool GetQuestTargetActive() const override { return _isQuestTargetActive; }
	// 퀘스트 마크 액티브인지
	virtual void SetQuestMarkActive(bool In_isActive, int In_typeIndex = 0) override;
	virtual bool IsQuestTarget() override;
	// 공격 가능 거리에서 네비 지형 체크할 필요가 있는가?
	virtual bool IsSkipTerrainCheck() const override;
	// 몬스터 애드 이펙트
	virtual void SetCognitionTargetEffect();
	//	LifeTime이 정해져 있는가
	virtual time_t GetEndOfLifeTime() { return 0; }
	virtual const FString GetOwnerName() { return TEXT(""); }
	virtual bool IsTreasureMonster() { return false; }
	virtual bool IsPlayerTeamTreasureMonster() { return false; }
	
	virtual FText GetNpcName() { return _npcData ? _npcData->nameText : FText::GetEmpty(); }

	void SetOffensive(bool inOffensive) { _isOffensive = inOffensive; }
	bool GetOffensive() { return _isOffensive; }
	//디버그를 위한 시야거리 설정
	void SetSightRadius(float inRange);

public:
	// 말풍선 출력 시스템
	virtual class UGsWidgetComponentTalkBalloon* SetTalkBalloonActive();
	virtual void HideTalkBalloon();

public:
	void Print(UWorld* inWorld);

public:
	// BP에서 간략한 접근을 위해 기본 TM정보 제공
	virtual FVector GetLocation() const override;
	virtual FVector2D GetLocation2D() const override;
	virtual FRotator GetRotation() const override;
	virtual FVector GetScale() const override;
};