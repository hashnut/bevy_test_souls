// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Management/GsScopeHolder.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameObject/Define/EGsPartsType.h"
#include "GsLobbyCharacterPresetManager.generated.h"

/**
 * 
 */
class AGsCharacterPlayer;
class AGsActorCreatePreset;
struct FGsSchemaCharacterCreateData;
struct FGsSchemaCharacterCreatePresetData;
class AGsActorCreateChManager;

UCLASS()
class T1PROJECT_API UGsLobbyCharacterPresetManager : 
	public UObject, 
	public IGsManager
{
	GENERATED_BODY()

protected:
	AGsActorCreateChManager* _actorCreateChManager = nullptr;
	AGsActorCreatePreset* _actorCreatePreset1;
	AGsActorCreatePreset* _actorCreatePreset2;
	AGsActorCreatePreset* _actorCreatePreset3;
	AGsActorCreatePreset* _actorCreatePreset4;

	// 캐릭터 생성, 커스터마이징에 사용하는 캐릭터
	UPROPERTY()
	class AGsCharacterPlayer* _character = nullptr;


	CreatureGenderType _currGender = CreatureGenderType::MAX;
	CreatureWeaponType _currWeapon = CreatureWeaponType::MAX;

	const int32 INVALID_ID = -1;
	int _selectIndex = 0;

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

public:
	uint32 GetPartsID(const EGsPartsType InType, uint32 InItemId);

	const struct FGsSchemaCharacterCreateData* GetCharacterCreateData(CreatureGenderType InGenderType) const;
	const struct FGsSchemaCharacterCreatePresetData* GetCharacterCreatePresetData(int inIndex) const;

public:
	void LoadCharacters();
	void UpdateCharacter(int inindex, AGsActorCreatePreset* inActorCreatePreset);
	void SetCharacterSelect(int inindex, bool inIsSelected);
	void OnStopSequencePlayer();
	void SetCharacterUnSelect();
	void OnStartRoamingLuna();
	int GetSelectIndex() { return _selectIndex; }
	CreatureGenderType GetGenderType() const;
};

#define LSLobbyCharacterPreset() UGsScopeHolder::GetLobbyManager<UGsLobbyCharacterPresetManager>(UGsScopeLobby::EManagerType::LobbyCharacterPreset)