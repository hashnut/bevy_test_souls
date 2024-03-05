// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSoundPlayer.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Engine/Classes/Sound/SoundBase.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Public/AudioDevice.h"
#include "Engine/Public/ActiveSound.h"

#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"

#include "T1Project.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"

#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "DataSchema/GsSchemaEnums.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"


#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Environment/GsAreaEnvHandler.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"

#include "Message/GsMessageGameObject.h"

#include "TimerManager.h"

#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "Data/GsLobbyConstant.h"



void UGsSoundPlayer::Initialize()
{
	if (LSLobbyData())
	{
		if (const UGsLobbyConstant* lobbyConstant = LSLobbyData()->GetLobbyConstantData())
		{
			_audioVolumeScale = lobbyConstant->_audioVolumeScale;
		}
	}	
}

// LocalPlayer가 발생하는 사운드는 모두 2D 사운드로 플레이
bool UGsSoundPlayer::IsLocalPlayer(USceneComponent* In_AttachToComponent)
{
	if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
	{
		// 특정 컨텐츠 모드에서는 무시
		if (FGsContentsManagerGame* contentsManagerGame = gameFlow->GetContentsManagerGame())
		{
			if (contentsManagerGame->GetCurrentContentsMode() == FGsContentsMode::ContentsSkill)
			{
				return false;
			}
		}

		if (UGsGameObjectBase* objectBase = GSGameObject()->FindObject(In_AttachToComponent->GetOwner()))
		{
			if (UGsGameObjectLocalPlayer* local = Cast<UGsGameObjectLocalPlayer>(objectBase))
			{
				return true;
			}
		}
	}

	return false;
}

// 타겟의 SpeedScale을 얻는다.
float UGsSoundPlayer::GetSpeedScale(USceneComponent* In_AttachToComponent)
{
	float speedScale = 1.0f;
	if (false == IsUseSpeedScale())
		return speedScale;

	UAudioComponent* audioCom = nullptr;	
	if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
	{		
		if (UGsGameObjectBase* objectBase = GSGameObject()->FindObject(In_AttachToComponent->GetOwner()))
		{
			if (FGsSkillHandlerBase* skill = objectBase->GetSkillHandler())
			{
				if (FGsSkillRunnerBase* skillRunner = skill->GetSkillRunner())
				{
					speedScale = skillRunner->GetSpeedScale();
				}
			}
		}
	}

	return speedScale;
}

// 대상에게 2D사운드 발생
UAudioComponent* UGsSoundPlayer::SpawnSound2D(const FGsSchemaSoundResData* In_ResData, USceneComponent* In_AttachToComponent)
{
	USoundBase* soundRes = GetSoundRes(In_ResData->path);
	if (nullptr == soundRes)
		return nullptr;

	float speedScale = GetSpeedScale(In_AttachToComponent);

	return UGameplayStatics::SpawnSound2D(In_AttachToComponent->GetWorld(),
		soundRes, In_ResData->volumeMultiplier, In_ResData->pitchMultiplier * speedScale);
}

// 대상에게 사운드를 부착
UAudioComponent* UGsSoundPlayer::SpawnSoundAttached(const FGsSchemaSoundResData* In_ResData, USceneComponent* In_AttachToComponent)
{
	USoundBase* soundRes = GetSoundRes(In_ResData->path);
	if (nullptr == soundRes)
		return nullptr;

	float speedScale = GetSpeedScale(In_AttachToComponent);

	return UGameplayStatics::SpawnSoundAttached(
			soundRes, In_AttachToComponent, In_ResData->attachName, FVector(ForceInit),
			EAttachLocation::SnapToTarget, false, In_ResData->volumeMultiplier, In_ResData->pitchMultiplier * speedScale);	
}

