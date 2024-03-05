#pragma once

#include "EGsPartsType.generated.h"

/**
* 캐릭터 파츠 타입
*/
UENUM(BlueprintType)
enum class EGsPartsType : uint8
{
	FACE,			// 얼굴
	HAIR,			// 머리
	BODY,			// 코스튬 바디
	WEAPON,			// 무기
	
	MAX,

	START = FACE	
};