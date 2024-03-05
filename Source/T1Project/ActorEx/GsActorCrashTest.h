// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsActorCrashTest.generated.h"

//쉽핑 모드에서는 치트가 사용되지 않기 때문에 크래쉬릭이 작동하는지 알기 어렵다
//특정 조건을 사용해 강제로 크래쉬를 일으켜 크래쉬 리포트가 쉽핑에서도 작동하는지 확인한다
UCLASS()
class T1PROJECT_API AGsActorCrashTest : public AActor
{
	GENERATED_BODY()	

public:
	UPROPERTY(EditAnywhere)
	class USphereComponent* _sphereCollision;

private:
	bool _isCrashTest;
	FString _crashltyicParam;
	
public:	
	// Sets default values for this actor's properties
	AGsActorCrashTest();

protected:
	// Called when the game starts or when spawned	
	virtual void BeginPlay() override;	


public:
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* inOverlappedComponent,
			AActor* inOtherActor,
			UPrimitiveComponent* inOtherComp,
			int32 inOtherBodyIndex,
			bool inIsFromSweep,
			const FHitResult& inSweepResult);
};
