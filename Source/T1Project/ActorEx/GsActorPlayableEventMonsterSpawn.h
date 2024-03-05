#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Classes/Engine/EngineTypes.h"
#include "GsActorPlayableEventMonsterSpawn.generated.h"

UCLASS()
class AGsActorPlayableEventMonsterSpawn : public AActor
{
	GENERATED_BODY()
public:
	//  »ç¿ëÇÒ skill id
	UPROPERTY(Category = PlayableEventNonPlayerComponent, EditAnywhere)
	int _useSkillId;

	// table id
	UPROPERTY(Category = PlayableEventNonPlayerComponent, EditAnywhere)
	int _tableId;

	UPROPERTY(Category = PlayableEventNonPlayerComponent, EditAnywhere)
	int _spawnId;

	UPROPERTY(Category = PlayableEventNonPlayerComponent, EditAnywhere)
	int _activeQuestId;
private:
	FTimerHandle _timerHandle;

	// logic func
public:
	void OnRequestRespawn();
	void RequestSpawnMonster();
	void ClearTimmer();
};