// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildManager.h"

#include "Runtime/ApplicationCore/Public/HAL/PlatformApplicationMisc.h"
#include "Runtime/Core/Public/Internationalization/Regex.h"
#include "Runtime/Core/Public/HAL/PlatformProcess.h"

#include "Net/GsNetSendServiceGuild.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsChatManager.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"
#include "DataSchema/Guild/GsSchemaGuildDailyActivity.h"
#include "DataSchema/Guild/GsSchemaGuildBuilding.h"
#include "DataSchema/Guild/GsSchemaGuildBuildingLevelData.h"
#include "DataSchema/Guild/GsSchemaGuildSnsShortcutUrl.h"
#include "DataSchema/GameObject/Camp/GsSchemaCampData.h"

#include "Guild/GsGuildHelper.h"

#include "UTIL/GsTimeSyncUtil.h"
#include "UTIL/GsTableUtil.h"

#include "T1Project.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Guild/GsGuildData.h"
#include "Classes/SecureVar/GsSecureVar.h"
#include "DataSchema/GsSchemaEnums.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UTIL/GsText.h"

void FGsGuildManager::Initialize()
{
	_sendController.Initialize();

	_myGuildData.Empty();
	_myGuildQuestHandler = TUniquePtr<FGsGuildQuestDataHandler>(new FGsGuildQuestDataHandler());

	// 레드닷 함수 연결.
	_redDotCheckMap.Empty();
	_redDotCheckMap.Emplace(EGsGuildRedDotType::Attendance, new FGsRedDotCheckFunc([this]()
		{
			return CheckRedDotAttendance();
		}));
	_redDotCheckMap.Emplace(EGsGuildRedDotType::Notice, new FGsRedDotCheckFunc([this]()
		{
			return CheckRedDotNoticeChanged();
		}));
	_redDotCheckMap.Emplace(EGsGuildRedDotType::Buildings, new FGsRedDotCheckFunc([this]()
		{
			return CheckRedDotBuildings();
		}));
	_redDotCheckMap.Emplace(EGsGuildRedDotType::Buffs, new FGsRedDotCheckFunc([this]()
		{
			return CheckRedDotBuff();
		}));
	_redDotCheckMap.Emplace(EGsGuildRedDotType::Approval, new FGsRedDotCheckFuncNumber([this]()
		{
			return GetJoinWaitingUserCount();
		}));
	_redDotCheckMap.Emplace(EGsGuildRedDotType::DiplomacyEnemy, new FGsRedDotCheckFunc([this]()
		{
			return CheckRedDotDiplomacyEnemy();
		}));
	_redDotCheckMap.Emplace(EGsGuildRedDotType::DiplomacyAlly, new FGsRedDotCheckFunc([this]()
		{
			return CheckRedDotDiplomacyAlly();
		}));
	_redDotCheckMap.Emplace(EGsGuildRedDotType::DiplomacyState, new FGsRedDotCheckFunc([this]()
		{
			return CheckRedDotDiplomacyState();
		}));
	_redDotCheckMap.Emplace(EGsGuildRedDotType::Quest, new FGsRedDotCheckFunc([this]()
		{
			return CheckRedDotQuest();
		}));
	_redDotCheckMap.Emplace(EGsGuildRedDotType::ItemCollection, new FGsRedDotCheckFunc([this]()
		{
			return CheckRedDotItemCollection();
		}));
	_redDotCheckMap.Emplace(EGsGuildRedDotType::GuildStorage, new FGsRedDotCheckFunc([this]()
		{
			return CheckRedDotStorage();
		}));
	_redDotCheckMap.Emplace(EGsGuildRedDotType::AcquisitionSetting, new FGsRedDotCheckFunc([this]()
		{
			return CheckRedDotAcquisitionSetting();
		}));
	_redDotCheckMap.Emplace(EGsGuildRedDotType::Division, new FGsRedDotCheckFunc([this]()
		{
			return CheckRedDotDivision();
		}));
	_redDotCheckMap.Emplace(EGsGuildRedDotType::Auction, new FGsRedDotCheckFunc([this]()
		{
			return CheckRedDotAuction();
		}));
	_redDotCheckMap.Emplace(EGsGuildRedDotType::GuildCalculate, new FGsRedDotCheckFunc([this]()
		{
			return CheckRedDotCalculate();
		}));
	_redDotCheckMap.Emplace(EGsGuildRedDotType::Sanctum, new FGsRedDotCheckFunc([this]()
		{
			return CheckRedDotSanctum();
		}));

	RegisterMessages();
}

void FGsGuildManager::Finalize()
{
	_sendController.ResetAllCoolTime();

	for (TPair<EGsGuildRedDotType,IGsRedDotCheck*>& itPair : _redDotCheckMap)
	{
		delete itPair.Value;
		itPair.Value = nullptr;
	}
	_redDotCheckMap.Empty();

	RemoveAll();

	UnRegisterMessages();

	_myGuildQuestHandler.Reset();
}


void FGsGuildManager::RegisterMessages()
{
	FGsMessageHolder* MessageManager = GMessage();
	if (nullptr == MessageManager)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	_listCheckRedDotHandler.Emplace(MessageManager->GetGuild().AddRaw(MessageContentGuild::CHECK_REDDOT
		, this, &FGsGuildManager::OnCheckRedDot));

	_listGameObjectHandler.Emplace(MessageManager->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE,
		this, &FGsGuildManager::OnLocalPlayerSpawnComplete));
}

void FGsGuildManager::UnRegisterMessages()
{
	FGsMessageHolder* MessageManager = GMessage();
	if (nullptr == MessageManager)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	for (const TPair<MessageContentGuild, FDelegateHandle>& guildDelegate : _listCheckRedDotHandler)
	{
		MessageManager->GetGuild().Remove(guildDelegate);
	}
	_listCheckRedDotHandler.Empty();

	for (const MsgGameObjHandle& gameobjectDelegate : _listGameObjectHandler)
	{
		MessageManager->GetGameObject().Remove(gameobjectDelegate);
	}
	_listGameObjectHandler.Empty();
}

bool FGsGuildManager::CheckRedDot(bool bInUpdate)
{
	// HUD 버튼에 레드닷 체크 시 들어오는 부분
	if (IsGuild())
	{
		for (TPair<EGsGuildRedDotType, IGsRedDotCheck*>& itPair : _redDotCheckMap)
		{
			if (itPair.Value->CheckRedDot(bInUpdate))
			{
				SetRedDot(true);
				return true;
			}
		}
	}

	SetRedDot(false);
	return false;
}

void FGsGuildManager::RemoveAll()
{
	// 길드 관련 정보 삭제
	RemoveAllGuildData(false);	

	if (_otherGuildDetailData)
	{
		delete _otherGuildDetailData;
		_otherGuildDetailData = nullptr;
	}

	if (_otherGuildBuildingData)
	{
		_otherGuildBuildingData->Empty();
		delete _otherGuildBuildingData;
		_otherGuildBuildingData = nullptr;
	}
}

void FGsGuildManager::RemoveWhenFlowChanged()
{
	// 로비로 갈 때 삭제할 것들
	RemoveAll();

	_sendController.ResetAllCoolTime();

	// 재가입 쿨타임 삭제
	_rejoinCoolTimeEndTime = 0;
}

void FGsGuildManager::RemoveAllGuildData(bool bInUpdatePassivity)
{
	_myGuildData.Empty();
	_myBuildingData.Empty();
	_myItemCollection.Leave();
	if (_myGuildQuestHandler.IsValid())
	{
		_myGuildQuestHandler->Clear();
	}
	_redDotCheckData.Empty();

	_joinGuildList.Empty();
	_joinWaitingGuildList.Empty();
	_poolGuildData.RemovePool();

	_joinWaitingUserCount = 0;
	_joinWaitingUserList.Empty();

	_memberList.Empty();
	_poolGuildMemberData.RemovePool();

	_myStorageData.RemoveAll();
	_myAuctionData.ResetData();

	_campDataList.Empty();
	_poolCampData.RemovePool();

	_authorityList.Empty();

	_agitDataController.Clear();

	_groupChatRoomData.Reset();

	// 레드닷 정보는 굳이 리셋안해도 이 시간보다 뒷시간이 잡힐테니 그냥 두어도 될 듯

	// 체커 쿨타임 리셋
	_sendController.ResetAllCoolTime();

	if (bInUpdatePassivity)
	{
		// 건물, 아이템 컬렉션 정보가 사라졌으므로 패시비티 갱신 요청
		FGsPrimitiveInt32 paramCollection(static_cast<int32>(PassivityOwnerType::GUILD_BUILDING));
		GMessage()->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_UPDATE_COLLECTION, &paramCollection);
	}

	if (FGsGameDataManager* gameDataMgr = GGameData())
	{
		gameDataMgr->ResetGuildPlayerData();
	}
}

void FGsGuildManager::RemoveAllGuildList()
{
	for (FGsGuildData* iter : _joinGuildList)
	{
		_poolGuildData.Release(iter);
	}
	_joinGuildList.Empty();
}

void FGsGuildManager::RemoveJoinGuild(GuildDBId InGuildDBId)
{
	for (FGsGuildData* guild : _joinGuildList)
	{
		if (InGuildDBId == guild->_guildDBId)
		{
			_joinGuildList.Remove(guild);
			_poolGuildData.Release(guild);
			return;
		}
	}
}

