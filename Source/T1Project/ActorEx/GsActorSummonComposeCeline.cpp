#include "GsActorSummonComposeCeline.h"

#include "Engine/Classes/Components/SceneComponent.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"

const FName NameSummonComposeCelineRootComponent = TEXT("Root");
const FName NameSummonComposeCelineMeshComponent = TEXT("Mesh");
const FName NameSummonComposeCelineMeshFaceComponent = TEXT("MeshFace");

AGsActorSummonComposeCeline::AGsActorSummonComposeCeline() :Super()
{

	RootComponent = CreateDefaultSubobject<USceneComponent>(NameSummonComposeCelineRootComponent);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(NameSummonComposeCelineMeshComponent);
	Mesh->SetupAttachment(RootComponent);

	MeshFace = CreateDefaultSubobject<USkeletalMeshComponent>(NameSummonComposeCelineMeshFaceComponent);
	MeshFace->SetupAttachment(Mesh);

	MeshFace->SetMasterPoseComponent(Mesh);
}

// 드래그 값 변경
void AGsActorSummonComposeCeline::OnDragProgress(float In_progressVal, float In_preProgressVal)
{
	Mesh->SetPosition(In_progressVal);
}

void AGsActorSummonComposeCeline::SetAnimationPause()
{
	Mesh->Stop();
}