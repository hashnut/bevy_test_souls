// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//show actor height with actor label in editor 

#include "ActorComponentEx/EGsMeshShaderEffect.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Debug/GsGameObjectDebugBase.h"
#include "Interface/GsMeshShaderInterface.h"

#include "Animation/GsHitAnimInstance.h"

#include "Animation/URO/GsUROInterface.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "ActorComponentEx/GsSkillGuideEffectComponent.h"
#include "ActorComponentEx/GsEffectComponent.h"
#include "GsCharacterBase.generated.h"

/**
 * GsActor의 실제 소멸 타이밍 처리 델리게이트
 * AActor 클래스의 FActorDestroyedSignature OnDestroyed 델리게이트로는 완전 소멸 타이밍을 처리 할수 없음
 * GsActor클래스는 이 델리게이트를 연결하여 객체의 소멸 타이밍 처리를 진행
 * 좀더 좋은 방법은 R&D가 더 필요해 보임
 * 언리얼 오브젝트 처리 관련 문서 참고 : \
 * https://docs.unrealengine.com/ko/Programming/UnrealArchitecture/Objects/Optimizations/index.html
 * https://docs.unrealengine.com/ko/Programming/UnrealArchitecture/Actors/ActorLifecycle/index.html
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorDeleteSignature, AActor*, DeleteActor);

/**
* T1Project에서 사용할 캐릭터 최상위 클래스
*/
class UAnimInstance;
class UGsAnimInstanceState;
class UGsHitEffectComponent;
class UBlackboardComponent;
class UGsMeshShaderControlComponent;
class UBehaviorTreeComponent;
class UGsQueryFloorHeightComponent;
class UGsEffectComponent;
class UParticleSystemComponent;
class UGsHommingProjectileComponent;
class UWidgetComponent;
class UGsUIBillboard;
class UAnimInstance;
class UGsCurveTimer;
class UGsHitAnimInstance;
class UGsWidgetComponentPooling;

struct FStreamableHandle;
struct FAnimUpdateRateParameters;

struct FGsSchemaCreatureCommonInfo;

const int32 DEFAULT_CULLING_DISTANCE = 1500.0f;

const FName PartsMeshComponentName[] = {
	TEXT("PartsSkeletalMeshFACE"),
	TEXT("PartsSkeletalMeshHAIR"),
	TEXT("PartsSkeletalMeshCLOAK"),
	TEXT("PartsStaticMeshWEAPON"),
	TEXT("PartsStaticMeshWEAPON_SUB"),
	TEXT("PartsSkeletalComponentFACE_SUB")
};
const int AttachPartsSkeletalMeshLength = 5;

struct FGsSpawnLocationLog
{
	FString hitActorName = TEXT("None");
	FVector serverSpawnLocation;
	FVector clientSpawnLocation;
	FVector hitActorLocation;
	float capsuleSize;
	bool isCanTeleport;
};

// 자연스러운 회전 연출을 위한 정보
USTRUCT(BlueprintType)
struct T1PROJECT_API FGsSmoothRotationInfo
{
	GENERATED_BODY()

public:
	// 추후 다시 구현 할 것 (시간 기준 회전 처리)
	UPROPERTY(EditAnywhere, Category = "SmoothRotation")
	class UCurveFloat* _curveData = nullptr;

	UPROPERTY(EditAnywhere, Category = "SmoothRotation")
	float _rotateSpeed = 0.f;				// 초당 회전 속도

	bool _isRotation;
	FRotator _currentRotation;
};

USTRUCT()
struct T1PROJECT_API FGsCharacterOrgMaterialInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class UMeshComponent* _meshComponent = nullptr;
	UPROPERTY()
	class UMaterialInterface* _material = nullptr;

	bool _isOverlayMaterial{ true };
	int  _materialIndex = 0; //_isOverlayMaterial == false 일때만
};

