// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsGameObjectNonPlayerBase.h"
#include "GameObject/Interaction/GsGameObjectInteractionInterface.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"
#include "Message/GsMessageInvasion.h"
#include "GsGameObjectNonPlayer.generated.h"

/**
 *
 */
UCLASS()
class T1PROJECT_API UGsGameObjectNonPlayer : public UGsGameObjectNonPlayerBase, public IGsGameObjectInteractionInterface	
{
	GENERATED_BODY()

	//interaction 
protected:
	FGsInteractionMessageParam _interactionMessageParam;
	// 이전 사이즈 값
	float _oldColliderSize = 0.0f;
	// 인터랙션 모드인가
	bool _isInteractionActive = false;
	// 침공퀘스트 주는 NPC인가?
	bool _isInvadeQuestNpc = false;

protected:	
	UPROPERTY()
	class UCapsuleComponent* _interactionBlockCollider = nullptr;

	float _newInteractionButtonSize;
	float _oldInteractionButtonSize;

	// 현재 인터랙션 컬리더 offset
	float _currentInteractionColliderOffset = 0.0f;

	// 보간용 사이즈
	float _currentLerpBlockColliderSize = 0.0f;
	bool _isOffensive;

	// 보물몬스터 소유자에 대한 정보
	struct FGsOwnerInfo
	{
		uint64		_gameId{ 0 };
		GuildDBId	_guildId{ 0 };
		UserDBId	_dBId{ 0 };
		FString		_name;
	};
	FGsOwnerInfo _ownInfo;

	//남은 life time
	time_t _lifeTime{ 0 };
	bool _partyMonster{ false };

protected:
	// 말풍선의 사용 여부 확인
	bool _isTalkBalloonActive{ false };

public:
	static int catchGhost;

protected:
	TArray<TPair<MessageInvasion, FDelegateHandle>> _invasionHandleArray;

public:
	virtual ~UGsGameObjectNonPlayer() = default;

	// 각종 가상함수
public:
	virtual void Initialize(const struct FGsSpawnParam* SpawnParam) override;
	virtual void InitializeActor(AActor* Owner) override;
	virtual void Update(float Delta) override;
	// 위젯 이름 show/hide 옵션 갱신
	virtual void UpdateWidgetShowNameOption() override;
	// widget hp bar show/hide cheat option
	virtual void UpdateWidgetShowHPBar() override;

protected:
	virtual void BindMessage() override;	
	virtual void UnBindMessage() override;

protected:
	virtual void RemoveSubClass() override;
	virtual void InitializeFSM() override;
	virtual void InitializeMovement() override;

private:
	void CallbackOverlapedCharacterBase(int32 inOverlapCount);
	void BlockLocalPlayer(bool InBlock);


protected:
	virtual TSharedPtr<FGsAbnormalityHandlerBase> CreateAbnormality() override;

	
public:
	virtual void UpdateTargetMark(bool inVisibleTargetMaker, bool inIsFirst = true) override;
	// 상황에 따른 Idle타입을 판단하여 AnimState에 전달
	virtual void ChangeIdleType(EGsAnimStateIdleType Type) override;

	virtual void SetWidgetTreasureMonsterInfo(FString userName, int32 remainTimeSec, int32 fullTimeSec = 0, uint64 ownGameId = 0,
		GuildDBId guildId = INVALID_GUILD_DB_ID, UserDBId ownDBId = INVALID_USER_DB_ID);
	virtual void ChangeWidgetTreasureMonsterGuildInfo(GuildDBId guildId);
	virtual void UpdateWidgetAliveTime(int32 remainTimeSec);

public:
	virtual bool OnHit(UGsGameObjectBase* Striker, UGsGameObjectBase* SubStriker, uint32 StriketSkillId,
		uint32 StrikerNotifyId, HitResultType ResultType,DamageByType DamageType, DamageStatType DamageStat,
		HealByType HealType, int Damage, PassivityOwnerType PassivityOwner, bool IsNoDamage) override;

	virtual void OnRunAway() override;

