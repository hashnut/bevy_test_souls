// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

/**
 * 프리셋 Helper 클래스
 */
class T1PROJECT_API FGsPresetHelper
{
public:
	static PresetValue GetPresetValueFromId(PresetId InPresetId);
	static bool IsPreset(PresetValue InPresetValue);

};
