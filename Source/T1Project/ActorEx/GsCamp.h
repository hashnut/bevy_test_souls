// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActorEx/GsCampBase.h"
#include "GameObject/Interaction/GsActorInteractionInterface.h"
#include "Animation/GsCompleteCampAnimInstance.h"
#include "GsCamp.generated.h"


class UDecalComponent;
class UWidgetComponent;
class USkeletalMeshComponent;
class UGsUIBillboardCreature;
class UGsUIBillboardInteractionButton;
class UGsUIBillboardCamp;
class UGsCampAnimInstance;
class AGsCharacterLocalPlayer;
class UGsUIBillboardCamp;
class UParticleSystem;
class UParticleSystemComponent;

/**
 * Camp Actor Class
 */
UCLASS()
class T1PROJECT_API AGsCamp : public AGsCampBase, public IGsActorInteractionInterface
{
	GENERATED_BODY()

	// shadowDecal
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Decal", meta = (AllowPrivateAccess = "true"))
	UDecalComponent* _shadowDecal = nullptr;

	// HP Bar
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* _hpBarWidget = nullptr;

	// 인터랙션 버튼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* _interactionWidget = nullptr;

	// 단계별 인터렉션 파티클
	UPROPERTY(EditAnywhere, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* _interactionParticle_1stComp;
	UPROPERTY(EditAnywhere, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* _interactionParticle_2ndComp;
	UPROPERTY(EditAnywhere, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* _interactionParticle_3rdComp;

	// 인터랙션 클릭 콜백
	TFunction<void(bool)> _callbackInteractionClick;

	//UPROPERTY(EditAnywhere, Category = Camp)
	//FVector _navigationAddOffset;
	UPROPERTY(EditAnywhere, Category = Camp)
	FVector _navModifierSize;


private:
	TWeakObjectPtr<UGsUIBillboardCamp> _hpBillboardPtr;

private:
	FVector _actorBoundsOrigin;
	FVector _actorBoundsExtends;
	FVector _actorMeshBoundsExtends;
	EGsCampType _curCampType = EGsCampType::ConstructType;

private:
	UPROPERTY()
	UGsCampAnimInstance* _campAnim = nullptr;

public:
	AGsCamp();

public:
	virtual void EndPlay(const EEndPlayReason::Type in_type) override;
	virtual FVector GetMoveGoalOffset(const AActor* MovingActor) const override { return _navigationAddOffset; }

protected:
	virtual void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void OnInteractionEffect(UParticleSystemComponent* InTargetComp);
	void OffInteractionEffect(UParticleSystemComponent* InTargetComp);

public:
	void FinishedAnim() override;

public:
	void DestoryPorc() override;

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

	virtual UGsUIBillboardCamp* GetCampNameplateWidget() const override;


public:
	// 그림자데칼 Visible
	void SetVisibleShadowDecal(bool InVisible);

public:
	void BeginDestroyAnim() override;
	void BeginHitAnim() override;

public:
	void SetNavigationAddOffset(FVector InVec) { _navigationAddOffset = InVec; }
	void OnInteractionEffect();
	void OffInteractionEffect();
	void SetHpBarAndNameWidgetVisibility(const bool InActive) override;
	void SetHpBarWidgetProgress(const float InValue) override;
	void SetCampName(const FText InText) override;
	void SetCurCampType(const EGsCampType InCampType) { _curCampType = InCampType; }
	void SetNavigationAddOffsetValue(FVector InVec) override;
	void SetAffectNavigation(const bool IsState);

public:
	void SetCampEmblemIcon(const uint8 InEmblemId) override;
	FVector GetActorBoundsExtends() override;
	FVector GetActorMeshBoundsExtends() override;
	float GetHpBarValue() override;
	float GetCampDiameter();
	UWidgetComponent* GetCampInteractionWidgetComponent();
	const EGsCampType GetCurCampType() { return _curCampType; }
	FVector GetNavModifierSizeVec() override;


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
	virtual void PlayAnimationConstructionStep(int32 InStep, bool bIsNormal) override;
};
