// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNetMessageHandlerGatewayGuild.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsChatManager.h"

#include "Message/MessageParam/GsGuildMessageParam.h"

#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaHelper.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GuildDungeon/GsGuildDungeonHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Guild/GsAgitHandler.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateGuild.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/Stage/StageGame/GsStageGameGuildDungeon.h"

#include "DataSchema/Guild/GuildDungeon/GsSchemaGuildDungeon.h"
#include "DataSchema/Guild/Agit/GsSchemaAgitConfig.h"

#include "Guild/GsGuildHelper.h"
#include "Guild/GsAgitDungeonRaidData.h"
#include "Currency/GsCurrencyHelper.h"

#include "GameObject/GsGlobalGameObjects.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"
#include "GameObject/Data/GsGameObjectDataRemotePlayer.h"

#include "UI/UIContent/Helper/GsUIHelper.h"



void FGsNetMessageHandlerGatewayGuild::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	MProtocalGateway& protocalGatewayHandler = inManager->GetProtocalGateway();

	InitializeMessageDelegateList(&protocalGatewayHandler);
	
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_LIST, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_WAITING_JOIN_GUILD_USER_LIST, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildWaitingJoinUserListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_REQUESTED_JOIN_GUILD_LIST, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildRequestedJoinListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_WAITING_JOIN_GUILD_USER_CNT, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildWaitingJoinUserCount));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_MY_GUILD_INFO, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildMyGuildInfo));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_OPEN_GUILD, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildOpenAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_OPEN_GUILD_FAILED, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildOpenFailedAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_JOIN_GUILD, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildJoinAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_LEAVE_GUILD, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildLeaveAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_EXPEL_GUILD, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildExpelAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_APPROVAL_TO_JOIN_GUILD, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildApprovalToJoinAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_DELETE_GUILD, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildDeleteAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_CANCELLED_JOIN_GUILD, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildCancelJoinAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_REJECTED_USER_JOIN_GUILD, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildRejectedUserJoinAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_MEMBER_LIST, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildMemeberListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_MEMBER_LIST_FAILED, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildMemeberListFailedAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_MEMBER_RIGHTS_BY_GRADE_LIST, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildMemberRightsByGradeListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_CHANGED_GUILD_MEMBER_RIGHTS_BY_GRADE, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildChangedMemberRightsByGradeAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_CHANGED_GUILD_MEMBER_GRADE, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildChangedMemberGradeAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_CHANGED_GUILD_NOTICE, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildChangedNoticeAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_CHANGED_GUILD_DESC, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildChangedDescAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_CHANGED_GUILD_JOIN_TYPE, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildChangedJoinTypeAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_CHANGED_GUILD_LIMITED_USER_LEVEL, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildChangedLimitedUserLevelAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_GUILD_JOIN_REJECTED,
		this, &FGsNetMessageHandlerGatewayGuild::GuildJoinRejectedNotify));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_GUILD_CONTRIBUITON_UPDATE,
		this, &FGsNetMessageHandlerGatewayGuild::GuildContributionUpdate));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_SNS_URL_SAVE,
		this, &FGsNetMessageHandlerGatewayGuild::GuildSaveSNSLinkAck));

	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_BUILDING_OPEN, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildBuildingOpenAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_BUILDING_LIST, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildBuildingListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_BUILDING_UPGRADE, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildBuildingUpgradeAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_BUILDING_PASSIVITY, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildBuildingPassivityAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_ALLY_REQUEST_LIST, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildAllyRequestListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_HOSTILE_LIST, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildHostileListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_ALLY_LIST, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildAllyListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_HOSTILE_ADD, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildHostileAddAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_HOSTILE_DELETE,
		this, &FGsNetMessageHandlerGatewayGuild::GuildHostileDeleteAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_ALLY_ADD, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildAllyAddAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_BREAK_GUILD_ALLY, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildAllyBreakAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_CANCEL_GUILD_ALLY_ADD, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildAllyCancelRejectAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_ACCEPT_GUILD_ALLY, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildAllyAcceptAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_INFORM_GUILD_HOSTILE_ADD,
		this, &FGsNetMessageHandlerGatewayGuild::InformGuildHostileAdd));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_INFORM_GUILD_HOSTILE_DELETE,
		this, &FGsNetMessageHandlerGatewayGuild::InformGuildHostileDelete));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_INFORM_GUILD_ALLY_REQUEST_FAILED,
		this, &FGsNetMessageHandlerGatewayGuild::InformGuildAllyRequestFailed));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_INFORM_GUILD_ALLY_ACCEPT_FAILED,
		this, &FGsNetMessageHandlerGatewayGuild::InformGuildAllyAcceptFailed));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_INFORM_GUILD_ALLY_ACCEPT,
		this, &FGsNetMessageHandlerGatewayGuild::InformGuildAllyAccept));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_INFORM_GUILD_ALLY,
		this, &FGsNetMessageHandlerGatewayGuild::InformGuildAlly));

	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_QUEST_INFO, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildQuestInfoAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_ITEM_COLLECTION_LIST, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildBuildItemCollenctionListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_DETAIL, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildDetailAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ERROR_GUILD_DETAIL, 
		this, &FGsNetMessageHandlerGatewayGuild::GuildDetailErrorAck));
	//AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_OPEN_GUILD_STORE, 
	//	this, &FGsNetMessageHandlerGatewayGuild::GuildStorageOpenAck));
	//AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_STORE_ITEM_LIST, 
	//	this, &FGsNetMessageHandlerGatewayGuild::GuildStorageItemListAck));
	//AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_STORE_GUILD_MEMBER_LIST, 
	//	this, &FGsNetMessageHandlerGatewayGuild::GuildStorageMemberListAck));
	//AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_STORE_ACQUIRED_ITEM_HISTORY, 
	//	this, &FGsNetMessageHandlerGatewayGuild::GuildStorageAcquiredHistoryAck));
	//AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_STORE_DISTRIBUTED_ITEM_HISTORY, 
	//	this, &FGsNetMessageHandlerGatewayGuild::GuildStorageDistributedHistoryAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_OPEN_GUILD_DUNGEON_INFO, 
		this, &FGsNetMessageHandlerGatewayGuild::AckOpenGuildDungeonInfo));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_INFORM_GUILD_DUNGEON_LEVEL, 
		this, &FGsNetMessageHandlerGatewayGuild::InformGuildDungeonLevel));

	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_AUCTION_LIST,
		this, &FGsNetMessageHandlerGatewayGuild::GuildAuctionListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_STORE_ITEM_DETAIL,
		this, &FGsNetMessageHandlerGatewayGuild::GuildAuctionItemDetail));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_STORE_ITEM_LIST_V2,
		this, &FGsNetMessageHandlerGatewayGuild::GuildDivisionListAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_GUILD_STORE_ITEM_CAPTURE_INFO_LIST,
		this, &FGsNetMessageHandlerGatewayGuild::GuildDivisionListDetail));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_GUILD_AUCTION_RETURN_CURRENCY,
		this, &FGsNetMessageHandlerGatewayGuild::GuildAuctionReturnCurrencyNotify));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_STORE_ITEM_AUCTION_REGISTER,
		this, &FGsNetMessageHandlerGatewayGuild::GuildAuctionRegisterAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_GUILD_AUCTION_BIDDER_INFO_LIST,
		this, &FGsNetMessageHandlerGatewayGuild::GuildAuctionBidderInfoList));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_GUILD_AUCTION_CALCULATE_LIST,
		this, &FGsNetMessageHandlerGatewayGuild::GuildCalculateListAck));

	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_SANCTUM_NEXUS_DESTRUCTION,
		this, &FGsNetMessageHandlerGatewayGuild::SanctumNexusDestructionAck));

	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_AGIT_DUNGEON_RAID,
		this, &FGsNetMessageHandlerGatewayGuild::AgitDungeonRaidAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_AGIT_DUNGEON_RAID_DETAIL,
		this, &FGsNetMessageHandlerGatewayGuild::AgitDungeonRaidDetailAck));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_AGIT_DUNGEON_RAID_OPEN,
		this, &FGsNetMessageHandlerGatewayGuild::AgitDungeonRaidOpen));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_AGIT_DUNGEON_RAID_CLOSE,
		this, &FGsNetMessageHandlerGatewayGuild::AgitDungeonRaidClose));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_AGIT_DUNGEON_RAID_CLEAR,
		this, &FGsNetMessageHandlerGatewayGuild::AgitDungeonRaidClear));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ALERT_AGIT_DESTROY,
		this, &FGsNetMessageHandlerGatewayGuild::AgitAlertDestory));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_AGIT_EXPIRE_TIME_UPDATE,
		this, &FGsNetMessageHandlerGatewayGuild::AgitExpireTimeUpdate));

	// 길드 그룹 챗 
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_LIST_READ>(PD::GC::GCPacketId::GC_ACK_GUILD_GROUP_CHAT_LIST,
		this, &FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatList));
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_INVITE_LIST_READ>(PD::GC::GCPacketId::GC_ACK_GUILD_GROUP_CHAT_INVITE_LIST,
		this, &FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatInvitationList));
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_MEMBER_LIST_READ>(PD::GC::GCPacketId::GC_ACK_GUILD_GROUP_CHAT_MEMBER_LIST,
		this, &FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatMemberList));
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_REQUEST_LIST_READ>(PD::GC::GCPacketId::GC_ACK_GUILD_GROUP_CHAT_REQUEST_LIST,
		this, &FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatReqInviteList)); 
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_CREATE_READ>(PD::GC::GCPacketId::GC_ACK_GUILD_GROUP_CHAT_CREATE,
		this, &FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatRoomCreate));
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_INVITE_READ>(PD::GC::GCPacketId::GC_ACK_GUILD_GROUP_CHAT_INVITE,
		this, &FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatInvite));
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_ACCEPT_READ>(PD::GC::GCPacketId::GC_ACK_GUILD_GROUP_CHAT_ACCEPT,
		this, &FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatInviteAccepted));
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_INVITE_REJECT_READ>(PD::GC::GCPacketId::GC_ACK_GUILD_GROUP_CHAT_INVITE_REJECT,
		this, &FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatInviteReject));
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_INVITE_CANCEL_READ>(PD::GC::GCPacketId::GC_ACK_GUILD_GROUP_CHAT_INVITE_CANCEL,
		this, &FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatInviteCancel)); 
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_KICK_READ>(PD::GC::GCPacketId::GC_ACK_GUILD_GROUP_CHAT_KICK,
		this, &FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatKickOut)); 
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_WITHDRAWAL_READ>(PD::GC::GCPacketId::GC_ACK_GUILD_GROUP_CHAT_WITHDRAWAL,
		this, &FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatWithDraw));
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_DELEGATE_READ>(PD::GC::GCPacketId::GC_ACK_GUILD_GROUP_CHAT_DELEGATE,
		this, &FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatEntrustMaster));
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_MODIFY_CHAT_GRADE_READ>(PD::GC::GCPacketId::GC_ACK_GUILD_GROUP_CHAT_MODIFY_CHAT_GRADE,
		this, &FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatAuthChanged));
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_GUILD_GROUP_CHAT_REQUEST_READ>(PD::GC::GCPacketId::GC_GUILD_GROUP_CHAT_REQUEST,
		this, &FGsNetMessageHandlerGatewayGuild::NotiGuildGroupChatInvitaionRequest)); 
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_GUILD_GROUP_CHAT_ENTERED_FOR_MY_GUILD_READ>(PD::GC::GCPacketId::GC_GUILD_GROUP_CHAT_ENTERED_FOR_MY_GUILD,
		this, &FGsNetMessageHandlerGatewayGuild::NotiMyGuildGroupChatRoomCreated)); 
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_GUILD_GROUP_CHAT_KICK_READ>(PD::GC::GCPacketId::GC_GUILD_GROUP_CHAT_KICK,
		this, &FGsNetMessageHandlerGatewayGuild::NotiMyGuildGuildGroupChatRoomKickedOut)); 
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_GUILD_GROUP_CHAT_KICK_FOR_OTHER_GUILD_READ>(PD::GC::GCPacketId::GC_GUILD_GROUP_CHAT_KICK_FOR_OTHER_GUILD,
		this, &FGsNetMessageHandlerGatewayGuild::NotiOtherGuildGuildGroupChatRoomKickedOut));
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_GUILD_GROUP_CHAT_WITHDRAWAL_READ>(PD::GC::GCPacketId::GC_GUILD_GROUP_CHAT_WITHDRAWAL,
		this, &FGsNetMessageHandlerGatewayGuild::NotiMyGuildGuildGroupChatRoomWithDraw)); 
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_GUILD_GROUP_CHAT_WITHDRAWAL_FOR_OTHER_GUILD_READ>(PD::GC::GCPacketId::GC_GUILD_GROUP_CHAT_WITHDRAWAL_FOR_OTHER_GUILD,
		this, &FGsNetMessageHandlerGatewayGuild::NotiOtherGuildGuildGroupChatRoomWithDraw));
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_GUILD_GROUP_CHAT_INVITED_READ>(PD::GC::GCPacketId::GC_GUILD_GROUP_CHAT_INVITED,
		this, &FGsNetMessageHandlerGatewayGuild::NotiGuestGuildGroupChatInvited));
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_GUILD_GROUP_CHAT_INVITE_REJECT_READ>(PD::GC::GCPacketId::GC_GUILD_GROUP_CHAT_INVITE_REJECT,
		this, &FGsNetMessageHandlerGatewayGuild::NotiGuestGuildGroupChatInvitationRejected)); 
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_GUILD_GROUP_CHAT_INVITE_CANCEL_READ>(PD::GC::GCPacketId::GC_GUILD_GROUP_CHAT_INVITE_CANCEL,
		this, &FGsNetMessageHandlerGatewayGuild::NotiGuestGuildGroupChatInvitationCanceled)); 
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_GUILD_GROUP_CHAT_ENTERED_FOR_OTHER_GUILD_READ>(PD::GC::GCPacketId::GC_GUILD_GROUP_CHAT_ENTERED_FOR_OTHER_GUILD,
		this, &FGsNetMessageHandlerGatewayGuild::NotiAllGuestGuildGroupChatInvitationAccepted));
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_GUILD_GROUP_CHAT_DELEGATE_READ>(PD::GC::GCPacketId::GC_GUILD_GROUP_CHAT_DELEGATE,
		this, &FGsNetMessageHandlerGatewayGuild::NotiAllGuildGroupChatMembersEntrutMaster));
	AddMessageDelegate(protocalGatewayHandler.AddRawPacket<PD::GC::PKT_GC_GUILD_GROUP_CHAT_MODIFY_CHAT_GRADE_READ>(PD::GC::GCPacketId::GC_GUILD_GROUP_CHAT_MODIFY_CHAT_GRADE,
		this, &FGsNetMessageHandlerGatewayGuild::NotiAllGuildGroupChatMembersAuthChanged));
	// end 길드 그룹 챗
}

