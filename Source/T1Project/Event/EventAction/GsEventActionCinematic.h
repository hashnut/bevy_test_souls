#pragma once

#include "Runtime/Core/Public/Containers/Map.h"
#include "Runtime/Core/Public/Delegates/IDelegateInstance.h"

#include "GsEventActionBase.h"
#include "Message/GsMessageSystem.h"

#include "GsEventActionCinematic.generated.h"

struct FGsSchemaBase;
struct IGsMessageParam;

UCLASS()
class UGsEventActionCinematic final : public UGsEventActionBase
{
	GENERATED_BODY()

private:
	int32 _sequenceResId;
	MsgSystemHandle _SequencePlayerStopHandle;

public:
	void BeginDestroy() final;

public:
	void Initialize(const FGsSchemaBase* EventActionData) final;
	bool ShouldReplayOnReconnection() const final;
	bool OnPlay() final;
	void OnFinish() final;
	bool OnRewind() final;

private:
	void AddObserver();
	void RemoveObserver();
	void OnStopMovie(const IGsMessageParam* Param);
};
