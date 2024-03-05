#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

// 2021/11/10 PKT - 이 부분은 서버에게 요청해야 할 것.
enum class ENodeDirection
{
	None,		// 2021/11/10 PKT - 초기값
	Left,		
	Right,
	Top,
	Bottom,

	Max,
};

struct MonsterAmountKnowledgeData
{
	Level level;
	int32 requireAmount;
	int32 rewardDataId;
};


// 2021/11/10 PKT - window에서 사용하게 될 탭 네이밍
enum class EMonsterCollectionMainTab
{
	Codex,		// 2021/11/10 PKT - 몬스터 지식 수량 관련 탭
	Slate,			// 2021/11/10 PKT - 몬스터 지식의 석판 관련 탭

	Max,
};


enum class EMonsterStoneSlateNodeState
{
	Disable,	// 2021/12/13 PKT - slate lock
	Normal,		// 2021/12/13 PKT - slate unlock
	Complete,	// 2021/12/13 PKT - node complete

	Max,
};

enum class EMonsterKnowledgeState
{
	Normal,
	LevelUpPossible,
	FinalLevel,

	Max,
};


// 2021/11/09 PKT -임시 나중에 서버에게 요청
enum class EMonsterKnowledgeSlateState
{
	Lock,
	UnLock,
};


// 2021/11/10 PKT - StoneSlate Changed State ReasonF
enum class EMonsterKnowledgeSlateChangedState
{
	StateChanged,		// 2021/11/10 PKT - 상태 변경
	MaterialItem,		// 2021/11/17 PKT - 재료 아이템
	//TreeCleanUp,		// 2021/11/10 PKT - Tree 초기화
	//NodeStateChanged,	// 2021/11/10 PKT - Node 상태 변경
};


enum class EMonsterPortraitDisplayOption
{
	None = 0,
	IgnoreReddot = 1 << 0,		// 2021/12/08 PKT - RedDot 기능을 활용하지 않겠다.
	HideProgressBar = 1 << 1,	// 2021/12/08 PKT - ProgressBar를 숨긴다.
	DisableTouch = 1 << 2,		// 2021/12/08 PKT - Touch 를 사용하지 않겠다.
};
ENUM_CLASS_FLAGS(EMonsterPortraitDisplayOption);