void FGsNetMessageHandlerGatewayGuild::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerGatewayGuild::GuildListAck(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result gcResult = static_cast<PD::Result>(pkt->Result());
		if (PD::Result::GUILD_SEARCH_ERROR_COOLDOWN == gcResult)
		{
			// TEXT: 잠시 후 다시 시도해주세요.
			FText textMsg;
			FText::FindText(TEXT("GuildText"), TEXT("Alert_Retry"), textMsg);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			
			return;
		}

		FGsUIHelper::PopupNetError(gcResult);
		return;
	}

	GGuild()->SetGuildList(*pkt);

	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_GUILD_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildWaitingJoinUserListAck(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_WAITING_JOIN_GUILD_USER_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_WAITING_JOIN_GUILD_USER_LIST_READ*>(
		inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGuild()->SetJoinWaitingUserList(*pkt);

	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_JOIN_WAITING_USER_LIST, nullptr);

	// 레드닷 체크 위한 갱신
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);
}

void FGsNetMessageHandlerGatewayGuild::GuildRequestedJoinListAck(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_REQUESTED_JOIN_GUILD_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_REQUESTED_JOIN_GUILD_LIST_READ*>(
		inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGuild()->SetRequestedJoinGuildList(*pkt);

	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_JOIN_WAITING_GUILD_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildWaitingJoinUserCount(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_WAITING_JOIN_GUILD_USER_CNT_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_WAITING_JOIN_GUILD_USER_CNT_READ*>(
		inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGuild()->SetJoinWaitingUserCount(pkt->Cnt());
}

void FGsNetMessageHandlerGatewayGuild::GuildMyGuildInfo(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_MY_GUILD_INFO_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_MY_GUILD_INFO_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	bool bIsGuild = GGuild()->IsGuild();

	GGuild()->SetMyGuildInfo(*pkt);

	if(bIsGuild != GGuild()->IsGuild())
		GSChat()->OnCheckGuildChat();
}

void FGsNetMessageHandlerGatewayGuild::GuildOpenAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_OPEN_GUILD_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_OPEN_GUILD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 정보 저장
	if (FGsGuildManager* guildMgr = GGuild())
	{
		bool bIsJoinOpen = false;
		if (const FGsGuildDataMine* myGuildData = guildMgr->GetMyGuildData())
		{
			// 첫 가입 시 받는 SC_USER_GUILD_INFO 에서는 _joinTime 정보를 주지 않으므로, 첫 가입 체크를 위해 사용 
			if (myGuildData->IsExistGuild() &&
				0 == myGuildData->_joinedTime)
			{
				bIsJoinOpen = true;
			}
		}

		guildMgr->SetUserGuildInfoDetail(*pkt);

		// 첫 가입 시 동맹/적대 네임플레이트 갱신. 다른 경우엔 월드 입장 시 준다.
		if (bIsJoinOpen)
		{
			CheckDiplomacyRemotePlayer(true);
		}
	}

	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_OPEN_GUILD, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildOpenFailedAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_OPEN_GUILD_FAILED_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_OPEN_GUILD_FAILED_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}
}

void FGsNetMessageHandlerGatewayGuild::GuildJoinAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_JOIN_GUILD_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_JOIN_GUILD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());

		if (PD::Result::GUILD_JOIN_ERROR_UNABLE == result ||
			PD::Result::GUILD_JOIN_ERROR_FULL == result)
		{
			// TEXT: 기사단 정보가 수정되었습니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_GuildinfoChange"));
			return;
		}
		else if (PD::Result::GUILD_JOIN_ERROR_FULL_OF_APPLICANTS == result)
		{
			// TEXT: 가입 대기요청 인원이 꽉찼습니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_FullApplicants"));
			return;
		}
		else if (PD::Result::GUILD_JOIN_ERROR_OVER_REQUEST_CNT == result)
		{
			// TEXT: 가입 요청 횟수 제한을 초과하였습니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_JoinRequestNumber_Over"));
			return;
		}
		else if (PD::Result::GUILD_JOIN_ERROR_USER_LEVEL == result)
		{
			// TEXT: 가입 제한 레벨보다 레벨이 낮습니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_CannotJoinGuild_LevelLimit"));
			return;
		}
		else if (PD::Result::GUILD_ERROR_HAD_GUILD == result)
		{
			// TEXT: 이미 가입된 기사단이 있습니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_MultipleJoin"));
			return;
		}
		else if (PD::Result::GUILD_ERROR_UNKNOWN_GUILD_ID == result)
		{
			// 해산된 길드 메시지 출력하고, 데이터 삭제 후 갱신
			// TEXT: 기사단이 존재하지 않습니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("UI_Diplomacy_EmptyName"));

			GuildDBId guildDBId = pkt->GuildDBId();
			if (FGsGuildManager* guildMgr = GGuild())
			{
				guildMgr->RemoveJoinGuild(guildDBId);
				guildMgr->RemoveJoinWaitingGuild(guildDBId);
			}

			// 갱신 후 
			GMessage()->GetGuild().SendMessage(MessageContentGuild::INVALIDATE_GUILD_JOIN_LIST, nullptr);
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	// 가입대기 상태
	if (pkt->IsWait())
	{
		GuildDBId guildDBId = pkt->GuildDBId();
		GGuild()->SetJoinResultWaiting(guildDBId);

		// 신청중 표시 갱신을 위한 메시지를 보냄
		GMessage()->GetGuild().SendMessage(MessageContentGuild::INVALIDATE_GUILD_JOIN_LIST, nullptr);
	}
	else
	{
		// 가입이 되었을 경우 SC_USER_GUILD_INFO 가 온다
	}
}

