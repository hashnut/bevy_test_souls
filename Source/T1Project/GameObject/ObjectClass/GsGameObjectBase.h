// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define SPAWN_OBJECT_SHOW_HIDE false

#include "GameFlow/GsGameFlow.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Define/GsGameObjectStruct.h"
#include "GameObject/GsGameObjectHandler.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageDelegateList.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Util/GsClassUtil.h"
#include "GsGameObjectBase.generated.h"

/**
 * 모든 스폰 관련 오브젝트들의 base클래스
 * 언리얼엔진에서 제공하는 로직을 제외한 오브젝트에 대한 클라이언트 로직 처리 담당
 */
class IGsGameObjectHandler;

UCLASS(BlueprintType)
class T1PROJECT_API UGsGameObjectBase :
	public UObject,
	public TGsMessageDelegateList<MGameObject, MessageGameObject>
{
	GENERATED_BODY()

public:
	// GameObject 자체 메세지 시스템
	using MGameObjectSelf =
		TGsMessageHandlerOneParam<MessageGameObject, const FGsGameObjectMessageParam*>;

protected:
	// GameObject Type 정의값 저장 (BitSet 정보)
	uint32						_objectType;

protected:
	//FGsTargetHandlerBase*		_targetHandler		= nullptr;
	class FGsGameObjectData*	_gameObjectData = nullptr;

	MGameObjectSelf				_gameObjectMessage;
	FGsGameObjectMessageParam	_messageParam;

	// 무적인가?(ai에서 사용)
	bool						_isInvincible = false;
	bool						_isSubClassCreate = false;
	// 이플레그가 설정되어있으면 서버에서는 이미 죽은경우지만 클라이언트에서 지연처리중
	bool						_isPendingKill = false;

	// Initialize 처리 분산용 Queue
	TQueue<TFunction< void()>>	_queueLateInitializeFunc;
	FTimerHandle				_lateInitializeTimer;

	ActorState					_ActorState = ActorState::NONE;
	FGsTM						_ActorTM;

protected:
	bool						_ActorVisible = true;

#if WITH_EDITOR
	class UGsDebugStateLogComponent* _debugLogComponent = nullptr;
#endif
	
private:
	// 핸들러들을 모아 관리한다.
	TArray<TSharedPtr<IGsGameObjectHandler>>	_handlerHolder;

public:
	UGsGameObjectBase();
	virtual ~UGsGameObjectBase();
	virtual void BeginDestroy() override;

public:
	// EGsGameObjectType 의 UENUM 정의를 제거 하여 해당 문자열 정보를 등록
	static FString GetObjectTypeName(EGsGameObjectType Type);

public:
	virtual void Initialize(const struct FGsSpawnParam* SpawnParam);
	virtual void Finalize();
	virtual void Update(float Delta);

	// Actor의 Despawn 처리와 GameObject 객체의 삭제는 별개로 구분짓는다.
	virtual bool IsDispose(FGsGameFlow::Mode Type) { return true; };

	// bak1210 : 핸들러관련 메소드
protected:
	void AppendInitialize(TSharedPtr<IGsGameObjectHandler> inHandler);
	void RemoveAllHandler();	
	
public:
	template <typename T = IGsGameObjectHandler>
	T* GetGameObjectHandler(GameObjectHandlerType inType) const
	{
		for (TSharedPtr<IGsGameObjectHandler> handler : _handlerHolder)
		{
			if (handler && handler->IsA(inType))
			{
				return StaticCastSharedPtr<T>(handler).Get();
				break;
			}
		}
		return nullptr;
	}

protected:
	// Delegate Bind
	virtual void BindMessage();
	virtual void UnBindMessage();

	// Actor의 Spawn 과 함께 다음 메서드 호출 순서를 따른다.
	// 1. ActorSpawned()
	// 2. InitializeActor()
	// 3. CreateSubClass()
	// 4. InitializeSubClass()
	// 5. BindMessage()
public:
	// Actor Spawn 완료
	virtual void ActorSpawned(class AActor* Spawn);
	// Actor Despawn
	virtual void ActorDespawn();

protected:
	virtual void InitializeActor(class AActor* Owner);

	// 실제 GameObject클래스 객체의 생성 소멸과 무관
	virtual void CreateSubClass();
	virtual void RemoveSubClass();

	virtual void InitializeFSM() {};

	virtual void LateCreateSubClass();
	void AddLateCreateSubClassEvent(TFunction<void()> Command);
	void ExecuteLateEvent();

protected:
	// 상속 되는 클래스에서 각 클래스의 기능에 맞는 SubClass들을 Override 하여 생성해 줘야 한다.
	virtual class FGsGameObjectData* CreateData(const struct FGsSpawnParam* SpawnParam);
#ifdef USE_GAMEOBJECT_DISPLAY_DATA
	virtual class FGsDisplayData* CreateDisplayData();
#endif

	// visible 관련
public:
	// visible 상태인지 갱신
	virtual void UpdateVisible() {}
	// visible 관련 정보 세팅(샌드백, 프랍 터치)
	virtual void SetVisibleInfo(const struct FGsSchemaSpawnElement* inVisible) {}
	// visible 바로 세팅(대사 연출 관련 처리)
	virtual void SetGameObjectActorVisible(bool In_isVisible);
	// 연출 Hidden 체크
	virtual bool IsHidden();
	// 위젯 이름 show/hide 옵션 갱신
	virtual void UpdateWidgetShowNameOption() {};
	// widget hp bar show/hide cheat option
	virtual void UpdateWidgetShowHPBar() {};

	// represent abnormality effect visible(dialog)
	void SetRepresentAbnormalityEffectVisible(bool In_isVisible);

	// 각종 이벤트 처리 함수
public:
	// 충돌 이벤트
	// 사실 이렇게 까지 심오해 질거라 생각 안함... 추후 Heal관련 패킷과 분리 진행이 필요
	virtual bool OnHit(UGsGameObjectBase* Striker, UGsGameObjectBase* SubStriker, uint32 StriketSkillId, 
		uint32 StrikerNotifyId, HitResultType ResultType = HitResultType::NORMAL,
		DamageByType DamageType = DamageByType::SKILL, DamageStatType DamageStat = DamageStatType::MAX,
		HealByType HealType = HealByType::MAX, int Damage = 1, 
		PassivityOwnerType PassivityOwner = PassivityOwnerType::MAX, bool IsNoDamage = false);
	//
	virtual void OnDie();
	virtual void OnRevive();
	

	virtual void OnRunAway();
	// iff 변경됨
	virtual void OnIFFChanged() {}

	virtual void OnTrigger(int64 TriggerGameId, EGsTriggerType TriggerType, bool IsActive) {}

	// Delegate Recv
public:
	virtual void OnGameObjectDataUpdate(EGsGameObjectDataMember MemberType);

	// 네트워크 응답 처리
public:
	virtual void NetDeathAck(PD::SC::PKT_SC_CREATURE_DEATH_READ* Packet);


	// Util 처리 함수
public:
	// 대상 방향으로 회전
	void LookAtTarget(const UGsGameObjectBase* LookTarget, bool IsSmoothRotation = false);
	// 포지션 방향으로 회전
	void LookAtPosition(const FVector& WorldPos, bool IsSmoothRotation = false);
	// 특정 방향으로 회전
	virtual void LookAt(const FRotator& LookRotator, bool IsSmoothRotation = false);

	// Getter 함수
public:
	// 전투 처리 관련 메서드
	virtual bool IsZeroHP() const;
	virtual bool IsAble();

	// 공격 가능 거리에서 네비 지형 체크할 필요가 있는가?
	virtual bool IsSkipTerrainCheck() const;

	// 죽음 지연처리가 필요한가 확인
	bool IsPendingKill();
	void ClearPendingKill();

	virtual bool IsPeacefulIdle() { return true;  }

public:
	int64 GetGameId() const;
	// GameObject 타입값 얻기
	virtual EGsGameObjectType	GetObjectType() const { return EGsGameObjectType::Base; }
	// 타입 조합값 얻기
	virtual uint32				GetObjectTypeMask() const { return _objectType; }
	// 포함되는 타입인가 확인
	virtual bool				IsObjectType(EGsGameObjectType Type) const { return CHECK_FLAG_TYPE(GetObjectTypeMask(), Type); }
	// 행동 제약 확인
	virtual bool				IsCrowdControlLock(CreatureActionType actionType) { return false; }
	virtual bool				IsCrowdControlLock() { return false; }							// 어떤 타입이라도 하나라도 존재

	virtual void				SetVisibleInteractionMoveTargetMarker(bool In_visible) {};

	// 현재 이 클래스가 유효한 클래스인가 검사
	static bool IsVaildClass(UGsGameObjectBase* GameObjectClass);
	bool IsVaildClass() const;

	// 락온 액티브
	// void SetLockOnActive(bool In_isActive);
	void SetPartySharedTargetActive(bool In_isActive);


//#pragma todo("bak1210 : 히트모션 발주 종료후 삭제할 코드")
	bool						LegacyHit = true;

public:
	MGameObjectSelf& GetMessage() { return _gameObjectMessage; }
	class FGsGameObjectData* GetData() const { return _gameObjectData; }
	bool GetIsInvincible() const { return _isInvincible; }

	virtual class FGsGameObjectStateManager* GetBaseFSM() const { return nullptr; }

	class FGsTargetHandlerBase* GetTargetHandler();
	class FGsSkillHandlerBase* GetSkillHandler();
	class FGsPartsHandlerBase* GetPartsHandler();
	class FGsMovementHandlerBase* GetMovementHandler();
	class FGsCustomizeHandler* GetCustomizeHandler();
	class FGsAbnormalityHandlerBase* GetAbnormalityHandler();
	class FGsDamageHandlerBase* GetDamageHandler();
	

	// GameObject 관련 클래스 Casting 처리 Util 함수
	template <typename T = UGsGameObjectBase>
	T* CastGameObject()
	{
		T* castObject = Cast<T>(this);
		check(castObject);
		return castObject;
	}
	template <typename T = FGsGameObjectData>
	T* GetCastData() const
	{
		return FGsClassUtil::FCheckStaticCast<T>(GetData());
	}
	template <typename T = class FGsTargetHandlerBase>
	T* GetCastTarget()
	{
		return FGsClassUtil::FCheckStaticCast<T>(GetTargetHandler());
	}

	void SetIsInvincible(bool In_newVal) { _isInvincible = In_newVal; }

public:
	// Actor 클래스 얻기
	UFUNCTION(BlueprintCallable, Category = "GameObject")
		virtual class AActor* GetActor() const { return nullptr; }
	virtual void ClearActor() {}
	virtual class AGsCharacterBase* GetCharacter() const { return nullptr; }
	virtual class UGsAnimInstanceState* GetAnimInstance() const { return nullptr; }
	//virtual FActorDeleteSignature* GetDeleteSignature() const { return nullptr; }

public:
	virtual const struct FGsSchemaCreatureCommonInfo* GetCommonInfoTable() const { return nullptr; }

	// BP 접근용 메서드 모음
public:
	// BP에서 간략한 접근을 위해 기본 TM정보 제공
	UFUNCTION(BlueprintCallable, Category = "GameObject")
	virtual FVector GetLocation() const;

	UFUNCTION(BlueprintCallable, Category = "GameObject")
	virtual FVector2D GetLocation2D() const;

	UFUNCTION(BlueprintCallable, Category = "GameObject")
	virtual FRotator GetRotation() const;

	UFUNCTION(BlueprintCallable, Category = "GameObject")
	virtual FVector GetScale() const;

	UFUNCTION(BlueprintCallable, Category = "GameObject")
	virtual FVector GetLootTargetPoint(UGsGameObjectBase* LootTarget) const;

	// 대상 오브젝트와의 거리 정보 (sqrt 적용 안된 버전)
	UFUNCTION(BlueprintCallable, Category = "GameObject")
	float GetTargetDistSquared(const UGsGameObjectBase* TargetObj) const;	

	virtual bool IsAutoTarget() { return true; }

	virtual bool IsUseAutoIdleEmotion();
	void SetAutoIdleEmotionEnable(bool set);

	//(비동기)액터의 상태를 체크하기 위한 함수
	virtual ActorState GetActorState() const { return _ActorState; }
	virtual void SetActorState(ActorState value) { _ActorState = value; }

	//(비동기)액터가 없을때(로딩중) 위치 정보 저장
	void SetActorLocation(const FVector& loc);
	void SetActorRotation(const FRotator& rot);

	const FVector& GetActorLocation() const;
	const FRotator& GetActorRotation() const;

	//(비동기) 액터로딩이 완료되었을때 인터페이스
	virtual void AsyncLoadClassComplete(class UObject* inRes) { check(0); };

#if WITH_EDITOR
	bool IsDebugStateLog() const { return nullptr != _debugLogComponent; }
	virtual void DebugAddStateLog(const FString& inStateMsg) {}
#endif
};
