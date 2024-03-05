// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trigger/BaseTriggerActor.h"
#include "ChangeLevelTriggerActor.generated.h"

class UGsEditorWidget;
class UBoxComponent;
class UWidgetComponentEx;

UCLASS(BlueprintType)
class T1PROJECT_API AChangeLevelTriggerActor : public ABaseTriggerActor
{
	GENERATED_BODY()
	
public:
	//Collision
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Change level actor")
	UBoxComponent* BoxCollision;
	//Widget
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Change level actor")
	int32 LevelKey;		

public:
	// Sets default values for this actor's properties
	AChangeLevelTriggerActor();

#if WITH_EDITOR
protected:
	virtual void RefreshWidgetText() override;
#endif

public:
	UFUNCTION()
	void OnChangeLevel(
		UPrimitiveComponent* in_overlappedComponent,
		AActor* in_otherActor,
		UPrimitiveComponent* in_otherComp,
		int32 in_otherBodyIndex,
		bool in_bFromSweep,
		const FHitResult& in_SweepResult
	);
};
