// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActorComponentEx/EGsMeshShaderEffect.h"
#include "GsCharacterBase.h"
#include "Templates/Function.h"
#include "GameObject/Define/EGsFootPrintMethod.h"
#include "GsCharacterPlayer.generated.h"

class UStaticMeshComponent;
class USkeletalMeshComponent;
class UDecalComponent;
class UChildActorComponent;
class UCurveFloat;
class UParticleSystem;
class AGsWeaponActor;
class FGsItem;
class UGsPhysicalMaterial;

struct FGsSchemaCostumeEffect;
struct FGsSchemaWeaponPeaceInfo;
struct FGsSchemaUserData;

USTRUCT(BlueprintType)
struct T1PROJECT_API FGsHitInfoStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	CreatureWeaponType					weaponType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UAnimInstance>	hitMotionInterface;
};


DECLARE_MULTICAST_DELEGATE(FGsWeaponLoadCompleteDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGsCostumeEffectLoadComplete, AActor*, LoadActor);

/**
* 플레이어 타입 처리 관련 Base Actor(ACharacter) 클래스
* OtherPlayer(타유저)에 대해 따로 처리해야될 부분이 생기면 이클래스를 파생하여 제작
**/
UCLASS()
class T1PROJECT_API AGsCharacterPlayer : public AGsCharacterBase
{
	GENERATED_BODY()
public:
	/** Default UObject constructor. */
	AGsCharacterPlayer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	// 무기 변경 효과 구조체
	struct FGsCharacterPlayerWeaponChangeInfo
	{
		UCurveFloat* _curveData = nullptr;
		TFunction<void()> _endCallback = nullptr;
		double _startTime = -1.f;
		float _endTime = -1.f;
		float _delta = 0.f;

		void Clear();
		void Start(UCurveFloat* CurveData, TFunction<void()> EndCallback);
		void Update(float Delta);
		bool IsValid() const { return _startTime > 0.f; }
	};	

protected:
	// 발자국 이펙트 구조체
	struct FGsFootPrint
	{
		UParticleSystem* effect = nullptr;
		UGsPhysicalMaterial* material = nullptr;
		EGsFootPrintMethod method = EGsFootPrintMethod::MATERIAL;
		FVector locationOffset = FVector::ZeroVector;
		FVector location = FVector::ZeroVector;
		FRotator rotation = FRotator::ZeroRotator;
	};

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Etc")
	void CallBlueprintApplyLodLevelDown(bool flag);

	UPROPERTY(BlueprintAssignable, Category = "Game")
	FGsCostumeEffectLoadComplete OnCostumeEffectLoadComplate;

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parts, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* _partsWeaponChildActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parts, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* _partsWeaponSubChildActor;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, EditFixedSize, Category = Parts, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* _partsSkeletalComponentHAIR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Decal", meta = (AllowPrivateAccess = "true"))
	UDecalComponent* _targetMaker = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NamePl", meta = (AllowPrivateAccess = "true"))
	float _namePlateZOffset = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "fairy", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* _fairySpringArm = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PoseAsset", meta = (AllowPrivateAccess = "true"))
	TArray<FGsHitInfoStruct> _hitPlayerInfo;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parts, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* _costumeEffectChildActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ToolStaticMesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* _toolEquipmentStaticMesh = nullptr;

	// 사망 시 보여질 액터(무덤)
	UPROPERTY()
	class AActor* _deathActor = nullptr;	

	//// 변신 액터
	//UPROPERTY()
	//class AActor* _polymorpActor = nullptr;
	//class USkeletalMeshComponent* _polymorpSkeletalMesh = nullptr;
	//class USkeletalMeshComponent* _orgMesh = nullptr;

	FGsCharacterPlayerWeaponChangeInfo _weaponChangeInfo;	

