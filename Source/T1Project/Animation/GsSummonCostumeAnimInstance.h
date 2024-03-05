#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimInstance.h"
#include "GsSummonCostumeAnimInstance.generated.h"

struct FAnimNode_StateMachine;

UENUM(BlueprintType)
enum class EGsStateSummonCostume : uint8
{
	None = 0,
	Spawn,
	Idle,
};


USTRUCT(BlueprintType)
struct FGsSummonCostumeAnimInstanceStateProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()
	// 멤버 변수
public:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsSummonCostumeAnimInstanceState")
	bool IsSpawn = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsSummonCostumeAnimInstanceState")
	bool IsIdle = false;
	
public:
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsSummonCostumeAnimInstanceState")
	EGsStateSummonCostume StateType = EGsStateSummonCostume::None;

	// 생성자
public:
	FGsSummonCostumeAnimInstanceStateProxy() :FAnimInstanceProxy() {}
	FGsSummonCostumeAnimInstanceStateProxy(UAnimInstance* Instance) :FAnimInstanceProxy(Instance) {}

public:
	virtual void PreUpdate(UAnimInstance* In_animInstance, float In_deltaSeconds) override;


private:
	void ChangeStateToBPAnimationState(EGsStateSummonCostume State);
	void InitializeAnimationState();
public:
	FAnimNode_StateMachine* GetFirstMachine();
};

UCLASS()
class T1PROJECT_API UGsSummonCostumeAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsSummonCostumeAnimInstanceState", Interp)
	EGsStateSummonCostume CurrentStateType = EGsStateSummonCostume::None;

private:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsSummonCostumeAnimInstanceState", meta = (AllowPrivateAccess = "true"))
	FGsSummonCostumeAnimInstanceStateProxy _proxy;

	bool _useAnimEnd{ false };
	float _playAnimTime{ 0.f };

	// 소멸자
public:
	UGsSummonCostumeAnimInstance() {}
	virtual ~UGsSummonCostumeAnimInstance() {}

	// 가상 함수
public:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 로직 함수
public:
	void ChangeState(EGsStateSummonCostume State);

	UFUNCTION(BlueprintCallable, Category = "GsSummonCostumeAnimInstanceState")
	void EndState(EGsStateSummonCostume State);

	// get
public:
	float GetCurrentAnimLength();
	bool IsCurrentAnimEnd();
};