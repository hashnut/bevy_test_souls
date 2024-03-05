// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GsUROComponent.generated.h"

const FName UROName = TEXT("UROComponent");

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class T1PROJECT_API UGsUROComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "URO")
	int32 _lod0Framerate = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "URO")
	int32 _lod1Framerate = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "URO")
	int32 _lod2Framerate = 2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "URO")
	int32 _lod3Framerate = 3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "URO")
	int32 _lod4Framerate = 4;

	//URO 
private:
	UPROPERTY(Transient)
	TMap<int32, int32> _lodToFrameSkipMap;

public:	
	// Sets default values for this component's properties
	UGsUROComponent();

public:
	virtual void BeginPlay() override;	
};
