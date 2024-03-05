// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsGameObjectPlayer.h"
#include "Message/GsMessageStage.h"
#include "Message/GsMessageFairy.h"
#include "T1Project.h"
#include "GsGameObjectLocalPlayer.generated.h"


namespace PD
{
	namespace SC
	{
		struct PKT_SC_ITEM_EQUIP_CHANGE_WEAPON_TYPE_FINISHED_READ;
	}
}


class FGsMovementHandlerRemote;
using FGsMovementHandlerSpectatorLocalPlayer = FGsMovementHandlerRemote;

/**
 * 내플레이어
 */
UCLASS()
class T1PROJECT_API UGsGameObjectLocalPlayer  : public UGsGameObjectPlayer
{
	GENERATED_BODY()

public:
	enum class EMovementType
	{
		LocalPlayer,		// 2021/10/19 PKT - 일반적인 상황
		Spectator,	// 2021/10/19 PKT - 오프라인 모드 등으로 인한 관중(모니터링) 모드
	};

protected:
	// 페어리 탈것 효과 검증 및 처리 구조체
	struct FairyVehicleInfo
	{
		// 페어리 탈것 효과 검증 플레그 (EGsFairyVehicleCondition)
		uint8 _condition;
		FTimerHandle _timerHandle;

		void StartTimer(class UWorld* inWorld, float inDelayTime = 0.f);
		void ClearTimer(class UWorld* inWorld);
		void Clear(class UWorld* inWorld);
		void Send();

		bool IsSuccess() const;

	} _fairyVehicleInfo;

	// 강제 접속 종료 처리 구조체
	struct ForceDisconnect
	{
		bool _isSafeZone = false;
		bool _isIdle = false;
		float _timer = 0.f;
		float _elapsed = 0.f;

		void ClearTimer();
		bool IsActive() const;

	} _forceDisconnect;	

#if WITH_EDITOR
public:
	bool showDebug = false;
#endif

private:	
	// 예약 관련 처리 모음
	TArray<class IGsReserveWork*> _arrayReserveWork;

	// 적대 관련 처리를 서버로직과 맞추기 위해 IffFactionHellper 구조체 사용
	SharedFunc::IffFactionHellper _netSyncIffhellper;

	// 로컬 플레이어 전용 화면 효과 제어 클래스
	class FGsScreenFilterLocalPlayer* _screenFiler = nullptr;

	FTimerHandle _timeCostumeEffect;

	int64 _ownTreasureGameId{ 0 };

	TArray<MsgStageHandle> _msgStageHandleList;

	using MsgFairyHandle = TPair<MessageFairy, FDelegateHandle>;
	TArray<MsgFairyHandle> _msgFairyHandleList;
	
	//quest npc 중복 체크하기 위한 컨테이너
	TArray<TWeakObjectPtr<class AActor>> _questNpcArray;

	bool _serverCollisionRule = true;
	bool _localCollisionRule = true;	

	// 루트 모션의 길이가 매우짧아 (ex 0.1초) 서버에서 Start(시작)와 동시에 end(종료)를 내려주는 경우가 있음
	// 이때는 클라이언트 자체 루트모션이 종료되기도 이전 상황이라 업데이트를 진행하면서 보간이동 되는 상황이 발생..
	// end패킷에서 받은 위치를 저장하고 최종 완료시점에 동기화를 진행...
	FVector _rootmotionSyncPos = FVector::ZeroVector;
	uint16 _rootmotionSyncDir = 0;

	// 스폰시, CS_REQ_SPAWN_ME_ACTION 패킷을 보냈는지 체크하는 변수.
	bool _spawnMeCompleteFinFlag = false;

protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = GameObject, Meta = (AllowPrivateAccess = true))
	class AGsCharacterLocalPlayer* _actor;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = GameObject, Meta = (AllowPrivateAccess = true))
	class UGsInputBindingLocalPlayer* _inputBinder;

	EMovementType _MovementType = EMovementType::LocalPlayer;
	FGsMovementHandlerSpectatorLocalPlayer* _movementHandlerspectator = nullptr;

