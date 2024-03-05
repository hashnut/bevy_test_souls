#include "GsStageLobbyBase.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Animation/AnimInstance.h"
#include "Engine/Classes/Animation/AnimBlueprintGeneratedClass.h"

#include "ActorEx/GsCharacterLobbyPlayer.h"

#include "Management/ScopeLobby/GsLobbyCharacterManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Camera/Lobby/GsCameraHandlerLobby.h"

#include "Animation/GsLobbyAnimInstanceState.h"

#include "DataSchema/Camera/GsSchemaCustomizeCameraData.h"
#include "DataSchema/Camera/GsSchemaContentsCameraData.h"
#include "Data/GsDataContainManager.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsGameObjectUtil.h"

void FGsStageLobbyBase::Exit()
{
	_character = nullptr;
}

void FGsStageLobbyBase::InitializeCamera(EGsCameraLobbyState In_StateType, EGsContentsCameraType In_camreaContentsType, float In_zoomRate)
{
	if (nullptr == _character)
	{
		return;
	}

	LSLobbyCharacter()->SetCustomizeCameraData(UGsTableUtil::FindRowById<UGsTableCustomizeCameraData, FGsSchemaCustomizeCameraData>(
		In_StateType));


	LSLobbyCharacter()->SetCameraContentsData(UGsTableUtil::FindRowById<UGsTableContentsCameraData, FGsSchemaContentsCameraData>(
		In_camreaContentsType));


	_character->InitState(In_zoomRate);	

	// 전방 바라보게 카메라 회전
	// 역방향
	// spawn rotaion base
	FTransform spawnTransform = FindCurrentStageSpawnTransform();

	FQuat quatRotation = spawnTransform.GetRotation();

	FVector vecRotation = quatRotation.Vector();


	FVector oppVec =  vecRotation * -1.0f;
	APlayerController* pc = CastChecked<APlayerController>(_character->Controller);

	if (pc == nullptr)
	{
		return;
	}
	UGsLobbyCharacterManager* characterMgr = LSLobbyCharacter();
	if (characterMgr == nullptr)
	{
		return;
	}

	const FGsSchemaCustomizeCameraData* currentData = characterMgr->GetCurrentCustomizeCameraData();
	if (nullptr == currentData)
	{
		return;
	}

	FRotator oppRot = oppVec.Rotation();
	oppRot.Pitch = currentData->InitPitchRotation;
	pc->SetControlRotation(oppRot);

	// init rotation
	_character->SetActorRotation(quatRotation);
}

ACharacter* FGsStageLobbyBase::SpawnActor(CreatureGenderType InGenderType)
{
	// 스폰 위치 반영
	FTransform spawnTm = FindCurrentStageSpawnTransform();
	return LSLobbyCharacter()->CreateCharacter(InGenderType, spawnTm);
}

FTransform FGsStageLobbyBase::FindCurrentStageSpawnTransform()
{
	TArray<AActor*>outActors;
	UGameplayStatics::GetAllActorsOfClass(LSLobbyCharacter()->GetWorld(), AActor::StaticClass(), outActors);

	FTransform spawnTm;
	FName posTag = GetCurrentPosTag();
	for (AActor* actor : outActors)
	{
		if (actor->ActorHasTag(posTag))
		{
			spawnTm = actor->GetTransform();
			break;
		}
	}

	return spawnTm;
}

void FGsStageLobbyBase::InitializeLookCameraNode(FGsStageMode::Lobby In_mode, AGsCharacterLobbyPlayer* In_char)
{
	if (In_char == nullptr)
	{
		return;
	}

	USkeletalMeshComponent* skeletalComponent = In_char->GetMesh();
	if (nullptr == skeletalComponent)
	{
		return;
	}
	UAnimInstance* animInstance = skeletalComponent->GetAnimInstance();
	if (animInstance == nullptr)
	{
		return;
	}
	UGsLobbyAnimInstanceState* lobbyAnimInstance = Cast<UGsLobbyAnimInstanceState>(animInstance);
	if (nullptr == lobbyAnimInstance)
	{
		return;
	}
	FAnimInstanceProxy* animInstanceProxy = lobbyAnimInstance->GetAnimInstanceProxy();
	if (nullptr == animInstanceProxy)
	{
		return;
	}
	FGsLobbyAnimInstanceStateProxy* lobbyAnimInstanceProxy = static_cast<FGsLobbyAnimInstanceStateProxy*>(animInstanceProxy);
	if (nullptr == lobbyAnimInstanceProxy)
	{
		return;
	}

	lobbyAnimInstanceProxy->_lobbyChar = In_char;
	lobbyAnimInstanceProxy->_currentStageMode = In_mode;
}