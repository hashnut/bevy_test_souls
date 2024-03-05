// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GsActorPropObject.h"
#include "GameObject/Interaction/GsActorInteractionInterface.h"
#include "GsActorDropObject.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UWidgetComponent;
class UGsDropObjectSpawnComponent;
class UParticleSystemComponent;

UCLASS()
//class T1PROJECT_API AGsActorDropObject : public AGsActorPropObject, public IGsActorInteractionInterface
class T1PROJECT_API AGsActorDropObject : public AGsActorPropObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* _mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UGsWidgetComponentButton* _widget;

	UPROPERTY()
	UGsDropObjectSpawnComponent* _spawnComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* _sphereCollision = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystemComponent* _gradeParticleSystemComponent;

	// 2022/08/01 PKT - 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystemComponent* _dropItemEffectParticleSystemComponent;
	
public:	
	// Sets default values for this actor's properties
	AGsActorDropObject();
	void SetInteractionClickCallback(TFunction<void(bool)> In_callback);

protected:
	// Called when the game starts or when spawned	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type inReason) override;

public:
	UStaticMeshComponent* GetMesh(){return _mesh;}
	class UGsWidgetComponentButton* GetWidget(){return _widget;}
	void Active(const bool inSpawn) const;
	void StartSpawn(const FVector& inStart, const FVector& inDest);
	USphereComponent* GetSphereComponent() { return _sphereCollision; }
	//virtual void ClickInteraction() override;
};
