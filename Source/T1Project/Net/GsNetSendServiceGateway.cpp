#include "GsNetSendService.h"

#include "Shared/Shared/SharedPackets/PD_Client_Gateway.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "Management/ScopeGlobal/GsHiveManager.h"

#include "User/GsSchemaUserData.h"
#include "GsNetAutoLogin.h"
#include "Option/GsInputSettings.h"
#include "InputCoreTypes.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/GsMessageSystem.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Input/GsInputEventMsgBase.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "GameMode/GsGameModeBase.h"
#include "Engine/Engine.h"


void FGsNetSendService::SendGatewayReqLogin()
{
	if (FGsNetManager* netMgr = GNet())
	{
		netMgr->ClearPacketSendBlock();

		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();

			GSLOG(Log, TEXT("[NetGatewayLog] SendGatewayReqLogin"));
			uint64 vid = netMgr->GetVId();			
			const FGsNetManager::FGsServerSelectData& serverSelect = netMgr->GetServerSelectData();

			// 아직 window 버전은 없기에 and,ios,editor 타입만 보냄
			// 추후 window 플랫폼 도입시 수정 필요			
			FString appID = FGsNetAutoLogin::GetAppID();			
			Did did = FGsNetAutoLogin::GetDID();
			DevicePlatformType platformType = AGsGameModeBase::GetPlatformType();
			FString deviceName = FGsNetAutoLogin::GetDeviceName();
			FString hiveLanguage = FGsNetAutoLogin::GetHiveLanguage();
			FString hiveCountry = FGsNetAutoLogin::GetHiveCountry();
						
			TArray<FString> idpList;
			// #todo-hive channelType, marketType 하드코딩 정식 하이브 함수 테스트후 변경해야됨.
			FString channelType = FString::Printf(TEXT("HIVE"));
			// FString marketType = FString::Printf(TEXT("marketType"));

#if PLATFORM_ANDROID
			const FString marketType(TEXT("GO"));
#elif PLATFORM_IOS
			const FString marketType(TEXT("AP"));
#elif PLATFORM_WINDOWS
			const FString marketType(TEXT("PC"));
#else
			const FString marketType(TEXT("PC"));
#endif

			FTimespan UTCOffset = FDateTime::Now() - FDateTime::UtcNow();
			int32 timeZoneValue = UTCOffset.GetHours();

#if !WITH_EDITOR
			if (UGsHiveManager* hiveMgr = GHive())
			{
				hiveMgr->HelperGetIDPList(idpList);
				//channelType = hiveMgr->GetChannel();
				//marketType = hiveMgr->GetMarket();
			}
#endif // !WITH_EDITOR

			PD::CG::PKT_CG_REQ_LOGIN_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize()
				, serverSelect._accoutId //account id
				, vid
				, serverSelect._authTickect //authTicket
				, (uint8)platformType //platform
				, appID //app id
				, did
				, deviceName
				, hiveLanguage
				, hiveCountry
				, timeZoneValue
				, channelType
				, marketType
				, netMgr->GetClientIpAddress());

			GSLOG(Log, TEXT("[NetGatewayLog] SendGatewayReqLogin worldId : %d"), serverSelect._worldId);
			GSLOG(Log, TEXT("[NetGatewayLog] SendGatewayReqLogin accoutId : %llu"), serverSelect._accoutId);
			GSLOG(Log, TEXT("[NetGatewayLog] SendGatewayReqLogin vid : %llu"), vid);
			GSLOG(Log, TEXT("[NetGatewayLog] SendGatewayReqLogin did : %llu"), did);

			for (FString IDP : idpList)
			{
				GSLOG(Error, TEXT("[NetGatewayLog][Login] - IDP : %s"), *IDP);
				builder.CreateIdpList(IDP);
			}			
	
			netBase->Send(builder.mPktWriter.GetPktSize());
			netBase->SetPacketSendBlock(true);
		}
	}	
}

