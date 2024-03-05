#pragma once

#include "GsEventActionImmediatelyFinish.h"

#include "GsEventActionPlaySound.generated.h"

struct FGsSchemaBase;
struct FGsSchemaSoundResData;

UCLASS()
class UGsEventActionPlaySound final : public UGsEventActionImmediatelyFinish
{
	GENERATED_BODY()

private:
	FName _soundResDataRowName;

public:
	void Initialize(const FGsSchemaBase* InEventActionData) final;

	bool OnAfterPlay() final;
};
