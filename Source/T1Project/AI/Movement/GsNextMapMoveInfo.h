#pragma once
#include "CoreMinimal.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"

// 다음맵 이동시 저장 정보(다음맵 도착후 처리 필요 정보)
struct FGsNextMapMoveInfo
{
	bool _isNextMoveOn = false;

	// 이동 컨텐츠 타입
	EGsMovementAutoContentsType _moveContentsType;
	// 이동할 map id
	int _moveMapId;
	// 이동할 위치
	FVector _movePos;
	// send warp flag
	bool _sendWarp = false;
	// 접근 거리 띄워서 도착 처리
	float _useAcceptanceRadius =0.0f;
};