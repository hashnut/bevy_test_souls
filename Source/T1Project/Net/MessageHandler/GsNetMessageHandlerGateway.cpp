// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNetMessageHandlerGateway.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedUnlockEnum.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"

#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "Management/ScopeLobby/GsLobbyPlayableEventManager.h"

#include "Management/ScopeGame/GsCommunityEventManager.h"
#include "Management/ScopeGame/GsRankingManager.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsUserProfileManager.h"
#include "Management/ScopeGame/GsPersonalTradingManager.h"
#include "Management/ScopeGame/GsBattlePassManager.h"
#include "Management/ScopeGame/GsBuddyManager.h"
#include "Management/ScopeGame/GsSanctumManager.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateDungeon.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Dungeon/GsDungeonStateHandler.h"

#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/MessageParam/GsStageMessageParam.h"

#include "Guild/GsGuildHelper.h"
#include "Currency/GsCurrencyHelper.h"

#include "Net/GsNetSendService.h"
#include "Net/GsNetAutoLogin.h"

#include "PlayableEvent/GsPlayableEventDefine.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "GsGameInstance.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "../Plugins/PlayGuard/Source/PlayGuard/Public/PlayGuardModule.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Management/ScopeGame/GsCraftManager.h"
#include "Message/MessageParam/GsCraftMessageParam.h"
#include "Management/ScopeGame/GsMailManager.h"
#include "Message/GsMessageSystem.h"
#include "Message/GsMessageUI.h"

#include "Input/GsInputEventMsgBase.h"
#include "DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "UserContext/GsUserContextData.h"

#include "Hive/GsHiveEventMsg.h"


void FGsNetMessageHandlerGateway::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	MProtocalGateway& protocalGatewayHandler = inManager->GetProtocalGateway();

	InitializeMessageDelegateList(&protocalGatewayHandler);

	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_CREATE_USER, this, &FGsNetMessageHandlerGateway::CreateUserAck
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_DELETE_USER, this, &FGsNetMessageHandlerGateway::DeleteUserAck
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_DELETE_USER_CANCEL, this, &FGsNetMessageHandlerGateway::DeleteUserCancelAck
	));	
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_LOGIN, this, &FGsNetMessageHandlerGateway::GatewayLoginAck
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_Z_PAY_LOGIN, this, &FGsNetMessageHandlerGateway::GatewayZpayLoginAck
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_GAME_GUARD_CSA, this, &FGsNetMessageHandlerGateway::GameGuardCSA
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_LOGIN_WAITING_INFO, this, &FGsNetMessageHandlerGateway::LoginWatingInfoAck
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_LOGIN_WAITING_CANCEL, this, &FGsNetMessageHandlerGateway::LoginWatingCancelAck
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_USER_LIST, this, &FGsNetMessageHandlerGateway::UserListAck
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_SELECT_USER, this, &FGsNetMessageHandlerGateway::SelectUserAck
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_RETURN_TO_LOBBY, this, &FGsNetMessageHandlerGateway::ReturnToLobbyAck
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_WORLDTIME, this, &FGsNetMessageHandlerGateway::WorldTimeAck)
	);
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_NOTIFY_RESET_TIME_TICKET, this, &FGsNetMessageHandlerGateway::ResetTimeTicketNotify)
	);

	//운영 공지
	//AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_NOTICE, this, &FGsNetMessageHandlerGateway::OperatorNotify));
	
	// PING
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_SERVER_PING, this, &FGsNetMessageHandlerGateway::ServerPingAck)
	);

	// 랭킹 시스템 패킷
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_RANK, this, &FGsNetMessageHandlerGateway::AckRankingList)
	);

	// 컨텐츠 언락 시스템
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_UNLOCKINFO, this, &FGsNetMessageHandlerGateway::UnlockInfo)
	);
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_UPDATE_UNLOCKINFO, this, &FGsNetMessageHandlerGateway::UpdateUnlockInfoAck)
	);
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_CONTENTS_LOCKED, this, &FGsNetMessageHandlerGateway::ContentsLockedAck)
	);
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_UNLOCK_CONTENTS_LIST, this, &FGsNetMessageHandlerGateway::LockedContentsListNotify)
	);
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_NOTICE_UNLOCK_CONTENTS_UPDATE, this, &FGsNetMessageHandlerGateway::ContentsUnlockUpdateNotify)
	);

	// 플레이어블 이벤트
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_PLAYABLE_EVENT_UPDATE, this, &FGsNetMessageHandlerGateway::PlayableEventUpdateAck
	));

	// 소셜 컨텐츠 : 프로필
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_USER_PROFILE, this, &FGsNetMessageHandlerGateway::AckUserProfile
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_USER_PROFILE_INTRODUCTION_UPDATE, this, &FGsNetMessageHandlerGateway::AckUserProfileIntroductionUpdate
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_USER_PROFILE_KEYWORD_INFO, this, &FGsNetMessageHandlerGateway::AckUserProfileKeywordInfo
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_USER_PROFILE_KEYWORD_UPDATE, this, &FGsNetMessageHandlerGateway::AckUserProfileKeywordUpdate
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_USER_PROFILE_COMMENT_LIST, this, &FGsNetMessageHandlerGateway::AckUserProfileCommentList
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_USER_PROFILE_COMMENT_INPUT, this, &FGsNetMessageHandlerGateway::AckUserProfileCommentInput
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_USER_PROFILE_COMMENT_DELETE, this, &FGsNetMessageHandlerGateway::AckUserProfileCommentDelete
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_NOTIFY_USER_PROFILE, this, &FGsNetMessageHandlerGateway::NotifyUserProfile
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_USER_PROFILE_NICKNAME_CHANGE_LIST, this, &FGsNetMessageHandlerGateway::AckUserProfileNickNameChangeList
	));

	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_NOTIFY_RESET_TIME_TICKET_DUNGEON, this, &FGsNetMessageHandlerGateway::DungeonGroupResetTimeTicketNotify
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_COOP_PARTY_LIST, this, &FGsNetMessageHandlerGateway::DungeonGroupPartyListAck
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_NOTIFY_COOP_PARTY_DUNGEON_INFO, this, &FGsNetMessageHandlerGateway::PartyDungeonGroupInfo
	));
	
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_COOP_PARTY_JOIN, this, &FGsNetMessageHandlerGateway::PartyDungeonGroupJoin
	));

	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_COOP_PARTY_AUTO_MATCHING, this, &FGsNetMessageHandlerGateway::PartyDungeonGroupMathcingAck
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_COOP_PARTY_AUTO_MATCHING_CANCEL, this, &FGsNetMessageHandlerGateway::PartyDungeonGroupMathcingCancelAck
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_COOP_PARTY_AUTO_MATCHING_CANCELED, this, &FGsNetMessageHandlerGateway::PartyDungeonGroupMathcingFailed
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_COOP_PARTY_FORCED_ENTER, this, &FGsNetMessageHandlerGateway::PatyDungeonForceEnterFailed
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_NOTIFY_COOP_PARTY_START, this, &FGsNetMessageHandlerGateway::PartDungeonInDungeonNotify
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_COOP_PARTY_SET_HELP, this, &FGsNetMessageHandlerGateway::partyDungeonAskHelpAck
	));	
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_NOTIFY_COOP_PARTY_HELP_INFO, this, &FGsNetMessageHandlerGateway::partyDungeonAskHelpNotify
	));

	
	// 유저 컨텍스트
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_USER_CONTEXT_UI, this, &FGsNetMessageHandlerGateway::AckUserContext
	));

	// 커뮤니티 소녀 말풍선
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_COMMUNITY_TALK_RANK, this, &FGsNetMessageHandlerGateway::AckCommunityTalkRank
	));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_COMMUNITY_TALK_MARKET, this, &FGsNetMessageHandlerGateway::AckCommunityTalkMarket
	));

	// 서버 Revision
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_SERVER_REVISION_INFO, this, &FGsNetMessageHandlerGateway::AckServerRevisionInfo
	));

	// 서버 Revision
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_KEYBOARD_MAPPING_SAVE, this, &FGsNetMessageHandlerGateway::AckKeyboardMappingSave
	));

	// 서버 Revision
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_KEYBOARD_MAPPING_LOAD, this, &FGsNetMessageHandlerGateway::AckKeyboardMappingLoad
	));

	// 제작
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_NOTIFY_CRAFT_STAT_LIST_WORLD_CHANGED, this, &FGsNetMessageHandlerGateway::NOTIFY_CRAFT_STAT_LIST_WORLD_CHANGED));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_CRAFT_STAT_LIST, this, &FGsNetMessageHandlerGateway::ACK_CRAFT_STAT_LIST));

	// 메일
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_NOTIFY_NEW_SYSTEM_MAIL, this, &FGsNetMessageHandlerGateway::NotifyNewSystemMail
	));	

	// 계정삭제
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_DELETE_ACCOUNT, this, &FGsNetMessageHandlerGateway::AckDeleteAccount
	));

	// 외형변경 요청 결과
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_CHARACTER_CUSTOMIZING, this, &FGsNetMessageHandlerGateway::AckCharacterCustomizing
	));

	// 계정충돌 정보
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_IDP_CONFLICT_PLYER, this, &FGsNetMessageHandlerGateway::IdpConflictPlayerAck
	));	

	// 강제 접속 종료
	AddMessageDelegate(protocalGatewayHandler.AddRaw(
		PD::GC::GCPacketId::GC_ACK_LEAVE_TO_INACTIVITY, this, &FGsNetMessageHandlerGateway::AckLeaveToInActivity
	));

	// 필드보스 마지막 처치 시간 (준규 차석님 요청)
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_COMPETITION_FIELD_BOSS_LAST_DEAD_TIME,
		this, &FGsNetMessageHandlerGateway::AckCompetitionFieldBossLastDeadTime));

	// 닉네임 변경권
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_CHECK_NICKNAME_CHANGE,
		this, &FGsNetMessageHandlerGateway::AckCheckNickNameChange));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_DECIDE_NICKNAME_CHANGE,
		this, &FGsNetMessageHandlerGateway::AckDecideNickNameChange));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_NOTIFY_BUDDY_NICKNAME_CHANGE,
		this, &FGsNetMessageHandlerGateway::NotifyBuddyNickNameChange));	

	// 성소 지도 정보 요청 응답
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_MAP_SANCTUM_INFO,
		this, &FGsNetMessageHandlerGateway::AckMapSanctumInfo));

	// 서버 이전 닉네임 변경, 길드 네임 변경
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_SERVER_MIGRATE_NICKNAME_CHANGE,
		this, &FGsNetMessageHandlerGateway::AckMigrationNickNameChanged));
	AddMessageDelegate(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_SERVER_MIGRATE_GUILD_NAME_CHANGE,
		this, &FGsNetMessageHandlerGateway::AckMigrationGuildNameChanged));
}