USTRUCT()
struct T1PROJECT_API FGsCharacterMaterialInfo
{
	GENERATED_BODY()

public:
	// 원복할 머터리얼 정보
	// 현재는 0번인덱스만 가정
	// 추후 여러개 인덱스 사용시 고려할 부분들이 있다.
	UPROPERTY()
	TArray<FGsCharacterOrgMaterialInfo> _originMaterials;

	// 현재 적용된 머터리얼 데이터 구조체 정보
	class UGsGameObjectMaterialData* _currentMaterialData = nullptr;

	bool _isAppliedOverlayMaterial{ true };
	// 업데이트 활성 유무
	bool _isUpdate = false;
	float _currTime = 0.f;
};


UCLASS()
class T1PROJECT_API AGsCharacterBase : public ACharacter, public IGsMeshShaderInterface,
	public IGsUROInterface
{
	GENERATED_BODY()
public:
	/** Default UObject constructor. */
	AGsCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameObjectDebug)
	FGsGameObjectDebugCreature _debugCreature;

	// 특정 목적으로 사용할 파티클 정보
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UParticleSystem* _customParticle = nullptr;
	
	// 오버렙된 Actor Count
	DECLARE_DELEGATE_OneParam(FOnOverlapOtherCharacterBase, int32)
	FOnOverlapOtherCharacterBase OnOverlapedCharacterBase;

protected:
	// 암묵적으로 EGsPartsType 으로 맞춤
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, EditFixedSize, Category = Parts, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* _partsSkeletalComponentFACE;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitEffect", meta = (AllowPrivateAccess = "true"))
	UGsHitEffectComponent* _hitEffect = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	UGsWidgetComponentPooling* _nameplateWidget = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI", meta = (AllowPrivateAccess = "true", Tooltip = "true일 경우 nameplate 컴퍼넌트 생성 안함"))
	bool _ignoreNameplateWidget = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QueryFloorHeight", meta = (AllowPrivateAccess = "true"))
	UGsQueryFloorHeightComponent* _queryHeight = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	UGsEffectComponent* _effectComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGsSkillGuideEffectComponent* _guideEffectComponent = nullptr;
	// 락온 위젯 컴퍼넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* _lockOnWidget = nullptr;
	// party shared target
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* _partySharedTargetWidget = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	UGsHommingProjectileComponent* _hommingProjectile = nullptr;

	// 부드러운 회전을 위한 정보
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SmoothRotation", meta = (AllowPrivateAccess = "true"))
	FGsSmoothRotationInfo _smoothRotationInfo;

	UPROPERTY()
	FGsCharacterMaterialInfo _materialInfo;

	// 생성시 스켈레탈 메시의 상대회전 정보 캐싱
	FRotator _originMeshRotation;

	// 생성시 기본 콜리전 타입 저장
	FName _oringinCollisionType;

	//Mesh shader effect
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Character base", meta = (AllowPrivateAccess = "true"))
	UGsMeshShaderControlComponent* _meshShaderComponent;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GameObjectDebug, meta = (DisplayPriority = 0))
	FString _ServerID;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GameObjectDebug, meta = (DisplayPriority = 0))
	TArray<FString> _eventLog;

protected:
	UPROPERTY(Transient)
	FVector	_lastLocation;

protected:
	TSharedPtr<FStreamableHandle> _hitMotionLoadHandle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAnimInstance> _hitMotionInterface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* _hitMotionBlendWeight;

#if WITH_EDITOR
	int SelectHitDirection = 0;
	int SelectHitBlendType = 0;
#endif

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGsHitAnimInstance* _hitInstance = nullptr;

	UPROPERTY(Transient)
	UGsCurveTimer* _hitMotionTimer = nullptr;

	bool _hitMotionPlay = false;

protected:
	UPROPERTY(Transient)
	UGsCurveTimer* _hitAmplitudeTimer;

	bool _hitAmplitudePlay = false;

	FVector _originMeshRelativeLocation;
	float _hitAmplitudeMultiplier;

protected:
	bool _bWasRendered = false;
	FFindFloorResult _findFloor;

