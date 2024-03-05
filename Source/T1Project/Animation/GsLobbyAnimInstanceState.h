#pragma once

#include "CoreMinimal.h"
#include "GsAnimInstanceState.h"
#include "Animation/AnimInstanceProxy.h"
#include "GameFlow/Stage/GsStageMode.h"
#include "GsLobbyAnimInstanceState.generated.h"


class UAnimSequenceBase;
class AGsCharacterLobbyPlayer;

USTRUCT(BlueprintType)
struct FGsLobbyAnimInstanceStateProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, BlueprintReadWrite, Category = "GsAnimInstanceState")
	bool IsSpawn = false;
	UPROPERTY(Transient, BlueprintReadWrite, Category = "GsAnimInstanceState")
	bool IsIdle = false;
	UPROPERTY(Transient, BlueprintReadWrite, Category = "GsAnimInstanceState")
	bool IsSelecSlot = false;
	UPROPERTY(Transient, BlueprintReadWrite, Category = "GsAnimInstanceState")
	bool IsGameEnter = false;

	// 부가 사용 정보
public:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	int RandomIndex = 0;

public:
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsAnimInstanceState")
	bool IsActiveMontage = false;
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "GsAnimInstanceState")
	EGsStateBase StateType = EGsStateBase::None;

private:
	uint8 bCalcSkeletalMeshResize : 1;

	// animnode_lookcamera init data
public:
	AGsCharacterLobbyPlayer* _lobbyChar;
	FGsStageMode::Lobby _currentStageMode = FGsStageMode::Lobby::LOBBY_MAX;

public:
	FGsLobbyAnimInstanceStateProxy() : FAnimInstanceProxy() {}
	FGsLobbyAnimInstanceStateProxy(UAnimInstance* Instance) : FAnimInstanceProxy(Instance) {}

	FAnimNode_StateMachine* GetFirstMachine();

public:
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual void PostUpdate(UAnimInstance* InAnimInstance) const override;
	virtual void PostEvaluate(UAnimInstance* InAnimInstance) override;

protected:
	virtual void CacheBones() override;

private:
	void ChangeStateAnimation(EGsStateBase State);
	void InitializeStateAnimation();
};
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsLobbyAnimInstanceState : public UGsAnimInstanceState
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState", meta = (AllowPrivateAccess = "true"))
	FGsLobbyAnimInstanceStateProxy _useProxy; //단순 프록시

protected:
	UPROPERTY(Transient, BlueprintReadWrite, Category = "GsAnimInstanceState")
	float _animRemainTIme{ 0.f };
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	bool _animOnly{ false };

	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void ReCalcSkeletalMeshRelativeLocation();

	UFUNCTION(BlueprintCallable, Category = "GsAnimInstanceState")
	void EndState();

	void OnGameEnterAction();
	void OnChangeSelectSlotAction();

public:
	UAnimSequenceBase* GetAnimSequence(FName StateName, FName In_assetName);
	FAnimInstanceProxy* GetAnimInstanceProxy();
};
