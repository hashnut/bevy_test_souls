// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Classes/GsManager.h"
#include "Management/GsScopeHolder.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "Sound/GsSoundPlayer.h"
#include "Sound/GsSoundMixerController.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Message/GsMessageSound.h"
#include "GsSoundManager.generated.h"

UCLASS()
class T1PROJECT_API UGsSoundManager :
	public UObject,
	public IGsManager
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	class UGsSoundMixerController*	_soundMixerController = nullptr;
	UPROPERTY()
	class UGsSoundPlayer*			_soundPlayer = nullptr;

private:
	TArray<TPair<MessageSound, FDelegateHandle>> _soundDelegates;	
	uint64 _currentBgmId = 0;
	const FGsSchemaSoundResData* _currentBgmSoundResData = nullptr;
	uint64 _currentEnvId = 0;
	const FGsSchemaSoundResData* _currentEnvSoundResData = nullptr;

public:
	virtual void BeginDestroy() override;

public:
	//IGsManager
	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual void Update(float inTick) override;

public:
	virtual void InitializeAfterResourceDownload() override;

public:
	void BindMessage();
	void UnbindMessage();

public:
	UFUNCTION(BlueprintCallable, Category = "Sound")
	static void StaticPushSoundMixer(EGsSoundMixType In_type);	// 테스트 용 (사운드 믹서를 변경한다.)
	UFUNCTION(BlueprintCallable, Category = "Sound")
	static void StaticPopSoundMixer(EGsSoundMixType In_type);						// 테스트 용 (사운드 믹서를 되돌린다.)
	UFUNCTION()
	void UpdateAudioListener(float inTick);

public:
	UGsSoundMixerController*	GetOrCreateSoundMixerController();	// 사운드 믹서를 컨트롤 한다.
	UGsSoundPlayer*				GetOrCreateSoundPlayer();			// 사운드 플레이어

public:	
	void OnPlayBGM(const struct IGsMessageParam* InParam);
	void PlayBGM(const FGsSchemaSoundResData* inRes);
	void PlayEnv(const FGsSchemaSoundResData* inRes);
	void PlayInvadeBGM();
	void PlayMapBGM();
	void StopBGM();
	void OnPauseBGM(const struct IGsMessageParam* InParam);

private:
	bool TryPlayFenceSound();
	bool TryPlayInvadeSound();

private:
	void PauseBGM(bool inValue);
	void PauseBGMInternal(bool inValue);
	void PauseEnvInternal(bool inValue);

	void StopBGMInternal();
	void StopEnvInternal();

	void OnStopBGM(const struct IGsMessageParam* InParam);
	
public:
	void SetSoundMaster();
	void SetSoundLobbyMaster();
};

#define GSound() UGsScopeHolder::GetGlobalManager<UGsSoundManager>(UGsScopeGlobal::EManagerType::Sound)
#define GSoundPlayer() (nullptr == GSound())? nullptr : GSound()->GetOrCreateSoundPlayer()