void FGsNetMessageHandlerGateway::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerGateway::CreateUserAck(const FGsNet::Packet & inPacket)
{
	// 블럭 ui 해제
	FGsUIHelper::HideBlockUIExplicit(FName(TEXT("CreateNickName")));

	PD::GC::PKT_GC_ACK_CREATE_USER_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_CREATE_USER_READ*>(inPacket.Buffer);

	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("G_ACK_CREATE_USER Failed, Result = %d"), (int)result);
#endif
		PD::Result gcResult = static_cast<PD::Result>(result);
		if (PD::Result::CREATE_USER_ERROR_ALREADY_EXIST_NAME == gcResult)
		{
			// TEXT: 이미 사용 중인 이름입니다.
			FText textMsg;
			if (FText::FindText(TEXT("LobbyText"), TEXT("ExistentCharacterName"), textMsg))
			{
				FGsUIHelper::TrayMessageTicker(textMsg);
			}
			return;
		}
		else if (PD::Result::CREATE_USER_ERROR_NOT_ALLOWED_NAME == gcResult)
		{
			FGsUIHelper::TraySectionMessageTickerNetError(gcResult);
			return;
		}
		
		FGsUIHelper::PopupNetError(gcResult);
		return;
	}

	if (FGsNetManager* netManager = GNet())
	{
		netManager->SetAdvReservationUserName(TEXT(""));
	}
		
	FGsHiveEventMsg specialMsg(EGsHiveActionType::SPECIAL, 0);    // 인자는 테이블에 정의된 id 값을 따릅니다
	GMessage()->GetHiveEvent().SendMessage(MessageContentHiveEvent::SEND_ANALYTICS_EVENT, &specialMsg);

	// 유저 리스트를 요청한다
	FGsNetSendService::SendGatewayReqUserList();
}

void FGsNetMessageHandlerGateway::DeleteUserAck(const FGsNet::Packet & inPacket)
{
	PD::GC::PKT_GC_ACK_DELETE_USER_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_DELETE_USER_READ*>(inPacket.Buffer);

	FGsUIHelper::HideBlockUI();

	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("GC_ACK_DELETE_USER Failed, Result = %d"), (int)result);
#endif

		PD::Result gcResult = static_cast<PD::Result>(result);
		if (PD::Result::DELETE_USER_ERROR_GUILD_MEMBER_CANNOT == gcResult)
		{
			// TEXT: 기사단 가입 상태인 캐릭터는 삭제 불가능합니다.
			FText textMsg;
			if (FText::FindText(TEXT("LobbyText"), TEXT("DeleteError_GuildJoin"), textMsg))
			{
				FGsUIHelper::TrayMessageTicker(textMsg);
			}
			return;
		}
		else if (PD::Result::DELETE_USER_ERROR_NOT_ENOUGHT_LEVEL == gcResult)
		{
			if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
			{
				int32 minLevel = lobbyDataMgr->GetDeleteUserMinLevel();

				// TEXT: {0} 레벨 미만인 캐릭터는 삭제 불가능합니다.
				FText textFormat;
				if (FText::FindText(TEXT("LobbyText"), TEXT("DeleteError_Level"), textFormat))
				{
					FText textMsg = FText::Format(textFormat, minLevel);
					FGsUIHelper::TrayMessageTicker(textMsg);

					return;
				}
			}
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}
	
	FGsNetSendService::SendGatewayReqUserList();
}

void FGsNetMessageHandlerGateway::DeleteUserCancelAck(const FGsNet::Packet &inPacket)
{
	PD::GC::PKT_GC_ACK_DELETE_USER_CANCEL_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_DELETE_USER_CANCEL_READ*>(inPacket.Buffer);

	FGsUIHelper::HideBlockUI();

	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("GC_ACK_DELETE_USER_CANCEL Failed, Result = %d"), (int)result);
		return FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
	}
	
	FGsNetSendService::SendGatewayReqUserList();
}

void FGsNetMessageHandlerGateway::GatewayLoginAck(const FGsNet::Packet & inPacket)
{
	PD::GC::PKT_GC_ACK_LOGIN_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_LOGIN_READ*>(inPacket.Buffer);

	int32_t result = pkt->Result();
	
	GSLOG(Log, TEXT("NetMessageHandlerGateway recv:PKT_GC_ACK_LOGIN"));

	GNet()->ClearPacketSendBlock();

	if (result != PACKET_RESULT_SUCCESS)
	{
		// 딤드 오프
		FGsUIHelper::TrayDimmed(false);
		FGsUIHelper::HideBlockUI();

		if (PD::Result::LOGIN_ERROR_ACCOUNT_EXPIRING_RETRY_AGAIN == result)
		{
			GMessage()->GetSystem().SendMessage(MessageSystem::GATEWAY_SERVER_LOGIN_RETRY);
			return;
		}
		else
		{
			/*GSLOG(Log, TEXT("G_ACK_LOGIN Failed, Result = %d"), result);
			TSharedPtr<FGsNetBase> active = GNet()->GetActive();
			if (active.IsValid())
			{
				active->Disconnect();
			}*/
			if (PD::Result::SERVER_MIGRATE_ERROR_DATA_TRANSFER_IN_PROCESS == result)
			{
				using namespace PD;

				FText message;
				FText::FindText(TEXT("LauncherNetText"),
					*(ResultEnumToString(Result::SERVER_MIGRATE_ERROR_DATA_TRANSFER_IN_PROCESS)), message);

				FGsUIHelper::PopupSystemMsg(message);
			}
			else
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
			}

			GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);

			return;
		}
	}
	else
	{
		if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
		{
			uint16 worldID = static_cast<uint16>(GNet()->GetWorldId());
			gameUserSettings->SetUserInfoSettingByType(EGsOptionUserInfo::LAST_ENTER_WORLD_NAME, FString::FromInt(worldID));
		}

#if WITH_EDITOR || PLATFORM_WINDOWS
		GInputSettings()->SetHasServerKeyMapping(pkt->HasKeyboardMapping());			
#endif		
	}

	//bak1210 : 서버와 동기화 타이머를 세팅합니다.
	if (FGsNetManager* netManager = GNet())
	{
		netManager->SetReconnectTicket(pkt->ReconnectTicket());

		netManager->InitTimeSync(pkt->Frequency(), pkt->TickCount(), pkt->TimeNow(), pkt->TimeDilation(), pkt->UtcOffset());

		if (false == pkt->Waiting())
		{
			// 유저 생성 성공
			// 유저 리스트를 요청한다
			FGsNetSendService::SendGatewayReqUserList();
		}	
	}
}


