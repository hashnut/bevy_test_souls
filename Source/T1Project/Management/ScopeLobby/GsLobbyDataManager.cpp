// Fill out your copyright notice in the Description page of Project Settings.
#include "GsLobbyDataManager.h"
#include "GsLobbyDataType.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_Auth_Client.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "User/GsSchemaUserData.h"

#include "Item/GsSchemaItemEquipDetail.h"
#include "Item/GsItemManager.h"

#include "Engine/Classes/Engine/Engine.h"
#include "Engine/AssetManager.h"

#include "Lobby/GsSchemaLobbyConfig.h"
#include "Data/GsLobbyConstant.h"
#include "Kismet/GameplayStatics.h"
#include "Data/GsLobbyConstantData.h"


UGsLobbyDataManager::UGsLobbyDataManager()
{
}

UGsLobbyDataManager::~UGsLobbyDataManager()
{
}

void UGsLobbyDataManager::Initialize()
{
	ClearNewUserName();	

	if (UWorld* world = GetWorld())
	{
		TArray<AActor*>outActors;
		UGameplayStatics::GetAllActorsOfClass(world, AActor::StaticClass(), outActors);

		for (AActor* actor : outActors)
		{
			UGsLobbyConstantData* lobbyDataInfo = Cast<UGsLobbyConstantData>(actor->GetComponentByClass(UGsLobbyConstantData::StaticClass()));
			if (nullptr != lobbyDataInfo)
			{
				_lobbyConstantData = lobbyDataInfo;
			}
		}
	}

	if (_lobbyConstantData)
	{
		if(UObject* objectLobbyConstant = UAssetManager::GetStreamableManager().LoadSynchronous(_lobbyConstantData->_pathLobbyConstant))
		{
			if (const UGsLobbyConstant* lobbyConstant = Cast<UGsLobbyConstant>(objectLobbyConstant))
			{
				_lobbyConstant = lobbyConstant;
			}
		}

		if (UObject* objectLocalizationConstant = UAssetManager::GetStreamableManager().LoadSynchronous(_lobbyConstantData->_pathLocalizationConstant))
		{
			if (const UGsLocalizationConstant* localizationConstant = Cast<UGsLocalizationConstant>(objectLocalizationConstant))
			{
				_lobbyLocalizationConstant = localizationConstant;
			}
		}
	}
}

void UGsLobbyDataManager::Finalize()
{
}

void UGsLobbyDataManager::SortingUserList(int inStartIndex, int inUserCount)
{
	for (int i = inStartIndex; i < inUserCount - 1; ++i)
	{
		for (int j = i + 1; j < inUserCount; ++j)
		{
			if (_lobbyUserListData[i]._lastLoginTime < _lobbyUserListData[j]._lastLoginTime)
			{
				_lobbyUserListData.Swap(j, i);
			}
		}
	}
}

bool UGsLobbyDataManager::SwapNewUserFirstList(int inUserCount)
{
	for (int i = 0; i < inUserCount; ++i)
	{
		if (_newUserName == _lobbyUserListData[i]._userName)
		{
			_lobbyUserListData.Swap(i, 0);
			return true;
		}
	}

	return false;
}

