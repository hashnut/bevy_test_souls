#include "GsUserProfileManager.h"
#include "DataSchema/UI/GsSchemaPlayerDetailInfoUIData.h"
#include "DataSchema/Social/GsSchemaSocialKeyword.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "Guild/GsGuildData.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsStatTypeNameSelector.h"
#include "Item/GsItemManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageContents.h"
#include "Net/GsNetSendService.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "UI/UIContent/Helper/GsUIHelper.h"


void FGsUserProfileManager::Initialize()
{
	Clear();
	_commentTimer.SetInterval(0.8f);
	_timerDelegate = _commentTimer.Router.AddRaw(this, &FGsUserProfileManager::ResetCommentTimer);
	_commentTimer.Stop();

	BindMessages();
}

void FGsUserProfileManager::Finalize()
{
	Clear();
	_commentTimer.Router.Remove(_timerDelegate);
	_commentTimer.Stop();

	UnbindMessages();
}

void FGsUserProfileManager::Update(float inTick)
{
	_commentTimer.Update(inTick);
}

bool FGsUserProfileManager::CheckRedDot(bool bInUpdate /*= true*/)
{
	return IsRedDot();
}

void FGsUserProfileManager::Set(PD::GC::PKT_GC_ACK_USER_PROFILE_READ& InPacket)
{
	ResetCommentTimer();

	if (IsMyProfile())
	{
		SetRedDot(false);
	}

	UserDBId prevUserDBId = INVALID_USER_DB_ID;

	if (_currentProfile.IsValid())
	{
		prevUserDBId = _currentProfile->_userDBId;
		_currentProfile->Clear();
	}
	else
	{
		_currentProfile = MakeUnique<UserProfile>();
	}

	if (prevUserDBId != InPacket.UserDBId())
	{
		// 이름 변경 이력 요청
		_nickNameChangeList.Empty();
		FGsNetSendService::SendReqUserProfileNickNameChangeList(InPacket.UserDBId());
	}
	
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;
		
	_isMyProfile = (InPacket.UserDBId() == userData->mUserDBId);
	if (IsMyProfile())
	{
		// 캐릭터 정보
		_currentProfile->_isVisible = true;
		_currentProfile->_userDBId = userData->mUserDBId;
		_currentProfile->_userName = userData->mUserName;
		_currentProfile->_userLevel = userData->mLevel;
		_currentProfile->_creatureGenderType = userData->mGender;
		
		// 기사단 정보
		const FGsGuildDataDetail* guildData = GGuild()->GetGuildDetailData(true);
		const FGsGuildDataMine* myGuildData = static_cast<const FGsGuildDataMine*>(guildData);
		if (nullptr != myGuildData && myGuildData->IsExistGuild())
		{
			_currentProfile->_isExistGuild = true;
			_currentProfile->_guildName = myGuildData->_guildName;
			_currentProfile->_guildEmblemId = myGuildData->_emblemId;
			_currentProfile->_memberGrade = myGuildData->_memberGrade;
		}
		else
		{
			_currentProfile->_isExistGuild = false;
		}
	}
	else
	{
		_currentProfile->_isVisible = InPacket.IsVisible();
		_currentProfile->_userDBId = InPacket.UserDBId();
		_currentProfile->_userName = InPacket.UserName();
		_currentProfile->_userLevel = InPacket.UserLevel();

		InPacket.MakeStatInfoListVector<TArray<StatInfo>, StatInfo>(_currentProfile->_statInfoList);

		TArray<ItemDataEquip> itemDataList;
		InPacket.MakeItemDataEquipListVector<TArray<ItemDataEquip>, ItemDataEquip>(itemDataList);

		_currentProfile->_equippedItemList.Empty();
		for (const ItemDataEquip& itemData : itemDataList)
		{
			const FGsSchemaItemCommon* tableData = FGsItem::GetTableDataByID(itemData.mTemplateId);
			if (nullptr == tableData)
				return;

			EGsEquipSlotType slotType = GItem()->GetEquipSlotType(tableData->categorySub, itemData.mItemEquipSlotIndex);
			_currentProfile->_equippedItemList.Emplace(slotType, itemData);
		}
		
		_currentProfile->_creatureGenderType = InPacket.CreatureGenderType();
		_currentProfile->_userLookInfo = InPacket.UserLookInfo();
		_currentProfile->_customizingFaceData = InPacket.CustomizingFaceData();
		_currentProfile->_customizingBodyData = InPacket.CustomizingBodyData();
		
		_currentProfile->_guildName = InPacket.GuildName();
		_currentProfile->_guildEmblemId = InPacket.GuildEmblemId();
		_currentProfile->_memberGrade = InPacket.MemberGrade();

		_currentProfile->_isExistGuild = _currentProfile->_guildName.IsEmpty() ? false : true;
	}
		
	_currentProfile->_accountGrade = InPacket.AccountGrade();
	_currentProfile->_rank = InPacket.UserRank();
	_currentProfile->_mostKeywordId = InPacket.MostKeywordId();
	_currentProfile->_mostKeywordCount = InPacket.MostKeywordCount();
	_currentProfile->_introductionText = InPacket.ProfileIntroduction();

	RefreshComment();

	if (0 >= _openedUserList.Num())
	{
		GSEventProgress()->OnReqStartEvent(EGsEventProgressType::USER_PROFILE, this);
	}
	else
	{
		if (_openedUserList.Last() != InPacket.UserDBId())
		{
			_openedUserList.Emplace(InPacket.UserDBId());
		}
		
		GMessage()->GetUserProfile().SendMessage(MessageContentUserProfile::INVALIDATE_ALL);
	}
}

