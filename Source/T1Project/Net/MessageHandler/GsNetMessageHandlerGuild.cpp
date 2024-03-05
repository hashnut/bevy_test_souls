// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNetMessageHandlerGuild.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

#include "Net/GsNetSendServiceGuild.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsDepotManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGame/GsCampManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"

#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Data/GsGameObjectDataPlayer.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/Data/GsGameObjectDataRemotePlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"
#include "GameObject/ObjectClass/GsGameObjectCamp.h"

#include "UI/UIContent/Tray/GsUITrayTickerJoinGuild.h"
#include "UI/UIContent/Popup/GsUIPopupCommonItemAction.h"
#include "UI/UIContent/Popup/GsUIPopupOKTwoMessage.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Message/MessageParam/GsGuildMessageParam.h"
#include "Currency/GsCurrencyHelper.h"

#include "DataSchema/Guild/GsSchemaGuildConfig.h"
#include "DataSchema/Guild/GsSchemaGuildBuilding.h"
#include "DataSchema/Guild/Agit/GsSchemaAgitConfig.h"
#include "DataSchema/Quest/Guild/GsSchemaQuestGuildStory.h"

#include "Guild/GsGuildItemCollectionData.h"
#include "Guild/GsGuildHelper.h"
#include "Guild/GsGuildStorageData.h"
#include "Guild/GsAgitDungeonRaidData.h"
#include "Item/GsItemManager.h"
#include "Quest/Table/GsQuestGuildTableCacheSet.h"

#include "GameFlow/Stage/StageGame/GsStageGameGuildDungeon.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateGuild.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/GuildDungeon/GsGuildDungeonHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Guild/GsAgitHandler.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"
#include "GameFlow/Stage/GsStageMode.h"

#include "UTIL/GsTimeSyncUtil.h"

#include "T1Project.h"


using namespace PD::SC;

void FGsNetMessageHandlerGuild::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	MProtocalWorld& world = inManager->GetProtocalWorld();
	
	InitializeMessageDelegateList(&world);
	
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_CREATE_GUILD,
		this, &FGsNetMessageHandlerGuild::CreateGuildAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_USER_GUILD_INFO,
		this, &FGsNetMessageHandlerGuild::UserGuildInfo));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_NOTIFY_USER_GUILD_INFO,
		this, &FGsNetMessageHandlerGuild::UserGuildInfoNotify));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_NOTIFY_CLEAR_USER_GUILD_INFO,
		this, &FGsNetMessageHandlerGuild::ClearUserGuildInfoNotify));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_NOTIFY_CHANGE_USER_GUILD_EMBLEM,
		this, &FGsNetMessageHandlerGuild::ChangeUserGuildEmblemNotify));	
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_CHANGED_USER_GUILD_MEMBER_RIGHTS,
		this, &FGsNetMessageHandlerGuild::ChangedUserGuildMemberRights));	
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_GUILD_DAILY_ACTIVITY,
		this, &FGsNetMessageHandlerGuild::GuildDailyActivityAck));
	
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_NOTIFY_GUILD_BUILDING,
		this, &FGsNetMessageHandlerGuild::GuildBuildingNotify));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_CHANGED_GUILD_EMBLEM,
		this, &FGsNetMessageHandlerGuild::ChangedGuildEmblemAck));

	AddMessageDelegate(world.AddRaw(SCPacketId::SC_NOTIFY_JOINED_GUILD,
		this, &FGsNetMessageHandlerGuild::JoinedGuildNotify));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_NOTIFY_LEAVED_GUILD,
		this, &FGsNetMessageHandlerGuild::LeavedGuildNotify));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_NOTIFY_LOGIN_GUILD,
		this, &FGsNetMessageHandlerGuild::LoginGuildNotify));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_NOTIFY_LOGOUT_GUILD,
		this, &FGsNetMessageHandlerGuild::LogoutGuildNotify));

	AddMessageDelegate(world.AddRaw(SCPacketId::SC_NOTIFY_REQ_JOIN_GUILD,
		this, &FGsNetMessageHandlerGuild::ReqJoinGuildNotify));

	AddMessageDelegate(world.AddRaw(SCPacketId::SC_INFORM_CHANGED_GUILD_MEMBER_GRADE,
		this, &FGsNetMessageHandlerGuild::InformChangedGuildMemberGrade));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_INFORM_CHANGED_GUILD_NOTICE,
		this, &FGsNetMessageHandlerGuild::InformChangedGuildNotice));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_GUILD_CURRENCY_UPDATE,
		this, &FGsNetMessageHandlerGuild::GuildCurrencyUpdate));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_INFORM_CHANGED_GUILD_EMBLEM,
		this, &FGsNetMessageHandlerGuild::InformChangedGuildEmblem));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_INFORM_DELEGATE_GUILD_CAPTAIN,
		this, &FGsNetMessageHandlerGuild::InformDelegateGuildCaptin));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_CHANGE_SNS_URL_GUILD,
		this, &FGsNetMessageHandlerGuild::GuildChangeSNSLinkNotify));

	// 2021/10/26 PKT - Item Collection
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_GUILD_ITEM_COLLECTION_BOOKMARK_CREATE, this, &FGsNetMessageHandlerGuild::ItemCollectionBookMarkActive));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_GUILD_ITEM_COLLECTION_BOOKMARK_DELETE, this, &FGsNetMessageHandlerGuild::ItemCollectionBookMarkInActive));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_GUILD_ITEM_COLLECTION_REGISTER, this, &FGsNetMessageHandlerGuild::ItemCollectionCompletedAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_INFORM_GUILD_ITEM_COLLECTION_REGISTER, this, &FGsNetMessageHandlerGuild::ItemCollectionRegisterNotify));

	// 창고
	//AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_GUILD_STORE_DISTRIBUTE_ITEM, this, &FGsNetMessageHandlerGuild::StorageDistributionAck));
	//AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_GUILD_STORE_EXTEND_SLOT, this, &FGsNetMessageHandlerGuild::StorageExtendSlotAck));
	//AddMessageDelegate(world.AddRaw(SCPacketId::SC_INFORM_GUILD_STORE_ACQUIRED_ITEM, this, &FGsNetMessageHandlerGuild::StorageAcquiredItemNotify));
	//AddMessageDelegate(world.AddRaw(SCPacketId::SC_INFORM_GUILD_STORE_DISTRIBUTE_ITEM, this, &FGsNetMessageHandlerGuild::StorageDistributedItemNotify));
	//AddMessageDelegate(world.AddRaw(SCPacketId::SC_INFORM_GUILD_STORE_EXTEND_SLOT, this, &FGsNetMessageHandlerGuild::StorageExtendSlotNotify));

	//길드 던전
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_FAILED_ENTER_GUILD_DUNGEON, this, &FGsNetMessageHandlerGuild::AckFailedEnterGuildDungeon));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_FAILED_LEAVE_GUILD_DUNGEON, this, &FGsNetMessageHandlerGuild::AckFailedLeaveGuildDungeon));
	//AddMessageDelegate(world.AddRaw(SCPacketId::SC_USE_UILD_DUNGEON_TICKET, this, &FGsNetMessageHandlerGuild::UseUildDungeonTicket));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_INFORM_GUILD_DUNGEON_RANK, this, &FGsNetMessageHandlerGuild::InformGuildDungeonRank));	
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_GUILD_DUNGEON_TICKET_EXPIRE_TIME, this, &FGsNetMessageHandlerGuild::GuildDungeonTicketExpireTime));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_GUILD_DUNGEON_CLEAR, this, &FGsNetMessageHandlerGuild::GuildDungeonClear));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_GUILD_DUNGEON_WARNING_TICKET_TIME_OUT, this, &FGsNetMessageHandlerGuild::GuildDungeonTicketTimeOut));

	// 경매 & 분배
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_GUILD_STORE_NOTIFY, this, &FGsNetMessageHandlerGuild::GuildStoreNotify));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_GUILD_ACQUISITION_SETTINGS, this, &FGsNetMessageHandlerGuild::SaveGuildAcquisitionNotify));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_GUILD_AUCTION_BIDDER, this, &FGsNetMessageHandlerGuild::GuildAuctionBidder));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_GUILD_AUCTION_RETURN_CURRENCY, this, &FGsNetMessageHandlerGuild::GuildAuctionReturnCurrency));

	// 기사단 영지
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_AGIT_RENTAL, 
		this, &FGsNetMessageHandlerGuild::AgitRentalAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_AGIT_RENTAL_EXTENSION, 
		this, &FGsNetMessageHandlerGuild::AgitRentalExtensionAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_AGIT_ENTER, 
		this, &FGsNetMessageHandlerGuild::AgitEnterAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_AGIT_LEAVE, 
		this, &FGsNetMessageHandlerGuild::AgitLeaveAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_OPEN_AGIT_DUNGEON_RAID, 
		this, &FGsNetMessageHandlerGuild::AgitDungeonRaidOpenAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_CLOSE_AGIT_DUNGEON_RAID,
		this, &FGsNetMessageHandlerGuild::AgitDungeonRaidCloseAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_ENTER_AGIT_DUNGEON_RAID, 
		this, &FGsNetMessageHandlerGuild::AgitDungeonRaidEnterAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_LEAVE_AGIT_DUNGEON_RAID, 
		this, &FGsNetMessageHandlerGuild::AgitDungeonRaidLeaveAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_AGIT_DUNGEON_RAID_CLEAR, 
		this, &FGsNetMessageHandlerGuild::AgitDungeonRaidClearNotify));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_AGIT_DUNGEON_RAID_BOOKMARK_LIST, 
		this, &FGsNetMessageHandlerGuild::AgitDungeonRaidBookmarkListAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_AGIT_DUNGEON_RAID_BOOKMARK_ADD, 
		this, &FGsNetMessageHandlerGuild::AgitDungeonRaidBookmarkAddAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_ACK_AGIT_DUNGEON_RAID_BOOKMARK_DELETE, 
		this, &FGsNetMessageHandlerGuild::AgitDungeonRaidBookmarkDeleteAck));
	AddMessageDelegate(world.AddRaw(SCPacketId::SC_INFORM_AGIT_DUNGEON_RAID_RANK,
		this, &FGsNetMessageHandlerGuild::AgitDungeonRaidInformRank));
}