void FGsNetMessageHandlerGatewayGuild::GuildLeaveAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_LEAVE_GUILD_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_LEAVE_GUILD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (PD::Result::GUILD_LEAVE_ERROR_GUILD_DUNGEON_PLAYING_USER == result)
		{
			// TEXT: 기사단 던전 참여 중에는 탈퇴할 수 없습니다
			ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_CannotQuitGuild_inDungeon"));
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	// 탈퇴했으므로 길드 관련 정보 전부 삭제
	GGuild()->RemoveAllGuildData();
	GSChat()->OnCheckGuildChat();

	FGsGameDataManager* gameDataMgr = GGameData();
	gameDataMgr->ClearGuildData();

	if (const FGsNetUserData* userData = gameDataMgr->GetUserData())
	{
		// TEXT: {0}님이 길드를 탈퇴하였습니다.
		FText textFormat;
		if (FText::FindText(TEXT("GuildText"), TEXT("Alert_QuitMember"), textFormat))
		{
			FText textMsg = FText::Format(textFormat, FText::FromString(userData->mUserName));
			FGsUIHelper::AddChatMessageSystem(textMsg);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg, TrayCondition::TC_Close);
		}
	}

	// 내 캐릭터 네임플레이트 갱신	
	if (UGsGameObjectBase* target = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer* localPlayer = target->CastGameObject<UGsGameObjectLocalPlayer>())
		{
			localPlayer->UpdateBillboard(EGsBillboardUpdateType::Guild);
		}
	}

	// 동맹/적대 표시 갱신을 위해 주변 유저들 네임플레이트 리셋
	CheckDiplomacyRemotePlayer(true);

	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_LEAVE, nullptr);
	
	// 성소 상태 갱신
	//GMessage()->GetSanctum().SendMessage(MessageContentSanctum::INVALIDATE_SANCTUM_STATE, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildExpelAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_EXPEL_GUILD_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_EXPEL_GUILD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (PD::Result::GUILD_LEAVE_ERROR_GUILD_DUNGEON_PLAYING_USER == result)
		{
			// TEXT: 기사단 던전 참여 중인 유저는 추방할 수 없습니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_CannotKickOut_inDungeon"));
			return;
		}

		if (PD::Result::GUILD_LEAVE_ERROR_CANNOT_STATE == result)
		{
			// TEXT: 기사단 던전 참여 중인 유저는 추방할 수 없습니다.
			FText message;
			FGsBattleArenaHelper::FindText(TEXT("Ticker_CannotKickOut_inGuildArena"), message);

			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
			return;
		}

		if (CheckAuthorityError(result))
		{
			return;
		}

		if (CheckMemberError(result))
		{
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	// ClearUserGuildInfoNotify 와의 처리 중복을 피하기 위해 검사함
	if (GGuild()->RemoveGuildMember(pkt->Expel_userDBId()))
	{
		GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_REMOVE_MEMBER, nullptr);
	}
}

void FGsNetMessageHandlerGatewayGuild::GuildApprovalToJoinAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_APPROVAL_TO_JOIN_GUILD_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_APPROVAL_TO_JOIN_GUILD_READ*>(InPacket.Buffer);
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
		
		// 기사단이 이미 있는유저. (승인하거나 다른데 가입했을때. 목록에서 삭제 후 갱신 메시지)
		if (PD::Result::GUILD_ERROR_HAD_GUILD == result)
		{
			// TEXT: 다른 기사단에 가입한 유저입니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_OtherGuildMember"));

			GGuild()->SetRejectJoin(pkt->UserDBId());
			GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_JOIN_APPROVE_RESULT, nullptr);

			return;
		}
		else if (PD::Result::GUILD_ERROR_NOT_REQUEST_JOIN == result)
		{
			// TEXT: 가입 신청을 취소한 유저입니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_CannotAccept_RequestCancel"));

			GGuild()->SetRejectJoin(pkt->UserDBId());
			GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_JOIN_APPROVE_RESULT, nullptr);
			return;
		}
		else if (PD::Result::GUILD_JOIN_ERROR_FULL == result)
		{
			// TEXT: 기사단 인원이 가득 차서 더 이상 가입할 수 없습니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_JoinGuildFull"));

			// 이 경우엔 유저를 목록에서 지우지 않도록 함
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	GGuild()->SetApproveJoin(pkt->UserDBId());
	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_JOIN_APPROVE_RESULT, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildDeleteAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_DELETE_GUILD_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_DELETE_GUILD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (PD::Result::GUILD_LEAVE_ERROR_GUILD_DUNGEON_PLAYING_USER == result)
		{
			// TEXT: 기사단 던전 참여 중에는 해산할 수 없습니다
			ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_CannotDeleteGuild_inDungeon"));
			return;
		}
		else if (PD::Result::GUILD_DELETE_ERROR_MEMBER_CNT == result)
		{
			// TEXT: 기사단에 1명 남았을 때만 해산할 수 있습니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_CannotDeleteGuild"));
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	// 길드 관련 정보 전부 삭제
	GGuild()->RemoveAllGuildData();
	GSChat()->OnCheckGuildChat();
	GGameData()->ClearGuildData();

	// 내 캐릭터 네임플레이트 갱신	
	if (UGsGameObjectBase* target = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer* localPlayer = target->CastGameObject<UGsGameObjectLocalPlayer>())
		{
			localPlayer->UpdateBillboard(EGsBillboardUpdateType::Guild);
		}
	}

	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_DISBAND, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildCancelJoinAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_CANCELLED_JOIN_GUILD_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_CANCELLED_JOIN_GUILD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());

		if (PD::Result::GUILD_ERROR_UNKNOWN_GUILD_ID == result)
		{
			// TEXT: 기사단이 존재하지 않습니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("UI_Diplomacy_EmptyName"));

			GuildDBId guildDBId = pkt->GuildDBId();
			if (FGsGuildManager* guildMgr = GGuild())
			{
				guildMgr->RemoveJoinGuild(guildDBId);
				guildMgr->RemoveJoinWaitingGuild(guildDBId);
			}

			// 갱신 후 
			GMessage()->GetGuild().SendMessage(MessageContentGuild::INVALIDATE_GUILD_JOIN_LIST, nullptr);
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	GGuild()->RemoveJoinWaitingGuild(pkt->GuildDBId());

	GMessage()->GetGuild().SendMessage(MessageContentGuild::INVALIDATE_GUILD_JOIN_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildRejectedUserJoinAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_REJECTED_USER_JOIN_GUILD_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_REJECTED_USER_JOIN_GUILD_READ*>(InPacket.Buffer);
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

		// 기사단이 이미 있는유저. (승인하거나 다른데 가입했을때. 목록에서 삭제 후 갱신 메시지)
		if (PD::Result::GUILD_ERROR_HAD_GUILD == result)
		{
			// TEXT: 다른 기사단에 가입한 유저입니다.
			ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_OtherGuildMember"));

			GGuild()->SetRejectJoin(pkt->RejectUserDBId());
			GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_JOIN_APPROVE_RESULT, nullptr);

			return;
		}
		else if (PD::Result::GUILD_ERROR_NOT_REQUEST_JOIN == result)
		{
			// 거절이니 굳이 메시지 출력할 필요없이 지움
			GGuild()->SetRejectJoin(pkt->RejectUserDBId());
			GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_JOIN_APPROVE_RESULT, nullptr);
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	GGuild()->SetRejectJoin(pkt->RejectUserDBId());
	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_JOIN_APPROVE_RESULT, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildMemeberListAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_MEMBER_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_MEMBER_LIST_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGuild()->SetGuildMemberList(*pkt);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_MEMBER_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildMemeberListFailedAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_MEMBER_LIST_FAILED_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_MEMBER_LIST_FAILED_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}
}

void FGsNetMessageHandlerGatewayGuild::GuildChangedMemberGradeAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_CHANGED_GUILD_MEMBER_GRADE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_CHANGED_GUILD_MEMBER_GRADE_READ*>(InPacket.Buffer);
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

		if (CheckMemberError(result))
		{
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	GGuild()->SetChangeMemberGrade(pkt->UserDBId(), pkt->MemberGrade());

	FGsGuildMsgParamUser param(pkt->UserDBId());
	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_CHANGE_MEMBER_GRADE, &param);
}

void FGsNetMessageHandlerGatewayGuild::GuildChangedMemberRightsByGradeAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_CHANGED_GUILD_MEMBER_RIGHTS_BY_GRADE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_CHANGED_GUILD_MEMBER_RIGHTS_BY_GRADE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	GGuild()->SetChangeAuthority(*pkt);

	// TEXT: 권한 정보가 수정되었습니다.
	FText textMsg;
	FText::FindText(TEXT("GuildText"), TEXT("Alert_AuthorityChange"), textMsg);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_CHANGE_AUTHORITY, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildMemberRightsByGradeListAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_MEMBER_RIGHTS_BY_GRADE_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_MEMBER_RIGHTS_BY_GRADE_LIST_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	GGuild()->SetAuthorityList(*pkt);

	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_AUTHORITY_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildChangedNoticeAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_CHANGED_GUILD_NOTICE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_CHANGED_GUILD_NOTICE_READ*>(InPacket.Buffer);
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

	GGuild()->SetChangeGuildString(pkt->GuildNotice(), true);

	FGsGuildMsgParamSetting param(FGsGuildMsgParamSetting::EGsGuildSettingType::Notice);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::CHANGE_GUILD_SETTING, &param);
}

void FGsNetMessageHandlerGatewayGuild::GuildChangedDescAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_CHANGED_GUILD_DESC_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_CHANGED_GUILD_DESC_READ*>(InPacket.Buffer);
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

	GGuild()->SetChangeGuildString(pkt->GuildDesc(), false);

	FGsGuildMsgParamSetting param(FGsGuildMsgParamSetting::EGsGuildSettingType::Introduction);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::CHANGE_GUILD_SETTING, &param);
}

void FGsNetMessageHandlerGatewayGuild::GuildChangedJoinTypeAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_CHANGED_GUILD_JOIN_TYPE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_CHANGED_GUILD_JOIN_TYPE_READ*>(InPacket.Buffer);
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

	GGuild()->SetChangeJoinType(pkt->GuildJoinType());

	FGsGuildMsgParamSetting param(FGsGuildMsgParamSetting::EGsGuildSettingType::JoinType);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::CHANGE_GUILD_SETTING, &param);
}

void FGsNetMessageHandlerGatewayGuild::GuildChangedLimitedUserLevelAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_CHANGED_GUILD_LIMITED_USER_LEVEL_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_CHANGED_GUILD_LIMITED_USER_LEVEL_READ*>(InPacket.Buffer);
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

	GGuild()->SetChangeLimitUserLevel(pkt->LimitedUserLevel());

	FGsGuildMsgParamSetting param(FGsGuildMsgParamSetting::EGsGuildSettingType::LimitLevel);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::CHANGE_GUILD_SETTING, &param);
}

void FGsNetMessageHandlerGatewayGuild::GuildJoinRejectedNotify(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_GUILD_JOIN_REJECTED_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_GUILD_JOIN_REJECTED_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGuild()->RemoveJoinWaitingGuild(pkt->RejectGuildDBId());

	GMessage()->GetGuild().SendMessage(MessageContentGuild::INVALIDATE_GUILD_JOIN_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildContributionUpdate(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_GUILD_CONTRIBUITON_UPDATE_READ* pkt = 
		reinterpret_cast<PD::GC::PKT_GC_GUILD_CONTRIBUITON_UPDATE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsGuildManager* guildMgr = GGuild())
	{
		guildMgr->SetGuildContribution(*pkt, true);
	}
}

void FGsNetMessageHandlerGatewayGuild::GuildSaveSNSLinkAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_SNS_URL_SAVE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_SNS_URL_SAVE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 요청에 대한 결과만 오고, SC_CHANGE_SNS_URL_GUILD 노티 통해 결과 정보가 온다
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
	}
}

