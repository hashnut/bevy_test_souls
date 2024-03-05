// Fill out your copyright notice in the Description page of Project Settings.


#include "GsLobbyCharacterPresetManager.h"
#include "GsLobbyCharacterManager.h"
#include "GameFramework/PlayerController.h"
#include "Classes/Camera/PlayerCameraManager.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Classes/Components/SkeletalMeshComponent.h"
#include "DataSchema/GsSchemaCharacterCreateData.h"

#include "GameObject/Define/EGsPartsType.h"

#include "Item/GsItemManager.h"
#include "DataSchema/User/GsSchemaUserData.h"
#include "DataSchema/GameObject/GsSchemaPlayerShape.h"
#include "DataSchema/GameObject/GsSchemaPartsShapeData.h"
#include "DataSchema/Camera/GsSchemaCustomizeCameraData.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "ActorEx/GsCharacterPlayer.h"
#include "ActorEx/GsCharacterLobbyPlayer.h"
#include "ActorEx/GsWeaponActor.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorEx/GsActorCreateChManager.h"

#include "GameObject/Define/GsGameObjectStruct.h"
#include "GameObject/ObjectClass/GsGameObjectPlayableEventLocalPlayer.h"
#include "GameObject/Parts/GsPartsHandlerBase.h"
#include "GameObject/GsSchemaPartsShapeData.h"

#include "Camera/Lobby/GsCameraHandlerLobby.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"

#include "Data/GsResourceManager.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Engine/StaticMesh.h"
#include "UTIL/GsLevelUtil.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsGameObjectUtil.h"
#include "T1Project.h"

#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SceneCapture2D.h"
#include "Components/CapsuleComponent.h"

#include "Engine/Public/UnrealEngine.h"
#include "ActorEx/GsActorCreatePreset.h"
#include "Lobby/GsSchemaCharacterCreatePresetData.h"
#include "Customize/GsSchemaCustomizePreset.h"

//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
void UGsLobbyCharacterPresetManager::Initialize()
{
}

void UGsLobbyCharacterPresetManager::Finalize()
{
	_actorCreateChManager = nullptr;
}

uint32 UGsLobbyCharacterPresetManager::GetPartsID(const EGsPartsType InType, uint32 InItemId)
{
	// 아이템 정보가 있을경우
	if (0 < InItemId)
	{
		if (const FGsSchemaPartsShapeData* partsData = GItem()->FindPartsSchemaDataByTID(InItemId))
		{
			return partsData->id;
		}
	}

	return 0;
}