void FGsNetSendService::SendZpayLogin()
{
	if (FGsNetManager* netMgr = GNet())
	{
		netMgr->ClearPacketSendBlock();

		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();

			GSLOG(Log, TEXT("[NetGatewayLog] SendZpayLogin"));
			uint64 vid = netMgr->GetVId();
			const FGsNetManager::FGsServerSelectData& serverSelect = netMgr->GetServerSelectData();

			FString appID = FGsNetAutoLogin::GetAppID();
			DevicePlatformType platformType = AGsGameModeBase::GetPlatformType();
			FString deviceName = FGsNetAutoLogin::GetDeviceName();
		

			TArray<FString> idpList;
			// #todo-hive channelType, marketType 하드코딩 정식 하이브 함수 테스트후 변경해야됨.

			PD::CG::PKT_CG_REQ_Z_PAY_LOGIN_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize()
				, serverSelect._accoutId //account id
				, vid
				, serverSelect._authTickect //authTicket
				, (uint8)platformType //platform
				, appID //app id
				, deviceName
				, netMgr->GetClientIpAddress());

			netBase->Send(builder.mPktWriter.GetPktSize());
			netBase->SetPacketSendBlock(true);

			FGsUIHelper::ShowBlockUI();
		}
	}
}

void FGsNetSendService::SendGatewayReqLoginWaitingInfo()
{
	GSLOG(Log, TEXT("[NetGatewayLog] SendGatewayReqLoginWaitingInfo"));

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();

			PD::CG::PKT_CG_REQ_LOGIN_WAITING_INFO_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());

			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendGatewayReqLoginWaitingCancel()
{
	GSLOG(Log, TEXT("[NetGatewayLog] SendGatewayReqLoginWaitingInfo"));

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();			

			PD::CG::PKT_CG_REQ_LOGIN_WAITING_CANCEL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());

			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendGatewayGameGuardCSA(const uchar* InContents, const uint32 InContentsSize)
{
	GSLOG(Log, TEXT("[NetGatewayLog] SendGatewayGameGuardCSA"));

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();

			int sizePacket = 0;
			const uchar* packet = nullptr;
			PD::CG::PKT_CG_GAME_GUARD_CSA_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), StaticCast<int>(InContentsSize), InContents);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendGatewayReqUserList()
{
	GSLOG(Log, TEXT("[NetGatewayLog] SendGatewayReqUserList"));

	if (FGsNetManager* netMgr = GNet())
	{
		FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("ReqUserList")));
		
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();

			//GSLOG(Log, TEXT("[NetGatewayLog] SendGatewayReqUserList"));

			PD::CG::PKT_CG_REQ_USER_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());

			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendGatewayReqCreateUser(
	const FString& name, CreatureGenderType genderType, const CustomizeFaceData& faceDataSize, const CustomizeBodyData& bodyDataSize
)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();

			CreatureId creatureId = 0;
			if (const UGsTable* table = FGsSchemaUserData::GetStaticTable())
			{
				TArray<const FGsSchemaUserData*> allRow;

				if (false == table->GetAllRows(allRow))
				{
					GSLOG(Error, TEXT("[NetGatewayLog] ==== SendGatewayReqCreateUser, SchemaUserData == nullptr"));
					return;
				}

				for (const FGsSchemaUserData* data : allRow)
				{
					if (data->genderType != genderType)
						continue;
					
					creatureId = data->commonInfoId;
					break;					
				}
			}		
			
			CreatureWeaponType weaponType = CreatureWeaponType::SWORD;
			PD::CG::PKT_CG_REQ_CREATE_USER_WRITE builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), name, creatureId, weaponType, faceDataSize, bodyDataSize, false
			);

#if WITH_EDITOR
			GSLOG(Log, TEXT("[NetGatewayLog] ==== SendGatewayReqCreateUser, name.Len(): %d ===="), name.Len());
			GSLOG(Log, TEXT("[NetGatewayLog] ==== SendGatewayReqCreateUser, name: %s ===="), *name);
			GSLOG(Log, TEXT("[NetGatewayLog] ==== SendGatewayReqCreateUser, sizeof(creatureId): %d ===="), sizeof(creatureId));
			GSLOG(Log, TEXT("[NetGatewayLog] ==== SendGatewayReqCreateUser, sizeof(weaponType): %d ===="), sizeof(weaponType));
			GSLOG(Log, TEXT("[NetGatewayLog] ==== SendGatewayReqCreateUser, sizeof(faceData): %d ===="), sizeof(faceDataSize));
			GSLOG(Log, TEXT("[NetGatewayLog] ==== SendGatewayReqCreateUser, sizeof(bodyData): %d ===="), sizeof(bodyDataSize));
			GSLOG(Log, TEXT("[NetGatewayLog] ==== SendGatewayReqCreateUser, PktSize: %d ===="), builder.mPktWriter.GetPktSize());
#endif
			FGsUIHelper::ShowBlockUI();

			netBase->Send(builder.mPktWriter.GetPktSize());
			LSLobbyData()->SetNewUserName(name);
		}
	}
}

