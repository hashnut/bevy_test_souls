// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameObjectDataLocalPlayer.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Item/GsItem.h"
#include "Item/GsItemManager.h"

#include "DataSchema/Item/GsSchemaItemEquipDetail.h"
#include "GameObject/Define/GsGameObjectStruct.h"

FGsGameObjectDataLocalPlayer::FGsGameObjectDataLocalPlayer(const FGsSpawnParam* SpawnParam) :
	Super(SpawnParam)
{
	_userBasicInfo = static_cast<const FGsSpawnParamLocalPlayer*>(SpawnParam)->_netUserData;
	_isShowAggro = false;
	_isShowContribution = false;
	if (nullptr != _userBasicInfo)
	{
		_tId = _userBasicInfo->mTid;
	}
}

void FGsGameObjectDataLocalPlayer::Initialize()
{
	Super::Initialize();
}

void FGsGameObjectDataLocalPlayer::Finalize()
{
	Super::Finalize();
}

void FGsGameObjectDataLocalPlayer::SetUserLookInfo(PD::SC::PKT_SC_USER_LOOK_INFO_CHANGED_READ& Packet)
{
	// 무기가 바뀌면 천마석(정령탄) 상태 갱신(소모 갯수 변경에 따른 사용가능 상태)
	bool isChangedWeapon = false;
	if (_userBasicInfo &&		
		Packet.LookInfo().mWeapon != _userBasicInfo->mLookInfo.mWeapon)
	{
		isChangedWeapon = true;
	}

	GGameData()->Set(Packet);

	Super::SetUserLookInfo(Packet);
}

void FGsGameObjectDataLocalPlayer::SetUserStatus(UserStatus inStatus)
{
	GGameData()->Set(inStatus);

	Super::SetUserStatus(inStatus);
}

void FGsGameObjectDataLocalPlayer::SetTerritoryType(TerritoryType inTerritoryType)
{
	GGameData()->SetTerritoryType(inTerritoryType);

	Super::SetTerritoryType(inTerritoryType);
}

void FGsGameObjectDataLocalPlayer::SetLevel(uint16 InLevel)
{
	GGameData()->SetLevel(InLevel);

	Super::SetLevel(InLevel);
}

void FGsGameObjectDataLocalPlayer::SetName(const FString& InName)
{
	// 이부분이 호출된다면 확인 필요
	// 정말 필요하다면 FGsNetUserData 에서 구성한다.
}

void FGsGameObjectDataLocalPlayer::SetGuildEmblemId(GuildEmblemId InEmblemId)
{
	GGameData()->SetChangeGuildEmblem(InEmblemId);
}

void FGsGameObjectDataLocalPlayer::SetClearGuildData()
{
	GGameData()->ClearGuildData();
}

void FGsGameObjectDataLocalPlayer::SetTitleData(const FGsTitlePlayerData::TitleSet& InFrontData, const FGsTitlePlayerData::TitleSet& InBackData)
{
	GGameData()->SetTitleData(InFrontData, InBackData);
}

void FGsGameObjectDataLocalPlayer::SetInvadeBattleLevel(InvadeBattleLevel InLevel)
{
	GGameData()->SetInvadeBattleLevel(InLevel);
}

uint64 FGsGameObjectDataLocalPlayer::GetUserDBID() const
{
	if (_userBasicInfo)
	{
		return _userBasicInfo->mUserDBId;
	}
	return 0;
}

uint16 FGsGameObjectDataLocalPlayer::GetLevel() const
{
	if (_userBasicInfo)
	{
		return _userBasicInfo->mLevel;
	}
	return Super::GetLevel();
}

FString FGsGameObjectDataLocalPlayer::GetName() const
{
	if (_userBasicInfo)
	{
		return _userBasicInfo->mUserName;
	}
	return Super::GetName();
}

uint64 FGsGameObjectDataLocalPlayer::GetUserExp() const
{
	if (_userBasicInfo)
	{
		return _userBasicInfo->mExp;
	}
	return 0;
}
const TArray<FGsNetUserData::FSkillSlot>* FGsGameObjectDataLocalPlayer::GetUserSkillList() const
{
	if (_userBasicInfo)
	{
		return &_userBasicInfo->listSkillSlot;
	}
	return nullptr;
}

