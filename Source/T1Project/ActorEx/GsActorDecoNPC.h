#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Animation/URO/GsUROInterface.h"

#include "GsActorDecoNPC.generated.h"

class USkeletalMeshComponent;

UCLASS()
class AGsActorDecoNPC : public AActor, public IGsUROInterface
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;

	// get func
public:
	virtual void GetSkeletalMeshs(TArray<USkeletalMeshComponent*>& Out_skeletalMesh) override;
};