void FGsNetMessageHandlerGatewayGuild::GuildBuildingListAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_BUILDING_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_BUILDING_LIST_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	GGuild()->SetGuildBuildingList(*pkt);

	FGsPrimitiveInt32 param(static_cast<int32>(PassivityOwnerType::GUILD_BUILDING));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_UPDATE_COLLECTION, &param);

	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_BUILDING_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildBuildingOpenAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_BUILDING_OPEN_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_BUILDING_OPEN_READ*>(InPacket.Buffer);
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

	GGuild()->SetBuildingOpen(pkt->ConstRefGuildBuildingData());

	// 갱신 메시지 보냄
	FGsGuildMsgParamInt param(pkt->ConstRefGuildBuildingData().mBuildingId);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_BUILDING_UPDATE, &param);
}

void FGsNetMessageHandlerGatewayGuild::GuildBuildingUpgradeAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_BUILDING_UPGRADE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_BUILDING_UPGRADE_READ*>(InPacket.Buffer);
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

	// Notify에서 한다
	GGuild()->SetBuildingUpgrade(pkt->GuildBuildingId(), pkt->GuildBuildingLevel());

	// Notify에서 한다
	// 갱신 메시지 보냄
	FGsGuildMsgParamInt param(pkt->GuildBuildingId());
	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_BUILDING_UPDATE, &param);
}

void FGsNetMessageHandlerGatewayGuild::GuildBuildingPassivityAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_BUILDING_PASSIVITY_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_BUILDING_PASSIVITY_READ*>(InPacket.Buffer);
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

	GGuild()->SetActiveGuildBuff(*pkt);

	// 건물 갱신 메시지
	FGsGuildMsgParamInt param(pkt->GuildBuildingId());
	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_BUILDING_UPDATE, &param);
}

void FGsNetMessageHandlerGatewayGuild::GuildAllyRequestListAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_ALLY_REQUEST_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_ALLY_REQUEST_LIST_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGuild()->SetAllyStateList(*pkt);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::DIPLOMACY_INVALIDATE_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildHostileListAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_HOSTILE_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_HOSTILE_LIST_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGuild()->SetEnemyList(*pkt);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::DIPLOMACY_INVALIDATE_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildAllyListAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_ALLY_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_ALLY_LIST_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGuild()->SetAllyList(*pkt);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::DIPLOMACY_INVALIDATE_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildHostileAddAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_HOSTILE_ADD_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_HOSTILE_ADD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (CheckGuildDiplomacyError(result))
		{
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}
	
	// TEXT: 적대를 신청했습니다.
	ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_Diplomacy_Ack_Enemy"));

	if (FGsGuildDataMine* myGuildData = GGuild()->GetMyGuildDataNoConst())
	{
		myGuildData->AddEnemyData(*pkt);
	}

	GMessage()->GetGuild().SendMessage(MessageContentGuild::DIPLOMACY_INVALIDATE_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildHostileDeleteAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_HOSTILE_DELETE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_HOSTILE_DELETE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (CheckGuildDiplomacyError(result))
		{
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	if (FGsGuildDataMine* myGuildData = GGuild()->GetMyGuildDataNoConst())
	{
		myGuildData->RemoveEnemyData(pkt->Hostile_guildId());
	}

	GMessage()->GetGuild().SendMessage(MessageContentGuild::DIPLOMACY_INVALIDATE_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildAllyAddAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_ALLY_ADD_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_ALLY_ADD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (CheckGuildDiplomacyError(result))
		{
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	// TEXT: 동맹을 신청했습니다.
	ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_Diplomacy_Ack_Ally"));

	// 성공 여부만 받는다.
	// 동맹 상태 들어갈 때 리스트 새로 받을 수 있게 쿨타임 갱신
	GGuild()->GetSendController().ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::AllyStateList);

	// InformGuildAllyAccept 에서 데이터 갱신(월드 서버에서 실패할 경우를 대비)
}

void FGsNetMessageHandlerGatewayGuild::GuildAllyBreakAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_BREAK_GUILD_ALLY_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_BREAK_GUILD_ALLY_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (PD::Result::GUILD_ERROR_EXIST_ALLY_GUILD == result)
		{
			if (FGsGuildManager* guildMgr = GGuild())
			{
				// TEXT: 정보가 수정되어 실패하였습니다. 페이지를 갱신합니다.
				ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_Fail_And_Refresh"));

				GMessage()->GetGuild().SendMessage(MessageContentGuild::DIPLOMACY_RESEND_LIST, nullptr);
				return;
			}
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	// TEXT: 동맹을 파기했습니다.
	ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_Diplomacy_Ack_Break_Ally"));

	// InformGuildAlly 에서 데이터 갱신(월드 서버에서 실패할 경우를 대비)
}

void FGsNetMessageHandlerGatewayGuild::GuildAllyCancelRejectAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_CANCEL_GUILD_ALLY_ADD_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_CANCEL_GUILD_ALLY_ADD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (PD::Result::GUILD_ERROR_EXIST_ALLY_REQUEST_GUILD == result ||
			PD::Result::GUILD_ERROR_UNKNOWN_TARGET_GUILD_ID == result) // 해체된 기사단 취소할 경우
		{
			if (FGsGuildManager* guildMgr = GGuild())
			{
				// TEXT: 정보가 수정되어 실패하였습니다. 페이지를 갱신합니다.
				ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_Fail_And_Refresh"));

				GMessage()->GetGuild().SendMessage(MessageContentGuild::DIPLOMACY_RESEND_LIST, nullptr);
				return;
			}
		}
		else if (CheckGuildDiplomacyError(result))
		{
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	// InformGuildAllyAccept 에서 데이터 갱신(월드 서버에서 실패할 경우를 대비)
}

void FGsNetMessageHandlerGatewayGuild::GuildAllyAcceptAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_ACCEPT_GUILD_ALLY_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_ACCEPT_GUILD_ALLY_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (PD::Result::GUILD_ERROR_EXIST_ALLY_REQUEST_GUILD == result ||
			PD::Result::GUILD_ERROR_UNKNOWN_TARGET_GUILD_ID == result ||
			PD::Result::GUILD_ERROR_ALLY_GUILD_ID == result)
		{
			if (FGsGuildManager* guildMgr = GGuild())
			{
				// TEXT: 정보가 수정되어 실패하였습니다. 페이지를 갱신합니다.
				ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_Fail_And_Refresh"));

				GMessage()->GetGuild().SendMessage(MessageContentGuild::DIPLOMACY_RESEND_LIST, nullptr);
				return;
			}
		}
		else if (CheckGuildDiplomacyError(result))
		{
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	// InformGuildAlly 에서 데이터 갱신(월드 서버에서 실패할 경우를 대비)
}

void FGsNetMessageHandlerGatewayGuild::InformGuildHostileAdd(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_INFORM_GUILD_HOSTILE_ADD_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_INFORM_GUILD_HOSTILE_ADD_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsGuildManager* guildMgr = GGuild();
	if (nullptr == guildMgr)
	{
		return;
	}

	// 삭제가 올 경우엔 지운 후 갱신
	if (FGsGuildDataMine* myGuildData = guildMgr->GetMyGuildDataNoConst())
	{
		myGuildData->AddEnemyData(*pkt);

		// 모자란 적대길드 정보를 다시 받을 수 있도록 쿨타임을 리셋함
		guildMgr->GetSendController().ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::EnemyList);
	}

	// TEXT: [{0}]기사단과 적대 관계가 되었습니다.
	FText textFormat;
	if (FText::FindText(TEXT("SystemChatText"), TEXT("SystemChat_Guild_WarBegin"), textFormat))
	{
		FText textOtherName;
		FGsGuildHelper::GetFormattedGuildName(pkt->GuildName(), pkt->ConstRefCurrentHomePlanetWorldId().st.worldId, 
			false, textOtherName);

		FText textMsg = FText::Format(textFormat, textOtherName);

		FGsUIHelper::AddChatMessageSystem(textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	}

	// 레드닷 체크
	guildMgr->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::DiplomacyEnemy);

	// HUD 갱신
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);

	// 길드창 갱신
	FGsGuildMsgParamDiplomacyNotify param(pkt->GuildDBId(), false);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::DIPLOMACY_ENEMY_STATE_NOTIFY, &param);

	// 주변 유저 네임플레이트 갱신
	CheckDiplomacyRemotePlayer(true);
}

void FGsNetMessageHandlerGatewayGuild::InformGuildHostileDelete(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_INFORM_GUILD_HOSTILE_DELETE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_INFORM_GUILD_HOSTILE_DELETE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsGuildManager* guildMgr = GGuild();
	if (nullptr == guildMgr)
	{
		return;
	}

	// 삭제가 올 경우엔 지운 후 갱신
	if (FGsGuildDataMine* myGuildData = guildMgr->GetMyGuildDataNoConst())
	{
		myGuildData->RemoveEnemyData(pkt->GuildDBId());
	}

	// TEXT: [{0}]기사단과 적대 관계가 해제 되었습니다.
	FText textFormat;
	if (FText::FindText(TEXT("SystemChatText"), TEXT("SystemChat_Guild_WarEnd"), textFormat))
	{
		FText textOtherName;
		FGsGuildHelper::GetFormattedGuildName(pkt->GuildName(), pkt->ConstRefCurrentHomePlanetWorldId().st.worldId, 
			false, textOtherName);

		FText textMsg = FText::Format(textFormat, textOtherName);

		FGsUIHelper::AddChatMessageSystem(textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	}

	// 삭제 시엔 레드닷 체크 넣지 않았음. 문제 시 추가

	// 길드창 갱신
	FGsGuildMsgParamDiplomacyNotify param(pkt->GuildDBId(), true);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::DIPLOMACY_ENEMY_STATE_NOTIFY, &param);

	// 주변 유저 네임플레이트 갱신
	CheckDiplomacyRemotePlayer(true);
}

void FGsNetMessageHandlerGatewayGuild::InformGuildAllyRequestFailed(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_INFORM_GUILD_ALLY_REQUEST_FAILED_READ* pkt = 
		reinterpret_cast<PD::GC::PKT_GC_INFORM_GUILD_ALLY_REQUEST_FAILED_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 실패의 원인을 출력
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());

		if (CheckGuildDiplomacyError(result))
		{
			return;
		}

		FGsUIHelper::PopupNetError(result);
	}
}

void FGsNetMessageHandlerGatewayGuild::InformGuildAllyAcceptFailed(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_INFORM_GUILD_ALLY_ACCEPT_FAILED_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_INFORM_GUILD_ALLY_ACCEPT_FAILED_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());

		if (CheckGuildDiplomacyError(result))
		{
			return;
		}

		FGsUIHelper::PopupNetError(result);
	}
}

