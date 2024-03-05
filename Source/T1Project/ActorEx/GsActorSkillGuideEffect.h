// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GsActorSkillGuideEffect.generated.h"

class UMaterialInstanceDynamic;

/**
 * 스킬 예시선용 staticMeshActor 클래스
 */
UCLASS()
class T1PROJECT_API AGsActorSkillGuideEffect final : public AActor
{
	GENERATED_BODY()

	private:
	//  ScalarParameter 
	const FName _scalarParameterDynamicRange = TEXT("DynamicRange");
	const FName _scalarParameterDegree = TEXT("Degree");
	const FName _scalarParameterMaxRadius = TEXT("Max Radius");
	const FName _scalarParameterMinRadius = TEXT("Min Radius");
	
	//const FName ScalarParameterVal2 = TEXT("Percent");

public:
	UPROPERTY(EditAnywhere, Category = "SkillGuideEffect")
	float _guageValue = 0.f;

	UPROPERTY(Category = StaticMeshActor, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|StaticMesh", AllowPrivateAccess = "true"))
	class UStaticMeshComponent* _meshComponent;

	UPROPERTY()
	uint8 _bConstantMaterial : 1;

private:
	UPROPERTY()
	UMaterialInterface* _originMaterial = nullptr;

	UPROPERTY()
	UMaterialInstanceDynamic* _dynamicMaterial = nullptr;

public:
	AGsActorSkillGuideEffect();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:
	// 0.f ~ 1.f
	void SetGauge(float Value);
	void SetMinRadius(float Value);
	void SetMaxRadius(float Value);
	void SetDegree(float Value);

private:
	void ScalarParameterValue(const FName& ParameterName, float Value);
};