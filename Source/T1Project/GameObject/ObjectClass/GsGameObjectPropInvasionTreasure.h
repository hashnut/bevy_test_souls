
#pragma once

#include "GsGameObjectPropTouch.h"
#include "GsGameObjectPropInvasionTreasure.generated.h"

/**
* 침공전 보물 인터랙션 프랍
**/

UCLASS()
class T1PROJECT_API UGsGameObjectPropInvasionTreasure : public UGsGameObjectPropTouch
{
	GENERATED_BODY()

public:
	virtual void CreateSubClass() override;
	virtual void ActorDespawn() override;
	virtual bool IsForceHideInteractionButton() override;
};