void FGsNetMessageHandlerGateway::GatewayZpayLoginAck(const FGsNet::Packet& inPacket)
{
	GSLOG(Log, TEXT("FGsNetMessageHandlerGateway::GatewayZpayLoginAck"));

	PD::GC::PKT_GC_ACK_Z_PAY_LOGIN_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_Z_PAY_LOGIN_READ*>(inPacket.Buffer);
	int32_t result = pkt->Result();

	FGsUIHelper::TrayDimmed(false);
	FGsUIHelper::HideBlockUI();
	GNet()->ClearPacketSendBlock();

	if (result != PACKET_RESULT_SUCCESS)
	{
		if (PD::Result::LOGIN_ERROR_ACCOUNT_EXPIRING_RETRY_AGAIN == result)
		{
			GMessage()->GetSystem().SendMessage(MessageSystem::GATEWAY_SERVER_LOGIN_RETRY);
			return;
		}
		else
		{
			/*GSLOG(Log, TEXT("G_ACK_LOGIN Failed, Result = %d"), result);
			TSharedPtr<FGsNetBase> active = GNet()->GetActive();
			if (active.IsValid())
			{
			active->Disconnect();
			}*/

			if (PD::Result::ZPAY_LOGIN_ERROR_UNLOCK_CONTENTS == result)
			{
				FText errorText;
				FText::FindText(TEXT("NetText"), TEXT("ZPAY_LOGIN_ERROR_UNLOCK_CONTENTS"), errorText);
				FGsUIHelper::PopupSystemMsg(errorText, []()
					{
						GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
					});
			}
			else
			{
				FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
				GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
			}

			return;
		}
	}

	/*
	else
	{
		if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
		{
			uint16 worldID = static_cast<uint16>(GNet()->GetWorldId());
			gameUserSettings->SetUserInfoSettingByType(EGsOptionUserInfo::LAST_ENTER_WORLD_NAME, FString::FromInt(worldID));
		}

#if WITH_EDITOR || PLATFORM_WINDOWS
		GInputSettings()->SetHasServerKeyMapping(pkt->HasKeyboardMapping());
#endif		
	}
	*/

	if (FGsNetManager* netManager = GNet())
	{
		netManager->SetReconnectTicket(pkt->ReconnectTicket());
		netManager->InitTimeSync(pkt->Frequency(), pkt->TickCount(), pkt->TimeNow(), pkt->TimeDilation(), pkt->UtcOffset());
	}

	GSLOG(Error, TEXT("[GatewayZpayLoginAck] pkt->UserDBId() : %llu, pkt->UserLevel(): %llu"), pkt->UserDBId(), pkt->UserLevel());
	GBMShop()->SetZpayUserDbid(pkt->UserDBId());
	GBMShop()->SetZpayUserLevel(pkt->UserLevel());

	// Z pay 사용 처리
	FGsLobbyFlowMsgParam param(MessageStage::ENTER_Z_PAY);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
}

void FGsNetMessageHandlerGateway::GameGuardCSA(const FGsNet::Packet& inPacket)
{
	// 서버로부터 게임가드 서버인증 패킷 수신 시, 게임가드 라이브러리로 내용 전달.
	// 이 후, 콜백 함수로 결과를 받아 다시 서버로 전송함.
	PD::GC::PKT_GC_GAME_GUARD_CSA_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_GAME_GUARD_CSA_READ*>(inPacket.Buffer);
	GSLOG(Log, TEXT("NetMessageHandlerGateway recv:PKT_GC_GAME_GUARD_CSA"));

	FPlayGuardModule* PlayGuardModule = FPlayGuardModule::Get();
	if (nullptr == PlayGuardModule)
	{
		GSLOG(Error, TEXT("nullptr == PlayGuardModule"));
	}
	else
	{
		TFunction<void(const uint8*, const uint32)> OnAuth3Response = [](const uint8* InContents, const uint32 InContentsSize) -> void {
			FGsNetSendService::SendGatewayGameGuardCSA(InContents, InContentsSize);
		};

		const uchar* Contents = pkt->Packet();
		const int ContentsSize = pkt->PacketSize();
		PlayGuardModule->Auth3(Contents, ContentsSize, 0, MoveTemp(OnAuth3Response));
	}
}

void FGsNetMessageHandlerGateway::LoginWatingInfoAck(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_LOGIN_WAITING_INFO_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_LOGIN_WAITING_INFO_READ*>(inPacket.Buffer);

	GSLOG(Log, TEXT("NetMessageHandlerGateway recv:PKT_GC_LOGIN_WAITING_INFO"));
	if (nullptr == pkt)
	{
		// 딤드 오프
		FGsUIHelper::TrayDimmed(false);
		GNet()->ClearPacketSendBlock();
		FGsUIHelper::HideBlockUI();

		return;
	}

	int waitingCount = pkt->WaitingCount();
	GSLOG(Log, TEXT("GC_LOGIN_WAITING_INFO WaitingCount = %d"), waitingCount);

#if WITH_EDITOR
	UKismetSystemLibrary::PrintString(GLevel()->GetWorld(), FString::FromInt(waitingCount), true, true, FLinearColor::Yellow, 30.0f);
#endif

	if (0 < waitingCount)
	{
		// 딤드 오프
		FGsUIHelper::TrayDimmed(false);
		GNet()->ClearPacketSendBlock();
		FGsUIHelper::HideBlockUI();

		GNet()->SetLoginWaitingStart(waitingCount);
	}
	else
	{
		// 유저 생성 성공
		// 유저 리스트를 요청한다
		GNet()->SetLoginWaitingStop();
		FGsNetSendService::SendGatewayReqUserList();
	}
}

void FGsNetMessageHandlerGateway::LoginWatingCancelAck(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_LOGIN_WAITING_CANCEL_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_LOGIN_WAITING_CANCEL_READ*>(inPacket.Buffer);

	// 딤드 오프
	FGsUIHelper::TrayDimmed(false);
	GNet()->ClearPacketSendBlock();
	FGsUIHelper::HideBlockUI();

	GSLOG(Log, TEXT("NetMessageHandlerGateway recv:PKT_GC_ACK_LOGIN_WAITING_CANCEL"));
	if (nullptr == pkt)
	{
		return;
	}

	GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
}

void FGsNetMessageHandlerGateway::UserListAck(const FGsNet::Packet & inPacket)
{
	PD::GC::PKT_GC_ACK_USER_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_USER_LIST_READ*>(inPacket.Buffer);
	
	GSLOG(Log, TEXT("NetMessageHandlerGateway recv:PKT_GC_ACK_USER_LIST"));

	// 딤드 오프
	FGsUIHelper::TrayDimmed(false);
	FGsUIHelper::HideBlockUIExplicit(FName(TEXT("ReqUserList")));

	GSLOG(Log, TEXT("After FGsUIHelper::TrayDimmed"));

	FGsMessageHolder* msg = GMessage();	
	if (nullptr == msg)
	{
		GSLOG(Log, TEXT("GMessage() is nullptr"));
		return;
	}

	UGsDataContainManager* dataManager = GData();
	if (dataManager == nullptr)
	{
		GSLOG(Log, TEXT("GData() dataManager is nullptr"));
		return;
	}
	UGsGlobalConstant* globalData = dataManager->GetGlobalData();
	if (globalData == nullptr)
	{
		GSLOG(Log, TEXT("GData() globalData is nullptr"));
		return;
	}

	// 이벤트 체크플래그
	EGsPlayableEventServerCheckPointType checkPointType = (EGsPlayableEventServerCheckPointType)pkt->PlayableEventCheckPoint();
	// cheat check(not play playeable event)
	if ((int)checkPointType <= (int)EGsPlayableEventServerCheckPointType::PrePlayableMovieComplete &&
		globalData->_isPlayPlayable == false)
	{
		checkPointType = EGsPlayableEventServerCheckPointType::PlayableEventComplete;
	}

	GGameData()->SetPlayableEventCheckPoint(checkPointType);
	GSLOG(Log, TEXT("After SetPlayableEventCheckPoint"));
	
	if (FGsNetManager* netMgr = GNet())
	{	
		// 사전예약 이름
		netMgr->SetAdvReservationUserName(pkt->AdvReservationUserName());
		// 캐릭터 선택창 부터 재연결이 가능하도록 셋팅
		TSharedPtr<FGsNetBase> active = netMgr->GetActive();
		if (active.IsValid())
		{
			active->SetReconnectAble(true);
		}
	}

	if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
	{
		if (false == lobbyDataMgr->Set(*pkt))
		{
			GSLOG(Log, TEXT("Empty MyCharacter"));

			bool isPlayableTestEnter = false;
			EGsPlayableEventServerCheckPointType testCheckPoint = EGsPlayableEventServerCheckPointType::PrePlayableMovieComplete;
			if (UGsLobbyPlayableEventManager*  playableEventManager = LSLobbyPlayableEvent())
			{
				isPlayableTestEnter = playableEventManager->GetAllIsTestEnterVal();
				testCheckPoint = playableEventManager->GetTestCheckPoint();
			}

			if (false == isPlayableTestEnter)
			{
				switch (checkPointType)
				{
				case EGsPlayableEventServerCheckPointType::First:
					FGsUIHelper::TrayDimmed(true);
					GMessage()->GetStage().SendMessage(MessageStage::ENTER_PRE_PLAYABLE_MOVIE_STATE);
					break;
				case EGsPlayableEventServerCheckPointType::PrePlayableMovieComplete:
					GMessage()->GetStage().SendMessage(MessageStage::ENTER_PLAYABLE_EVENT);
					break;
				case EGsPlayableEventServerCheckPointType::PlayableEventComplete:
					GMessage()->GetStage().SendMessage(MessageStage::ENTER_POST_PLAYABLE_MOVIE_STATE);
					break;
				case EGsPlayableEventServerCheckPointType::PostPlayableMovieComplete:
					GMessage()->GetStage().SendMessage(MessageStage::ENTER_CHARACTER_EMPTY_STATE);
					break;
				case EGsPlayableEventServerCheckPointType::CharacterEmptyMovieComplete:
					GMessage()->GetStage().SendMessage(MessageStage::ENTER_CHARACTERCREATE_STAGE);
					break;
				case EGsPlayableEventServerCheckPointType::GameFirstEnterMovieComplete:					
					{
						// 모든 연출을 끝냈다.
						FGsUIHelper::TrayFadeIn(2.0);
						GMessage()->GetStage().SendMessage(MessageStage::ENTER_CHARACTERCREATE_STAGE);
					}
					break;
				default:
					break;
				}
			}
			else
			{
				GSLOG(Log, TEXT("ENTER_PLAYABLE_EVENT"));
				FGsUIHelper::TrayFadeIn(2.0);
				switch (testCheckPoint)
				{
				case EGsPlayableEventServerCheckPointType::PlayableEventComplete:
					GMessage()->GetStage().SendMessage(MessageStage::ENTER_POST_PLAYABLE_MOVIE_STATE);
					break;
				default:
					GMessage()->GetStage().SendMessage(MessageStage::ENTER_PLAYABLE_EVENT);
					break;
				}				
			}
		}
		else
		{			
			if (FGsNetManager* netManager = GNet())
			{
				if ((int32)MessageSystem::BACKTO_LOBBY == netManager->GetReturnToStageType())
				{
					GSLOG(Log, TEXT("MessageSystem::BACKTO_LOBBY && ENTER_CHARACTERSELECT_STAGE"));

					netManager->SetReturnToStageType(-1);
					
					msg->GetStage().SendMessage(MessageStage::ENTER_CHARACTERSELECT_STAGE);
					msg->GetContentsLobby().SendMessage(MessageContentsLobby::UPDATE_UI);
					return;
				}
				
			/*	if (FGsNetAutoLogin* autoLogin = netManager->GetAutoLogin())
				{
					if (autoLogin->Enter(EGsAutoConnectType::CONNECT_CHARACTER))
						return;
				}*/

				FGsUIHelper::HideBlockUI();
			}
			
			FGsUIHelper::TrayFadeIn(2.0);

			GSLOG(Log, TEXT("ENTER_CHARACTERSELECT_STAGE"));
			msg->GetStage().SendMessage(MessageStage::ENTER_CHARACTERSELECT_STAGE);
			msg->GetContentsLobby().SendMessage(MessageContentsLobby::UPDATE_UI);
		}
	}
	else
	{
		GSLOG(Log, TEXT("LSLobbyData() nullptr"));
	}
}

