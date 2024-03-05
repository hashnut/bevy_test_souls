#pragma once

#include "EGsDialogCameraViewType.generated.h"

// 대사 카메라 뷰 타입
UENUM(BlueprintType)
enum class EGsDialogCameraViewType :uint8 
{
	None,						// 초기화 된 키값 그대로 쓰면 에러나서 넣음
	DialogBackLocalSingleNpc,	// 로컬 뒤로, npc 하나
	DialogBackNpc,				// npc 뒤로
	DialogSide,					// 옆으로
	DialogGreetingLocal,		// 로컬 그리팅
	DialogGreetingNpc,			// npc 그리팅
	DialogBackLocalMultiNpc,	// 로컬 뒤로, npc 여럿
	DialogGreetingObject,		// 오브젝트 그리팅
	DialogGreetingLocalFairy,	// 로컬과 페어리 그리팅(중간점을 그리팅한다)
	DialogGreetingSpawnFairy,	// fairy greeting(fairy spawn)
	DialogGreetingNpcFairy,		// npc greeting (target fairy)
};