	virtual void OnIFFChanged() override;

#if	!UE_BUILD_SHIPPING
public:
	void OnDebugTextInfo(PD::SC::PKT_SC_DEBUG_TEXT_INFO_READ* packet = nullptr);
	void ShowDebugTextInfo(bool isOn = false);
#endif

public:
	virtual void NetDeathAck(PD::SC::PKT_SC_CREATURE_DEATH_READ* Packet) override;

	//Interaction
public:

	// 인터랙션 디버그 범위 그리기
	virtual void DrawDebugInteractionRange() override;
	// 인터랙션 버튼 클릭
	virtual void OnClickInteract(bool In_isByWidget) override;
	
	// visible 상태인지 갱신
	virtual void UpdateVisible() override;

	// 인터렉션 기능 상태 갱신
	void UpdateInteractionEvent();

	// 일반 로직(none 가상)

	// state
public:
	void WaitUntilDie(FName in_anim);

	//boss
public:
	bool IsBossMonster() const;
	void BossMonsterDieProc();

public:
	void ChangeCollisionPresetToPawn();
	// 타게팅
public:
	// 몸통 클릭 타게팅
	void OnClickTargeting();

	// show/ hide
public:
	// 스폰 디스폰 이펙트 출력
	void OnSpawnDespawnEffect();

	// interaction button
public:
	// 위젯 인터랙션 버튼 show/hide 옵션 갱신
	void UpdateWidgetShowInteractionButtonOption();

protected:
	void InvalidFieldBossBillboard();
	void InvalidateMinimapTreasureMonster();

	virtual void OnChangedStat(const class FGsStatInfo& inStateInfo) override;

public:
	virtual void LookAt(const FRotator& LookRotator, bool IsSmoothRotation = false) override;

	// get, set 함수
public:
	virtual EGsGameObjectType GetObjectType() const override { return EGsGameObjectType::NonPlayer; }
	UFUNCTION(BlueprintCallable, Category = "GameObject")
	class AGsCharacterNonPlayer* GetNonCharacter();

	//interaction
public:
	// 인터랙션 액티브 처리
	virtual void SetInteractionActive(bool In_isActive, int In_Index = 0) override;
	// 인터랙션 액티브인지
	virtual bool GetIsActiveInteraction() override { return _isInteractionActive; }

	//quest
public:
	// 퀘스트 타겟 액티브인지
	virtual void SetQuestTargetActive(bool In_isActive, int In_Index = 0) override;

public:
	virtual void ActorDespawn() override;
	virtual void OnDie() override;

public:
	virtual time_t GetEndOfLifeTime() override { return _lifeTime; }
	virtual const FString GetOwnerName() { return _ownInfo._name; }
	virtual bool IsTreasureMonster() override;
	virtual bool IsPlayerTeamTreasureMonster() override;
	virtual FText GetNpcName() override;
	NpcFunctionType GetNpcFunctionType();

	virtual bool IsAble() override;

	virtual void SetVisibleInteractionMoveTargetMarker(bool In_visible) override;

	bool IsMyTreasureMonster();
	bool IsMyPartyMonster();
	bool IsMyGuildMemberOwnMonster();	
	bool IsMonster();

	void SetValidCheckPartyMonster();

private:
	bool CheckInvasionBattleMode();
	void OnStartInvasion(const struct IGsMessageParam* inParam);
	void OnFinishInvasion(const struct IGsMessageParam* inParam);

public:
	//말풍선 활성화 기능 
	void ActiveTalkBallon(bool inActive);
	//인터렉션 버튼 활성화 기능 
	void ActiveInteractionIcon(bool inActive);

public:
	bool IsInTown();

public:
	// 말풍선 출력 시스템	
	UFUNCTION()
	void OnChangedTalkBalloonState();	
	virtual UGsWidgetComponentTalkBalloon* SetTalkBalloonActive() override;
	virtual void HideTalkBalloon() override;
	static bool IsFunctionalNpc(NpcFunctionType inFunction);
	void SetNpcCollision();
	void SetAffectNavigation();

public:
	virtual const FGsSchemaShapeData* GetPolymorphShapeData() override;
};