const FGsNetUserData::FSkillSlot* FGsGameObjectDataLocalPlayer::GetUserSkillInfo(int32 SkillId) const
{
	if (_userBasicInfo)
	{
		return _userBasicInfo->listSkillSlot.FindByPredicate([SkillId](const FGsNetUserData::FSkillSlot& e)
		{
			return e.mSkillId == SkillId;
		});
	}
	return nullptr;
}

const UserLookInfo* FGsGameObjectDataLocalPlayer::GetUserLookInfo() const
{
	if (_userBasicInfo)
	{
		return &_userBasicInfo->mLookInfo;
	}
	return nullptr;
}

const CustomizeFaceData* FGsGameObjectDataLocalPlayer::GetCustomizeFaceData() const
{
	if (_userBasicInfo)
	{
		return &_userBasicInfo->mFaceData;
	}
	return nullptr;
}

const CustomizeBodyData* FGsGameObjectDataLocalPlayer::GetCustomizeBodyData() const
{
	if (_userBasicInfo)
	{
		return &_userBasicInfo->mBodyData;
	}
	return nullptr;
}

UserStatus FGsGameObjectDataLocalPlayer::GetUserStatus() const
{
	if (_userBasicInfo)
	{
		return _userBasicInfo->mUserStatus;
	}
	return UserStatus::USER_STATE_NORMAL;
}

TerritoryType FGsGameObjectDataLocalPlayer::GetTerritoryType() const
{
	if (GGameData())
	{
		return GGameData()->GetTerritoryType();
	}
	return TerritoryType::NONE;
}

CreatureGenderType FGsGameObjectDataLocalPlayer::GetCreatureGenderType() const
{
	if (_userBasicInfo)
	{
		return _userBasicInfo->mGender;
	}
	return CreatureGenderType::MAX;
}

CreatureWeaponType FGsGameObjectDataLocalPlayer::GetCurrentWeaponType() const
{
	if (_userBasicInfo)
	{
		if (_userBasicInfo->mLookInfo.mWeapon == 0)
		{
			return CreatureWeaponType::HAND;
		}

		if (const FGsSchemaItemEquipDetail* equipTable = 
			UGsItemManager::GetEquipItemTableDataByTID(_userBasicInfo->mLookInfo.mWeapon))
		{
			return equipTable->weaponType;
		}
	}
	return CreatureWeaponType::NONE;
}
ItemGrade		   FGsGameObjectDataLocalPlayer::GetCurrentWeaponGrade() const
{
	if (_userBasicInfo)
	{
		if (_userBasicInfo->mLookInfo.mWeapon == 0)
		{
			return ItemGrade::NORMAL;
		}

		if (const FGsSchemaItemCommon* itemTable =
			FGsItem::GetTableDataByID(_userBasicInfo->mLookInfo.mWeapon))
		{
			return itemTable->grade;
		}
	}
	return ItemGrade::NONE;
}

const FGsGuildPlayerData* FGsGameObjectDataLocalPlayer::GetGuildData() const
{
	if (_userBasicInfo)
	{
		return &_userBasicInfo->mGuildData;
	}

	return nullptr;
}

bool FGsGameObjectDataLocalPlayer::IsMyGuild() const
{
	// 내 정보이므로 길드가 있으면 true
	if (_userBasicInfo)
	{
		if (INVALID_GUILD_DB_ID != _userBasicInfo->mGuildData._guildDBId)
		{
			return true;
		}
	}

	return false;
}

AccountGrade FGsGameObjectDataLocalPlayer::GetAccountGrade() const
{
	if (_userBasicInfo)
	{
		return _userBasicInfo->mAccountGrade;
	}

	return AccountGrade::NORMAL;
}

WorldId FGsGameObjectDataLocalPlayer::GetHomeWorldId() const
{
	if (_userBasicInfo)
	{
		return _userBasicInfo->mHomeWorldId;
	}

	return 0;
}

FString FGsGameObjectDataLocalPlayer::GetGMPrefix() const
{
	if (_userBasicInfo)
	{
		return _userBasicInfo->mGMPrefix;
	}

	return TEXT("");
}

const FGsTitlePlayerData* FGsGameObjectDataLocalPlayer::GetTitleData() const
{
	if (_userBasicInfo)
	{
		return &(_userBasicInfo->_titleData);
	}

	return nullptr;
}

InvadeBattleLevel FGsGameObjectDataLocalPlayer::GetInvadeBattleLevel() const
{
	if (_userBasicInfo)
	{
		return _userBasicInfo->_InvadeBattleLevel;
	}

	return -1;
}