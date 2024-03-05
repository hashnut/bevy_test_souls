// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Sound/SoundBase.h"
#include "GsSoundPlayer.generated.h"

UENUM(BlueprintType)
enum class EGsAudioParameterType : uint8
{
	None,
	EnvTimeline,
	EnvWeather,
	CreatureWeapon,
	FootPrint,
	HitCritical,
};

struct FGsSchemaSoundResData;
class USceneComponent;
class UAudioComponent;
class USoundBase;
/**
 * 사운드 플레이어
 */
UCLASS()
class T1PROJECT_API UGsSoundPlayer : public UObject
{
	GENERATED_BODY()

protected:
	uint64 _bgmID;
	uint64 _envID;
	bool _isUseSpeedScale = true;
	
	UPROPERTY()
	class UCurveFloat* _audioVolumeScale;

public:
	void Initialize();

private:
	// LocalPlayer가 발생하는 사운드는 모두 2D 사운드로 플레이
	bool IsLocalPlayer(USceneComponent* In_AttachToComponent);
	// 타겟의 SpeedScale을 얻는다.
	float GetSpeedScale(USceneComponent* In_AttachToComponent);
	
	// 대상에게 2D사운드 발생
	UAudioComponent* SpawnSound2D(const FGsSchemaSoundResData* In_ResData, USceneComponent* In_AttachToComponent);
	// 대상에게 사운드를 부착
	UAudioComponent* SpawnSoundAttached(const FGsSchemaSoundResData* In_ResData, USceneComponent* In_AttachToComponent);
	// 대상을 위치에 사운드 발생
	UAudioComponent* SpawnSoundAtLocation(const FGsSchemaSoundResData* In_ResData, USceneComponent* In_AttachToComponent);

	// 두개의 사운드 점점작게 점점크게 체인지
	uint64 PlaySoundFadeOutFadeIn(uint64 In_FadeOutAudioId, const FGsSchemaSoundResData* In_FadeInResData);

public:		
	// 대상에게 사운드를 플레이한다.
	uint64 PlaySoundTarget(const FGsSchemaSoundResData* In_ResData, USceneComponent* In_AttachToComponent,
		EGsAudioParameterType In_Type = EGsAudioParameterType::None, float In_value = 0.0f);
	uint64 PlaySoundTarget(const FGsSchemaSoundResData* In_ResData, USceneComponent* In_AttachToComponent,
		const TArray<TPair<EGsAudioParameterType, float>>& In_ParameterList);
	// 대상이 존재하지 않는 UI, 환경음
	uint64 PlaySound2D(const FGsSchemaSoundResData* In_ResData);
	uint64 PlaySound2D(const FString& In_SoundResRowName);
	uint64 PlaySound2D(const FString& In_SoundResRowName, const float InSpeedScale);
	// bgm
	uint64 PlaySoundBgm(const FGsSchemaSoundResData* In_ResData);
	uint64 PlaySoundBgm(const FString& In_SoundResRowName);
	// env
	uint64 PlaySoundEnv(const FGsSchemaSoundResData* In_ResData);
	uint64 PlaySoundEnv(const FString& In_SoundResRowName);
	// 애니메이션 Notify
	uint64 PlaySoundAnimNotify(USoundBase* In_Sound, USceneComponent* In_AttachToComponent,
		FName In_AttachPointName, bool In_bFollow, float In_VolumeMultiplier, float In_PitchMultiplier, bool In_bSpeedScale);

	uint64 PlaySoundByPath(const FSoftObjectPath& InPath);
	uint64 PlaySoundBgmByPath(const FSoftObjectPath& InPath);
	uint64 PlaySoundObject(USoundBase* In_Sound);
	
	void StopSoundBgm();
	// 사운드를 중지 또는 종료시킨다.
	void StopSound(uint64 In_SoundID);	
	// Actor에 어태치 된 사운드를 다 종료시킨다.
	void StopSound(USceneComponent* In_DetachToComponent);
	// 사운드Env를 중지 또는 종료시킨다.
	void StopSoundEnv(uint64 In_SoundID);
	// 사운드가 플레이 중인지 체크한다.
	bool  IsPlaying(uint64 In_SoundID) const;							
	// 사운드큐에 명령어 전달
	void SetAudioParameter(EGsAudioParameterType In_Type, float In_value, UAudioComponent* In_AudioCom = nullptr);
	// 일시멈춤 on/off
	void PauseSound(bool In_IsPause, uint64 In_SoundID);
	// bgm 일시 멈춤
	void PauseSoundBgm(bool In_IsPause);
	void PauseSoundEnv(bool In_IsPause);

private:
	// class 셋팅이 안되는 경우 체크
	void CheckSoundClassData(const USoundBase* In_Sound);

private:
	// 사운드 파라미터 key Name 정의
	void GetAudioParameterName(EGsAudioParameterType InParameterType, OUT FString& outStr);
	// 사운드 애셋을 얻어온다.
	USoundBase* GetSoundRes(const FSoftObjectPath& In_SoundResPath);		
	// 리소스 테이블을 얻어온다.
	const FGsSchemaSoundResData* GetSoundResData(const FString& In_SoundResKeyword) const;
	// 리소스 테이블을 얻어온다.
	const FGsSchemaSoundResData* GetSoundResData(int In_SoundResID) const;	
	// 현재 플레이 중인 Env을 얻어온다.	
	UAudioComponent* GetEnvAudioComponent();

public:
	void SetUseSpeedScale(bool inValue) { _isUseSpeedScale = inValue; }
	bool IsUseSpeedScale() { return _isUseSpeedScale; }
};