bool UGsLobbyDataManager::Set(PD::GC::PKT_GC_ACK_USER_LIST_READ& Packet)
{
	_lobbyUserListData.Empty();
	
	int packetUserNum = Packet.GetUserListCount();
	if (true == _isDeleteUser)
	{
		_isDeleteUser = false;
		if (0 >= packetUserNum)
			return true;
	}
	
	// 내 캐릭터가 없다.
	_isCharacterEmpty = false;
	if (0 >= packetUserNum)
	{
		_isCharacterEmpty = true;
		return false;
	}

	const UGsTable* table = FGsSchemaUserData::GetStaticTable();
	if (nullptr == table)
		return false;

	TArray<const FGsSchemaUserData*> allRow;
	if (false == table->GetAllRows(allRow))
		return false;

	// TEST
	int32 loopCount = 0;

	for (
		PD::GC::PKT_GC_ACK_USER_LIST_READ::UserListIterator iterator = Packet.GetFirstUserListIterator();
		iterator != Packet.GetLastUserListIterator();
		++iterator
		)
	{
		CreatureGenderType genderType = CreatureGenderType::MALE;
		CreatureId creatureId = iterator->CreatureId();
		for (const FGsSchemaUserData* data : allRow)
		{
			if (data->commonInfoId != creatureId)
				continue;

			genderType = data->genderType;
			break;			
		}		

		CreatureWeaponType weaponType = CreatureWeaponType::HAND;
		if (const FGsSchemaItemEquipDetail* equipTable = UGsItemManager::GetEquipItemTableDataByTID(iterator->ConstRefLookInfo().mWeapon))
		{
			weaponType = equipTable->weaponType;
		}		

		_lobbyUserListData.Emplace(
			iterator->UserDBId(), creatureId, genderType, weaponType, iterator->UserLevel(),
			iterator->UserName(), iterator->LastLoginTime(), iterator->MapId(),
			iterator->ConstRefLookInfo(), iterator->ConstRefCustomizingFaceData(), 
			iterator->ConstRefCustomizingBodyData(),
			iterator->LookInfo().mWeapon, iterator->LookInfo().mWeaponLevel,
			iterator->ExpiringTime(), iterator->CreatedTime(), iterator->GuildName(),
			iterator->IsServerMigrateUserNameChanged(),
			iterator->IsServerMigrateGuildNameChanged()
		);

		//GSLOG(Warning, TEXT("UGsLobbyDataManager::Set - FaceID : %d"), iterator->ConstRefCustomizingFaceData().mFaceId);
		//GSLOG(Warning, TEXT("UGsLobbyDataManager::Set - FaceID : %d"), iterator->ConstRefCustomizingFaceData().mFaceId);
		//GSLOG(Warning, TEXT("UGsLobbyDataManager::Set - FaceID : %d"), iterator->ConstRefCustomizingFaceData().mFaceId);

		++loopCount;
	}

	_characterFreeSlotCount = Packet.CharacterFreeSlotCount(); // 기본 소유 캐릭터 슬롯 개수
	_characterPaidSlotCount = Packet.CharacterPaidSlotCount(); // 캐릭터 슬롯을 유료로 확장한 개수.
	_customizingTicketCount = Packet.CustomizingTicketCount(); // 캐릭터 슬롯을 유료로 확장한 개수.

	int32 pktCount = Packet.GetUserListCount();
	if (pktCount != loopCount)
	{
		const FString locString = FString::Printf(TEXT("[USER_TEST] Error: pktCount: %d, loopCount: %d"),
			pktCount, loopCount);

		FVector2D textScale{ 2.f, 2.f };
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, locString, true, textScale);
	}

	int8 listNum = _lobbyUserListData.Num();
	bool isNewUser = false;
	if (0 < listNum)
	{	
		SortingUserList((true == SwapNewUserFirstList(listNum)) ? 1 : 0, listNum);
		ClearNewUserName();
	}

	return true;
}


const FGsSchemaLobbyConfig* UGsLobbyDataManager::GetLobbyConfig()
{
	if (nullptr != _lobbyConfig)
	{
		return _lobbyConfig;
	}

	// 1개 열밖에 없음.
	const FGsSchemaLobbyConfig* row = nullptr;
	if (const UGsTable* table = FGsSchemaLobbyConfig::GetStaticTable())
	{
		if (table->FindRow(TEXT("0"), row))
		{
			return _lobbyConfig = row;
		}
	}

#if WITH_EDITOR
	GSLOG(Error, TEXT("[Lobby] Fail to get FGsSchemaLobbyConfig."));
#endif
	return nullptr;
}


bool UGsLobbyDataManager::GetIsBackToCharacterSelect()
{
	bool returnValue = _isBackToCharacterSelect;
	_isBackToCharacterSelect = false;

	return returnValue;
}