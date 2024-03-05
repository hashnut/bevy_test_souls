// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

/*
 * 자동이동 Block 컨텐츠, 맵등 리스트 관리
 */
struct T1PROJECT_API FGsAutoMoveBlockList
{
public:
	static bool IsAutoMovePossibility();
	static bool IsMapTypeCheck();
	static bool IsContentTypeCheck();
	static bool IsArenaCheck();
};
