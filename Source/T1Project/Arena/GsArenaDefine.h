// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

enum class EGsArenaInState : uint8
{
	NONE,			// 디폴트
	WAIT,			// 입장 대기
	COUNT_DOWN,		// 카운트 다운
	ARENA_START,	// 대결 시작. 대결 중
	ARENA_END,		// 대결 종료
};

enum class EGsArenaOutState : uint8
{
	NONE,		// 일반 상태
	REQUEST,	// 대결 요청중인 상태
	RECEIVE,	// 대결 요청받은 상태
	ENTERED,	// 대결 진입한 상태
};

struct ArenaUserData
{
public:
	UserDBId _targetUserDBId = INVALID_USER_DB_ID;	// 상대의 UserDBId
	Level _targetLevel = INVALID_LEVEL;				// 상대의 Level
	FString _targetUserName;						// 상대의 이름
	bool _targetVisible = false;					// 상대의 정보공개여부

	void SetData(UserDBId InUserDBId, Level InLevel, FString InUserName, bool InIsVisible);
	void Reset();

};