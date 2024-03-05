
#pragma once

#include "IDelegateInstance.h"
#include "Classes/GsMessageHandler.h"
#include "Classes/GsMessageHandlerOneParam.h"

// bak1210 
// 컨텐츠 레이어의 메시지는 여기에 추가합니다.
// 게임로직에서 활용되는 메시지

UENUM()
enum class MessageContentPKBook : uint16
{
	NONE,								// 2022/09/21 PKT - 프로세스 상 필요해서 정의( 하는 일 없음.)
	UI_INVALID_RECORD,					// 2022/09/20 PKT - Record Data
	UI_OPEN_DETAIL_POPUP,				// 2022/09/20 PKT - record Detail Data
	UI_INVALID_RECORD_DETAILDATA,		// 2022/09/20 PKT - Invlid record Detail Data

	UI_INVALID_DETECTIONTARGET,			// 2022/09/20 PKT - Detection Target Data

	UI_INVALID_DETECTIONTARGET_LIST,	// 2022/09/20 PKT - List Detection Target Data

	ON_CLOSE_TARGET_DECTION_ALERT,							// 2022/09/30 PKT - 알람
};

// 공유 선언
using	MsgPKBookHandle = TPair<MessageContentPKBook, FDelegateHandle>;
using	MsgPKBookArray = TArray<MsgPKBookHandle>;
