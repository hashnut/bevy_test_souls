// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "GsFairyAnimInstance.generated.h"


UENUM(BlueprintType)
enum class EGsStateFairy : uint8
{
	None = 0,
	Spawn,
	Idle,
	Move,
	Skill,
	Special,
	Despawn,
	Destroy,
};

USTRUCT(BlueprintType)
struct FGsFairyAnimInstanceStateProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
	bool IsSpawn = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
	bool IsIdle = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
	bool IsDespawn = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
	bool IsUseSkill = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
	bool IsSpecial = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
	bool IsMove = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
	bool IsDestroy = false;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
	int32 IdleIndex = 0;

public:
	//UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsFairyAnimInstanceState")
	//bool IsActiveMontage = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
	EGsStateFairy StateType = EGsStateFairy::None;

public:
	FGsFairyAnimInstanceStateProxy() : FAnimInstanceProxy() {}
	FGsFairyAnimInstanceStateProxy(UAnimInstance* Instance) : FAnimInstanceProxy(Instance) {}

	FAnimNode_StateMachine* GetFirstMachine();
public:
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual void PostEvaluate(UAnimInstance* InAnimInstance) override;

private:
	void ChangeStateToBPAnimationState(EGsStateFairy State);
	void InitializeAnimationState();
};

/**
 *
 */
UCLASS()
class T1PROJECT_API UGsFairyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState", meta = (AllowPrivateAccess = "true"))
	EGsStateFairy CurrentStateType = EGsStateFairy::None;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState", meta = (AllowPrivateAccess = "true"))
	FGsFairyAnimInstanceStateProxy _proxy;

private:
	EGsStateFairy _restartState{ EGsStateFairy::None };

	float _playAnimTime{ 0.f };

	bool _isWaitingState{ false };
	bool _isMoveState{ false };
	bool _useAnimEnd{ false };

	TFunction<void(EGsStateFairy)> _cbAnimEnd{ nullptr };
public:
	// MoveState 안에서 분기처리용
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsFairyAnimInstanceState")
	bool _isVehicleState{ false };

protected:
	UFUNCTION(BlueprintCallable)
	void EndState(EGsStateFairy State);

public:
	UGsFairyAnimInstance();
	virtual ~UGsFairyAnimInstance();

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void ResetCurrentState(FName FsmName);
	void ChangeState(EGsStateFairy State, bool loopState = false, TFunction<void(EGsStateFairy)> endCB = nullptr);

	float GetCurrentAnimLength();
	bool IsCurrentAnimEnd();
	
	void SetMoveState(bool isMove);
	bool IsMoveState() { return _isMoveState; }

	void SetWaitingState(bool isWaiting);
	bool IsWaitingState() { return _isWaitingState; }

	void SetVehicleState(bool isVehicle);
	bool IsVehicleState() { return _isVehicleState; }
	
	EGsStateFairy GetCurrentState() { return CurrentStateType; }

private:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}

	friend struct FGsFairyAnimInstanceStateProxy;
};
