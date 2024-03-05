#include "GsActorSummonUnveilObject.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Components/SceneComponent.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"

const FName NameSummonUnveilObjectRootComponent = TEXT("Root");
const FName NameSummonUnveilObjectMeshComponent = TEXT("Mesh");
const FName NameSummonUnveilObjectMeshFaceComponent = TEXT("MeshFace");

const FName NameSummonUnveilObjectParticleSystemComponent = TEXT("Particle");

AGsActorSummonUnveilObject::AGsActorSummonUnveilObject()
	: Super()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(NameSummonUnveilObjectRootComponent);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(NameSummonUnveilObjectMeshComponent);
	Mesh->SetupAttachment(RootComponent);

	MeshFace = CreateDefaultSubobject<USkeletalMeshComponent>(NameSummonUnveilObjectMeshFaceComponent);
	MeshFace->SetupAttachment(Mesh);

	MeshFace->SetMasterPoseComponent(Mesh);

	_particleGrade = CreateDefaultSubobject<UParticleSystemComponent>(NameSummonUnveilObjectParticleSystemComponent);
	_particleGrade->SetupAttachment(Mesh);
}
void AGsActorSummonUnveilObject::BeginPlay()
{
	Super::BeginPlay();

	InitScale();

	
}

// hide Ã³¸®
void AGsActorSummonUnveilObject::SetHide(bool In_isHide)
{
	SetActorHiddenInGame(In_isHide);

	if (In_isHide == false)
	{
		_particleGrade->Activate(true);
	}
}

void AGsActorSummonUnveilObject::DestroySelf()
{
	Destroy();
}


void AGsActorSummonUnveilObject::Finalize()
{
	
}