#pragma once

#include "CoreMinimal.h"
#include "GsAnimInstanceState.h"
#include "Animation/AnimInstanceProxy.h"
#include "GsCommunityGirlAnimInstanceState.generated.h"

USTRUCT(BlueprintType)
struct FGsCommunityGirlAnimInstanceStateProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, BlueprintReadWrite, Category = "GsAnimInstanceState")
	bool IsSpawn = false;
	UPROPERTY(Transient, BlueprintReadWrite, Category = "GsAnimInstanceState")
	bool IsIdle = false;
	UPROPERTY(Transient, BlueprintReadWrite, Category = "GsAnimInstanceState")
	bool IsIdleBreak = false;
	UPROPERTY(Transient, BlueprintReadWrite, Category = "GsAnimInstanceState")
	bool IsSelectab = false;
	UPROPERTY(Transient, BlueprintReadWrite, Category = "GsAnimInstanceState")
	bool IsTouchAct = false;
public:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	int RandomIndex = 0;

public:
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsAnimInstanceState")
	bool IsActiveMontage = false;
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsAnimInstanceState")
	EGsStateBase StateType = EGsStateBase::None;

public:
	FGsCommunityGirlAnimInstanceStateProxy() : FAnimInstanceProxy() {}
	FGsCommunityGirlAnimInstanceStateProxy(UAnimInstance* Instance) : FAnimInstanceProxy(Instance) {}

	FAnimNode_StateMachine* GetFirstMachine();

public:
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual void PostUpdate(UAnimInstance* InAnimInstance) const override;
	virtual void PostEvaluate(UAnimInstance* InAnimInstance) override;

private:
	void ChangeStateAnimation(EGsStateBase State);
	void InitializeStateAnimation();
};
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsCommunityGirlAnimInstanceState : public UGsAnimInstanceState
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState", meta = (AllowPrivateAccess = "true"))
	FGsCommunityGirlAnimInstanceStateProxy _useProxy; //단순 프록시

protected:
	UPROPERTY(Transient, BlueprintReadWrite, Category = "GsAnimInstanceState")
	float _animRemainTIme{ 0.f };
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool _animOnly{ false };

	float _specialState{ 100.f };

public:
	TFunction<void(EGsStateBase)> _cbStartAction{ nullptr };

protected:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}

	UFUNCTION(BlueprintCallable, Category = "GsAnimInstanceState")
	void SetIdleBreak();

	UFUNCTION(BlueprintCallable, Category = "GsAnimInstanceState")
	void EndState();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void TouchAction(int touchIndex = 2, bool isRandom = true);
	void SelectTabAction();
};
