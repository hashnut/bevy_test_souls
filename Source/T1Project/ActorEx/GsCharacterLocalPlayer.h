// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsCharacterPlayer.h"
#include "UTIL/GsTimeUtil.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "ActorComponentEx/GsCameraPostProcessComponent.h"
#include "Templates/Function.h"
#include "T1Project.h"
#include "GsCharacterLocalPlayer.generated.h"

/**
* 언리얼 엔진 로직 담당 Local캐릭터 전용 클래스
* 로컬 BP제작시 이 클래스를 기반으로 제작
* ACharacter 클래스를 상속받은 커스텀 ACharacter 클래스
* 조작 제어를 위해 UGsInputBindingBase를 설정하게 구성
*/

class AGsPlayerController;
class UBlackboardComponent;
class UGsGameObjectLocalPlayer;
class UMaterialInstanceConstant;
class UMaterialInstanceDynamic;
class UWidgetInteractionComponent;
struct FGsSchemaPartsSchema;

UCLASS()
class T1PROJECT_API AGsCharacterLocalPlayer : public AGsCharacterPlayer
{
	GENERATED_BODY()

public:
	AGsCharacterLocalPlayer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameObjectDebug)
		FGsGameObjectDebugLocalPlayer _debugLocalPlayer;

protected:
	TWeakObjectPtr<class UGsGameObjectLocalPlayer> _owner;

	// InputBinder
	class UGsInputBindingBase* _inputBinder = nullptr;

	// 캐릭터 따라가기 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* _cameraBoom = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* _followCamera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UGsSkillCollisionComponent* _skillCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UGsSkillCollisioHandlerComponent* _skillCollisionHandle = nullptr;

	UPROPERTY(Transient)
		class AGsPlayerController* _playerController;

	UPROPERTY(EditDefaultsOnly, Category = GsCharacterLocalPlayer, meta = (AllowPrivateAccess = "true"))
		class UGsCameraPostProcessComponent* _cameraPostProcess;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GsCharacterLocalPlayer, meta = (AllowPrivateAccess = "true"))
		TArray<int> _customAvoidanceConsiderationRadius;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character base", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* _questNpcSphereComp;

	// 2022/11/07 PKT - Nearby Pawn Detection Sphere Comp
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character base", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* _nearbyPawnDetectionBoxComp;

public:
	// 특수한 목적을 가진 delegate 캠프 타입 오브젝트와 충돌 되었을 경우 이벤트 알림	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitCamp, const class AActor*)
	FOnHitCamp OnHitCampEvent;

private:
	// 2022/11/11 PKT - Pawn Detection Flag
	bool _isPawnDetectionEnable = false;
	bool _isPawnDetectionCollisionEnable = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type inReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual FName GetWidgetClassName() const override { return TEXT("LocalPlayerWidget"); }

	//Player post process effect
	//Player post process Death dimed
public:
	UFUNCTION(CallInEditor, Category = "PlayerEffect")
		void PlayDeathEffect();
	void StopDeathEffect();

	UFUNCTION(CallInEditor, Category = "PlayerEffect")
		void PlayResurrectionEffect();
	void PlayBlurEffect();
	void StopBlurEffect();

	void OnHitNotifyByCampObject(const class AActor* CampObject);

#if WITH_EDITOR
public:
	UFUNCTION(CallInEditor, Category = "PlayerEffect")
		void PlayPlayerDissolve();
	UFUNCTION(CallInEditor, Category = "PlayerEffect")
		void PlayPlayerFresnel();
#endif

private:
	void OnTickDeathDimed(const class UPostProcessCurve* inValue);
	void OnTickDeathDimedFadeOut(const class UPostProcessCurve* inValue);
	void OnTickBlurEffect(const class UPostProcessCurve* InValue);

	//Player hp low effect
public:
	UFUNCTION(CallInEditor, Category = "PlayerEffect")
	void BeginHpWarningEffect();
	void StopHpWarningEffect();
	void CheckWarnHp(const class FGsStatInfo& InInfo);

	void SetConfuseEffect(bool bIsConfused);

public:
	void PlaySkillLearnEffect(int32 InEffectIndex);

public:
	// 초기화
	void Initialize(class UGsGameObjectLocalPlayer* In_owner);
	void BindMessage();

	void EnableCollision();
	void DisableCollision();

	UFUNCTION(Exec, Category = ExecFunctions)
		void SetTouchMoveFactor(float In_val);
	UFUNCTION(Exec, Category = ExecFunctions)
		void SetTouchMoveLimit(float In_val);
	UFUNCTION(Exec, Category = ExecFunctions)
		void ShowTouchMoveValue();
	UFUNCTION(Exec, Category = ExecFunctions)
		void SetTouchZoomFactor(float In_val);
	UFUNCTION(Exec, Category = ExecFunctions)
		void ShowTouchZoomValue();
	UFUNCTION(Exec, Category = ExecFunctions)
		void ShowTouchCMD();
	UFUNCTION(Exec, Category = ExecFunctions)
		void SetPitchRotMul(float In_val);

	void SetAttachFollowCamera(bool In_isAttach);

	void SetAttachFollowCameraToController(bool In_isAttach);

public:
	//Actor
	virtual void MoveBlockedBy(const FHitResult& Impact) override;

public:
	FORCEINLINE class UGsInputBindingBase* GetInputBinder() const { return _inputBinder; }
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return _cameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return _followCamera; }
	FORCEINLINE class UGsSkillCollisionComponent* GetSkillCollisionComponennt() const { return _skillCollision; }

	void SetInputBinder(class UGsInputBindingBase* Binder);

	void SetSpawnState(bool in_spawn);

	virtual void ChangeWeaponSocketProcess(TArray<FName> ListSocketName, TArray<FVector> ListOffset,
		class UCurveFloat* WeaponDissolve, class UCurveFloat* WeaponFrsnel, class UParticleSystem* Particle,
		bool IsStartProcess, TFunction<void()> EndCallback = nullptr) override;

public:
	class AGsPlayerController* GetPlayerController();

public:
	UFUNCTION(BlueprintCallable, Category = "AGsCharacterLocalPlayer")
		class UGsGameObjectLocalPlayer* GetGameObjectPlayer();

public:
	void SetQuestNpcSphereRadius(float inRadius);
	void ActiveQuestNpcSphere(bool inActive = true);
	class USphereComponent* GetQuestNpcSphereComp();

	// 2022/11/07 PKT - Pawn Detection
public:
	// 2022/11/11 PKT - Pawn 폰탐지 활성화 여부( 마을 지역은 비활성화 )
	void SetPawnDetectionBoxEnable(bool InEnable);
	
	void ClearPawnDetectionCollisionOverlaps();
	// 2022/11/11 PKT - Collision 활성화 여부( 퀘스트 이동 시 활성화 )
	void SetPawnDetectionCollisionEnable(bool InEnable);

	void SetAvoidanceConsiderationRadius(int InIdex);

public:
	class UBoxComponent* GetPawnDetectionBoxComp();

public:
	virtual bool SetWeaponBlueprintClass(FSoftObjectPath inPath, FGsWeaponLoadCompleteDelegate& inAfter, bool isAsync = false) override;
	virtual bool SetWeaponSubBlueprintClass(FSoftObjectPath inPath, FGsWeaponLoadCompleteDelegate& inAfter, bool isAsync = false) override;
};
