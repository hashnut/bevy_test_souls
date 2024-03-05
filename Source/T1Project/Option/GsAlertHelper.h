// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum class EGsAlertType : uint8;

/**
 * 
 */
class T1PROJECT_API FGsAlertHelper
{
public:
	static void PlayAlertEffect(EGsAlertType inAlertType, FSoftObjectPath inResource = FSoftObjectPath(TEXT("None")));
};
