#include "GsActorPlayableEventNPCAniPlay.h"

#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Components/SceneComponent.h"

const FName NamePlayableEventNPCAniPlayRootComponent = TEXT("Root");
const FName NamePlayableEventNPCAniPlayMeshComponent = TEXT("Mesh");
const FName NamePlayableEventNPCAniPlayMeshFaceComponent = TEXT("MeshFace");

AGsActorPlayableEventNPCAniPlay::AGsActorPlayableEventNPCAniPlay()
	: Super()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(NamePlayableEventNPCAniPlayRootComponent);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(NamePlayableEventNPCAniPlayMeshComponent);
	Mesh->SetupAttachment(RootComponent);

	MeshFace = CreateDefaultSubobject<USkeletalMeshComponent>(NamePlayableEventNPCAniPlayMeshFaceComponent);
	MeshFace->SetupAttachment(Mesh);

	MeshFace->SetMasterPoseComponent(Mesh);
}

// ani 상태 변경
void AGsActorPlayableEventNPCAniPlay::ChangeAniState(EGsStatePlayableEventNPC In_state)
{
	USkeletalMeshComponent* myBodyMesh = GetPartsSkeletalMeshComponentBody();
	if (myBodyMesh == nullptr)
	{
		return;
	}

	UGsPlayableEventNPCAnimInstance* animInstance =
		Cast<UGsPlayableEventNPCAnimInstance>(myBodyMesh->GetAnimInstance());

	if (animInstance == nullptr)
	{
		return;
	}

	animInstance->ChangeState(In_state);
}