void FGsUserProfileManager::Set(PD::GC::PKT_GC_ACK_USER_PROFILE_INTRODUCTION_UPDATE_READ& InPacket)
{
	_currentProfile->_introductionText = InPacket.IntroductionText();

	GMessage()->GetUserProfile().SendMessage(MessageContentUserProfile::INVALIDATE_INTRODUCTION);
}

void FGsUserProfileManager::Set(PD::GC::PKT_GC_ACK_USER_PROFILE_KEYWORD_INFO_READ& InPacket)
{
	if (GetTargetUserDBID() == InPacket.UserDBId())
	{
		_selectedKeyword = InPacket.SelectedKeyword();
		
		_currentKeywordCountMap.Empty();

		const UGsTableSocialKeyword* table = Cast<UGsTableSocialKeyword>(FGsSchemaSocialKeyword::GetStaticTable());
		if (nullptr == table)
			return;

		TArray<const FGsSchemaSocialKeyword*> keywordList;
		if (table->GetAllRows<FGsSchemaSocialKeyword>(keywordList))
		{
			for (const FGsSchemaSocialKeyword* keyword : keywordList)
			{
				_currentKeywordCountMap.Emplace(keyword->id, 0);
			}
		}

		for (PD::GC::PKT_GC_ACK_USER_PROFILE_KEYWORD_INFO_READ::SocialKeywordIterator iter = InPacket.GetFirstSocialKeywordIterator(); 
			iter != InPacket.GetLastSocialKeywordIterator(); ++iter)
		{
			if (_currentKeywordCountMap.Contains(iter->KeywordId()))
			{
				_currentKeywordCountMap[iter->KeywordId()] = iter->KeywordCount();
			}
		}

		GMessage()->GetUserProfile().SendMessage(MessageContentUserProfile::OPEN_KEYWORD_VOTE);
	}
}

void FGsUserProfileManager::Set(PD::GC::PKT_GC_ACK_USER_PROFILE_KEYWORD_UPDATE_READ& InPacket)
{
	if (GetTargetUserDBID() == InPacket.ProfileUserDBId())
	{
		if (_currentProfile.IsValid())
		{
			_currentProfile->_mostKeywordId = InPacket.MostKeyworkId();
			_currentProfile->_mostKeywordCount = InPacket.KeywordCount();

			GMessage()->GetUserProfile().SendMessage(MessageContentUserProfile::INVALIDATE_KEYWORD);
		}
	}
}

