// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSoundMixerController.h"
#include "Engine/Classes/Sound/SoundMix.h"

#include "AudioDevice.h"

#include "T1Project.h"

#include "GsTableManager.h"
#include "Data/GsDataContainManager.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Sound/GsSchemaSoundMixData.h"
#include "DataSchema/Sound/GsSchemaSoundClassData.h"
#include "UTIL/GsText.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Message/GsMessageSystem.h"
#include "Management/GsMessageHolder.h"


void UGsSoundMixerController::BeginDestroy()
{
	Super::BeginDestroy();

	_soundMixMaster = nullptr;
	_soundMix = nullptr;
	_soundMixList.Empty();
}

void UGsSoundMixerController::Initialize()
{
	if(USoundMix* soundMix = GetSoundMix(EGsSoundMixType::Master))
	{
		_soundMixMaster = soundMix;
		if (nullptr != _soundMixMaster)
		{
			_soundMix = _soundMixMaster;
		}
	}

	_soundMixList.Add(EGsSoundMixType::Master);
	RegisterMessages();
}

void UGsSoundMixerController::Finalize()
{
	UnregisterMessages();
	_soundMixMaster = nullptr;
	_soundMix = nullptr;
	_soundMixList.Empty();
}


void UGsSoundMixerController::RegisterMessages()
{
	// 메시지 등록
	FGsMessageHolder* msg = GMessage();

	MSystem& msgSystem = msg->GetSystem();
	_msgSystemHandleList.Emplace(msgSystem.AddUObject(MessageSystem::RECONNECT_END,
		this, &UGsSoundMixerController::OnReconnectEnd));
	_msgSystemHandleList.Emplace(msgSystem.AddUObject(MessageSystem::GAMEMODE_WORLD_EXIT,
		this, &UGsSoundMixerController::OnGameModeWorldExit));
}

void UGsSoundMixerController::UnregisterMessages()
{
	// 메시지 해제
	FGsMessageHolder* msg = GMessage();

	MSystem& msgSystem = msg->GetSystem();
	for (MsgSystemHandle& msgSystemHandle : _msgSystemHandleList)
	{
		msgSystem.Remove(msgSystemHandle);
	}
	_msgSystemHandleList.Empty();
}

void UGsSoundMixerController::OnReconnectEnd()
{
	ClserSoundMixer();
}

void UGsSoundMixerController::OnGameModeWorldExit()
{
	ClserSoundMixer();
}

void UGsSoundMixerController::ClserSoundMixer()
{
	_soundMix = nullptr;
	_soundMixList.Empty();

	// 모든 믹서 클리어
	UGameplayStatics::ClearSoundMixModifiers(GetWorld());
	// 마스터 믹서 셋팅
	USoundMix* soundMix = GetSoundMix(EGsSoundMixType::Master);
	if (nullptr != soundMix)
	{
		_soundMix = soundMix;
		_soundMixList.Add(EGsSoundMixType::Master);

		UGameplayStatics::PushSoundMixModifier(GetWorld(), soundMix);		

#if SOUND_DEBUG
		FString pathName = _soundMix->GetPathName();
		GSLOG(Log, TEXT("UGsSoundMixerController::ClserSoundMix MasterMixerPath : %s"), *pathName);
#endif	
	}
}

// 사운드 볼륨을 조절한다.
void UGsSoundMixerController::SetVolume(EGsSoundVolumeType In_Type, float In_Value)
{
	USoundClass* soundClass = GetSoundClass(In_Type);
	if (nullptr == soundClass)
	{		
		FString volumeType = FGsTextUtil::GetStringFromEnum(TEXT("EGsSoundVolumeType"), In_Type);
		GSLOG(Error, TEXT("sound class is missing - type : %s"), *volumeType);
		return;
	}

	soundClass->Properties.Volume = FMath::Clamp<float>(In_Value, 0.f, 1.f);

#if !UE_BUILD_SHIPPING
	FString volumeType = FGsTextUtil::GetStringFromEnum(TEXT("EGsSoundVolumeType"), In_Type);
	GSLOG(Log, TEXT("sound debug - type : %s\tvolume : %f"), *volumeType, In_Value);
#endif	
}

