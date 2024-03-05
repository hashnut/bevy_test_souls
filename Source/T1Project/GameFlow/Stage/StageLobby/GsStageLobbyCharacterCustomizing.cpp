// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStageLobbyCharacterCustomizing.h"
#include "ActorEx/GsCharacterLobbyPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFlow/Stage/GsStageMode.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "Management/ScopeLobby/GsLobbyCharacterManager.h"
#include "Management/ScopeLobby/GsLobbyCharacterPresetManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "DataCenter/Public/Camera/Define/EGsCameraLobbyState.h"
#include "DataSchema/Camera/GsSchemaCustomizeCameraData.h"
#include "DataSchema/Camera/GsSchemaContentsCameraData.h"

#include "Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Camera/CameraComponent.h"

#include "Sound/GsSoundPlayer.h"

#include "Camera/Lobby/GsCameraHandlerLobby.h"

FGsStageLobbyCharacterCustomizing::FGsStageLobbyCharacterCustomizing() : FGsStageLobbyBase(FGsStageMode::Lobby::CHARACTER_CUSTOMIZING)
{
}

FGsStageLobbyCharacterCustomizing::~FGsStageLobbyCharacterCustomizing()
{
}

void FGsStageLobbyCharacterCustomizing::Enter()
{
	FGsStageLobbyBase::Enter();

	UGsLobbyCharacterPresetManager* characterPresetMgr = LSLobbyCharacterPreset();
	if (nullptr == characterPresetMgr)
		return;

	if(ACharacter* spawnChar = SpawnActor(characterPresetMgr->GetGenderType()))
	{
		if (AGsCharacterLobbyPlayer* lobbyChar = Cast<AGsCharacterLobbyPlayer>(spawnChar))
		{
			InitializeLookCameraNode(FGsStageMode::Lobby::CHARACTER_CUSTOMIZING, lobbyChar);
		}
	}

	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		soundPlayer->PlaySoundBgm("Character");
	}
}

void FGsStageLobbyCharacterCustomizing::Exit()
{
	if (LSLobbyCharacter())
	{
		LSLobbyCharacter()->RemoveAll();
	}

	FGsStageLobbyBase::Exit();
}

void FGsStageLobbyCharacterCustomizing::Update(float In_deltaTime)
{
	FGsStageLobbyBase::Update(In_deltaTime);

	if (nullptr == _character)
	{
		return;
	}

	USkeletalMeshComponent* skeletalComponent = _character->GetMesh();
	if (nullptr == skeletalComponent)
	{
		return;
	}

	FGsCameraHandlerLobby* lobbyCam = _character->GetCameraHandlerLobby();
	if (lobbyCam == nullptr)
	{
		return;
	}

	FVector nowHeadPos = GetCurrentHeadPos(skeletalComponent);

	lobbyCam->SetCurrentHeadPos(nowHeadPos);
}

FVector FGsStageLobbyCharacterCustomizing::GetCurrentHeadPos(USkeletalMeshComponent* In_skeletalMeshComponent)
{
	FVector findPos = In_skeletalMeshComponent->GetBoneLocation(LobbyCamHeadBoneName);

	return findPos;
}

FName FGsStageLobbyCharacterCustomizing::GetCurrentPosTag()
{
	return GData()->GetGlobalData()->Lobby_Pos_CustomizeCharacter;
}

AGsCharacterLobbyPlayer* FGsStageLobbyCharacterCustomizing::SetCharacter(CreatureGenderType InGenderType)
{
	if (nullptr == _character)
	{
		_character = Cast<AGsCharacterLobbyPlayer>(SpawnActor(InGenderType));

		// 카메라 설정값 초기화
		InitializeCamera(EGsCameraLobbyState::CHARACTER_CUSTOMIZING, EGsContentsCameraType::LobbyCustomizing);

		// init actor rotation
		InitializeActorRotation();
		return _character;
	}
	else
	{
		// 버그
	}

	return nullptr;
}

ACharacter* FGsStageLobbyCharacterCustomizing::SpawnActor(CreatureGenderType InGenderType)
{
	ACharacter* character = FGsStageLobbyBase::SpawnActor(InGenderType);
	return character;
}
void FGsStageLobbyCharacterCustomizing::InitializeActorRotation()
{
	if (nullptr == _character)
	{
		return;
	}

	UCameraComponent* camComponent = _character->GetFollowCamera();
	if (camComponent == nullptr)
	{
		return;
	}

	FVector camPos = camComponent->GetComponentLocation();

	UGsLobbyCharacterManager* characterMgr = LSLobbyCharacter();
	if (characterMgr == nullptr)
	{
		return;
	}

	const FGsSchemaContentsCameraData* currentCameraContentsData = characterMgr->GetCurrentCameraContentsData();
	if (nullptr == currentCameraContentsData)
	{
		return;
	}

	FVector currentContentsOffset = currentCameraContentsData->CamearPositionOffset;

	const FGsSchemaCustomizeCameraData* currentData = characterMgr->GetCurrentCustomizeCameraData();
	if (nullptr == currentData)
	{
		return;
	}

	currentContentsOffset.Y += currentData->WholeBodyShiftCameraPosY;

	FVector lastCamPos = camPos + currentContentsOffset;

	FVector actorLocation = _character->GetActorLocation();

	FVector dirLookVec = actorLocation - lastCamPos;

	dirLookVec.Normalize();
	FRotator dirLookRot = dirLookVec.Rotation();

	FRotator nowRot = _character->GetActorRotation();
	nowRot.Yaw = dirLookRot.Yaw;

	_character->SetActorRotation(nowRot);
}