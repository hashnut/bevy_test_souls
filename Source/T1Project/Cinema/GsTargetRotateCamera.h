// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "GsTargetRotateCamera.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API AGsTargetRotateCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Camera")
	AActor* _target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float _degree = 45.0f;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Camera")
	FVector _initialRotateVector;
	UPROPERTY(BlueprintReadOnly, Category = "Camera")
	FVector _currentRotateVector;

public:
	AGsTargetRotateCamera();
	virtual void BeginPlay() override;
	virtual void Tick(float inDelta) override;
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void Active(bool inInit);	
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void Deactive();	
};
