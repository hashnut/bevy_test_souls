// Fill out your copyright notice in the Description page of Project Settings.


#include "GsActorPropHit.h"
#include "WidgetComponent.h"
#include "Components/PrimitiveComponent.h"
#include "ActorComponentEx/GsMeshShaderControlComponent.h"
#include "UI/UIContent/Billboard/GsUIBillboardInteractionButton.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"

#define TEST_PROP_HIT true

AGsActorPropHit::AGsActorPropHit()
{
	PrimaryActorTick.bCanEverTick = true;

	_propMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	_propMesh->SetupAttachment(RootComponent);

	/*_questWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("QuestWidget"));
	_questWidgetComponent->SetupAttachment(RootComponent);	*/

	_questTargetMark = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("QuestMarkMesh"));
	if (_questTargetMark)
	{
		_questTargetMark->SetCastShadow(false);
		_questTargetMark->SetupAttachment(_propMesh);
	}

	_meshShaderComponent = CreateDefaultSubobject<UGsMeshShaderControlComponent>(TEXT("MeshShader"));
}

void AGsActorPropHit::BeginPlay()
{
	Super::BeginPlay();

#if TEST_PROP_HIT
	SetUseInteractionButton(true);
#endif
}

void AGsActorPropHit::EndPlay(EEndPlayReason::Type inReason)
{
	Super::EndPlay(inReason);
}

void AGsActorPropHit::ClickInteraction()
{
}

UGsUIBillboardInteractionButton* AGsActorPropHit::GetInteractionButton() const
{	
	return nullptr;
}

void AGsActorPropHit::SetUseInteractionButton(bool inUse)
{

}