void FGsGuildManager::RemoveJoinWaitingGuild(GuildDBId InGuildDBId)
{
	for (FGsGuildData* guild : _joinWaitingGuildList)
	{
		if (InGuildDBId == guild->_guildDBId)
		{
			_joinWaitingGuildList.Remove(guild);
			_poolGuildData.Release(guild);
			return;
		}
	}
}

void FGsGuildManager::RemoveAllJoinWaitingGuild()
{
	for (FGsGuildData* iter : _joinWaitingGuildList)
	{
		_poolGuildData.Release(iter);
	}
	_joinWaitingGuildList.Empty();
}

bool FGsGuildManager::RemoveGuildMember(UserDBId InUserDBId)
{
	for (FGsGuildMemberData* member : _memberList)
	{
		if (InUserDBId == member->_userDBId)
		{
			_memberList.Remove(member);
			_poolGuildMemberData.Release(member);
			--_myGuildData._memberNum;

			return true;
		}
	}

	return false;
}

bool FGsGuildManager::RemoveGuildMember(const FString& InUserName)
{
	for (FGsGuildMemberData* member : _memberList)
	{
		if (InUserName == member->_userName)
		{
			_memberList.Remove(member);
			_poolGuildMemberData.Release(member);
			--_myGuildData._memberNum;

			return true;
		}
	}

	return false;
}

void FGsGuildManager::RemoveAllGuildMember()
{
	for (FGsGuildMemberData* member : _memberList)
	{
		_poolGuildMemberData.Release(member);
	}
	_memberList.Empty();
}

bool FGsGuildManager::RemoveCampData(CampId InId)
{
	if (INVALID_CAMP_ID == InId)
	{
		if (0 < _campDataList.Num())
		{
			_campDataList.Empty();
			_poolCampData.ReleaseAll();

			return true;
		}
	}

	for (FGsGuildCampData* camp : _campDataList)
	{
		if (InId == camp->_campId)
		{
			_campDataList.Remove(camp);
			_poolCampData.Release(camp);

			return true;
		}
	}

	return false;
}

void FGsGuildManager::ResetTimeContents(ResetTimeTicket InType)
{
	switch (InType)
	{
	case ResetTimeTicket::RTT_DAILY:
		ResetDaily();
		break;
	case ResetTimeTicket::RTT_WEEKLY:
		ResetItemCollectionList(ResetTimeTicket::RTT_WEEKLY);
		break;
	case ResetTimeTicket::RTT_GUILD_QUEST:
		ResetQuest();
		break;
	case ResetTimeTicket::RTT_GUILD_ZEAL_POINT_RESET:
		_myGuildData.SetZealPoint(0);
		break;
	case ResetTimeTicket::RTT_AGIT_DUNGEON_RAID:
		_agitDataController.ResetDungeon();
		break;
	}

	// HUD 레드닷 갱신요청
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, false);
}

void FGsGuildManager::ResetDaily()
{
	// 길드가 없으면 무시
	if (false == _myGuildData.IsExistGuild())
	{
		return;
	}

	_myGuildData.ResetDaily();

	// 정보를 새로받을 수 있게 쿨타임 리셋
	_sendController.ResetAllCoolTime();
}

void FGsGuildManager::ResetQuest()
{
	// 길드가 없으면 무시
	if (false == _myGuildData.IsExistGuild())
	{
		return;
	}

	// 주의: 진행중이던 퀘스트는 UpdateQuest 패킷에 DELETED타입으로 오며, 이를 받아서 삭제한다.
	if (_myGuildQuestHandler.IsValid())
	{
		_myGuildQuestHandler->Clear();

		// 마지막 퀘스트 갱신 시간을 현재 시간으로 갱신
		_myGuildQuestHandler->SetLastQuestResetTime(FGsTimeSyncUtil::GetServerNowSecond());

		_sendController.ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::QuestInfo);

		CheckRedDotByType(EGsGuildRedDotType::Quest);
	}
}

void FGsGuildManager::ResetItemCollectionList(ResetTimeTicket InType)
{
	if (false == _myGuildData.IsExistGuild() || ResetTimeTicket::RTT_WEEKLY != InType)
	{
		return;
	}

	//_myItemCollection.
}

void FGsGuildManager::SetMyGuildInfo(PD::GC::PKT_GC_MY_GUILD_INFO_READ& InPacket)
{
	_myGuildData.SetMyGuildInfo(InPacket);
	//_myStorageData.SetLastItemInfo(InPacket.LastItemAcquiredTime(), InPacket.LastItemDistributedTime());
	_myAuctionData.SetAcquisitionSettings(InPacket.ConstRefAcquisitionSettings());
	_myAuctionData.SetLastUpdateTimeDivision(InPacket.LastItemAcquiredTime());
	_myAuctionData.SetLastUpdateTimeCalculate (InPacket.LastItemDistributedTime());
	_redDotCheckData.SetData(InPacket.mRedDotDetail);

	_myItemCollection.Enter(this);

	_myBuildingData.Empty();
	_myBuildingData.SetBuffReactivateTime(InPacket.ConstRefGuildDetailInfo().mBuffReactivateTime);

	TArray<GuildBuildingData> tempList;
	InPacket.MakeGuildBuildingDataVector<TArray<GuildBuildingData>, GuildBuildingData>(tempList);
	for (GuildBuildingData& iter : tempList)
	{
		SetBuildingDataInter(iter, true);
	}

	_sendController.ResetAllCoolTime();

	if (_myGuildQuestHandler.IsValid())
	{
		_myGuildQuestHandler->SetLastQuestResetTime(InPacket.GuildQuestBeginTime());
	}

	CheckRedDotByType(EGsGuildRedDotType::Attendance);
	CheckRedDotByType(EGsGuildRedDotType::Notice);
	CheckRedDotByType(EGsGuildRedDotType::Buildings);
	CheckRedDotByType(EGsGuildRedDotType::Buffs);

	InitializeGroupChatRoom(true);
}

void FGsGuildManager::SetUserGuildInfo(PD::SC::PKT_SC_USER_GUILD_INFO_READ& InPacket)
{
	_myGuildData.SetUserGuildInfo(InPacket);
	_myStorageData.Initialize();
	_myAuctionData.Initialize();
	
	// 2021/11/23 PKT - InGame 내에서 길드 가입시 호출
	_myItemCollection.Enter(this);
}

void FGsGuildManager::SetUserGuildInfoDetail(PD::GC::PKT_GC_ACK_OPEN_GUILD_READ& InPacket)
{
	_myGuildData.Set(InPacket);
	_myAuctionData.SetAcquisitionSettings(InPacket.ConstRefAcquisitionSettings());
	_agitDataController.SetRentalEndDate(InPacket.ConstRefGuildDetailInfo().mAgitRentalEndDate);

	CheckRedDotByType(EGsGuildRedDotType::Attendance);
	CheckRedDotByType(EGsGuildRedDotType::Notice);

	// Enemy는 일부 누락된 정보가 오기 때문에 다시 받아야 함.
	// Ally는 같은 정보이므로 다시 받을 필요 없음.
	// AllyState는 비워버리기 때문에 다시 받아야 함.
	_sendController.ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::EnemyList);
	//_sendController.ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::AllyList);
	_sendController.ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::AllyStateList);

	InitializeGroupChatRoom(false);
}

void FGsGuildManager::SetGuildList(PD::GC::PKT_GC_ACK_GUILD_LIST_READ& InPacket)
{
	RemoveAllGuildList();

	for (PD::GC::PKT_GC_ACK_GUILD_LIST_READ::GuildListIterator iter = InPacket.GetFirstGuildListIterator();
		iter != InPacket.GetLastGuildListIterator(); ++iter)
	{
		FGsGuildData* data = _poolGuildData.Claim();
		data->Set(*iter);
		
		_joinGuildList.Emplace(data);
	}

	/*
#if WITH_EDITOR
	FDateTime serverTime = FGsTimeSyncUtil::ConvertToDateTime(InPacket.ReJoinCoolDownTime());
	FDateTime utcTime = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(InPacket.ReJoinCoolDownTime());
	FDateTime clientTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(InPacket.ReJoinCoolDownTime());
	GSLOG(Warning, TEXT("rejoinCoolTime: time_t: %d, server: %s / ToUtc: %s, ToClient: %s"), 
		InPacket.ReJoinCoolDownTime(), *serverTime.ToString(), *utcTime.ToString(), *clientTime.ToString());
#endif
	*/

	// 주의: 서버 기준시로 받는다. 쿨타임 종료되는 시간을 받는다.
	_rejoinCoolTimeEndTime = InPacket.ReJoinCoolDownTime();
}

void FGsGuildManager::SetRequestedJoinGuildList(PD::GC::PKT_GC_ACK_REQUESTED_JOIN_GUILD_LIST_READ& InPacket)
{
	RemoveAllJoinWaitingGuild();

	for (PD::GC::PKT_GC_ACK_REQUESTED_JOIN_GUILD_LIST_READ::GuildListIterator iter = InPacket.GetFirstGuildListIterator();
		iter != InPacket.GetLastGuildListIterator(); ++iter)
	{
		FGsGuildData* data = _poolGuildData.Claim();
		data->Set(*iter);

		_joinWaitingGuildList.Emplace(data);
	}
}

