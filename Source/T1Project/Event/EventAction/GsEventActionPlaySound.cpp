#include "GsEventActionPlaySound.h"

#include "GsSchemaBase.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeGlobal.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsEventManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Sound/GsSoundPlayer.h"

void UGsEventActionPlaySound::Initialize(const FGsSchemaBase* InEventActionData)
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

bool UGsEventActionPlaySound::OnAfterPlay()
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

	const uint64 SoundHandle = SoundPlayer->PlaySound2D(_soundResDataRowName.ToString());
	if (0 >= SoundHandle)
	{
		GSLOG(Error, TEXT("0 >= SoundHandle"));
		return false;
	}

	UGsEventManager* EventManager = GSEvent();
	if (nullptr == EventManager)
	{
		GSLOG(Error, TEXT("nullptr == EventManager"));
		return false;
	}

	if (!EventManager->EnqueuePlayingSound(_soundResDataRowName, SoundHandle))
	{
		GSLOG(Error, TEXT("이벤트액션에서 사운드 플레이 실패하여 즉시 사운드 정지시킴. RowName: %s, SoundHandle:%llu"), *_soundResDataRowName.ToString(), SoundHandle);
		SoundPlayer->StopSound(SoundHandle);
		return false;
	}

	return true;
}
