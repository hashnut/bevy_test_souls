#pragma once

/* 
* pc 이름 타입
*/

enum class EGsOptionValuePCNameType : uint8
{
	ALL_OFF			=	0 ,	// 모두 끄기
	MY_NAME_ON		=	1,	// 내이름만 표기
	OTHER_NAME_ON	=	2,	// 다른 유저만 표기
	ALL_ON			=	3,	// 모두 표기
};

/*
* 자동 아이템 루팅: 무게 제한
*/

enum class EGsOptionValueAutoItemLootWeightLimit : uint8
{
	OFF						=	0,	// OFF(제한 없음)
	WEIGHT_50_PER_UNDER		=	1,	// 50% 미만
	WEIGHT_70_PER_UNDER		=	2,	// 70% 미만
	WEIGHT_90_PER_UNDER		=	3,	// 90% 미만
	
	
};

/*
* 벨소리 종류 정의
*/
UENUM(BlueprintType)
enum class EGsAlertType : uint8
{
	RingerAlert = 0 UMETA(Tooltip = "벨소리"),
	PvpAlert = 1 UMETA(Tooltip = "PVP 공격"),
	LowHpAlert = 2 UMETA(Tooltip = "체력 경고"),
	VibrateAlert = 3 UMETA(Tooltip = "진동"),

	MAX = 4 UMETA(Tooltip = "소리 없음")
};