void FGsNetMessageHandlerGuild::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerGuild::CreateGuildAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_CREATE_GUILD_READ* pkt = reinterpret_cast<PKT_SC_ACK_CREATE_GUILD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());

		if (PD::Result::GUILD_CREATE_ERROR_DUPLICATION_NAME == result)
		{
			// TEXT: 이미 존재하는 이름입니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_SameName"));
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	// 기사단 창설 했을때는 수호탑을 설치할 수 있기에 true 설정
	if (FGsCampManager* campMgr = GSCamp())
	{
		campMgr->SetIsAllowedBuildCamp(true);
	}

	// 이어 오는 SC_USER_GUILD_INFO 에서 데이터 저장, 처리
}

void FGsNetMessageHandlerGuild::UserGuildInfo(const FGsNet::Packet& InPacket)
{
	PKT_SC_USER_GUILD_INFO_READ* pkt = reinterpret_cast<PKT_SC_USER_GUILD_INFO_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsGuildManager* guildMgr = GGuild();
	bool bIsGuild = guildMgr->IsGuild();

	guildMgr->SetUserGuildInfo(*pkt);
	GGameData()->SetGuildData(*pkt);

	// 빌보드 갱신 위해 동맹/적대 정보 요청
	FGsNetSendServiceGuild::SendReqHostileList();
	FGsNetSendServiceGuild::SendReqAllyList();

	// 메시지
	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_JOIN_SUCCESS, nullptr);

	// 첫 가입 시 티커 출력
	if (false == bIsGuild)
	{
		// 로비->인게임 중엔 로딩화면에서는 출력하지 않음
		if (false == GUI()->IsActiveLoadingUI() &&
			FGsGameFlow::Mode::GAME == GMode()->GetCurrentFlowType())
		{
			// 권한이 길마일 경우 창설했다고 판단한다
			bool bIsCreate = FGsGuildHelper::IsMasterAuthority(guildMgr->GetAuthority());

			TrayJoinGuildTicker(pkt->GuildName(), pkt->GuildEmblemId(), bIsCreate);
		}

		if (UGsChatManager* chatMgr = GSChat())
		{
			chatMgr->OnCheckGuildChat();
		}
	}

	// 월드 진입도중 받을 수 있다 널체크
	if (UGsGameObjectManager* gameObjectMgr = GSGameObject())
	{
		if (UGsGameObjectBase* target = gameObjectMgr->FindObject(EGsGameObjectType::LocalPlayer))
		{
			if (UGsGameObjectLocalPlayer* localPlayer = target->CastGameObject<UGsGameObjectLocalPlayer>())
			{
				localPlayer->UpdateBillboard(EGsBillboardUpdateType::Guild);
			}
		}

		GMessage()->GetSanctum().SendMessage(MessageContentSanctum::INVALIDATE_SANCTUM_STATE, nullptr);
	}
}

void FGsNetMessageHandlerGuild::UserGuildInfoNotify(const FGsNet::Packet& InPacket)
{
	PKT_SC_NOTIFY_USER_GUILD_INFO_READ* pkt = reinterpret_cast<PKT_SC_NOTIFY_USER_GUILD_INFO_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 월드 진입도중 받을 수 있다 널체크
	if (UGsGameObjectManager* gameObjectMgr = GSGameObject())
	{
		if (UGsGameObjectBase* target = gameObjectMgr->FindObject(EGsGameObjectType::RemotePlayer, pkt->GameId()))
		{
			if (FGsGameObjectDataRemotePlayer* data = target->GetCastData<FGsGameObjectDataRemotePlayer>())
			{
				data->SetGuildData(*pkt);
			}

			if (UGsGameObjectRemotePlayer* remotePlayer = target->CastGameObject<UGsGameObjectRemotePlayer>())
			{
				remotePlayer->UpdateBillboard(EGsBillboardUpdateType::Guild);
			}
		}
	}
}

void FGsNetMessageHandlerGuild::ClearUserGuildInfoNotify(const FGsNet::Packet& InPacket)
{
	PKT_SC_NOTIFY_CLEAR_USER_GUILD_INFO_READ* pkt = reinterpret_cast<PKT_SC_NOTIFY_CLEAR_USER_GUILD_INFO_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsGameDataManager* gameDataMgr = GGameData();

	// 내가 길드에서 추방/탈퇴 되었는지
	bool bIsMyNotify = false;

	// 월드 진입 도중 문제가 될 경우, 스폰전이라 GSGameObject()->FindObject로 찾아지지 않음
	if (const FGsNetUserData* myData = gameDataMgr->GetUserData())
	{
		// 내 정보이다
		if (pkt->GameId() == myData->mGameId)
		{
			// 길드 정보 삭제
			GGuild()->RemoveAllGuildData();

			// 원래 SetClearGuildData 에 의해 불려야하지만, 스폰 전 월드 진입도중 받으면 없을 수도 있으므로 직접 지운다.
			gameDataMgr->ClearGuildData();

			// 길드 페이지를 보거나 하고 있을 경우, 창을 닫고 State 변경을 해주기 위해 메시지를 전송
			GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_LEAVE, nullptr);			

			GSChat()->OnCheckGuildChat();

			bIsMyNotify = true;
		}
	}

	// 월드 진입 중 받을 수 있으므로 널체크 수행
	if (UGsGameObjectManager* gameObjectMgr = GSGameObject())
	{
		UGsGameObjectBase* target = gameObjectMgr->FindObject(EGsGameObjectType::Player, pkt->GameId());
		if (target)
		{
			if (FGsGameObjectDataPlayer* data = target->GetCastData<FGsGameObjectDataPlayer>())
			{
				// 네임플레이트 정보 삭제
				data->SetClearGuildData();
			}

			// 네임플레이트 갱신
			if (UGsGameObjectPlayer* player = target->CastGameObject<UGsGameObjectPlayer>())
			{
				player->UpdateBillboard(EGsBillboardUpdateType::Guild);
			}
		}
		
		// 내가 길드에서 추방/탈퇴된 경우, 주변 유저 외교상태 체크 및 네임플레이트 갱신
		if (bIsMyNotify)
		{
			CheckDiplomacyRemotePlayer(true);

			// 성소 상태 갱신
			GMessage()->GetSanctum().SendMessage(MessageContentSanctum::INVALIDATE_SANCTUM_STATE, nullptr);
		}
	}
}

void FGsNetMessageHandlerGuild::ChangeUserGuildEmblemNotify(const FGsNet::Packet& InPacket)
{
	PKT_SC_NOTIFY_CHANGE_USER_GUILD_EMBLEM_READ* pkt = reinterpret_cast<PKT_SC_NOTIFY_CHANGE_USER_GUILD_EMBLEM_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 월드 진입 중 받을 수 있으므로 널체크 수행
	if (UGsGameObjectManager* gameObjectMgr = GSGameObject())
	{
		if (UGsGameObjectBase* target = gameObjectMgr->FindObject(EGsGameObjectType::Player, pkt->GameId()))
		{
			if (EGsGameObjectType::LocalPlayer == target->GetObjectType())
			{
				GGuild()->SetChangeEmblem(pkt->GuildEmblemId());

				// 문장 변경노티 SC_INFORM_CHANGED_GUILD_EMBLEM가 존재하므로 따로 메시지 보내지 않음
				//FGsGuildMsgParamSetting param(FGsGuildMsgParamSetting::EGsGuildSettingType::Emblem);
				//GMessage()->GetGuild().SendMessage(MessageContentGuild::CHANGE_GUILD_SETTING, &param);
			}

			if (FGsGameObjectDataPlayer* data = target->GetCastData<FGsGameObjectDataPlayer>())
			{
				data->SetGuildEmblemId(pkt->GuildEmblemId());
			}

			if (UGsGameObjectPlayer* player = target->CastGameObject<UGsGameObjectPlayer>())
			{
				player->UpdateBillboard(EGsBillboardUpdateType::Guild);
			}
		}
		if (UGsGameObjectBase* cmapTarget = gameObjectMgr->FindObject(EGsGameObjectType::Camp, pkt->SpawnCampGameId()))
		{
			if (UGsGameObjectCamp* campObject = cmapTarget->CastGameObject<UGsGameObjectCamp>())
			{
				campObject->SetGuildEmblemID(pkt->GuildEmblemId());
				campObject->UpdateEmblemIcon();
			}
		}
	}
}

void FGsNetMessageHandlerGuild::ChangedUserGuildMemberRights(const FGsNet::Packet& InPacket)
{
	PKT_SC_CHANGED_USER_GUILD_MEMBER_RIGHTS_READ* pkt = reinterpret_cast<PKT_SC_CHANGED_USER_GUILD_MEMBER_RIGHTS_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGuild()->SetChangedMyAuthority(pkt->GuildRightsLevel());

	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_CHANGE_MY_AUTHORITY, nullptr);

	// TEXT: 권한 정보가 수정되었습니다.
	FText textMsg;
	FText::FindText(TEXT("GuildText"), TEXT("Alert_AuthorityChange"), textMsg);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg, TrayCondition::TC_Active, SectionPriority::None);
}

