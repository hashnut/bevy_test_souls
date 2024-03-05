// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

/*
 * FGsUIHelper
 * 각종 UI 편의용 static 함수 등록
 */
struct T1PROJECT_API FGsUIBaseHelper
{
public:
	// 주어진 위젯의 UInvalidationBox들을 찾아서 저장. 중첩될 경우 상위의 UInvalidationBox만 저장
	static void RecursiveFindInvalidationBox(class UWidget* InWidget, OUT TArray<class UInvalidationBox*>& OutArray);

	static void InvalidateInvalidationBox(TArray<class UInvalidationBox*>& InArray);
};
