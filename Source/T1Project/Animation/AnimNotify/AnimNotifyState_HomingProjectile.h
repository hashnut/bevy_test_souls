// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState_TimedParticleEffect.h"
#include "AnimNotifyState_HomingProjectile.generated.h"

UENUM(BlueprintType, meta = (Tooltip = "유도체 연출 타입"))
enum class EGsHomingHitParticleType : uint8
{
	NORMAL = 0 UMETA(Tooltip = "기본형"),
	RETURN = 1 UMETA(Tooltip = "선정 대상을 목적위치로 하여 목적위치 부터 회귀"),
};

USTRUCT(BlueprintType)
struct T1PROJECT_API FGsHomingHitParticleInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UParticleSystem* PSTemplate = nullptr;

	UPROPERTY(EditAnywhere)
	FVector LocationOffset;

	UPROPERTY(EditAnywhere)
	FRotator RotationOffset;

	UPROPERTY(EditAnywhere)
	FVector Scale;

	UPROPERTY(EditAnywhere)
	uint32 Attached : 1; 	//~ Does not follow coding standard due to redirection from BP	

	UPROPERTY(EditAnywhere)
	FName SocketName;

	UPROPERTY(EditAnywhere, Category = ParticleSystem, meta = (DisplayName = "Destroy Immediately", ToolTip = "Whether the particle system should be immediately destroyed at the end of the notify state or be allowed to finish"))
	bool bDestroyAtEnd;

	// 
	UPROPERTY(EditAnywhere, meta = (ToolTip = "Attacker Look Dir"))
	bool UserCharacterOffset;
};

USTRUCT()
struct T1PROJECT_API FGsHomingTargetInfo
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* GoalActor = nullptr;

	FVector GoalPos;
	FRotator TargetRot;

	FTransform SpawnTm;
	FTransform HitOffsetTm;

	bool IsHit = false;
	int32 EventId;			// SkillNotifyId를 입력
};

/**
 * 
 */
UCLASS()
class T1PROJECT_API UAnimNotifyState_HomingProjectile : public UAnimNotifyState_TimedParticleEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	EGsHomingHitParticleType _type = EGsHomingHitParticleType::NORMAL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ToolTip = "연동이 필요한 SkillNotifyId 입력"))
	int _skillNotifyId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	float _moveSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	class UCurveVector* _curveData;
	
	// Return Type 파티클
	UPROPERTY(EditAnywhere, Meta = (EditCondition = "_type == EGsHomingHitParticleType::RETURN", EditConditionHides))
	class UParticleSystem* _PSReturnTemplate = nullptr;

	// Hit Effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitEffect")
	FGsHomingHitParticleInfo _hitEffectData;

	UPROPERTY(Transient)
	class UGsHommingProjectileComponent* _effectComponent = nullptr;

protected:
	UPROPERTY()
	TArray<FGsHomingTargetInfo> _listTargetInfo;

#if WITH_EDITORONLY_DATA
	class UGsGlobalConstant* _globalConstant = nullptr;
	class UCurveFloat* _curveFloat = nullptr;
	class AActor* _hitTargetActor = nullptr;
	float _animIdelStateTimer = 0.f;
	FDelegateHandle _delegatehandle;
#endif

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation) override;

protected:
	void OnParticleCollide(class UAnimSequenceBase* Animation, class AActor* CasterActor, const FVector& HitLocation);

	void NewHommingTargetInfo(OUT FGsHomingTargetInfo& outInfo, OUT float& outTargetSize, 
		class AGsCharacterBase* OwnerActor, class AGsCharacterBase* TargetActor, bool IsMiss);
};