void FGsNetMessageHandlerGuild::GuildDailyActivityAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_GUILD_DAILY_ACTIVITY_READ* pkt = reinterpret_cast<PKT_SC_ACK_GUILD_DAILY_ACTIVITY_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (PD::Result::GUILD_ACTIVITY_ERROR_GUILD_JOIN == result)
		{
			// TEXT: 가입 당일에는 출석과 기부를 할 수 없습니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_CannotAttendanceDonation_JoinDay"));
			return;
		}
		else if(PD::Result::GUILD_ERROR_DAILY_DONATE_OVER == result)
		{	
			if (GuildDailyActivityType::NORMAL_DONATE == pkt->GuildDailyActivityType() ||
				GuildDailyActivityType::ADVANCED_DONATE == pkt->GuildDailyActivityType())
			{
				// TEXT: 일일 기부 횟수를 모두 소진하였습니다.
				ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_DailyDonationEnd"));
				return;
			}
		}
		else if (PD::Result::GUILD_ERROR_ATTENDANCE_ALREADY == result)
		{
			if (GuildDailyActivityType::ATTENDANCE == pkt->GuildDailyActivityType())
			{
				// TEXT: 이미 출석 체크하였습니다.
				ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_MultipleAttandance"));
				return;
			}
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	// 횟수차감, 길드 운영비 관련 처리
	GGuild()->SetDailyActivity(*pkt);

	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_DAILY_ACTIVITY, nullptr);

	switch (pkt->GuildDailyActivityType())
	{
	case GuildDailyActivityType::ATTENDANCE:
	{
		// TEXT: 출석했습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_Attendace"), textMsg);
		FGsUIHelper::AddChatMessageSystem(textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg, TrayCondition::TC_Close);
	}
	break;
	case GuildDailyActivityType::NORMAL_DONATE:
	{
		// TEXT: 일반 기부를 했습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_NormalDonate"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			soundPlayer->PlaySound2D(TEXT("UI_RewardOne"));
		}
	}
	break;

	case GuildDailyActivityType::ADVANCED_DONATE:
	{
		// TEXT: 고급 기부를 했습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_AdvancedDonate"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			soundPlayer->PlaySound2D(TEXT("UI_RewardOne"));
		}
	}
	break;
	}
}

void FGsNetMessageHandlerGuild::GuildBuildingNotify(const FGsNet::Packet& InPacket)
{
	PKT_SC_NOTIFY_GUILD_BUILDING_READ* pkt = reinterpret_cast<PKT_SC_NOTIFY_GUILD_BUILDING_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGuild()->SetBuildingNotify(*pkt);

	FGsMessageHolder* messageMgr = GMessage();

	// 건물 갱신 메시지
	FGsGuildMsgParamInt param(pkt->ConstRefGuildBuildingData().mBuildingId);
	messageMgr->GetGuild().SendMessage(MessageContentGuild::RECEIVE_BUILDING_UPDATE, &param);

	// 패시비티 갱신 메시지
	FGsPrimitiveInt32 paramCollection(static_cast<int32>(PassivityOwnerType::GUILD_BUILDING));
	messageMgr->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_UPDATE_COLLECTION, &paramCollection);

	// HUD메뉴 레드닷 검사 알림
	messageMgr->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[Guild] GuildBuildingNotify: type: %d"), pkt->NotifyType());
#endif

	// 알림 출력
	const FGsSchemaGuildBuilding* tableData = FGsGuildHelper::GetGuildBuilding(
		pkt->ConstRefGuildBuildingData().mBuildingId);
	if (tableData)
	{
		if (GuildBuildingNotifyType::OPEN_CLOSE == pkt->NotifyType())
		{
			// TEXT: 기사단 건물 {0}이/가 건설 완료되었습니다.
			FText textFormat;
			if (FText::FindText(TEXT("GuildText"), TEXT("Alert_BuildingComplete"), textFormat))
			{
				FText textMsg = FText::Format(textFormat, tableData->nameText);
				FGsUIHelper::AddChatMessageSystem(textMsg);
			}
		}
		else if (GuildBuildingNotifyType::UPGRADE_END == pkt->NotifyType())
		{
			// TEXT: 기사단 건물 {0}이/가 레벨업하였습니다.
			FText textFormat;
			if (FText::FindText(TEXT("GuildText"), TEXT("Alert_LevelUpComplete"), textFormat))
			{
				FText textMsg = FText::Format(textFormat, tableData->nameText);
				FGsUIHelper::AddChatMessageSystem(textMsg);
			}
		}
		else if (GuildBuildingNotifyType::PASSIVITY_START == pkt->NotifyType())
		{
			// TEXT: 기사단 버프 {0}이/가 활성화 되었습니다.
			FText textFormat;
			if (FText::FindText(TEXT("GuildText"), TEXT("Alert_BuffStart"), textFormat))
			{
				FText textMsg = FText::Format(textFormat, tableData->nameText);

				// 티커, 시스템 채팅 모두 출력
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
				FGsUIHelper::AddChatMessageSystem(textMsg);
			}
		}
	}
}

void FGsNetMessageHandlerGuild::ChangedGuildEmblemAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_CHANGED_GUILD_EMBLEM_READ* pkt = reinterpret_cast<PKT_SC_ACK_CHANGED_GUILD_EMBLEM_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (CheckAuthorityError(result))
		{
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	// 문장 변경 저장
	GGuild()->SetChangeEmblem(pkt->GuildEmblemId());

	FGsGuildMsgParamSetting param(FGsGuildMsgParamSetting::EGsGuildSettingType::Emblem);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::CHANGE_GUILD_SETTING, &param);
}

void FGsNetMessageHandlerGuild::JoinedGuildNotify(const FGsNet::Packet& InPacket)
{
	PKT_SC_NOTIFY_JOINED_GUILD_READ* pkt = reinterpret_cast<PKT_SC_NOTIFY_JOINED_GUILD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FString userName = pkt->UserName();

	// TEXT: {0}님이 길드에 가입했습니다.
	FText textFormat;
	if (FText::FindText(TEXT("GuildText"), TEXT("Alert_NewMember"), textFormat))
	{
		FText textMsg = FText::Format(textFormat, FText::FromString(userName));
		FGsUIHelper::AddChatMessageSystem(textMsg);
		
		bool bIsMyNotify = false;
		if (const FGsNetUserData* userData = GGameData()->GetUserData())
		{
			if (userData->mUserName == userName)
			{
				bIsMyNotify = true;
			}
		}

		// 내 가입 노티인 경우엔 다른 티커를 찍으므로 이 메시지는 출력하지 않음
		if (false == bIsMyNotify)
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg, TrayCondition::TC_Close);
		}
	}

	GMessage()->GetGuild().SendMessage(MessageContentGuild::NOTIFY_JOIN_MEMBER, nullptr);
}

void FGsNetMessageHandlerGuild::LeavedGuildNotify(const FGsNet::Packet& InPacket)
{
	PKT_SC_NOTIFY_LEAVED_GUILD_READ* pkt = reinterpret_cast<PKT_SC_NOTIFY_LEAVED_GUILD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 삭제 성공 시 멤버리스트 갱신 메시지
	if (GGuild()->RemoveGuildMember(pkt->UserName()))
	{
		GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_REMOVE_MEMBER, nullptr);
	}

	// 추방/탈퇴 구분
	if (pkt->IsExpel())
	{
		// TEXT: {0}님이 기사단에서 추방되었습니다.
		FText textFormat;
		if (FText::FindText(TEXT("GuildText"), TEXT("Alert_KickOutMember"), textFormat))
		{
			FText textMsg = FText::Format(textFormat, FText::FromString(pkt->UserName()));
			FGsUIHelper::AddChatMessageSystem(textMsg);
		}
	}
	else
	{
		// TEXT: {0}님이 길드를 탈퇴하였습니다.
		FText textFormat;
		if (FText::FindText(TEXT("GuildText"), TEXT("Alert_QuitMember"), textFormat))
		{
			FText textMsg = FText::Format(textFormat, FText::FromString(pkt->UserName()));
			FGsUIHelper::AddChatMessageSystem(textMsg);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg, TrayCondition::TC_Close);
		}
	}
}

void FGsNetMessageHandlerGuild::LoginGuildNotify(const FGsNet::Packet& InPacket)
{
	PKT_SC_NOTIFY_LOGIN_GUILD_READ* pkt = reinterpret_cast<PKT_SC_NOTIFY_LOGIN_GUILD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FString strTargetPlayerName = pkt->UserName();

	// 내 정보는 찍지 않는다
	if (const FGsNetUserData* userData = GGameData()->GetUserData())
	{
		if (userData->mUserName == strTargetPlayerName)
		{
			return;
		}
	}

	GuildMemberGradeType memberGrade = pkt->MemberGrade();

	// 기사단 알림이 꺼져있으면 티커 및 채팅에 노출하지 않는다. (기사단원 접속 알림 옵션)
	if (FGsServerOption* serverOption = GServerOption())
	{
		const int bHideTicker = serverOption->GetOptionLoginAlert(EGsOptionLoginAlert::GUILD_ALERT);
		{
			if (0 == bHideTicker)
			{
				return;
			}
		}
	}

	// 일급기사 이상 등급만 티커출력
	if (GuildMemberGradeType::FIRSTKNIGHT <= memberGrade)
	{		
		// TEXT: {0} {1}님이 접속하였습니다.
		FText textFormatTicker;
		if (FText::FindText(TEXT("GuildText"), TEXT("Alert_ConnectHighMember"), textFormatTicker))
		{
			FText textMemeberGrade;
			FGsGuildHelper::GetMemberGradeText(memberGrade, textMemeberGrade);

			//FString strGrade = TEXT("<P>") + textMemeberGrade.ToString() + TEXT("</>");
			//FString strName = TEXT("<P>") + strTargetPlayerName + TEXT("</>");
			FText textMsgTicker = FText::Format(textFormatTicker, textMemeberGrade, FText::FromString(strTargetPlayerName));
			FGsUIHelper::AddChatMessageSystem(textMsgTicker);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsgTicker, TrayCondition::TC_Close, SectionPriority::None);
		}
	}
	else
	{
		// 채팅 메시지만 출력
		// TEXT: {0}님이 접속하였습니다.
		FText textFormat;
		if (FText::FindText(TEXT("GuildText"), TEXT("Alert_ConnectMember"), textFormat))
		{
			FText textMsg = FText::Format(textFormat, FText::FromString(strTargetPlayerName));
			FGsUIHelper::AddChatMessageSystem(textMsg);
		}
	}
}

void FGsNetMessageHandlerGuild::LogoutGuildNotify(const FGsNet::Packet& InPacket)
{
	PKT_SC_NOTIFY_LOGOUT_GUILD_READ* pkt = reinterpret_cast<PKT_SC_NOTIFY_LOGOUT_GUILD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// TODO: 필요한 동작이 있으면 연결
}

void FGsNetMessageHandlerGuild::ReqJoinGuildNotify(const FGsNet::Packet& InPacket)
{
	PKT_SC_NOTIFY_REQ_JOIN_GUILD_READ* pkt = reinterpret_cast<PKT_SC_NOTIFY_REQ_JOIN_GUILD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsGuildManager* guildMgr = GGuild())
	{
		guildMgr->SetJoinWaitingUserCount(pkt->RequestedJoinUserCnt());
		guildMgr->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::Approval);
	}	
	
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::NOTIFY_JOIN_REQUEST, nullptr);
}