	CreatureWeaponType _weaponType;

protected:
	// Footprint Effect (맵마다 다른 이펙트 출력 지원)
	FGsFootPrint _defaultFootPrint;
	TArray<FGsFootPrint> _footPrintTriggerList;
	TArray<FGsFootPrint> _footPrintOverlapList;

	//costumeEffect
	const FGsSchemaCostumeEffect* _equipCostumeEffectInfo = nullptr;

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type inReason) override;

public:
	virtual bool LoadHitAnimInstance(const FSoftObjectPath& path) override;
	//virtual class USkeletalMeshComponent* GetCandidateMesh() override;
	
	//virtual void SetChangeAllMaterials(class UGsGameObjectMaterialData* inMaterialData) override;
	//virtual void RestoreAllMaterials() override;
public:
	void SetWeaponType(CreatureWeaponType inWeaponType) { _weaponType = inWeaponType; }
	void SetWeaponTypeChangeHitPose(CreatureWeaponType inWeaponType);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginDestroy() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Actor
	// bak1210 ; 물같은 오버렙이 필요한 데이터를 설정한다.. 
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
	void SetActorHiddenInGame(bool bNewHidden) override;

protected:
	virtual void InitWidgetComponent() override;

protected:
	virtual FName GetWidgetClassName() const { return TEXT("RemotePlayerWidget"); }

protected:
	// 외부(BP) 에서 설정 하게 하는게 좋을듯
	void InitializePartsComponent(UPrimitiveComponent* Component);
	void InitializeDecals();

public:
	// 사망연출 켜기/끄기
	virtual void SetEnableDeathActor(bool bInEnable);

	virtual void SetVisibleTargetMaker(bool InVisible, class UMaterialInterface* InMaterial) override;

	virtual EGsAnimStateBlendType DecideBlendTypebyAnimState(EGsStateBase currentState) override;
	
	//Shader
public:
	virtual void StopMeshShader() override;
	virtual void StartMeshShader(EGsMeshShaderEffect inType, bool inIsRestor = false) override;
	void StartMeshShader(EGsMeshShaderEffect inType, const FGsSchemaCreatureCommonInfo* inInfo) override;

	// 무기 슬롯 변경 효과 처리
	virtual void ChangeWeaponSocketProcess(TArray<FName> ListSocketName, TArray<FVector> ListOffset, 
		UCurveFloat* WeaponDissolve, UCurveFloat* WeaponFrsnel, UParticleSystem* Particle, 
		bool IsStartProcess, TFunction<void()> EndCallback = nullptr);

	//// 메시 변경
	//// 실제로는 원본 메시는 Hide처리하고 새로운 Actor를 Attach하여 동작
	//void ChangeMesh(const FSoftObjectPath& inModelPath);
	//// 부착된 메시를 Despawn 시키고 기존 메시로 되돌림
	//void RestoreMesh();

	//void RestoreAttachedHiddenMesh(TArray<AActor*>& childActors); //Weapon

	//virtual bool IsMeshChanged() override { return _polymorpActor != nullptr; }

	//Weapon
private:
	AGsWeaponActor* GetWeapon_Internal(UChildActorComponent* inChildActor) const;
	bool SetWeaponBlueprintClass_Internal(UChildActorComponent* inChildActor, FSoftObjectPath inPath, FGsWeaponLoadCompleteDelegate& inAfter, bool isAsync);
	bool SetWeaponEnchantEffect_Internal(AGsWeaponActor* inWeapon, FGsItem* item);
	bool SetWeaponEnchantEffect_Internal(AGsWeaponActor* inWeapon, CreatureWeaponType inWeaponType, int32 inWeaponLevel);
	void DestroyWeapon_Internal(UChildActorComponent* inChildActor);

public:
	AGsWeaponActor* GetWeapon() const;		
	AGsWeaponActor* GetWeaponSub() const;		
	UChildActorComponent* GetWeaponChildActorComponent() const {return _partsWeaponChildActor;}
	UChildActorComponent* GetWeaponSubChildActorComponent() const {return _partsWeaponSubChildActor;}

	USkeletalMeshComponent* GetPartsSkeletalComponentHAIR() const { return _partsSkeletalComponentHAIR; }
	USpringArmComponent* GetPartsFairySpringArm() const { return _fairySpringArm; }
	
