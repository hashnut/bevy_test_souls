// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GsCompleteCampAnimInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGsStateCampComplete : uint8
{
	None = 0,
	PreSpawn_CompleteCamp,
	Spawn_CompleteCamp,
	Idle_CompleteCamp,
	Hit_CompleteCamp,
	Destroy_CompleteCamp,
};

USTRUCT(BlueprintType)
struct FGsCompleteCampAnimInstanceStateProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsCompleteCampAnimInstance")
	bool IsPreSpawn = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsCompleteCampAnimInstance")
	bool IsSpawn = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsCompleteCampAnimInstance")
	bool IsIdle = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsCompleteCampAnimInstance")
	bool IsDamage = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsCompleteCampAnimInstance")
	bool IsDestroy = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsCampAnimInstanceState")
	int SpawnTypeIndex = 0;

	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsCompleteCampAnimInstance")
	EGsStateCampComplete CompleteCampStateType = EGsStateCampComplete::PreSpawn_CompleteCamp;

public:
	FGsCompleteCampAnimInstanceStateProxy() : FAnimInstanceProxy() {}
	FGsCompleteCampAnimInstanceStateProxy(UAnimInstance* Instance) : FAnimInstanceProxy(Instance) {}

public:
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual void PostEvaluate(UAnimInstance* InAnimInstance) override;

private:
	void ChangeStateToBPAnimationState(EGsStateCampComplete State);
	void InitializeAnimationState();
};


UCLASS()
class T1PROJECT_API UGsCompleteCampAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	friend struct FGsCompleteCampAnimInstanceStateProxy;

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsCompleteCampAnimInstance", meta = (AllowPrivateAccess = "true"))
	EGsStateCampComplete CurrentStateType = EGsStateCampComplete::None;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsCompleteCampAnimInstance", meta = (AllowPrivateAccess = "true"))
	FGsCompleteCampAnimInstanceStateProxy _completeCampAnimStateProxy;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsCompleteCampAnimInstance")
	float _animSpeedValue = 1.0f;

protected:
	int _spawnTypeIndex = 0;
	float _time{ 0.f };

public:
	UGsCompleteCampAnimInstance();
	virtual ~UGsCompleteCampAnimInstance();

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable, Category = "GsCompleteCampAnimInstance")
	void EndState(EGsStateCampComplete State);

public:
	void SetSpawnTypeIndex(int InNum);
	void SetAnimSpeed(float InSpeedValue) { _animSpeedValue = InSpeedValue; }

public:
	void ResetCurrentState(FName FsmName);
	void ChangeState(EGsStateCampComplete State);

public:
	EGsStateCampComplete GetCurStateType() { return CurrentStateType; }
	float GetStateAnimationLength(FName FsmName, FName StateName)
	{
		int idx = GetInstanceAssetPlayerIndex(FsmName, StateName);
		return GetInstanceAssetPlayerLength(idx);
	}

private:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}
	
};