const FGsSchemaCharacterCreateData* UGsLobbyCharacterPresetManager::GetCharacterCreateData(CreatureGenderType InGenderType) const
{
	const UGsTableCharacterCreateData* table = Cast<UGsTableCharacterCreateData>(FGsSchemaCharacterCreateData::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaCharacterCreateData* schemaCharacterCreateData = nullptr;
	if (false == table->FindRowById(InGenderType, schemaCharacterCreateData))
	{
		return nullptr;
	}

	return schemaCharacterCreateData;
}

const FGsSchemaCharacterCreatePresetData* UGsLobbyCharacterPresetManager::GetCharacterCreatePresetData(int inIndex) const
{
	const UGsTableCharacterCreatePresetData* table = 
		Cast<UGsTableCharacterCreatePresetData>(FGsSchemaCharacterCreatePresetData::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaCharacterCreatePresetData* schemaCharacterCreatePresetData = nullptr;
	if (false == table->FindRowById(inIndex, schemaCharacterCreatePresetData))
	{
		return nullptr;
	}

	return schemaCharacterCreatePresetData;
}


void UGsLobbyCharacterPresetManager::LoadCharacters()
{
	if (nullptr == GEngine)
	{
		return;
	}

	if (nullptr == GEngine->GameViewport)
	{
		return;
	}

	UWorld* world = GEngine->GameViewport->GetWorld();
	if (nullptr == world)
	{
		return;
	}

	if (nullptr != _actorCreateChManager)
	{
		return;
	}

	if (AGsActorCreateChManager* createChManager =
		Cast<AGsActorCreateChManager>(UGameplayStatics::GetActorOfClass(world, AGsActorCreateChManager::StaticClass())))
	{
		_actorCreateChManager = createChManager;
	}

	TArray<AActor*>outActors;	
	UGameplayStatics::GetAllActorsOfClass(world, AGsActorCreatePreset::StaticClass(), outActors);

	int8 index = 0;
	for (AActor* actor : outActors)
	{
		AGsActorCreatePreset* actorCreatePreset = Cast<AGsActorCreatePreset>(actor);
		if (nullptr != actorCreatePreset)
		{			
			if (UGsDataContainManager* dataContainManager = GData())
			{
				if (UGsGlobalConstant* globalConstant = dataContainManager->GetGlobalData())
				{
					if (actorCreatePreset->ActorHasTag(globalConstant->LobbyCharacterPreset1_Tag))
					{
						UpdateCharacter(1, _actorCreatePreset1 = actorCreatePreset);
					}
					else if (actorCreatePreset->ActorHasTag(globalConstant->LobbyCharacterPreset2_Tag))
					{
						UpdateCharacter(2, _actorCreatePreset2 = actorCreatePreset);
					}
					else if (actorCreatePreset->ActorHasTag(globalConstant->LobbyCharacterPreset3_Tag))
					{
						UpdateCharacter(3, _actorCreatePreset3 = actorCreatePreset);
					}
					else if (actorCreatePreset->ActorHasTag(globalConstant->LobbyCharacterPreset4_Tag))
					{
						UpdateCharacter(4, _actorCreatePreset4 = actorCreatePreset);
					}
				}
			}
		}
	}

	_selectIndex = 0;
}

void UGsLobbyCharacterPresetManager::UpdateCharacter(int inindex, AGsActorCreatePreset* inActorCreatePreset)
{
	if (nullptr == inActorCreatePreset)
		return;

	const FGsSchemaCharacterCreatePresetData* characterCreatePresetData = GetCharacterCreatePresetData(inindex);
	if (nullptr != characterCreatePresetData)
	{
		const FGsSchemaCustomizePreset* schemaCustomizePreset = characterCreatePresetData->customizePreset.GetRow();
		if (nullptr != schemaCustomizePreset)
		{
			//bak1210 : 내부에서 수행하도록 변경
			/*inActorCreatePreset->SetBodyChange(characterCreatePresetData->bodyPartsShapeId, characterCreatePresetData->genderType);*/
			inActorCreatePreset->SetCustomize(characterCreatePresetData, inindex);					
		}			
	}
}

void UGsLobbyCharacterPresetManager::SetCharacterSelect(int inindex, bool inIsSelected)
{
	if (inIsSelected)
	{
		_selectIndex = inindex;
	}
	
	if (_actorCreateChManager)
	{
		if (inIsSelected)
		{
			_actorCreateChManager->OnSetSelectCH(inindex - 1);
		}
		else
		{
			//_actorCreateChManager->OnSetUnselectCH();
		}
	}
}

void UGsLobbyCharacterPresetManager::SetCharacterUnSelect()
{
	if (_actorCreateChManager)
	{
		_actorCreateChManager->OnSetUnselectCH();
	}
}

CreatureGenderType UGsLobbyCharacterPresetManager::GetGenderType() const
{
	const FGsSchemaCharacterCreatePresetData* characterCreatePresetData = GetCharacterCreatePresetData(_selectIndex);
	if (nullptr != characterCreatePresetData)
	{
		return characterCreatePresetData->genderType;
	}

	return CreatureGenderType::MALE;
}

void UGsLobbyCharacterPresetManager::OnStopSequencePlayer()
{
	OnStartRoamingLuna();
}

void UGsLobbyCharacterPresetManager::OnStartRoamingLuna()
{
	if (_actorCreateChManager)
	{
		_actorCreateChManager->OnStartRoamingLuna();
	}
}
