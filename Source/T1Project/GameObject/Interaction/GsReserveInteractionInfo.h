#pragma once

#include "GameObject/Interaction/GsInteractionMoveType.h"
#include "GameObject/Define/GsGameObjectDefine.h"

struct FGsSchemaNpcData;
struct FGsSchemaPropData;

// 타겟에 대한 정보
struct FGsReserveInteractionTargetInfo
{
	EGsGameObjectType _gameObjectType = EGsGameObjectType::Base;
	int _tblId = 0;
	// 타겟 테이블 데이터 (Npc)
	const FGsSchemaNpcData* _npcData = nullptr;
	// 타겟 테이블 데이터 (Prop)
	const FGsSchemaPropData* _propData = nullptr;	

	void Clear()
	{
		_gameObjectType = EGsGameObjectType::Base;
		_tblId = 0;		
		_npcData = nullptr;
		_propData = nullptr;
	}	
};

// 예약 인터랙션 구조(스킬의 그것과 같음)
// 통일 되어야 될려나
struct FGsReserveInteractionInfo
{
	// 이동 타입(위치, 타겟)
	EGsInteractionMoveType _moveType;
	// 이동할 맵 id
	int _mapId = 0;
	// 이동 위치
	FVector _pos;
	// 이만큼 덜가게 처리 값
	float _addAcceptanceRadius = 0.0f;
	// 이동 완료인지
	bool _isMoveComplete = false;

	FGsReserveInteractionTargetInfo _target;
};

// 인터랙션 경과 시간 체크 
struct FGsInteractionTimer
{
	bool _isEnable = false;
	float _currTime = -1.f; // Sec
	float _endTime = -1.f;

	bool IsValid() const
	{
		return _isEnable && _currTime < _endTime;
	}

	void Clear()
	{
		_isEnable = false;
		_currTime = -1.f;
		 _endTime = -1.f;
	}

	float GetRemainTime() const
	{
		float time = _endTime - _currTime;
		return time < 0 ? 0 : time;
	}
};
