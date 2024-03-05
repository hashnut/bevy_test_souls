// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Animation/URO/GsUROInterface.h"
#include "GsSkeletalMeshActor.generated.h"

/**
 *
 */
class UCurveFloat;
class USkeletalMeshComponent;
class UGsCurveTimer;


UCLASS()
class T1PROJECT_API AGsSkeletalMeshActor : public AActor, public IGsUROInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;	

	UPROPERTY(EditAnywhere)
	FString MaterialParamName;

	UPROPERTY(EditAnywhere)
	UCurveFloat* MaterialParamValue;

private:
	UPROPERTY()
	UGsCurveTimer* _timer;

public:
	AGsSkeletalMeshActor();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type inEndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetSkeletalMeshs(TArray<USkeletalMeshComponent*>& Out_skeletalMesh) override;

public:
	USkeletalMeshComponent* GetSkeletalMeshComponent() { return SkeletalMeshComponent; }
};
