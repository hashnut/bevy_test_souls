#include "GsActorCreatePreset.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "GameObject/Customize/GsCustomizeHandler.h"
#include "GameObject/Customize/GsCustomizeFunc.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Summon/GsSchemaSummonCostumeGradeRes.h"
#include "Lobby/GsSchemaCharacterCreatePresetData.h"
#include "Customize/GsSchemaCustomizeMenu.h"
#include "Customize/GsSchemaCustomizePreset.h"

#include "UTIL/GsTableUtil.h"

#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Components/SceneComponent.h"
#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"

#include "UI/UIContent/Window/Customize/GsCustomizeUIData.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

const FName PRESET_01 = TEXT("1");
const FName PRESET_02 = TEXT("2");
const FName PRESET_03 = TEXT("3");
const FName PRESET_04 = TEXT("4");


AGsActorCreatePreset::AGsActorCreatePreset()
	:Super()
{
	bCanBeInCluster = false;
}

void AGsActorCreatePreset::BeginPlay()
{
	if (USkeletalMeshComponent* skeletalComponent = GetMesh())
	{
		_vecOriginSkeletalLoc = skeletalComponent->GetRelativeLocation();
		
		if (UGsDataContainManager* dataContainManager = GData())
		{
			if (UGsGlobalConstant* globalConstant = dataContainManager->GetGlobalData())
			{
				float posZ = 0;
				if (ActorHasTag(globalConstant->LobbyCharacterPreset1_Tag))
				{
					posZ = globalConstant->LobbyCharacterPreset1_PosZ;
				}
				else if (ActorHasTag(globalConstant->LobbyCharacterPreset2_Tag))
				{
					posZ = globalConstant->LobbyCharacterPreset2_PosZ;
				}
				else if (ActorHasTag(globalConstant->LobbyCharacterPreset3_Tag))
				{
					posZ = globalConstant->LobbyCharacterPreset3_PosZ;
				}
				else if (ActorHasTag(globalConstant->LobbyCharacterPreset4_Tag))
				{
					posZ = globalConstant->LobbyCharacterPreset4_PosZ;
				}

				// 메시 위치 설정
				float actorLocationZ = _vecOriginSkeletalLoc.Z + posZ;
				skeletalComponent->SetRelativeLocation(FVector(_vecOriginSkeletalLoc.X, _vecOriginSkeletalLoc.Y, actorLocationZ));
			}
		} 
	}

	Super::BeginPlay();
}

void AGsActorCreatePreset::SetCustomize(const FGsSchemaCharacterCreatePresetData* characterCreatePresetData, int inIndex)
{
	if (nullptr == characterCreatePresetData)
		return;

	const FGsSchemaCustomizePreset* customizePreset = characterCreatePresetData->customizePreset.GetRow();
	if (nullptr == customizePreset)
		return;
	
	if (nullptr == _customizeHandler)
	{
		_customizeHandler = MakeShared<FGsCustomizeHandler>();
		_customizeHandler->InitializeActor(this);				
	}	

	if (nullptr == _customizeHandler)
		return;

	if (!_uiCustomizeData.IsValid())
	{
		_uiCustomizeData = MakeShareable<FGsCustomizeUIData>(new FGsCustomizeUIData);
	}

	FName menuKey = FName(*FString::FromInt(inIndex));
	if (const UGsTable* table = FGsSchemaCustomizeMenu::GetStaticTable())
	{
		const FGsSchemaCustomizeMenu* row = nullptr;
		if (table->FindRow<FGsSchemaCustomizeMenu>(menuKey, row))
		{
			_uiCustomizeData->SetMenu(row);
		}
	}	
	_customizeHandler->InitializeScaleDisplacementAllBone(characterCreatePresetData->genderType);

	_uiCustomizeData->SetValue(EGsUICustomizeType::FACE_INDEX, customizePreset->faceIndex);
	_uiCustomizeData->SetValue(EGsUICustomizeType::HAIR_INDEX, customizePreset->hairIndex);
	_uiCustomizeData->SetValue(EGsUICustomizeType::TATTOO_INDEX, customizePreset->tattooIndex);
	_uiCustomizeData->SetValue(EGsUICustomizeType::HAIR_COLOR_INDEX, customizePreset->hairColorIndex);
	_uiCustomizeData->SetValue(EGsUICustomizeType::SKIN_COLOR_INDEX, customizePreset->skinColorIndex);
	_uiCustomizeData->SetValue(EGsUICustomizeType::PUPIL_COLOR_INDEX, customizePreset->pupilColorIndex);
	_uiCustomizeData->SetValue(EGsUICustomizeType::BROW_COLOR_INDEX, customizePreset->browColorIndex);

	_uiCustomizeData->SetValue(EGsUICustomizeType::HEIGHT, customizePreset->height);
	_uiCustomizeData->SetValue(EGsUICustomizeType::HEAD_SIZE, customizePreset->headSize);
	_uiCustomizeData->SetValue(EGsUICustomizeType::CHEST_SIZE, customizePreset->chestSize);
	_uiCustomizeData->SetValue(EGsUICustomizeType::WAIST_SIZE, customizePreset->waistSize);
	_uiCustomizeData->SetValue(EGsUICustomizeType::PELVIS_SIZE, customizePreset->pelvisSize);
	_uiCustomizeData->SetValue(EGsUICustomizeType::LEG_SIZE, customizePreset->legSize);
	
	_customizeHandler->SetBodyParts(characterCreatePresetData->bodyPartsShapeId);
	_customizeHandler->SetByCustomizeUIData(_uiCustomizeData.Get());
	_customizeHandler->SetWeaponChange();
}