void FGsNetSendService::SendGatewayReqCustomizing(CreatureGenderType genderType, const CustomizeFaceData& faceDataSize, const CustomizeBodyData& bodyDataSize)
{
	if (FGsNetManager* netMgr = GNet())
	{
		FGsUIHelper::ShowBlockUI();

		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			UserDBId userDBId = 0;
			CreatureId creatureId = 0;

			if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
			{
				if (!lobbyDataMgr->IsChangeCustomizing())
					return;
							
				userDBId = lobbyDataMgr->GetCustomizingUserDBId();
				//lobbyDataMgr->ClearCustomizingUserDBId();				
			}

			if (const UGsTable* table = FGsSchemaUserData::GetStaticTable())
			{
				TArray<const FGsSchemaUserData*> allRow;

				if (false == table->GetAllRows(allRow))
				{
					GSLOG(Error, TEXT("[NetGatewayLog] ==== SendGatewayReqCreateUser, SchemaUserData == nullptr"));
					return;
				}

				for (const FGsSchemaUserData* data : allRow)
				{
					if (data->genderType != genderType)
						continue;

					creatureId = data->commonInfoId;
					break;
				}
			}

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			
			PD::CG::PKT_CG_REQ_CHARACTER_CUSTOMIZING_WRITE builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), userDBId, creatureId, faceDataSize, bodyDataSize);

#if WITH_EDITOR
			GSLOG(Log, TEXT("[NetGatewayLog] ==== SendGatewayReqCustomizing, userDBId : %llu ===="), userDBId);
			GSLOG(Log, TEXT("[NetGatewayLog] ==== SendGatewayReqCustomizing, creatureId: %d ===="), creatureId);
			GSLOG(Log, TEXT("[NetGatewayLog] ==== SendGatewayReqCustomizing, sizeof(faceData): %d ===="), sizeof(faceDataSize));
			GSLOG(Log, TEXT("[NetGatewayLog] ==== SendGatewayReqCustomizing, sizeof(bodyData): %d ===="), sizeof(bodyDataSize));
			GSLOG(Log, TEXT("[NetGatewayLog] ==== SendGatewayReqCustomizing, PktSize: %d ===="), builder.mPktWriter.GetPktSize());
#endif
			netBase->Send(builder.mPktWriter.GetPktSize());			
		}
	}
}

void FGsNetSendService::SendGatewayReqSelectUser(UserDBId userId)
{
	if(FGsNetManager* netMgr = GNet())
	{
		//bak1210 : BlockUI 대신 SendBlock 사용
		/*netMgr->ShowBlockUI(true);*/
		netMgr->SetSelectUserDBId(userId);
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			// bgm 멈춤
			FGsPrimitiveUInt8 param(static_cast<uint8>(true));
			GMessage()->GetSound().SendMessage(MessageSound::PAUSE_BGM, &param);

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_SELECT_USER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), userId);

			netBase->Send(builder.mPktWriter.GetPktSize());
				
			netBase->SetPacketSendBlock(true);
		}
	}
}

void FGsNetSendService::SendGatewayReqDeleteUser(UserDBId userId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		FGsUIHelper::ShowBlockUI();
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_DELETE_USER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), userId);
			GNet()->GetActive()->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendGatewayReqDeleteUserCancel(UserDBId userId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		FGsUIHelper::ShowBlockUI();
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_DELETE_USER_CANCEL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), userId);
			GNet()->GetActive()->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendGatewayLoadTopoFin(int32 areaId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		//bak1210 

		//! 중요 ! 
		/*클라이언트는 [워프 요청] ~[워프 성공 or 실패] 까지 패킷 전송을 하지 않도록 수정합니다
			- 워프 또는 워프 치트 요청 시, “패킷 전송 불가능 상태” 설정.
			- “SC_ACK_WARP_FAILURE” 또는 “SC_SPAWN_ME_COMPLETE” 수신 시, “패킷 전송 불가능 상태” 해제.

			- LoadTopoFin은 보내야하는 상황 인지... 함수호출만 하고 다시 SendBlock처리
		*/
		netMgr->ClearPacketSendBlock();


		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			//GSLOG(Log, TEXT("[NetGatewayLog] SendWorldLoadTopoFin"));
			PD::CG::PKT_CG_ACK_LOAD_TOPO_FIN_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), areaId);

			netBase->Send(builder.mPktWriter.GetPktSize(), true);

			//-LoadTopoFin은 보내야하는 상황 인지... 함수호출만 하고 다시 SendBlock처리
			netBase->SetPacketSendBlock(true);
		}
	}	
}

