// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Option/GsGameUserSettings.h"
#include "DataSchema/GsSchemaEnums.h"
#include "GsSoundUtil.generated.h"

/**
 *
 */
UCLASS()
class T1PROJECT_API UGsSoundUtil : public UObject
{
	GENERATED_BODY()

public:
	//mute
	static void MuteAllSound();
	static void Mute(EGsSoundVolumeType inType);
	static void MuteMasterSound();
	static void MuteMusicSound();
	static void MuteGameSound();
	static void MuteNoticeSound();

	//recovery
	static void ResetAllSoundVolumeBySettingValue();
	static void ResetSoundVolumeBySettingValue(EGsOptionAudio inType);
	static void ResetWholeSoundVolumeBySettingValue();
	static void ResetMusicSoundVolumeBySettingValue();
	static void ResetGameSoundVolumeBySettingValue();
	static void ResetNoticeSoundVolumeBySettingValue();

	//set sound volume
	static void SetSoundVolum(EGsSoundVolumeType inType, float inValue);
	static void SetMasterSoundVolum(float inValue);
	static void SetMusicSoundVolum(float inValue);
	static void SetGameSoundVolum(float inValue);
	static void SetNoticeSoundVolum(float inValue);

	static EGsSoundVolumeType GetVolumeType(EGsOptionAudio inType);

	static bool IsOptionAudioOn(EGsOptionAudio inType);
};