void FGsNetMessageHandlerGuild::InformChangedGuildMemberGrade(const FGsNet::Packet& InPacket)
{
	PKT_SC_INFORM_CHANGED_GUILD_MEMBER_GRADE_READ* pkt = reinterpret_cast<PKT_SC_INFORM_CHANGED_GUILD_MEMBER_GRADE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 변경 정보 저장(내 정보일 수도 있으므로, 리스트는 없을 수도 있음)
	GGuild()->SetChangeMemberGrade(pkt->UserName(), pkt->GuildMemeberGrade());

	FString strName = pkt->UserName();
	FText textMemberGrade;
	FGsGuildHelper::GetMemberGradeText(pkt->GuildMemeberGrade(), textMemberGrade);

	// TEXT: {0}님이 {1} 이/가 되었습니다.
	FText textFormatSystem;
	FText textMsgSystem;
	if (FText::FindText(TEXT("GuildText"), TEXT("Alert_RankChange_Other"), textFormatSystem))
	{
		textMsgSystem = FText::Format(textFormatSystem, FText::FromString(strName), textMemberGrade);
		FGsUIHelper::AddChatMessageSystem(textMsgSystem);
	}

	bool bIsMyNotify = false;
	if (const FGsNetUserData* userData = GGameData()->GetUserData())
	{
		if (strName == userData->mUserName)
		{
			bIsMyNotify = true;
		}
	}

	// 내 정보일 때 메시지 티커
	if (bIsMyNotify)
	{
		// TEXT: 기사단 계급이 {0}로 변경되었습니다.
		FText textFormat;
		if (FText::FindText(TEXT("GuildText"), TEXT("Alert_RankChange_Me"), textFormat))
		{
			FText textMsg = FText::Format(textFormat, textMemberGrade);
			// SectionPriority::None으로 해야 큐잉 됨
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg, TrayCondition::TC_Active, SectionPriority::None);
		}
	}
	else
	{
		// 부단장 이상일 경우, 메시지 티커
		if (GuildMemberGradeType::SUBCAPTAIN <= pkt->GuildMemeberGrade())
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsgSystem, TrayCondition::TC_Active, SectionPriority::None);
		}
	}

	// 길마가 바뀌었거나, 길드원 목록을 보고있을 때 길드 화면 갱신을 해야하므로 메시지를 보냄
	FGsGuildMsgParamMemberGrade param(strName, pkt->GuildMemeberGrade());
	GMessage()->GetGuild().SendMessage(MessageContentGuild::NOTIFY_CHANGE_MEMBER_GRADE, &param);
}

void FGsNetMessageHandlerGuild::InformChangedGuildNotice(const FGsNet::Packet& InPacket)
{
	PKT_SC_INFORM_CHANGED_GUILD_NOTICE_READ* pkt = reinterpret_cast<PKT_SC_INFORM_CHANGED_GUILD_NOTICE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 변경된 공지 저장
	GGuild()->SetChangeGuildString(pkt->GuildNotice(), true);

	// TEXT: 기사단 공지가 변경되었습니다.
	FText textMsgSystem;
	if (FText::FindText(TEXT("GuildText"), TEXT("Alert_NoticeChange"), textMsgSystem))
	{
		FGsUIHelper::AddChatMessageSystem(textMsgSystem);
	}

	FGsMessageHolder* messageMgr = GMessage();
	// 길드 창 보고있을 경우, 레드닷 출력 위해 알림
	messageMgr->GetGuild().SendMessage(MessageContentGuild::NOTIFY_CHANGE_NOTICE, nullptr);
	// HUD메뉴 레드닷 검사 알림
	messageMgr->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);
}

void FGsNetMessageHandlerGuild::GuildCurrencyUpdate(const FGsNet::Packet& InPacket)
{
	PKT_SC_GUILD_CURRENCY_UPDATE_READ* pkt = reinterpret_cast<PKT_SC_GUILD_CURRENCY_UPDATE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsCurrencyHelper::SetGuildAsset(pkt->CurrencyType(), pkt->CurrencyFinal());
}

void FGsNetMessageHandlerGuild::InformChangedGuildEmblem(const FGsNet::Packet& InPacket)
{
	PKT_SC_INFORM_CHANGED_GUILD_EMBLEM_READ* pkt = reinterpret_cast<PKT_SC_INFORM_CHANGED_GUILD_EMBLEM_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGuild()->SetChangeEmblem(pkt->GuildEmblemId());
	GGameData()->SetChangeGuildEmblem(pkt->GuildEmblemId());

	// 월드 진입 중 노티가 오면 에러날 수 있어서 널체크
	// 네임 플레이트 갱신
	if (UGsGameObjectManager* gameObjectMgr = GSGameObject())
	{
		if (UGsGameObjectBase* target = gameObjectMgr->FindObject(EGsGameObjectType::LocalPlayer))
		{
			if (FGsGameObjectDataPlayer* data = target->GetCastData<FGsGameObjectDataPlayer>())
			{
				data->SetGuildEmblemId(pkt->GuildEmblemId());
			}

			if (UGsGameObjectPlayer* player = target->CastGameObject<UGsGameObjectPlayer>())
			{
				player->UpdateBillboard(EGsBillboardUpdateType::Guild);
			}
		}
	}
	

	// 길드 메인페이지 보고있을 경우에 갱신 위함
	FGsGuildMsgParamSetting param(FGsGuildMsgParamSetting::EGsGuildSettingType::Emblem);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::CHANGE_GUILD_SETTING, &param);
}

void FGsNetMessageHandlerGuild::InformDelegateGuildCaptin(const FGsNet::Packet& InPacket)
{
	// 패킷 읽을 필요없이 출력

	// TEXT: 기사단장이 바뀌었습니다 - 장기 미접속으로 기사단 탈퇴
	ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_ChangeGuildHead"));	
}

void FGsNetMessageHandlerGuild::GuildChangeSNSLinkNotify(const FGsNet::Packet& InPacket)
{
	PKT_SC_CHANGE_SNS_URL_GUILD_READ* pkt = reinterpret_cast<PKT_SC_CHANGE_SNS_URL_GUILD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 저장
	GGuild()->SetSNSLink(pkt->SnsId(), pkt->SnsUrl());

	// 갱신알림
	FGsGuildMsgParamSetting param(FGsGuildMsgParamSetting::EGsGuildSettingType::SNSLink);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::CHANGE_GUILD_SETTING, &param);
}

bool FGsNetMessageHandlerGuild::CheckAuthorityError(PD::Result InResult)
{
	if (PD::Result::GUILD_ERROR_GRADE == InResult)
	{
		// TEXT: 권한이 없습니다.
		ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_NoAuthority"));
		return true;
	}

	return false;
}

void FGsNetMessageHandlerGuild::ItemCollectionBookMarkActive(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_GUILD_ITEM_COLLECTION_BOOKMARK_CREATE_READ* pkt = reinterpret_cast<PKT_SC_ACK_GUILD_ITEM_COLLECTION_BOOKMARK_CREATE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGuild()->BookMarkItemCollection(pkt->CollectionId(), true);
}


void FGsNetMessageHandlerGuild::ItemCollectionBookMarkInActive(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_GUILD_ITEM_COLLECTION_BOOKMARK_DELETE_READ* pkt = reinterpret_cast<PKT_SC_ACK_GUILD_ITEM_COLLECTION_BOOKMARK_DELETE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGuild()->BookMarkItemCollection(pkt->CollectionId(), false);
}

void FGsNetMessageHandlerGuild::ItemCollectionCompletedAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_GUILD_ITEM_COLLECTION_REGISTER_READ* pkt = reinterpret_cast<PKT_SC_ACK_GUILD_ITEM_COLLECTION_REGISTER_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());

		// 2022/02/07 PKT - 이미 등록된 슬롯 or 이미 완료한 컬렉션 조건 or 이미 완료한 컬렉션
		if (PD::Result::GUILD_ITEM_COLLECTION_ERROR_ALREADY_REGISTERED_CONDITION == result
			|| PD::Result::GUILD_ITEM_COLLECTION_ALREADY_CLEAR_CONDITION == result
			|| PD::Result::GUILD_ITEM_COLLECTION_ALREADY_CLEAR_COLLECTION == result)
		{
			//이미 등록된 아이템 컬렉션 슬롯입니다.
			FText textMessageTicker;
			FText::FindText(TEXT("GuildText"), TEXT("Error_Already_Registered_Condition"), textMessageTicker);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMessageTicker);
		}
		else
		{
			FGsUIHelper::PopupNetError(result);
		}
		return;
	}

	FGsGuildManager* guildManager = GGuild();
	if (nullptr != guildManager)
	{
		guildManager->CompletedItemCollectionConditionAck(*pkt);
	}
}

void FGsNetMessageHandlerGuild::ItemCollectionRegisterNotify(const FGsNet::Packet& InPacket)
{	
	PKT_SC_INFORM_GUILD_ITEM_COLLECTION_REGISTER_READ* pkt = reinterpret_cast<PKT_SC_INFORM_GUILD_ITEM_COLLECTION_REGISTER_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGuild()->CompletedItemCollectionConditionNotify(*pkt);
}

