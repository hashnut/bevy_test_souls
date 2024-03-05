#include "GsActorDecoNPC.h"

#include "Engine/Classes/Components/SkeletalMeshComponent.h"

// can't use structor timming
// get mesh component is null
// component init timming is later than actor structor

void AGsActorDecoNPC::BeginPlay()
{
	Super::BeginPlay();

	InitSkeletalMeshURO();
}

void AGsActorDecoNPC::GetSkeletalMeshs(TArray<USkeletalMeshComponent*>& Out_skeletalMesh)
{	
	GetComponents<USkeletalMeshComponent>(Out_skeletalMesh, true);
}

