#pragma once

#include "GsEventActionBase.h"

#include "GsEventActionDelay.generated.h"

UCLASS()
class UGsEventActionDelay : public UGsEventActionBase
{
	GENERATED_BODY()

private:
	float _delaySeconds = 0.0f;

public:
	bool OnAfterPlay() final;

public:
	void SetDelaySeconds(const float DelaySeconds);
};
