// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Management/GsScopeHolder.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameObject/Define/EGsPartsType.h"
#include "GsLobbyCharacterManager.generated.h"

/**
 * 로비 캐릭터 모델 관리에 사용.
 */
class AGsCharacterPlayer;
class UGsGameObjectPlayableEventLocalPlayer;

struct FGsSchemaCustomizeCameraData;
struct FGsSchemaContentsCameraData;

UCLASS()
class T1PROJECT_API UGsLobbyCharacterManager : 
	public UObject, 
	public IGsManager
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	AGsCharacterPlayer* _character[(int)CreatureGenderType::MAX] = { nullptr, };

	const FGsSchemaCustomizeCameraData* _currentCamData;
	const FGsSchemaContentsCameraData* _currentCamContentsData;

	// 최근 머리 본 위치
	FVector _currentHeadBonePos;
	// 플레이 이벤트 로컬
	UPROPERTY(Transient, VisibleInstanceOnly, Meta = (AllowPrivateAccess = true))
	UGsGameObjectPlayableEventLocalPlayer* _playableEventLocal;

public:
	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual void Update(float DeltaTime) override;

public:
	// 로비 캐릭터 스폰, 디스폰
	class AGsCharacterPlayer* CreateCharacter(CreatureGenderType InGender, FTransform& InSpawnTm);
	void RemoveAll();
	void RemoveCharacter();

	// 플레이어블 이벤트 로컬 스폰
	void SpawnPlayableEventLocal();
	// 플레이어블 이벤트 로컬 디스폰
	void DespawnPlayableEventLocal();
	
	// 외부 사용 데이터 설정
	void SetCustomizeCameraData(const FGsSchemaCustomizeCameraData* InData)
	{
		_currentCamData = InData;
	}
	const FGsSchemaCustomizeCameraData* GetCurrentCustomizeCameraData()
	{
		return _currentCamData;
	}

	void SetCameraContentsData(const FGsSchemaContentsCameraData* InData)
	{
		_currentCamContentsData = InData;
	}
	const FGsSchemaContentsCameraData* GetCurrentCameraContentsData()
	{
		return _currentCamContentsData;
	}

	// 현재 아무 역활을 하고있지 않음
	FVector GetCurrentHeadBonePos()
	{
		return _currentHeadBonePos;
	}

	UGsGameObjectPlayableEventLocalPlayer* GetPlayableEventLocalPlayer()
	{
		return _playableEventLocal;
	}
public:
	uint32 GetPartsID(const EGsPartsType InType, uint32 InItemId);	

protected:
	// 액터 BP 클래스 정보 얻기
	const struct FGsSchemaCharacterCreateData* GetCharacterCreateData(CreatureGenderType InGenderType) const;
	FString GetCharacterClassPath(CreatureGenderType InGenderType) const;
};

#define LSLobbyCharacter() UGsScopeHolder::GetLobbyManager<UGsLobbyCharacterManager>(UGsScopeLobby::EManagerType::LobbyCharacter)