// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSoundManager.h"
#include "Sound/GsSoundMixerController.h"
#include "Camera/CameraComponent.h"

#include "Engine/Classes/Sound/SoundBase.h"

#include "Classes/Engine/DataTable.h"
#include "Data/GsDataContainManager.h"

#include "Engine/Engine.h"
#include "Engine/Public/ActiveSound.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

#include "Management/ScopeGame/GsSkillPreviewManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectPlayableEventLocalPlayer.h"

#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ControllerEx/GsPlayerController.h"

#include "GsGameFlowManager.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"

#include "GameMode/GsGameModeBase.h"
#include "GameMode/GsGameModeDefine.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeLobby/GsLobbyCharacterManager.h"

#include "GameObject/Fence/GsFenceHandler.h"
#include "GsLevelManager.h"
#include "GsGameDataManager.h"
#include "Management/ScopeGame/GsInvadeBattleLevelManager.h"
#include "DataSchema/InterServer/GsSchemaInvadeWorldConfigDataEx.h"

#include "Option/GsGameUserSettings.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Message/GsMessageSystem.h"


void UGsSoundManager::BeginDestroy()
{
	Super::BeginDestroy();

	_soundMixerController	= nullptr;
	_soundPlayer			= nullptr;
}

void UGsSoundManager::Initialize()
{
	BindMessage();
}

void UGsSoundManager::Finalize()
{
	if (nullptr != _soundMixerController)
	{
		_soundMixerController->ConditionalBeginDestroy();
	}

	if (nullptr != _soundPlayer)
	{
		_soundPlayer->ConditionalBeginDestroy();
	}

	UnbindMessage();

	StopBGM();
}

void UGsSoundManager::Update(float inTick)
{	
	UpdateAudioListener(inTick);
}

void UGsSoundManager::InitializeAfterResourceDownload()
{
	
}

void UGsSoundManager::BindMessage()
{
	MSound& sound = GMessage()->GetSound();

	_soundDelegates.Emplace(sound.AddUObject(MessageSound::PLAY_BGM, this, &UGsSoundManager::OnPlayBGM));
	_soundDelegates.Emplace(sound.AddUObject(MessageSound::STOP_BGM, this, &UGsSoundManager::OnStopBGM));
	_soundDelegates.Emplace(sound.AddUObject(MessageSound::PAUSE_BGM, this, &UGsSoundManager::OnPauseBGM));
}

void UGsSoundManager::UnbindMessage()
{
	MSound& sound = GMessage()->GetSound();
	for (TPair<MessageSound, FDelegateHandle> soundHandle : _soundDelegates)
	{
		if (soundHandle.Value.IsValid())
		{
			sound.Remove(soundHandle);
		}		
	}
}