void FGsNetMessageHandlerGuild::StorageDistributionAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_GUILD_STORE_DISTRIBUTE_ITEM_READ* pkt = reinterpret_cast<PKT_SC_ACK_GUILD_STORE_DISTRIBUTE_ITEM_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
		return;

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_INVALIDATE_ALL_DATA, nullptr);
		PD::Result result = static_cast<PD::Result>(pkt->Result());

		switch (result)
		{
		case PD::Result::GUILD_STORE_ERROR_NOT_GUILD_MEMBER_TO_DIRTIBUTE:
		case PD::Result::GUILD_STORE_ERROR_NO_GUILD_MEMBER_TO_RECEIVE_ITEM:
		{
			// 길드원이 아닙니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("GuildSafe_Popup_NotInList"));
			return;
		}
		case PD::Result::GUILD_STORE_ERROR_UNKNOWN_GUILD_ID:
		case PD::Result::GUILD_STORE_ERROR_NOT_HAVE_GRADE_TO_DISTRIBUTE:
		case PD::Result::GUILD_STORE_ERROR_GRADE:
		{
			// 분배할 권한이 없습니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("GuildSafe_Popup_NoAuthority"));
			return;
		}
		case PD::Result::GUILD_STORE_ERROR_NO_ITEM_TO_DISTRIBUTE:
		{
			// 분배할 아이템이 없습니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("GuildSafe_UI_HaveNoItem"));
			return;
		}
		case PD::Result::GUILD_STORE_ERROR_ITEM_AMOUNT:
		case PD::Result::GUILD_STORE_ERROR_EXPIRED_ITEM:
		{
			// 아이템 수량 오류
			ShowMessageTicker(TEXT("GuildText"), TEXT("GuildSafe_Popup_NotEnoughAmount"));
			return;
		}
		case PD::Result::GUILD_STORE_ERROR_TRY_AGAIN:
		{
			// 잠시 후 다시 시도(두 서버에서 요청이 들어온 경우 하나의 서버에서 온 요청만 처리)
			ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_Retry"));
			return;
		}
		default:
		{
			FGsUIHelper::PopupNetError(result);
			return;
		}
		}
	}

	// {0} 아이템이 {1} 님에게 분배되었습니다.
	const FGsSchemaItemCommon* itemData = GItem()->GetItemTableDataByTID(pkt->ItemId());
	if (nullptr == itemData)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[Guild] StorageDistributionAck : Cannot Find ItemCommonData. TID : %d"), pkt->ItemId());
#endif
		return;
	}
		
	FText findText;
	if (FText::FindText(TEXT("GuildText"), TEXT("GuildSafe_Ticker_ItemDistributed"), findText))
	{
		findText = FText::Format(findText, itemData->name, FText::FromString(pkt->UserName()));
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
	}
}

void FGsNetMessageHandlerGuild::StorageExtendSlotAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_GUILD_STORE_EXTEND_SLOT_READ* pkt = reinterpret_cast<PKT_SC_ACK_GUILD_STORE_EXTEND_SLOT_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
		return;

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_INVALIDATE_ALL_DATA, nullptr);
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		switch (result)
		{
		case PD::Result::GUILD_STORE_ERROR_UNKNOWN_GUILD_ID:
		{
			// 길드원이 아닙니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("GuildGift_Alert_NotInGuild"));
			return;
		}
		case PD::Result::GUILD_EXTEND_ERROR_NOT_ENOUGH_MONEY:
		{
			// 확장에 필요한 재화 부족
			ShowMessageTicker(TEXT("GuildText"), TEXT("Ticker_NotEnough_Cost"));
			return;
		}
		case PD::Result::GUILD_EXTEND_ERROR_MAX_SLOT:
		{
			// 더 이상 슬롯을 확장 할수 없습니다
			ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_Retry"));
			return;
		}
		default:
		{
			FGsUIHelper::PopupNetError(result);
			return;
		}
		}
		return;
	}
}

void FGsNetMessageHandlerGuild::StorageAcquiredItemNotify(const FGsNet::Packet& InPacket)
{
	// 기사단 창고 아이템 획득 알림
	if (FGsGuildStorageData* storageData = GGuild()->GetMyGuildStorageData())
	{
		storageData->SetDirtyAcquired(true, true);
	}
}

void FGsNetMessageHandlerGuild::StorageDistributedItemNotify(const FGsNet::Packet& InPacket)
{
	// 기사단 창고 아이템 분배 알림
	if (FGsGuildStorageData* storageData = GGuild()->GetMyGuildStorageData())
	{
		storageData->SetDirtyDistributed(true, true);
	}
}

void FGsNetMessageHandlerGuild::StorageExtendSlotNotify(const FGsNet::Packet& InPacket)
{
	PKT_SC_INFORM_GUILD_STORE_EXTEND_SLOT_READ* pkt = reinterpret_cast<PKT_SC_INFORM_GUILD_STORE_EXTEND_SLOT_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
		return;

	// 기사단 창고 정보 갱신
	if (FGsGuildStorageData* storageData = GGuild()->GetMyGuildStorageData())
	{
		storageData->SetInventoryInfo(pkt->TotalSlotCount(), pkt->ExtendedNumber());
	}

	// 기사단 창고 슬롯 확장 알림
	FGsGuildMsgParamStorageExtendSlot param(pkt->TotalSlotCount(), pkt->ExtendedNumber());
	GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_EXTEND_SLOT, &param);
}

bool FGsNetMessageHandlerGuild::CheckMemberError(PD::Result InResult)
{
	if (PD::Result::GUILD_GRADE_ERROR_INAVLID_MEMBER == InResult ||
		PD::Result::GUILD_ERROR_NOT_EXSIT_MEMEBER == InResult)
	{
		// TEXT: 인원 정보가 수정되었습니다.
		ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_MemberChange"));
		return true;
	}
	else if (PD::Result::GUILD_ERROR_CHANGE_MEMBER_GRADE == InResult ||
		PD::Result::GUILD_GRADE_ERROR_DO_NOT_CHANGE_GRADE_LEVEL == InResult)
	{
		// TEXT: 나보다 등급이 낮은 유저만 임명할 수 있습니다.
		ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_CannotAppoint_Rank"));
		return true;
	}
	else if (PD::Result::GUILD_ERROR_FULL_GRADE_NUM == InResult)
	{
		// TEXT: 해당 등급의 인원이 이미 최대입니다.
		ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_CannotAppoint_Full"));
		return true;
	}

	return false;
}

void FGsNetMessageHandlerGuild::CheckDiplomacyRemotePlayer(bool bInvalidateNamePlate)
{
	// 월드 진입도중 받을 수 있다 널체크
	if (UGsGameObjectManager* gameObjectMgr = GSGameObject())
	{
		TArray<UGsGameObjectBase*> remoteList = gameObjectMgr->FindObjectArray(EGsGameObjectType::RemotePlayer);
		for (UGsGameObjectBase* player : remoteList)
		{
			if (FGsGameObjectDataRemotePlayer* data = player->GetCastData<FGsGameObjectDataRemotePlayer>())
			{
				data->CheckAndSetGuildDiplomacy();

				if (bInvalidateNamePlate)
				{
					if (UGsGameObjectRemotePlayer* remotePlayer = player->CastGameObject<UGsGameObjectRemotePlayer>())
					{
						// 개선: 동맹여부 마크만 갱신
						remotePlayer->UpdateBillboard(EGsBillboardUpdateType::Guild);
						remotePlayer->UpdateBillboard(EGsBillboardUpdateType::NameColor);
					}
				}
			}
		}
	}
}

void FGsNetMessageHandlerGuild::TrayJoinGuildTicker(const FString& InGuildName, int32 InEmblemId, bool bIsCreate)
{
	// 레벨업 등 UI가 겹치는 상황을 대비해서 큐로 넣는다
	GUI()->PushQueue(TEXT("TrayTickerJoinGuild"), 0, [InGuildName, InEmblemId, bIsCreate](UGsUIWidgetBase* InWidget)
		{
			if (InWidget)
			{
				if (UGsUITrayTickerJoinGuild* trayTicker = Cast<UGsUITrayTickerJoinGuild>(InWidget))
				{
					trayTicker->SetData(InGuildName, InEmblemId, bIsCreate);
				}
			}
		});
}

void FGsNetMessageHandlerGuild::ShowMessageTicker(const FTextKey& InNamespace, const FTextKey& InKey)
{
	FText textMsg;
	if (FText::FindText(InNamespace, InKey, textMsg))
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	}	
}

void FGsNetMessageHandlerGuild::ShowMessageTicker(const FString& InFormat)
{
	if (false == InFormat.IsEmpty())
	{
		FText textMsg;
		textMsg = FText::FromString(InFormat);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	}
}

void FGsNetMessageHandlerGuild::PopupOkTwoMessage(const FText& InTextFirst, const FText& InTextSecond)
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupOKTwoMessage"));
	if (widget.IsValid())
	{
		if (UGsUIPopupOKTwoMessage* popup = Cast<UGsUIPopupOKTwoMessage>(widget.Get()))
		{
			popup->SetData(InTextFirst, InTextSecond);
		}
	}	
}

void FGsNetMessageHandlerGuild::PopupOkTwoMessage(StoryId InStoryId, const FText& InTextSecond)
{
	TSharedPtr<FGsQuestGuildStoryTableCache> table = GSQuest()->GetQuestGuildStoryTableCache(InStoryId);
	if (false == table.IsValid())
	{
		return;
	}

	if (table->_schemaStoryItem)
	{
		PopupOkTwoMessage(table->_schemaStoryItem->questTitleText, InTextSecond);
	}
}