// 사운드믹서를 변경한다.
void UGsSoundMixerController::OnMixChanged(EGsSoundMixType In_Type)
{
	int num = _soundMixList.Num();
	_soundMixList.Add(In_Type);

	USoundMix* soundMixPush = nullptr;
	USoundMix* soundMixPop = nullptr;

	if (num > 0)
	{
		soundMixPush = GetSoundMix(_soundMixList[num]);

#if SOUND_DEBUG				
		{
			FString mixType = FGsTextUtil::GetStringFromEnum(TEXT("EGsSoundMixType"), _soundMixList[num]);
			GSLOG(Log, TEXT("UGsSoundMixerController - OnMixChanged - push type : %s"), *mixType);
		}
#endif
	}

	if (num >= 1)
	{
		soundMixPop = GetSoundMix(_soundMixList[num - 1]);
	}

	if (nullptr != soundMixPush &&
		nullptr != soundMixPop)
	{
		SetSoundMix(soundMixPush, soundMixPop);

#if SOUND_DEBUG				
		{
			FString mixType = FGsTextUtil::GetStringFromEnum(TEXT("EGsSoundMixType"), _soundMixList[num - 1]);
			GSLOG(Log, TEXT("UGsSoundMixerController - OnMixChanged - pop type : %s"), *mixType);
		}
#endif
	}

#if SOUND_DEBUG		
	{
		FString log = ToString();
		GSLOG(Log, TEXT("%s"), *log);
	}
#endif
}

// 대상의 사운드믹서를 끄고 디폴트로 변경한다.
void UGsSoundMixerController::OffMixMode(EGsSoundMixType In_Type)
{
	int num = _soundMixList.Num();

	USoundMix* soundMixPush = nullptr;
	USoundMix* soundMixPop = nullptr;

	int32 IndexLast;
	if (_soundMixList.FindLast(In_Type, IndexLast))
	{
		soundMixPop = GetSoundMix(_soundMixList[IndexLast]);
		_soundMixList.RemoveAt(IndexLast);

#if SOUND_DEBUG				
		{
			FString mixType = FGsTextUtil::GetStringFromEnum(TEXT("EGsSoundMixType"), In_Type);
			GSLOG(Log, TEXT("UGsSoundMixerController - OffMixMode - pop - type : %s"), *mixType);
		}		
#endif

#if SOUND_DEBUG		
		{
			FString log = ToString();
			GSLOG(Log, TEXT("%s"), *log);
		}
#endif
	}

	num = _soundMixList.Num();
	if (num > 0)
	{
		soundMixPush = GetSoundMix(_soundMixList[num - 1]);

#if SOUND_DEBUG				
		{
			FString mixType = FGsTextUtil::GetStringFromEnum(TEXT("EGsSoundMixType"), _soundMixList[num - 1]);
			GSLOG(Log, TEXT("UGsSoundMixerController - OffMixMode - push - type : %s"), *mixType);
		}
#endif

#if SOUND_DEBUG		
		{
			FString log = ToString();
			GSLOG(Log, TEXT("%s"), *log);
		}
#endif
	}

	if (nullptr != soundMixPush &&
		nullptr != soundMixPop)
	{
		SetSoundMix(soundMixPush, soundMixPop);
	}
}

// 마스터믹서로 돌아간다.
void UGsSoundMixerController::OffMixMode()
{
	if (nullptr != _soundMix &&
		nullptr != _soundMixMaster)
	{
		SetSoundMix(_soundMixMaster, _soundMix);
	}
}

// 사운드 믹서를 꺼내고 변경한다.
void UGsSoundMixerController::SetSoundMix(USoundMix* In_PushSoundMix, USoundMix* In_PopSoundMix)
{	
	if (In_PopSoundMix)
	{
		UGameplayStatics::ClearSoundMixModifiers(GetWorld());

#if SOUND_DEBUG			
		FString pathName = In_PopSoundMix->GetPathName();
		GSLOG(Log, TEXT("UGsSoundMixerController - SetSoundMix - ClearSoundMixModifiers - path : %s"), *pathName);
#endif	
	}

	if (nullptr != In_PushSoundMix)
	{
		UGameplayStatics::PushSoundMixModifier(GetWorld(), In_PushSoundMix);		

#if SOUND_DEBUG
		FString pathName = In_PushSoundMix->GetPathName();		
		GSLOG(Log, TEXT("UGsSoundMixerController - SetSoundMix - PushSoundMixModifier - path : %s"), *pathName);
#endif	
	}

#if SOUND_DEBUG
	TMap<USoundMix*, FSoundMixState> mixMap = GetWorld()->GetAudioDevice()->GetSoundMixModifiers();
	for (const TPair<USoundMix*, FSoundMixState>& mixState : mixMap)
	{		
		GSLOG(Log, TEXT("UGsSoundMixerController - SetSoundMix - GetSoundMixModifiers - path : %s\tref count : %d")
			, *mixState.Key->GetPathName()
			, mixState.Value.ActiveRefCount);
	}
#endif
}