void FGsNetMessageHandlerGateway::SelectUserAck(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_SELECT_USER_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_SELECT_USER_READ*>(inPacket.Buffer);

	GSLOG(Log, TEXT("NetMessageHandlerGateway recv:PKT_GC_ACK_SELECT_USER"));

	// 2022/11/01 PKT - 오프라인 플레이 같은 경우 케릭터 선택 섹션을 건너 띄는 문제점 때문에 딤드가 사라지지 않았던 현상이 있음. 그래서 여기서 딤드를 한번더 꺼줌.
	FGsUIHelper::TrayDimmed(false);	
	GNet()->ClearPacketSendBlock();

	FGsUIHelper::HideBlockUIExplicit(FName(TEXT("LobbySelectUser")));
	FGsUIHelper::ForcedHideBlockUI(EGsHideBlockUIFactor::ACK_SELECT_USER);

	int32_t result = pkt->Result();

	if (result != PACKET_RESULT_SUCCESS)
	{
		// bgm 다시시작
		FGsPrimitiveUInt8 param(static_cast<uint8>(false));
		GMessage()->GetSound().SendMessage(MessageSound::PAUSE_BGM, &param);

		GSLOG(Log, TEXT("G_ACK_SELECT_USER Failed, Result = %d"), (int)result);
		GNet()->ClearPacketSendBlock();		
		return FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
	}
	else
	{
		FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("EnterIngame")));

		if (FGsNetManager* netManager = GNet())
		{
			uint64 userDBId = static_cast<uint64>(netManager->GetSelectUserDBId());
#if WITH_EDITOR
			if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
			{
				gameUserSettings->SetUserInfoSettingByType(EGsOptionUserInfo::LAST_ENTER_CHARACTER, FString::Printf(TEXT("%llu"), userDBId));
			}
#endif			
		}

		// GameFlow State Change : Lobby -> Game
		FGsMessageHolder* messageHodler = GMessage();
		check(messageHodler);

		// 월드 입장 전 로비 bgm 삭제
		GMessage()->GetSound().SendMessage(MessageSound::STOP_BGM, nullptr);

		GSLOG(Log, TEXT("GameFlow State Change ~~~ "));

		GSLOG(Log, TEXT("StartPlayGameModeWorld s"));
		GScope()->StartPlayGameModeWorld();
		GSLOG(Log, TEXT("StartPlayGameModeWorld e"));

		MSystem& system = messageHodler->GetSystem();
		GSLOG(Log, TEXT("MSystem Addr [%p]"), &system);
		system.SendMessage(MessageSystem::ENTER_INGAME);

		GSLOG(Log, TEXT("SelectUserAck End"));

		GGameData()->SetCharacterCreatedTime(pkt->CreatedTime());
	}
}

void FGsNetMessageHandlerGateway::ReturnToLobbyAck(const FGsNet::Packet &inPacket)
{
	PD::GC::PKT_GC_ACK_RETURN_TO_LOBBY_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_RETURN_TO_LOBBY_READ*>(inPacket.Buffer);

	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	EnterWorldFailReason failReason = pkt->EnterWorldFailReason();
	if (failReason != EnterWorldFailReason::EWFR_NONE)
	{
		FText textMsg;
		FText::FindText(TEXT("NetText"), TEXT("ENTER_WORLD_FAIL_REASON"), textMsg);
		textMsg = FText::Format(textMsg, static_cast<int>(failReason));

		FGsUIHelper::PopupSystemMsg(textMsg, []()
			{
				FGsMessageHolder* messageHodler = GMessage();

				FGsUIHelper::HideBlockUI();
				GMessage()->GetSystem().SendMessage(MessageSystem::BACKTO_LOBBY);
			});
	}
	else
	{
		FGsUIHelper::HideBlockUI();
		GMessage()->GetSystem().SendMessage(MessageSystem::BACKTO_LOBBY);
	}
}

void FGsNetMessageHandlerGateway::WorldTimeAck(const FGsNet::Packet& inPacket)
{
	if (PD::GC::PKT_GC_ACK_WORLDTIME_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_WORLDTIME_READ*>(inPacket.Buffer))
	{
		if (FGsNetManager* netMgr = GNet())
		{
			netMgr->SetTimeNow(pkt->TimeNow(), pkt->UtcOffset());
		}

		// 싱크 메시지 보냄
		GMessage()->GetSystem().SendMessage(MessageSystem::SYNC_WORLD_TIME);
	}
}

void FGsNetMessageHandlerGateway::ResetTimeTicketNotify(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_NOTIFY_RESET_TIME_TICKET_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_NOTIFY_RESET_TIME_TICKET_READ*>(
	inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	#if WITH_EDITOR
	FDateTime serverTime = FGsTimeSyncUtil::GetServerNowDateTime();

	GSLOG(Warning, TEXT("ResetTimeTicket: %d, serverNow: %s, dateTimeNow: %s"),
		pkt->RttType(), *serverTime.ToString(), *FDateTime::Now().ToString());
	#endif

	// TODO: 매니저 등의 데이터 리셋 추가하기
	// 길드정보 리셋
	GGuild()->ResetTimeContents(pkt->RttType());
	GSCommunityEvent()->ResetTimeContents(pkt->RttType());
	GSChat()->ResetFreeInvasionChatCount(pkt->RttType());
	GSPersonalTrading()->ResetTimeContents(pkt->RttType());
	GSBattlePass()->ResetTimeContents(pkt->RttType());
	// ...

	// 리셋 메시지 발송. 각 컨텐츠에서 받아서 리셋할 경우
	FGsPrimitiveInt32 param(static_cast<int32>(pkt->RttType()));
	GMessage()->GetSystemParam().SendMessage(MessageSystem::RESET_TIME_CONTENTS, &param);
}

void FGsNetMessageHandlerGateway::DungeonGroupResetTimeTicketNotify(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[DungeonGroup] NOTIFY_RESET_TIME_TICKET_DUNGEON_READ"));
#endif

	if (auto pkt = reinterpret_cast<PD::GC::PKT_GC_NOTIFY_RESET_TIME_TICKET_DUNGEON_READ*>(inPacket.Buffer))
	{
		//GSDungeon()->Set(*pkt);
	}
}

void FGsNetMessageHandlerGateway::DungeonGroupPartyListAck(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[DungeonGroup] ACK_COOP_PARTY_LIST_READ"));
#endif

	if (auto pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_COOP_PARTY_LIST_READ*>(inPacket.Buffer))
	{
		GSDungeon()->RecvPartyEntryList(*pkt);
	}
}

void FGsNetMessageHandlerGateway::PartyDungeonGroupInfo(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[DungeonGroup] NOTIFY_COOP_PARTY_DUNGEON_INFO"));
#endif

	PD::GC::PKT_GC_NOTIFY_COOP_PARTY_DUNGEON_INFO_READ* pkt 
		= reinterpret_cast<PD::GC::PKT_GC_NOTIFY_COOP_PARTY_DUNGEON_INFO_READ*>(inPacket.Buffer);

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateDungeon* gameStateDungeon = contents->GetContentsStateObject<FGsGameStateDungeon>(FGsContentsMode::InGame::ContentsDungeon);
	if (nullptr == gameStateDungeon)
	{
		GSLOG(Error, TEXT("nullptr == gameStateDungeon"));
		return;
	}

	if (FGsDungeonStateHandler* stateHandler = gameStateDungeon->StateHandler())
	{
		stateHandler->RecvInPartyNotify(*pkt);
	}	
}

void FGsNetMessageHandlerGateway::PartyDungeonGroupJoin(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[DungeonGroup] GC_ACK_COOP_PARTY_JOIN"));
#endif

	PD::GC::PKT_GC_ACK_COOP_PARTY_JOIN_READ* pkt
		= reinterpret_cast<PD::GC::PKT_GC_ACK_COOP_PARTY_JOIN_READ*>(inPacket.Buffer);

	// 실패 시 에러 팝업
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::TraySectionMessageTickerNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateDungeon* gameStateDungeon = contents->GetContentsStateObject<FGsGameStateDungeon>(FGsContentsMode::InGame::ContentsDungeon);
	if (nullptr == gameStateDungeon)
	{
		GSLOG(Error, TEXT("nullptr == gameStateDungeon"));
		return;
	}

	if (FGsDungeonStateHandler* stateHandler = gameStateDungeon->StateHandler())
	{
		stateHandler->RecvJoinPartyDungeon(*pkt);
	}
}

void FGsNetMessageHandlerGateway::PartyDungeonGroupMathcingAck(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[DungeonGroup] ACK_COOP_PARTY_AUTO_MATCHING"));
#endif

	PD::GC::PKT_GC_ACK_COOP_PARTY_AUTO_MATCHING_READ* pkt
		= reinterpret_cast<PD::GC::PKT_GC_ACK_COOP_PARTY_AUTO_MATCHING_READ*>(inPacket.Buffer);

	// 실패 시 에러 팝업
	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::TraySectionMessageTickerNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}	

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateDungeon* gameStateDungeon = contents->GetContentsStateObject<FGsGameStateDungeon>(FGsContentsMode::InGame::ContentsDungeon);
	if (nullptr == gameStateDungeon)
	{
		GSLOG(Error, TEXT("nullptr == gameStateDungeon"));
		return;
	}

	if (FGsDungeonStateHandler* stateHandler = gameStateDungeon->StateHandler())
	{
		stateHandler->RecvMatchingAck(*pkt);
	}
}

