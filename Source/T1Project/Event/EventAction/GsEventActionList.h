#pragma once

#include "../GsEventActionSequencer.h"
#include "GsEventActionBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsEventActionList.generated.h"

UCLASS()
class UGsEventActionList final : public UGsEventActionBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<UGsEventActionBase*> _eventActions;

public:
	const int32 GetEventActionNum() const;
	bool GetValidEventActionAt(const EventActionIndex Index, OUT UGsEventActionBase*& OutEventAction) const;
	void AddEventAction(UGsEventActionBase* EventAction);
};