void FGsNetSendService::SendWorldAdminCommandQA(const FString& command, bool inSendBlock)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_ADMIN_COMMAND_QA_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), command);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendRequestReconnect()
{
	GSLOG(Log, TEXT("[NetGatewayLog] SendRequestReconnect()"));

	if (FGsNetManager* netMgr = GNet())
	{		
		GSLOG(Log, TEXT("[NetGatewayLog] FGsNetSendService::SendRequestReconnect, FGsNetManager* netMgr = GNet()"));

		netMgr->ClearPacketSendBlock();

//	재연결 시
//	hive 로그인 체크해서 로비로 보내기
#if !WITH_EDITOR		
		if (UGsHiveManager* hiveMgr = GHive())
		{
			if (false == hiveMgr->IsSignIn())
			{
				if (FGsGameFlowManager* flowManager = GMode())
				{
					if (FGsGameFlow::Mode::GAME == flowManager->GetCurrentFlowType())
					{
						GNet()->OnExitGame();
						GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
					}
				}
			}
		}
#endif

		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			ReconnectTicket ticket = netMgr->GetReconnectTicket();
			AccountDBId accountId = netMgr->GetAccoutDBId();

			PD::CG::PKT_CG_REQ_RECONNECT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				accountId, ticket);

			GSLOG(Log, TEXT("[NetGatewayLog] Reconnection TicketID [%llu] AccountID [%llu]"), ticket, accountId);

			GSLOG(Log, TEXT("[NetGatewayLog] FGsNetSendService::SendRequestReconnect, TicketID [%llu] AccountID [%llu]"), ticket, accountId);
			netBase->Send(builder.mPktWriter.GetPktSize());

			netBase->SetPacketSendBlock(true);
		}
	}
}


void FGsNetSendService::SendGatewayReturnToLobby()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_RETURN_TO_LOBBY_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());

			// 로비로 가다가 패킷을 보내 튕기는 현상 방지. GameFlow Exit 시 해제
			netBase->SetPacketSendBlock(true);
		}
	}
}

void FGsNetSendService::SendRequestWorldTime()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_WORLDTIME_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

// 현재 친구, 요청 , 초대 리스트 한번에 요청 
void FGsNetSendService::SendBuddyList()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_BUDDY_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());

#if WITH_EDITOR
			GSLOG(Log, TEXT("[NetGatewayLog][Buddy] SEND BUDDY_LIST"));
#endif
		}
	}
}

// 친구 초대 v1
void FGsNetSendService::SendBuddyInvite(const FString& inUserName)
{	
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_BUDDY_INVITE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inUserName);
			netBase->Send(builder.mPktWriter.GetPktSize());

#if WITH_EDITOR
			GSLOG(Log, TEXT("[NetGatewayLog][Buddy] SEND BUDDY_INVITE- ID: %s"), *inUserName);
#endif
		}
	}
}

// 친구 삭제
void FGsNetSendService::SendBuddyDelete(const UserDBId inUserId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_BUDDY_DELETE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inUserId);
			netBase->Send(builder.mPktWriter.GetPktSize());

#if WITH_EDITOR
			GSLOG(Log, TEXT("[NetGatewayLog][Buddy] SEND BUDDY_DELETE- ID: %llu"), inUserId);
#endif
		}
	}
}

// 친구 요청 수락
void FGsNetSendService::SendBuddyAccept(const UserDBId inUserId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_BUDDY_ACCEPT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inUserId);
			netBase->Send(builder.mPktWriter.GetPktSize());

#if WITH_EDITOR
			GSLOG(Log, TEXT("[NetGatewayLog][Buddy] SEND BUDDY_ACCEPT- ID: %llu"), inUserId);
#endif
		}
	}
}

// 친구 요청 거절 하거나 친구 추가 철회 할 때
void FGsNetSendService::SendBuddyDeleteInvitation(const UserDBId inSenderUserDBId, const UserDBId inReceiverUserDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_BUDDY_DELETE_INVITATION_WRITE builder(
						sendBuffer->GetBuffer(), sendBuffer->GetSize(), inSenderUserDBId, inReceiverUserDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());


#if WITH_EDITOR
			GSLOG(Log, TEXT("[NetGatewayLog][Buddy] SEND BUDDY_DELETE_INVITATION- SenderUserDBId: %llu, ReceiverUserDBId : %llu"), 
				inSenderUserDBId, inReceiverUserDBId);
#endif
		}
	}
}