void FGsNetMessageHandlerGatewayGuild::InformGuildAllyAccept(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_INFORM_GUILD_ALLY_ACCEPT_READ* pkt = 
		reinterpret_cast<PD::GC::PKT_GC_INFORM_GUILD_ALLY_ACCEPT_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsGuildManager* guildMgr = GGuild();
	if (nullptr == guildMgr)
	{
		return;
	}

	if (FGsGuildDataMine* myGuildData = guildMgr->GetMyGuildDataNoConst())
	{
		// 동맹 요청 취소 시
		if (pkt->Cancel())
		{
			GuildDBId removeGuildDBId = pkt->GuildId();
			myGuildData->RemoveAllyStateData(removeGuildDBId);
		}
		else
		{
			// 신규 요청이 왔을 때
			if (nullptr == myGuildData->GetAllyStateData(pkt->GuildId()))
			{
				FGsGuildDiplomacyDataState stateData;
				stateData.SetData(*pkt);

				// 상태정보 저장. 새로 저장될 경우, Setting에 저장된 정보와 다르므로 레드닷이 뜰 것.
				myGuildData->AddAllyStateData(stateData, false);

				// 레드닷 갱신
				guildMgr->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::DiplomacyState);

				// HUD 갱신
				GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);

				// 다음에 들어갈 때 완전한 정보로 다시받을 수 있게 쿨타임 리셋
				guildMgr->GetSendController().ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::AllyStateList);
			}
		}

		// 길드 창 갱신
		FGsGuildMsgParamDiplomacyNotify param(pkt->GuildId(), pkt->Cancel());
		GMessage()->GetGuild().SendMessage(MessageContentGuild::DIPLOMACY_ALLY_STATE_NOTIFY, &param);
	}
}

void FGsNetMessageHandlerGatewayGuild::InformGuildAlly(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_INFORM_GUILD_ALLY_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_INFORM_GUILD_ALLY_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsGuildManager* guildMgr = GGuild();
	if (nullptr == guildMgr)
	{
		return;
	}

	if (FGsGuildDataMine* myGuildData = guildMgr->GetMyGuildDataNoConst())
	{
		if (pkt->Deleted())
		{
			// TEXT: [{0}]기사단과 동맹 관계가 해제 되었습니다.
			FText textFormat;
			if (FText::FindText(TEXT("SystemChatText"), TEXT("SystemChat_Guild_AllyEnd"), textFormat))
			{
				FText textOtherName;
				FGsGuildHelper::GetFormattedGuildName(pkt->GuildName(),
					pkt->ConstRefCurrentHomePlanetWorldId().st.worldId, false, textOtherName);

				FText textMsg = FText::Format(textFormat, textOtherName);

				FGsUIHelper::AddChatMessageSystem(textMsg);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}

			GuildDBId removeGuildDBId = pkt->GuildId();
			myGuildData->RemoveAllyData(removeGuildDBId);
		}
		else
		{
			// 동맹 상태 리스트에 정보가 있을 경우에 삭제 처리
			myGuildData->RemoveAllyStateData(pkt->GuildId());
			// 동맹 리스트에 정보 추가
			myGuildData->AddAllyData(*pkt);

			// TEXT: [{0}]기사단과 [{1}]기사단이 동맹이 되었습니다.
			FText textFormat;
			if (FText::FindText(TEXT("GuildText"), TEXT("Alert_Diplomacy_Notify_Ally"), textFormat))
			{
				FText textOtherName;
				FGsGuildHelper::GetFormattedGuildName(pkt->GuildName(),
					pkt->ConstRefCurrentHomePlanetWorldId().st.worldId, false, textOtherName);

				FText textMsg = FText::Format(textFormat,
					FText::FromString(myGuildData->_guildName), textOtherName);

				FGsUIHelper::AddChatMessageSystem(textMsg);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}

			// 레드닷 갱신
			guildMgr->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::DiplomacyAlly);

			// HUD 갱신
			GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);

			// 길드 페이지일 경우, 노티 정보에 누락된 정보를 추가로 받기 위해 요청 쿨타임 리셋
			guildMgr->GetSendController().ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::AllyList);
		}
	}

	//if (auto chatMng = GSChat())
	//{
	//	chatMng->OnCheckGuildChat();
	//}

	// 길드 창 갱신
	FGsGuildMsgParamDiplomacyNotify param(pkt->GuildId(), pkt->Deleted());
	GMessage()->GetGuild().SendMessage(MessageContentGuild::DIPLOMACY_ALLY_STATE_NOTIFY, &param);

	// 주변 유저 네임플레이트 갱신
	CheckDiplomacyRemotePlayer(true);
}

void FGsNetMessageHandlerGatewayGuild::GuildQuestInfoAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_QUEST_INFO_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_QUEST_INFO_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	GGuild()->SetQuestInfo(*pkt);

	GMessage()->GetGuild().SendMessage(MessageContentGuild::RECEIVE_QUEST_INFO, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildBuildItemCollenctionListAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_ITEM_COLLECTION_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_ITEM_COLLECTION_LIST_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		FGsUIHelper::PopupNetError(result);
		return;
	}

	GGuild()->BuildItemCollectionList(*pkt);
}

void FGsNetMessageHandlerGatewayGuild::GuildDetailAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_DETAIL_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_DETAIL_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGuild()->SetOtherGuildDetailData(pkt, false);

	if (FGsMessageHolder* msgMgr = GMessage())
	{
		msgMgr->GetGuild().SendMessage(MessageContentGuild::RECEIVE_OTHER_GUILD_DETAIL_INFO, nullptr);
		msgMgr->GetGuild().SendMessage(MessageContentGuild::INVALIDATE_GUILD_JOIN_LIST, nullptr);
	}
}

void FGsNetMessageHandlerGatewayGuild::GuildDetailErrorAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ERROR_GUILD_DETAIL_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ERROR_GUILD_DETAIL_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}
	
	PD::Result result = static_cast<PD::Result>(pkt->Result());

	if (PD::Result::GUILD_ERROR_UNKNOWN_GUILD_ID == result)
	{
		// TEXT: 기사단이 존재하지 않습니다.
		ShowMessageTicker(TEXT("GuildText"), TEXT("UI_Diplomacy_EmptyName"));

		GGuild()->SetOtherGuildDetailData(nullptr, true);

		// 해당 길드 삭제 후 리스트 갱신 요청
		GMessage()->GetGuild().SendMessage(MessageContentGuild::INVALIDATE_GUILD_JOIN_LIST, nullptr);
	}
	else
	{
		GGuild()->SetOtherGuildDetailData(nullptr, false);

		// 팝업 출력
		FGsUIHelper::PopupNetError(result);
	}
}