void FGsGuildManager::SetJoinResultWaiting(GuildDBId InGuildDBId)
{
	FGsGuildData** waitingGuildPtr = _joinWaitingGuildList.FindByPredicate([InGuildDBId](FGsGuildData* InData)
		{
			return (InGuildDBId == InData->_guildDBId) ? true : false;
		});

	if (nullptr == waitingGuildPtr)
	{
		FGsGuildData** guildPtr = _joinGuildList.FindByPredicate([InGuildDBId](FGsGuildData* InData)
			{
				return (InGuildDBId == InData->_guildDBId) ? true : false;
			});

		if (guildPtr)
		{
			FGsGuildData* data = _poolGuildData.Claim();
			data->Set(**guildPtr);

			_joinWaitingGuildList.Emplace(data);
		}
	}

	// 신청 리스트 클릭 시 무조건 다시받도록 쿨타임 리셋
	_sendController.ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::WaitingGuildList);
}

void FGsGuildManager::SetJoinWaitingUserList(PD::GC::PKT_GC_ACK_WAITING_JOIN_GUILD_USER_LIST_READ& InPacket)
{
	int32 prevNum = _joinWaitingUserCount;

	_joinWaitingUserList.Empty();

	for (PD::GC::PKT_GC_ACK_WAITING_JOIN_GUILD_USER_LIST_READ::UserListIterator iter = InPacket.GetFirstUserListIterator();
		iter != InPacket.GetLastUserListIterator(); ++iter)
	{
		_joinWaitingUserList.Emplace(FGsGuildJoinWaitingUser(*iter));
	}	

	_joinWaitingUserCount = _joinWaitingUserList.Num();
	if (prevNum != _joinWaitingUserCount)
	{
		CheckRedDotByType(EGsGuildRedDotType::Approval);

		// 있다가 없어졌거나 없다가 생긴경우를 체크
		if (0 == prevNum || 0 == _joinWaitingUserCount)
		{
			GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);
		}
	}
}

void FGsGuildManager::SetGuildMemberList(PD::GC::PKT_GC_ACK_GUILD_MEMBER_LIST_READ& InPacket)
{
	RemoveAllGuildMember();
	
	for (PD::GC::PKT_GC_ACK_GUILD_MEMBER_LIST_READ::MemberListIterator iter = InPacket.GetFirstMemberListIterator();
		iter != InPacket.GetLastMemberListIterator(); ++iter)
	{
		FGsGuildMemberData* data = _poolGuildMemberData.Claim();
		data->Set(*iter);

		_memberList.Emplace(data);
	}

	_myGuildData._memberNum = _memberList.Num();
}

void FGsGuildManager::SetApproveJoin(UserDBId InUserDBId)
{
	for (int i = 0, maxCount = _joinWaitingUserList.Num(); i < maxCount; ++i)
	{
		if (InUserDBId == _joinWaitingUserList[i]._userDBId)
		{
			_joinWaitingUserList.RemoveAt(i);
			_joinWaitingUserCount = _joinWaitingUserList.Num();
			break;
		}
	}

	++_myGuildData._memberNum;

	// 멤버창 들어갈 때 새 정보 받아야 하므로 리셋
	_sendController.ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::MemberList);

	CheckRedDotByType(EGsGuildRedDotType::Approval);
}

void FGsGuildManager::SetRejectJoin(UserDBId InUserDBId)
{
	for (int i = 0, maxCount = _joinWaitingUserList.Num(); i < maxCount; ++i)
	{
		if (InUserDBId == _joinWaitingUserList[i]._userDBId)
		{
			_joinWaitingUserList.RemoveAt(i);
			_joinWaitingUserCount = _joinWaitingUserList.Num();
			break;
		}
	}

	CheckRedDotByType(EGsGuildRedDotType::Approval);
}

void FGsGuildManager::SetAuthorityList(PD::GC::PKT_GC_ACK_GUILD_MEMBER_RIGHTS_BY_GRADE_LIST_READ& InPacket)
{
	_authorityList.Empty();
	InPacket.MakeGuildMemberRightsbyGradeVector<TArray<GuildMemberRightsbyGrade>, GuildMemberRightsbyGrade>(
		_authorityList);
}

void FGsGuildManager::SetChangeAuthority(PD::GC::PKT_GC_ACK_CHANGED_GUILD_MEMBER_RIGHTS_BY_GRADE_READ& InPacket)
{
	TArray<GuildMemberRightsbyGrade> tempList;
	InPacket.MakeGuildMemberRightsbyGradeVector<TArray<GuildMemberRightsbyGrade>, GuildMemberRightsbyGrade>(
		tempList);

	for (GuildMemberRightsbyGrade& itTemp : tempList)
	{
		for (GuildMemberRightsbyGrade& itData : _authorityList)
		{
			if (itTemp.mMemberGradeType == itData.mMemberGradeType)
			{
				itData.mMemeberRights = itTemp.mMemeberRights;
			}
		}
	}
}

void FGsGuildManager::SetGuildChatList(PD::GC::PKT_GC_GUILD_CHAT_LIST_RECENT_READ& InPacket)
{
	_loggedChatList.Empty();

	auto count = InPacket.GetGuildChatListCount();
	auto iter = InPacket.GetFirstGuildChatListIterator();

	auto enditer = InPacket.GetLastGuildChatListIterator();
	for (; iter != enditer; ++iter)
	{
		_loggedChatList.Add(FGsGuildChatList{ iter->GuildChatDBId(), iter->MessageTime(), iter->UserName(), iter->Message(), iter->BadgeRank()});
	}
}


//void FGsGuildManager::SetAllyGuildChatList(PD::GC::PKT_GC_GUILD_ALLY_CHAT_LIST_RECENT_READ& InPacket)
//{
//	_loggedAllyChatList.Empty();
//
//	auto iter = InPacket.GetFirstGuildChatListIterator();
//
//	auto enditer = InPacket.GetLastGuildChatListIterator();
//	for (; iter != enditer; ++iter)
//	{
//		_loggedAllyChatList.Add(FGsGuildChatList{ iter->GuildChatDBId(), iter->MessageTime(), iter->UserName(), iter->Message(), INVALID_RANK,
//			iter->HomeWorldId(), iter->AccountGrade(), iter->GuildId(), iter->GuildEmblemId()});
//	}
//}


void FGsGuildManager::BuildItemCollectionList(PD::GC::PKT_GC_ACK_GUILD_ITEM_COLLECTION_LIST_READ& InPacket)
{
	// 2021/10/26 PKT - Build 용 Item Collection List
	TArray<GuildItemCollectionInfo> buildCollectionIdList;
	InPacket.MakeGuildItemCollectionInfoListVector<TArray<GuildItemCollectionInfo>, GuildItemCollectionInfo>(buildCollectionIdList);

	// 2022/02/24 PKT - 등록된 컬렉션 정보
	TArray<GuildCollectionRegisterData> registerCollectionConditionList;
	using RegisterInfoListIt = PD::GC::PKT_GC_ACK_GUILD_ITEM_COLLECTION_LIST_READ::CompletedRegisterInfoListIterator;
	for (RegisterInfoListIt iter = InPacket.GetFirstCompletedRegisterInfoListIterator(); 
		iter != InPacket.GetLastCompletedRegisterInfoListIterator(); ++iter)
	{
		GuildCollectionRegisterData registerData;
		registerData._collectionConditionId = iter->CompletedConditionId();
		registerData._registerUserName = FText::FromString(iter->UserName());
		
		registerCollectionConditionList.Emplace(registerData);
	}

	// 2021/10/26 PKT - 북마크 상태에 대한 Collection List
	TArray<CollectionId> bookMarkCollectionList;
	InPacket.MakeBookmarkCollectionIdListVector<TArray<CollectionId>, CollectionId>(bookMarkCollectionList);

	// 2021/10/26 PKT - 길드 아이템 컬렉션에 등록한 횟수
	const int32 RegisterCount = InPacket.RegisterCount();

	_myItemCollection.Ack_ItemCollectionSlotListAll(buildCollectionIdList
													, registerCollectionConditionList
													, bookMarkCollectionList
													, RegisterCount);
}

void FGsGuildManager::CompletedItemCollectionConditionAck(PD::SC::PKT_SC_ACK_GUILD_ITEM_COLLECTION_REGISTER_READ& InPacket)
{	// 2021/10/28 PKT - 자신이 등록 했을 때 오는 패킷
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
	{
		GSLOG(Error, TEXT("nullptr == userData"));
		return;
	}

	FText userName = FText::FromString(userData->mUserName);	
	_myItemCollection.Ack_CompleteItemCollectionCondition(InPacket.ConditionId(), userName, InPacket.RegisterCount());
}

void FGsGuildManager::CompletedItemCollectionConditionNotify(PD::SC::PKT_SC_INFORM_GUILD_ITEM_COLLECTION_REGISTER_READ& InPacket)
{	// 2021/10/28 PKT - 길드원이 등록 했을 때 오는 패킷
	FText registerName = FText::FromString(InPacket.UserName());
	_myItemCollection.Ack_CompleteItemCollectionCondition(InPacket.ConditionId(), registerName);
}

void FGsGuildManager::BookMarkItemCollection(CollectionId InCollectionId, bool InFlag)
{
	_myItemCollection.Ack_ItemCollectionBookMark(InCollectionId, InFlag);
}