public:
	virtual void Initialize(const struct FGsSpawnParam* SpawnParam) override;
	virtual bool IsDispose(FGsGameFlow::Mode Type) override { return Type != FGsGameFlow::Mode::GAME; }
	virtual void Update(float Delta) override;
	virtual void Finalize() override;

protected:
	virtual void BindMessage() override;
	virtual void UnBindMessage() override;

public:
	virtual void ActorSpawned(class AActor* Spawn) override;
	virtual void ActorDespawn() override;
	virtual void CreateSubClass() override;
	void SetLocalPlayerHidden();

	void SetServerCollisionRule(bool inRule) { _serverCollisionRule = inRule; }

protected:
	virtual void InitializeActor(AActor* Owner) override;	

protected:	
	virtual void RemoveSubClass() override;

// 전투 평화 상태
public:
	virtual int UpdateCombatStatus(bool InImmediately = false) override;

protected:
	virtual void InitializeFSM() override;

protected:
	virtual FGsGameObjectData*					CreateData(const struct FGsSpawnParam* SpawnParam) override;
	virtual TSharedPtr<FGsTargetHandlerBase>	CreateTarget() override;
	virtual TSharedPtr<FGsSkillHandlerBase>		CreateSkill() override;
	virtual TSharedPtr<FGsPartsHandlerBase>		CreateParts() override;
	virtual TSharedPtr<FGsMovementHandlerBase>	CreateMovement() override;
	virtual TSharedPtr<FGsAbnormalityHandlerBase>CreateAbnormality() override;
	virtual TSharedPtr<FGsInteractionHandlerBase>CreateInteractionHandler() override;
	virtual class TSharedPtr<FGsStatBase>		CreateCreatureStat() override;
	class FGsScreenFilterLocalPlayer*			CreateScreenFilter();

	virtual TSharedPtr<FGsFairyHandlerPlayer>	CreateFairyHandler() override;

private:
	void CallbackOverlapedCharacterBase(int32 inOverlapCount);

public:
	virtual void NetChangeWeaponFinishedAck(PD::SC::PKT_SC_ITEM_EQUIP_CHANGE_WEAPON_TYPE_FINISHED_READ* Packet) override;
	virtual void NetAbnormalityResultAck(PD::SC::PKT_SC_ADD_ABNORMALITY_RESULT_READ* Packet) override;
	// 대화 연출 안에서 로컬 사라지는 이슈
	// abnormality remove시 update visible에서
	// UGsGameObjectBase::IsHidden 타면
	// 대화 연출중이라 사라지게됨
	// override해서 actor hidden만 뱉게 하자
	virtual bool IsHidden() override;

	// IFF 처리용 SharedFunc::IffFactionHellper 구조체를 사용하기 위해 데이터 정보를 이곳에서 동기화
	// 원본 데이터들은 각각 FGsServerOption, FGsGameObjectData에 저장 되어 있음.
	void CombatOptionUpdate();
	void NetIffFactionActorUpdate(const IffFactionActor& IffFactionActor);	

	bool IsOwnTreasureMonster() { return _ownTreasureGameId > 0; }
	void SetOwnTreasureMonsterId(int64 gameId){ _ownTreasureGameId = gameId; }
	int64 GetOwnTreasureMonsterId() { return _ownTreasureGameId; }

