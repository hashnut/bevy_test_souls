#pragma once

#include "GsEventActionImmediatelyFinish.h"

#include "GsEventActionDimmedOn.generated.h"

UCLASS()
class UGsEventActionDimmedOn final : public UGsEventActionImmediatelyFinish
{
	GENERATED_BODY()

public:
	bool OnAfterPlay() final;
};