void FGsGuildManager::SetDungeonBestLevel(GuildDunGeonLevel InBestLevel)
{
	if (IsGuild())
	{
		_myGuildData._dungeonBestLevel = InBestLevel;
	}
}

void FGsGuildManager::SetClearSanctumNexusData()
{
	_mySanctumData.Clear();
}

void FGsGuildManager::SetSanctumOccupySuccessTime()
{
	if (IsGuild())
	{
		_myGuildData.SetSanctumOccupySuccessTime();
	}
}

void FGsGuildManager::SetChangedMyAuthority(GuildRightsLevel InRights)
{
	_myGuildData._authority = InRights;
}

void FGsGuildManager::SetChangeGuildString(const FString& InString, bool bIsNotice)
{
	if (bIsNotice)
	{
		_myGuildData.SetNotice(InString, true);		
	}
	else
	{
		_myGuildData.SetIntroduction(InString);
	}
}

void FGsGuildManager::SetChangeJoinType(GuildJoinType InType)
{
	_myGuildData._joinType = InType;
}

void FGsGuildManager::SetChangeEmblem(GuildEmblemId InEmblemId)
{
	_myGuildData._emblemId = InEmblemId;
}

void FGsGuildManager::SetChangeLimitUserLevel(Level InLevel)
{
	_myGuildData._joinLimitLevel = InLevel;
}

void FGsGuildManager::SetChangeMemberGrade(UserDBId InUserDBId, GuildMemberGradeType InGrade)
{
	// 길드 멤버 화면에 안들어가있는 상태면 이 리스트는 없을 수 있다
	for (FGsGuildMemberData* member : _memberList)
	{
		if (InUserDBId == member->_userDBId)
		{
			member->_memberGrade = InGrade;

			// 길마 변경 시
			if (GuildMemberGradeType::CAPTAIN == InGrade)
			{
				_myGuildData._masterName = member->_userName;
			}
		}
	}

	// 내가 길마고, 길마 권한 넘길때는 내 정보 변경 가능할지도
	if (GetMyUserDBId() == InUserDBId)
	{
		_myGuildData._memberGrade = InGrade;
	}
}

void FGsGuildManager::SetChangeMemberGrade(const FString& InUserName, GuildMemberGradeType InGrade)
{
	// 길드 멤버 화면에 안들어가있는 상태면 이 리스트는 없을 수 있다
	for (FGsGuildMemberData* member : _memberList)
	{
		if (InUserName == member->_userName)
		{
			member->_memberGrade = InGrade;
		}
	}

	// 길마 변경 시
	if (GuildMemberGradeType::CAPTAIN == InGrade)
	{
		_myGuildData._masterName = InUserName;
	}

	// 내 정보면 
	if (const FGsNetUserData* userData = GGameData()->GetUserData())
	{
		if (InUserName == userData->mUserName)
		{
			_myGuildData._memberGrade = InGrade;
		}
	}
}

void FGsGuildManager::SetDailyActivity(PD::SC::PKT_SC_ACK_GUILD_DAILY_ACTIVITY_READ& InPacket)
{
	switch (InPacket.GuildDailyActivityType())
	{
	case GuildDailyActivityType::ATTENDANCE:
	{
		_myGuildData._attendance = (0 < InPacket.GuildDailyActivityCnt()) ? true : false;

		// 길드 메인페이지 출석인원 표시 위해
		++_myGuildData._attendanceCount;
	}
	break;
	case GuildDailyActivityType::NORMAL_DONATE:
	case GuildDailyActivityType::ADVANCED_DONATE:
	{
		_myGuildData._donationCount = InPacket.GuildDailyActivityCnt();
	}
	break;
	}

	// 멤버 리스트 쿨타임 리셋(길드원 탭 들어갈 때 출석 및 공헌도 갱신받기 위함)
	_sendController.ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::MemberList);

	CheckRedDotByType(EGsGuildRedDotType::Attendance);
}

void FGsGuildManager::SetGuildContribution(PD::GC::PKT_GC_GUILD_CONTRIBUITON_UPDATE_READ& InPacket, bool bInUpdate)
{
	GuildLevel prevLevel = _myGuildData._guildLevel;
	_myGuildData._guildLevel = InPacket.GuildLevel();
	_myGuildData._contribution = InPacket.GuildContribution();
	_myGuildData._campLevel = InPacket.CampLevel();

	if (bInUpdate)
	{
		GMessage()->GetGuild().SendMessage(MessageContentGuild::CHANGE_CONTRIBUTION, nullptr);

		if (prevLevel < _myGuildData._guildLevel)
		{
			GMessage()->GetGuild().SendMessage(MessageContentGuild::CHANGE_GUILD_LEVEL, nullptr);
		}
	}
}

void FGsGuildManager::SetGuildAsset(Currency InAmount, bool bInUpdate)
{
	_myGuildData._guildAsset.Set(InAmount);

	if (bInUpdate)
	{
		GMessage()->GetUserBasicInfo().SendMessage(MessageUserInfo::CURRENCY,
			static_cast<uint64>(CurrencyType::GUILD_ASSET));
	}
}

void FGsGuildManager::SetSNSLink(int32 InIndex, const FString& InURL)
{
	_myGuildData.SetSNSLink(InIndex, InURL);
}

void FGsGuildManager::SetGuildBuildingList(PD::GC::PKT_GC_ACK_GUILD_BUILDING_LIST_READ& InPacket)
{
	_myBuildingData.Empty();
	_myBuildingData.SetBuffReactivateTime(InPacket.BuffReactivateRemainTime());

	TArray<GuildBuildingData> tempList;
	InPacket.MakeGuildBuildingDataVector<TArray<GuildBuildingData>, GuildBuildingData>(tempList);
	for (GuildBuildingData& iter : tempList)
	{
		SetBuildingDataInter(iter, true);
	}

	_mySanctumData.SetData(InPacket.ConstRefSanctumNexusInfo());

	/*
	// 캠프정보 비우고 새로 넣기
	_campDataList.Empty();
	_poolCampData.ReleaseAll();

	TArray<GuildCampInfo> tempCampList;
	InPacket.MakeGuildCampInfoVector<TArray<GuildCampInfo>, GuildCampInfo>(tempCampList);
	for (GuildCampInfo& iter : tempCampList)
	{
		FGsGuildCampData* campData = _poolCampData.Claim();
		campData->SetData(iter);

		_campDataList.Emplace(campData);
	}
	*/
}

void FGsGuildManager::SetBuildingOpen(const GuildBuildingData& InData)
{
	SetBuildingDataInter(InData, true);
}

void FGsGuildManager::SetBuildingUpgrade(GuildBuildingId InId, GuildBuildingLevel InNextLevel)
{
	_myBuildingData.SetUpgradeLevel(InId, InNextLevel);
}

void FGsGuildManager::SetActiveGuildBuff(PD::GC::PKT_GC_ACK_GUILD_BUILDING_PASSIVITY_READ& InPacket)
{
	_myBuildingData.SetActiveGuildBuff(InPacket.GuildBuildingId(), InPacket.PassivityId(), InPacket.Cooldown());
}

void FGsGuildManager::SetBuildingNotify(PD::SC::PKT_SC_NOTIFY_GUILD_BUILDING_READ& InPacket)
{
	SetBuildingDataInter(InPacket.ConstRefGuildBuildingData(), true);

	CheckRedDotByType(EGsGuildRedDotType::Buildings);
	CheckRedDotByType(EGsGuildRedDotType::Buffs);

	// 길드 버프 활성/종료 시, UI에서 나가지 않고 유지하면, 재활성 대기시간을 받을 수 없다.
	// 따라서 계산하여 강제로 세팅을 해주는데, 이후 건물리스트 등을 다시 받으면 서버 값으로 덮어쓸 수 있다.
	// 종료에만 해줘도 되는걸 시작시에도 하는 이유는, Notify가 오는데 몇초정도 딜레이가 있을 수 있기 때문
	switch (InPacket.NotifyType())
	{
	case GuildBuildingNotifyType::PASSIVITY_START: // 건물 패시브 시작
	{
		if (const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig())
		{
			const FGsGuildBuildingData* buidlingData = _myBuildingData.GetBuildingData(
				InPacket.ConstRefGuildBuildingData().mBuildingId);

			if (buidlingData)
			{
				// 기본 재활성 시간(분)을 초로 변환 후, 버프 남은 시간을 더해서 세팅
				float reactiveSec = guildConfig->guildBuffReactivateMin * 60.f + buidlingData->GetBuffRemainTime();
				_myBuildingData.SetBuffReactivateTime(reactiveSec);
			}
		}
	}
	break;
	case GuildBuildingNotifyType::PASSIVITY_END: // 건물 패시브 종료
	{
		if (const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig())
		{
			// 기본 재활성 시간(분)을 초로 변환하여 세팅
			float reactiveSec = guildConfig->guildBuffReactivateMin * 60.f;
			_myBuildingData.SetBuffReactivateTime(reactiveSec);
		}
	}
	break;
	case GuildBuildingNotifyType::BUFF_REACTIVATE_TIME: // 건물 패시브 재 사용 시간 종료
	{
		_myBuildingData.SetBuffReactivateTime(0.f);
	}
	break;
	}
}