void FGsNetMessageHandlerGuild::AckFailedEnterGuildDungeon(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_FAILED_ENTER_GUILD_DUNGEON_READ* pkt = reinterpret_cast<PKT_SC_ACK_FAILED_ENTER_GUILD_DUNGEON_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS == pkt->Result())
	{
		return;
	}

	//길드 던전 입장 실패시 다시 인터렉션 가능하다록 처리
	if (FGsInteractionManager* interactionManager = GSInteraction())
	{
		interactionManager->SetGuildDungeonWarpReq(false);
	}

	PD::Result result = static_cast<PD::Result>(pkt->Result());	

	switch (result)
	{
	case PD::Result::GUILD_DUNGEON_WARP_ERROR_NOT_ENOUGH_TICKET:
	{
		ShowMessageTicker(TEXT("GuildText"), TEXT("Dungeon_CanNotJoin_NoTicket"));
		return;
	}
	case PD::Result::GUILD_DUNGEON_WARP_ERROR_IN_GUILD_DUNGEON_MAP:
	{
		ShowMessageTicker(TEXT("GuildText"), TEXT("Dungeon_CanNotJoin_inDungeon"));
		return;
	}
	case PD::Result::GUILD_DUNGEON_WARP_ERROR_STORE_FULL:
	{
		ShowMessageTicker(TEXT("GuildText"), TEXT("Dungeon_CanNotJoin_StorageFull"));
		return;
	}
	case PD::Result::GUILD_DUNGEON_WARP_ERROR_DONT_DESTROY:
	{
		ShowMessageTicker(TEXT("GuildText"), TEXT("Dungeon_CanNotJoin_NotClear"));
		return;
	}
	case PD::Result::GUILD_DUNGEON_WARP_ERROR_ABNORMALITY_EFFECT:
	{
		ShowMessageTicker(TEXT("GuildText"), TEXT("Dungeon_CanNotJoin_Abnormality"));
		return;
	}	
	case PD::Result::GUILD_DUNGEON_RESET_TIME:
	{
		// 길드원이 아닙니다.
		ShowMessageTicker(TEXT("GuildText"), TEXT("Dungeon_CanNotJoin_initialization"));
		return;
	}
	case PD::Result::GUILD_DUNGEON_WARP_ERROR_ENOUGH_GUILD_LEVEL:
	{
		// 길드원이 아닙니다.
		FText textMsg;
		if (FText::FindText(TEXT("GuildText"), TEXT("Dungeon_CanNotJoin_GuildLevel"), textMsg))
		{
			const FGsSchemaGuildConfig* configData = FGsGuildHelper::GetGuildConfig();
			ShowMessageTicker(FString::Format(*textMsg.ToString(), { configData->dungeonEnterLevel }));
		}
		return;
	}
	case PD::Result::GUILD_DUNGEON_WARP_ERROR_THE_DAY_JOINED_GUILD:
	{
		// 길드원이 아닙니다.
		ShowMessageTicker(TEXT("GuildText"), TEXT("Dungeon_CanNotJoin_7days"));
		return;
	}
	case PD::Result::GUILD_DUNGEON_WARP_ERROR_ANOTHER_GUILD_LEVEL:
	{
		// 길드원이 아닙니다.
		ShowMessageTicker(TEXT("GuildText"), TEXT("Dungeon_CanNotJoin_AlreadyClear"));
		return;
	}
	default:
	{
		// 길드원이 아닙니다.
		ShowMessageTicker(TEXT("GuildText"), TEXT("Dungeon_CanNotJoin"));
		return;
	}
	}

	//FGsUIHelper::PopupNetError(result);
}

void FGsNetMessageHandlerGuild::AckFailedLeaveGuildDungeon(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_FAILED_LEAVE_GUILD_DUNGEON_READ* pkt = reinterpret_cast<PKT_SC_ACK_FAILED_LEAVE_GUILD_DUNGEON_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());

		//if (PD::Result::GUILD_CREATE_ERROR_DUPLICATION_NAME == result)
		//{
		//	// TEXT: 이미 존재하는 이름입니다.
		//	ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_SameName"));
		//	return;
		//}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	//Leave guild dungeon
}

//void FGsNetMessageHandlerGuild::UseUildDungeonTicket(const FGsNet::Packet& InPacket)
//{
//	PKT_SC_USE_UILD_DUNGEON_TICKET_READ* pkt = reinterpret_cast<PKT_SC_USE_UILD_DUNGEON_TICKET_READ*>(InPacket.Buffer);
//	if (nullptr == pkt)
//	{
//		return;
//	}
//
//	int32 ticketCount = pkt->FinalTicketCount();
//
//	//get user data
//
//	//set user guild data
//}

void FGsNetMessageHandlerGuild::InformGuildDungeonRank(const FGsNet::Packet& InPacket)
{
	PKT_SC_INFORM_GUILD_DUNGEON_RANK_READ* pkt = reinterpret_cast<PKT_SC_INFORM_GUILD_DUNGEON_RANK_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}	
	
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsStageManagerGame* stageMgr = gameFlow->GetStageManagerGame();
	if (nullptr == stageMgr)
	{
		return;
	}

	if (stageMgr->GetCurrentStageMode() != FGsStageMode::Game::GUILD_DUNGEON)
	{
		return;
	}

	TSharedPtr<FGsStageGameBase> stage = stageMgr->Find(FGsStageMode::Game::GUILD_DUNGEON);
	FGsStageGameGuildDungeon* guildDungeon = static_cast<FGsStageGameGuildDungeon*>(stage.Get());
	if (nullptr == guildDungeon)
	{
		return;
	}

	guildDungeon->SetContributionArray(pkt);
}

void FGsNetMessageHandlerGuild::GuildDungeonTicketExpireTime(const FGsNet::Packet& InPacket)
{
	PKT_SC_GUILD_DUNGEON_TICKET_EXPIRE_TIME_READ* pkt = reinterpret_cast<PKT_SC_GUILD_DUNGEON_TICKET_EXPIRE_TIME_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FDateTime expireTime = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(pkt->ExpireTime());
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{		
		return;
	}

	FGsStageManagerGame* stageMgr = gameFlow->GetStageManagerGame();
	if (nullptr == stageMgr)
	{	
		return;
	}

	if (stageMgr->GetCurrentStageMode() != FGsStageMode::Game::GUILD_DUNGEON)
	{	
		return;
	}

	TSharedPtr<FGsStageGameBase> stage = stageMgr->Find(FGsStageMode::Game::GUILD_DUNGEON);
	FGsStageGameGuildDungeon* guildDungeon = static_cast<FGsStageGameGuildDungeon*>(stage.Get());
	if (nullptr == guildDungeon)
	{
		return;
	}

	guildDungeon->SetExpireTime(expireTime);		
}

void FGsNetMessageHandlerGuild::GuildDungeonClear(const FGsNet::Packet& InPacket)
{
	PKT_SC_GUILD_DUNGEON_CLEAR_READ* pkt = reinterpret_cast<PKT_SC_GUILD_DUNGEON_CLEAR_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FDateTime clearTime = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(pkt->ClearTimeSec());
	int32 dungeonLevel = pkt->DungeonLevel();

	TArray<ItemIdAmountPair> guildRewardList;
	pkt->MakeGetRewardItemsGuildVector<TArray<ItemIdAmountPair>, ItemIdAmountPair>(guildRewardList);

	TArray<CurrencyPair> guildRewardCurrencyList;
	pkt->MakeGetRewardCurrencyListGuildVector<TArray<CurrencyPair>, CurrencyPair>(guildRewardCurrencyList);

	TArray<ItemIdAmountPair> personRewardList;
	pkt->MakeGetRewardItemsPersonVector<TArray<ItemIdAmountPair>, ItemIdAmountPair>(personRewardList);	

	TArray<CurrencyPair> personRewardCurrencyList;
	pkt->MakeGetRewardCurrencyListPersonVector<TArray<CurrencyPair>, CurrencyPair>(personRewardCurrencyList);

	FGsGuildDungeonClearMsgParam param(clearTime, dungeonLevel, guildRewardList, personRewardList, guildRewardCurrencyList, personRewardCurrencyList);
	GMessage()->GetGuildDungeon().SendMessage(MessageContentGuildDungeon::CLEAR, &param);
}

void FGsNetMessageHandlerGuild::GuildDungeonTicketTimeOut(const FGsNet::Packet& InPacket)
{
	PKT_SC_GUILD_DUNGEON_WARNING_TICKET_TIME_OUT_READ* pkt = reinterpret_cast<PKT_SC_GUILD_DUNGEON_WARNING_TICKET_TIME_OUT_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FText findText;
	FText::FindText(TEXT("GuildText"), TEXT("Dungeon_KickOut_NoRemainTime"), findText);

	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
}

