#pragma once

#include "GsEventActionImmediatelyFinish.h"

#include "GsEventActionDimmedOff.generated.h"

UCLASS()
class UGsEventActionDimmedOff final : public UGsEventActionImmediatelyFinish
{
	GENERATED_BODY()

public:
	bool OnAfterPlay() final;
};