void FGsGuildManager::SetOtherGuildDetailData(PD::GC::PKT_GC_ACK_GUILD_DETAIL_READ* InPacket, bool bIsRemoved)
{
	if (bIsRemoved)
	{
		if (INVALID_GUILD_DB_ID != _tempOtherGuildDBId)
		{
			// 길드리스트에 존재하는 길드라면 제거함
			RemoveJoinGuild(_tempOtherGuildDBId);
			RemoveJoinWaitingGuild(_tempOtherGuildDBId);

			_tempOtherGuildDBId = INVALID_GUILD_DB_ID;
		}

		if (_otherGuildDetailData)
		{
			_otherGuildDetailData->Empty();
		}

		if (_otherGuildBuildingData)
		{
			_otherGuildBuildingData->Empty();
		}

		return;
	}

	_tempOtherGuildDBId = INVALID_GUILD_DB_ID;

	// 길드 상세 정보
	if (nullptr == _otherGuildDetailData)
	{
		_otherGuildDetailData = new FGsGuildDataDetail();
	}
	_otherGuildDetailData->Empty();
	_otherGuildDetailData->SetOtherGuildDetail(*InPacket);

	// 건물 정보
	if (nullptr == _otherGuildBuildingData)
	{
		_otherGuildBuildingData = new FGsGuildBuildingCollectionData();
	}
	_otherGuildBuildingData->Empty();

	TArray<GuildBuildingData> tempList;
	InPacket->MakeGuildBuildingDataVector<TArray<GuildBuildingData>, GuildBuildingData>(tempList);
	for (GuildBuildingData& iter : tempList)
	{
		_otherGuildBuildingData->SetBuildingData(iter);
	}

	// 길드리스트에 존재하는 길드라면 데이터 갱신
	GuildDBId guildDBId = InPacket->GuildDetailInfo().mGuildDBId;
	for (FGsGuildData* iter : _joinGuildList)
	{
		if (guildDBId == iter->_guildDBId)
		{
			iter->SetOtherGuildDetail(*InPacket);
		}
	}
}

void FGsGuildManager::SetQuestInfo(PD::GC::PKT_GC_ACK_GUILD_QUEST_INFO_READ& InPacket)
{
	SetGuildZealPoint(InPacket.GuildZealPoint());

	if (_myGuildQuestHandler.IsValid())
	{
		_myGuildQuestHandler->SetQuestInfo(&InPacket);
	}
}

void FGsGuildManager::SetGuildZealPoint(GuildZealPoint InZealPoint)
{
	_myGuildData.SetZealPoint(InZealPoint);
}

void FGsGuildManager::SetEnemyList(PD::GC::PKT_GC_ACK_GUILD_HOSTILE_LIST_READ& InPacket)
{
	_myGuildData._enemyList.Empty();

	for (PD::GC::PKT_GC_ACK_GUILD_HOSTILE_LIST_READ::ListIterator iter = InPacket.GetFirstListIterator();
		iter != InPacket.GetLastListIterator(); ++iter)
	{
		FGsGuildDiplomacyDataEnemy data;
		data.SetListData(*iter);

		_myGuildData._enemyList.Emplace(data);
	}
}

void FGsGuildManager::SetAllyList(PD::GC::PKT_GC_ACK_GUILD_ALLY_LIST_READ& InPacket)
{
	_myGuildData._allyList.Empty();

	for (PD::GC::PKT_GC_ACK_GUILD_ALLY_LIST_READ::ListIterator iter = InPacket.GetFirstListIterator();
		iter != InPacket.GetLastListIterator(); ++iter)
	{
		FGsGuildDiplomacyDataAlly data;
		data.SetListData(*iter);

		_myGuildData._allyList.Emplace(data);
	}
}

void FGsGuildManager::SetAllyStateList(PD::GC::PKT_GC_ACK_GUILD_ALLY_REQUEST_LIST_READ& InPacket)
{
	_myGuildData._stateList.Empty();

	for (PD::GC::PKT_GC_ACK_GUILD_ALLY_REQUEST_LIST_READ::ListIterator iter = InPacket.GetFirstListIterator();
		iter != InPacket.GetLastListIterator(); ++iter)
	{
		FGsGuildDiplomacyDataState data;
		data.SetData(*iter);

		_myGuildData._stateList.Emplace(data);
	}
}

void FGsGuildManager::SetBuildingDataInter(const GuildBuildingData& InData, bool bInCheckType)
{
	_myBuildingData.SetBuildingData(InData);

	if (bInCheckType)
	{
		// 인원 수 증가, 레벨업 일 수 있으므로 내 데이터 갱신
		if (const FGsGuildBuildingData* building = _myBuildingData.GetBuildingData(InData.mBuildingId))
		{
			if (building->_tableData)
			{
				const FGsSchemaGuildBuildingLevelData* levelData = FGsGuildHelper::GetBuildingLevelData(
					building->_tableData, building->GetCurrBuildingLevel());

				if (levelData)
				{
					if (GuildStatType::GUILD_USER_NUMBER == building->_tableData->guildStatType)
					{
						_myGuildData._maxMemberNum = levelData->guildStatValue;
					}
				}
			}
		}
	}
}

void FGsGuildManager::SendRequestGuildDetail(GuildDBId InGuildDBId)
{
	_tempOtherGuildDBId = InGuildDBId;

	FGsNetSendServiceGuild::SendReqGuildDetail(InGuildDBId);
}

bool FGsGuildManager::SendRequestChangeAuthority(TArray<GuildRightsLevel>& InDataList)
{
	TArray<GuildMemberRightsbyGrade> changedList;

	uint8 gradeIndex = static_cast<uint8>(GuildMemberGradeType::MAX) - 1;
	for (int32 i = 0; i < InDataList.Num(); ++i)
	{
		GuildMemberGradeType memberGrade = static_cast<GuildMemberGradeType>(gradeIndex);
		GuildRightsLevel authority = GetAuthorityByMemberGrade(memberGrade);
		if (authority != InDataList[i])
		{
			changedList.Emplace(GuildMemberRightsbyGrade(memberGrade, InDataList[i]));
		}

		--gradeIndex;
	}

	if (0 < changedList.Num())
	{
		if (const FGsNetUserData* userData = GGameData()->GetUserData())
		{
			FGsNetSendServiceGuild::SendReqChangeAuthorityList(userData->mUserDBId, changedList);
			return true;
		}
	}

	return false;
}

void FGsGuildManager::SaveLastSettings()
{
	// _redDotCheckInfo 활용 로직 개선 필요
	GuildRedDotDetail redDotInfo;
	_redDotCheckData.SetRedDotDetailData(redDotInfo);
	
	FGsNetSendServiceGuild::SendReqGuildRedDotSave(redDotInfo);
}

IGsRedDotCheck* FGsGuildManager::GetRedDotCheck(EGsGuildRedDotType InType)
{
	if (IGsRedDotCheck** checkerPtr = _redDotCheckMap.Find(InType))
	{
		return *checkerPtr;
	}

	return nullptr;
}

const IGsRedDotCheck* FGsGuildManager::GetRedDotCheckConst(EGsGuildRedDotType InType) const
{
	if (IGsRedDotCheck*const* checkerPtr = _redDotCheckMap.Find(InType))
	{
		return *checkerPtr;
	}

	return nullptr;
}

bool FGsGuildManager::CheckRedDotAttendance() const
{
	if (IsGuild())
	{
		// 출석을 안했음
		if (false == _myGuildData._attendance)
		{
			// 가입한 당일은 출석 못하므로 검사
			if (false == _myGuildData.IsJoinedDay())
			{
				return true;
			}
		}
	}

	return false;
}

bool FGsGuildManager::CheckRedDotNoticeChanged() const
{
	if (IsGuild())
	{
		if (_redDotCheckData._noticeCheckTime < _myGuildData._noticeLastUpdateTime)
		{
			return true;
		}
	}

	return false;
}

bool FGsGuildManager::CheckRedDotBuildings() const
{
	if (IsGuild())
	{
		for (const FGsGuildBuildingData& iter : _myBuildingData._dataList)
		{
			if (CheckRedDotBuildingInter(iter))
			{
				return true;
			}
		}
	}

	return false;
}

