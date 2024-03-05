#pragma once
#include "GsAnimInstanceState.h"
#include "Animation/AnimInstanceProxy.h"
#include "GsCostumeAnimInstanceState.generated.h"

USTRUCT(BlueprintType)
struct FGsCostumeAnimInstanceStateProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient, BlueprintReadWrite, Category = "GsAnimInstanceState")
	bool IsSpawn = false;
	UPROPERTY(Transient, BlueprintReadWrite, Category = "GsAnimInstanceState")
	bool IsIdle = false;
	UPROPERTY(Transient, BlueprintReadWrite, Category = "GsAnimInstanceState")
	bool IsWearOn = false;
	UPROPERTY(Transient, BlueprintReadWrite, Category = "GsAnimInstanceState")
	bool IsWearChange = false;
public:

	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsAnimInstanceState")
	bool IsActiveMontage = false;
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsAnimInstanceState")
	EGsStateBase StateType = EGsStateBase::None;

public:
	FGsCostumeAnimInstanceStateProxy() : FAnimInstanceProxy() {}
	FGsCostumeAnimInstanceStateProxy(UAnimInstance* Instance) : FAnimInstanceProxy(Instance) {}

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
 * 캐릭터의 애님 상속하여 코스춤에서 사용 - GsCharacterBase 인 경우 UGsAnimInstanceState로서 참조되는 곳이 많아, 상속받아 제작
 * 블루프린트에서 AnimationEnd시 EndState하여 state를 호출하도록 설정 - 애님 state machine 을 사용하지 않음
 */
UCLASS()
class T1PROJECT_API UGsCostumeAnimInstanceState : public UGsAnimInstanceState
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient, BlueprintReadWrite, Category = "GsAnimInstanceState")
	float _animRemainTIme{ 0.f };
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool _animOnly{ false };

private:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState", meta = (AllowPrivateAccess = "true"))
	FGsCostumeAnimInstanceStateProxy _useProxy; //단순 프록시

	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "GsAnimInstanceState")
	void StartState();
	UFUNCTION(BlueprintCallable, Category = "GsAnimInstanceState")
	void EndState();

	void ChangeCostumeState();
};