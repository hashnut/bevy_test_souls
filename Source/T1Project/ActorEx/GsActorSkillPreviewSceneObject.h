// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GsActorSkillPreviewSceneObject.generated.h"

UCLASS()
class T1PROJECT_API AGsActorSkillPreviewSceneObject : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* _dummyOwner = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* _dummyPartyPlayer_Left = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* _dummyPartyPlayer_Right = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* _dummyEnemyPlayer_Center = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* _dummyEnemyPlayer_Left = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* _dummyEnemyPlayer_Right = nullptr;

	class UCameraComponent* _camera = nullptr;
	
public:	
	// Sets default values for this actor's properties
	AGsActorSkillPreviewSceneObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetFieldOfView_Camera(float Fov);
	void SetTransform_Camera(const FTransform& Tm);
	void SetTransform_Owner(const FTransform& Tm);
	void SetTransform_PartyPlayer_Left(const FTransform& Tm);
	void SetTransform_PartyPlayer_Right(const FTransform& Tm);
	void SetTransform_EnemyPlayer_Center(const FTransform& Tm);
	void SetTransform_EnemyPlayer_Left(const FTransform& Tm);
	void SetTransform_EnemyPlayer_Right(const FTransform& Tm);

public:
	class UCameraComponent* GetCameraComponent() const { return _camera; }
};