// 친구 레드닷 삭제
void FGsNetSendService::SendBuddyTabRemoveRedDot(const BuddyTabType inTabType)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_BUDDY_TAB_REMOVE_RED_DOT_WRITE builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), inTabType);
			netBase->Send(builder.mPktWriter.GetPktSize());


#if WITH_EDITOR
			GSLOG(Log, TEXT("[NetGatewayLog][Buddy] BUDDY_TAB_REMOVE_RED_DOT- TabType : %d"), (int)inTabType);
#endif
		}
	}
}

// 친구 v2
// 현재 친구, 요청 , 초대 리스트 한번에 요청 
void FGsNetSendService::SendBuddyList_v2()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_BUDDY_LIST_V2_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			netBase->Send(builder.mPktWriter.GetPktSize());

#if WITH_EDITOR
			GSLOG(Log, TEXT("[NetGatewayLog][Buddy] SEND BUDDY_LIST_V2"));
#endif
		}
	}
}

// 친구 초대 v2
void FGsNetSendService::SendBuddyInvite_v2(WorldId inWorldId, const FString& inUserName)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_BUDDY_INVITE_V2_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inWorldId, inUserName);
			netBase->Send(builder.mPktWriter.GetPktSize());

#if WITH_EDITOR
			GSLOG(Log, TEXT("[NetGatewayLog][Buddy] SEND BUDDY_INVITE_V2- ID: %s"), *inUserName);
#endif
		}
	}
}

// 친구 삭제
void FGsNetSendService::SendBuddyDelete_v2(const UserDBId inUserId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_BUDDY_DELETE_V2_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inUserId);
			netBase->Send(builder.mPktWriter.GetPktSize());

#if WITH_EDITOR
			GSLOG(Log, TEXT("[NetGatewayLog][Buddy] SEND BUDDY_DELETE_V2- ID: %llu"), inUserId);
#endif
		}
	}
}

// 친구 요청 수락
void FGsNetSendService::SendBuddyAccept_v2(const UserDBId inUserId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_BUDDY_ACCEPT_V2_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inUserId);
			netBase->Send(builder.mPktWriter.GetPktSize());

#if WITH_EDITOR
			GSLOG(Log, TEXT("[NetGatewayLog][Buddy] SEND BUDDY_ACCEPT_V2- ID: %llu"), inUserId);
#endif
		}
	}
}

// 친구 요청 거절 하거나 친구 추가 철회 할 때
void FGsNetSendService::SendBuddyDeleteInvitation_v2(const UserDBId inSenderUserDBId, const UserDBId inReceiverUserDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_BUDDY_DELETE_INVITATION_V2_WRITE builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), inSenderUserDBId, inReceiverUserDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());


#if WITH_EDITOR
			GSLOG(Log, TEXT("[NetGatewayLog][Buddy] SEND BUDDY_DELETE_INVITATION_V2- SenderUserDBId: %llu, ReceiverUserDBId : %llu"),
				inSenderUserDBId, inReceiverUserDBId);
#endif
		}
	}
}

void FGsNetSendService::SendReqChat(ChatType chatType, const FString chatMessage, bool isPayChat /*= false*/, uint64 roomId /*=0*/)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

#if WITH_EDITOR
	GSLOG(Log, TEXT("[NetGatewayLog] FGsNetSendService::SendReqChat type [%d]: %s %s"), (int)chatType, *chatMessage, isPayChat ? TEXT("true") : TEXT("false"));
