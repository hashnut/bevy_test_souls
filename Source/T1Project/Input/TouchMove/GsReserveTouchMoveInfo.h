#pragma once

#include "CoreMinimal.h"

/*
	터치 이동 예약 정보(FGsReserveWaypointInfo에서 분리)
	:  처음엔 waypoint 같이 썻는데
	중간에 다른 waypoint로 교체시 실패 콜백이 오지 않는다
	
	void FGsMovementStateAuto::ClearOldFailDelegate(const FGsAutoMoveCompletedOneDelegate& In_newFailDelegate)
	if (newPoint !=	oldPoint) -> 이부분에서 같다고 쳐서 콜백이 안불려짐

	나머지도 다 분리 해야함(같은 성공, 실패 콜백 불려 있는 부분)
*/

struct FGsReserveTouchMoveInfo
{
	// 예약되어 있는가
	bool _isReserved = false;
	// 이동할 위치
	FVector _touchPos;
};