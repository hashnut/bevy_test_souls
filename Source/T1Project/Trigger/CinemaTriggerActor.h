// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTriggerActor.h"
#include "GameFramework/Actor.h"
#include "CinemaTriggerActor.generated.h"

class UGsEditorWidget;
class UBoxComponent;
class UWidgetComponentEx;
class ULevelSequence;

UCLASS(BlueprintType)
class T1PROJECT_API ACinemaTriggerActor : public ABaseTriggerActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Change level actor")
	FString LevelSequenceID;
	//Collision
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Change level actor")
	UBoxComponent* BoxCollision;

public:
	// 디버깅용 로그 출력할지
	static bool IsShowDebugLog;

public:
	// Sets default values for this actor's properties
	ACinemaTriggerActor();

#if WITH_EDITOR
protected:
	void RefreshWidgetText();
#endif

public:
	UFUNCTION()
	void OnPlayCinema(
		UPrimitiveComponent* in_overlappedComponent,
		AActor* in_otherActor,
		UPrimitiveComponent* in_otherComp,
		int32 in_otherBodyIndex,
		bool in_bFromSweep,
		const FHitResult& in_SweepResult
	);
};