void FGsNetMessageHandlerGatewayGuild::GuildStorageOpenAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_OPEN_GUILD_STORE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_OPEN_GUILD_STORE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
		return;

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("GC_ACK_OPEN_GUILD_STORE Failed, Result = %d"), pkt->Result());
#endif
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		
		if (PD::Result::GUILD_ERROR_UNKNOWN == result)
		{
			// TEXT: 기사단 가입 상태인 캐릭터는 삭제 불가능합니다. 
			FText textMsg;
			if (FText::FindText(TEXT("LobbyText"), TEXT("DeleteError_GuildJoin"), textMsg))
			{
				FGsUIHelper::TrayMessageTicker(textMsg);
			}
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	GGuild()->GetMyGuildStorageData()->SetData(*pkt);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_INVALIDATE_PAGE, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildStorageItemListAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_STORE_ITEM_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_STORE_ITEM_LIST_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
		return;

	GGuild()->GetMyGuildStorageData()->SetData(*pkt);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_INVALIDATE_INVENTORY, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildStorageMemberListAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_STORE_GUILD_MEMBER_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_STORE_GUILD_MEMBER_LIST_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
		return;

	GGuild()->GetMyGuildStorageData()->SetData(*pkt);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_INVALIDATE_PAGE, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildStorageAcquiredHistoryAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_STORE_ACQUIRED_ITEM_HISTORY_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_STORE_ACQUIRED_ITEM_HISTORY_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
		return;

	GGuild()->GetMyGuildStorageData()->SetData(*pkt);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_INVALIDATE_PAGE, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildStorageDistributedHistoryAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_STORE_DISTRIBUTED_ITEM_HISTORY_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_STORE_DISTRIBUTED_ITEM_HISTORY_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
		return;

	GGuild()->GetMyGuildStorageData()->SetData(*pkt);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_INVALIDATE_PAGE, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::AckOpenGuildDungeonInfo(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_OPEN_GUILD_DUNGEON_INFO_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_OPEN_GUILD_DUNGEON_INFO_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	TArray<TSharedPtr<FGsUserData>> userList;
	for (PD::GC::PKT_GC_ACK_OPEN_GUILD_DUNGEON_INFO_READ::EnterUserListIterator iter = pkt->GetFirstEnterUserListIterator();
		iter != pkt->GetLastEnterUserListIterator();
		iter++)
	{
		TSharedPtr<FGsUserData> userData = MakeShareable(new FGsUserData());
		userData->_level = iter->Level();
		userData->_userName = iter->UserName();
		userList.Add(userData);
	}

	TArray<TSharedPtr<FGsContributionUserData>> contributionList;
	for (PD::GC::PKT_GC_ACK_OPEN_GUILD_DUNGEON_INFO_READ::RankingUserListIterator iter = pkt->GetFirstRankingUserListIterator();
		iter != pkt->GetLastRankingUserListIterator();
		iter++)
	{
		TSharedPtr<FGsContributionUserData> contributionData = MakeShareable(new FGsContributionUserData());
		contributionData->_contribution = iter->ContributePoint();
		contributionData->_userName = iter->UserName();
		contributionList.Add(contributionData);
	}

	//save guild dungeon data
	GGameData()->Set(*pkt);

	//get guild contents
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contentsManagerGame = gameFlow->GetContentsManagerGame();
	if (nullptr == contentsManagerGame)
	{
		return;
	}

	if (FGsContentsMode::InGame::ContentsGuild != contentsManagerGame->GetCurrentContentsMode())
	{
		return;
	}

	FGsGameStateGuild* gameStateGuild = contentsManagerGame->GetContentsStateObject<FGsGameStateGuild>(FGsContentsMode::InGame::ContentsGuild);
	if (nullptr == gameStateGuild)
	{
		return;
	}

	FGsGuildDungeonHandler* guildDungeonHandler = gameStateGuild->GetGuildDungeonHandler();
	if (nullptr == guildDungeonHandler)
	{
		return;
	}

	guildDungeonHandler->SetBossHpPercent(pkt->BossHpRate());
	guildDungeonHandler->SetTicketCount(pkt->TicketCount());
	guildDungeonHandler->SetContributionUserList(contributionList);
	guildDungeonHandler->SetCurrentEnterUserList(userList);

	//invalid guild dungeon
	GMessage()->GetGuild().SendMessage(MessageContentGuild::INVALID_DUNGOEN_TAB, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::InformGuildDungeonLevel(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_INFORM_GUILD_DUNGEON_LEVEL_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_INFORM_GUILD_DUNGEON_LEVEL_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	//save guild dungeon data
	GGameData()->Set(*pkt);

	GGuild()->SetDungeonBestLevel(pkt->BestLevel());

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contentsManagerGame = gameFlow->GetContentsManagerGame();
	if (nullptr == contentsManagerGame)
	{
		return;
	}

	if (FGsContentsMode::InGame::ContentsGuild != contentsManagerGame->GetCurrentContentsMode())
	{
		return;
	}

	FGsGameStateGuild* gameStateGuild = contentsManagerGame->GetContentsStateObject<FGsGameStateGuild>(FGsContentsMode::InGame::ContentsGuild);
	if (nullptr == gameStateGuild)
	{
		return;
	}

	FGsGuildDungeonHandler* guildDungeonHandler = gameStateGuild->GetGuildDungeonHandler();
	if (nullptr == guildDungeonHandler)
	{
		return;
	}

	guildDungeonHandler->SetBossHpPercent(100.0f);
	guildDungeonHandler->ClearLocalPlayerContribution();
	guildDungeonHandler->ClearEnterUserList();
	guildDungeonHandler->ClearContributionUserList();

	//invalid guild dungeon
	GMessage()->GetGuild().SendMessage(MessageContentGuild::INVALID_DUNGOEN_TAB, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildAuctionListAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_AUCTION_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_AUCTION_LIST_READ*>(InPacket.Buffer);
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

	TArray<GuildStoreItemData> auctionList;
	for (int32 offset = 0; offset < pkt->GuildStoreItemDataListSize(); offset += sizeof(GuildStoreItemData))
	{
		const GuildStoreItemData* data = reinterpret_cast<const GuildStoreItemData*>(pkt->GuildStoreItemDataList() + offset);
		auctionList.Add(*data);
	}

	auctionData->AddAuctionItemList(auctionList);
	if (auctionData->GetAuctionListCount() == 0)
	{
		auctionData->SetAuctionListCount(pkt->ListCount());
	}

	if (pkt->Completed())
	{
		FGsUIHelper::HideBlockUI();
	}
}

void FGsNetMessageHandlerGatewayGuild::GuildAuctionBidderInfoList(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_GUILD_AUCTION_BIDDER_INFO_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_GUILD_AUCTION_BIDDER_INFO_LIST_READ*>(InPacket.Buffer);
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

	FText emptyUserName;
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Drop_Character"), emptyUserName);
	FString strEmptyUserName = emptyUserName.ToString();

	for (PD::GC::PKT_GC_GUILD_AUCTION_BIDDER_INFO_LIST_READ::AuctionBidderInfoIterator iter = pkt->GetFirstAuctionBidderInfoIterator();
		iter != pkt->GetLastAuctionBidderInfoIterator(); ++iter)
	{
		if (iter->UserDBId() != INVALID_USER_DB_ID && iter->UserName().IsEmpty())
		{
			auctionData->AddUserName(iter->UserDBId(), strEmptyUserName);
			auctionData->SetAuctionBidder(iter->GuildAuctionId(), strEmptyUserName);
		}
		else
		{
			auctionData->AddUserName(iter->UserDBId(), iter->UserName());
			auctionData->SetAuctionBidder(iter->GuildAuctionId(), iter->UserName());
		}
		
	}

	GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_INVALIDATE_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildAuctionItemDetail(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();
	PD::GC::PKT_GC_ACK_GUILD_STORE_ITEM_DETAIL_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_STORE_ITEM_DETAIL_READ*>(InPacket.Buffer);
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

	FText emptyUserName;
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Drop_Character"), emptyUserName);
	FString strEmptyUserName = emptyUserName.ToString();

	TArray<UserDBId> contributorMemberList;
	for (PD::GC::PKT_GC_ACK_GUILD_STORE_ITEM_DETAIL_READ::ContributorMemberIterator iter = pkt->GetFirstContributorMemberIterator();
		iter != pkt->GetLastContributorMemberIterator(); ++iter)
	{
		if (iter->UserDBId() != INVALID_USER_DB_ID && iter->UserName().IsEmpty())
		{
			auctionData->AddUserName(iter->UserDBId(), strEmptyUserName);
		}
		else
		{
			auctionData->AddUserName(iter->UserDBId(), iter->UserName());
		}
		contributorMemberList.Emplace(iter->UserDBId());
	}

	Currency calculatedPrice = 0;
	TArray<TPair<GuildMemberGradeType, UserDBId>> calculateMemberList;
	for (PD::GC::PKT_GC_ACK_GUILD_STORE_ITEM_DETAIL_READ::CalculateMemberIterator iter = pkt->GetFirstCalculateMemberIterator();
		iter != pkt->GetLastCalculateMemberIterator(); ++iter)
	{
		if (iter->UserDBId() != INVALID_USER_DB_ID && iter->UserName().IsEmpty())
		{
			auctionData->AddUserName(iter->UserDBId(), strEmptyUserName);
		}
		else
		{
			auctionData->AddUserName(iter->UserDBId(), iter->UserName());
		}
		calculateMemberList.Emplace(iter->GradeType(), iter->UserDBId());
		calculatedPrice = iter->Currency();
	}

	auctionData->UpdateDetail(pkt->ItemDBId(), contributorMemberList, calculateMemberList, calculatedPrice);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::AUCTION_DETAIL_RECEIVE, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildDivisionListAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_STORE_ITEM_LIST_V2_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_STORE_ITEM_LIST_V2_READ*>(InPacket.Buffer);
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

	TArray<GuildStoreItemData> divisionList;
	for (int32 offset = 0; offset < pkt->GuildStoreItemDataListSize(); offset += sizeof(GuildStoreItemData))
	{
		const GuildStoreItemData* data = reinterpret_cast<const GuildStoreItemData*>(pkt->GuildStoreItemDataList() + offset);
		divisionList.Add(*data);
	}

	auctionData->AddDivisionItemList(divisionList);
	if (auctionData->GetDivisionListCount() == 0)
	{
		auctionData->SetDivisionListCount(pkt->ListCount());
	}
	else
	{
		//ShowMessageTicker(TEXT("GuildAuctionUIText"), TEXT("Division_List_Renew"));
	}
	
	if (pkt->Completed())
	{
		FGsUIHelper::HideBlockUI();

		const TArray<FGsGuildDivisionItem>& realDivisionList = auctionData->GetDivisionList();
		if (realDivisionList.Num() <= 0)
		{
			GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_INVALIDATE_LIST, nullptr);
		}
		else
		{
			// 목록 정보가 있을 경우 GUILD_STORE_ITEM_CAPTURE_INFO_LIST 에서 갱신한다.
		}
	}
}

void FGsNetMessageHandlerGatewayGuild::GuildDivisionListDetail(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_GUILD_STORE_ITEM_CAPTURE_INFO_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_GUILD_STORE_ITEM_CAPTURE_INFO_LIST_READ*>(InPacket.Buffer);
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

	FText emptyUserName;
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Drop_Character"), emptyUserName);
	FString strEmptyUserName = emptyUserName.ToString();

	for (PD::GC::PKT_GC_GUILD_STORE_ITEM_CAPTURE_INFO_LIST_READ::GuildStoreItemCaptureIterator iter = pkt->GetFirstGuildStoreItemCaptureIterator();
		iter != pkt->GetLastGuildStoreItemCaptureIterator(); ++iter)
	{
		if (iter->ProcurerUserDBId() != INVALID_USER_DB_ID && iter->ProcurerUserName().IsEmpty())
		{
			auctionData->AddUserName(iter->ProcurerUserDBId(), strEmptyUserName);
		}
		else
		{
			auctionData->AddUserName(iter->ProcurerUserDBId(), iter->ProcurerUserName());
		}

		if (iter->BidderUserDBId() != INVALID_USER_DB_ID && iter->BidderUserName().IsEmpty())
		{
			auctionData->AddUserName(iter->BidderUserDBId(), strEmptyUserName);
			auctionData->UpdateDetailCaptureInfo(iter->ItemDBId(), iter->ProcurerUserDBId(), strEmptyUserName);
		}
		else
		{
			auctionData->AddUserName(iter->BidderUserDBId(), iter->BidderUserName());
			auctionData->UpdateDetailCaptureInfo(iter->ItemDBId(), iter->ProcurerUserDBId(), iter->BidderUserName());
		}
	}

	GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_INVALIDATE_LIST, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildAuctionReturnCurrencyNotify(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_GUILD_AUCTION_RETURN_CURRENCY_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_GUILD_AUCTION_RETURN_CURRENCY_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 로그인 & 경매장 정보 요청 & 회수금 변경 때 정보 수신
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	auctionData->SetRemainPayback(pkt->Currency());
	GMessage()->GetGuild().SendMessage(MessageContentGuild::AUCTION_PAYBACK_UPDATED, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::GuildAuctionRegisterAck(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();
	PD::GC::PKT_GC_ACK_GUILD_STORE_ITEM_AUCTION_REGISTER_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_STORE_ITEM_AUCTION_REGISTER_READ*>(InPacket.Buffer);
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

	auctionData->CheckPacketError(pkt->Result());

	if (pkt->Result() != PACKET_RESULT_SUCCESS)
	{
		// 분배 목록 정보 초기화
		auctionData->ResetDivisionList();

		// 분배 탭일 경우 바로 정보 요청 및 UI 갱신
		// 다른 탭일 경우 분배 탭 들어갈 때 정보 재요청
		GMessage()->GetGuild().SendMessage(MessageContentGuild::DIVISION_REGISTER, nullptr);
	}
}

void FGsNetMessageHandlerGatewayGuild::GuildCalculateListAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_GUILD_AUCTION_CALCULATE_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_GUILD_AUCTION_CALCULATE_LIST_READ*>(InPacket.Buffer);
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

	TArray<GuildStoreItemData> itemDataList;
	for (int32 offset = 0; offset < pkt->GuildStoreItemDataListSize(); offset += sizeof(GuildStoreItemData))
	{
		const GuildStoreItemData* data = reinterpret_cast<const GuildStoreItemData*>(pkt->GuildStoreItemDataList() + offset);
		itemDataList.Add(*data);
	}

	auctionData->AddCalculateItemList(itemDataList);
	if (auctionData->GetCalculateListCount() == 0)
	{
		auctionData->SetCalculateListCount(pkt->ListCount());
	}

	if (pkt->Completed())
	{
		FGsUIHelper::HideBlockUI();
		const TArray<FGsGuildDivisionItem>& realCalculateList = auctionData->GetCalculateList();
		if (realCalculateList.Num() <= 0)
		{
			GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_INVALIDATE_LIST, nullptr);
		}
		else
		{
			// 목록 정보가 있을 경우 GUILD_STORE_ITEM_CAPTURE_INFO_LIST 에서 갱신한다.
		}
	}
}

void FGsNetMessageHandlerGatewayGuild::SanctumNexusDestructionAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_SANCTUM_NEXUS_DESTRUCTION_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_SANCTUM_NEXUS_DESTRUCTION_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	// 주의: 파괴 요청에 대한 응답이므로, 실제 파괴 노티(SC_GUILD_MEMBER_SANCTUM_NEXUS_NOTIFY)를 받고 정보를 비운다
}

void FGsNetMessageHandlerGatewayGuild::AgitDungeonRaidAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_AGIT_DUNGEON_RAID_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_AGIT_DUNGEON_RAID_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());

		// 주의: 리셋 티켓을 받으면 1초 뒤에 요청을 하는데, 이 시점에도 리셋이 안되었으면 다시 온다
		if (PD::Result::AGIT_DUNGEON_RAID_NOT_YET_RESET == result)
		{
			GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_DUNGEON_RAID_YET_RESET, nullptr);
			return;
		}

		if (FGsAgitHandler::CheckAgitDungeonError(pkt->Result()))
		{
			return;
		}

		if (FGsAgitHandler::CheckAgitError(pkt->Result()))
		{
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	if (FGsGuildManager* guildMgr = GGuild())
	{
		if (FGsAgitDataController* agitController = guildMgr->GetAgitDataController())
		{
			if (FGsAgitDungeonRaidData* raidData = agitController->GetRaidData())
			{
				raidData->SetData(pkt);
			}
		}
	}

	GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_DUNGEON_RAID_INFO, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::AgitDungeonRaidDetailAck(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_AGIT_DUNGEON_RAID_DETAIL_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_AGIT_DUNGEON_RAID_DETAIL_READ*>(InPacket.Buffer);
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
				raidData->SetDetailData(pkt);
			}
		}
	}

	FGsGuildMsgParamInt param(pkt->RaidId());
	GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_DUNGEON_RAID_DETAIL_INFO, &param);
}

