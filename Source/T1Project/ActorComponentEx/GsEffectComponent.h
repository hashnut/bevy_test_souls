#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Classes/Particles/WorldPSCPool.h"
#include "Engine/Classes/Engine/EngineTypes.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Public/Containers/Map.h"

#include "GsEffectComponent.generated.h"

/*
	파티클 출력 컴퍼넌트
*/
class UParticleSystem;
class UParticleSystemComponent;

// 빔파티클 정보 구조체
USTRUCT(BlueprintType)
struct FGsBeamParticleEffectInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	class UParticleSystemComponent* _particle = nullptr;
	UPROPERTY()
	class AActor* _targetActor = nullptr;
	UPROPERTY(EditDefaultsOnly)
	FName _sourceSocket = TEXT("BeamSourceSocket");
	UPROPERTY()
	FName _targetSocket;
	UPROPERTY(EditDefaultsOnly)
	TArray<int32> _listEmitterIndex;

	// 타겟을 잃었을경우 이걸 쓴다...
	FVector _lastTargetPos = FVector::ZeroVector;

	float _lifeTime = 0.f;

	bool IsValid();	
};

//pair관리후 e.Value.Key 보다는 명확한것이 좋을 듯 하여 구조체
USTRUCT(BlueprintType)
struct FGsParticleByPart
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	class UParticleSystemComponent* Active;

	FName _socketName = NAME_None;
public:
	FName SocketName() { return _socketName; }
	bool UseSocket() { return !_socketName.IsNone(); }	
};

//pair관리후 e.Value.Key 보다는 명확한것이 좋을 듯 하여 구조체
USTRUCT(BlueprintType)
struct FGsSoketParticleArray
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<FGsParticleByPart> ActiveParts;
public:
	void Clear();
	void Activate();
	void Deactivate();
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class T1PROJECT_API UGsEffectComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	
	// 이펙트 붙일 타겟
	UPROPERTY(Transient)
	USceneComponent*	_attachTarget;
	bool				_enable = false;

	// 디버프 파티클
	UPROPERTY(EditDefaultsOnly, Category = "GsParticle")
	UParticleSystemComponent* _debuffParticle = nullptr;

	// abnormality 이펙트(represent) 대표. 부위별로 listing. 부위없는 것은 다 출력
	UPROPERTY()
	TMap<int32, FGsSoketParticleArray> _mapAbnormalityEffectByParts;

	// BeamParticle 정보
	UPROPERTY(EditDefaultsOnly, Category = "GsParticle")
	FGsBeamParticleEffectInfo _beamParticleInfo;	

	// 애드 이펙트 정보
	UPROPERTY(EditDefaultsOnly, Category = "GsParticle")
	UParticleSystemComponent* _attackCognitionEffect = nullptr;

	// 탈것 버프 이펙트
	UPROPERTY(EditDefaultsOnly, Category = "GsParticle")
	UParticleSystemComponent* _vehicleEffect = nullptr;

	// 애드 이펙트 관리 타이머
	float _attackCognitionTime = 0.f;
	float _attackCognitionEndTime = 1.f;

	// 생성자
public:
	UGsEffectComponent();
	virtual ~UGsEffectComponent();

	void ClearResource();
	// 가상함수
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginDestroy() override;

	// 로직함수
public:
	// 이펙트 출력
	UParticleSystemComponent* OnEffect(
		UParticleSystem* In_particle, bool In_isAutoDestroy = true,
		EPSCPoolMethod In_poolType = EPSCPoolMethod::AutoRelease,
		EAttachLocation::Type In_locationType = EAttachLocation::Type::KeepRelativeOffset,
		FVector In_location = FVector(ForceInit),
		FVector In_Scale = FVector::OneVector,
		FName In_attachPointName = NAME_None,
		bool In_isMeshAttach = false);

	// 위치에 이펙트 출력
	UParticleSystemComponent* OnEffectPos(UParticleSystem* In_particle, const FVector& In_pos, const FVector& In_Scale,
		EPSCPoolMethod In_poolType = EPSCPoolMethod::AutoRelease);

	// 소켓 위치에 이펙트 출력
	UParticleSystemComponent* OnEffectSocketPos(UParticleSystem* In_particle, FName In_socketName,
		float InScaleMultiplier = 1.0f, EPSCPoolMethod In_poolType = EPSCPoolMethod::AutoRelease);

	void OnDebuffActive(UParticleSystem* In_particle = nullptr);
	void OnDebuffDeactive();

	// 대표 이펙트 출력
	void OnPresentAbnormalityEffectActive(int32 In_key, UParticleSystem* In_particle,
		EGsAbnormalityEffectParticleType attachType = EGsAbnormalityEffectParticleType::EmitterAttached,
		FName In_socketName = NAME_None, FVector In_posOffset = FVector::ZeroVector, 
		FVector In_Scale = FVector::OneVector, EPSCPoolMethod In_PoolType = EPSCPoolMethod::ManualRelease);
	void OnPresentAbnormalityEffectDeactive(int32 In_key, FName In_socketName = NAME_None);
	void OnPresentAbnormalityEffectDeactive(int32 In_key, UParticleSystemComponent* deactiveComponent);
	void OnAllPresentAbnormalityEffectsDeactive(int32 In_key);


	// 애드 이펙트
	UParticleSystemComponent* OnCognitionTargetEffectActive(class UParticleSystem* In_Particle,
		const FName& In_TargetSocketName, float In_DestoryTime, const FVector& In_posOffset);
	void OnCognitionTargetEffectDeActive();
	bool IsCognitionTargetEffectActive() const;

	// 탈것 버프 이펙트
	UParticleSystemComponent* OnVehicleEffectActive(class UParticleSystem* In_Particle);
	void OnVehicleEffectDeActive();
	bool IsVehicleEffectActive();

	// 붙일 타겟 찾기
	void FindAttachTarget();
	// represent abnormality effect visible
	void SetRepresentAbnormalityEffectVisible(bool In_isVisible);

public:
	void OnBeamParticleEffectActive(class UParticleSystem* In_particle, class AActor* In_Target, const FName& In_TargetSocketName, 
		float In_LifeTime = 0.f);
	void OnBeamParticleEffectDeactive();

private:
	void UpdateBeamParticleEffect(float DeltaTime);

	UFUNCTION()
	void OnTargetActorDestroyed(AActor* DestroyedActor);
};