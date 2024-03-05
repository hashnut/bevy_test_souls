// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsGameObjectBase.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Animation/GsAnimInstanceState.h"
#include "GameObject/Movement/GsMovementRunner.h"
#include "GsGameObjectCreature.generated.h"

namespace PD
{ 
	namespace SC
	{ 
		struct PKT_SC_SKILL_DAMAGED_READ;
		struct PKT_SC_PASSIVITY_DAMAGED_READ;
		struct PKT_SC_SKILL_HEALED_READ;
		struct PKT_SC_ADD_ABNORMALITY_RESULT_READ;
	} 
}

struct FGsSchemaShapeData;

/**
 * 운동 능력이 있는 오브젝트 Base클래스
 * Skill, FSM, Parts, Movment 등의 기능 탑제
 */
UCLASS()
class T1PROJECT_API UGsGameObjectCreature : public UGsGameObjectBase, public FGsMovementRunner
{
	GENERATED_BODY()
	
protected:
	friend class FGsMovementRunner;
	// 최초 스폰 정보
	SPAWN_REASON _spawnReason = SPAWN_REASON::NORMAL;

	const struct FGsSchemaCreatureCommonInfo* _creatureCommonInfo;

protected:
	bool				_clientCollisionRule = false;


protected:
	class FGsGameObjectStateManager* _fsm		= nullptr;
	// 스텟변경 핸들러
	FDelegateHandle		_changeStatHandle;
	// 디스플레이용 스탯 변경 핸들러
	FDelegateHandle		_displayStatHandle;
	// 유도체 히트 이벤트 핸들러
	FDelegateHandle		_homingProjectileHandle;


protected:
	bool					_targetMakerVisible = false;

	struct BillboardCreatureInfo
	{
		bool isTarget = false;
		bool isFirstTarget = false;
		bool isEnemy = false;
	};
	BillboardCreatureInfo	_targetInfo;

	// 연출용 이동 설정, 서버에서는 Teleport된 상태
	struct FGsMoveLocation
	{
		FVector2D _stratPos = FVector2D::ZeroVector;
		FVector2D _destPos = FVector2D::ZeroVector;
		float _duration = 0.f;
		float _elapsed = 0.f;
		bool _isUpdate = false;
		bool IsValid()
		{
			return _isUpdate;
		}
	} _moveLocationInfo;

	// 루트 모션 이동 설정
	struct FRootmotionInfo
	{
		// 루트모션 활성화 정보 서버에서 응답을 받을때까지의 판단을 위해 정보를 저장해둔다.
		const struct FGsSchemaRootmotionSet* _rootmotionTable = nullptr;
		// 루트 모션 계산 적용 처리 플레그
		bool _isRootmotionUpdate = false;

		// 외부 요인에 의해 강제 루트모션 처리 막음
		bool _rootmotionLock = false;
		// 외부 요인에 의해 네비데이터 검사를 스킵한다.
		bool _rootmotionIgnoreNavData = false;
		// 루트모션 TransForm 정보
		FTransform _rootmotionTm;
		// 서버에서 알려준 최종 목적 좌표
		FVector _destPos;

		// 캐릭터 충돌 처리용 캐시 정보
		FCollisionQueryParams _collisionQueryParam;
		FCollisionObjectQueryParams _collisionObjectQueryParam;
		FCollisionShape _collisionShape;

	} _rootmotionInfo;

public:
	UGsGameObjectCreature();
	virtual ~UGsGameObjectCreature();

	virtual void Initialize(const struct FGsSpawnParam* SpawnParam) override;
	virtual void Update(float Delta) override;
	virtual void CreateSubClass() override;

protected:
	virtual void InitializeActor(class AActor* Owner) override;	
	virtual void RemoveSubClass() override;

protected:
	virtual class FGsGameObjectData* CreateData(const struct FGsSpawnParam* SpawnParam) override;
	virtual TSharedPtr<class FGsTargetHandlerBase>	CreateTarget();
	virtual class FGsGameObjectStateManager* CreateBaseFSM();
	virtual TSharedPtr<class FGsSkillHandlerBase> CreateSkill();
	virtual TSharedPtr<class FGsPartsHandlerBase> CreateParts();
	virtual TSharedPtr<class FGsMovementHandlerBase> CreateMovement();
	virtual TSharedPtr<class FGsCustomizeHandler> CreateCustomize();
	virtual TSharedPtr<class FGsAbnormalityHandlerBase> CreateAbnormality();
	virtual TSharedPtr<class FGsStatBase> CreateCreatureStat();
	virtual TSharedPtr<class FGsDamageHandlerBase> CreateDamageHandler();

	TSharedPtr<class FGsUseItemEffectHandler> CreateUseItemEffectHandler();
protected:
	virtual void BindMessage() override;
	virtual void OnTargetClear(const struct IGsMessageParam* inParam);

public:
	virtual void UpdateByTarget(bool inVisibleTargetMaker, bool isTarget, bool inFirst, bool isEnemy);
	virtual void UpdateTargetMark(bool inVisible, bool inIsFirst = true);

	// 강제 연출 이동 처리
	// FGsSchemaSkillNotifyMoveLocation 구조체를 기반으로 이동 거리와 위치를 정하여 이동
	// LocalPlayer는 이동 업데이트가 한정적이어서 GameObject클래스에서 처리
	virtual void StartMoveLocation(const FVector& inCurrPos, const FVector& inDestPos, int inMoveSpeed);
	virtual void StartMoveLocation(const FVector& inCurrPos, const FVector& inDestPos, float inDuration);

#if UE_EDITOR
	// 루트모션 동기화 디버그용
	FVector _debugRootmotionStartPos;
	FDateTime _debugRootmotioncurrentTime;
#endif
	// 루트 모션 이동 정보 설정
	virtual void StartRootmotion(uint16 inDir, const FVector& inPos, const FVector& inDestPos, int inRootmotionId);
	virtual void EndRootmotion(uint16 inDir, const FVector& inPos, int inRootmotionId);
	virtual void ClearRootmotionData();