void FGsNetMessageHandlerGateway::PartyDungeonGroupMathcingCancelAck(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[DungeonGroup] ACK_COOP_PARTY_AUTO_MATCHING_CANCEL"));
#endif

	PD::GC::PKT_GC_ACK_COOP_PARTY_AUTO_MATCHING_CANCEL_READ* pkt
		= reinterpret_cast<PD::GC::PKT_GC_ACK_COOP_PARTY_AUTO_MATCHING_CANCEL_READ*>(inPacket.Buffer);

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateDungeon* gameStateDungeon = contents->GetContentsStateObject<FGsGameStateDungeon>(FGsContentsMode::InGame::ContentsDungeon);
	if (nullptr == gameStateDungeon)
	{
		GSLOG(Error, TEXT("nullptr == gameStateDungeon"));
		return;
	}

	if (FGsDungeonStateHandler* stateHandler = gameStateDungeon->StateHandler())
	{
		stateHandler->RecvMatchingCancelAck(*pkt);
	}
}

void FGsNetMessageHandlerGateway::PartyDungeonGroupMathcingFailed(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[DungeonGroup] COOP_PARTY_AUTO_MATCHING_CANCELED"));
#endif

	PD::GC::PKT_GC_COOP_PARTY_AUTO_MATCHING_CANCELED_READ* pkt
		= reinterpret_cast<PD::GC::PKT_GC_COOP_PARTY_AUTO_MATCHING_CANCELED_READ*>(inPacket.Buffer);

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateDungeon* gameStateDungeon = contents->GetContentsStateObject<FGsGameStateDungeon>(FGsContentsMode::InGame::ContentsDungeon);
	if (nullptr == gameStateDungeon)
	{
		GSLOG(Error, TEXT("nullptr == gameStateDungeon"));
		return;
	}

	if (FGsDungeonStateHandler* stateHandler = gameStateDungeon->StateHandler())
	{
		stateHandler->RecvMatchingFailedNotify(*pkt);
	}
}

void FGsNetMessageHandlerGateway::PatyDungeonForceEnterFailed(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[DungeonGroup] ACK_COOP_PARTY_FORCED_ENTER"));
#endif

	PD::GC::PKT_GC_ACK_COOP_PARTY_FORCED_ENTER_READ* pkt
		= reinterpret_cast<PD::GC::PKT_GC_ACK_COOP_PARTY_FORCED_ENTER_READ*>(inPacket.Buffer);

	FGsUIHelper::TraySectionMessageTickerNetError(static_cast<PD::Result>(pkt->Result()));
}

void FGsNetMessageHandlerGateway::PartDungeonInDungeonNotify(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[DungeonGroup] ACK_COOP_PARTY_FORCED_ENTER"));
#endif

	PD::GC::PKT_GC_NOTIFY_COOP_PARTY_START_READ* pkt
		= reinterpret_cast<PD::GC::PKT_GC_NOTIFY_COOP_PARTY_START_READ*>(inPacket.Buffer);

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		FGsUIHelper::TraySectionMessageTickerNetError(static_cast<PD::Result>(pkt->Result()));
		return;
	}
}

void FGsNetMessageHandlerGateway::partyDungeonAskHelpAck(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_COOP_PARTY_SET_HELP_READ* pkt
		= reinterpret_cast<PD::GC::PKT_GC_ACK_COOP_PARTY_SET_HELP_READ*>(inPacket.Buffer);

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateDungeon* gameStateDungeon = contents->GetContentsStateObject<FGsGameStateDungeon>(FGsContentsMode::InGame::ContentsDungeon);
	if (nullptr == gameStateDungeon)
	{
		GSLOG(Error, TEXT("nullptr == gameStateDungeon"));
		return;
	}

	if (FGsDungeonStateHandler* stateHandler = gameStateDungeon->StateHandler())
	{
		stateHandler->RecvPartyDungeonAskHelpAck(*pkt);
	}
}

void FGsNetMessageHandlerGateway::partyDungeonAskHelpNotify(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_NOTIFY_COOP_PARTY_HELP_INFO_READ* pkt
		= reinterpret_cast<PD::GC::PKT_GC_NOTIFY_COOP_PARTY_HELP_INFO_READ*>(inPacket.Buffer);

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateDungeon* gameStateDungeon = contents->GetContentsStateObject<FGsGameStateDungeon>(FGsContentsMode::InGame::ContentsDungeon);
	if (nullptr == gameStateDungeon)
	{
		GSLOG(Error, TEXT("nullptr == gameStateDungeon"));
		return;
	}

	if (FGsDungeonStateHandler* stateHandler = gameStateDungeon->StateHandler())
	{
		stateHandler->RecvPartyDungeonHelpInfo(*pkt);
	}
}

//void FGsNetMessageHandlerGateway::OperatorNotify(const FGsNet::Packet& inPacket)
//{
//	if (PD::GC::PKT_GC_NOTICE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_NOTICE_READ*>(inPacket.Buffer))
//	{
//		auto type = pkt->NoticeType();
//		auto noticeMsg = pkt->Notice();
//
//		//아직 type 별로 어떤 처리를 해야하는지 아직 없다.
//		FGsUIHelper::TrayOperatorMessageTicker(type, FText::FromString(noticeMsg));
//	}
//}
//

void FGsNetMessageHandlerGateway::ServerPingAck(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_SERVER_PING_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_SERVER_PING_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	uint64 pingSeq = pkt->PingSeq();	
	uint64 serverTickCount = pkt->ServerTickCount();
	FGsNetSendService::SendGatewayServerPong(pingSeq, serverTickCount);
}

void FGsNetMessageHandlerGateway::AckRankingList(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Ranking] AckRankingList"));
#endif

	PD::GC::PKT_GC_ACK_RANK_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_RANK_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
		return;

	if (FGsRankingManager* rankingManager = GSRanking())
	{
		rankingManager->Set(*pkt);
	}
}

void FGsNetMessageHandlerGateway::ShowMessageTicker(const FTextKey& InNamespace, const FTextKey& InKey)
{
	FText textMsg;
	if (FText::FindText(InNamespace, InKey, textMsg))
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	}
}

void FGsNetMessageHandlerGateway::UnlockInfo(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_UNLOCKINFO_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_UNLOCKINFO_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsUnlockManager* unlockManager = GSUnlock())
	{
		unlockManager->OnReceiveUnlockInfo(*pkt);
	}
}

void FGsNetMessageHandlerGateway::UpdateUnlockInfoAck(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_UPDATE_UNLOCKINFO_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_UPDATE_UNLOCKINFO_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}
	// 결과에 따른 행동. 에러 출력?
}