// 대상을 위치에 사운드 발생
UAudioComponent* UGsSoundPlayer::SpawnSoundAtLocation(const FGsSchemaSoundResData* In_ResData, USceneComponent* In_AttachToComponent)
{
	USoundBase* soundRes = GetSoundRes(In_ResData->path);
	if (nullptr == soundRes)
		return nullptr;

	float speedScale = GetSpeedScale(In_AttachToComponent);

	return UGameplayStatics::SpawnSoundAtLocation(In_AttachToComponent->GetWorld(), soundRes, In_AttachToComponent->GetComponentLocation(),
		FRotator::ZeroRotator, In_ResData->volumeMultiplier, In_ResData->pitchMultiplier * speedScale);
}

// 대상에게 사운드를 플레이한다.
uint64 UGsSoundPlayer::PlaySoundTarget(const FGsSchemaSoundResData* In_ResData, USceneComponent* In_AttachToComponent,
	EGsAudioParameterType In_Type/* EGsAudioParameterType::None*/, float In_value)
{
	if (nullptr == In_AttachToComponent)
		return 0;

	if (nullptr == In_ResData)
		return 0;

	UAudioComponent* audioCom = nullptr;
	if (IsLocalPlayer(In_AttachToComponent))
	{
		audioCom = SpawnSound2D(In_ResData, In_AttachToComponent);
	}
	else
	{
		if (true == In_ResData->bFollow)
			audioCom = SpawnSoundAttached(In_ResData, In_AttachToComponent);
		else
			audioCom = SpawnSoundAtLocation(In_ResData, In_AttachToComponent);
	}

	if (In_Type != EGsAudioParameterType::None)
	{
		SetAudioParameter(In_Type, In_value, audioCom);
	}

	return (nullptr != audioCom) ? audioCom->GetAudioComponentID() : 0;
}

uint64 UGsSoundPlayer::PlaySoundTarget(const FGsSchemaSoundResData* In_ResData, USceneComponent* In_AttachToComponent,
	const TArray<TPair<EGsAudioParameterType, float>>& In_ParameterList)
{
	if (nullptr == In_AttachToComponent)
		return 0;

	if (nullptr == In_ResData)
		return 0;

	UAudioComponent* audioCom = nullptr;
	if (IsLocalPlayer(In_AttachToComponent))
	{
		audioCom = SpawnSound2D(In_ResData, In_AttachToComponent);
	}
	else
	{
		if (true == In_ResData->bFollow)
			audioCom = SpawnSoundAttached(In_ResData, In_AttachToComponent);
		else
			audioCom = SpawnSoundAtLocation(In_ResData, In_AttachToComponent);
	}

	if (nullptr != audioCom)
	{
		for (auto& iter: In_ParameterList)
		{
			SetAudioParameter(iter.Key, iter.Value, audioCom);
		}
	}

	return (nullptr != audioCom) ? audioCom->GetAudioComponentID() : 0;
}

// 대상이 존재하지 않는 UI, 환경음
uint64 UGsSoundPlayer::PlaySound2D(const FGsSchemaSoundResData* In_ResData)
{
	if (nullptr == In_ResData)
		return 0;

	USoundBase* soundRes = GetSoundRes(In_ResData->path);
	if (nullptr == soundRes)
		return 0;

	UAudioComponent* audioCom = UGameplayStatics::SpawnSound2D(GetWorld(), soundRes);

	return (nullptr != audioCom) ? audioCom->GetAudioComponentID() : 0;
}

uint64 UGsSoundPlayer::PlaySound2D(const FString& In_SoundResRowName)
{
	if (In_SoundResRowName.IsEmpty())
		return 0;

	const FGsSchemaSoundResData* resData = GetSoundResData(In_SoundResRowName);
	if (nullptr == resData)
		return 0;

	return PlaySound2D(resData);
}