#endif

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_CHAT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), chatType, chatMessage, isPayChat, roomId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendReqWhisper(const FString toChatNick, const FString chatMessage, WorldId worldId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

#if WITH_EDITOR
	GSLOG(Log, TEXT("[NetGatewayLog] FGsNetSendService::SendReqWhisper to %s: %s %d"), *toChatNick, *chatMessage, worldId);
#endif

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_CHAT_WHISPER_V2_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), toChatNick, chatMessage, worldId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendReqReportChat(const FString& nickName, const FString& chatContent, ReportBadChatType type)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_REPORT_BAD_CHAT_WRITE builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), nickName, chatContent, type);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendReqUnblockChatUser(UserDBId uid)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_CHAT_BLACKLIST_REMOVE_WRITE builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), uid);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendReqBlockChatUser(const FString& nickName)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_CHAT_BLACKLIST_ADD_WRITE builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), nickName);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendReqCheckHomeUserExistByName(const FString& nickName)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_CHECK_USER_EXIST_WRITE  builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), nickName);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendGatewayServerPong(uint64 pingSeq, uint64 inServerTickCount)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_SERVER_PONG_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), pingSeq, inServerTickCount);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendReqRankingList(RankType InRankingType, WorldId InWorldId, int32 InMinRank)
{
	// 서버로 부터 MinRank ~ MinRank+100 개의 정보를 받는다.
	// 랭킹 컨텐츠 초기 진입 시 0 을 전송해 현재 속한 순위의 정보를 받거나 1 ~ 100위 정보를 받는다.
	if (FGsNetManager* netMgr = GNet())
	{
		FGsUIHelper::ShowBlockUI();
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_RANK_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InRankingType, InWorldId, InMinRank);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendPartyCreate(PartyDropOwnershipType In_type)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_PARTY_CREATE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), In_type);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendPartyInvite(const FString& In_inviteeName, WorldId In_homeWorldId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_PARTY_INVITE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), In_inviteeName, In_homeWorldId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}
void FGsNetSendService::SendPartyConfirmInvitation(bool In_accept)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_PARTY_CONFIRM_INVITATION_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), In_accept);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendPartyLeave()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_PARTY_LEAVE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}
void FGsNetSendService::SendPartyKick(int In_partySlotIndex)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_PARTY_KICK_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), In_partySlotIndex);
	netBase->Send(builder.mPktWriter.GetPktSize());
}
void FGsNetSendService::SendPartyChangeLeader(int In_partySlotIndex)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_PARTY_CHANGE_LEADER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), In_partySlotIndex);
	netBase->Send(builder.mPktWriter.GetPktSize());
}
void FGsNetSendService::SendPartyChangeDropOwnershipType(PartyDropOwnershipType In_type)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_PARTY_CHANGE_DROP_OWNERSHIP_TYPE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), In_type);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendReqPartyDungeonpartyList(DungeonGroupId inId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_COOP_PARTY_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendReqPartyDungeonCreateParty(DungeonGroupId InId, DungeonDifficulty InDifficulty)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_COOP_PARTY_CREATE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InId, InDifficulty);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendPartyDungeonAutoMatching(DungeonGroupId InId, DungeonDifficulty InDifficulty)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_COOP_PARTY_AUTO_MATCHING_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InId, InDifficulty);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendPartyDungeonAutoMatchingCancel()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_COOP_PARTY_AUTO_MATCHING_CANCEL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendPartyDungeonJoin(PartyId InPartyId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_COOP_PARTY_JOIN_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InPartyId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendPartyDungeonBreakInto(PartyId InPartyId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_COOP_PARTY_FORCED_ENTER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InPartyId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendPartyDungeonReady(bool InIsReady)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_COOP_PARTY_READY_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InIsReady);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendPartyDungeonEntrance()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_COOP_PARTY_START_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());

	// 파티던전으로 워프 시도 시, 단축키를 바로 막아준다
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_WAIT_SERVER_RESPONSE, true);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);
}

