#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GsActorPlayableEventBPSpawn.generated.h"

UCLASS()
class AGsActorPlayableEventBPSpawn : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(Category = PlayableEventSpawn, EditAnywhere)
	int _spawnId;
	UPROPERTY(Category = PlayableEventSpawn, EditAnywhere)
	int _activeQuestId;
	UPROPERTY(Category = PlayableEventSpawn, EditAnywhere)
	int _deactiveQuestId;
	UPROPERTY(Category = PlayableEventSpawn, EditAnywhere)
	FSoftObjectPath _spawnTargetBP;

};