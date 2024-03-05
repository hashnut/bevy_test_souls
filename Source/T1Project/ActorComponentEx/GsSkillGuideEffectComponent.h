// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "Classes/Components/DecalComponent.h"
#include "DataSchema/Skill/GsSchemaSkillCollision.h"
#include "GsSkillGuideEffectComponent.generated.h"

struct FGsSchemaSkillNotifySkillGuide;
class AGsActorSkillGuideEffect;
class UMaterialInterface;

USTRUCT()
struct FGsSkillGuideDecalEffect
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	UDecalComponent* _validDecal = nullptr;
	UPROPERTY()
	UDecalComponent* _invalidDecal = nullptr;

	void Valid()
	{
		_validDecal->SetVisibility(true);
		_invalidDecal->SetVisibility(false);
		_validDecal->SetComponentTickEnabled(true);
		_invalidDecal->SetComponentTickEnabled(false);
	}

	void Invalid()
	{
		_validDecal->SetVisibility(false);
		_invalidDecal->SetVisibility(true);
		_validDecal->SetComponentTickEnabled(false);
		_invalidDecal->SetComponentTickEnabled(true);
	}

	void Off()
	{
		_validDecal->SetVisibility(false);
		_invalidDecal->SetVisibility(false);
		_validDecal->SetComponentTickEnabled(false);
		_invalidDecal->SetComponentTickEnabled(false);
	}
};

/**
* 스킬 예시선 처리 컴퍼넌트
*/
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class T1PROJECT_API UGsSkillGuideEffectComponent : public USceneComponent
{
	GENERATED_BODY()
	
	// 기본 스케일 적용 단위 (Radius)
	UPROPERTY(EditAnywhere)
	float _scaleUnit = 100.f;

	// ray max distance
	const float _rayCastMaxDistance = 1000000.f;

	// 일단 디버그용 정보 (UPROPERTY 가 붙어 #if WITH_EDITOR 사용할수 없는듯)
	UPROPERTY(EditAnywhere)
	FSoftObjectPath _debugPath;
	UPROPERTY(EditAnywhere)
	float _debugWidth = 100.f;
	UPROPERTY(EditAnywhere)
	float _debugHegiht = 100.f;
	UPROPERTY(EditAnywhere)
	float _debugRadius = 100.f;
	//

	float _elapsedTime = 0.f;
	
#if WITH_EDITOR
public:
	FSoftObjectPath				_preDebugPath;
	AActor*						_debugSpawnActor = nullptr;
#endif

private:
	// 데칼 처리용 관리
	UPROPERTY(EditAnywhere)
	FGsSkillGuideDecalEffect _decalEffect;

private:
	// 누르고 있는 동안 지속적으로 스폰되어 있는 타입
	AActor*							_preesActor = nullptr;
	UDecalComponent*				_preesDecal = nullptr;
	FVector _joyAxis;
	FVector _lastPosition;

public:	
	// Sets default values for this component's properties
	UGsSkillGuideEffectComponent();

public:
	virtual void BeginPlay() override;

public:
	// 월드 포지션 얻기
//	FVector GetLocation() const							{ return _attachScene->GetComponentLocation(); }
	// 현재 회전값 얻기
//	FRotator GetRotation() const						{ return _attachScene->GetComponentRotation(); }

	// 타입에 따른 스케일 정보
	FVector GetScale(const FGsSchemaSkillCollision& CollisionInfo);
	
	FVector GetJoyAxis() const							{ return _joyAxis; }
	AActor* GetPreesActor() const						{ return _preesActor; }
	UDecalComponent* GetPressDecal() const				{ return _preesDecal; }
	const FVector& GetLastPressPosition() const			{ return _lastPosition; }

public:
	void Set(const FGsSchemaSkillCollision& CollisionInfo, const FSoftObjectPath& EffectPath, float Duration, bool IsAttach = false);
	void Set(const FVector& Location, const FGsSchemaSkillCollision& CollisionInfo, const FSoftObjectPath& EffectPath, 
		float Duration);

protected:
	AGsActorSkillGuideEffect* SpawnGuideEffect(const FString& path, const FVector& Location, const FRotator& Rotator,
		const FVector& Scale, bool IsAttach = true, float LifeTime = 0.f
	);

	// 현재 좌표에 해당하는 지형 높이 얻기
	float GetSurface(const FVector2D& Point);

public:
	// 누르고 있는 상태 처리
	// Press타입 생성 처리
	AActor* OnStaticMeshEffect(const FGsSchemaSkillCollision& CollisionInfo,
		const FSoftObjectPath& GuideEffectPath);

	// 타겟 방향 지정
	void OnPressEffectToTarget(AActor* Target);
	// 현재 방향에서 최대 사거리 지정
	void OnPressEffectToMaxDistance(float MaxDistance);
	// 회전 처리
	void PressEffectRotator(const FVector& Pos, float MaxDistance);

	// Actor 소멸
	void ClearPressEffect();

#if WITH_EDITOR
public:
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
	AActor* Spawn(TSubclassOf<AActor> Actor, const FTransform& Transform);
#endif
};
