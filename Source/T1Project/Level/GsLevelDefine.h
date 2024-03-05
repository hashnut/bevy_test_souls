// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
/**
 *
 */

// LoadTopo시 연출 타입
enum class EGsLoadTopoEffectType
{
	NONE = -1,
	DIFF_MAP = 0, // 다른 맵: 워프연출(인게임, 살아있을때) -> FadeOut -> 로딩창 -> FadeIn -> 스폰연출
	SAME_MAP_DIFF_POS, // 같은맵 다른좌표: 워프연출(인게임, 살아있을때) -> FadeOut -> FadeIn -> 스폰연출
	SAME_MAP_SAME_POS, // 같은맵 같은좌표: FadeIn -> FadeOut
};