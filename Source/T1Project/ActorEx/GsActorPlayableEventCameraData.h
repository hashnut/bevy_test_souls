#pragma once
#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "GsActorPlayableEventCameraData.generated.h"

UCLASS()
class T1PROJECT_API AGsActorPlayableEventCameraData : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = PlayableEventNonPlayerComponent, EditAnywhere)
	bool _isStartData;
};
