#pragma once

#include "GsEventActionBase.h"

#include "GsEventActionPlayMovie.generated.h"

struct FGsSchemaBase;
struct IGsMessageParam;

UCLASS()
class UGsEventActionPlayMovie final : public UGsEventActionBase
{
	GENERATED_BODY()

private:
	FName _movieDataRowName;

public:
	void Initialize(const FGsSchemaBase* InEventActionData) final;
	bool ShouldReplayOnReconnection() const final;
	bool OnPlay() final;
	bool OnRewind() final;

protected:
	void OnMovieStopped();
};