public:
	UPROPERTY(BlueprintAssignable, Category = "Game")
	FActorDeleteSignature OnActorDelete;

	//debug
private:
	UPROPERTY()
	bool _isShowLogOnce = false;

public:
	FGsSpawnLocationLog _spawnLog;
	FGsSpawnLocationLog _moveResetLog;

protected:
	// 변신 액터 - 타 몬스터에 적용을 위해 옮긴다
	UPROPERTY()
	class AActor* _polymorpActor = nullptr;

	class USkeletalMeshComponent* _polymorpSkeletalMesh = nullptr;
	class USkeletalMeshComponent* _orgMesh = nullptr;

	UPROPERTY()
	TArray<USceneComponent*> _hiddenComponents;

public:
	virtual void OnConstruction(const FTransform& in_trs) override;

public:
	virtual void FinishDestroy() override;

protected:
	virtual void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type in_type) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) override;

public:
	virtual void SetSmoothRotaion(const FRotator& NewRotation);
	virtual void StopCurrentSkillEffectNSound();

protected:
	virtual void OnChangeRenderState(bool bIsRendered);
	virtual float GetLastRenderTimeOnScreenMesh() const;

	virtual void InitWidgetComponent();

public:
	// 피격 연출 처리
	virtual void OnHit(HitResultType ResultType, EGsStateBase CurrentState,
		const struct FGsSchemaCreatureCommonInfo* CommonData);

	virtual void OnHitEffect(const FVector& attackerLocation,
		HitResultType ResultType,
		const struct FGsSchemaSkillSet* SkillData,
		const struct FGsSchemaSkillNotifySet* SkillNotifyData,
		const struct FGsSchemaPassivitySet* PassivityData,
		const struct FGsSchemaProjectileSet* ProjectileData,
		const struct FGsSchemaExtraTypeEffectSet* DamageTypeEffectSet,
		const struct FGsSchemaHealByTypeEffectSet* HealTypeEffectSet,
		bool IsAttackerHeightFixed = false);

public:
	virtual class UGsUIBillboardCreature* GetWidget();

protected:
	virtual EGsAnimStateBlendType DecideBlendTypebyAnimState(EGsStateBase currentState);

	void DoHitMotion(EGsStateBase CurrentState);
	void DoHitAplitude();
	void ProcessHitMotion(float fDelta);
	void ProcessAmplitude(float fDelta);
	void ProcessSmoothRotaiton(float fDelta);
	// 하위 클래스들의 파츠 정보가 다름
	virtual void ProcessMaterialParams(float fDelta);

	void UpdateMaterialParams(class UMeshComponent* MeshComp, class UGsGameObjectMaterialData* MaterialData,
		int MaterialIndex, float fCurrent, float fDelta, bool isOverlayMaterial = false);

	UFUNCTION()
	void FinishBlendWeight(const UObject* timer);

	UFUNCTION()
	void FinishAmplitude(const UObject* timer);

protected:
	// 타입별 히트 처리 관련 함수
	void ResultHitEffectInfo(OUT class UGsHitEffectData*& OutEffectData, OUT class UGsHitEffectData*& OutAdditiveEffectData,
		OUT const struct FGsSchemaSoundResData*& OutSoundData, OUT CreatureWeaponType& OutWeaponType,
		const struct FGsSchemaSkillSet* SkillData,
		const struct FGsSchemaSkillNotifySet* SkillNotifyData,
		const struct FGsSchemaPassivitySet* PassivityData,
		const struct FGsSchemaProjectileSet* ProjectileData,
		const struct FGsSchemaExtraTypeEffectSet* DamageTypeEffectSet,
		const struct FGsSchemaHealByTypeEffectSet* HealTypeEffectSet
	);