void FGsNetMessageHandlerGateway::ContentsLockedAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	
	PD::GC::PKT_GC_CONTENTS_LOCKED_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_CONTENTS_LOCKED_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsUnlockManager* unlockManager = GSUnlock();
	if (nullptr == unlockManager)
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	uiManager->CloseAllStack();

	switch (pkt->UnlockType())
	{
	case UnlockType::CONTENTS_CONDITION:
	{
		unlockManager->ShowLockTicker(pkt->ContentsId(), pkt->ContentsCondition());
		break;
	}
	case UnlockType::CATEGORY:
	{
		unlockManager->ShowLockTicker(pkt->ContentsId(), pkt->Category());
		break;
	}
	}
}

void FGsNetMessageHandlerGateway::LockedContentsListNotify(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_UNLOCK_CONTENTS_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_UNLOCK_CONTENTS_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsUnlockManager* unlockManager = GSUnlock())
	{
		unlockManager->Set(*pkt);
	}
}

void FGsNetMessageHandlerGateway::ContentsUnlockUpdateNotify(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_NOTICE_UNLOCK_CONTENTS_UPDATE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_NOTICE_UNLOCK_CONTENTS_UPDATE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsUnlockManager* unlockManager = GSUnlock())
	{
		unlockManager->Set(*pkt);
	}
}

void FGsNetMessageHandlerGateway::PlayableEventUpdateAck(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_PLAYABLE_EVENT_UPDATE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_PLAYABLE_EVENT_UPDATE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	int result = pkt->Result();
	int checkPoint = pkt->PlayableEventCheckPoint();
#if WITH_EDITOR
	GSLOG(Log, TEXT("GC_ACK_PLAYABLE_EVENT_UPDATE Result = %d, checkPoint = %d"), result, checkPoint);
#endif
}

void FGsNetMessageHandlerGateway::AckUserProfile(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_USER_PROFILE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_USER_PROFILE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsUserProfileManager* userProfileManager = GSUserProfile();
	if (nullptr == userProfileManager)
		return;

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result gcResult = static_cast<PD::Result>(pkt->Result());
		switch (gcResult)
		{
		case PD::Result::PROFILE_ERROR_INVALID_USER:
			GMessage()->GetUserProfile().SendMessage(MessageContentUserProfile::ERROR_INVALID_USER);
			ShowMessageTicker(TEXT("SocialText"), TEXT("ticker_characterMissing"));
			return;
		}

		FGsUIHelper::PopupNetError(gcResult);
		return;
	}

	userProfileManager->Set(*pkt);
}

void FGsNetMessageHandlerGateway::AckUserProfileIntroductionUpdate(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_USER_PROFILE_INTRODUCTION_UPDATE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_USER_PROFILE_INTRODUCTION_UPDATE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsUserProfileManager* userProfileManager = GSUserProfile();
	if (nullptr == userProfileManager)
		return;

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result gcResult = static_cast<PD::Result>(pkt->Result());
		FGsUIHelper::PopupNetError(gcResult);
		return;
	}

	userProfileManager->Set(*pkt);
}

void FGsNetMessageHandlerGateway::AckUserProfileKeywordInfo(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_USER_PROFILE_KEYWORD_INFO_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_USER_PROFILE_KEYWORD_INFO_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsUserProfileManager* userProfileManager = GSUserProfile();
	if (nullptr == userProfileManager)
		return;

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result gcResult = static_cast<PD::Result>(pkt->Result());
		switch (gcResult)
		{
		case PD::Result::PROFILE_ERROR_INVALID_USER:
			GMessage()->GetUserProfile().SendMessage(MessageContentUserProfile::ERROR_INVALID_USER);
			ShowMessageTicker(TEXT("SocialText"), TEXT("ticker_characterMissing"));
			return;
		}
		FGsUIHelper::PopupNetError(gcResult);
		return;
	}

	userProfileManager->Set(*pkt);
}

void FGsNetMessageHandlerGateway::AckUserProfileKeywordUpdate(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_USER_PROFILE_KEYWORD_UPDATE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_USER_PROFILE_KEYWORD_UPDATE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsUserProfileManager* userProfileManager = GSUserProfile();
	if (nullptr == userProfileManager)
		return;

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result gcResult = static_cast<PD::Result>(pkt->Result());

		if (PD::Result::PROFILE_ERROR_INVALID_USER == gcResult)
		{
			GMessage()->GetUserProfile().SendMessage(MessageContentUserProfile::ERROR_INVALID_USER);
			ShowMessageTicker(TEXT("SocialText"), TEXT("ticker_characterMissing"));
			return;
		}
		FGsUIHelper::PopupNetError(gcResult);
		return;
	}

	userProfileManager->Set(*pkt);
}

void FGsNetMessageHandlerGateway::AckUserProfileCommentList(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_USER_PROFILE_COMMENT_LIST_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_USER_PROFILE_COMMENT_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsUserProfileManager* userProfileManager = GSUserProfile();
	if (nullptr == userProfileManager)
		return;

	userProfileManager->Set(*pkt);
}

void FGsNetMessageHandlerGateway::AckUserProfileCommentInput(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_USER_PROFILE_COMMENT_INPUT_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_USER_PROFILE_COMMENT_INPUT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsUserProfileManager* userProfileManager = GSUserProfile();
	if (nullptr == userProfileManager)
		return;

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result gcResult = static_cast<PD::Result>(pkt->Result());
		switch (gcResult)
		{
		case PD::Result::PROFILE_ERROR_INVALID_USER:
			ShowMessageTicker(TEXT("SocialText"), TEXT("ticker_characterMissing"));
			return;
		case PD::Result::PROFILE_ERROR_COMMENT_FULL:
			ShowMessageTicker(TEXT("SocialText"), TEXT("comment_ticker_countMax"));
			return;
		}

		FGsUIHelper::PopupNetError(gcResult);
		return;
	}

	userProfileManager->Set(*pkt);
}

void FGsNetMessageHandlerGateway::AckUserProfileCommentDelete(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_USER_PROFILE_COMMENT_DELETE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_USER_PROFILE_COMMENT_DELETE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsUserProfileManager* userProfileManager = GSUserProfile();
	if (nullptr == userProfileManager)
		return;

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result gcResult = static_cast<PD::Result>(pkt->Result());

		switch (gcResult)
		{
		case PD::Result::PROFILE_ERROR_INVALID_USER:
			GMessage()->GetUserProfile().SendMessage(MessageContentUserProfile::ERROR_INVALID_USER);
			ShowMessageTicker(TEXT("SocialText"), TEXT("ticker_characterMissing"));
			return;
		case PD::Result::PROFILE_ERROR_COMMENT_DELETED:
			GMessage()->GetUserProfile().SendMessage(MessageContentUserProfile::INVALIDATE_COMMENT);
			return;
		}

		FGsUIHelper::PopupNetError(gcResult);
		return;
	}

	userProfileManager->Set(*pkt);
}

void FGsNetMessageHandlerGateway::NotifyUserProfile(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_NOTIFY_USER_PROFILE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_NOTIFY_USER_PROFILE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsUserProfileManager* userProfileManager = GSUserProfile();
	if (nullptr == userProfileManager)
		return;

	userProfileManager->SetRedDot(true);
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::USER_PROFILE, true);
}

void FGsNetMessageHandlerGateway::AckUserProfileNickNameChangeList(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_USER_PROFILE_NICKNAME_CHANGE_LIST_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_USER_PROFILE_NICKNAME_CHANGE_LIST_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsUserProfileManager* userProfileMgr = GSUserProfile())
	{
		userProfileMgr->SetNickNameChangeList(*pkt);
	}
}

void FGsNetMessageHandlerGateway::AckUserContext(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_USER_CONTEXT_UI_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_USER_CONTEXT_UI_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result gcResult = static_cast<PD::Result>(pkt->Result());
		switch (gcResult)
		{
		case PD::Result::USER_CONTEXT_UI_ERROR_INVALID_PROFILE_USER:
			GMessage()->GetUserContext().SendMessage(MessageContentUserContext::INVALID_USER, nullptr);
			return;
		}

		FGsUIHelper::PopupNetError(gcResult);
		return;
	}

	FGsGameDataManager* gameDataMgr = GGameData();
	if (nullptr == gameDataMgr)
		return;

	WorldId myHomeWorldId = gameDataMgr->GetUserData()->mHomeWorldId;

	FGsUserContextInfo info(*pkt, myHomeWorldId);
	FGsUIMsgParamContextUserInfo param(info);
	GMessage()->GetUserContext().SendMessage(MessageContentUserContext::VALID_USER, &param);
}

void FGsNetMessageHandlerGateway::AckCommunityTalkRank(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_COMMUNITY_TALK_RANK_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_COMMUNITY_TALK_RANK_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsTalkBalloonRankMessageParam param(pkt->TopRankGuildNameOnServer(), pkt->TopRankUserNameOnServer(),
		pkt->TopRankGuildNameOnAllServer(), pkt->TopRankUserNameOnAllServer());
	GMessage()->GetGameObject().SendMessage(MessageGameObject::TALKBALLOON_COMMUNITY_RANK, &param);
}

void FGsNetMessageHandlerGateway::AckCommunityTalkMarket(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_COMMUNITY_TALK_MARKET_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_COMMUNITY_TALK_MARKET_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	FGsTalkBalloonMarketMessageParam param(pkt->HighestPriceItemId(), pkt->HighestGradeItemId());
	GMessage()->GetGameObject().SendMessage(MessageGameObject::TALKBALLOON_COMMUNITY_MARKET, &param);
}

