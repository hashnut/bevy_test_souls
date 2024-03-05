// Fill out your copyright notice in the Description page of Project Settings.


#include "GsPartsHandlerPlayer.h"

#include "Engine/Public/SkeletalMeshMerge.h"
#include "Engine/Classes/Components/MeshComponent.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "T1Project.h"

#include "ActorEx/GsCharacterPlayer.h"

#include "Util/GsTableUtil.h"

#include "DataSchema/User/GsSchemaUserData.h"
#include "DataSchema/GameObject/GsSchemaPartsShapeData.h"
#include "DataSchema/GameObject/GsSchemaPlayerShape.h"

#include "GameObject/Define/EGsPartsType.h"
#include "GameObject/ObjectClass/GsGameObjectPlayer.h"
#include "GameObject/Data/GsGameObjectDataPlayer.h"
#include "GameObject/Customize/GsCustomizeHandler.h"

#include "Item/GsItemManager.h"
#include "Item/GsItemEquipment.h"

#include "Management/GsMessageHolder.h"
#include "Message/GsMessageGameObject.h"

void FGsPartsHandlerPlayer::Initialize(UGsGameObjectBase* Owner)
{
	Super::Initialize(Owner);
}

const FGsSchemaPartsShapeData* FGsPartsHandlerPlayer::GetDefaultPartsModel(EGsPartsType InType) const
{
	// 디폴트 파츠 정보 얻기
	UGsGameObjectPlayer* player = _owner->CastGameObject<UGsGameObjectPlayer>();
	const FGsSchemaUserData* pcTable = player->GetPcTable();
	if (const FGsSchemaPlayerShape* shapeData = pcTable->shapeId.GetRow())
	{
		if (const FGsSchemaPartsShapeDataRow* findData = shapeData->defaultParts.Find(InType))
		{
			return findData->GetRow();
		}
	}
	return nullptr;
}

uint32 FGsPartsHandlerPlayer::GetDefaultPartsModelIndex(EGsPartsType InType) const
{
	if (const FGsSchemaPartsShapeData* defaultParts = GetDefaultPartsModel(InType))
	{
		return defaultParts->id;
	}

	return 0;
}

void FGsPartsHandlerPlayer::RestoreAllParts()
{
	FGsCustomizeHandler* handler = _owner->GetCustomizeHandler();
	if (nullptr == handler)
		return;

	for (uint8 i = (uint8)EGsPartsType::START; i < (uint8)EGsPartsType::MAX; ++i)
	{
		AttachParts(handler, nullptr, (EGsPartsType)i);
	}

}

void FGsPartsHandlerPlayer::Finalize()
{
	Super::Finalize();
}

void FGsPartsHandlerPlayer::InitializeActor(class AActor* Owner)
{
	UpdatePartsLayerData();
}

void FGsPartsHandlerPlayer::InitPartsLayer(UGsGameObjectBase* Owner)
{
	if (AGsCharacterPlayer* actor = Cast<AGsCharacterPlayer>(Owner->GetActor()))
	{
		// Test Code
		//AddPartsLayer(EVisibleLayer::Top, actor->GetPartsStaticComponentWEAPON());
		//AddPartsLayer(EVisibleLayer::First, actor->GetPartsSkeletalComponentFACE());
		//AddPartsLayer(EVisibleLayer::First, actor->GetPartsSkeletalComponentHAIR());
		//AddPartsLayer(EVisibleLayer::First, actor->GetPartsSkeletalComponentCLOAK());
	}
}


void FGsPartsHandlerPlayer::SetPartsData(const UserLookInfo* LookInfoData)
{
	Super::SetPartsData(LookInfoData);

	AddPartsProc(*LookInfoData);

	UpdatePartsLayerData();
}

void FGsPartsHandlerPlayer::AddPartsProc(const UserLookInfo InLookInfo)
{
	for (uint8 i = (uint8)EGsPartsType::START; i < (uint8)EGsPartsType::MAX; ++i)
	{
		EGsPartsType partsType = EGsPartsType(i);
		uint32 lookInfoItemID = GetLookInfoByPartsType(partsType, InLookInfo);
		if (0 >= lookInfoItemID)
		{
			_arrPartsData[i] = nullptr;
			continue;
		}

		const FGsSchemaPartsShapeData* partsSchemaData = GItem()->FindPartsSchemaDataByTID(lookInfoItemID);
		// 찾지 못했다면 Default 데이터 설정
		if (nullptr == partsSchemaData)
		{
			partsSchemaData = GetDefaultPartsModel(partsType);
		}
		_arrPartsData[i] = partsSchemaData;
	}
}