void FGsNetMessageHandlerGatewayGuild::AgitDungeonRaidOpen(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_AGIT_DUNGEON_RAID_OPEN_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_AGIT_DUNGEON_RAID_OPEN_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 상태 저장
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
	
	// 갱신 코드
	FGsGuildMsgParamAgitDungeonState paramState(pkt->RaidId(), AgitDungeonRaidState::OPEN, true);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_DUNGEON_RAID_OPEN_CHANGED, &paramState);

	// 티커 및 채팅 출력
	FGsGuildMsgParamInt param(pkt->RaidId());
	GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_DUNGEON_RAID_START_NOTIFY, &param);
}

void FGsNetMessageHandlerGatewayGuild::AgitDungeonRaidClose(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_AGIT_DUNGEON_RAID_CLOSE_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_AGIT_DUNGEON_RAID_CLOSE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 상태 저장
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

	// 갱신 코드
	FGsGuildMsgParamAgitDungeonState paramState(pkt->RaidId(), AgitDungeonRaidState::CLOSED, true);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_DUNGEON_RAID_OPEN_CHANGED, &paramState);

	// 티커 및 채팅 출력
	FGsGuildMsgParamInt param(pkt->RaidId());
	GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_DUNGEON_RAID_CLOSE_NOTIFY, &param);
}

void FGsNetMessageHandlerGatewayGuild::AgitDungeonRaidClear(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_AGIT_DUNGEON_RAID_CLEAR_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_AGIT_DUNGEON_RAID_CLEAR_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 상태 저장
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

	// 갱신 코드
	FGsGuildMsgParamAgitDungeonState paramState(pkt->RaidId(), AgitDungeonRaidState::CLEAR, true);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_DUNGEON_RAID_OPEN_CHANGED, &paramState);

	// 티커 및 채팅 출력
	FGsGuildMsgParamInt param(pkt->RaidId());
	GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_DUNGEON_RAID_CLEAR_NOTIFY, &param);
}

void FGsNetMessageHandlerGatewayGuild::AgitAlertDestory(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ALERT_AGIT_DESTROY_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ALERT_AGIT_DESTROY_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsGuildMsgParamInt param(pkt->RemainSecTime());
	GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_ALERT_DESTROY, &param);

	// 영지 파괴상황. 강제 갱신 보냄
	if (0 == pkt->RemainSecTime())
	{
		GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_STATE_CHANGED, nullptr);
	}
}

void FGsNetMessageHandlerGatewayGuild::AgitExpireTimeUpdate(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_AGIT_EXPIRE_TIME_UPDATE_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_AGIT_EXPIRE_TIME_UPDATE_READ*>(InPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 정보갱신
	if (FGsAgitDataController* agitController = GGuild()->GetAgitDataController())
	{
		agitController->SetRentalEndDate(pkt->ExpireTime());
	}

	// 영지 대여/연장 성공 메시지 출력
	if (const FGsSchemaAgitConfig* config = FGsGuildHelper::GetAgitConfig())
	{
		if (pkt->IsExtension())
		{
			// TEXT: {0}님이 기사단 영지 {1}일 연장에 성공했습니다
			FText textFormat;
			if (FText::FindText(TEXT("GuildText"), TEXT("Agit_RentalExtension_Success"), textFormat))
			{
				FText textMsg = FText::Format(textFormat, 
					FText::FromString(pkt->UserName()), config->rentalExtensionPeriodD);
				
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}

		}
		else
		{
			// TEXT: {0}님이 기사단 영지 {1}일 대여에 성공했습니다
			FText textFormat;
			if (FText::FindText(TEXT("GuildText"), TEXT("Agit_Rental_Success"), textFormat))
			{
				FText textMsg = FText::Format(textFormat,
					FText::FromString(pkt->UserName()), config->rentalPeriodD);

				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}
		}
	}
	
	// 영지 페이지를 보고있을 경우를 위한 업데이트
	GMessage()->GetGuild().SendMessage(MessageContentGuild::AGIT_STATE_CHANGED, nullptr);
}

void FGsNetMessageHandlerGatewayGuild::ShowMessageTicker(const FTextKey& InNamespace, const FTextKey& InKey)
{
	FText textMsg;
	if (FText::FindText(InNamespace, InKey, textMsg))
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	}
}

bool FGsNetMessageHandlerGatewayGuild::CheckAuthorityError(PD::Result InResult)
{
	if (PD::Result::GUILD_ERROR_GRADE == InResult)
	{
		// TEXT: 권한이 없습니다.
		ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_NoAuthority"));
		return true;
	}

	return false;
}

bool FGsNetMessageHandlerGatewayGuild::CheckMemberError(PD::Result InResult)
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

void FGsNetMessageHandlerGatewayGuild::CheckDiplomacyRemotePlayer(bool bInvalidateNamePlate)
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

bool FGsNetMessageHandlerGatewayGuild::CheckGuildDiplomacyError(PD::Result InResult)
{
	switch (InResult)
	{
	case PD::Result::GUILD_ERROR_OPPONENT_FULL:
	case PD::Result::GUILD_ERROR_ERROR_FULL:
	case PD::Result::GUILD_ERROR_UNKNOWN_TARGET_GUILD_ID:
	{
		FGsUIHelper::TraySectionMessageTickerNetError(InResult);
		return true;
	}
	case PD::Result::GUILD_ERROR_ALLY_GUILD_ID:
	{
		// TEXT: 동맹 중인 기사단입니다.
		ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_Diplomacy_Already_Ally"));
		return true;
	}
	case PD::Result::GUILD_ERROR_ALLY_REQUEST_GUILD_ID:
	{
		// TEXT: 동맹 대기 중인 기사단 입니다.
		ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_Diplomacy_Already_Ally_Request"));
		return true;
	}
	case PD::Result::GUILD_ERROR_HOSTILE_GUILD_ID:
	{
		// TEXT: 적대 중인 기사단입니다.
		ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_Diplomacy_Already_Enemy"));
		return true;
	}
	case PD::Result::GUILD_ERROR_SAME_GUILD_ID:
	{
		// TEXT: 현재 가입되어 있는 기사단입니다.
		ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_Diplomacy_MyGuild"));
		return true;
	}
	case PD::Result::GUILD_ERROR_EXIST_HOSTILE_GUILD:
	{
		// TEXT: 적대 기사단이 존재하지 않습니다.
		ShowMessageTicker(TEXT("GuildText"), TEXT("UI_Diplomacy_EmptyEnemyGuild"));
		return true;
	}
	case PD::Result::GUILD_ERROR_INTER_ALLY_PREPARING:
	{
		// TEXT: 인터서버 동맹 기능은 준비중입니다.
		ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_Diplomacy_Preparing_Ally"));
		return true;
	}
	case PD::Result::GUILD_ERROR_INTER_ALLY_REQUEST_UNKNOWN_GUILD_ID:
	{
		// TEXT: 기사단이 존재하지 않습니다.
		ShowMessageTicker(TEXT("GuildText"), TEXT("GUILD_ERROR_INTER_ALLY_REQUEST_UNKNOWN_GUILD_ID"));
		return true;
	}
	case PD::Result::GUILD_ERROR_INTER_ALLY_ACCEPT_UNKNOWN_GUILD_ID:
	{
		// TEXT: 기사단이 존재하지 않습니다.
		ShowMessageTicker(TEXT("GuildText"), TEXT("GUILD_ERROR_INTER_ALLY_ACCEPT_UNKNOWN_GUILD_ID"));
		return true;
	}
	}

	return false;
}

void FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatList(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		if (data->IsSuccess())
		{
			auto pkt = data->PacketCast<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_LIST_READ>();
			auto lastIter = pkt->GetLastGroupListIterator();

			TArray<FGsGroupChatRoomInfo> rooms;
			for (auto iter = pkt->GetFirstGroupListIterator(); iter != lastIter; iter++)
			{
				rooms.Add(FGsGroupChatRoomInfo{ iter->RoomId(), iter->RoomName(), iter->GuildCount(), static_cast<GuildMemberGradeType>(iter->ChatGrade()), iter->LeaderGuildId() });
			}

			GGuild()->AddListGroupChatRooms(rooms);
		}
	}
}

void FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatInvitationList(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		if (data->IsSuccess())
		{
			auto pkt = data->PacketCast<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_INVITE_LIST_READ>();
			auto lastIter = pkt->GetLastMemberListIterator();

			TArray<FGsGuildGroupChatInviteHost> invitations;

			for (auto iter = pkt->GetFirstMemberListIterator(); iter != lastIter; iter++)
			{
				FGsGuildGroupChatMemberGuild guildData(INVALID_GUILD_DB_ID, iter->GuildName(), iter->CaptainName(), iter->GuildEmblemId(), iter->CurrentHomePlanetWorldId() );
				invitations.Add(FGsGuildGroupChatInviteHost(iter->RoomId(), iter->RoomName(), guildData));
			}

			GGuild()->AddListInvitationToGroupChat(invitations);
		}
	}
}

void FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatMemberList(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		if (data->IsSuccess())
		{
			auto pkt = data->PacketCast<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_MEMBER_LIST_READ>();
			auto lastIter = pkt->GetLastMemberListIterator();

			GuildGroupChatRoomId roomId = INVALID_GUILD_GROUP_CHAT_ROOM_ID;
			TArray<FGsGuildGroupChatMemberGuild> members;
			for (auto iter = pkt->GetFirstMemberListIterator(); iter != lastIter; iter++)
			{
				roomId = roomId == INVALID_GUILD_GROUP_CHAT_ROOM_ID ? iter->RoomId() : roomId;
				if (roomId != iter->RoomId())
				{
					GGuild()->AddListGroupMemberGuilds(roomId, members);
					roomId = iter->RoomId();
					members.Reset();
				}
				members.Add(FGsGuildGroupChatMemberGuild(iter->GuildId(), iter->GuildName(), iter->CaptainName(), iter->GuildEmblemId(), iter->CurrentHomePlanetWorldId()));
			}

			GGuild()->AddListGroupMemberGuilds(roomId, members);
		}
	}
}

void FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatReqInviteList(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		if (data->IsSuccess())
		{
			auto pkt = data->PacketCast<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_REQUEST_LIST_READ>();
			GuildGroupChatRoomId roomId = pkt->RoomId();

			auto lastIter = pkt->GetLastRequestListIterator();

			TArray<FGsGuildGroupChatMemberGuild> guests;
			for (auto iter = pkt->GetFirstRequestListIterator(); iter != lastIter; iter++)
			{
				guests.Add(FGsGuildGroupChatMemberGuild(iter->GuildId(), iter->GuildName(), iter->CaptainName(), iter->GuildEmblemId(), iter->CurrentHomePlanetWorldId()));
			}

			GGuild()->AddListInvitationGuests(roomId, guests);
		}
	}
}

void FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatRoomCreate(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		if (data->IsSuccess())
		{
			auto pkt = data->PacketCast<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_CREATE_READ>();
		}
	}
}

void FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatInvite(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		if (data->IsSuccess())
		{
			auto pkt = data->PacketCast<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_INVITE_READ>();
		}
	}
}

void FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatInviteAccepted(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		if (data->IsSuccess())
		{
			//auto pkt = data->PacketCast<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_ACCEPT_READ>();
		}
	}
}

void FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatInviteReject(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		if (data->IsSuccess())
		{
			//auto pkt = data->PacketCast<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_INVITE_REJECT_READ>();
		}
	}
}

void FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatInviteCancel(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		if (data->IsSuccess())
		{
			//auto pkt = data->PacketCast<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_INVITE_CANCEL_READ>();
		}
	}
}

void FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatKickOut(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		if (data->IsSuccess())
		{
			//auto pkt = data->PacketCast<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_KICK_READ>();
		}
	}
}

void FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatWithDraw(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		if (data->IsSuccess())
		{
			//auto pkt = data->PacketCast<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_WITHDRAWAL_READ>();
		}
	}
}

void FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatEntrustMaster(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		if (data->IsSuccess())
		{
			//auto pkt = data->PacketCast<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_DELEGATE_READ>();
		}
	}
}

void FGsNetMessageHandlerGatewayGuild::AckGuildGroupChatAuthChanged(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		if (data->IsSuccess())
		{
			//auto pkt = data->PacketCast<PD::GC::PKT_GC_ACK_GUILD_GROUP_CHAT_MODIFY_CHAT_GRADE_READ>();
		}
	}
}

// 초대 했음을 알림 - 리더길드의 외교권자
void FGsNetMessageHandlerGatewayGuild::NotiGuildGroupChatInvitaionRequest(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		auto pkt = data->PacketCast<PD::GC::PKT_GC_GUILD_GROUP_CHAT_REQUEST_READ>();
		GGuild()->OnNotificationGuestAdded(pkt->RoomId());
	}
}

void FGsNetMessageHandlerGatewayGuild::NotiMyGuildGroupChatRoomCreated(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		auto pkt = data->PacketCast<PD::GC::PKT_GC_GUILD_GROUP_CHAT_ENTERED_FOR_MY_GUILD_READ>();

		// 패킷 갱신전까지 막아둠
		if(pkt->IsCreate() == false)
		{
			FText Ticker_ChatGroup_JoinGroupChatingRoom;
			//“{0} 그룹 채팅방에 참여하였습니다.“
			FText::FindText(TEXT("ChatText"), TEXT("Ticker_ChatGroup_JoinGroupChatingRoom"), Ticker_ChatGroup_JoinGroupChatingRoom);
			FText msg = FText::Format(Ticker_ChatGroup_JoinGroupChatingRoom, FText::FromString(pkt->RoomName()));
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, msg);

			GGuild()->OnNotificationEnterGroupChat(pkt->RoomId()); //roomList, invitationList 요청
		}
		else
		{
			GGuild()->AddGroupChatRoom(
				FGsGroupChatRoomInfo{ pkt->RoomId(), pkt->RoomName(), pkt->GuildCount(), 
				static_cast<GuildMemberGradeType>(pkt->ChatGrade()),  GGuild()->GetGuildDBId()}, true);
		}
	}
}

void FGsNetMessageHandlerGatewayGuild::NotiMyGuildGuildGroupChatRoomKickedOut(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		auto pkt = data->PacketCast<PD::GC::PKT_GC_GUILD_GROUP_CHAT_KICK_READ>();
		GGuild()->RemoveGroupChatRoom(pkt->RoomId(), MessageContentChat::CHATGROUP_KICKOUT);
	}
}

void FGsNetMessageHandlerGatewayGuild::NotiOtherGuildGuildGroupChatRoomKickedOut(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		auto pkt = data->PacketCast<PD::GC::PKT_GC_GUILD_GROUP_CHAT_KICK_FOR_OTHER_GUILD_READ>();
		GGuild()->RemoveGroupMemberGuild(pkt->RoomId(), pkt->GuildId(), MessageContentChat::CHATGROUP_KICKOUT);
	}
}

void FGsNetMessageHandlerGatewayGuild::NotiMyGuildGuildGroupChatRoomWithDraw(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		auto pkt = data->PacketCast<PD::GC::PKT_GC_GUILD_GROUP_CHAT_WITHDRAWAL_READ>();
		GGuild()->RemoveGroupChatRoom(pkt->RoomId(), MessageContentChat::CHATGROUP_LEAVE);
	}
}

void FGsNetMessageHandlerGatewayGuild::NotiOtherGuildGuildGroupChatRoomWithDraw(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		auto pkt = data->PacketCast<PD::GC::PKT_GC_GUILD_GROUP_CHAT_WITHDRAWAL_FOR_OTHER_GUILD_READ>();
		GGuild()->RemoveGroupMemberGuild(pkt->RoomId(), pkt->GuildId(), MessageContentChat::CHATGROUP_LEAVE);
	}
}

void FGsNetMessageHandlerGatewayGuild::NotiGuestGuildGroupChatInvited(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		auto pkt = data->PacketCast<PD::GC::PKT_GC_GUILD_GROUP_CHAT_INVITED_READ>();
		GGuild()->OnNotificationInvitedToGroupChat(pkt->RoomId()); //RoomId가 초대장 list에 추가되었으니 갱신요구해라 reason : 초대함
	}
}

void FGsNetMessageHandlerGatewayGuild::NotiGuestGuildGroupChatInvitationRejected(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		auto pkt = data->PacketCast<PD::GC::PKT_GC_GUILD_GROUP_CHAT_INVITE_REJECT_READ>(); //초대받은 기사단의  외교 권한자 에게만
		GGuild()->RemoveInvitationToGroupChat(pkt->RoomId(), TEXT(""), MessageContentChat::CHATGROUP_INVITATION_REJECTED); //내가 받은 초대장이 초대 거절로 인해 삭제됨 이유는 : 거절
	}
}

void FGsNetMessageHandlerGatewayGuild::NotiHostGuildGroupChatInvitationRejected(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{	
		//GGuild()->RemoveInvitationGuest(RoomId(), guildId, MessageContentChat::CHATGROUP_INVITATION_REJECTED); //내가 보낸 초대장이 상대의 초대 거절로 인해 삭제됨 이유는 : 거절
	}
}

void FGsNetMessageHandlerGatewayGuild::NotiGuestGuildGroupChatInvitationCanceled(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		auto pkt = data->PacketCast<PD::GC::PKT_GC_GUILD_GROUP_CHAT_INVITE_CANCEL_READ>(); //초대받은 기사단의  외교 권한자 에게만
		GGuild()->RemoveInvitationToGroupChat(pkt->RoomId(), TEXT(""), MessageContentChat::CHATGROUP_INVITATION_CANCELD); //내가 받은 초대장이 상대의 초대 취소로 인해 삭제됨 이유는 : 취소
	}
}

void FGsNetMessageHandlerGatewayGuild::NotiHostGuildGroupChatInvitationCanceled(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		//GGuild()->RemoveInvitationGuest(RoomId(), guildId, MessageContentChat::CHATGROUP_INVITATION_CANCELD); //내가 보낸 초대장이 초대 취소로 인해 삭제됨 이유는 : 취소
	}
}

void FGsNetMessageHandlerGatewayGuild::NotiAllGuestGuildGroupChatInvitationAccepted(const FGsNet::Packet& InPacket)
{
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		auto pkt = data->PacketCast<PD::GC::PKT_GC_GUILD_GROUP_CHAT_ENTERED_FOR_OTHER_GUILD_READ>();

		GGuild()->RemoveInvitationToGroupChat(pkt->RoomId(), pkt->GuildName(), MessageContentChat::CHATGROUP_INVITATION_ACCEPTED); //내가 보낸 초대장이 승인 되었음을 모든 길드에게 알림
		GGuild()->RemoveInvitationGuest(pkt->RoomId(), pkt->GuildDBId(), MessageContentChat::CHATGROUP_INVITATION_ACCEPTED); //내가 보낸 초대장이 초대 승인으로 인해 삭제됨 이유는 : 승인
	}
}

void FGsNetMessageHandlerGatewayGuild::NotiAllGuildGroupChatMembersEntrutMaster(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		auto pkt = data->PacketCast < PD::GC::PKT_GC_GUILD_GROUP_CHAT_DELEGATE_READ>();

		GGuild()->OnNotificationEntrustedWithRoomMaster(pkt->RoomId(), pkt->GuildDBId(), pkt->GuildName());
	}
}

void FGsNetMessageHandlerGatewayGuild::NotiAllGuildGroupChatMembersAuthChanged(const FGsNet::Packet& InPacket)
{
	FGsUIHelper::HideBlockUI();
	if (const auto data = static_cast<const FGsAckPacket*>(&InPacket))
	{
		auto pkt = data->PacketCast<PD::GC::PKT_GC_GUILD_GROUP_CHAT_MODIFY_CHAT_GRADE_READ>();

		GGuild()->OnNotificaationAuthorizedChanged(pkt->RoomId(), static_cast<GuildMemberGradeType>(pkt->ChatGrade()));
	}
}