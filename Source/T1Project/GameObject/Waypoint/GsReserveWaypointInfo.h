#pragma once

#include "CoreMinimal.h"
#include "GameObject/Movement/GsAutoMoveInfo.h"

/*
   예약 웨이 포인트 구조(FGsReserveInteractionInfo 과 비슷함)
*/
struct FGsReserveWaypointInfo
{
    // 현재 인덱스
    int _currentIndex;
    // 이동할 map Id
    int _mapId;
    // 이동할 위치
    TArray<FVector> _arrayPos;
    // 센터 노드인지
    bool _isCenterNode = false;
    // 외부 접근 반경 값(보통은 안씀)
    float _useAcceptanceRadius = 0.0f;

	// 이동 성공 처리
	FGsAutoMoveCompletedDelegate	_funcSuccessDelegate;
	// 이동 실패 처리
	FGsAutoMoveCompletedOneDelegate	_funcFailDelegate;
};