void FGsNetSendService::SendPartyDungeonAskHelp()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_COOP_PARTY_SET_HELP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendReqUpdateUnlockInfo(int32 InSerialNumber, uint64 InLevel)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_UPDATE_UNLOCKINFO_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InSerialNumber, InLevel);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendReqPlayableEventUpdate(int In_checkPoint)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_PLAYABLE_EVENT_UPDATE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), In_checkPoint);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendReqUserProfile(UserDBId InProfileUserDBId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsUIHelper::ShowBlockUI();
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_USER_PROFILE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InProfileUserDBId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendReqUserProfileIntroductionUpdate(const FString& InIntroductionText)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_USER_PROFILE_INTRODUCTION_UPDATE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InIntroductionText);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendReqUserProfileKeywordInfo(UserDBId InProfileUserDBId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsUIHelper::ShowBlockUI();
	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_USER_PROFILE_KEYWORD_INFO_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InProfileUserDBId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendReqUserProfileKeywordUpdate(UserDBId InProfileUserDBId, uint64 InKeywordId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_USER_PROFILE_KEYWORD_UPDATE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InProfileUserDBId, InKeywordId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendReqUserProfileCommentList(UserDBId InProfileUserDBId, time_t InLastCommentTime)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_USER_PROFILE_COMMENT_LIST_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InProfileUserDBId, InLastCommentTime);
	netBase->Send(builder.mPktWriter.GetPktSize());
}
void FGsNetSendService::SendReqUserProfileCommentInput(UserDBId InProfileUserDBId, const FString& InCommentText)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_USER_PROFILE_COMMENT_INPUT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InProfileUserDBId, InCommentText);
	netBase->Send(builder.mPktWriter.GetPktSize());
}
void FGsNetSendService::SendReqUserProfileCommentDelete(UserDBId InProfileUserDBId, UserDBId InCommentUserDBId)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_USER_PROFILE_COMMENT_DELETE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InProfileUserDBId, InCommentUserDBId);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendReqUserProfileNickNameChangeList(UserDBId InUserDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_USER_PROFILE_NICKNAME_CHANGE_LIST_WRITE builder(
				sendBuffer->GetBuffer(), sendBuffer->GetSize(), InUserDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendReqUserContext(const FString& InUserName)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_USER_CONTEXT_UI_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InUserName);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendReqCommunityTalkRank()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_COMMUNITY_TALK_RANK_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendReqCommunityTalkMarket()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_COMMUNITY_TALK_MARKET_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendKeyboardMappingSave()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_KEYBOARD_MAPPING_SAVE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());

	TMap<EGsKeyMappingType, FInputActionKeyMapping> inputActionBaseKeyMappings;
	TMap<EGsKeyMappingType, FInputActionKeyMapping> inputActionKeyMappings;
	TMap<EGsKeyMappingType, FInputAxisKeyMapping> inputAxisKeyMappings;
	GInputSettings()->GetKeyMappingDatas(inputActionBaseKeyMappings, inputActionKeyMappings, inputAxisKeyMappings);

	auto Func = [&](TMap<EGsKeyMappingType, FInputActionKeyMapping>& InMap)
	{
		for (auto& iter : InMap)
		{
			uint8 type = static_cast<uint8>(iter.Key);
			FInputActionKeyMapping& actionKeyMapping = iter.Value;

			const uint32* KeyCodePtr;
			const uint32* CharacterCodePtr;
			FInputKeyManager::Get().GetCodesFromKey(actionKeyMapping.Key, KeyCodePtr, CharacterCodePtr);
			uint32 KeyCode = KeyCodePtr ? *KeyCodePtr : 0;
			uint32 CharacterCode = CharacterCodePtr ? *CharacterCodePtr : 0;

			EGsOptionKeyActionFlags flgs = EGsOptionKeyActionFlags::NONE;
			if (actionKeyMapping.bShift)
			{
				flgs |= EGsOptionKeyActionFlags::SHIFT;
			}
			else if (actionKeyMapping.bCtrl)
			{
				flgs |= EGsOptionKeyActionFlags::CTRL;
			}
			else if (actionKeyMapping.bAlt)
			{
				flgs |= EGsOptionKeyActionFlags::ALT;
			}
			else if (actionKeyMapping.bCmd)
			{
				flgs |= EGsOptionKeyActionFlags::CMD;
			}

			builder.CreateKeyList(type, KeyCode, CharacterCode, static_cast<uint8>(flgs), 1.0f);
		}
	};

	Func(inputActionBaseKeyMappings);
	Func(inputActionKeyMappings);

	for (auto& iter : inputAxisKeyMappings)
	{
		uint8 type = static_cast<uint8>(iter.Key);
		FInputAxisKeyMapping& axisKeyMapping = iter.Value;	

		const uint32* KeyCodePtr;
		const uint32* CharacterCodePtr;
		FInputKeyManager::Get().GetCodesFromKey(axisKeyMapping.Key, KeyCodePtr, CharacterCodePtr);
		uint32 KeyCode = KeyCodePtr ? *KeyCodePtr : 0;
		uint32 CharacterCode = CharacterCodePtr ? *CharacterCodePtr : 0;

		builder.CreateKeyList(type, KeyCode, CharacterCode, static_cast<uint8>(EGsOptionKeyActionFlags::NONE), axisKeyMapping.Scale);
	}

	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendKeyboardMappingLoad()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_KEYBOARD_MAPPING_LOAD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

