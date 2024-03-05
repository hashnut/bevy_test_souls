// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsUIContentDefine.generated.h"

UENUM(BlueprintType)
enum class EGsEquipSlotType : uint8
{
	WEAPON_TYPE = 0,		// 무기
	HELMET_TYPE,			// 투구
	ARMOR_TYPE,				// 갑옷
	GLOVE_TYPE,				// 장갑
	SHOES_TYPE,				// 신발
	BELT_TYPE,				// 벨트

	SCARF_TYPE,				// 스카프
	NECKLACE_TYPE,			// 목걸이
	EAR_RING_LEFT_TYPE,		// 귀걸이_왼쪽
	EAR_RING_RIGHT_TYPE,	// 귀걸이_오른쪽
	RING_LEFT_TYPE,			// 반지_왼쪽
	RING_RIGHT_TYPE,		// 반지_오른쪽
	CHARM_LEFT_TYPE,		// 참_왼쪽
	CHARM_RIGHT_TYPE,		// 참_오른쪽
	BRACELET_LEFT_TYPE,		// 팔찌_왼쪽
	BRACELET_RIGHT_TYPE,	// 팔찌_오른쪽
	DURAL_LEFT_TYPE,		// 듀랄_왼쪽
	MARBLE_LEFT_TYPE,		// 보주_왼쪽
	MARBLE_RIGHT_TYPE,		// 보주_오른쪽

	SLOT_MAX,
};

UENUM(BlueprintType)
enum class EGsItemEnchantActionType : uint8
{
	ENCHANT_SINGLE = 0, // 단일강화
	ENCHANT_MULTI,		// 멀티강화
	REFINE_OPTION,		// 제련 옵션
	MAGICAL_FORGE,		// 마법 부여
	
	ACTION_MAX,
};