void FGsPartsHandlerPlayer::InvalidatePartsData(const UserLookInfo* LookInfoData)
{
	Super::InvalidatePartsData(LookInfoData);

	AddPartsProc(*LookInfoData);

	UpdatePartsLayerData();

	// Send Message
	GMessage()->GetGameObject().SendMessage(MessageGameObject::UPDATE_USER_PARTS, nullptr);
}

void FGsPartsHandlerPlayer::UpdatePartsLayerData()
{
	FGsCustomizeHandler* handler = _owner->GetCustomizeHandler();
	if (nullptr == handler)
		return;

	// 파츠가 변경되기 전에 머터리얼 정보를 갱신
	AGsCharacterBase* characterActor = _owner->GetCharacter();
	if (nullptr == characterActor)
		return;

	//if (characterActor->IsMaterialChanged())
	//{
	//	characterActor->RestoreAllMaterials();
	//}

	for (uint8 i = (uint8)EGsPartsType::START; i < (uint8)EGsPartsType::MAX; ++i)
	{
		EGsPartsType partsType = (EGsPartsType)i;
		const FGsSchemaPartsShapeData* equipData = _arrPartsData[(int)partsType];
		AttachParts(handler, equipData, partsType);
	}
}

void FGsPartsHandlerPlayer::AttachParts(FGsCustomizeHandler* InHandler, const FGsSchemaPartsShapeData* InData, const EGsPartsType InPartsType)
{
	UGsGameObjectPlayer* creature = _owner->CastGameObject<UGsGameObjectPlayer>();
	if (nullptr == creature)
		return;

	FGsGameObjectDataPlayer* data = creature->GetCastData<FGsGameObjectDataPlayer>();
	if (nullptr == data)
		return;

	int32 keyID = 0;
	bool isRestoreParts = nullptr == InData ? true : false;
	if (false == isRestoreParts)
	{
		keyID = InData->id;
	}

	switch (InPartsType)
	{
	case EGsPartsType::WEAPON:
		if (isRestoreParts)
			InHandler->RestoreWeapon();
		else
		{
			CreatureWeaponType weaponType = data->GetCurrentWeaponType();			
			int32 weaponLevel = data->GetUserLookInfo()->mWeaponLevel;

			FGsWeaponLoadCompleteDelegate after;

			TWeakObjectPtr<UGsGameObjectPlayer> weakOwner(creature);
			after.AddLambda([weakOwner]()->void {
				if (false == weakOwner.IsValid())
					return;

				if (weakOwner.Get()->IsBattleMode())
					return;

				FGsGameObjectDataPlayer* data = weakOwner.Get()->GetCastData<FGsGameObjectDataPlayer>();
				if (nullptr == data)
					return;

				const FGsSchemaUserData* pcTable = weakOwner.Get()->GetPcTable();
				if (nullptr == pcTable)
					return;

				if (AGsCharacterPlayer* player = weakOwner.Get()->GetPlayerCharacter())
				{
					CreatureWeaponType currWeapon = data->GetCurrentWeaponType();

					if (const FGsSchemaWeaponSocketInfo* findInfo = pcTable->weaponSocketList.Find(currWeapon))
					{
						player->SetWeaponTransformByCurve(player->GetWeaponChildActorComponent(), &findInfo->peace, false);
						player->SetWeaponTransformByCurve(player->GetWeaponSubChildActorComponent(), &findInfo->peace, false);
					}
				}
				});
			
			InHandler->SetWeaponChange(keyID, weaponType, weaponLevel, after);
		}			
		break;
	case EGsPartsType::BODY:
		/*if (isRestoreParts)
			InHandler->RestoreBody(data->GetCustomizeBodyData());
		else
		{
			InHandler->SetBodyChange(keyID);
			InHandler->SetSkinColorChange(data->GetCustomizeBodyData()->mSkinColorIndex);
		}*/
		break;
	case EGsPartsType::HAIR:
		// hair는 userlookinfo 구조체에 없으므로, 고정임
		//InHandler->RestoreHair(data->GetCustomizeFaceData());
		break;
	case EGsPartsType::FACE:
		// InHandler는 userlookinfo 구조체에 없으므로, 고정임
		//InHandler->RestoreFace(data->GetCustomizeFaceData());
		break;
	}
}

uint32 FGsPartsHandlerPlayer::GetLookInfoByPartsType(const EGsPartsType InPartsType , const UserLookInfo InLookInfo)
{
	switch (InPartsType)
	{
	case EGsPartsType::WEAPON:
		return InLookInfo.mWeapon;
	case EGsPartsType::BODY:
		return 0;
	case EGsPartsType::HAIR:
		// hair는 userlookinfo 구조체에 없으므로, 고정임
		return 0;
	case EGsPartsType::FACE:
		// InHandler는 userlookinfo 구조체에 없으므로, 고정임
		return 0;
	}
	return 0;
}