uint64 UGsSoundPlayer::PlaySound2D(const FString& In_SoundResRowName, const float InSpeedScale)
{
	if (In_SoundResRowName.IsEmpty())
		return 0;

	const FGsSchemaSoundResData* resData = GetSoundResData(In_SoundResRowName);
	if (nullptr == resData)
		return 0;

	USoundBase* soundRes = GetSoundRes(resData->path);
	if (nullptr == soundRes)
		return 0;

	UAudioComponent* audioCom = UGameplayStatics::SpawnSound2D(GetWorld(), soundRes, 1.f ,1.f * InSpeedScale);

	return (nullptr != audioCom) ? audioCom->GetAudioComponentID() : 0;

	return PlaySound2D(resData);
}

uint64 UGsSoundPlayer::PlaySoundObject(USoundBase* In_Sound)
{
	if (nullptr == In_Sound)
		return 0;

	UAudioComponent* audioCom = UGameplayStatics::SpawnSound2D(GetWorld(), In_Sound);

	return (nullptr != audioCom) ? audioCom->GetAudioComponentID() : 0;
}

// 두개의 사운드 점점작게 점점크게 체인지
uint64 UGsSoundPlayer::PlaySoundFadeOutFadeIn(uint64 In_FadeOutAudioId, const FGsSchemaSoundResData* In_FadeInResData)
{
	uint64 outAudioId = In_FadeOutAudioId;

	UAudioComponent* afterAudioCom = nullptr;
	USoundBase* afterSound = nullptr;

	if (nullptr == In_FadeInResData)
		return outAudioId;

	afterSound = GetSoundRes(In_FadeInResData->path);
	if (nullptr == afterSound)
		return outAudioId;

	GSLOG(Log, TEXT("[SoundPlayer] PlaySoundFadeOutFadeIn FadeOutAudioId : %llu, FadeInResData : %llu"), In_FadeOutAudioId, outAudioId);

	if (UAudioComponent* beforeAudioComp = UAudioComponent::GetAudioComponentFromID(outAudioId))
	{
		if (USoundBase* beforeSound = beforeAudioComp->Sound)
		{
			if (beforeSound->GetFullName() != afterSound->GetFullName())
			{
				if (beforeAudioComp->IsPlaying())
				{
					beforeAudioComp->FadeOut(3.0f, 0.0f);
				}
			}
			else
			{
				if (beforeAudioComp->IsPlaying())
				{
					return outAudioId;
				}
			}
		}
	}

	afterAudioCom = UGameplayStatics::CreateSound2D(GetWorld(), afterSound);
	if (nullptr != afterAudioCom)
	{
		afterAudioCom->Play();
		afterAudioCom->FadeIn(2.0f, 1.0f);
		outAudioId = afterAudioCom->GetAudioComponentID();
	}

	return outAudioId;
}

// bgm
uint64 UGsSoundPlayer::PlaySoundBgm(const FString& In_SoundResRowName)
{
	if (In_SoundResRowName.IsEmpty())
		return _bgmID;

	const FGsSchemaSoundResData* resData = GetSoundResData(In_SoundResRowName);
	if (nullptr == resData)
		return _bgmID;

	return _bgmID = PlaySoundBgm(resData);
}

uint64 UGsSoundPlayer::PlaySoundBgm(const FGsSchemaSoundResData* In_ResData)
{
	return _bgmID = PlaySoundFadeOutFadeIn(_bgmID, In_ResData);	
}

// env
uint64 UGsSoundPlayer::PlaySoundEnv(const FGsSchemaSoundResData* In_ResData)
{
	return _envID = PlaySoundFadeOutFadeIn(_envID, In_ResData);
}

uint64 UGsSoundPlayer::PlaySoundEnv(const FString& In_SoundResRowName)
{
	if (In_SoundResRowName.IsEmpty())
		return _envID;

	const FGsSchemaSoundResData* resData = GetSoundResData(In_SoundResRowName);
	if (nullptr == resData)
		return _bgmID;

	return _envID = PlaySoundFadeOutFadeIn(_envID, resData);
}