public:	
	UGsWidgetComponentPooling* GetNameWidgetComponent() { return _nameplateWidget; }
	// 높이 얻어오기 컴포넌트
	UGsQueryFloorHeightComponent* GetQueryFloorHeightComponent() const { return _queryHeight; }
	UWidgetComponent* GetLockOnWidgetComponent() { return _lockOnWidget; }
	UWidgetComponent* GetPartySharedTargetWidgetComponent() { return _partySharedTargetWidget; }

	UGsUIBillboard* GetLockOnWidget() const;
	UGsUIBillboard* GetPartySharedTargetWidget() const;
public:
	// Main 스켈레탈 메쉬와 그 자식들이 가지는 PrimiviteComponent들의 필요 셋업
	void SetupPrimitives(bool drawXRayVision = false);
	void SetupPrimitives(AActor* Actor, bool drawXRayVision = false);

	// 타겟 마킹비지빌리티 설정
	virtual void SetVisibleTargetMaker(bool InVisible, class UMaterialInterface* InMaterial) {} 

public:
	void InitMeshShaderComponent(const FGsSchemaCreatureCommonInfo* inCommonInfo);

	// 액터 위치기준 구 그리기
	void DrawActorDebugSphere(float In_range);

	// Spawn
public:
	void RelocationCharacter();
public:
	void SetLastLocation(const FVector& inLocation);

	// 유도체의 목표지점이될 위치 설정 (타겟이 되는 액터의 발사체 목표 정보 설정)
	// const FVector& inRandomSize : X 넓이, Z 높이
	FVector GetTargetHitPoint(const FVector& inCasterPos, const FVector& inCasterDir, const FName& inHitSoketName,
		bool inRandom, const FVector& inRandomSize);

public:
	// 머터리얼 체인지 ( 블루프린트에서도 제공 )
	UFUNCTION(BlueprintCallable)
	virtual bool IsMaterialChanged() { return _materialInfo._currentMaterialData != nullptr; }
	UFUNCTION(BlueprintCallable)
	virtual void SetChangeAllMaterials(class UGsGameObjectMaterialData* inMaterialData, bool useOnlyOverlayMaterial = true);
	UFUNCTION(BlueprintCallable)
	virtual void RestoreAllMaterials();

	UFUNCTION(BlueprintCallable)
	void UpdateOriginalMaterial(UMeshComponent* meshCompo, UMaterialInterface* newMaterial);
	UFUNCTION(BlueprintCallable)
	void UpdateAllChangedMaterials();

	// metarial을 갈아 입을 때 되돌려 놓지 않으면, 다시 입을 때 changed 되어 있음
	UFUNCTION(BlueprintCallable)
	void RestorePreEquipMaterial(UMeshComponent* skeletalMeshComp);
	UFUNCTION(BlueprintCallable)
	void RestoreAttachedActorMaterials(TArray<AActor*>& childActors);

	UFUNCTION(BlueprintCallable)
	virtual bool IsOverlayMaterialChanged() { return _materialInfo._currentMaterialData != nullptr && _materialInfo._isAppliedOverlayMaterial; }

public:
	// 메시 변경
	UFUNCTION(BlueprintCallable)
	virtual bool IsMeshChanged() { return _polymorpActor != nullptr; }
	// 실제로는 원본 메시는 Hide처리하고 새로운 Actor를 Attach하여 동작
	UFUNCTION(BlueprintCallable)
	virtual void ChangeMesh(const FSoftObjectPath& inModelPath);
	// 부착된 메시를 Despawn 시키고 기존 메시로 되돌림
	UFUNCTION(BlueprintCallable)
	virtual void RestoreMesh();
	// 부착된 액터(무기..)을 mesh Changed 상태에서 변경할 때, 기존 무기등을 되돌려줌 (리소스 재사용)
	UFUNCTION(BlueprintCallable)
	void RestoreAttachedHiddenMesh(TArray<AActor*>& childActors); //Weapon
	UFUNCTION(BlueprintCallable)
	void ApplyAttachedHiddenMesh(TArray<AActor*>& childActors);
	//hidden mesh를 갈아 입을 때 되돌려 놓지 않으면, 다시 입을 때 hidden
	UFUNCTION(BlueprintCallable)
	void RestorePreEquipHiddenMesh(UMeshComponent* skeletalMeshComp);
	UFUNCTION(BlueprintCallable)
	void RestoreMeshState(UMeshComponent* skeletalMeshComp);
