// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameObjectDataRemotePlayer.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "DataSchema/User/GsSchemaUserData.h"
#include "Util/GsTableUtil.h"

#include "Item/GsItem.h"
#include "Item/GsSchemaItemEquipDetail.h"
#include "Item/GsItemManager.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GameObject/Define/GsGameObjectStruct.h"

FGsGameObjectDataRemotePlayer::FGsGameObjectDataRemotePlayer(const FGsSpawnParam* SpawnParam) :
	Super(SpawnParam)
{
	const FGsSpawnParamPlayer* castParam = static_cast<const FGsSpawnParamPlayer*>(SpawnParam);

	_userDBId = castParam->_userDBId;

	SetUserLookInfo(castParam->_lookInfo);
	SetCustomizeFaceData(castParam->_customizeFaceData);
	SetCustomizeBodyData(castParam->_customizeBodyData);
	SetUserStatus(castParam->_userStatus);
	SetName(castParam->_name);

	_accountGrade = castParam->_accountGrade;
	_homeWorldId = castParam->_homeWorldId;
	_gmPrefix = castParam->_gmPrefix;

	_guildData.Set(castParam->_guildData);

	_titleData.CleanUp();
	_invadeBattleLevel = 0;

	if (const FGsSchemaUserData* userTable = UGsTableUtil::FindRow<FGsSchemaUserData>(GetTableId()))
	{
		SetCreatureGenderType(userTable->genderType);
	}

	_isShowAggro = false;
	_isShowContribution = false;
}

void FGsGameObjectDataRemotePlayer::SetCustomizeFaceData(const CustomizeFaceData& data)
{
	_faceData.mFaceId			= data.mFaceId;
	_faceData.mHairId			= data.mHairId;
	_faceData.mTattooId			= data.mTattooId;
	_faceData.mHairColorIndex	= data.mHairColorIndex;
	_faceData.mSkinColorIndex	= data.mSkinColorIndex;
	_faceData.mBrowColorIndex	= data.mBrowColorIndex;
	_faceData.mPupilColorIndex	= data.mPupilColorIndex;
	_faceData.mCheekSize		= data.mCheekSize;
	_faceData.mChinLength		= data.mChinLength;
	_faceData.mChinSize			= data.mCheekSize;
	_faceData.mNosePosition		= data.mNosePosition;
	_faceData.mEyePosition		= data.mEyePosition;
	_faceData.mEyeSpacing		= data.mEyeSpacing;
	_faceData.mEyeSize			= data.mEyeSize;
	_faceData.mBrowPosition		= data.mBrowPosition;
	_faceData.mBrowAngle		= data.mBrowAngle;
	_faceData.mBrowLength		= data.mBrowLength;
	_faceData.mMouthPosition	= data.mMouthPosition;
	_faceData.mMouthSize		= data.mMouthSize;
}

void FGsGameObjectDataRemotePlayer::SetCustomizeBodyData(const CustomizeBodyData& data)
{
	/*_bodyData.mBodyId			= data.mBodyId;*/
	_bodyData.mSkinColorIndex	= data.mSkinColorIndex;
	_bodyData.mHeight			= data.mHeight;
	_bodyData.mHeadSize			= data.mHeadSize;
	_bodyData.mChestSize		= data.mChestSize;
	_bodyData.mWaistSize		= data.mWaistSize;
	_bodyData.mPelvisSize		= data.mPelvisSize;
	_bodyData.mLegSize			= data.mLegSize;
}

void FGsGameObjectDataRemotePlayer::SetUserLookInfo(PD::SC::PKT_SC_USER_LOOK_INFO_CHANGED_READ& Packet)
{
	_userLookInfo = Packet.LookInfo();
}

void FGsGameObjectDataRemotePlayer::SetUserLookInfo(const UserLookInfo& data)
{
	_userLookInfo.mWeapon = data.mWeapon;
	/*_userLookInfo.mArmor = data.mArmor;*/
	_userLookInfo.mFairy = data.mFairy;
	_userLookInfo.mCostumeId = data.mCostumeId;
	_userLookInfo.mViewHelmet = data.mViewHelmet;
	_userLookInfo.mPVPAlignmentGrade = data.mPVPAlignmentGrade;
	_userLookInfo.mWeaponLevel = data.mWeaponLevel;
}

void FGsGameObjectDataRemotePlayer::SetCreatureGenderType(CreatureGenderType inGenderType)
{
	_creatureGenderType = inGenderType;
}

void FGsGameObjectDataRemotePlayer::SetGuildEmblemId(GuildEmblemId InEmblemId)
{
	_guildData._emblemId = InEmblemId;
}

void FGsGameObjectDataRemotePlayer::SetClearGuildData()
{
	_guildData.Empty();
}

void FGsGameObjectDataRemotePlayer::SetTitleData(const FGsTitlePlayerData::TitleSet& InFrontData, const FGsTitlePlayerData::TitleSet& InBackData)
{
	_titleData.Set(InFrontData, InBackData);
}

void FGsGameObjectDataRemotePlayer::SetInvadeBattleLevel(InvadeBattleLevel InLevel)
{
	_invadeBattleLevel = InLevel;
}

CreatureWeaponType FGsGameObjectDataRemotePlayer::GetCurrentWeaponType() const
{
	if (_userLookInfo.mWeapon > 0)
	{
		if (const FGsSchemaItemEquipDetail* equipTable = 
			UGsItemManager::GetEquipItemTableDataByTID(_userLookInfo.mWeapon))
		{
			return equipTable->weaponType;
		}
	}
	return CreatureWeaponType::HAND;
}
ItemGrade		   FGsGameObjectDataRemotePlayer::GetCurrentWeaponGrade() const
{
	if (_userLookInfo.mWeapon > 0)
	{
		if (const FGsSchemaItemCommon* itemTable =
			FGsItem::GetTableDataByID(_userLookInfo.mWeapon))
		{
			return itemTable->grade;
		}

	}
	return ItemGrade::NONE;
}

void FGsGameObjectDataRemotePlayer::SetGuildData(PD::SC::PKT_SC_NOTIFY_USER_GUILD_INFO_READ& InPacket)
{
	_guildData.Set(InPacket);
}

void FGsGameObjectDataRemotePlayer::CheckAndSetGuildDiplomacy()
{
	_guildData.CheckAndSetGuildDiplomacy();
}

const FGsTitlePlayerData* FGsGameObjectDataRemotePlayer::GetTitleData() const
{
	return &(_titleData);
}

InvadeBattleLevel FGsGameObjectDataRemotePlayer::GetInvadeBattleLevel() const
{
	return _invadeBattleLevel;
}