	//스텟 변경관련 처리추가
protected:
	virtual void OnChangedStat(const class FGsStatInfo& inStateInfo);
	virtual void OnChandeDisplayStat(const class FGsStatInfo& inStateInfo);
	virtual void OnHitHomingProjectile(class AActor* Caster, int32 SkillNotifyId);

	// 서버는 즉시 위치 갱신 되었지만, 일정 시간(또는 거리)동안 클라이언트에서 이동 연출 처리
	virtual void ClientMoveUpdate(float inDelta);
	// 루트 모션 위치 처리
	virtual void UpdateRootmotion(float inDelta);
	
	// 네트워크 응답 처리
public:
	virtual void NetSkillDamagedAck(PD::SC::PKT_SC_SKILL_DAMAGED_READ* Packet);
	virtual void NetSkillDamagedAck(PD::SC::PKT_SC_PASSIVITY_DAMAGED_READ* Packet);
	virtual void NetSkillHealAck(PD::SC::PKT_SC_SKILL_HEALED_READ* Packet);
	virtual void NetAbnormalityResultAck(PD::SC::PKT_SC_ADD_ABNORMALITY_RESULT_READ* Packet);

public:	 
	virtual void OnDie() override;
	virtual bool OnHit(UGsGameObjectBase* Striker, UGsGameObjectBase* SubStriker, uint32 StrikerSkillId, uint32 StrikerNotifyId,
		HitResultType ResultType = HitResultType::NORMAL, 
		DamageByType DamageType = DamageByType::SKILL, 
		DamageStatType DamageStat = DamageStatType::MAX,
		HealByType HealType = HealByType::MAX,
		int Damage = 1,
		PassivityOwnerType PassivityOwner = PassivityOwnerType::MAX,
		bool IsNoDamage = false) override;
	virtual void OnStiffen(const FVector& StiffenPos, float fDuration) {};

	// 머터리얼 변경
	virtual void OnOverlayMaterialChange(class UGsGameObjectMaterialData* In_Data);

	//매쉬 변경
	virtual void OnPolymorpChange();
	virtual void OnRestorePolymorph();

	// Vehicle 연출
	virtual void OnVehicleEffect(bool useTalk = true);
	virtual void OnRestoreVehicleEffect();

	virtual const FGsSchemaShapeData* GetPolymorphShapeData(){ return nullptr; }

public:
	virtual int UpdateCombatStatus(bool InImmediately = false);
	virtual void SetDealScroll(const FName& InDealScrollType, const FString& InValue, const FVector& InDir);
	virtual void SetEmoticon(EGsEmoticonType InType);
	// 상황에 따른 Idle타입을 판단하여 AnimState에 전달
	virtual void ChangeIdleType(EGsAnimStateIdleType Type);

public:
	virtual EGsGameObjectType GetObjectType() const override;
	virtual const struct FGsSchemaCreatureCommonInfo* GetCommonInfoTable() const override { return _creatureCommonInfo; }

public:
	virtual class AActor* GetActor() const override;
	virtual class AGsCharacterBase* GetCharacter() const override;
	virtual class UGsAnimInstanceState* GetAnimInstance() const override;
	//virtual FActorDeleteSignature* GetDeleteSignature() const override;

public:
	virtual class FGsGameObjectStateManager* GetBaseFSM() const override			{ return _fsm; }

	template <typename T = class FGsSkillHandlerBase>
	T* GetCastSkill() 
	{
		return FGsClassUtil::FCheckStaticCast<T>(GetSkillHandler());
	}
	template <typename T = class FGsPartsHandlerBase>
	T* GetCastParts() 
	{
		return FGsClassUtil::FCheckStaticCast<T>(GetPartsHandler());
	}
	template <typename T = class FGsMovementHandlerBase>
	T* GetCastMovement() 
	{
		return FGsClassUtil::FCheckStaticCast<T>(GetMovementHandler());
	}

public:
	virtual const class FGsStatInfo* GetCreatureStatInfoByType(StatType InStatType) const;
	//virtual void SetCreatureStatInfoByType(StatType InStatType, int32 InValue);
	virtual class FGsGameObjectDataCreature* GetGameObjectDataCreature() const;

public:
	virtual bool IsCrowdControlLock() override;
	virtual bool IsCrowdControlLock(CreatureActionType creatureActionType) override;
	
	virtual bool IsAble() override;
	virtual bool IsZeroHP() const override;

	// visible 상태인지 갱신
	virtual void UpdateVisible() override;

	virtual bool IsTargetedAvailableState();

public:	
	bool IsDeadState() const;
	bool IsBattleMode() const;
	EGsStateBase GetStateType() const;
	virtual void PrintStateType() const;

	virtual bool IsPeacefulIdle() override;

	virtual void ApplyMoveSpeedFromStatInfo();

public:
	// 루트모션 중인가?
	bool IsRootmotion(bool isUpdate = false) const;
	// 외부 요인에 의해 강제 루트모션 처리 막음
	void SetRootmotionLock(bool inIsLock);
	// 외부 요인에 의해 네비데이터 검사를 스킵한다.
	void SetRootmotionIgnoreNavData(bool inIsIgnore);

public:
	
	void ApplyWalkSpeedFromCommonInfo();

public:
	void ActiveGravity(bool inActive);
	class FGsUseItemEffectHandler* GetUseItemEffectHandler();	

#if WITH_EDITOR
	virtual void DebugAddStateLog(const FString& inStateMsg) override;
#endif
};