// 사운드 믹서 애셋을 얻어온다.
const FGsSchemaSoundMixData* UGsSoundMixerController::GetSoundMixData(EGsSoundMixType In_Type) const
{
	const UGsTableManager* tblManager = &(UGsTableManager::GetInstance());
	if (tblManager == nullptr)
	{
#if !UE_BUILD_SHIPPING	
		FString mixTypeType = FGsTextUtil::GetStringFromEnum(TEXT("EGsSoundMixType"), In_Type);
		GSLOG(Error, TEXT("[sound] GetSoundMixData -> tblManager == nullptr : \tSoundMixType : %s"), *mixTypeType);
#endif
		return nullptr;
	}

	const UGsTable* table = FGsSchemaSoundMixData::GetStaticTable();
	if (nullptr == table)
	{
#if !UE_BUILD_SHIPPING
		GSLOG(Error, TEXT("[SoundMixData] Fail to find row. key: %s"), *FString::FromInt((uint8)In_Type));
#endif
		return nullptr;
	}
	
	const FGsSchemaSoundMixData* Row = nullptr;
	table->FindRow<FGsSchemaSoundMixData>(FName(*FString::FromInt((uint8)In_Type)), Row);
	return Row;
}

const FGsSchemaSoundClassData* UGsSoundMixerController::GetSoundClassData(EGsSoundVolumeType In_Type) const
{
	const UGsTableManager* tblManager = &(UGsTableManager::GetInstance());
	if (tblManager == nullptr)
	{
#if !UE_BUILD_SHIPPING	
		FString volumeType = FGsTextUtil::GetStringFromEnum(TEXT("EGsSoundVolumeType"), In_Type);
		GSLOG(Error, TEXT("[sound] GetSoundClassData -> tblManager == nullptr : \volumeType : %s"), *volumeType);
#endif

		return nullptr;
	}

	const UGsTable* table = FGsSchemaSoundClassData::GetStaticTable();
	if (nullptr == table)
	{
#if !UE_BUILD_SHIPPING	
		FString volumeType = FGsTextUtil::GetStringFromEnum(TEXT("EGsSoundVolumeType"), In_Type);
		GSLOG(Error, TEXT("[sound] GetSoundClassData -> table == nullptr : \volumeType : %s"), *volumeType);
#endif

		return nullptr;
	}

	const FGsSchemaSoundClassData* Row = nullptr;
	if(false == table->FindRow<FGsSchemaSoundClassData>(FName(*FString::FromInt((uint8)In_Type)), Row))
	{
#if !UE_BUILD_SHIPPING	
		GSLOG(Error, TEXT("[SoundMixData] Fail to find row. key: %s"), *FString::FromInt((uint8)In_Type));
#endif

		return nullptr;		
	}

	return Row;
}

USoundMix* UGsSoundMixerController::GetSoundMix(EGsSoundMixType In_Type)
{
	const FGsSchemaSoundMixData* mixData = GetSoundMixData(In_Type);

	if (nullptr != mixData)
	{
#if 0		
		USoundMix* load = Cast<USoundMix>(UAssetManager::GetStreamableManager().LoadSynchronous(mixData->soundMixPath));
#else
		USoundMix* load = Cast<USoundMix>(mixData->soundMixPath.TryLoad());
#endif		

		if (nullptr != load)
		{
			return load;
		}
		else
		{
#if !UE_BUILD_SHIPPING	
			GSLOG(Error, TEXT("[SoundMixer] GetMixerRes : %s - load == nullptr"), *mixData->soundMixPath.ToString());
#endif
		}
	}

	return nullptr;
}

FString UGsSoundMixerController::ToString()
{	
	FString log = TEXT("UGsSoundMixerController - mix list : ");
	for (EGsSoundMixType mix : _soundMixList)
	{		
		log += (TEXT("\t") + FGsTextUtil::GetStringFromEnum(TEXT("EGsSoundMixType"), mix));
	}

	return log;
}

USoundClass* UGsSoundMixerController::GetSoundClass(EGsSoundVolumeType In_Type)
{
	const FGsSchemaSoundClassData* classData = GetSoundClassData(In_Type);

	if (nullptr != classData)
	{
#if 0		
		USoundClass* load = Cast<USoundClass>(UAssetManager::GetStreamableManager().LoadSynchronous(classData->soundClassPath));
#else
		USoundClass* load = Cast<USoundClass>(classData->soundClassPath.TryLoad());
#endif		

		if (nullptr != load)
		{
			return load;
		}
		else
		{
#if !UE_BUILD_SHIPPING	
			GSLOG(Error, TEXT("[SoundMixer] GetMixerRes : %s - load == nullptr"), *classData->soundClassPath.ToString());
#endif
		}
	}

	return nullptr;
}