public:
	virtual void NetSkillDamagedAck(PD::SC::PKT_SC_SKILL_DAMAGED_READ* Packet) override;

	virtual bool OnHit(UGsGameObjectBase* Striker, UGsGameObjectBase* SubStriker, uint32 StriketSkillId, 		
		uint32 StrikerNotifyId, HitResultType ResultType = HitResultType::NORMAL,
		DamageByType DamageType = DamageByType::SKILL, DamageStatType DamageStat = DamageStatType::MAX,
		HealByType HealType = HealByType::MAX, int Damage = 1, PassivityOwnerType PassivityOwner = PassivityOwnerType::MAX,
		bool IsNoDamage = false) override;
	virtual void OnDie() override;
	virtual void OnRevive() override;
	// iff 변경됨
	virtual void OnIFFChanged()override;
	virtual void OnWarp() override;
	virtual void OnSpawn(bool In_isAlive, bool In_isNormalReason = true) override;
	// GameObjectData 갱신
	virtual void OnGameObjectDataUpdate(EGsGameObjectDataMember MemberType) override;

	virtual void OnLearnSkill(SkillId InSkillId) override;
	virtual void ChangeFairyEquip(FairyId id) override;
	virtual void ChangeCostumeEquip(CostumeId lookatId, CostumeId costumeId, bool isHairOn = true) override;

	virtual void OnTrigger(int64 TriggerGameId, EGsTriggerType TriggerType, bool IsActive) override;

protected:
	virtual void OnTargetClear(const struct IGsMessageParam* inParam) override;
	void OnChangeSafetyMode(const struct IGsMessageParam* inParam);

	void OnStageChange(const struct IGsMessageParam* inParam);
	void OnFairyChanged(const struct IGsMessageParam* inParam);

public:
	void BlockOtherPlayer(bool InBlock);
	void UpdateCollisionPreset();

	virtual void StartRootmotion(uint16 inDir, const FVector& inPos, const FVector& inDestPos, int inRootmotionId) override;
	virtual void EndRootmotion(uint16 inDir, const FVector& inPos, int inRootmotionId) override;
	void SyncRootmotionPosition();

protected:
	virtual void ClientMoveUpdate(float inDelta) override;

	// 메세지 Recv
public:
	void OnUpdateCollectionPassivity(const struct IGsMessageParam* InParam);

private:
	void OnQuestNpcSpawn(const struct IGsMessageParam* inParam);
	void OnQuestNpcDespawn(const struct IGsMessageParam* inParam);

public:
	void UpdateSpeedMovementHandler();

public:
	void ChangeFreezeState();
	bool SetTeleport();

	// 시퀀스 시작 이벤트
	void OnStartSequence();
	// 시퀀스 끝 이벤트
	void OnEndSequence();

	// 자동전투, 퀘스트 자동진행 중지
	void StopAuto(bool In_isStopAI = false);

public:
	void CheckInteractionCampState(UGsGameObjectBase* InCampObject);
	void CheckInteractionSanctumState(UGsGameObjectBase* InSanctumObject);

public:
	bool IsEnemy(UGsGameObjectBase* Target);
	bool IsParty(UGsGameObjectBase* Target);
	bool IsGuildMemeber(UGsGameObjectBase* Target);
	bool IsAllyGuildIdMember(UGsGameObjectBase* Target);

	virtual bool CanTarget(UGsGameObjectBase* Target);
	bool CanScan(UGsGameObjectBase* Target);
	bool CanInteraction(UGsGameObjectBase* Target);
	// 캠프랑 부딪혔다면
	// 충돌체 안으로 들어온 이벤트(수호탑 내에 퀘스트 목표가 있을 때 진행 불가 이슈)
	// https://jira.com2us.com/jira/browse/C2URWQ-6363
	void OnCampHit(const class AActor* In_camp);

public:
	virtual EGsGameObjectType GetObjectType() const override;

public:
	virtual class AActor* GetActor() const override;
	virtual void ClearActor() override; 

	UFUNCTION(BlueprintCallable, Category = "GameObject")
	class AGsCharacterLocalPlayer* GetLocalCharacter() const;

	void SwitchSpectatorMode(bool InActive);

	//퀘스트 NPC 근처에 있으면 플레이어 추가 