public:
	UChildActorComponent* GetCostumeEffectChildActorComponent() const { return _costumeEffectChildActor; }

	//footPrint Effect
public:
	bool IsValidFootPrintTrigger() { return (_footPrintTriggerList.Num() > 0) ? true : false; }
	bool IsValidFootPrintOverlap() { return (_footPrintOverlapList.Num() > 0) ? true : false; }

public:	
	//virtual UGsAnimInstanceState* GetAnim() const override;

	UGsPhysicalMaterial* GetDefaultFootPrintMaterial() { return _defaultFootPrint.material; }
	UParticleSystem* GetDefaultFootPrintEffect() { return _defaultFootPrint.effect; }
	EGsFootPrintMethod GetDefaultFootPrintMethod() { return _defaultFootPrint.method; }

	UParticleSystem* GetFootPrintTriggerEffect() { return IsValidFootPrintTrigger() ? _footPrintTriggerList.Last().effect : nullptr; }
	UGsPhysicalMaterial* GetFootPrintTriggerMaterial() { return IsValidFootPrintTrigger() ? _footPrintTriggerList.Last().material : nullptr; }
	EGsFootPrintMethod GetFootPrintMethod() { return IsValidFootPrintOverlap() ? _footPrintOverlapList.Last().method : EGsFootPrintMethod::MATERIAL; }
	UGsPhysicalMaterial* GetFootPrintOverlapedMaterial() { return IsValidFootPrintOverlap() ? _footPrintOverlapList.Last().material : nullptr; }
	FVector	GetFootPrintOverlapedLocation() { return IsValidFootPrintOverlap() ? _footPrintOverlapList.Last().location : FVector::ZeroVector; }
	FRotator GetFootPrintOverlapedRotation() { return IsValidFootPrintOverlap() ? _footPrintOverlapList.Last().rotation : FRotator::ZeroRotator; }


public:
	void RestoreDefaultFootPrint();

public:
	void AddFootPrintEffectByTrigger(class UParticleSystem* inEffect, class UGsPhysicalMaterial* inMaterial);
	void RemoveFootPrintEffectByTrigger(class UParticleSystem* inEffect, class UGsPhysicalMaterial* inMaterial);

public:
	virtual bool SetWeaponBlueprintClass(FSoftObjectPath inPath, FGsWeaponLoadCompleteDelegate& inAfter, bool isAsync = false);
	virtual bool SetWeaponSubBlueprintClass(FSoftObjectPath inPath, FGsWeaponLoadCompleteDelegate& inAfter, bool isAsync = false);

	bool SetWeaponEnchantEffect(FGsItem* item);
	bool SetWeaponEnchantEffect(CreatureWeaponType inWeaponType, int32 inWeaponLevel);
	bool SetWeaponSubEnchantEffect(FGsItem* item);	
	bool SetWeaponSubEnchantEffect(CreatureWeaponType inWeaponType, int32 inWeaponLevel);
	void DestroyWeapon();
	void DestroyWeaponSub();
	void SetHiddenWeapon(bool inHidden);
	void SetWeaponTransformByCurve(UChildActorComponent* weaponComponent, const FGsSchemaWeaponPeaceInfo* Info, bool InBattle);
	void StopWeaponTransformByCurve();


	void SetCostumeEffectBlueprintClass(const FGsSchemaCostumeEffect* InEffectInfo, bool isAsync = false);
	void DestroyCostumeEffect();
	void SetHiddenCostumeEffect(bool inHidden);	

	void SetToolWeaponStaticMeshVisible(bool InVisibilityFlag);	

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	void InvalidateCostumeEffectChildActor();
#endif
};
