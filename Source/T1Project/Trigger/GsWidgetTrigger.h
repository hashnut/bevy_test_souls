// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trigger/BaseTriggerActor.h"
#include "GsWidgetTrigger.generated.h"

/**
 * 
 */
 class UBoxComponent;

UCLASS()
class T1PROJECT_API AGsWidgetTrigger : public ABaseTriggerActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Widget")
		FString _uiName;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Change level actor")
		UBoxComponent* _boxCollision;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Change level actor")
		bool _isDeactiveSameMovie = true;

public:
	AGsWidgetTrigger();

public:
	UFUNCTION()
		void OnShowWidget(UPrimitiveComponent* inOverlappedComponent,
			AActor* inOtherActor,
			UPrimitiveComponent* inOtherComp,
			int32 inOtherBodyIndex,
			bool inIsFromSweep,
			const FHitResult& inSweepResult);

public:
	static void ActiveAllWidgetTrigger(UWorld* inWorld, bool inTrigger = false, FString inMovieName = "");

#if WITH_EDITOR
protected:
	virtual void RefreshWidgetText() override;

public:
	virtual void SetVisible(bool inVisible) override;
#endif
	
};