bool FGsNetSendService::SendReqDeleteAccount()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Send, GateWay Server DELETE_ACCOUNT"));

	FVector2D textScale{ 1.f, 1.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

#if !WITH_EDITOR
	GSLOG(Log, TEXT("[NetGatewayLog] Try to send SendReqDeleteAccount()"));

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();

			UGsHiveManager* hiveMgr = GHive();
			if (nullptr == hiveMgr)
			{
				GSLOG(Log, TEXT("[NetGatewayLog] Failed to send SendReqDeleteAccount()"));
				return false;
			}

			PD::CG::PKT_CG_REQ_DELETE_ACCOUNT_WRITE builder(
				sendBuffer->GetBuffer(),
				sendBuffer->GetSize(),
				hiveMgr->GetPlayerToken(),
				hiveMgr->GetConfigurationGetAppId(),
				FCString::Strtoui64(*hiveMgr->GetDid(), NULL, 10));
			netBase->Send(builder.mPktWriter.GetPktSize());
			GSLOG(Log, TEXT("[NetGatewayLog] Sended SendReqDeleteAccount()"));
			return true;
		}
	}

	GSLOG(Log, TEXT("[NetGatewayLog] Failed to send SendReqDeleteAccount()"));
	return false;
#else
	GSLOG(Log, TEXT("[NetGatewayLog] Skip SendReqDeleteAccount()"));

#if FLOW_DUBUG_TEST
	// UI 다 닫고
	GUI()->CloseAllStack();

	FText text;
	FText::FindText(TEXT("LauncherText"), TEXT("Popup_Delete_Result"), text);
	FGsUIHelper::PopupSystemMsg(text, []()
		{
			GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);

		});
#endif

	return true;
#endif
}


void FGsNetSendService::SendCombatAutoPlay(bool InIsAuto)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_COMBAT_AUTO_PLAY_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InIsAuto);
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendReqIdpConflictPlayer()
{
	GSLOG(Log, TEXT("[NetGatewayLog]Try to send SendReqIdpConflictPlayer()"));

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			UGsHiveManager* hiveMgr = GHive();
			if (nullptr == hiveMgr)
			{
				GSLOG(Log, TEXT("[NetGatewayLog] Failed to send SendReqIdpConflictPlayer(), HiveManager is null"));
			}

			GSLOG(Log, TEXT("[NetGatewayLog] SendReqIdpConflictPlayer() hiveMgr->GetConflictPlayer().playerId : %llu"), hiveMgr->GetConflictPlayer().playerId);

			PD::CG::PKT_CG_REQ_IDP_CONFLICT_PLYER_WRITE builder(
				sendBuffer->GetBuffer(),
				sendBuffer->GetSize(),
				hiveMgr->GetConflictPlayer().playerId);

			netBase->Send(builder.mPktWriter.GetPktSize());

			GSLOG(Log, TEXT("[NetGatewayLog] Sended SendReqIdpConflictPlayer()"));
		}
	}

	GSLOG(Log, TEXT("[NetGatewayLog] Failed to send SendReqIdpConflictPlayer()"));
}

void FGsNetSendService::SendReqLeaveToInActivity(int32 InIdleTimeSecond)
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}
	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_REQ_LEAVE_TO_INACTIVITY_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InIdleTimeSecond);
	netBase->Send(builder.mPktWriter.GetPktSize());
}


void FGsNetSendService::SendUserGameExit()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
	PD::CG::PKT_CG_USER_GAME_EXIT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
	netBase->Send(builder.mPktWriter.GetPktSize());
}

void FGsNetSendService::SendReqCheckNickNameChange(const FString& InNickName)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_CHECK_NICKNAME_CHANGE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InNickName);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendReqDecideNickNameChange(const FString& InNickName, ItemDBId InTicketItemDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_DECIDE_NICKNAME_CHANGE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InNickName, InTicketItemDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

// 성소(지도 정보 요청)
// 처음엔 FGsNetSendServiceWorld 에 있었는데
// gateway로 변경(차준규차석님 요청)
void FGsNetSendService::SendReqMapSanctumInfo(MapId In_mapId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_MAP_SANCTUM_INFO_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				In_mapId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendReqBattleArenaApply(MapId InMapId, bool InIsApply)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILDWAR_ENTRY_REGISTER_UPDATE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InMapId, InIsApply);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendReqBattleArenaEnter(MapId InMapId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_GUILDWAR_ENTER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InMapId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendReqBattleArenaLocalInfo(GuildDBId InLocalGuildDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILDWAR_MY_RANK_INFO_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InLocalGuildDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendReqBattleArenaRankInfo(GuildWarTierDataId InTierDataId, TArray<int32> InIndexSet)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CG::PKT_CG_REQ_GUILDWAR_RANK_INFO_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InTierDataId, &InIndexSet);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}