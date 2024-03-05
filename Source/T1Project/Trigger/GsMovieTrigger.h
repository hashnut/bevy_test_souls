// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trigger/BaseTriggerActor.h"
#include "Message/GsMessageUI.h"
#include "GsMovieTrigger.generated.h"

/**
 *
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopMovie);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayMovie);

 class UBoxComponent;

UCLASS()
class T1PROJECT_API AGsMovieTrigger : public ABaseTriggerActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Movie")
		FString _movieName;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Change level actor")
		UBoxComponent* _boxCollision;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Change level actor")
		bool _isDeactiveSameMovie = true;

	UPROPERTY(BlueprintAssignable)
		FPlayMovie _playMovieEvent;
	UPROPERTY(BlueprintAssignable)
		FStopMovie _stopMovieEvent;
	MsgUIHandle _stopMovieHandle;

	bool _usedMessageStop = false;

public:
	AGsMovieTrigger();
	virtual void EndPlay(EEndPlayReason::Type inReason) override;

public:
	UFUNCTION()
		void OnPlayMovie(UPrimitiveComponent* inOverlappedComponent,
			AActor* inOtherActor,
			UPrimitiveComponent* inOtherComp,
			int32 inOtherBodyIndex,
			bool inIsFromSweep,
			const FHitResult& inSweepResult);
	
	void OnStopMovie(const struct IGsMessageParam*);

public:
	static void ActiveAllMovieTrigger(UWorld* inWorld, bool inTrigger = false, FString inMovieName = "");

#if WITH_EDITOR
protected:
	virtual void RefreshWidgetText() override;

public:
	virtual void SetVisible(bool inVisible) override;
#endif
};