void FGsNetMessageHandlerGuild::GuildStoreNotify(const FGsNet::Packet& InPacket)
{
	PKT_SC_GUILD_STORE_NOTIFY_READ* pkt = reinterpret_cast<PKT_SC_GUILD_STORE_NOTIFY_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	int32 itemCount = pkt->ItemCount();
	FString userName = pkt->UserName();
	const GuildStoreItemData&  guildStoreItemData = pkt->ConstRefGuildStoreItemData();
	GuildStoreItemNotifyType notifyType = (GuildStoreItemNotifyType)pkt->NotifyType();
	switch (notifyType)
	{
	case GuildStoreItemNotifyType::REGISTER: // 등록
	{ 
		// TEXT: {0}개 아이템이 기사단 경매에 등록되었습니다.
		FText findText;
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_Notify"), findText);
		FText textMsg = FText::Format(findText, itemCount);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

		// 분배 목록 정보 초기화
		auctionData->ResetDivisionList();

		// 분배 탭일 경우 바로 정보 요청 및 UI 갱신
		// 다른 탭일 경우 분배 탭 들어갈 때 정보 재요청
		GMessage()->GetGuild().SendMessage(MessageContentGuild::DIVISION_REGISTER, nullptr);
		
		// 레드닷 설정
		auctionData->SetLastUpdateTimeAuction(FGsTimeSyncUtil::GetServerNowSecond());
		guildManager->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::Auction);

		// 레드닷 경매 탭 및 HUD 갱신 요청
		GMessage()->GetGuild().SendMessage(MessageContentGuild::AUCTION_INVALIDATE_REDDOT, nullptr);
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);
		break;
	}
	case GuildStoreItemNotifyType::NORMAL: // 입찰
	{
		if (const FGsSchemaItemCommon* itemCommonData = UGsItemManager::GetItemTableDataByTID(guildStoreItemData.mItemDataEquip.mTemplateId))
		{
			// TEXT: {0}님이 기사단 경매에서 {1}에 상위 입찰했습니다.
			FText findText;
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Bid_Success_Notify"), findText);
			FText textMsg = FText::Format(findText, FText::FromString(userName), itemCommonData->name);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

			// 경매 아이템 갱신
			TArray<GuildStoreItemData> updateItemData;
			updateItemData.Emplace(guildStoreItemData);

			auctionData->UpdateAuctionItemList(updateItemData);
			auctionData->SetAuctionBidder(guildStoreItemData.mGuildAuctionId, userName);

			// 분배 탭일 때 UI 갱신
			FGsGuildMsgParamUInt64 param(guildStoreItemData.mGuildAuctionId);
			GMessage()->GetGuild().SendMessage(MessageContentGuild::AUCTION_UPDATE_ITEM, &param);
		}
		break;
	}
	case GuildStoreItemNotifyType::DONE: // 경매 완료
	{
		TArray<GuildStoreItemData> doneItemData;
		doneItemData.Emplace(guildStoreItemData);

		auctionData->UpdateDivisionItemList(doneItemData);
		auctionData->UpdateAuctionItemList(doneItemData);
		auctionData->SetAuctionBidder(guildStoreItemData.mGuildAuctionId, userName);
		auctionData->SetDivisionBidder(guildStoreItemData.mItemDataEquip.mItemDBId, userName);

		FGsGuildMsgParamUInt64 paramAuction(guildStoreItemData.mGuildAuctionId);
		GMessage()->GetGuild().SendMessage(MessageContentGuild::AUCTION_REMOVE_ITEM, &paramAuction);

		FGsGuildMsgParamUInt64 paramDivision(guildStoreItemData.mItemDataEquip.mItemDBId);
		GMessage()->GetGuild().SendMessage(MessageContentGuild::DIVISION_UPDATE_ITEM, &paramDivision);

		// 레드닷 설정
		auctionData->SetLastUpdateTimeCalculate(FGsTimeSyncUtil::GetServerNowSecond());
		guildManager->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::GuildCalculate);

		// 레드닷 경매 탭 및 HUD 갱신 요청
		GMessage()->GetGuild().SendMessage(MessageContentGuild::AUCTION_INVALIDATE_REDDOT, nullptr);
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);
		break;
	}
	case GuildStoreItemNotifyType::ACQUISITION: // 획득
	{
		// 레드닷 설정
		auctionData->SetLastUpdateTimeDivision(FGsTimeSyncUtil::GetServerNowSecond());
		guildManager->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::Division);

		// 레드닷 경매 탭 및 HUD 갱신 요청
		GMessage()->GetGuild().SendMessage(MessageContentGuild::DIVISION_INVALIDATE_REDDOT, nullptr);
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);
		break;
	}
	case GuildStoreItemNotifyType::ACQUISITION_SETTING: // 획득 설정
	{
		// TEXT: {0}님이 기사단 획득 설정을 변경했습니다.
		FText findText;
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Alert_Acquisition_Modify"), findText);
		FText textMsg = FText::Format(findText, FText::FromString(userName));
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		break;
	}
	case GuildStoreItemNotifyType::STORAGE_TIME_AUTO_AUCTION: // 보관시간초과로 인한 자동 경매 시작
	{
		if (const FGsSchemaItemCommon* itemCommonData = UGsItemManager::GetItemTableDataByTID(guildStoreItemData.mItemDataEquip.mTemplateId))
		{
			// TEXT: 보관 시간이 초과되어 {아이템명}의 일반 경매가 시작됩니다.
			FText findText;
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Exceed_List_Time_Limit_Notify"), findText);
			FText textMsg = FText::Format(findText, itemCommonData->name);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}

		// 분배 아이템 갱신
		TArray<GuildStoreItemData> updateItemData;
		updateItemData.Emplace(guildStoreItemData);

		auctionData->UpdateDivisionItemList(updateItemData);
		auctionData->RemoveCheckedItemAll();

		FGsGuildMsgParamUInt64 paramDivision(guildStoreItemData.mItemDataEquip.mItemDBId);
		GMessage()->GetGuild().SendMessage(MessageContentGuild::DIVISION_UPDATE_ITEM, &paramDivision);

		// 레드닷 설정
		auctionData->SetLastUpdateTimeAuction(FGsTimeSyncUtil::GetServerNowSecond());
		guildManager->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::Auction);

		// 레드닷 경매 탭 및 HUD 갱신 요청
		GMessage()->GetGuild().SendMessage(MessageContentGuild::AUCTION_INVALIDATE_REDDOT, nullptr);
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);
		break;
	}
	case GuildStoreItemNotifyType::ACQUISITION_AUTO_AUCTION: // 획득내역초과로 인한 자동 경매 시작
	{
		if (const FGsSchemaItemCommon* itemCommonData = UGsItemManager::GetItemTableDataByTID(guildStoreItemData.mItemDataEquip.mTemplateId))
		{
			// TEXT: 획득 내역이 초과되어 {아이템명}의 일반 경매가 시작됩니다.
			FText findText;
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Exceed_List_Count_Limit_Notify"), findText);
			FText textMsg = FText::Format(findText, itemCommonData->name);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}

		// 분배 아이템 갱신
		TArray<GuildStoreItemData> updateItemData;
		updateItemData.Emplace(guildStoreItemData);

		auctionData->UpdateDivisionItemList(updateItemData);
		auctionData->RemoveCheckedItemAll();

		FGsGuildMsgParamUInt64 paramDivision(guildStoreItemData.mItemDataEquip.mItemDBId);
		GMessage()->GetGuild().SendMessage(MessageContentGuild::DIVISION_UPDATE_ITEM, &paramDivision);

		// 레드닷 설정
		auctionData->SetLastUpdateTimeAuction(FGsTimeSyncUtil::GetServerNowSecond());
		guildManager->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::Auction);

		// 레드닷 경매 탭 및 HUD 갱신 요청
		GMessage()->GetGuild().SendMessage(MessageContentGuild::AUCTION_INVALIDATE_REDDOT, nullptr);
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);
		break;
	}
	case GuildStoreItemNotifyType::FAILED:
	{
		TArray<GuildStoreItemData> failedItemData;
		failedItemData.Emplace(guildStoreItemData);

		auctionData->UpdateDivisionItemList(failedItemData);
		auctionData->UpdateAuctionItemList(failedItemData);
		auctionData->SetAuctionBidder(guildStoreItemData.mGuildAuctionId, userName);
		auctionData->SetDivisionBidder(guildStoreItemData.mItemDataEquip.mItemDBId, userName);

		FGsGuildMsgParamUInt64 paramAuction(guildStoreItemData.mGuildAuctionId);
		GMessage()->GetGuild().SendMessage(MessageContentGuild::AUCTION_REMOVE_ITEM, &paramAuction);

		FGsGuildMsgParamUInt64 paramDivision(guildStoreItemData.mItemDataEquip.mItemDBId);
		GMessage()->GetGuild().SendMessage(MessageContentGuild::DIVISION_UPDATE_ITEM, &paramDivision);

		// 레드닷 설정
		auctionData->SetLastUpdateTimeCalculate(FGsTimeSyncUtil::GetServerNowSecond());
		guildManager->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::GuildCalculate);

		// 레드닷 경매 탭 및 HUD 갱신 요청
		GMessage()->GetGuild().SendMessage(MessageContentGuild::AUCTION_INVALIDATE_REDDOT, nullptr);
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);
		break;
	}
	case GuildStoreItemNotifyType::DIRECT:
	{
		if (const FGsSchemaItemCommon* itemCommonData = UGsItemManager::GetItemTableDataByTID(guildStoreItemData.mItemDataEquip.mTemplateId))
		{
			// TEXT: {0} 아이템이 {1} 님에게 지정 지급 되었습니다.
			FText findText;
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_Notify_Appointment_Provide"), findText);
			FText textMsg = FText::Format(findText, itemCommonData->name, FText::FromString(userName));
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

			// 분배 목록 정보 초기화
			auctionData->ResetDivisionList();

			// 분배 탭일 경우 바로 정보 요청 및 UI 갱신
			// 다른 탭일 경우 분배 탭 들어갈 때 정보 재요청
			GMessage()->GetGuild().SendMessage(MessageContentGuild::DIVISION_REGISTER, nullptr);

			// 레드닷 설정
			auctionData->SetLastUpdateTimeCalculate(FGsTimeSyncUtil::GetServerNowSecond());
			guildManager->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::GuildCalculate);

			// 레드닷 경매 탭 및 HUD 갱신 요청
			GMessage()->GetGuild().SendMessage(MessageContentGuild::AUCTION_INVALIDATE_REDDOT, nullptr);
			GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);
		}
		break;
	}
	}
}

void FGsNetMessageHandlerGuild::SaveGuildAcquisitionNotify(const FGsNet::Packet& InPacket)
{
	PKT_SC_GUILD_ACQUISITION_SETTINGS_READ* pkt = reinterpret_cast<PKT_SC_GUILD_ACQUISITION_SETTINGS_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// CG_SAVE_GUILD_ACQUISITION_SETTINGS 로 획득 설정이 변경되면 이 패킷이 기사단원 모두에게 보내진다.
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	if (guildManager->GetGuildDBId() == pkt->ConstRefAcquisitionSettings().mGuildDBId)
	{
		auctionData->SetAcquisitionSettings(pkt->ConstRefAcquisitionSettings());

		// 열려있던 획득 설정 팝업 닫기
		GUI()->CloseByKeyName(TEXT("PopupGuildOption"));

		// 레드닷 설정
		auctionData->SetLastUpdateTimeAcquisition(FGsTimeSyncUtil::GetServerNowSecond());
		guildManager->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::AcquisitionSetting);

		// 레드닷 운영 탭 및 HUD 갱신 요청
		FGsGuildMsgParamSetting param(FGsGuildMsgParamSetting::EGsGuildSettingType::Acquisition);
		GMessage()->GetGuild().SendMessage(MessageContentGuild::CHANGE_GUILD_SETTING, &param);
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);
	}
}

void FGsNetMessageHandlerGuild::GuildAuctionBidder(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_GUILD_AUCTION_BIDDER_READ* pkt = reinterpret_cast<PKT_SC_ACK_GUILD_AUCTION_BIDDER_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	// 에러 처리만 진행된다. 입찰 결과는 전체 노티파이를 통해 처리된다. (SC_GUILD_STORE_NOTIFY)
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		auctionData->CheckPacketError(pkt->Result());

		GMessage()->GetGuild().SendMessage(MessageContentGuild::AUCTION_LIST_RESET, nullptr);
		return;
	}
}

void FGsNetMessageHandlerGuild::GuildAuctionReturnCurrency(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();
	PKT_SC_ACK_GUILD_AUCTION_RETURN_CURRENCY_READ* pkt = reinterpret_cast<PKT_SC_ACK_GUILD_AUCTION_RETURN_CURRENCY_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;
	
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		auctionData->CheckPacketError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	// TEXT: 입찰금이 회수되었습니다.
	FText textMsg;
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Withdraw_Notify"), textMsg);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

	// 회수금 초기화
	auctionData->SetRemainPayback(0);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::AUCTION_PAYBACK_UPDATED, nullptr);
}