void FGsUserProfileManager::Set(PD::GC::PKT_GC_ACK_USER_PROFILE_COMMENT_LIST_READ& InPacket)
{
	if (IsMyProfile())
	{
		SetRedDot(false);
	}

	_existOldCommentInServer = false;
	if (GetTargetUserDBID() == InPacket.ProfileUserDBId())
	{
		for (PD::GC::PKT_GC_ACK_USER_PROFILE_COMMENT_LIST_READ::CommentInfolistIterator iter = InPacket.GetFirstCommentInfolistIterator();
			iter != InPacket.GetLastCommentInfolistIterator(); ++iter)
		{
			bool findSameId = false;
			for (const UserComment& commentData : _currentCommentList)
			{
				if (commentData._userDBId == iter->UserDBId())
				{
					findSameId = true;
					break;
				}
			}
			if (false == findSameId)
			{
				_existOldCommentInServer = true;
				_currentCommentList.Emplace(iter->UserDBId(), iter->UserName(), iter->CommentText(), iter->WriteTime());
			}
		}

		_currentCommentList.Sort([](const UserComment& itemA, const UserComment& itemB)
			{
				if (itemA._writeTime == itemB._writeTime)
					return itemA._userDBId < itemB._userDBId;

				return itemA._writeTime > itemB._writeTime;
			}
		);

		GMessage()->GetUserProfile().SendMessage(MessageContentUserProfile::INVALIDATE_COMMENT);
	}
}

void FGsUserProfileManager::Set(PD::GC::PKT_GC_ACK_USER_PROFILE_COMMENT_INPUT_READ& InPacket)
{
	RefreshComment();
}

void FGsUserProfileManager::Set(PD::GC::PKT_GC_ACK_USER_PROFILE_COMMENT_DELETE_READ& InPacket)
{
	GMessage()->GetUserProfile().SendMessage(MessageContentUserProfile::INVALIDATE_COMMENT);
}

void FGsUserProfileManager::SetNickNameChangeList(PD::GC::PKT_GC_ACK_USER_PROFILE_NICKNAME_CHANGE_LIST_READ& InPacket)
{
	_nickNameChangeList.Empty();

	for (PD::GC::PKT_GC_ACK_USER_PROFILE_NICKNAME_CHANGE_LIST_READ::NickNameChangeListIterator iter =
		InPacket.GetFirstNickNameChangeListIterator();
		iter != InPacket.GetLastNickNameChangeListIterator(); ++iter)
	{
		_nickNameChangeList.Emplace(TPair<FString, time_t>(iter->BeforeName(), iter->CreateTime()));
	}

	GMessage()->GetUserProfile().SendMessage(MessageContentUserProfile::INVALIDATE_NICKNAME_HISTORY);
}

bool FGsUserProfileManager::IsExistProfile()
{
	return (_openedUserList.Num() > 0);
}

bool FGsUserProfileManager::IsExistGuild()
{
	return _currentProfile.IsValid() ? _currentProfile->_isExistGuild : false;
}

bool FGsUserProfileManager::IsVisible()
{
	return _currentProfile.IsValid() ? _currentProfile->_isVisible : false;
}

bool FGsUserProfileManager::IsGM()
{
	return _currentProfile.IsValid() ? (_currentProfile->_accountGrade == AccountGrade::GM) : false;
}

UserDBId FGsUserProfileManager::PopLastOpenedUserDBID()
{
	if (_openedUserList.Num() > 0)
	{
		_openedUserList.RemoveAt(_openedUserList.Num() - 1);
	}
	return (_openedUserList.Num() > 0) ? _openedUserList.Last() : INVALID_USER_DB_ID;
}

UserDBId FGsUserProfileManager::GetTargetUserDBID()
{
	return _currentProfile.IsValid() ? _currentProfile->_userDBId : INVALID_USER_DB_ID;
}

FString FGsUserProfileManager::GetTargetUserName()
{
	return _currentProfile.IsValid() ? _currentProfile->_userName : TEXT("");
}

