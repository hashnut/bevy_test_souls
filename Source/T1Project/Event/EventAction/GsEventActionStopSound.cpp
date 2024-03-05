#include "GsEventActionStopSound.h"

#include "GsSchemaBase.h"
#include "Management/GsScopeGlobal.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Sound/GsSoundPlayer.h"
#include "Management/ScopeGame/GsEventManager.h"
#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"

void UGsEventActionStopSound::Initialize(const FGsSchemaBase* InEventActionData)
{
	Super::Initialize(InEventActionData);

	if (nullptr == InEventActionData)
	{
		GSLOG(Error, TEXT("nullptr == InEventActionData"));
	}
	else
	{
		_soundResDataRowName = InEventActionData->GetRowName();
		GSLOG(Log, TEXT("_soundResDataRowName: %s"), *_soundResDataRowName.ToString());
	}
}

bool UGsEventActionStopSound::OnAfterPlay()
{
	if (_soundResDataRowName.IsNone())
	{
		GSLOG(Error, TEXT("nullptr == _soundResData"));
		return false;
	}

	UGsSoundPlayer* SoundPlayer = GSoundPlayer();
	if (nullptr == SoundPlayer)
	{
		GSLOG(Error, TEXT("nullptr == SoundPlayer"));
		return false;
	}

	UGsEventManager* EventManager = GSEvent();
	if (nullptr == EventManager)
	{
		GSLOG(Error, TEXT("nullptr == EventManager"));
		return false;
	}

	const uint64 SoundHandle = EventManager->DequeuePlayingSound(_soundResDataRowName);
	if (0 >= SoundHandle)
	{
		GSLOG(Error, TEXT("0 >= SoundHandle"));
		return false;
	}

	SoundPlayer->StopSound(SoundHandle);
	return true;
}
