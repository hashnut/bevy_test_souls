// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStageLobbyCharacterSelect.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Classes/Components/SkeletalMeshComponent.h"

#include "ActorEx/GsCharacterLobbyPlayer.h"

#include "GameFlow/Stage/GsStageMode.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "Management/ScopeLobby/GsLobbyCharacterManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"

#include "DataCenter/Public/Camera/Define/EGsCameraLobbyState.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Sound/GsSoundPlayer.h"

#include "Camera/Lobby/GsCameraHandlerLobby.h"

#include "UTIL/GsGameObjectUtil.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "Animation/GsAnimInstanceState.h"
#include "GameFramework/SpringArmComponent.h"

// 애니랑 상관없이 고정된 head 본 값
const FName LobbyCamFixedHeadBoneName = TEXT("FXHeadSocket");
const FName LOBBY_BONE_CENTER = TEXT("Bip001");

FGsStageLobbyCharacterSelect::FGsStageLobbyCharacterSelect() : FGsStageLobbyBase(FGsStageMode::Lobby::CHARACTER_SELECT)
{
}

FGsStageLobbyCharacterSelect::~FGsStageLobbyCharacterSelect()
{
}

void FGsStageLobbyCharacterSelect::Enter()
{
	Super::Enter();

	// 남,여 선 스폰후 hide
	if (ACharacter* character = SpawnActor(CreatureGenderType::MALE))
	{
		character->SetActorHiddenInGame(true);
		character->SetActorEnableCollision(false);
		if (AGsCharacterLobbyPlayer* lobbyChar = Cast<AGsCharacterLobbyPlayer>(character))
		{
			InitializeLookCameraNode(FGsStageMode::Lobby::CHARACTER_SELECT, lobbyChar);
		}
	}

	if (ACharacter* character = SpawnActor(CreatureGenderType::FEMALE))
	{
		character->SetActorHiddenInGame(true);
		character->SetActorEnableCollision(false);

		if (AGsCharacterLobbyPlayer* lobbyChar = Cast<AGsCharacterLobbyPlayer>(character))
		{
			InitializeLookCameraNode(FGsStageMode::Lobby::CHARACTER_SELECT, lobbyChar);
		}
	}

	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		soundPlayer->PlaySoundBgm("CharacterSelect");
	}
}

void FGsStageLobbyCharacterSelect::Exit()
{
	if (LSLobbyCharacter())
	{
		LSLobbyCharacter()->RemoveAll();
	}

	Super::Exit();
}

void FGsStageLobbyCharacterSelect::Update(float In_deltaTime)
{
	Super::Update(In_deltaTime);

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

AGsCharacterLobbyPlayer* FGsStageLobbyCharacterSelect::SetCharacter(CreatureGenderType InGenderType)
{
	if (_character && _currentGenderType == InGenderType)
	{
		return _character;
	}

	// 기존 캐릭터가 있다면??
	if (_character)
	{
		ShowCharacter(false);
	}
	
	// 생성 요청 및 설정
	_character = Cast<AGsCharacterLobbyPlayer>(SpawnActor(InGenderType));
	if (_character)
	{
		// 애니메이션 초기화
		if (UGsAnimInstanceState* anim = _character->GetAnim())
		{
			anim->ResetCurrentState(anim->StateMachineName);
		}
	}

	ShowCharacter(true);

	if (APlayerController* controller = _character->GetWorld()->GetFirstPlayerController())
	{
		controller->Possess(_character);
		controller->SetViewTarget(_character);
	}

	// 카메라 설정값 초기화
	float zoomRate = GData()->GetGlobalData()->SelectInitZoomRate;
	InitializeCamera(EGsCameraLobbyState::CHARACTER_SELECT, EGsContentsCameraType::LobbyCharacterSelect,zoomRate);



	return _character;
}

void FGsStageLobbyCharacterSelect::ShowCharacter(bool bInShow)
{
	if (_character)
	{
		_character->SetActorHiddenInGame(!bInShow);
		_character->SetActorEnableCollision(bInShow);
	}
}

ACharacter* FGsStageLobbyCharacterSelect::SpawnActor(CreatureGenderType InGenderType)
{
	ACharacter* character = FGsStageLobbyBase::SpawnActor(InGenderType);
	return character;
}

FVector FGsStageLobbyCharacterSelect::GetCurrentHeadPos(USkeletalMeshComponent* In_skeletalMeshComponent)
{
	FVector findPos = FVector::ZeroVector;
	
	if (nullptr == In_skeletalMeshComponent)
	{
		return FVector::ZeroVector;
	}

	FTransform headTm =
		In_skeletalMeshComponent->GetSocketTransform(LobbyCamFixedHeadBoneName);

	FVector headSocketPos = headTm.GetLocation();

	FVector vecSpaceConstant = In_skeletalMeshComponent->GetBoneLocation(FName("Root"));
	FVector vecSpaceBot = In_skeletalMeshComponent->GetBoneLocation(FName("Bip001-L-Toe0"));

	float offsetZ = vecSpaceConstant.Z - vecSpaceBot.Z;
	findPos = FVector(headSocketPos.X, headSocketPos.Y, headSocketPos.Z + offsetZ);

	return headSocketPos;
}

FName FGsStageLobbyCharacterSelect::GetCurrentPosTag()
{
	return GData()->GetGlobalData()->Lobby_Pos_CLobbyCharacter;
}