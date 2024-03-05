// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActorEx/GsCampBase.h"
#include "GameObject/Interaction/GsActorInteractionInterface.h"
#include "Animation/GsCompleteCampAnimInstance.h"
#include "GsCompleteCamp.generated.h"

class UDecalComponent;
class UWidgetComponent;
class UGsUIBillboardCreature;
class UGsUIBillboardInteractionButton;
class UGsUIBillboardCamp;
class UGsCompleteCampAnimInstance;
class UGsUIBillboardCamp;
class UParticleSystem;
class UParticleSystemComponent;

/**
 * 
 */
UCLASS()
class T1PROJECT_API AGsCompleteCamp : public AGsCampBase, public IGsActorInteractionInterface
{
	GENERATED_BODY()

private:
	const float DAMAGE_EFFECT_FIRST = 0.85f;
	const float DAMAGE_EFFECT_SECOND = 0.50f;
	const float DAMAGE_EFFECT_THIRD = 0.25f;	

private:
	enum CampParticleType : uint8
	{
		IdleParticleType = 0,
		DamageFire85Type,
		DamageFire50Type,
		DamageFire25Type,

		TypeMax,
	};


	// shadowDecal
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Decal", meta = (AllowPrivateAccess = "true"))
	UDecalComponent* _shadowDecal = nullptr;

	// HP Bar
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* _hpBarWidget = nullptr;

	// 인터랙션 버튼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* _interactionWidget = nullptr;

	// idle 상태 파티클
	UPROPERTY(EditAnywhere, Category = "Idle_Particle", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* _idleParticleEffect;
	UPROPERTY(EditAnywhere, Category = "Idle_Particle", meta = (AllowPrivateAccess = "true"))
	FVector _idleParticleEffectPos;

	// 1단계 불길 이펙트
	UPROPERTY(EditAnywhere, Category = "Damage85_Particle", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* _damageParticleEffect85;
	UPROPERTY(EditAnywhere, Category = "Damage85_Particle", meta = (AllowPrivateAccess = "true"))
	FVector _damageParticleEffect85Pos;

	// 2단계 불길 이펙트
	UPROPERTY(EditAnywhere, Category = "Damage50_Particle", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* _damageParticleEffect50;
	UPROPERTY(EditAnywhere, Category = "Damage50_Particle", meta = (AllowPrivateAccess = "true"))
	FVector _damageParticleEffect50Pos;

	// 3단계 불길 이펙트
	UPROPERTY(EditAnywhere, Category = "Damage25_Particle", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* _damageParticleEffect25;
	UPROPERTY(EditAnywhere, Category = "Damage25_Particle", meta = (AllowPrivateAccess = "true"))
	FVector _damageParticleEffect25Pos;

	// 인터랙션 클릭 콜백
	TFunction<void(bool)> _callbackInteractionClick;

protected:
	//UPROPERTY(EditAnywhere, Category = Camp)
	//FVector _navigationAddOffset;
	UPROPERTY(EditAnywhere, Category = Camp)
	FVector _navModifierSize;

private:
	UPROPERTY()
	UGsCompleteCampAnimInstance* _completeCampAnim = nullptr;

private:
	TWeakObjectPtr<UGsUIBillboardCamp> _hpBillboardPtr;

private:
	UPROPERTY()
	UParticleSystemComponent* _campIdleParticleComp = nullptr;
	UPROPERTY()
	UParticleSystemComponent* _campDamage85ParticleComp = nullptr;
	UPROPERTY()
	UParticleSystemComponent* _campDamage50ParticleComp = nullptr;
	UPROPERTY()
	UParticleSystemComponent* _campDamage25ParticleComp = nullptr;

private:
	FVector _actorMeshBoundsExtends;
	FVector _actorBoundsExtends;

public:
	AGsCompleteCamp();

public:
	virtual void EndPlay(const EEndPlayReason::Type in_type) override;
	virtual FVector GetMoveGoalOffset(const AActor* MovingActor) const override { return _navigationAddOffset; }

protected:
	virtual void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void FinishedAnim() override;
	void DestoryPorc() override;

public:
	void ChangeAnimState(EGsStateCampComplete InType);
	void BeginDestroyAnim() override;
	void BeginHitAnim() override;

public:
	void OnIdleEffect();
	void OffIdleEffect(bool InImmediately = false);

public:
	void CheckDamageEffect(const float InCurValue);
	void AllOffDamageEffect();
	

private:
	void OffDamageEffect(const CampParticleType InParticleType, bool InImmediately = false);
	void OnDamageEffect(const CampParticleType InParticleType);

public:
	void SetSpawnIndex(uint8 InNum);

protected:
	void InitializeDecals();

protected:
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

public:
	virtual UGsUIBillboardInteractionButton* GetInteractionButton() const override;
	virtual void InitWidgetComponent() override;
	// IGsActorInteractionInterface
	// 인터랙션 클릭
	virtual void ClickInteraction() override;
	// 인터랙션 위젯 컴퍼넌트 접근
	virtual UWidgetComponent* GetInteractionWidgetComponent() override { return _interactionWidget; }
	// 인터랙션 클릭 콜백 세팅
	virtual void SetInteractionClickCallback(TFunction<void(bool)> In_callback) override;
	// 인터랙션 버튼 쓸지
	virtual void SetUseInteractionButton(bool In_isUse) override;

public:
	virtual UGsUIBillboardCamp* GetCampNameplateWidget() const override;

public:
	// 그림자데칼 Visible
	void SetVisibleShadowDecal(bool InVisible);

public:
	FVector GetActorBoundsExtends() override;
	FVector GetActorMeshBoundsExtends() override;
	void SetHpBarAndNameWidgetVisibility(const bool InActive) override;
	void SetHpBarWidgetProgress(const float InValue) override;
	void SetCampName(const FText InText) override;
	FVector GetNavModifierSizeVec() override;
	void SetNavigationAddOffsetValue(FVector InVec) override;
	void SetCampEmblemIcon(const uint8 InEmblemId) override;


private:
	bool IsVisibleShadowDecal() const;

//----------------------------------------------------------------------------------------------------------------------
	// 성소 구현을 위해 임시로 추가한 항목들
protected:
	// SetUseInteractionButton 으로 교체될 항목
	virtual void SetUseInteractionButtonSanctum(bool InUse) override;

public:
	virtual void PlayAnimationHit() override;
	virtual void PlayAnimationDestroy() override;

	virtual void SetDammageEffect(float InRate) override;

public:
	// 
	void ShowHpBar(bool bVisible);

	//UMaterialInterface
	void SetMaterial(class UMaterialInterface* InMaterial);
	void SetMaterialColor(bool bInEnable);

};
