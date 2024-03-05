// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "GsAnimInstanceState.h"
#include "GsSandbagAnimInstance.generated.h"


UENUM(BlueprintType)
enum class EGsStateSandbag : uint8
{
	None = 0,
	Spawn,
	Idle,
	Hit,
	Dying,
	Dead,
};

USTRUCT(BlueprintType)
struct FGsSandbagAnimInstanceStateProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
		bool IsSpawn = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
		bool IsIdle = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
		bool IsHit = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
		bool IsDying = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
		bool IsDead = false;

public:
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsFairyAnimInstanceState")
		bool IsActiveMontage = false;
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsFairyAnimInstanceState")
		EGsStateBase StateType = EGsStateBase::Idle;

public:
	FGsSandbagAnimInstanceStateProxy() : FAnimInstanceProxy() {}
	FGsSandbagAnimInstanceStateProxy(UAnimInstance* Instance) : FAnimInstanceProxy(Instance) {}

public:
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual void PostEvaluate(UAnimInstance* InAnimInstance) override;

private:
	void ChangeStateToBPAnimationState(EGsStateBase State);
	void InitializeAnimationState();
};

/**
 *
 */
UCLASS()
class T1PROJECT_API UGsSandbagAnimInstance : public UGsAnimInstanceState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GsFairyAnimInstanceState")
		void EndState(EGsStateBase State);

protected:
	float _time{ 0.f };

public:
	UGsSandbagAnimInstance();
	virtual ~UGsSandbagAnimInstance();

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void ResetCurrentState(FName FsmName);
	void ChangeState(EGsStateBase State);
	float GetStateAnimationLength(FName FsmName, FName StateName)
	{
		int idx = GetInstanceAssetPlayerIndex(FsmName, StateName);
		return GetInstanceAssetPlayerLength(idx);
	}

private:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState", meta = (AllowPrivateAccess = "true"))
		FGsSandbagAnimInstanceStateProxy _sandbagProxy;

	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}

	friend struct FGsSandbagAnimInstanceStateProxy;
};