// 애니메이션 Notify
uint64 UGsSoundPlayer::PlaySoundAnimNotify(
	USoundBase* In_Sound, USceneComponent* In_AttachToComponent,
	FName In_AttachPointName, bool In_bFollow, float In_VolumeMultiplier, float In_PitchMultiplier, bool In_bSpeedScale)
{
	if (nullptr == In_Sound)	
		return 0;	

	if (nullptr == In_AttachToComponent)
		return 0;	

	float speedScale = (In_bSpeedScale) ? GetSpeedScale(In_AttachToComponent) : 1.0f;
	float volumeScale = 1.0;
	if (In_bSpeedScale)
	{
		if(_audioVolumeScale)
			volumeScale = _audioVolumeScale->GetFloatValue(speedScale);
	}

	UAudioComponent* audioCom = nullptr;
	if (IsLocalPlayer(In_AttachToComponent))
	{
		audioCom = UGameplayStatics::SpawnSound2D(In_AttachToComponent->GetWorld(),
			In_Sound, In_VolumeMultiplier * volumeScale, In_PitchMultiplier * speedScale);
	}
	else
	{
		if (true == In_bFollow)
		{
			audioCom = UGameplayStatics::SpawnSoundAttached(In_Sound, In_AttachToComponent, In_AttachPointName, FVector(ForceInit),
				EAttachLocation::SnapToTarget, false, In_VolumeMultiplier * volumeScale, In_PitchMultiplier * speedScale);
		}
		else
		{
			audioCom = UGameplayStatics::SpawnSoundAtLocation(In_AttachToComponent->GetWorld(), In_Sound, In_AttachToComponent->GetComponentLocation(),
				FRotator::ZeroRotator, In_VolumeMultiplier * volumeScale, In_PitchMultiplier * speedScale);
		}
	}			

	return (nullptr != audioCom) ? audioCom->GetAudioComponentID() : -1;
}

uint64 UGsSoundPlayer::PlaySoundByPath(const FSoftObjectPath& InPath)
{
	USoundBase* afterSound = nullptr;

	afterSound = GetSoundRes(InPath);
	if (nullptr == afterSound)
		return 0;

	return PlaySoundObject(afterSound);
}

uint64 UGsSoundPlayer::PlaySoundBgmByPath(const FSoftObjectPath& InPath)
{
	uint64 outAudioId = _bgmID;

	UAudioComponent* afterAudioCom = nullptr;
	USoundBase* afterSound = nullptr;

	afterSound = GetSoundRes(InPath);
	if (nullptr == afterSound)
		return outAudioId;

	if (UAudioComponent* beforeAudioComp = UAudioComponent::GetAudioComponentFromID(outAudioId))
	{
		if (USoundBase* beforeSound = beforeAudioComp->Sound)
		{
			if (beforeSound->GetFullName() != afterSound->GetFullName())
			{
				if (beforeAudioComp->IsPlaying())
				{
					beforeAudioComp->FadeOut(3.0f, 0.0f);
				}
			}
			else
			{
				if (beforeAudioComp->IsPlaying())
				{
					return outAudioId;
				}
			}
		}
	}

	afterAudioCom = UGameplayStatics::CreateSound2D(GetWorld(), afterSound);
	if (nullptr != afterAudioCom)
	{
		afterAudioCom->Play();
		afterAudioCom->FadeIn(2.0f, 1.0f);
		_bgmID = outAudioId = afterAudioCom->GetAudioComponentID();
	}

	return outAudioId;
}

void UGsSoundPlayer::SetAudioParameter(EGsAudioParameterType In_Type, float In_value, UAudioComponent* In_AudioCom)
{
	FString parameterName;
	GetAudioParameterName(In_Type, parameterName);

	switch (In_Type)
	{
	case EGsAudioParameterType::EnvTimeline:
	case EGsAudioParameterType::EnvWeather:
		{
			UAudioComponent* envAudioComponent = GetEnvAudioComponent();
			if (nullptr == envAudioComponent)
				return;

			envAudioComponent->SetFloatParameter(FName(*parameterName), In_value);
		}
		break;
	case EGsAudioParameterType::CreatureWeapon:
	case EGsAudioParameterType::FootPrint:
		{
			if (nullptr == In_AudioCom)
				return;
		
			In_AudioCom->SetIntParameter(FName(*parameterName), static_cast<int>(In_value));
		}
		break;
	case EGsAudioParameterType::HitCritical:
		{
			if (nullptr == In_AudioCom)
				return;

			In_AudioCom->SetBoolParameter(FName(*parameterName), static_cast<bool>(In_value));
		}
		break;
	default:
		break;
	}
}

