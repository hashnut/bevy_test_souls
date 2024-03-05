// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Animation/GsAnimInstanceState.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GsCampAnimInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGsStateCamp : uint8
{
	None = 0,
	Building_Camp,
	BuildingComplete,
	Idle_Camp,
	Hit_Camp,
	Destroy_Camp,
};

USTRUCT(BlueprintType)
struct FGsCampAnimInstanceStateProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()
public:
	// 건설단계 무관, 이중 하나만 true
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsCampAnimInstanceState")
	bool IsIdle = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsCampAnimInstanceState")
	bool IsDamage = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsCampAnimInstanceState")
	bool IsDestroy = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsCampAnimInstanceState")
	bool IsBuilding = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsCampAnimInstanceState")
	bool IsCompleteBuild = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsCampAnimInstanceState")
	int CurrentStep = 0;

	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsCampAnimInstanceState")
	EGsStateCamp CampStateType = EGsStateCamp::None;

public:
	FGsCampAnimInstanceStateProxy() : FAnimInstanceProxy() {}
	FGsCampAnimInstanceStateProxy(UAnimInstance* Instance) : FAnimInstanceProxy(Instance) {}

public:
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual void PostEvaluate(UAnimInstance* InAnimInstance) override;

private:
	void ChangeStateToBPAnimationState(EGsStateCamp State);
	void InitializeAnimationState();
};


UCLASS()
class T1PROJECT_API UGsCampAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	friend struct FGsCampAnimInstanceStateProxy;

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsCampAnimInstanceState", meta = (AllowPrivateAccess = "true"))
	EGsStateCamp CurrentStateType = EGsStateCamp::None;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsCampAnimInstanceState", meta = (AllowPrivateAccess = "true"))
	FGsCampAnimInstanceStateProxy _campAnimStateProxy;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsCampAnimInstanceState")
	float _animSpeedValue = 1.0f;

protected:
	int _currentStep = 0;
	float _time{ 0.f };

private:
	bool _isAllowedBuildCampFlag = false;

public:
	UGsCampAnimInstance();
	virtual ~UGsCampAnimInstance();

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable, Category = "GsCampAnimInstanceState")
	void EndState(EGsStateCamp State);
	UFUNCTION(BlueprintCallable, Category = "GsCampAnimInstanceState")
	void ImpossibleBuildCamp();
	UFUNCTION(BlueprintCallable, Category = "GsCampAnimInstanceState")
	void PossibleBuildCamp();
	UFUNCTION(BlueprintCallable, Category = "GsCampAnimInstanceState")
	bool IsAllowedBuildCamp();

public:
	void SetAnimSpeed(float InSpeedValue) { _animSpeedValue = InSpeedValue; }
	void SetCampCurrentStep(int InStepNum);
	void SetIsBuildableCampFlag(bool InIsBuildable) { _isAllowedBuildCampFlag = InIsBuildable; }

public:
	void ResetCurrentState(FName FsmName);
	void ChangeState(EGsStateCamp State);

public:
	int GetCurStepNum() { return _currentStep; }
	EGsStateCamp GetCurStateType() { return CurrentStateType; }
	float GetStateAnimationLength(FName FsmName, FName StateName)
	{
		int idx = GetInstanceAssetPlayerIndex(FsmName, StateName);
		return GetInstanceAssetPlayerLength(idx);
	}

private:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}
};
