// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUROComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Cheat/GsCheatManager.h"
#include "T1Project.h"


const float DEFAULT_ANIMATION_RATE = 30.0f;
const float DEFAULT_MAX_EVALRATE_FOR_INTERPORATION = 0.0f;

// Sets default values for this component's properties
UGsUROComponent::UGsUROComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UGsUROComponent::BeginPlay()
{
	Super::BeginPlay();	

//Disable URO because of URO policy not setted until M1 finish
	return;

	AActor* owner = GetOwner();
	if (nullptr == owner)
		return;

	ACharacter* character = Cast<ACharacter>(owner);
	if (nullptr == character)
		return;

	USkeletalMeshComponent* mesh = character->GetMesh();
	if (nullptr == mesh)
		return;
	
	mesh->bEnableUpdateRateOptimizations = true;
	mesh->bDisplayDebugUpdateRateOptimizations = UGsCheatManager::_isShowAnimURO;

	mesh->AnimUpdateRateParams->bShouldUseLodMap = true;

	//URO
	_lodToFrameSkipMap.Empty();
	_lodToFrameSkipMap.FindOrAdd(0) = _lod0Framerate;
	_lodToFrameSkipMap.FindOrAdd(1) = _lod1Framerate;
	_lodToFrameSkipMap.FindOrAdd(2) = _lod2Framerate;
	_lodToFrameSkipMap.FindOrAdd(3) = _lod3Framerate;
	_lodToFrameSkipMap.FindOrAdd(4) = _lod4Framerate;

	mesh->AnimUpdateRateParams->BaseNonRenderedUpdateRate = DEFAULT_ANIMATION_RATE;
	mesh->AnimUpdateRateParams->MaxEvalRateForInterpolation = DEFAULT_MAX_EVALRATE_FOR_INTERPORATION;
	mesh->AnimUpdateRateParams->ShiftBucket = EUpdateRateShiftBucket::ShiftBucket0;
	mesh->AnimUpdateRateParams->LODToFrameSkipMap = _lodToFrameSkipMap;
}