void UGsSoundPlayer::StopSoundBgm()
{
	if (0 >= _bgmID)
	{
		return;
	}

	if (UAudioComponent* audioComponent = UAudioComponent::GetAudioComponentFromID(_bgmID))
	{
		audioComponent->Stop();

		GSLOG(Log, TEXT("[SoundPlayer] StopSoundBgm ID : %llu"), _bgmID);
	}
}

// AudioComponent ID로 사운드를 중지 또는 종료시킨다.
void UGsSoundPlayer::StopSound(uint64 In_SoundID)
{	
	if (UAudioComponent* audioComponent = UAudioComponent::GetAudioComponentFromID(In_SoundID))
	{
		audioComponent->Stop();
	}
}

// Actor에 어태치 된 사운드를 다 종료시킨다.
void UGsSoundPlayer::StopSound(USceneComponent* In_DetachToComponent)
{
	if (nullptr == In_DetachToComponent)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[SoundPlayer] StopSound -> In_DetachToComponent == nullptr"));
#endif // WITH_EDITOR
		return;
	}

	TArray<USceneComponent*> childComponents;	
	In_DetachToComponent->GetChildrenComponents(true, childComponents);
	for (USceneComponent* childComponent : childComponents)
	{
		if (UAudioComponent * audio = Cast<UAudioComponent>(childComponent))
		{
			audio->Stop();
		}
	}	
}

void UGsSoundPlayer::StopSoundEnv(uint64 In_SoundID)
{
	if (_envID != In_SoundID)
		return;

	if (UAudioComponent* audioComponent = UAudioComponent::GetAudioComponentFromID(In_SoundID))
	{
		audioComponent->FadeOut(2.0f, 0.0f);
	}
}

void UGsSoundPlayer::PauseSound(bool In_IsPause, uint64 In_SoundID)
{
	if (IsPlaying(In_SoundID))
	{
		UAudioComponent* audioComponent = UAudioComponent::GetAudioComponentFromID(In_SoundID);
		if (nullptr != audioComponent)
		{
			audioComponent->SetPaused(In_IsPause);
		}
	}
}

void UGsSoundPlayer::PauseSoundBgm(bool In_IsPause)
{
	if (0 >= _bgmID)
	{
		return;
	}

	if (UAudioComponent* audioComponent = UAudioComponent::GetAudioComponentFromID(_bgmID))
	{
		audioComponent->SetPaused(In_IsPause);
		GSLOG(Log, TEXT("[SoundPlayer] PauseSoundBgm ID : %llu, pause : %d"), _bgmID, In_IsPause);
	}
}

void UGsSoundPlayer::PauseSoundEnv(bool In_IsPause)
{
	if (0 >= _envID)
	{
		return;
	}

	if (UAudioComponent* audioComponent = UAudioComponent::GetAudioComponentFromID(_envID))
	{
		audioComponent->SetPaused(In_IsPause);
		GSLOG(Log, TEXT("[SoundPlayer] PauseSoundEnv ID : %llu, pause : %d"), _envID, In_IsPause);
	}
}

// 해당 사운드가 플레이 중인지 체크한다.
bool UGsSoundPlayer::IsPlaying(uint64 In_SoundID) const
{
	UAudioComponent* audioComponent = UAudioComponent::GetAudioComponentFromID(In_SoundID);
	return (nullptr == audioComponent)? false : audioComponent->IsPlaying();
}