protected:
	//기본을 overlayMaterial로
	void AddOriginalMaterialInfo(class UMeshComponent* inMeshComponent, class UMaterialInterface* inMaterial = nullptr, bool isOverlayMaterial = true, int inMaterialIndex = -1);

	//마네킹으로 사용시 채널 변경 함수 
public:
	void ChangeLightingChannels(bool channel1 = true, bool channel2 = false, bool channel3 = false);

public:
	virtual UGsAnimInstanceState* GetAnim() const;
	virtual UAnimInstance* GetAnimationInstance() const;

	virtual void GetSkeletalMeshs(TArray<USkeletalMeshComponent*>& Out_skeletalMesh) override;
	virtual class USkeletalMeshComponent* GetCandidateMesh();

public:
	virtual bool LoadHitAnimInstance(const FSoftObjectPath& path);
	virtual void HitAnimInstanceDeferred();

	void InitInternalTimer();

public:
	FORCEINLINE UGsHitEffectComponent* GetHitEffect() const { return _hitEffect; }
	virtual UGsMeshShaderControlComponent* GetMeshShaderComponent() const override { return _meshShaderComponent; }
	virtual void StartMeshShader(EGsMeshShaderEffect inType, bool inIsRestor = false) override;
	virtual void StartMeshShader(EGsMeshShaderEffect inType, const FGsSchemaCreatureCommonInfo* inInfo);
	virtual void StopMeshShader() override;
	UGsEffectComponent* GetEffectComponent() const { return _effectComponent; }
	UGsSkillGuideEffectComponent* GetGuideEffect() const { return _guideEffectComponent; }
	UGsHommingProjectileComponent* GetHommingProjectile() const { return _hommingProjectile;}

public:
	// 현재는 BODY를 Base SkeletalMesh로 둔다.
	USkeletalMeshComponent* GetPartsSkeletalComponentBODY() const { return GetMesh(); }
	USkeletalMeshComponent* GetPartsSkeletalComponentFACE() const { return _partsSkeletalComponentFACE; }

public:
	void ActiveGravity(bool inActive);
	// 위젯 컴퍼넌트의 위젯이 생성 된뒤
	void PostWidgetComponentMakeWidget();
public:
	/**
	 * Called from CharacterMovementComponent to notify the character that the movement mode has changed.
	 * @param	PrevMovementMode	Movement mode before the change
	 * @param	PrevCustomMode		Custom mode before the change (applicable if PrevMovementMode is Custom)
	 */
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

public:
	virtual void SetCollisionProfileName(FName InCollisionProfileName, bool bUpdateOverlaps = true);
	virtual void SetDefaultCollisionProfile();

#if WITH_EDITOR
public:
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

	virtual bool ShouldTickIfViewportsOnly() const override;

public:
	// 피격 연출 효과 적용 플레그
	virtual bool IsHitEffect() const { return true; }
	virtual bool IsHitFresnel() const { return true; }

	// 네이티브 -> 블루프린트 통신 함수
public:
	// 배틀, 평화 상태 모드
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveBattleMode(bool IsBattle);

	// 상태이상 추가 제거 이벤트
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveAddAbnormality(int Id);
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveRemoveAbnormality(int Id);

	// Hp 변화에 따른 이벤트
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveChangeHP(int CurrHp, int MaxHp);

	//아트 연출을 위한 블루프린트 이벤트 추가 
public:
	UFUNCTION(BlueprintImplementableEvent)
	void IdleEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void DyingEvent();

public:
	void UpdateRenderState();

public:
	virtual void EffectsHiddenInGame(bool set);

	virtual void SetActorHiddenInGame(bool bNewHidden) override;

	//virtual TArray<class UMaterialInterface*> GetMaterialsByMesh(USkeletalMeshComponent* InSkeletalMeshComp);

};
