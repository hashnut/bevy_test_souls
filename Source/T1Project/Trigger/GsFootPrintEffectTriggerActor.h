// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trigger/BaseTriggerActor.h"
#include "GsFootPrintEffectTriggerActor.generated.h"

/**
 * 발자국 이펙트 변경용 트리거
 * 대상은 PC 
 */

class UGsPhysicalMaterial;

UCLASS()
class T1PROJECT_API AGsFootPrintEffectTriggerActor : public ABaseTriggerActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "FootPrint")
	USceneComponent* _Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FootPrint")
	UParticleSystem* FootPrintEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FootPrint")
	UGsPhysicalMaterial* FootPrintMaterial;

	//Collision
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "FootPrint")
	UShapeComponent* Collision;

public:
	AGsFootPrintEffectTriggerActor();

public:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:
	UFUNCTION()
		void OnCollsionBeginOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

	UFUNCTION()
		void OnCollsionEndOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		);

#if WITH_EDITOR
protected:
	virtual void RefreshWidgetText() override;

public:
	virtual void SetVisible(bool inVisible) override;
#endif
};
