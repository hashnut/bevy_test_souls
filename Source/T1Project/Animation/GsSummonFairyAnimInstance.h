#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimInstance.h"
#include "GsSummonFairyAnimInstance.generated.h"


struct FAnimNode_StateMachine;

UENUM(BlueprintType)
enum class EGsStateSummonFairy : uint8
{
	None = 0,
	Spawn,
	Idle,
	Appear,
};

USTRUCT(BlueprintType)
struct FGsSummonFairyAnimInstanceStateProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()
	// 멤버 변수
public:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsSummonFairyAnimInstanceState")
	bool IsSpawn = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsSummonFairyAnimInstanceState")
	bool IsIdle = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsSummonFairyAnimInstanceState")
	bool IsAppear = false;
public:
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsSummonFairyAnimInstanceState")
	EGsStateSummonFairy StateType = EGsStateSummonFairy::None;

	// 생성자
public:
	FGsSummonFairyAnimInstanceStateProxy() :FAnimInstanceProxy() {}
	FGsSummonFairyAnimInstanceStateProxy(UAnimInstance* Instance) :FAnimInstanceProxy(Instance) {}

public:
	virtual void PreUpdate(UAnimInstance* In_animInstance, float In_deltaSeconds) override;
	

private:
	void ChangeStateToBPAnimationState(EGsStateSummonFairy State);
	void InitializeAnimationState();
public:
	FAnimNode_StateMachine* GetFirstMachine();
};

UCLASS()
class T1PROJECT_API UGsSummonFairyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsSummonFairyAnimInstanceState", Interp)
	EGsStateSummonFairy CurrentStateType = EGsStateSummonFairy::None;

private:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsSummonFairyAnimInstanceState", meta = (AllowPrivateAccess = "true"))
	FGsSummonFairyAnimInstanceStateProxy _proxy;

	bool _useAnimEnd{ false };
	float _playAnimTime{ 0.f };

	// 소멸자
public:
	UGsSummonFairyAnimInstance() {}
	virtual ~UGsSummonFairyAnimInstance() {}

	// 가상 함수
public:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 로직 함수
public:	
	void ChangeState(EGsStateSummonFairy State);

	UFUNCTION(BlueprintCallable, Category = "GsSummonFairyAnimInstanceState")
	void EndState(EGsStateSummonFairy State);

	// get
public:
	float GetCurrentAnimLength();
	bool IsCurrentAnimEnd();
};