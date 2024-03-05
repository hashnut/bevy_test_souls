// Fill out your copyright notice in the Description page of Project Settings.


#include "GsPresetHelper.h"


PresetValue FGsPresetHelper::GetPresetValueFromId(PresetId InPresetId)
{
	return 1 << (InPresetId - 1);
}

bool FGsPresetHelper::IsPreset(PresetValue InPresetValue)
{
	for (uint8 presetId = MIN_PRESET_ID; presetId <= MAX_PRESET_ID;++presetId)
	{
		uint8 result = InPresetValue & 1 << (presetId - 1);
		if (result > 0)
		{
			return true;
		}
	}
	return false;
}
