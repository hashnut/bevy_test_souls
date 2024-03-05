#include "GsEventActionDelay.h"

#include "T1Project.h"
#include "TimerManager.h"

bool UGsEventActionDelay::OnAfterPlay()
{
	const UWorld* World = GetWorld();
	if (nullptr == World)
	{
		GSLOG(Error, TEXT("nullptr == World"));
		return false;
	}

	FTimerHandle TimerHandle;
	World->GetTimerManager().SetTimer(TimerHandle, this, &UGsEventActionDelay::Finish, _delaySeconds, false);
	return true;
}

void UGsEventActionDelay::SetDelaySeconds(const float DelaySeconds)
{
	_delaySeconds = DelaySeconds;
}
