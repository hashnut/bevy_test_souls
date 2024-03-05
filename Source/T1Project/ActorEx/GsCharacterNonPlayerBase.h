// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsCharacterBase.h"
#include "GameObject/Interaction/GsActorInteractionInterface.h"
#include "Quest/GsActorQuestTargetInterface.h"
#include "GsCharacterNonPlayerBase.generated.h"

class UBlackboardComponent;
class UGsUROComponent;
class USphereComponent;
class UWidgetComponent;
class UStaticMeshComponent;
class UGsUIBillboardTalkBalloon;

UCLASS()
class T1PROJECT_API AGsCharacterNonPlayerBase : public AGsCharacterBase, public IGsActorQuestTargetInterface,
	public IGsActorInteractionInterface
{
	GENERATED_BODY()

		//skill
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	UMaterialParameterCollection* _mapParameterCollection = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool _spawnEffectFlag = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UGsSkillCollisionComponent* _skillCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UGsSkillCollisioHandlerComponent* _skillCollisionHandle = nullptr;

	//target
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Decal", meta = (AllowPrivateAccess = "true"))
	UDecalComponent* _targetMaker = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Decal", meta = (AllowPrivateAccess = "true"))
	UDecalComponent* _shadowDecal = nullptr;

	//quest
protected:
	// 퀘스트 타겟 마크
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* _questTargetMark;

protected:
	// 말풍선
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* _talkBalloonWidget = nullptr;

	//URO
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character base", meta = (AllowPrivateAccess = "true"))
	UGsUROComponent* _uroComponent;

protected:
	// 인터랙션 클릭 콜백
	TFunction<void(bool)> _callbackInteractionClick;

public:
	// Sets default values for this character's properties
	AGsCharacterNonPlayerBase();

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type in_type) override;

protected:
	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void InitWidgetComponent() override;

protected:
	void InitializeDecals();

	//target
public:
	// 그림자데칼 보여질지 말지
	void SetVisibleShadowDecal(bool InVisible);

	virtual void SetVisibleTargetMaker(bool InVisible, class UMaterialInterface* InMaterial) override;

	virtual void OnHit(HitResultType ResultType, EGsStateBase CurrentState, 
		const struct FGsSchemaCreatureCommonInfo* CommonData) override;

	//quest
public:
	// 퀘스트 타겟 마크 접근
	virtual UStaticMeshComponent* GetQuestTargetMark() override { return _questTargetMark; }

	// 말풍선 위젯 컴포넌트 접근
	UWidgetComponent* GetTalkBalloonWidgetComponent() { return _talkBalloonWidget; }
	// 말풍선 위젯 쓸지
	void SetUseTalkBalloon(bool In_isUse);
	UGsUIBillboardTalkBalloon* GetTalkBalloon() const;
	void ShowTalkBalloonOnChangedState(uint8 InState);

public:
	// 인터랙션 클릭 콜백 세팅
	virtual void SetInteractionClickCallback(TFunction<void(bool)> In_callback) override;
	// 인터랙션 클릭
	virtual void ClickInteraction() override;

private:
	bool IsVisibleShadowDecal() const;

	// 스폰 연출 관련
	static const FName DISSOLVE_INTENSITY;
	static const FName GLOW_WIDTH;
	static const FName SELECT_GLOW_COLOR;
	static const FName GLOW_SECOND_COLOR;	

	// geter
	static const FName SPAWN_EFFECT_TIME;
	static const FName SPAWN_EFFECT_RANDOM_TIME;

	static const float GLOW_WIDTH_FACTOR;
	static const float GLOW_WIDTH_DEFAULT_FACTOR;
	static const float SELECT_GLOW_COLOR_FACTOR;
	static const float SELECT_GLOW_COLOR_DEFAULT_FACTOR;
	static const float DISSOLVE_START_DEFAULT_FACTOR;
	static const float DISSOLVE_END_DEFAULT_FACTOR;
	static const float DISSOLVE_TIME_DEFAULT_FACTOR;
	static const float SPAWN_EFFECT_RANDOM_TIME_DEFAULT_FACTOR;

	USkeletalMeshComponent* ApplySpawnEffectMesh;
	USkeletalMeshComponent* ApplySpawnEffectFaceMesh;
	UMaterialInstanceDynamic* TempDynamicMaterial;
	UMaterialInterface* TempDynamicOverlayMaterial;

	//FTimerHandle DelayTimerHandle;

	bool SpawnEffectPlay;
	float SpawnEffectTimeCur;
	float SpawnEffectTime;
	float SpawnDIStartValue;
	float SpawnDIEndValue;
	float SpawnEffectRandomTime;
	float SpawnEffectTotalDeltaTime;
	float SpawnRandomTime;

	void PlaySpawnEffect();
	void UpdateSpawnEffect(float DeltaTime);	
};
