#pragma once

#include "Classes/GsMessageHandler.h"
#include "Classes/GsMessageHandlerOneParam.h"
// bak1210 
// 인풋에 관련된 메시지만 추가할것
UENUM()
enum class MessageInput : uint8
{
	PressSkill,  // 
	ReleaseSkill,
	ClickSkill,
	ViewChange,
	BlockInput,
	ReleaseJoystick,
	PressJoystick,
	TestPrintTouch,
	TerrainTouchMove,
	QuickExecute,
	TouchPress,
	TouchRelease,
	Window_keyboard_Action,			// 윈도우 키보드 액션
	UnmountVehicle, // 탈것 내리기
};

using	MsgInputHandle = TPair<MessageInput, FDelegateHandle>;
using	MsgInputHandleArray = TArray<MsgInputHandle>;

using	MInput = TGsMessageHandlerOneParam<MessageInput, const class FGsInputEventMsgBase&>;
using	MWindowInput = TGsMessageHandlerOneParam<MessageInput, const struct IGsMessageParam*>;