void FGsNetMessageHandlerGateway::AckServerRevisionInfo(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_SERVER_REVISION_INFO_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_SERVER_REVISION_INFO_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsNetManager* netManager = GNet())
	{
		netManager->SetServerRevisionInfo(pkt->ServerRevisionInfo());
	}
}

void FGsNetMessageHandlerGateway::AckKeyboardMappingSave(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_KEYBOARD_MAPPING_SAVE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_KEYBOARD_MAPPING_SAVE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	// 키매핑성공
#if WITH_EDITOR
	GSLOG(Log, TEXT("GC_ACK_KEYBOARD_MAPPING_SAVE, Result = %d"), (int)pkt->Result());
#endif
}

void FGsNetMessageHandlerGateway::AckKeyboardMappingLoad(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_KEYBOARD_MAPPING_LOAD_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_KEYBOARD_MAPPING_LOAD_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GInputSettings()->LoadServerKeyboardMappingData(*pkt);
}

void FGsNetMessageHandlerGateway::NOTIFY_CRAFT_STAT_LIST_WORLD_CHANGED(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_NOTIFY_CRAFT_STAT_LIST_WORLD_CHANGED_READ* Packet = reinterpret_cast<PD::GC::PKT_GC_NOTIFY_CRAFT_STAT_LIST_WORLD_CHANGED_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	FGsCraftMessageParamNotifyCraftStatListWorldChanged().Send();
}

void FGsNetMessageHandlerGateway::ACK_CRAFT_STAT_LIST(const FGsNet::Packet& InPacket)
{
	PD::GC::PKT_GC_ACK_CRAFT_STAT_LIST_READ* Packet = reinterpret_cast<PD::GC::PKT_GC_ACK_CRAFT_STAT_LIST_READ*>(InPacket.Buffer);
	if (nullptr == Packet)
	{
		GSLOG(Error, TEXT("nullptr == Packet"));
		return;
	}

	const CraftTabId CurrentCraftTabId = Packet->CraftTabId();
	TArray<CraftIdAmountPair> CraftStatListServer;				// : 현재까지 서버에 제작된 개수 목록.
	TArray<CraftIdAmountPair> CraftStatListServerAccount;		// : 현재까지 서버 계정에 제작된 개수 목록.

	Packet->MakeCraftStatListServerVector<TArray<CraftIdAmountPair>, CraftIdAmountPair>(CraftStatListServer);
	Packet->MakeCraftStatListAccountVector<TArray<CraftIdAmountPair>, CraftIdAmountPair>(CraftStatListServerAccount);

#if WITH_EDITOR
	FString PacketStr;
	PacketStr.Append(TEXT("FGsNetMessageHandlerCraft::ACK_CRAFT_STAT_LIST(\n"));

	FString TabName(TEXT("<Unknown>"));
	if (UGsCraftManager* CraftManager = GSCraft())
	{
		const FGsCraftTab* CraftTab = CraftManager->GetTab(CurrentCraftTabId);
		TabName = (nullptr != CraftTab) ? CraftTab->GetTabTitle().ToString() : TEXT("INVALID_TAB_NAME");
	}
	PacketStr.Appendf(TEXT(" - CraftTabId: %d (%s)\n"), CurrentCraftTabId, *TabName);

	PacketStr.Appendf(TEXT(" - CraftStatListServer.Num(): %d\n"), CraftStatListServer.Num());
	for (int32 i = 0; i < CraftStatListServer.Num(); ++i)
	{
		const CraftIdAmountPair& CraftStatListServerIt = CraftStatListServer[i];
		const UGsCraftManager* CraftManager = GSCraft();
		const FString CraftName = (nullptr != CraftManager && nullptr != CraftManager->GetCraftTarget(CraftStatListServerIt.mCraftId)) ? CraftManager->GetCraftTarget(CraftStatListServerIt.mCraftId)->GetCraftName().ToString() : TEXT("INVALID_CRAFT_NAME");
		PacketStr.Appendf(TEXT(" -- [%d] { mCraftId:%d, mItemAmount:%d} (%s)\n"), i, CraftStatListServerIt.mCraftId, CraftStatListServerIt.mItemAmount, *CraftName);
	}

	PacketStr.Appendf(TEXT(" - CraftStatListServerAccount.Num(): %d\n"), CraftStatListServerAccount.Num());
	for (int32 i = 0; i < CraftStatListServerAccount.Num(); ++i)
	{
		const CraftIdAmountPair& CraftStatListServerAccountIt = CraftStatListServerAccount[i];
		const UGsCraftManager* CraftManager = GSCraft();
		const FString CraftName = (nullptr != CraftManager && nullptr != CraftManager->GetCraftTarget(CraftStatListServerAccountIt.mCraftId)) ? CraftManager->GetCraftTarget(CraftStatListServerAccountIt.mCraftId)->GetCraftName().ToString() : TEXT("INVALID_CRAFT_NAME");
		PacketStr.Appendf(TEXT(" -- [%d] { mCraftId:%d, mItemAmount:%d} (%s)\n"), i, CraftStatListServerAccountIt.mCraftId, CraftStatListServerAccountIt.mItemAmount, *CraftName);
	}

	PacketStr.Append(TEXT(")\n"));
	GSLOG(Log, TEXT("%s"), *PacketStr);
#endif

	FGsCraftMessageParamAckCraftStatList(CurrentCraftTabId, MoveTemp(CraftStatListServer), MoveTemp(CraftStatListServerAccount)).Send();
}


void FGsNetMessageHandlerGateway::NotifyNewSystemMail(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[mail] GC_NOTIFY_NEW_SYSTEM_MAIL"));
#endif

	PD::GC::PKT_GC_NOTIFY_NEW_SYSTEM_MAIL_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_NOTIFY_NEW_SYSTEM_MAIL_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	GGameData()->Set(*pkt);
	//GMessage()->GetMail().SendMessage(MessageContentMail::NOTIFY_MAIL_UNREAD, nullptr);

	if (FGsMailManager* mailManager = GSMail())
	{
		mailManager->SendReqNewSystmeMailRecv();
	}
}

void FGsNetMessageHandlerGateway::AckDeleteAccount(const FGsNet::Packet& inPacket)
{
#if WITH_EDITOR	
	GSLOG(Log, TEXT("[Account] GC_ACK_DELETE_ACCOUNT"));
#endif

#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Ack, GateWay Server GC_ACK_DELETE_ACCOUNT"));

	FVector2D textScale{ 1.f, 1.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

#if FLOW_DUBUG_TEST
	// UI 다 닫고
	GUI()->CloseAllStack();

	FText text;
	FText::FindText(TEXT("LauncherText"), TEXT("Popup_Delete_Result"), text);
	FGsUIHelper::PopupSystemMsg(text, []()
		{
			GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
		});

	return;
#endif

	PD::GC::PKT_GC_ACK_DELETE_ACCOUNT_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_DELETE_ACCOUNT_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		GSLOG(Log, TEXT("FGsNetMessageHandlerGateway recv -> nullptr == pkt"));
		return;
	}

	int32_t result = pkt->Result();
	if (result != PD::Result::RESULT_SUCCESS)
	{
		GSLOG(Error, TEXT("FGsNetMessageHandlerGateway::AckDeleteAccount(%d)"), pkt->Result());
		return FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
	}

	// UI 다 닫고
	//GUI()->CloseAllStack();

	UGsHiveManager* hiveMgr = GHive();
	if (nullptr != hiveMgr)
	{
		hiveMgr->SetIsDeleteAccountSuccess(true);

		if (true == hiveMgr->IsSignIn())
		{
			hiveMgr->SignOut();
		}
	}

	GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);	
}

void FGsNetMessageHandlerGateway::AckCharacterCustomizing(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PD::GC::PKT_GC_ACK_CHARACTER_CUSTOMIZING_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_CHARACTER_CUSTOMIZING_READ*>(inPacket.Buffer);

	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("ACK_CHARACTER_CUSTOMIZING Failed, Result = %d"), (int)result);
#endif
		PD::Result gcResult = static_cast<PD::Result>(result);
		FGsUIHelper::PopupNetError(gcResult);
		return;
	}

	int remainCustomizingTicketCount = pkt->RemainCustomizingTicketCount();

#if WITH_EDITOR
	GSLOG(Log, TEXT("GC_ACK_CHARACTER_CUSTOMIZING Result = %d, remainCustomizingTicketCount = %d"), result, remainCustomizingTicketCount);
#endif
	// 유저 리스트를 요청한다
	FGsNetSendService::SendGatewayReqUserList();
}

