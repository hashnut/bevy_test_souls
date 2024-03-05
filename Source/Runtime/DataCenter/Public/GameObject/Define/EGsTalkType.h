#pragma once

#include "EGsTalkType.generated.h"

// 말풍선 출력 순서에 관한 타입 정의
UENUM(BlueprintType)
enum class EGsTalkType : uint8
{
	// 순차 출력
	ORDER,	
	// 랜덤 출력
	RANDOM,	
};

UENUM(BlueprintType)
enum class EGsTalkBalloonDataType : uint8
{
	// 단순 출력
	NONE,
	// 현재 서버 랭킹 1위
	RANK_LOCAL_LEVEL_1ST,
	// 전체 서버 랭킹 1위
	RANK_WORLD_LEVEL_1ST,
	// 최고가 등록 물품
	MARKET_PRICE_TOP,
	// 현재 착용 무기의 최고 등급 무기
	MARKET_WEAPON_GRADE_TOP,
	// 캐릭터 누적 접속일
	ACHIEVEMENT_LOGIN_ACCUMULATE,
	// 캐릭터 생성일
	ACHIEVEMENT_CREATE_DATE,
	// 보상 안내
	REDDOT_REWARD,
	// 뉴스
	REDDOT_NEWS,
	// 공지사항
	ANNOUNCEMENT,
};

UENUM(BlueprintType)
enum class EGsTalkBalloonState : uint8
{
	None,
	Spawn,
	Idle,
	ForwardWalk,
	Run,
	Attack,
	Hit,
	Dying,
	Dead,
	RunAway,
	Roar,

	MAX
};