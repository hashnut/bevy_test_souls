#pragma once

#include "CoreMinimal.h"

/*
	퀘스트 kill 타입 이동 예약 정보
*/

struct FGsReserveQuestKillMoveInfo
{
	// 예약되어 있는가
	bool _isReserved = false;
	// 이동 위치
	FVector _targetPos;
	// 해당 map id
	int _targetMapId;
	// 도달 인정 추가 거리(요기까지만 와도 도착 인정)
	float _acceptanceRadius;
};

struct FGsReserveQuestKillCenterMoveInfo
{
	// 예약되어 있는가
	bool _isReserved = false;
	// 이동 위치
	FVector _targetPos;
	// 도달 인정 추가 거리(요기까지만 와도 도착 인정)
	float _acceptanceRadius;
};