// 사운드 파라미터 key Name 정의
void UGsSoundPlayer::GetAudioParameterName(EGsAudioParameterType InParameterType, OUT FString& outStr)
{	
	switch (InParameterType)
	{
	case EGsAudioParameterType::EnvTimeline:
		outStr = "EGsEnvTimeline";
		break;
	case EGsAudioParameterType::EnvWeather:
		outStr = "EGsEnvWeather";
		break;
	case EGsAudioParameterType::CreatureWeapon:
		outStr = "EGsWeaponType";
		break;
	case EGsAudioParameterType::FootPrint:
		outStr = "EGsFootSoundType";
		break;
	case EGsAudioParameterType::HitCritical:
		outStr = "EGsboolCrit";
		break;
	default:
		outStr = "";
		break;
	}
}

// 현재 플레이 중인 Env을 얻어온다.	
UAudioComponent* UGsSoundPlayer::GetEnvAudioComponent()
{
	if (0 >= _envID)
		return nullptr;

	UAudioComponent* audioComponent = UAudioComponent::GetAudioComponentFromID(_envID);
	if (nullptr == audioComponent)
		return nullptr;

	if (!audioComponent->IsPlaying())
		return nullptr;
	
	return audioComponent;
}

// 사운드 애셋을 얻어온다.
USoundBase* UGsSoundPlayer::GetSoundRes(const FSoftObjectPath& In_SoundResPath)
{
#if 0
	UObject* load = In_SoundResPath.TryLoad();
#else
	USoundBase* load = Cast<USoundBase>(UAssetManager::GetStreamableManager().LoadSynchronous(In_SoundResPath));
#endif

	if (nullptr == load)
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("[SoundPlayer] GetSoundRes : %s - load == nullptr"), *In_SoundResPath.ToString());
#endif // WITH_EDITOR
	}
	return load;

#if 0
#if WITH_EDITOR
	const USoundBase* sound = Cast<USoundBase>(load);
	CheckSoundClassData(sound);
#endif // WITH_EDITOR
	return Cast<USoundBase>(load);
#endif
}

// 리소스 테이블을 얻어온다.
const FGsSchemaSoundResData * UGsSoundPlayer::GetSoundResData(const FString& In_SoundResKeyword) const
{
	const UGsTable* table = FGsSchemaSoundResData::GetStaticTable();
	if (table)
	{
		const FGsSchemaSoundResData* Row = nullptr;
		table->FindRow<FGsSchemaSoundResData>(FName(*In_SoundResKeyword), Row);
		return Row;
	}

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[SoundPlayer] GetSoundResData SoundResKeyword : %s - data == nullptr"), *In_SoundResKeyword);
#endif // WITH_EDITOR

	return nullptr;
}

// 리소스 테이블을 얻어온다.
const FGsSchemaSoundResData * UGsSoundPlayer::GetSoundResData(int In_SoundResID) const
{
	const UGsTableSoundResData* table = Cast<UGsTableSoundResData>(FGsSchemaSoundResData::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaSoundResData* schemaSoundResData = nullptr;
	if (false == table->FindRowById(In_SoundResID, schemaSoundResData))
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[SoundPlayer] GetSoundResData SoundResID : %d - data == nullptr"), In_SoundResID);
#endif // WITH_EDITOR

		return nullptr;
	}

	return schemaSoundResData;
}

// class 셋팅이 안되는 경우 체크
void UGsSoundPlayer::CheckSoundClassData(const USoundBase* In_Sound)
{
	if (nullptr == In_Sound)
		return;

	if (USoundClass* soundClass = In_Sound->GetSoundClass())
	{
		if (TEXT("Master") == soundClass->GetName())
		{
			GSLOG(Warning, TEXT("[SoundPlayer] CheckSoundClassData : %s - SoundClass == Master"), *In_Sound->GetName());
		}
	}
	else
	{
		GSLOG(Warning, TEXT("[SoundPlayer] CheckSoundClassData : %s - SoundClass == nullptr"), *In_Sound->GetName());
	}
}