public:
	bool AddQuestNpc(class AActor* inActor);
	void RemoveQuestNpc(class AActor* inActor);

	void ClearPawnDetectionCollisionOverlaps();
	// 2022/11/09 PKT - 주변 사물(Remote Player)를 회피기능
	void SetPawnDetectionCollisionEnable(bool InFlag);
	
	void UpdateRVOAvoidance(bool inDisable = false);
	void UpdateAllFairyVehicleCondition();
	void SetFairyVehicleCondition(EGsFairyVehicleCondition inType, bool inRemove = false, bool inUpdate = true);
	void OnVehicleSendTimer(float inDelayTime = 0.f);

	// 강제 연결 해제 조건 설정
	void SetForceDisconnectIdle(bool isIdleState);
	// 예약된 데이터 삭제(현재 저장된 예약데이터 제외)
	// https://jira.com2us.com/jira/browse/C2URWQ-6334
	void ClearReserveDataByReserveWork(IGsReserveWork* In_currentReserveWork = nullptr);

protected:
	UFUNCTION()
	void OnEnterQuestNpc(class UPrimitiveComponent* inOverlappedComponent, class AActor* inOtherActor, class UPrimitiveComponent* inOtherComp, int32 inOtherBodyIndex, bool inbFromSweep, const FHitResult& inSweepResult);
	UFUNCTION()
	void OnLeaveQuestNpc(class UPrimitiveComponent* inOverlappedComponent, class AActor* inOtherActor, class UPrimitiveComponent* inOtherComp, int32 inOtherBodyIndex);

	UFUNCTION()
	void OnEnterDetectionPawn(class UPrimitiveComponent* inOverlappedComponent, class AActor* inOtherActor, class UPrimitiveComponent* inOtherComp, int32 inOtherBodyIndex, bool inbFromSweep, const FHitResult& inSweepResult);
	UFUNCTION()
	void OnleaveDetectionPawn(class UPrimitiveComponent* inOverlappedComponent, class AActor* inOtherActor, class UPrimitiveComponent* inOtherComp, int32 inOtherBodyIndex);

public:
	class FGsQuestHandler*	GetQuestHandler();
	class FGsFenceHandler*	GetFenceHandler();
	class FGsCameraModeHandler*	GetCameraModeHandler();
	class FGsAreaEnvHandler* GetAreaEnvHandler();
	class FGsLootingHandlerLocalPlayer* GetLootingHandler();
	class FGsScreenFilterLocalPlayer* GetScreenFilter();
	class FGsAIReserveHandler* GetAIReserve();
	class FGsAIAvoidCollisionHandler* GetAIAvoidCollision();

	const TArray<class IGsReserveWork*>& GetReserveWorks();

	UGsInputBindingLocalPlayer* GetInputBinder()
	{
		return _inputBinder;
	}

	EMovementType GetMovementType() const { return _MovementType; }

	bool IsSpectatorMode() 
	{
		return GetMovementType() == EMovementType::Spectator;
	}

	bool GetSpawnMeCompleteFinFlag() { return _spawnMeCompleteFinFlag; }
	void SetSpawnMeCompleteFinFlag(bool InFlag) { _spawnMeCompleteFinFlag = InFlag; }

	//bak1210 : Stiffen(경직) 처리를 이동불가 처리로 변경하게되어
	// 기존 상태이상로직을 같이 활용해야하는 상황 발생 -> 추후 경직도 상태이상으로 구현논의(서버팀장님 1차협의 완료)
	// 그전까지는 상태이상과 경직을 동시 처리하기 위한 구조를 GameObject레벨로 이동하여 처리한다.
public:
	enum class EMoveLockType
	{
		Abnormality,
		Stiffen,
		MoveLockTypeMax
	};
	
	virtual void OnStiffen(const FVector& StiffenPos, float fDuration) override;
	void MoveLock(EMoveLockType inType, bool isEnable);
private:
	TArray<bool> _moveLockFlag;
	FTimerHandle _moveLockTimer;
	void InitMoveLock();
	void InternalMoveLock(bool isEnable);

};
