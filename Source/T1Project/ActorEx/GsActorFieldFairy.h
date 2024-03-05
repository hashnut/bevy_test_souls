#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fairy/GsSchemaFairyEffect.h"
#include "Animation/URO/GsUROInterface.h"
#include "GsActorFieldFairy.generated.h"

class UGsFairyEffectComponent;
class UGsFairyAnimInstance;
class UParticleSystemComponent;
class UParticleSystem;
class UGsMeshShaderControlComponent;
class UWidgetComponent;
enum class ETeleportEffectType : uint8;


/*
* Object Class가 가지는 FSM State 만큼의 기능을 할 필요가 없으므로,
* Object Class 없이, skill이나 spawn 때 간단하게 몽타주 play만 하고, idle 상태에 머무른다
*/

DECLARE_DELEGATE(FFairyDespawnAnimEnd);

UCLASS()
class T1PROJECT_API AGsActorFieldFairy : public AActor, public IGsUROInterface
{
	GENERATED_BODY()

//private:
//	FTimerHandle _desapwnTimer;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Etc")
	void CallBlueprintApplyLodLevelDown(bool flag);

protected:
	UPROPERTY(Category = "GsActor", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* _mesh;

	UPROPERTY(Category = "GsActor", VisibleAnywhere, BlueprintReadWrite, EditFixedSize, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* _meshFace;

	UPROPERTY(Category = "Effect", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGsFairyEffectComponent* _skillEffectComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UGsMeshShaderControlComponent* _meshShaderComponent;

	UPROPERTY(EditAnywhere, Category = "Effect")
	FSoftObjectPath _fairySpawnEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	FSoftObjectPath _fairyTeleportStartMontagePath;

	UPROPERTY(EditAnywhere, Category = "Effect")
	FSoftObjectPath _fairyTeleportArriveMontagePath;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TArray<UParticleSystem*> _fairyTeleportParticleList;

	// 말풍선
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* _talkBalloonWidget = nullptr;


private:
	float specialState = 0.f;
	float _fairyScale{ 1.0f };
	bool _isAsyncSkillEffect = true;
	TArray<UParticleSystemComponent*> _baseParticles;

	const FGsSchemaFairyEffect* _skillEffect{ nullptr };
	const FGsSchemaFairyEffect* _vehicleEffect{ nullptr };

public:
	// Sets default values for this actor's properties
	AGsActorFieldFairy();

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reaseon) override;
	virtual void Destroyed() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	virtual void GetSkeletalMeshs(TArray<USkeletalMeshComponent*>& Out_skeletalMesh) override;
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return _mesh; }
	UWidgetComponent* GetFairyTalkWidget() const { return _talkBalloonWidget; }

	UGsFairyAnimInstance* GetAnim();

	void SetActorHiddenInGame(bool bNewHidden) override;

	UFUNCTION(BlueprintCallable)
	void ChangeLightingChannels(bool channel1 = true, bool channel2 = false, bool channel3 = false);

	// BP 노드 통신 함수 추가
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateFairyOwner(bool isLocalPlayerFairy);

	//UFUNCTION(BlueprintCallable)
	//void AnimNoti_FairySkillEffect();

	UFUNCTION(BlueprintCallable)
	bool AnimNoti_AttachFairySkillEffect(FName socket, FVector scale, FVector locOffset, FRotator rotOffset, USkeletalMeshComponent* MeshComp = nullptr);

	UFUNCTION(BlueprintCallable)
	UParticleSystem* GetPSTemplate();

	void OnActiveVehicleEffect();
	void OnDeactiveVehicleEffect();

	// 워프 시, 애니메이션 상태를 변경
	void OnFairyWarpEffect();
	void OnFairySpawnEffect();

	// 워프 시, 파티클을 보여주며 페어리 본체를 숨기는 노티파이에 대응
	void OnFairyTeleportParticleEffect(ETeleportEffectType teleportEffectType, FName socket, FVector scale, FVector locOffset, FRotator rotOffset, USkeletalMeshComponent* MeshComp = nullptr, bool attached = true);

protected:
	void InitActorAction();

private:
	UFUNCTION()
	void OnDestroy();

	void ClearParticle();

	//get set
public:
	void SetSpawn(bool instant = true);
	void SetDespawn(bool instant = true, FFairyDespawnAnimEnd endSpawn = nullptr);
	void SetSkill(bool instant = true); //const FSoftObjectPath& effectPath);
	void SetSelected(bool instant = true); //UI 에서만 사용한다.
	void SetSpecial();

	void SetIsMove(bool isMove = false);
	void SetIsVehicleMove(bool isMove = false);
	void SetIsWaiting(bool isWaiting = false);
	void SetBaseEffect(const TArray<FGsSchemaFairyEffect>& baseEffectData, 
		const FGsSchemaFairyEffect* skillEffect, const FGsSchemaFairyEffect* vehicleEffect);

	// 데이터 세팅
public:
	void SetTeleportEffect(const FSoftObjectPath& fairyTeleportStartMontage, const FSoftObjectPath& fairyTeleportArriveMontage, const TArray<UParticleSystem*>& fairyTeleportParticleList);
};

