#pragma once

UENUM()
enum class MessageSound : uint8
{
	PLAY_BGM,
	STOP_BGM,
	PAUSE_BGM,
};

using	MsgSoundHandle = TPair<MessageSound, FDelegateHandle>;
using	MsgSoundHandleArray = TArray<MsgSoundHandle>;