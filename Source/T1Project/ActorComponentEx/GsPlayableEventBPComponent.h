#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GsPlayableEventBPComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UGsPlayableEventBPComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = PlayableEventSpawn, EditAnywhere)
	int _deactiveQuestId;
	UPROPERTY(Category = PlayableEventSpawn, EditAnywhere)
	int _spawnId;
};