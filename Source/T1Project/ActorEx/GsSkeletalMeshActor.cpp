// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkeletalMeshActor.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
//#include "Util/GsTimeUtil.h"


AGsSkeletalMeshActor::AGsSkeletalMeshActor()
{
	PrimaryActorTick.bCanEverTick = true;


	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	if (SceneComponent)
	{
		SetRootComponent(SceneComponent);
		
	}

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	if(SkeletalMeshComponent)
	{
		SkeletalMeshComponent->SetupAttachment(SceneComponent);
	}
}

void AGsSkeletalMeshActor::BeginPlay()
{
	Super::BeginPlay();
	InitSkeletalMeshURO();
}

void AGsSkeletalMeshActor::EndPlay(const EEndPlayReason::Type inEndPlayReason)
{
	
	Super::EndPlay(inEndPlayReason);
}

void AGsSkeletalMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AGsSkeletalMeshActor::GetSkeletalMeshs(TArray<USkeletalMeshComponent*>& Out_skeletalMesh)
{
	Out_skeletalMesh.Add(GetSkeletalMeshComponent());
}