void UGsSoundManager::UpdateAudioListener(float inTick)
{
	UGsGameObjectBase* player = nullptr;

	bool isLobby = false;
	if (UWorld* world = GetWorld())
	{
		if (AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>())
		{
			if (gameMode->GetGameModeType() == EGsGameModeType::Lobby)
			{
				isLobby = true;
			}
		}
	}
	if (isLobby == false)
	{
		UGsGameObjectManager* objectManager = GSGameObject();
		if (nullptr == objectManager)
			return;

		player = objectManager->FindObject(EGsGameObjectType::LocalPlayer);
		
	}
	else
	{
		UGsLobbyCharacterManager* lobbyCharManager = LSLobbyCharacter();
		if (nullptr == lobbyCharManager)
		{
			return;
		}

		player = lobbyCharManager->GetPlayableEventLocalPlayer();
	}
	if (nullptr == player)
	{
		return;
	}

	UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(player);
	AGsCharacterLocalPlayer* localChar = localPlayer->GetLocalCharacter();
	if (nullptr == localChar)
		return;

	AGsPlayerController* controller = localChar->GetPlayerController();
	if (nullptr == controller)
		return;

	UCameraComponent* cameraComponent = localChar->GetFollowCamera();
	if (nullptr == cameraComponent)
		return;

	AActor* actor = localPlayer->GetActor();
	if (nullptr == actor)
		return;

	FVector vec = actor->GetActorLocation();
	if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
	{
		// 특정 컨텐츠 모드에서는 무시
		if (FGsContentsManagerGame* contentsManagerGame = gameFlow->GetContentsManagerGame())
		{
			if (contentsManagerGame->GetCurrentContentsMode() == FGsContentsMode::ContentsSkill)
			{
				if (UGsSkillPreviewManager* skillPreviewManager = GSSkillPreview())
				{
					if (class UGsGameObjectBase* object =
						skillPreviewManager->FindObject((int64)EGsSkillPreivewGameObjectStartID::OwnerPlayer))
					{
						vec = object->GetLocation();
					}

					cameraComponent = skillPreviewManager->GetSceneCamera();
				}
			}
		}
	}

	if (cameraComponent)
	{
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(cameraComponent->GetComponentLocation(), vec);
		controller->SetAudioListenerOverride(nullptr, vec, rot);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SoundMixer
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UGsSoundManager::StaticPushSoundMixer(EGsSoundMixType In_type)
{
	if (UGsSoundManager* soundManager = GSound())
	{
		if (UGsSoundMixerController* mixerContoller = soundManager->GetOrCreateSoundMixerController())
		{
			mixerContoller->OnMixChanged(In_type);
		}
	}
}

void UGsSoundManager::StaticPopSoundMixer(EGsSoundMixType In_type)
{
	if (UGsSoundManager* soundManager = GSound())
	{
		if (UGsSoundMixerController* mixerContoller = soundManager->GetOrCreateSoundMixerController())
		{
			mixerContoller->OffMixMode(In_type);
		}
	}
}

UGsSoundMixerController* UGsSoundManager::GetOrCreateSoundMixerController()
{
	if (nullptr == _soundMixerController)
	{
		_soundMixerController = NewObject<UGsSoundMixerController>(this);
		_soundMixerController->Initialize();
	}

	return _soundMixerController;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SoundPlayer
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UGsSoundPlayer* UGsSoundManager::GetOrCreateSoundPlayer()
{
	if (nullptr == _soundPlayer)
	{
		_soundPlayer = NewObject<UGsSoundPlayer>(this);
		_soundPlayer->Initialize();
	}

	return _soundPlayer;
}

/// <summary>
/// 배경 음악 우선 순위
/// 1. 사운드 펜스
/// 2. 침공전 펜스
/// 3. 마을 펜스
/// 4. 필드 내 침공전
/// 5. 필드 내 맵 배경 음악
/// </summary>
/// <param name="InParam"></param>
void UGsSoundManager::OnPlayBGM(const struct IGsMessageParam* InParam)
{		
	//배경 음악이 존재하는 펜스 중 가장 높은 우선 순위의 펜스를 가져온다. 
	//펜스에 음악이 설정되어있지 않을 경우 필드 이벤트 음악을 기준으로 나간다.
	//1. 사운드 펜스
	//2. 침공전 펜스
	//3. 마을 펜스, 아레나 레디, 아레나, Arrival
	if (TryPlayFenceSound())
	{
		return;
	}
	
	if (TryPlayInvadeSound())
	{
		return;
	}
	
	//필드 내 맵 배경 음악 및 환경 음악 재생	
	PlayMapBGM();
}

void UGsSoundManager::StopBGM()
{
	if (nullptr == _soundPlayer)
	{
		return;
	}

	StopBGMInternal();
	StopEnvInternal();
}

void UGsSoundManager::OnPauseBGM(const struct IGsMessageParam* InParam)
{
	const FGsPrimitiveUInt8* Param =
		InParam->Cast<const FGsPrimitiveUInt8>();

	PauseBGM(static_cast<bool>(Param->_data));
}

bool UGsSoundManager::TryPlayFenceSound()
{
	UGsGameObjectManager* gameObjectManager = GSGameObject();
	if (nullptr == gameObjectManager)
	{
		return false;
	}

	UGsGameObjectBase* find = gameObjectManager->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == find)
	{
		return false;
	}

	UGsGameObjectLocalPlayer* player = find->CastGameObject<UGsGameObjectLocalPlayer>();
	if (nullptr == player)
	{
		return false;
	}

	AGsCharacterBase* playerCharacter = player->GetCharacter();
	if (nullptr == playerCharacter)
	{
		return false;
	}
			
	FVector location = playerCharacter->GetActorLocation();
	FGsFence* soundFence = nullptr;
	if (false == GFence()->TryGetFirstPrioritySoundFence(soundFence, location))
	{
		return false;
	}
	
	if (soundFence->GetTerritoryType() == TerritoryType::TOWN_CHAOS)
	{
		if (TryPlayInvadeSound())
		{
			return true;
		}
	}
	else
	{
		FGsSchemaSoundResDataRow soundRes;
		if (soundFence->TryGetActiveSound(soundRes))
		{
			PlayBGM(soundRes.GetRow());
			return true;
		}
	}

	return false;
}

bool UGsSoundManager::TryPlayInvadeSound()
{
	if (FGsGameDataManager* gameDataManager = GGameData())
	{
		//필드 내 침공전 배경 음악 재생
		if (gameDataManager->GetInvasionDefenseState()
			|| gameDataManager->IsInvadeWorld())
		{
			PlayInvadeBGM();
			return true;
		}
	}

	return false;
}

void UGsSoundManager::PauseBGM(bool inValue)
{
	PauseBGMInternal(inValue);
	PauseEnvInternal(inValue);
}

void UGsSoundManager::PauseBGMInternal(bool inValue)
{
	if (_soundPlayer)
	{
		_soundPlayer->PauseSoundBgm(inValue);
	}
}

void UGsSoundManager::PauseEnvInternal(bool inValue)
{
	if (_soundPlayer)
	{
		_soundPlayer->PauseSoundEnv(inValue);
	}
}

void UGsSoundManager::StopBGMInternal()
{
	if (0 != _currentBgmId)
	{
		_soundPlayer->StopSound(_currentBgmId);
		_currentBgmSoundResData = nullptr;
		_currentBgmId = 0;
	}
}

void UGsSoundManager::StopEnvInternal()
{
	if (0 != _currentEnvId)
	{
		_soundPlayer->StopSound(_currentEnvId);
		_currentEnvSoundResData = nullptr;
		_currentEnvId = 0;
	}
}

void UGsSoundManager::OnStopBGM(const struct IGsMessageParam* InParam)
{
	StopBGM();
}

//펜스의 BGM을 플레이 하기 위한 함수
void UGsSoundManager::PlayBGM(const FGsSchemaSoundResData* inRes)
{
	if (nullptr == inRes)
	{
		StopBGMInternal();
		return;
	}

	if (_currentBgmSoundResData != inRes)
	{
		if (UGsSoundPlayer* soundPlayer = GetOrCreateSoundPlayer())
		{
			soundPlayer->StopSound(_currentBgmId);
			_currentBgmSoundResData = inRes;
			_currentBgmId = soundPlayer->PlaySoundBgm(inRes);
		}
	}	
	else
	{
		PauseBGMInternal(false);
	}
}

void UGsSoundManager::PlayEnv(const FGsSchemaSoundResData* inRes)
{
	if (nullptr == inRes)
	{
		StopEnvInternal();
		return;
	}

	if (_currentEnvSoundResData != inRes)
	{
		_soundPlayer->StopSound(_currentEnvId);
		_currentEnvSoundResData = inRes;
		_currentEnvId = _soundPlayer->PlaySoundEnv(inRes);
	}
	else
	{
		PauseEnvInternal(false);
	}
}

//침공전 배경 음악 재생
void UGsSoundManager::PlayInvadeBGM()
{
	const FGsSchemaInvadeWorldConfigDataEx* config = FGsInvadeBattleLevelManager::GetInvadeWorldConfigDataEx();
	if (nullptr == config)
	{
		return;
	}
	
	const FGsSchemaSoundResData* bgmSound = config->invasionBGM.GetRow();
	if (bgmSound)
	{
		StopEnvInternal();
		PlayBGM(bgmSound);
	}
	else
	{
		StopBGMInternal();
		GSLOG(Error, TEXT("invade bgm is missing"));		
	}
}

//맵 배경 음악 및 환경 음악 재생 
void UGsSoundManager::PlayMapBGM()
{
	UGsLevelManager* levelManager = GLevel();
	if (nullptr == levelManager)
	{
		return;
	}

	//마을
	const FGsSchemaMapData* mapData = levelManager->GetCurrentMapInfo();
	if (mapData)
	{
		//배경 음악 재생
		const FGsSchemaSoundResData* bgmSound = mapData->bgm.GetRow();
		if (bgmSound)
		{
			PlayBGM(bgmSound);
		}
		else
		{
			StopBGMInternal();
			GSLOG(Warning, TEXT("map bgm is missing - map id : %d"), mapData->id);
		}
	}	

	const FGsSchemaAreaData* areaData = mapData->areaId.GetRow();
	if (areaData)
	{
		//환경 음악 재생(새 소리 등)
		const FGsSchemaSoundResData* envSound = areaData->areaEnv.audioPath.GetRow();
		if (envSound)
		{
			PlayEnv(envSound);
		}
		else
		{
			StopEnvInternal();
			GSLOG(Warning, TEXT("area env bgm is missing - area id : %d"), areaData->id);
		}
	}	
}

void UGsSoundManager::SetSoundMaster()
{	
	/*if (UGsSoundMixerController* mixer = GetOrCreateSoundMixerController())
	{
		mixer->SetGameMaster();		
	}	

	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		gameUserSettings->ApplyAudioSettings();
	}*/
}

void UGsSoundManager::SetSoundLobbyMaster()
{
	/*if (UGsSoundMixerController* mixer = GetOrCreateSoundMixerController())
	{
		mixer->SetGameLobbyMaster();
	}

	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		gameUserSettings->ApplyAudioSettings();
	}*/
}