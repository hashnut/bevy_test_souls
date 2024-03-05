// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Sound/GsSchemaSoundMixData.h"
#include "DataSchema/Sound/GsSchemaSoundClassData.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "Message/GsMessageSystem.h"
#include "GsSoundMixerController.generated.h"

class USoundMix;
enum class ESoundMixType : uint8;

UCLASS()
class T1PROJECT_API UGsSoundMixerController : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	USoundMix*	_soundMix		= nullptr;
	UPROPERTY()
	USoundMix*	_soundMixMaster	= nullptr;
	UPROPERTY()
	USoundMix* _soundMixBackUp = nullptr;

	TArray<EGsSoundMixType> _soundMixList;

private:
	MsgSystemHandleArray _msgSystemHandleList;

public:
	virtual void BeginDestroy() override;

public:
	virtual void Initialize();
	virtual void Finalize();

protected:
	void RegisterMessages();
	void UnregisterMessages();

	void OnReconnectEnd();
	void OnGameModeWorldExit();

	void ClserSoundMixer();

public:
	void OnMixChanged(EGsSoundMixType In_Type);		// 사운드믹서를 변경한다.	
	void OffMixMode(EGsSoundMixType In_Type);		// 대상의 사운드믹서를 끄고 디폴트로 변경한다.	
	
	void SetVolume(EGsSoundVolumeType In_Type, float In_Value);		// 사운드볼륨을 변경한다.
	void SetSoundMix(USoundMix* In_PushSoundMix, USoundMix* In_PopSoundMix);
	USoundMix* GetSoundMix(EGsSoundMixType In_Type);
	FString ToString();
		
private:
	void OffMixMode();	// 사운드믹서를 디폴트로 변경한다.

	const FGsSchemaSoundMixData* GetSoundMixData(EGsSoundMixType In_Type) const;
	const FGsSchemaSoundClassData* GetSoundClassData(EGsSoundVolumeType In_Type) const;


	USoundClass* GetSoundClass(EGsSoundVolumeType In_Type);
};
