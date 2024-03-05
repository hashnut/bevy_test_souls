#pragma once

#include "CoreMinimal.h"

#include "Runtime/DataCenter/Public/PlayableEvent/GsStatePlayableEventNPCType.h"

#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimInstance.h"
#include "GsPlayableEventNPCAnimInstance.generated.h"

struct FAnimNode_StateMachine;



USTRUCT(BlueprintType)
struct FGsPlayableEventNPCAnimInstanceStateProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()
	// 멤버 변수
public:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsPlayableNPCAnimInstanceState")
	bool IsIdle = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsPlayableNPCAnimInstanceState")
	bool IsSpawn = false;	
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsPlayableNPCAnimInstanceState")
	bool IsCastingLooping = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsPlayableNPCAnimInstanceState")
	bool IsAttackLooping = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsPlayableNPCAnimInstanceState")
	bool IsAction = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsPlayableNPCAnimInstanceState")
	bool IsSkill = false;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsPlayableNPCAnimInstanceState")
	bool IsDamaged = false;
public:
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsSummonFairyAnimInstanceState")
	EGsStatePlayableEventNPC StateType = EGsStatePlayableEventNPC::None;

	// 생성자
public:
	FGsPlayableEventNPCAnimInstanceStateProxy() :FAnimInstanceProxy() {}
	FGsPlayableEventNPCAnimInstanceStateProxy(UAnimInstance* Instance) :FAnimInstanceProxy(Instance) {}

public:
	virtual void PreUpdate(UAnimInstance* In_animInstance, float In_deltaSeconds) override;


private:
	void ChangeStateToBPAnimationState(EGsStatePlayableEventNPC State);
	void InitializeAnimationState();
public:
	FAnimNode_StateMachine* GetFirstMachine();
};

UCLASS()
class T1PROJECT_API UGsPlayableEventNPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsPlayableNPCAnimInstanceState", Interp)
	EGsStatePlayableEventNPC CurrentStateType = EGsStatePlayableEventNPC::None;

private:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsPlayableNPCAnimInstanceState", meta = (AllowPrivateAccess = "true"))
	FGsPlayableEventNPCAnimInstanceStateProxy _proxy;

	bool _useAnimEnd{ false };
	float _playAnimTime{ 0.f };

	// 소멸자
public:
	UGsPlayableEventNPCAnimInstance() {}
	virtual ~UGsPlayableEventNPCAnimInstance() {}

	// 가상 함수
public:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 로직 함수
public:
	void ChangeState(EGsStatePlayableEventNPC State);

	UFUNCTION(BlueprintCallable, Category = "GsPlayableNPCAnimInstanceState")
	void EndState(EGsStatePlayableEventNPC State);

	// get
public:
	float GetCurrentAnimLength();
	bool IsCurrentAnimEnd();
};