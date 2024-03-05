// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trigger/BaseTriggerActor.h"
#include "QuestAreaTriggerActor.generated.h"

class UGsEditorWidget;
class UWidgetComponentEx;
class UCapsuleComponent;

UCLASS(BlueprintType)
class T1PROJECT_API AQuestAreaTriggerActor : public ABaseTriggerActor
{
	GENERATED_BODY()

public:
	//Collision
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Change level actor")
	UCapsuleComponent* CapsulCollision;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Change level actor")
	int32 QuestID;

	//Widget
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Change level actor")
	int32 LevelKey;

public:
	// Sets default values for this actor's properties
	AQuestAreaTriggerActor();

#if WITH_EDITOR
protected:
	virtual void RefreshWidgetText() override;
#endif

public:
	UFUNCTION()
	void OnQuestStart(
		UPrimitiveComponent* in_overlappedComponent,
		AActor* in_otherActor,
		UPrimitiveComponent* in_otherComp,
		int32 in_otherBodyIndex,
		bool in_bFromSweep,
		const FHitResult& in_SweepResult
	);
};