Level FGsUserProfileManager::GetTargetUserLevel()
{
	return _currentProfile.IsValid() ? _currentProfile->_userLevel : INVALID_LEVEL;
}

uint64 FGsUserProfileManager::GetMostKeywordId()
{
	return _currentProfile.IsValid() ? _currentProfile->_mostKeywordId : 0;
}

uint64 FGsUserProfileManager::GetMostKeywordCount()
{
	return _currentProfile.IsValid() ? _currentProfile->_mostKeywordCount : 0;
}

uint64 FGsUserProfileManager::GetKeywordCount(uint64 InKeywordId)
{
	return _currentKeywordCountMap.Contains(InKeywordId) ? _currentKeywordCountMap[InKeywordId] : 0;
}

uint64 FGsUserProfileManager::GetSelectedKeyword()
{
	return _selectedKeyword;
}

FString FGsUserProfileManager::GetTargetGuildName()
{
	return _currentProfile.IsValid() ? _currentProfile->_guildName : TEXT("");
}

GuildEmblemId FGsUserProfileManager::GetTargetGuildEmblemId()
{
	return _currentProfile.IsValid() ? _currentProfile->_guildEmblemId : INVALID_GUILD_EMBLEM_ID;
}

GuildMemberGradeType FGsUserProfileManager::GetTargetGuildMemberGrade()
{
	return _currentProfile.IsValid() ? _currentProfile->_memberGrade : GuildMemberGradeType::UNKNOWN;
}

FString FGsUserProfileManager::GetIntroductionText()
{
	return _currentProfile.IsValid() ? _currentProfile->_introductionText : TEXT("");
}

uint32 FGsUserProfileManager::GetRank()
{
	return _currentProfile.IsValid() ? _currentProfile->_rank : 0;
}

bool FGsUserProfileManager::GetTargetUserStat(OUT TArray<StatInfo>& OutStatInfoList)
{
	if (IsMyProfile())
	{
		CreatureWeaponType weaponType = CreatureWeaponType::HAND;
		TWeakPtr<FGsItem> item = GItem()->GetPlayerEquipedItemBySlotType(EGsEquipSlotType::WEAPON_TYPE);
		if (true == item.IsValid() && nullptr != item.Pin()->GetCastItem<FGsItemEquipment>())
		{
			FGsItemEquipment* equipItem = item.Pin()->GetCastItem<FGsItemEquipment>();
			weaponType = equipItem->GetCreatureWeaponType();
		}

		const UGsTablePlayerDetailInfoUIData* data = Cast<UGsTablePlayerDetailInfoUIData>(FGsSchemaPlayerDetailInfoUIData::GetStaticTable());
		if (nullptr == data)
			return false;

		const FGsSchemaPlayerDetailInfoUIData* row = nullptr;
		if (false == data->FindRowById(weaponType, row))
		{
			weaponType = CreatureWeaponType::HAND;
			if (false == data->FindRowById(weaponType, row))
			{
				return false;
			}
		}

		OutStatInfoList.Empty();
		TArray<FGsStatTypeNameSelector> displayTypeList = row->displayStatType;
		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* localPlayer = mgr->FindObject(EGsGameObjectType::LocalPlayer))
			{
				UGsGameObjectLocalPlayer* local = localPlayer->CastGameObject<UGsGameObjectLocalPlayer>();
				for (uint8 i = 0; i < displayTypeList.Num(); ++i)
				{
					const FGsStatInfo* statInfo = local->GetCreatureStatInfoByType(displayTypeList[i]);
					if (nullptr == statInfo)
						continue;

					OutStatInfoList.Emplace(statInfo->GetStatType(),statInfo->GetStatValue());
				}
			}
		}
	}
	else
	{
		CreatureWeaponType weaponType = CreatureWeaponType::HAND;
		const FGsSchemaItemEquipDetail* equipItem = UGsItemManager::GetEquipItemTableDataByTID(_currentProfile->_userLookInfo.mWeapon);
		if (nullptr != equipItem)
		{
			weaponType = equipItem->weaponType;
		}

		if (CreatureWeaponType::NONE == weaponType)
			return false;

		const UGsTablePlayerDetailInfoUIData* data = Cast<UGsTablePlayerDetailInfoUIData>(FGsSchemaPlayerDetailInfoUIData::GetStaticTable());
		if (nullptr == data)
			return false;
		

		const FGsSchemaPlayerDetailInfoUIData* row = nullptr;
		if (false == data->FindRowById(weaponType, row))
		{
			weaponType = CreatureWeaponType::HAND;
			if (false == data->FindRowById(weaponType, row))
			{
				return false;
			}
		}
			

		OutStatInfoList.Empty();
		TArray<FGsStatTypeNameSelector> displayTypeList = row->displayStatType;

		for (int32 typeIndex = 0; typeIndex < displayTypeList.Num(); ++typeIndex)
		{
			for (int32 statIndex = 0; statIndex < _currentProfile->_statInfoList.Num(); ++statIndex)
			{
				const StatInfo& statInfo = _currentProfile->_statInfoList[statIndex];
				if (statInfo.mType == displayTypeList[typeIndex])
				{
					OutStatInfoList.Emplace(statInfo.mType, statInfo.mValue);
					break;
				}
			}
		}
	}
	return true;
}