void FGsNetMessageHandlerGateway::IdpConflictPlayerAck(const FGsNet::Packet& inPacket)
{
	GSLOG(Log, TEXT("FGsNetMessageHandlerGateway::IdpConflictPlayerAck()"));

	PD::GC::PKT_GC_ACK_IDP_CONFLICT_PLYER_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_IDP_CONFLICT_PLYER_READ*>(inPacket.Buffer);

	if (nullptr == pkt || pkt->Result() != PD::Result::RESULT_SUCCESS)
	{
		GSLOG(Error, TEXT("nullptr == pkt || pkt->Result() != PD::Result::RESULT_SUCCESS"));
		return;
	}

	UGsHiveManager* hiveMgr = GHive();
	if (nullptr != hiveMgr)
	{
		TArray<TMap<FString, FString>> UserInfoMapArray;
		TArray<FString> ServerNameArray;
		TArray<FString> UserNameArray;
		for (PD::GC::PKT_GC_ACK_IDP_CONFLICT_PLYER_READ::UserListIterator iterator = pkt->GetFirstUserListIterator();
			iterator != pkt->GetLastUserListIterator();
			++iterator
			)
		{
			int32 idx = UserInfoMapArray.Add(TMap<FString, FString>());

			UserInfoMapArray[idx].Add(iterator->UserInfoType0(), iterator->UserInfoValue0());
			UserInfoMapArray[idx].Add(iterator->UserInfoType1(), iterator->UserInfoValue1());

			GSLOG(Log, TEXT("UserInfoMapArray.Add(TMap<FString, FString>()) idx : %d"), idx);
			GSLOG(Log, TEXT("iterator->UserInfoType0() : %s, iterator->UserInfoValue0() : %s"), *(iterator->UserInfoType0()), *(iterator->UserInfoValue0()));
			GSLOG(Log, TEXT("iterator->UserInfoType1() : %s, iterator->UserInfoValue1() : %s"), *(iterator->UserInfoType1()), *(iterator->UserInfoValue1()));

			ServerNameArray.Add(iterator->ServerName());
			UserNameArray.Add(iterator->UserName());

			GSLOG(Log, TEXT("iterator->ServerName() : %s, iterator->UserName() : %s"), *(iterator->ServerName()), *(iterator->UserName()));
		}

		hiveMgr->ShowConflictHelperLobbyAndInGame(UserInfoMapArray, ServerNameArray, UserNameArray);
	}
}

void FGsNetMessageHandlerGateway::AckLeaveToInActivity(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_LEAVE_TO_INACTIVITY_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_LEAVE_TO_INACTIVITY_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (false == pkt->IsServerBusy())
	{
		return;
	}

	if (FGsNetManager* netManager = GNet())
	{
 		TSharedPtr<FGsNetBase> netActive = netManager->GetActive(); 		
 		netActive->OnExitGame();

		FText textForceDisconnect;
		FText::FindText(TEXT("UICommonText"), TEXT("Notice_ForceDisconnect"), textForceDisconnect);
		FGsUIHelper::PopupOK(textForceDisconnect, []() {
			if (UGsHiveManager* hiveMgr = GHive())
			{
				hiveMgr->ExitGame();
			}});
	}
}

void FGsNetMessageHandlerGateway::AckCompetitionFieldBossLastDeadTime(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_COMPETITION_FIELD_BOSS_LAST_DEAD_TIME_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_COMPETITION_FIELD_BOSS_LAST_DEAD_TIME_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	//save competiton boss last dead time to boss handler
	if (FGsGameStateBoss* stateBoss = FGsGameStateBoss::GetInstance())
	{
		stateBoss->SetLastDeadTime(pkt->LastDeadTime());
		stateBoss->InvalidateDeathTime();
	}
}

void FGsNetMessageHandlerGateway::AckCheckNickNameChange(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUIExplicit(FName(TEXT("ChangeNickNameCheck")));

	PD::GC::PKT_GC_ACK_CHECK_NICKNAME_CHANGE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_CHECK_NICKNAME_CHANGE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (CheckNickNameError(result))
		{
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	GMessage()->GetItemContents().SendMessage(MessageContentItem::CHANGE_NICKNAME_CHECK_VALID, nullptr);
}

void FGsNetMessageHandlerGateway::AckDecideNickNameChange(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUIExplicit(FName(TEXT("ChangeNickNameConfirm")));

	PD::GC::PKT_GC_ACK_DECIDE_NICKNAME_CHANGE_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_DECIDE_NICKNAME_CHANGE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		if (FGsMessageHolder* msgMgr = GMessage())
		{
			FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_COMMON_POPUP, false);
			msgMgr->GetInput().SendMessage(MessageInput::BlockInput, msg);
		}

		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		if (FGsMessageHolder* msgMgr = GMessage())
		{
			FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_COMMON_POPUP, false);
			msgMgr->GetInput().SendMessage(MessageInput::BlockInput, msg);
		}

		PD::Result result = static_cast<PD::Result>(pkt->Result());
		if (CheckNickNameError(result))
		{
			return;
		}

		FGsUIHelper::PopupNetError(result);
		return;
	}

	GMessage()->GetItemContents().SendMessage(MessageContentItem::CHANGE_NICKNAME_CONFIRM, nullptr);
}

void FGsNetMessageHandlerGateway::NotifyBuddyNickNameChange(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_NOTIFY_BUDDY_NICKNAME_CHANGE_READ* pkt = 
		reinterpret_cast<PD::GC::PKT_GC_NOTIFY_BUDDY_NICKNAME_CHANGE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (FGsBuddyManager* buddyMgr = GSBuddy())
	{
		buddyMgr->ChangeBuddyNickName(pkt->UserDBId(), pkt->UserName(), pkt->WorldId());
	}
}

bool FGsNetMessageHandlerGateway::CheckNickNameError(PD::Result InResult)
{
	FText textMsg;

	switch (InResult)
	{
	case PD::Result::NICKNAME_CHANGE_ERROR_DUPLICATION_NAME:
	{
		// TEXT: 중복된 캐릭터명입니다.
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUI_SameNickName_Ticker"), textMsg);
	}
	break;
	case PD::Result::NICKNAME_CHANGE_ERROR_INVALID_LENGTH_NAME:
	{
		// TEXT: 글자 수 조건이 맞지 않습니다.
		FText::FindText(TEXT("GuildText"), TEXT("Alert_WrongTextLength"), textMsg);
	}
	break;
	case PD::Result::NICKNAME_CHANGE_ERROR_NOT_ALLOWED_NAME:
	{
		// TEXT: 지원하지 않는 문자가 포함 되어있습니다.
		FText::FindText(TEXT("GuildText"), TEXT("Alert_WrongLanguage"), textMsg);
	}
	break;
	case PD::Result::NICKNAME_CHANGE_ERROR_NOT_TOWN:
	{
		// TEXT: 해당 아이템은 마을 이외의 지역에서는 사용이 불가능합니다.
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUI_Notice_OutOfTown"), textMsg);
	}
	break;
	case PD::Result::NICKNAME_CHANGE_ERROR_GUILD_APPLICANTS:
	{
		// TEXT: 기사단 가입 신청 상태에서는 해당 아이템을 사용할 수 없습니다.
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUI_Notice_NoGuildJoin"), textMsg);
	}
	break;
	}

	// 메시지가 있으면 티커 출력
	if (false == textMsg.IsEmpty())
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		return true;
	}

	return false;
}
void FGsNetMessageHandlerGateway::AckMapSanctumInfo(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_MAP_SANCTUM_INFO_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_MAP_SANCTUM_INFO_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
	{
		return;
	}

	if (PACKET_RESULT_SUCCESS != pkt->Result())
	{
		PD::Result resultType = static_cast<PD::Result>(pkt->Result());

		
		// 티커 처리 안된 항목 팝업 처리
		FGsUIHelper::PopupNetError(resultType);
		return;
	}

	if (FGsSanctumManager* sanctumMgr = GSSanctum())
	{
		sanctumMgr->SetMapSanctumInfo(pkt);
	}
	// 성소 정보 갱신
	GMessage()->GetUI().SendMessage(MessageUI::UPDATED_SANCTUM_INFO, nullptr);
}

void FGsNetMessageHandlerGateway::AckMigrationNickNameChanged(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_SERVER_MIGRATE_NICKNAME_CHANGE_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_SERVER_MIGRATE_NICKNAME_CHANGE_READ*>(inPacket.Buffer);

	FGsUIHelper::HideBlockUI();

	if (pkt != nullptr)
	{
		if (PACKET_RESULT_SUCCESS == pkt->Result())
		{
			FGsNetSendService::SendGatewayReqUserList();
		}
		else
		{
			PD::Result resultType = static_cast<PD::Result>(pkt->Result());
			if (CheckNickNameError(resultType))
			{
				return;
			}

			FGsUIHelper::PopupNetError(resultType);
		}
	}
}

void FGsNetMessageHandlerGateway::AckMigrationGuildNameChanged(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_ACK_SERVER_MIGRATE_GUILD_NAME_CHANGE_READ* pkt =
		reinterpret_cast<PD::GC::PKT_GC_ACK_SERVER_MIGRATE_GUILD_NAME_CHANGE_READ*>(inPacket.Buffer);

	FGsUIHelper::HideBlockUI();

	if (pkt != nullptr)
	{
		if (PACKET_RESULT_SUCCESS == pkt->Result())
		{
			FGsNetSendService::SendGatewayReqUserList();
		}
		else
		{
			PD::Result resultType = static_cast<PD::Result>(pkt->Result());
			if (PD::Result::GUILD_CREATE_ERROR_DUPLICATION_NAME == resultType)
			{
				ShowMessageTicker(TEXT("GuildText"), TEXT("Alert_SameName"));
			}
			else
			{
				if (CheckNickNameError(resultType))
				{
					return;
				}
				FGsUIHelper::PopupNetError(resultType);
			}
		}
	}
}