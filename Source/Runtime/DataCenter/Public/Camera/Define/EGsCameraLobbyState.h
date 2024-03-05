#pragma once

#include "EGsCameraLobbyState.generated.h"

// 카메라 로비 스테이트 타입
UENUM(BlueprintType)
enum class EGsCameraLobbyState :uint8
{
	NONE,
	CHARACTER_SELECT,				// 캐릭터 선택
	CHARACTER_CUSTOMIZING,			// 캐릭터 커스터 마이징
};
