#pragma once

#include "../GsStageMode.h"
#include "../../../Classes/GsState.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Camera/Define/EGsCameraLobbyState.h"

class USkeletalMeshComponent;

class AGsCharacterLobbyPlayer;
class ACharacter;

const FName LobbyCamHeadBoneName = TEXT("Bip001-Head");

//------------------------------------------------------------------------------
// 로비 스테이지를 관리할때 필요한 메모리 할당관리자(상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
class FGsStageLobbyBase : public TGsState<FGsStageMode::Lobby>
{
public:
	FGsStageLobbyBase() : TGsState<FGsStageMode::Lobby>(FGsStageMode::Lobby::LOBBY_MAX) {}
	explicit FGsStageLobbyBase(FGsStageMode::Lobby inMode) : TGsState<FGsStageMode::Lobby>(inMode) {}
	virtual ~FGsStageLobbyBase() = default;


protected:
	class AGsCharacterLobbyPlayer* _character = nullptr;
	// const struct FGsSchemaCustomizeCameraData* _currentCamData = nullptr;

public:
	virtual void Enter() override {};
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override {};

public:
	virtual class AGsCharacterLobbyPlayer* SetCharacter(CreatureGenderType InGenderType)
	{
		return nullptr;
	}
	virtual void ShowCharacter(bool bInShow) {};
	class AGsCharacterLobbyPlayer* GetCurrentLobbyPlayer() const { return _character; }	

protected:
	virtual ACharacter* SpawnActor(CreatureGenderType InGenderType);

public:
	virtual void OnReconectionStart() {};
	virtual void OnReconectionEnd() {};

protected:
	virtual void InitializeCamera(EGsCameraLobbyState In_StateType, EGsContentsCameraType In_camreaContentsType,float In_zoomRate = 1.f);
	
public:
	void InitializeLookCameraNode(FGsStageMode::Lobby In_mode, AGsCharacterLobbyPlayer* In_char);
	//get
public:
	AGsCharacterLobbyPlayer* GetCharacter() const { return _character; }
	virtual FVector GetCurrentHeadPos(USkeletalMeshComponent* In_skeletalMeshComponent)
	{
		return FVector::ZeroVector;
	};

	virtual FName GetCurrentPosTag() { return NAME_None; };


public:
	FTransform FindCurrentStageSpawnTransform();
};
