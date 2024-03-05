// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "GsPropAnimInstance.generated.h"


UENUM(BlueprintType)
enum class EGsStateProp : uint8
{
	None = 0,
	Spawn,
	Idle,
	Action,
	Finish,
};

USTRUCT(BlueprintType)
struct FGsPropAnimInstanceStateProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
	bool IsSpawn = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
	bool IsIdle = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
	bool IsFinish = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
	bool IsAction = false;

public:
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsFairyAnimInstanceState")
	bool IsActiveMontage = false;
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsFairyAnimInstanceState")
		EGsStateProp StateType = EGsStateProp::None;

public:
	FGsPropAnimInstanceStateProxy() : FAnimInstanceProxy() {}
	FGsPropAnimInstanceStateProxy(UAnimInstance* Instance) : FAnimInstanceProxy(Instance) {}

public:
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual void PostEvaluate(UAnimInstance* InAnimInstance) override;

private:
	void ChangeStateToBPAnimationState(EGsStateProp State);
	void InitializeAnimationState();
};

/**
 *
 */
UCLASS()
class T1PROJECT_API UGsPropAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
	bool IsActiveMontage = false;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
		EGsStateProp CurrentStateType = EGsStateProp::None;

	UFUNCTION(BlueprintCallable, Category = "GsFairyAnimInstanceState")
	void EndState(EGsStateProp State);

protected:
	float _time{ 0.f };

public:
	UGsPropAnimInstance();
	virtual ~UGsPropAnimInstance();

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void ResetCurrentState(FName FsmName);
	void ChangeState(EGsStateProp State);
	float GetStateAnimationLength(FName FsmName, FName StateName)
	{
		int idx = GetInstanceAssetPlayerIndex(FsmName, StateName);
		return GetInstanceAssetPlayerLength(idx);
	}

private:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState", meta = (AllowPrivateAccess = "true"))
		FGsPropAnimInstanceStateProxy _proxy;

	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}

	friend struct FGsPropAnimInstanceStateProxy;
};
