// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsIconDefine.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsIconDefine : public UObject
{
	GENERATED_BODY()
	
};

UENUM(BlueprintType)
enum class EGsIconSlotType : uint8
{
	SImpleType = 0,		// 각종 아이템 아이콘 슬롯에서 사용할 될 슬롯타입
	HudType,			// hud quickslot 타입

	MaxType,
};

UENUM(BlueprintType)
enum class EGsIconSlotCountType : uint8
{
	One = 0,
	Two,
	Three,
	Four,
	Five,
	Six,

	MaxSlotType,
};

UENUM(BlueprintType)
enum class EGsIconRedDotType : uint8
{
	NORMAL = 0 UMETA(Tooltip = "일반"),
	CHECK UMETA(Tooltip = "체크"),
	REINFORCE UMETA(Tooltip = "강화"),
	NUMBER UMETA(Tooltip = "숫자"),
};

