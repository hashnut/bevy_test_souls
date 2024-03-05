// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSoundUtil.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Option/GsGameUserSettings.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

void UGsSoundUtil::MuteAllSound()
{
	MuteMasterSound();
	MuteGameSound();
	MuteMusicSound();
	MuteNoticeSound();
}

void UGsSoundUtil::Mute(EGsSoundVolumeType inType)
{
	SetSoundVolum(inType, 0);
}

void UGsSoundUtil::MuteMasterSound()
{
	SetMasterSoundVolum(0);
}


void UGsSoundUtil::MuteMusicSound()
{
	SetMusicSoundVolum(0);
}


void UGsSoundUtil::MuteGameSound()
{
	SetGameSoundVolum(0);
}

void UGsSoundUtil::MuteNoticeSound()
{
	SetNoticeSoundVolum(0);
}

void UGsSoundUtil::ResetAllSoundVolumeBySettingValue()
{
	ResetWholeSoundVolumeBySettingValue();
	ResetMusicSoundVolumeBySettingValue();
	ResetGameSoundVolumeBySettingValue();
	ResetNoticeSoundVolumeBySettingValue();
}

void UGsSoundUtil::ResetSoundVolumeBySettingValue(EGsOptionAudio inType)
{
	if (UGsGameUserSettings* settings = GGameUserSettings())
	{
		if (settings->GetAudioSettings().Contains(inType))
		{
			float value = settings->GetAudioSettings()[inType];
			SetSoundVolum(GetVolumeType(inType), value * 0.01f);
		}
	}
}

void UGsSoundUtil::ResetWholeSoundVolumeBySettingValue()
{
	if (IsOptionAudioOn(EGsOptionAudio::WHOLE_ON))
	{
		ResetSoundVolumeBySettingValue(EGsOptionAudio::WHOLE_VOLUME);
	}
}

void UGsSoundUtil::ResetMusicSoundVolumeBySettingValue()
{
	if (IsOptionAudioOn(EGsOptionAudio::MUSIC_ON))
	{
		ResetSoundVolumeBySettingValue(EGsOptionAudio::MUSIC_VOLUME);
	}
}

void UGsSoundUtil::ResetGameSoundVolumeBySettingValue()
{
	if (IsOptionAudioOn(EGsOptionAudio::EFFECT_ON))
	{
		ResetSoundVolumeBySettingValue(EGsOptionAudio::EFFECT_VOLUME);
	}
}

void UGsSoundUtil::ResetNoticeSoundVolumeBySettingValue()
{
	if (IsOptionAudioOn(EGsOptionAudio::NOTICE_ON))
	{
		ResetSoundVolumeBySettingValue(EGsOptionAudio::NOTICE_VOLUME);
	}
}

void UGsSoundUtil::SetSoundVolum(EGsSoundVolumeType inType, float inValue)
{
	if (UGsSoundMixerController* mixer = GSound()->GetOrCreateSoundMixerController())
	{
		mixer->SetVolume(inType, inValue);
	}
}

void UGsSoundUtil::SetMasterSoundVolum(float inValue)
{
	SetSoundVolum(EGsSoundVolumeType::Master, inValue);
}

void UGsSoundUtil::SetMusicSoundVolum(float inValue)
{
	SetSoundVolum(EGsSoundVolumeType::Music, inValue);
}

void UGsSoundUtil::SetGameSoundVolum(float inValue)
{
	SetSoundVolum(EGsSoundVolumeType::Game, inValue);
}

void UGsSoundUtil::SetNoticeSoundVolum(float inValue)
{
	SetSoundVolum(EGsSoundVolumeType::Notice, inValue);
}

EGsSoundVolumeType UGsSoundUtil::GetVolumeType(EGsOptionAudio inType)
{	
	switch (inType)
	{
	case EGsOptionAudio::WHOLE_VOLUME:
		return EGsSoundVolumeType::Master;
		break;	
	case EGsOptionAudio::MUSIC_VOLUME:
		return EGsSoundVolumeType::Music;
		break;	
	case EGsOptionAudio::EFFECT_VOLUME:
		return EGsSoundVolumeType::Game;
		break;
	case EGsOptionAudio::NOTICE_VOLUME:
		return EGsSoundVolumeType::Notice;
	}

	return EGsSoundVolumeType::Max;
}

bool UGsSoundUtil::IsOptionAudioOn(EGsOptionAudio inType)
{
	if (UGsGameUserSettings* settings = GGameUserSettings())
	{
		if (settings->GetAudioSettings().Contains(inType))
		{
			return static_cast<bool>(settings->GetAudioSettings()[inType]);
		}
	}
	return false;
}