const ItemDataEquip* FGsUserProfileManager::GetTargetUserItemDataEquip(EGsEquipSlotType InSlotType)
{
	if (_currentProfile.IsValid())
	{
		if (_currentProfile->_equippedItemList.Contains(InSlotType))
		{
			return &_currentProfile->_equippedItemList[InSlotType];
		}
	}
	return nullptr;
}

CreatureGenderType FGsUserProfileManager::GetGenderType()
{
	return _currentProfile->_creatureGenderType;
}

const UserLookInfo* FGsUserProfileManager::GetUserLookInfo()
{
	if (IsMyProfile())
	{
		UGsGameObjectLocalPlayer* player = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer));
		if (nullptr == player)
			return nullptr;

		FGsGameObjectDataLocalPlayer* playerData = player->GetCastData<FGsGameObjectDataLocalPlayer>();
		if (nullptr == playerData)
			return nullptr;

		return playerData->GetUserLookInfo();
	}
	else
	{
		return &_currentProfile->_userLookInfo;
	}
}

const CustomizeFaceData* FGsUserProfileManager::GetCustomizeFaceData()
{
	if (IsMyProfile())
	{
		UGsGameObjectLocalPlayer* player = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer));
		if (nullptr == player)
			return nullptr;

		FGsGameObjectDataLocalPlayer* playerData = player->GetCastData<FGsGameObjectDataLocalPlayer>();
		if (nullptr == playerData)
			return nullptr;

		return playerData->GetCustomizeFaceData();
	}
	else
	{
		return &_currentProfile->_customizingFaceData;
	}
}

const CustomizeBodyData* FGsUserProfileManager::GetCustomizeBodyData()
{
	if (IsMyProfile())
	{
		UGsGameObjectLocalPlayer* player = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer));
		if (nullptr == player)
			return nullptr;

		FGsGameObjectDataLocalPlayer* playerData = player->GetCastData<FGsGameObjectDataLocalPlayer>();
		if (nullptr == playerData)
			return nullptr;

		return playerData->GetCustomizeBodyData();
	}
	else
	{
		return &_currentProfile->_customizingBodyData;
	}
}

time_t FGsUserProfileManager::GetLastCommentWriteTime()
{
	if (_currentCommentList.Num() > 0)
	{
		return _currentCommentList.Last()._writeTime;
	}
	
	return 0;
}

int32 FGsUserProfileManager::GetCommentCount()
{
	return _currentCommentList.Num();
}

const UserComment* FGsUserProfileManager::GetComment(int32 InIndex)
{
	if (_currentCommentList.Num() > InIndex)
	{
		return &_currentCommentList[InIndex];
	}
	return nullptr;
}

void FGsUserProfileManager::SetKeywordCountPlus(uint64 InKeywordId)
{
	if (_currentKeywordCountMap.Contains(InKeywordId))
	{
		++_currentKeywordCountMap[InKeywordId];
	}
}