void FGsNetMessageHandlerGuild::AgitRentalAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_AGIT_RENTAL_READ* pkt = reinterpret_cast<PKT_SC_ACK_AGIT_RENTAL_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{	
		if (FGsAgitHandler::CheckAgitError(pkt->Result()))
		{
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (FGsAgitDataController* agitcontroller = guildMgr->GetAgitDataController())
		{
			agitcontroller->SetRentalEndDate(pkt->ExpireTime());
		}
	}

	GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_STATE_CHANGED, nullptr);
}

void FGsNetMessageHandlerGuild::AgitRentalExtensionAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_AGIT_RENTAL_EXTENSION_READ* pkt = reinterpret_cast<PKT_SC_ACK_AGIT_RENTAL_EXTENSION_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		if (FGsAgitHandler::CheckAgitError(pkt->Result()))
		{
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (FGsAgitDataController* agitcontroller = guildMgr->GetAgitDataController())
		{
			agitcontroller->SetRentalEndDate(pkt->ExpireTime());
		}
	}

	GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_STATE_CHANGED, nullptr);
}

void FGsNetMessageHandlerGuild::AgitEnterAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_AGIT_ENTER_READ* pkt = reinterpret_cast<PKT_SC_ACK_AGIT_ENTER_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		if (FGsAgitHandler::CheckAgitError(pkt->Result()))
		{
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	// 맵 이동이라 따로 처리할 것은 없다. 블락이나 광클 처리 필요
}

void FGsNetMessageHandlerGuild::AgitLeaveAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_AGIT_LEAVE_READ* pkt = reinterpret_cast<PKT_SC_ACK_AGIT_LEAVE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		if (FGsAgitHandler::CheckAgitError(pkt->Result()))
		{
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	// 맵 이동이라 따로 처리할 것은 없다. 블락이나 광클 처리 필요
}

void FGsNetMessageHandlerGuild::AgitDungeonRaidOpenAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_OPEN_AGIT_DUNGEON_RAID_READ* pkt = reinterpret_cast<PKT_SC_ACK_OPEN_AGIT_DUNGEON_RAID_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		if (FGsAgitHandler::CheckAgitDungeonError(pkt->Result()))
		{
			return;
		}

		if (FGsAgitHandler::CheckAgitError(pkt->Result()))
		{
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (FGsAgitDataController* agitController = guildMgr->GetAgitDataController())
		{
			if (FGsAgitDungeonRaidData* raidData = agitController->GetRaidData())
			{
				raidData->SetRaidState(pkt->RaidId(), AgitDungeonRaidState::OPEN);
			}
		}
	}

	FGsGuildMsgParamAgitDungeonState paramState(pkt->RaidId(), AgitDungeonRaidState::OPEN, false);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_DUNGEON_RAID_OPEN_CHANGED, &paramState);

	FGsUIHelper::HideBlockUI();
}

void FGsNetMessageHandlerGuild::AgitDungeonRaidCloseAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_CLOSE_AGIT_DUNGEON_RAID_READ* pkt = reinterpret_cast<PKT_SC_ACK_CLOSE_AGIT_DUNGEON_RAID_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		if (FGsAgitHandler::CheckAgitDungeonError(pkt->Result()))
		{
			return;
		}

		if (FGsAgitHandler::CheckAgitError(pkt->Result()))
		{
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (FGsAgitDataController* agitController = guildMgr->GetAgitDataController())
		{
			if (FGsAgitDungeonRaidData* raidData = agitController->GetRaidData())
			{
				raidData->SetRaidState(pkt->RaidId(), AgitDungeonRaidState::CLOSED);
			}
		}
	}

	FGsGuildMsgParamAgitDungeonState paramState(pkt->RaidId(), AgitDungeonRaidState::CLOSED, false);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_DUNGEON_RAID_OPEN_CHANGED, &paramState);

	FGsUIHelper::HideBlockUI();
}

void FGsNetMessageHandlerGuild::AgitDungeonRaidEnterAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_ENTER_AGIT_DUNGEON_RAID_READ* pkt = reinterpret_cast<PKT_SC_ACK_ENTER_AGIT_DUNGEON_RAID_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		if (FGsAgitHandler::CheckAgitDungeonError(pkt->Result()))
		{
			return;
		}

		if (FGsAgitHandler::CheckAgitError(pkt->Result()))
		{
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (FGsAgitDataController* agitController = guildMgr->GetAgitDataController())
		{
			if (FGsAgitDungeonRaidData* raidData = agitController->GetRaidData())
			{
				raidData->SetEnterRaidId(pkt->RaidId());
			}
		}
	}
}

void FGsNetMessageHandlerGuild::AgitDungeonRaidLeaveAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_LEAVE_AGIT_DUNGEON_RAID_READ* pkt = reinterpret_cast<PKT_SC_ACK_LEAVE_AGIT_DUNGEON_RAID_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		if (FGsAgitHandler::CheckAgitDungeonError(pkt->Result()))
		{
			return;
		}

		if (FGsAgitHandler::CheckAgitError(pkt->Result()))
		{
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}
}

void FGsNetMessageHandlerGuild::AgitDungeonRaidClearNotify(const FGsNet::Packet& InPacket)
{
	PKT_SC_AGIT_DUNGEON_RAID_CLEAR_READ* pkt = reinterpret_cast<PKT_SC_AGIT_DUNGEON_RAID_CLEAR_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (FGsAgitDataController* agitController = guildMgr->GetAgitDataController())
		{
			if (FGsAgitDungeonRaidData* raidData = agitController->GetRaidData())
			{
				raidData->SetRaidState(pkt->RaidId(), AgitDungeonRaidState::CLEAR);
			}
		}
	}

	FDateTime clearTime = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(pkt->ClearTimeSec());
	int32 dungeonLevel = pkt->RaidId();

	TArray<ItemIdAmountPair> guildRewardList;
	pkt->MakeGetRewardItemsGuildVector<TArray<ItemIdAmountPair>, ItemIdAmountPair>(guildRewardList);
	TArray<CurrencyPair> guildRewardCurrencyList;
	pkt->MakeGetRewardCurrencyListGuildVector<TArray<CurrencyPair>, CurrencyPair>(guildRewardCurrencyList);

	TArray<ItemIdAmountPair> personRewardList;
	pkt->MakeGetRewardItemsPersonVector<TArray<ItemIdAmountPair>, ItemIdAmountPair>(personRewardList);
	TArray<CurrencyPair> personRewardCurrencyList;
	pkt->MakeGetRewardCurrencyListPersonVector<TArray<CurrencyPair>, CurrencyPair>(personRewardCurrencyList);

	FGsGuildDungeonClearMsgParam param(clearTime, dungeonLevel, guildRewardList, personRewardList, guildRewardCurrencyList, personRewardCurrencyList);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_DUNGEON_RAID_RESULT, &param);
}

void FGsNetMessageHandlerGuild::AgitDungeonRaidBookmarkListAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_AGIT_DUNGEON_RAID_BOOKMARK_LIST_READ* pkt =
		reinterpret_cast<PKT_SC_ACK_AGIT_DUNGEON_RAID_BOOKMARK_LIST_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		if (FGsAgitHandler::CheckAgitDungeonError(pkt->Result()))
		{
			return;
		}

		if (FGsAgitHandler::CheckAgitError(pkt->Result()))
		{
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (FGsAgitDataController* agitController = guildMgr->GetAgitDataController())
		{
			if (FGsAgitDungeonRaidData* raidData = agitController->GetRaidData())
			{
				raidData->SetBookmarkList(pkt);
			}
		}
	}

	GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_DUNGEON_RAID_BOOKMARK_LIST, nullptr);
}

void FGsNetMessageHandlerGuild::AgitDungeonRaidBookmarkAddAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_AGIT_DUNGEON_RAID_BOOKMARK_ADD_READ* pkt =
		reinterpret_cast<PKT_SC_ACK_AGIT_DUNGEON_RAID_BOOKMARK_ADD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		if (FGsAgitHandler::CheckAgitDungeonError(pkt->Result()))
		{
			return;
		}

		if (FGsAgitHandler::CheckAgitError(pkt->Result()))
		{
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (FGsAgitDataController* agitController = guildMgr->GetAgitDataController())
		{
			if (FGsAgitDungeonRaidData* raidData = agitController->GetRaidData())
			{
				raidData->AddBookmark(pkt->RaidId());
			}
		}
	}

	GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_DUNGEON_RAID_BOOKMARK_CHANGED, nullptr);
}

void FGsNetMessageHandlerGuild::AgitDungeonRaidBookmarkDeleteAck(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_AGIT_DUNGEON_RAID_BOOKMARK_ADD_READ* pkt =
		reinterpret_cast<PKT_SC_ACK_AGIT_DUNGEON_RAID_BOOKMARK_ADD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		if (FGsAgitHandler::CheckAgitDungeonError(pkt->Result()))
		{
			return;
		}

		if (FGsAgitHandler::CheckAgitError(pkt->Result()))
		{
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (FGsAgitDataController* agitController = guildMgr->GetAgitDataController())
		{
			if (FGsAgitDungeonRaidData* raidData = agitController->GetRaidData())
			{
				raidData->RemoveBookmark(pkt->RaidId());
			}
		}
	}

	GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_DUNGEON_RAID_BOOKMARK_CHANGED, nullptr);
}

void FGsNetMessageHandlerGuild::AgitDungeonRaidInformRank(const FGsNet::Packet& InPacket)
{
	PKT_SC_INFORM_AGIT_DUNGEON_RAID_RANK_READ* pkt =
		reinterpret_cast<PKT_SC_INFORM_AGIT_DUNGEON_RAID_RANK_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (FGsAgitDataController* agitController = guildMgr->GetAgitDataController())
		{
			if (FGsAgitDungeonRaidData* raidData = agitController->GetRaidData())
			{
				raidData->SetDungeonRaidRank(pkt);

				GMessage()->GetGuildDungeon().SendMessage(MessageContentGuildDungeon::UPDATE_AGIT_RAID_RANKING, nullptr);
			}
		}
	}
}
