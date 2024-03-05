// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"

/**
 * 효과 텍스트 출력 방식
 */
enum class EGsEffectTextOption : int32
{
	DEFAULT = 0, // 기본값: 줄바꿈 출력, 전체 출력

	COMMA = 1 << 0, // 줄바꿈 없이 콤마로 출력
	SKIP_INVOKE_CAUSE = 1 << 1, // Invoke Cause 정보 안찍음
	SKIP_INVOKE_RATE = 1 << 2, // Invoke Rate 정보 안찍음
	SKIP_INVOKE_TARGET = 1 << 3, // Invoke Target 정보 안찍음
	SKIP_STAT_TIME = 1 << 4, // Stat 시간정보 출력 안함

	ABNORAMLITY_APPLY_TYPE_ADD_GROUP = 1 << 5, // 일반(ADD)과 ADD_GROUP을 구분 위함
	PASSIVITY_INVOKE_TARGET_SHORT = 1 << 6, // "나에게" 대신 "자신", "대상에게" 대신 "대상" 으로 출력
};

struct T1PROJECT_API FGsEffectTextOptionFunc
{
	static void AddOption(EGsEffectTextOption InOption, OUT int32& OutOption)
	{
		OutOption |= static_cast<int32>(InOption);
	}

	static bool IsOption(int32 InOption, EGsEffectTextOption InCheck)
	{
		return (0 != (InOption & static_cast<int32>(InCheck))) ? true : false;
	}
};

/**
 * 콜렉션을 정보 수집을 위한 데이터 묶음. 동일 Stat은 합산해서 저장.
 */
struct T1PROJECT_API FGsEffectTextCollection
{
	TMap<StatType, int32> _statMap; // Key: StatType, Value: value	
	TArray<TPair<FText, FText>> _effectList;

	void Empty()
	{
		_statMap.Empty();
		_effectList.Empty();
	}
};

static const int INVALID_EFFECT_TEXT_LEVEL = -1;
static const int NOT_USE_EFFECT_TEXT_LEVEL = -1;
static const int DEFAULT_EFFECT_TEXT_LEVEL = 1;
