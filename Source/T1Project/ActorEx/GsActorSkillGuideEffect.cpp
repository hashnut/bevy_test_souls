// Fill out your copyright notice in the Description page of Project Settings.


#include "GsActorSkillGuideEffect.h"
#include "Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
//#include "Classes/Materials/MaterialInstanceConstant.h"

AGsActorSkillGuideEffect::AGsActorSkillGuideEffect()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent0"));

	_meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent0"));
	_meshComponent->SetupAttachment(RootComponent);

	// GarbageCollection 관련 설정
	// 활성화시키면, Actor Clustering에 병합되며 레벨 언로드시 모두 파괴된다
	//bCanBeInCluster = true;
}

void AGsActorSkillGuideEffect::BeginPlay()
{
	Super::BeginPlay();
	_bConstantMaterial = false;
}

void AGsActorSkillGuideEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (false == _bConstantMaterial)
	{
		SetGauge(1.f - GetLifeSpan() / InitialLifeSpan);
	}
}

void AGsActorSkillGuideEffect::SetGauge(float Value)
{
	ScalarParameterValue(_scalarParameterDynamicRange, Value);
}

void AGsActorSkillGuideEffect::SetMinRadius(float Value)
{
	ScalarParameterValue(_scalarParameterMinRadius, Value);
}

void AGsActorSkillGuideEffect::SetMaxRadius(float Value)
{
	ScalarParameterValue(_scalarParameterMaxRadius, Value);
}

void AGsActorSkillGuideEffect::SetDegree(float Value)
{
	ScalarParameterValue(_scalarParameterDegree, Value);
}

void AGsActorSkillGuideEffect::ScalarParameterValue(const FName& ParameterName, float Value)
{
	if (_meshComponent)
	{
		_dynamicMaterial = _meshComponent->CreateAndSetMaterialInstanceDynamic(0);
		if (_dynamicMaterial)
		{
			_dynamicMaterial->SetScalarParameterValue(ParameterName, Value);
		}
	}
}

void AGsActorSkillGuideEffect::BeginDestroy()
{
	Super::BeginDestroy();
}


#if WITH_EDITOR
void AGsActorSkillGuideEffect::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	if (GetWorld() && !GetWorld()->IsGameWorld())
	{
		SetGauge(_guageValue);
	}
}
#endif