bool FGsGuildManager::CheckRedDotBuff() const
{
	if (IsGuild())
	{
		for (const FGsGuildBuildingData& iter : _myBuildingData._dataList)
		{
			// 활성화 중인 버프가 아니면 스킵
			if (false == iter.IsBuffActive())
			{
				continue;
			}

			for (const FGsGuildPassivityData& passivityData : iter._passivityList)
			{
				if (0 == passivityData._buffEndTime)
				{
					continue;
				}

				// 버프 종료시간에서  버프지속시간을 빼면 버프가시작된시간이 나옴
				// time_t 는 초단위로 동작하는 정보이므로 그냥 빼도 무관함
				time_t buffStartTime = passivityData._buffEndTime - FMath::FloorToInt(iter.GetBuffDuration());
				if (_redDotCheckData._buffCheckTime < buffStartTime)
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool FGsGuildManager::CheckRedDotBuildingInter(const FGsGuildBuildingData& InData) const
{
	// 건설 중 레드닷이 뜨지 않도록, 건설중 체크 제외
	if (GuildBuildingStateType::GUILD_BUILDING_STATE_OPEN != InData._buildingState)
	{
		if (_redDotCheckData._buildingCheckTime < InData._lastLevelUpTime)
		{
			return true;
		}
	}

	return false;
}

void FGsGuildManager::CheckRedDotByType(EGsGuildRedDotType InType)
{
	if (IGsRedDotCheck* checker = GetRedDotCheck(InType))
	{
		checker->CheckRedDot(true);
	}
}

void FGsGuildManager::CheckAllRedDot()
{
	for (TPair<EGsGuildRedDotType, IGsRedDotCheck*>& itPair : _redDotCheckMap)
	{
		itPair.Value->CheckRedDot(true);
	}
}

bool FGsGuildManager::IsRedDotBuilding(GuildBuildingId InId) const
{
	if (const FGsGuildBuildingData* buildingData = GetBuildingData(InId))
	{
		if (CheckRedDotBuildingInter(*buildingData))
		{
			return true;
		}
	}

	return false;
}

bool FGsGuildManager::IsRedDotByType(EGsGuildRedDotType InType) const
{
	if (const IGsRedDotCheck* checker = GetRedDotCheckConst(InType))
	{
		return checker->IsRedDot();
	}

	return false;
}

int32 FGsGuildManager::GetRedDotApproval() const
{
	if (const IGsRedDotCheck* checker = GetRedDotCheckConst(EGsGuildRedDotType::Approval))
	{
		return checker->GetRedDotCount();
	}

	return 0;
}

void FGsGuildManager::OnCheckRedDot(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsPrimitiveInt32* param = StaticCast<const FGsPrimitiveInt32*>(InParam);		
	EGsGuildRedDotType type = StaticCast<EGsGuildRedDotType>(param->_data);	

	if (IGsRedDotCheck* checker = GetRedDotCheck(type))
	{
		checker->CheckRedDot(true);
	}
}

bool FGsGuildManager::IsJoinWaitingGuild(GuildDBId InGuildDBId) const
{
	for (FGsGuildData* data : _joinWaitingGuildList)
	{
		if (InGuildDBId == data->_guildDBId)
		{
			return true;
		}
	}

	return false;
}

bool FGsGuildManager::IsExistActiveBuff() const
{
	return _myBuildingData.IsExistActiveBuff();
}

bool FGsGuildManager::IsEnemyGuild(GuildDBId InGuildDBId) const
{
	if (IsGuild())
	{
		return _myGuildData.IsEnemy(InGuildDBId);
	}

	return false;
}

bool FGsGuildManager::IsAllyGuild(GuildDBId InGuildDBId) const
{
	if (IsGuild())
	{
		return _myGuildData.IsAlly(InGuildDBId);
	}

	return false;
}

bool FGsGuildManager::CheckRedDotDiplomacyEnemy()
{
	if (0 < _myGuildData._enemyList.Num())
	{
		if (_redDotCheckData._diplomacyEnemyCheckTime < _myGuildData._enemyLastUpdateTime)
		{
			return true;
		}
	}

	return false;
}

bool FGsGuildManager::CheckRedDotDiplomacyAlly()
{
	if (0 < _myGuildData._allyList.Num())
	{
		if (_redDotCheckData._diplomacyAllyCheckTime < _myGuildData._allyLastUpdateTime)
		{
			return true;
		}
	}
	
	return false;
}

bool FGsGuildManager::CheckRedDotDiplomacyState()
{
	if (0 < _myGuildData._stateList.Num())
	{
		// 권한 없으면 레드닷 뜨지 않음
		if (FGsGuildHelper::CanDiplomacy(GetAuthority()))
		{
			if (_redDotCheckData._diplomacyStateCheckTime < _myGuildData._stateLastUpdateTime)
			{	
				return true;
			}
		}
	}

	return false;
}

bool FGsGuildManager::CheckRedDotQuest() const
{
	// 내 길드가 있는지 체크
	if (false == IsGuild())
	{
		return false;
	}

	// 길드 퀘스트 가능한 레벨인지 체크
	if (const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig())
	{
		if (GetGuildLevel() < guildConfig->guildQuestOpenLevel)
		{
			return false;
		}
	}

	/*
	// 맵 이동시의 크래시 이슈로 코드 막음
	// 보상 수령 대기중인 기사단 퀘스트가 있을 경우 
	if (FGsGuildHelper::IsWaitRewardQuest())
	{
		return true;
	}
	*/

	// 마지막으로 체크한 시간 이후에 리셋된 경우
	if (_myGuildQuestHandler.IsValid())
	{
		if (_redDotCheckData._questCheckTime < _myGuildQuestHandler->GetLastQuestResetTime())
		{
			return true;
		}
	}

	return false;
}

bool FGsGuildManager::CheckRedDotItemCollection() const
{
	return _myItemCollection.IsRedDot();
}

bool FGsGuildManager::CheckRedDotStorage() const
{
	FGsGuildStorageData* myGuildStorageData = GGuild()->GetMyGuildStorageData();
	return myGuildStorageData->GetDirtyAcquired() || myGuildStorageData->GetDirtyDistributed();
	return false;
}


bool FGsGuildManager::CheckRedDotAcquisitionSetting() const
{
	return _redDotCheckData._acquisitionCheckTime < _myAuctionData.GetLastUpdateTimeAcquisition();
}

bool FGsGuildManager::CheckRedDotDivision() const
{
	return _redDotCheckData._divisionCheckTime < _myAuctionData.GetLastUpdateTimeDivision();
}

bool FGsGuildManager::CheckRedDotAuction() const
{
	return _redDotCheckData._auctionCheckTime < _myAuctionData.GetLastUpdateTimeAuction();
}

bool FGsGuildManager::CheckRedDotCalculate() const
{
	return _redDotCheckData._calculateCheckTime < _myAuctionData.GetLastUpdateTimeCalculate();
}

bool FGsGuildManager::CheckRedDotSanctum() const
{
	if (IsGuild())
	{
		if (_redDotCheckData._sanctumCheckTime < _myGuildData._sanctumLastUpdateTime)
		{
			return true;
		}
	}

	return false;
}

GuildRightsLevel FGsGuildManager::GetAuthorityByMemberGrade(GuildMemberGradeType InGrade) const
{
	for (const GuildMemberRightsbyGrade& iter : _authorityList)
	{
		if (InGrade == iter.mMemberGradeType)
		{
			return iter.mMemeberRights;
		}
	}

	return GUILD_RIGHTS_LEVEL_NONE;
}

float FGsGuildManager::GetReJoinRemainTime() const
{
	time_t second = _rejoinCoolTimeEndTime - FGsTimeSyncUtil::GetServerNowSecond();
	return (0 < second) ? second : 0;
}

Currency FGsGuildManager::GetGuildAssetAmount() const
{
	Currency Value;
	_myGuildData._guildAsset.Get(Value);
	return Value;
}

int32 FGsGuildManager::GetJoinWaitingUserCount() const
{
	if (IsGuild())
	{
		if (FGsGuildHelper::CanManageMember(GetAuthority()))
		{
			return _joinWaitingUserCount;
		}
	}

	return 0;
}

const FGsGuildCampData* FGsGuildManager::GetCampData(int32 InIndex) const
{
	if (_campDataList.IsValidIndex(InIndex))
	{
		return _campDataList[InIndex];
	}

	return nullptr;
}

const FGsGuildDataDetail* FGsGuildManager::GetGuildDetailData(bool bIsMyGuild) const
{
	return bIsMyGuild ? &_myGuildData : _otherGuildDetailData;
}

const FGsGuildBuildingData* FGsGuildManager::GetBuildingData(GuildBuildingId InId, bool bIsMyGuild) const
{
	if (bIsMyGuild)
	{
		return _myBuildingData.GetBuildingData(InId);
	}

	if (_otherGuildBuildingData)
	{
		return _otherGuildBuildingData->GetBuildingData(InId);
	}

	return nullptr;
}

void FGsGuildManager::InitializeGroupChatRoom(bool init)
{
	_groupChatRoomData.SetInitRefresh(init);
	if (init)
	{	
		_groupChatRoomData.InitGroupChatRoom(_myGuildData._guildDBId);
		_groupChatRoomData.InitInvitations();
	}

	FGsNetSendServiceGuild::SendReqGuildGroupChatList();

	if (FGsGuildHelper::CanDiplomacy(GetAuthority()))
	{
		FGsNetSendServiceGuild::SendReqGuildGroupChatInvitaionList();
	}
}

void FGsGuildManager::AddListGroupChatRooms(TArray<FGsGroupChatRoomInfo>& roomInfos)
{
	bool refresh = _groupChatRoomData.GetInitRefresh() == true;
	for (auto& info : roomInfos)
	{
		AddGroupChatRoom(info, refresh);
	}

	if (_groupChatRoomData.GetInitRefresh())
	{
		GMessage()->GetChat().SendMessage(MessageContentChat::CHATGROUP_LIST, nullptr);
	}
	_groupChatRoomData.SetInitRefresh(false);
}

void FGsGuildManager::AddGroupChatRoom(FGsGroupChatRoomInfo& roomInfo, bool refresh /*= false*/)
{
	if (_groupChatRoomData.AddGroupChatRoom(roomInfo, true))
	{
		FGsNetSendServiceGuild::SendReqGuildGroupChatMemberList(roomInfo._roomId);

		if (auto room = _groupChatRoomData.GetGroupChatRoom(roomInfo._roomId).Pin())
		{
			if (room->IsRoomMasterAuthorized())
			{
				FGsNetSendServiceGuild::SendReqGuildGroupChatInviteGuestList(roomInfo._roomId);
			}
		}

		if (refresh)
		{
			FGsUIMsgParamChatGroup param(roomInfo._roomId);
			GMessage()->GetChat().SendMessage(MessageContentChat::CHATGROUP_ADD, &param);
		}
	}
	else if (refresh)
	{
		FGsUIMsgParamChatGroup param(roomInfo._roomId);
		GMessage()->GetChat().SendMessage(MessageContentChat::CHATGROUP_INVALIDATE, &param);
	}
}

void FGsGuildManager::AddGroupChatRoom(FGsGroupChatRoomInfo&& roomInfo, bool refresh /*= false*/)
{
	FGsGroupChatRoomInfo newRoomInfo = roomInfo;
	AddGroupChatRoom(newRoomInfo, refresh);
}

void FGsGuildManager::RemoveGroupChatRoom(GuildGroupChatRoomId roomId, MessageContentChat reason /*= MessageContentChat::CHATGROUP_LEAVE*/)
{
	if (reason == MessageContentChat::CHATGROUP_KICKOUT)
	{
		FText msgFormat;
		FText::FindText(TEXT("ChatText"), TEXT("SystemChat_GroupChatKick"), msgFormat);
		if (auto room = GetGroupChatRoom(roomId).Pin())
		{
			GSChat()->OnSystemChat(FString::Format(*msgFormat.ToString(), { *room->GetGroupRoomName() }));
		}
	}
	else if (reason == MessageContentChat::CHATGROUP_LEAVE)
	{
		FText msgFormat;
		FText::FindText(TEXT("ChatText"), TEXT("SystemChat_KnightExit"), msgFormat);
		if (auto room = GGuild()->GetGroupChatRoom(roomId).Pin())
		{
			GSChat()->OnSystemChat(FString::Format(*msgFormat.ToString(), { *room->GetGroupRoomName() }));
		}
	}

	if (_groupChatRoomData.RemoveChatRoom(roomId) == false)
	{
		_groupChatRoomData.InitInvitations();

		if (FGsGuildHelper::CanDiplomacy(GetAuthority()))
		{
			FGsNetSendServiceGuild::SendReqGuildGroupChatInvitaionList();
		}
	}

	FGsUIMsgParamChatGroup param(roomId);
	GMessage()->GetChat().SendMessage(MessageContentChat::CHATGROUP_REMOVE, &param);
}

void FGsGuildManager::AddListInvitationToGroupChat(TArray<FGsGuildGroupChatInviteHost>& invitations)
{
	for (auto& invitation : invitations)
	{
		AddInvitationToGroupChat(invitation);
	}

	GMessage()->GetChat().SendMessage(MessageContentChat::CHATGROUP_INVITATION_LIST, nullptr);
}

void FGsGuildManager::AddInvitationToGroupChat(FGsGuildGroupChatInviteHost& invitation)
{
	_groupChatRoomData.AddGroupChatInvitation(invitation.GetRoomId(), invitation);
}

void FGsGuildManager::RemoveInvitationToGroupChat(GuildGroupChatRoomId roomId, FString guildName, MessageContentChat reason /*= MessageContentChat::CHATGROUP_INVITATION_ACCEPTED*/)
{
	if (reason == MessageContentChat::CHATGROUP_INVITATION_ACCEPTED)
	{
		FText Ticker_ChatGroup_JoinKnightsChatingRoom;
		//“{0} 기사단이 채팅방에 참여하였습니다“
		FText::FindText(TEXT("ChatText"), TEXT("Ticker_ChatGroup_JoinKnightsChatingRoom"), Ticker_ChatGroup_JoinKnightsChatingRoom);
		GSChat()->OnGroupChatSystemMessage(FText::Format(Ticker_ChatGroup_JoinKnightsChatingRoom, FText::FromString(guildName)).ToString(), roomId);

		FGsNetSendServiceGuild::SendReqGuildGroupChatMemberList(roomId);
	}
	else if (reason == MessageContentChat::CHATGROUP_INVITATION_REJECTED)
	{
		

	}
	else if (reason == MessageContentChat::CHATGROUP_INVITATION_CANCELD)
	{
	}

	_groupChatRoomData.RemoveGroupChatInvitation(roomId);

	FGsUIMsgParamChatGroup param(roomId);
	GMessage()->GetChat().SendMessage(MessageContentChat::CHATGROUP_INVITATION_REMOVE, &param);
}

void FGsGuildManager::AddListGroupMemberGuilds(GuildGroupChatRoomId roomId, TArray<FGsGuildGroupChatMemberGuild>& members)
{
	for (auto& member : members)
	{
		AddGroupMemberGuild(roomId, member);
	}

	FGsUIMsgParamChatGroup param(roomId);
	GMessage()->GetChat().SendMessage(MessageContentChat::CHATGROUP_MEMBER_LIST, &param);
}

void FGsGuildManager::AddGroupMemberGuild(GuildGroupChatRoomId roomId, FGsGuildGroupChatMemberGuild& member)
{
	_groupChatRoomData.AddGroupChatMemberGuild(roomId, member);
}

void FGsGuildManager::RemoveGroupMemberGuild(GuildGroupChatRoomId roomId, GuildDBId guildId, MessageContentChat reason /*= MessageContentChat::CHATGROUP_LEAVE*/)
{
	if (reason == MessageContentChat::CHATGROUP_KICKOUT)
	{
		FText msgFormat;
		FText::FindText(TEXT("ChatText"), TEXT("SystemChat_KnightKick"), msgFormat); //0} 기사단이 그룹 채팅방에서 추방되었습니다.” 
		if (auto room = GGuild()->GetGroupChatRoom(roomId).Pin())
		{
			if (auto member = room->GetMember(guildId).Pin())
			{
				GSChat()->OnGroupChatSystemMessage(FString::Format(*msgFormat.ToString(), { *member->_guildName }), roomId);
			}
		}
	}
	else if (reason == MessageContentChat::CHATGROUP_LEAVE)
	{
		FText msgFormat;
		FText::FindText(TEXT("ChatText"), TEXT("SystemChat_GroupChatExit"), msgFormat); //0} 기사단이 그룹 채팅방에서 퇴장하였습니다.” 
		if (auto room = GGuild()->GetGroupChatRoom(roomId).Pin())
		{
			if (auto member = room->GetMember(guildId).Pin())
			{
				GSChat()->OnGroupChatSystemMessage(FString::Format(*msgFormat.ToString(), { *member->_guildName }), roomId);
			}
		}
	}

	_groupChatRoomData.RemoveGroupChatMemberGuild(roomId, guildId);

	FGsUIMsgParamChatGroup param(roomId, guildId);
	GMessage()->GetChat().SendMessage(MessageContentChat::CHATGROUP_MEMBER_REMOVE, &param);
}

void FGsGuildManager::AddListInvitationGuests(GuildGroupChatRoomId roomId, TArray<FGsGuildGroupChatMemberGuild>& guests)
{
	for (auto& guest : guests)
	{
		AddInvitationGuest(roomId, guest);
	}

	FGsUIMsgParamChatGroup param(roomId);
	GMessage()->GetChat().SendMessage(MessageContentChat::CHATGROUP_GUEST_LIST, &param);
}

void FGsGuildManager::AddInvitationGuest(GuildGroupChatRoomId roomId, FGsGuildGroupChatMemberGuild& guest)
{
	_groupChatRoomData.AddGroupChatInvitationGuestGuild(roomId, guest);
}

void FGsGuildManager::RemoveInvitationGuest(GuildGroupChatRoomId roomId, GuildDBId guildId, MessageContentChat reason)
{
	if (reason == MessageContentChat::CHATGROUP_INVITATION_CANCELD)
	{
		if (guildId == INVALID_GUILD_DB_ID)
		{
			FGsNetSendServiceGuild::SendReqGuildGroupChatInviteGuestList(roomId);
			return;
		}
	}
	else if (reason == MessageContentChat::CHATGROUP_INVITATION_ACCEPTED)
	{
	}

	_groupChatRoomData.RemoveGroupChatInvitationGuestGuild(roomId, guildId);

	FGsUIMsgParamChatGroup param(roomId, guildId);
	GMessage()->GetChat().SendMessage(MessageContentChat::CHATGROUP_GUEST_REMOVE, &param);
}

void FGsGuildManager::OnNotificationGuestAdded(GuildGroupChatRoomId roomId)
{
	FGsNetSendServiceGuild::SendReqGuildGroupChatInviteGuestList(roomId);
}

void FGsGuildManager::OnNotificationEnterGroupChat(GuildGroupChatRoomId roomId)
{
	InitializeGroupChatRoom(false);
}

void FGsGuildManager::OnNotificationInvitedToGroupChat(GuildGroupChatRoomId roomId)
{
	if (FGsGuildHelper::CanDiplomacy(GetAuthority()))
	{
		FGsNetSendServiceGuild::SendReqGuildGroupChatInvitaionList();
	}
}

void FGsGuildManager::OnNotificationEntrustedWithRoomMaster(GuildGroupChatRoomId roomId, GuildDBId guildId, FString guildName)
{
	FText Popup_ChatGroup_EntrustOk;
	FText::FindText(TEXT("ChatText"), TEXT("Popup_ChatGroup_EntrustOk"), Popup_ChatGroup_EntrustOk);
	FText msg = FText::Format(Popup_ChatGroup_EntrustOk, FText::FromString(guildName));
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, msg);

	_groupChatRoomData.ChangeGroupChatRoomMaster(roomId, guildId);

	FGsUIMsgParamChatGroup param(roomId);
	GMessage()->GetChat().SendMessage(MessageContentChat::CHATGROUP_ROOMMASTER_ENTRUSTED, &param);
}

void FGsGuildManager::OnNotificaationAuthorizedChanged(GuildGroupChatRoomId roomId, GuildMemberGradeType grade)
{
	_groupChatRoomData.ChangeGroupChatRoomAuthChanged(roomId, grade);

	FGsUIMsgParamChatGroup param(roomId);
	GMessage()->GetChat().SendMessage(MessageContentChat::CHATGROUP_ROOMAUTHGRADE_CHANGED, &param);
}

int FGsGuildManager::GetGroupChatRooms(OUT TArray<TWeakPtr<FGsGuildGroupChatRoom>>& rooms)
{
	return _groupChatRoomData.GetGroupChatRooms(rooms);
}

TWeakPtr<FGsGuildGroupChatRoom> FGsGuildManager::GetGroupChatRoom(GuildGroupChatRoomId roomId)
{
	return _groupChatRoomData.GetGroupChatRoom(roomId);
}

TWeakPtr<FGsGuildGroupChatInviteHost> FGsGuildManager::GetInvitation(GuildGroupChatRoomId roomId)
{
	return _groupChatRoomData.GetInvitationToGroupChat(roomId);
}

int32 FGsGuildManager::GetSnsUrlMaxCount() const
{
	UGsDataContainManager* DataContainManager = GData();
	if (nullptr != DataContainManager)
	{
		const UGsGlobalConstant* GlobalConstant = DataContainManager->GetGlobalData();
		if (nullptr != GlobalConstant)
		{
			return GlobalConstant->_guildSnsUrlMaxCount;
		}
	}
	return 0;
}

int32 FGsGuildManager::GetSnsUrlMaxLength() const
{
	UGsDataContainManager* DataContainManager = GData();
	if (nullptr != DataContainManager)
	{
		const UGsGlobalConstant* GlobalConstant = DataContainManager->GetGlobalData();
		if (nullptr != GlobalConstant)
		{
			return GlobalConstant->_guildSnsUrlMaxLength;
		}
	}
	return 40;
}

UserDBId FGsGuildManager::GetMyUserDBId() const
{
	if (const FGsNetUserData* userData = GGameData()->GetUserData())
	{
		return userData->mUserDBId;
	}

	return INVALID_USER_DB_ID;
}

FString FGsGuildManager::GetUserName() const
{
	if (const FGsNetUserData* userData = GGameData()->GetUserData())
	{
		return userData->mUserName;
	}

	return TEXT("");
}

void FGsGuildManager::OnLocalPlayerSpawnComplete(const struct IGsMessageParam* InParam)
{
	_myStorageData.Initialize();
	_myAuctionData.Initialize();
}

const FGsSchemaGuildSnsShortcutUrl* FGsGuildManager::FindSnsLinkInfo(const FString& InSnsUrl) const
{
	const UGsTableGuildSnsShortcutUrl* SnsShortcutUrlTable = Cast<UGsTableGuildSnsShortcutUrl>(FGsSchemaGuildSnsShortcutUrl::GetStaticTable());
	if (nullptr == SnsShortcutUrlTable)
	{
		GSLOG(Error, TEXT("nullptr == SnsShortcutUrlTable"));
		return nullptr;
	}

	TArray<const FGsSchemaGuildSnsShortcutUrl*> SnsShortcutUrls;
	if (!SnsShortcutUrlTable->GetAllRows(SnsShortcutUrls))
	{
		GSLOG(Error, TEXT("SnsShortcutUrlTable->GetAllRows(SnsShortcutUrls)"));
		return nullptr;
	}

	for (const FGsSchemaGuildSnsShortcutUrl* SnsShortcutUrl : SnsShortcutUrls)
	{
		if (nullptr == SnsShortcutUrl)
		{
			GSLOG(Error, TEXT("nullptr == SnsShortcutUrl"));
			continue;
		}

		if (!InSnsUrl.StartsWith(SnsShortcutUrl->urlStartWith) && InSnsUrl != SnsShortcutUrl->urlStartWith)
		{
			continue;
		}

		if (!SnsShortcutUrl->strictTest.IsEmpty() && FRegexMatcher(SnsShortcutUrl->strictTest, InSnsUrl).FindNext())
		{
			continue;
		}

		return SnsShortcutUrl;
	}

	return nullptr;
}

const TArray<FString> FGsGuildManager::MakeSnsLinks(const bool InIsMyGuild) const
{
	TArray<FString> SnsLinks;

	const FGsGuildManager* GuildManager = GGuild();
	if (nullptr == GuildManager)
	{
		GSLOG(Warning, TEXT("nullptr == GuildManager"));
	}
	else
	{
		if (InIsMyGuild)
		{
			const FGsGuildDataMine* GuildDataMine = GuildManager->GetMyGuildData();
			if (nullptr != GuildDataMine)
			{
				SnsLinks = GuildDataMine->_SNSLinkList;
				SnsLinks.SetNum(GuildManager->GetSnsUrlMaxCount());
			}
		}
	}

	return SnsLinks;
}

static void TryToActionSnsLinkStatic(bool InSuccess, const FString& InOriginal, const FString& InChanged)
{
	const FGsGuildManager* GuildManager = GGuild();
	if (nullptr == GuildManager)
	{
		GSLOG(Warning, TEXT("nullptr == GuildManager"));
	}
	else
	{
		GuildManager->TryToActionSnsLinkDelayed(InChanged);
	}
}

void FGsGuildManager::TryToActionSnsLink(const FString& InSnsUrl) const
{
	const FGsGuildManager* GuildManager = GGuild();
	if (nullptr == GuildManager)
	{
		GSLOG(Warning, TEXT("nullptr == GuildManager"));
		return;
	}

	const FGsSchemaGuildSnsShortcutUrl* SnsShortcutUrl = GuildManager->FindSnsLinkInfo(InSnsUrl);
	if (nullptr == SnsShortcutUrl || SnsShortcutUrl->checkBanWord)
	{
		FGsTextUtil::ChangeBanSentence(InSnsUrl, &TryToActionSnsLinkStatic);
	}
	else
	{
		GuildManager->TryToActionSnsLinkDelayed(InSnsUrl);
	}
}

void FGsGuildManager::TryToActionSnsLinkDelayed(const FString& InSnsUrl) const
{
	EGsGuildSnsShortcutAction SnsShortcutAction = EGsGuildSnsShortcutAction::None;

	UGsDataContainManager* DataContainManager = GData();
	if (nullptr != DataContainManager)
	{
		const UGsGlobalConstant* GlobalConstant = DataContainManager->GetGlobalData();
		if (nullptr != GlobalConstant)
		{
			SnsShortcutAction = GlobalConstant->_guildSnsLinkUnknownAction;
		}
	}

	const FGsSchemaGuildSnsShortcutUrl* SnsShortcutUrl = FindSnsLinkInfo(InSnsUrl);
	if (nullptr == SnsShortcutUrl)
	{
		GSLOG(Log, TEXT("nullptr == SnsShortcutUrl, InSnsUrl:%s"), *InSnsUrl);
	}
	else
	{
		SnsShortcutAction = SnsShortcutUrl->click;
	}

	switch (SnsShortcutAction)
	{
	case EGsGuildSnsShortcutAction::CopyToClipboard:
	{
		// 클립보드로 복사 시도
		FPlatformApplicationMisc::ClipboardCopy(*InSnsUrl);

		FText MessageText;
		if (FText::FindText(TEXT("GuildText"), TEXT("Alert_SNS_Wrong_Form_Link"), MessageText))
		{
			FGsUIHelper::TrayMessageTicker(MessageText);
		}
		else
		{
			GSLOG(Error, TEXT("FText::FindText(TEXT(GuildText), TEXT(Alert_SNS_Wrong_Form_Link), MessageText)"));
		}
	}
	break;

	case EGsGuildSnsShortcutAction::OpenWebBrowser:
	{
		// URL 오픈 시도
		FString ErrorResult;
		FPlatformProcess::LaunchURL(*InSnsUrl, nullptr, &ErrorResult);
		if (!ErrorResult.IsEmpty())
		{
			FGsUIHelper::TrayMessageTicker(FText::FromString(ErrorResult));
		}
	}
	break;

	default:
	{
		// 그 외, 모든 경우.
		FText ErrorText;
		if (FText::FindText(TEXT("GuildText"), TEXT("Alert_SNS_No_Link"), ErrorText))
		{
			FGsUIHelper::TrayMessageTicker(ErrorText);
		}
		else
		{
			GSLOG(Error, TEXT("FText::FindText(TEXT(GuildText), TEXT(Alert_SNS_No_Link), ErrorText)"));
		}
	}
	break;

	}
}
