#include "GsEventActionList.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

const int32 UGsEventActionList::GetEventActionNum() const
{
	return _eventActions.Num();
}

bool UGsEventActionList::GetValidEventActionAt(const EventActionIndex Index, OUT UGsEventActionBase*& OutEventAction)
	const
{
	if (0 <= Index && Index < _eventActions.Num())
	{
		OutEventAction = _eventActions[Index];
		return true;
	}

	OutEventAction = nullptr;
	return false;
}

void UGsEventActionList::AddEventAction(UGsEventActionBase* EventAction)
{
	_eventActions.Add(EventAction);
}
