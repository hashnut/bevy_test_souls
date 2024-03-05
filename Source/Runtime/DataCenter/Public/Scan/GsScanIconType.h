#pragma once

#include "GsScanIconType.generated.h"

/**/
UENUM(BlueprintType)
enum class EGsScanIconType : uint8
{
	None			= 0,				// 없음
	Raid			= 1,				// 레이드
	Boss			= 2,				// 보스
	Named			= 3,				// 네임드
	MainQuest		= 4,				// 메인 퀘스트
	SubQuest		= 5,				// 서브 퀘스트
	RepeatQuest		= 6,				// 반복 퀘스트
	TreasureMonster = 7,				
};