void FGsUserProfileManager::SetKeywordCountMinus(uint64 InKeywordId)
{
	if (_currentKeywordCountMap.Contains(InKeywordId))
	{
		--_currentKeywordCountMap[InKeywordId];
	}
}

bool FGsUserProfileManager::CanRequestComment()
{
	return _canRequestComment && _existOldCommentInServer;
}

void FGsUserProfileManager::RequestComment()
{
	_canRequestComment = false;
	_commentTimer.Start();
	FGsNetSendService::SendReqUserProfileCommentList(GetTargetUserDBID(), GetLastCommentWriteTime() - 1);
}

void FGsUserProfileManager::ResetCommentTimer()
{
	_canRequestComment = true;
	_commentTimer.Stop();
}

void FGsUserProfileManager::RefreshComment()
{
	_currentCommentList.Empty();
	FGsNetSendService::SendReqUserProfileCommentList(GetTargetUserDBID(), 0);
}

void FGsUserProfileManager::DeleteComment(UserDBId InUserDBId)
{
	for (uint8 i = 0; i < _currentCommentList.Num(); ++i)
	{
		if (_currentCommentList[i]._userDBId == InUserDBId)
		{
			const FGsNetUserData* userData = GGameData()->GetUserData();
			if (nullptr == userData)
				return;

			// 내 프로필이거나 내가 작성한 프로필일 경우 삭제
			if (IsMyProfile() || InUserDBId == userData->mUserDBId)
			{
				_currentCommentList.RemoveAt(i);
				break;
			}
		}
	}

	FGsNetSendService::SendReqUserProfileCommentDelete(GetTargetUserDBID(), InUserDBId);
}

void FGsUserProfileManager::Clear()
{
	_openedUserList.Empty();
	_currentProfile.Reset();
	_currentCommentList.Empty();
	_currentKeywordCountMap.Empty();
	_selectedKeyword = 0;
	_canRequestComment = true;
	_nickNameChangeList.Empty();
}

void FGsUserProfileManager::ErrorInvalidUser()
{
	_openedUserList.Emplace(0);
}

void FGsUserProfileManager::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{
	if (false == _loadTopoStart)
	{
		_openedUserList.Emplace(_currentProfile->_userDBId);
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_USER_PROFILE);
	}
}

void FGsUserProfileManager::FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType)
{
	
}

void FGsUserProfileManager::BindMessages()
{
	FGsMessageHolder* messageMgr = GMessage();

	MSystemParam& msgSystem = messageMgr->GetSystemParam();
	_msgHandleSystemList.Emplace(msgSystem.AddRaw(MessageSystem::LOAD_TOPO_START,
		this, &FGsUserProfileManager::OnLoadTopoStart));

	MGameObject& msgGameObject = messageMgr->GetGameObject();
	_msgHandleGameObjectList.Emplace(msgGameObject.AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE,
		this, &FGsUserProfileManager::OnLocalSpawnMeComplete));
}

void FGsUserProfileManager::UnbindMessages()
{
	FGsMessageHolder* messageMgr = GMessage();
	if (nullptr == messageMgr)
	{
		return;
	}

	MSystemParam& msgSystem = messageMgr->GetSystemParam();
	for (MsgSystemHandle& handle : _msgHandleSystemList)
	{
		msgSystem.Remove(handle);
	}
	_msgHandleSystemList.Empty();

	MGameObject& msgGameObject = messageMgr->GetGameObject();
	for (MsgGameObjHandle& handle : _msgHandleGameObjectList)
	{
		msgGameObject.Remove(handle);
	}
	_msgHandleGameObjectList.Empty();
}

void FGsUserProfileManager::OnLoadTopoStart(const IGsMessageParam* inParam)
{
	_loadTopoStart = true;
}

void FGsUserProfileManager::OnLocalSpawnMeComplete(const IGsMessageParam*)
{
	_loadTopoStart = false;

	Clear();
}

void FGsUserProfileManager::UserProfile::Clear()
{
	_statInfoList.Empty();
	_equippedItemList.Empty();
}
