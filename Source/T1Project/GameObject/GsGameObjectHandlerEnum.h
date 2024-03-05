#pragma once

#include "CoreMinimal.h"

/*
	핸들러 검색 및 정의용
*/

enum class GameObjectHandlerType : uint8
{
	NONE			= 0,	// 유효하지 않은 타입 - 에러값

	//BASE  1 ~ 10
	TARGET			= 1,	// 
	
	//CREATURE 20 ~ 50
	SKILL			= 20,	// 
	PARTS			= 21,
	MOVEMENT		= 22,
	CUSTOMIZE		= 23,
	ABNORMALITY		= 24,
	DAMAGE			= 25,

	//PLAYER  50 ~ 100
	INTERACTION		= 50,
	FAIRY			= 51,
	USE_ITEM_EFFECT = 52,

	//LOCAL 100 ~ 255
	LOOKAT			= 100,
	COOLTIME		= 101,
	QUEST			= 102,
	FENCE			= 103,
	CAMERA			= 104,
	LOOT			= 105,
	AREAENV			= 106,
	AI_RESERVE		= 107,
	AI_AVOIDCOLLISION = 108,	// 2023/10/12 PKT - 충돌 회피	
};

enum class ActorState : uint8
{
	NONE,
	CLEAR,
	LOADING,
	ACTIVE,
};