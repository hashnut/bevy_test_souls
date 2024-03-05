// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNetMessageHandlerAuth.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"

#include "Net/GsNetGame.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/GsScopeGlobal.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Management/ScopeGlobal/GsDivergenceManager.h"

#include "DataSchema/InterServer/World/GsSchemaWorldData.h"

#include "Net/GsNetSendService.h"
#include "Net/GsNetAutoLogin.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Data/GsDataContainManager.h"

#include "Message/MessageParam/GsStageMessageParam.h"
#include "Message/GsMessageStage.h"
#include "Message/GsMessageSystem.h"
#include "Shared/Shared/SharedPackets/PD_Auth_Client.h"
#include "Net/GSNet.h"
#include "../Plugins/PlayGuard/Source/PlayGuard/Public/PlayGuardModule.h"
#include "Management/ScopeGlobal/GsSoundManager.h"


using namespace PD::AC;

void FGsNetMessageHandlerAuth::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	MProtocalAuth& protocolAuth = inManager->GetProtocalAuth();

	InitializeMessageDelegateList(&protocolAuth);

	//AddMessageDelegate(GMessage()->GetProtocalAuth().AddRaw(PacketId::AC_PONG,
		//this, &FGsNetMessageHandlerLobbyAuth::PongAck));

	AddMessageDelegate(protocolAuth.AddRaw(
		ACPacketId::AC_KICKED_BY_TIMEOUT, this, &FGsNetMessageHandlerAuth::KickedByTimeOutAck
	));
	AddMessageDelegate(protocolAuth.AddRaw(
		ACPacketId::AC_ACK_PRE_LOGIN, this, &FGsNetMessageHandlerAuth::PreLoginAck
	));

	AddMessageDelegate(protocolAuth.AddRaw(
		ACPacketId::AC_ACK_LOGIN, this, &FGsNetMessageHandlerAuth::LoginAck
	));
	AddMessageDelegate(protocolAuth.AddRaw(
		ACPacketId::AC_ACK_SELECT_SERVER, this, &FGsNetMessageHandlerAuth::ServerSelectAck
	));
	AddMessageDelegate(protocolAuth.AddRaw(
		ACPacketId::AC_ACK_CHECK_PASSWORD, this, &FGsNetMessageHandlerAuth::PasswordAck
	));	
	AddMessageDelegate(protocolAuth.AddRaw(
		ACPacketId::AC_ACK_CHECK_VERSION, this, &FGsNetMessageHandlerAuth::CheckVersionAck
	));
	AddMessageDelegate(protocolAuth.AddRaw(
		ACPacketId::AC_ACK_DELETE_ACCOUNT, this, &FGsNetMessageHandlerAuth::AckDeleteAccount
	));	
	AddMessageDelegate(protocolAuth.AddRaw(
		ACPacketId::AC_ACK_APP_GUARD_AUTH, this, &FGsNetMessageHandlerAuth::AckAppGuardAuth
	));
	AddMessageDelegate(protocolAuth.AddRaw(
		ACPacketId::AC_ACK_IDP_CONFLICT_PLYER, this, &FGsNetMessageHandlerAuth::IdpConflictPlayerAck
	));	
}

void FGsNetMessageHandlerAuth::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsNetMessageHandlerAuth::KickedByTimeOutAck(const FGsNet::Packet& inPacket)
{
	//일정시간 반응이 없어 접속이 종료 되었습니다.
	GSLOG(Log, TEXT("[NetAuthLog][FGsNetMessageHandlerAuth] ===== KickedByTimeOutAck ===="));
	TSharedPtr<FGsNetBase> active = GNet()->GetActive();
	if (active.IsValid())
	{
		active->Disconnect();
	}

	FText errorText;
	FText::FindText(TEXT("LauncherNetText"), TEXT("KickedByTimeOut"), errorText);

	FGsUIHelper::PopupSystemMsg(errorText, []()
	{
		FGsUIHelper::HideBlockUI();
		GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
	});
}

void FGsNetMessageHandlerAuth::PasswordAck(const FGsNet::Packet &inPacket)
{
	PKT_AC_ACK_CHECK_PASSWORD_READ * pkt = reinterpret_cast<PKT_AC_ACK_CHECK_PASSWORD_READ*>(inPacket.Buffer);

	FGsUIHelper::HideBlockUI();
	
	int32_t result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("[NetAuthLog] AC_ACK_CHECK_PASSWORD Failed, Result = %d"), result);
		return FGsUIHelper::PopupNetError(static_cast<PD::Result>(result), true);
	}		
}

void FGsNetMessageHandlerAuth::CheckVersionAck(const FGsNet::Packet& inPacket)
{
	PKT_AC_ACK_CHECK_VERSION_READ* pkt = reinterpret_cast<PKT_AC_ACK_CHECK_VERSION_READ*>(inPacket.Buffer);

	FGsUIHelper::HideBlockUI();

	int32_t result = pkt->Result();
	uint64 clientVersion = pkt->ClientVersion();
	uint64 serverVersion = pkt->ServerVersion();

	FString versionText = FString::Format(TEXT("ClientVersion : {0}, ServerVersion : {1}"), { clientVersion, serverVersion });

	if (result != PACKET_RESULT_SUCCESS)
	{		
#if !WITH_EDITOR
		// 버젼체크가 실패하면 로그인 스테이트에 결과를 알려준다.
		GMessage()->GetSystemParam().SendMessage(MessageSystem::NET_CHECKVERSION_FAILED, nullptr);
#endif

		GSLOG(Log, TEXT("[NetAuthLog] AC_ACK_CHECK_VERSION Failed, Result = %d, ClientVersion : %llu, ServerVersion : %llu"), result, clientVersion, serverVersion);
		return FGsUIHelper::PopupNetError(static_cast<PD::Result>(result), versionText, true);
	}

	GSLOG(Log, TEXT("[NetAuthLog] AC_ACK_CHECK_VERSION -> {0}"), *versionText);

	//FGsNetSendService::SendAuthReqLogin(GNet()->GetVId());
	// 버전체크 이후 프리로그인 시도
	FGsNetSendService::SendAuthReqPreLogin(GNet()->GetVId());	
}

static void PreLoginAckCompleted(const bool InUseZpay)
{
	GMessage()->GetSystem().SendMessage(MessageSystem::NET_PRE_LOGIN_SUCCEEDED);

	// 임시 : 프리 로그인 끝나고 바로 로그인전송
	FGsNetSendService::SendAuthReqLogin(InUseZpay);
}

static void StaticQuitGame()
{
	UGsScopeHolder* ScopeHolder = GScope();
	if (nullptr != ScopeHolder)
	{
		UGsScopeGlobal* ScopeGlobal = ScopeHolder->GetGlobalScope();
		if (nullptr != ScopeGlobal)
		{
			GSLOG(Log, TEXT("StaticQuitGame() in GsNetMessageHandlerAuth"));
			ScopeGlobal->QuitGame();
			return;
		}
	}

	GSLOG(Error, TEXT("Failed StaticQuitGame() in GsNetMessageHandlerAuth"));
}

void FGsNetMessageHandlerAuth::PreLoginAck(const FGsNet::Packet& inPacket)
{
	PKT_AC_ACK_PRE_LOGIN_READ* pkt = reinterpret_cast<PKT_AC_ACK_PRE_LOGIN_READ*>(inPacket.Buffer);
	
	GSLOG(Log, TEXT("[NetAuthLog] NetMessageHandlerAuth recv:PKT_AC_ACK_PRE_LOGIN_READ: %s"), *LexToString(*pkt));

	FGsNetManager* netManager = GNet();

	int32_t result = pkt->Result();
	int32_t resultDetail = pkt->ResultDetail();
	FString hiveHostCategory = pkt->HiveHostCategory();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("[NetAuthLog] AC_ACK_PRE_LOGIN Failed, Result = %d, resultDetail = %d, hiveHostCategory = %s"), result, resultDetail, *hiveHostCategory);

		FGsUIHelper::HideBlockUI();

#if !WITH_EDITOR
		// 프리로그인에 실패 하면 로그인 스테이트에 결과를 알려준다.
		GMessage()->GetSystemParam().SendMessage(MessageSystem::NET_PRE_LOGIN_FAILED, nullptr);
#endif
		return FGsUIHelper::PopupNetError(static_cast<PD::Result>(result), static_cast<PD::Result>(resultDetail), hiveHostCategory, true);
	}

	GSLOG(Log, TEXT("[NetAuthLog] AC_ACK_PRE_LOGIN -> hiveHostCategory : %s"), *hiveHostCategory);

	if (nullptr != netManager)
	{
		netManager->SetPreLogin(pkt);
	}

	FGsBMShopManager* bmShopMgr = GBMShop();
	if (nullptr == bmShopMgr)
	{
		return;
	}

	bmShopMgr->ResetShopData();

	// Windows 플랫폼에서 현재 접속 중인지 여부
	bool IsConnectingOnWindows = false;

#if PLATFORM_ANDROID || PLATFORM_IOS
	IsConnectingOnWindows = pkt->CanZPay();
#endif

#if WITH_EDITOR
	IsConnectingOnWindows = IsConnectingOnWindows || (nullptr != GBMShop() && GBMShop()->_editorZpay);
#endif

	if (IsConnectingOnWindows)
	{
		// PC 접속중이라면, PC 접속을 끊고 모바일로 접속할 것인지 팝업으로 확인
		FText PopupMessageFormat;
		FText::FindText(TEXT("BMShopText"), TEXT("Mobile_Connect_Popup_Desc"), PopupMessageFormat);
		const FText PopupMessage = FText::Format(PopupMessageFormat, FText::FromString(pkt->AppendixName()));
		FGsUIHelper::PopupSystemYesNo(PopupMessage, [this](bool bYes)
			{
				if (bYes)
				{
					// 임시 : 프리 로그인 끝나고 바로 로그인전송
					PreLoginAckCompleted(false);
				}
				else
				{
					StaticQuitGame();
				}
			});
	}
	else
	{
		// 임시 : 프리 로그인 끝나고 바로 로그인전송
		PreLoginAckCompleted(false);
	}
}

void FGsNetMessageHandlerAuth::LoginAck(const FGsNet::Packet& inPacket)
{
	GSLOG(Log, TEXT("[NetAuthLog] FGsNetMessageHandlerAuth::LoginAck()"));

	PKT_AC_ACK_LOGIN_READ* pkt = reinterpret_cast<PKT_AC_ACK_LOGIN_READ*>(inPacket.Buffer);

	_cachedSucceededPktAcAckSelectServer.Reset();

	{
		// AppGuard 관련 처리는 로그인 결과와 상관없이 처리해야 함.
		FPlayGuardModule* PlayGuardModule = FPlayGuardModule::Get();
		const FString UniqueClientID = pkt->AppGuardUniqueClientID();
		GSLOG(Log, TEXT("[NetAuthLog] UniqueClientID: '%s'"), *UniqueClientID);
		if (nullptr != PlayGuardModule && !UniqueClientID.IsEmpty())
		{
			// 앱가드로 UniqueClientId 전달.
			PlayGuardModule->SetUniqueClientID(UniqueClientID);
		}
		else
		{
			GSLOG(Error, TEXT("[NetAuthLog] nullptr == PlayGuardModule"));
		}
	}

	FGsNetManager* netManager = GNet();

	GSLOG(Log, TEXT("[NetAuthLog] NetMessageHandlerAuth recv:PKT_AC_ACK_LOGIN"));

	int32_t result = pkt->Result();	
	int32_t resultDetail = pkt->ResultDetail();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("[NetAuthLog] A_ACK_LOGIN Failed, Result = %d"), result);

		FGsUIHelper::HideBlockUI();

#if !WITH_EDITOR
		// 로그인에 실패 하면 로그인 스테이트에 결과를 알려준다.
		GMessage()->GetSystemParam().SendMessage(MessageSystem::NET_LOGIN_FAILED, nullptr);
#endif
		return FGsUIHelper::PopupNetError(static_cast<PD::Result>(result), static_cast<PD::Result>(resultDetail), true);
	}
	
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		FString did = FString::Printf(TEXT("%llu"), GNet()->GetVId());
		gameUserSettings->SetUserInfoSettingByType(EGsOptionUserInfo::VID_NAME, did);
	}	

	if (nullptr != netManager)
	{		
		FGsUIHelper::HideBlockUI();

		if (false == netManager->SetServerList(pkt))
		{
			// 활성화 된 서버가 존재하지 않는다.
			FGsUIHelper::PopupNetError(TEXT("cannotfindserver"));
			return;
		}

		/*if (FGsNetAutoLogin* autoLogin = netManager->GetAutoLogin())
		{
			if (autoLogin->Enter(EGsAutoConnectType::CONNECT_SERVER))
			{
				return;
			}
		}*/
	}

	GMessage()->GetSystemParam().SendMessage(MessageSystem::NET_LOGIN_SUCCEEDED, nullptr);
}

void FGsNetMessageHandlerAuth::ServerSelectAck(const FGsNet::Packet& inPacket)
{
	GSLOG(Log, TEXT("[NetAuthLog] FGsNetMessageHandlerAuth::ServerSelectAck()"));

	PKT_AC_ACK_SELECT_SERVER_READ* pkt = reinterpret_cast<PKT_AC_ACK_SELECT_SERVER_READ*>(inPacket.Buffer);

	GSLOG(Log, TEXT("[NetAuthLog] NetMessageHandlerAuth recv:PKT_AC_ACK_SELECT_SERVER"));

	FGsNetManager* netManager = GNet();
	if (nullptr == netManager)
	{
		// 딤드 오프
		FGsUIHelper::TrayDimmed(false);
		GSLOG(Error, TEXT("[NetAuthLog] GsNetMessageHandlerAuth::ServerSelectAck, nullptr == netManager"));
		return;
	}

	//TSharedPtr<FGsNetBase> active = GNet()->GetActive();
	//if (active.IsValid())
	//{
	//	active->SetPacketSendBlock(false);
	//}
	////netManager->ShowBlockUI(false);

	GSLOG(Log, TEXT("[NetAuthLog] A_ACK_SELECT_SERVER, PlanetWorldId = %d"), pkt->PlanetWorldId());

	PD::Result result = static_cast<PD::Result>(pkt->Result());
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Log, TEXT("[NetAuthLog] A_ACK_SELECT_SERVER Failed, Result = %d"), result);
		// 딤드 오프
		FGsUIHelper::TrayDimmed(false);
		FGsUIHelper::HideBlockUI();

		if (PD::Result::SELECT_SERVER_ERROR_INACCESSIBLE == result)
		{
			FText findText;
			FText::FindText(TEXT("LauncherText"), TEXT("Server_Entry_Wanning_01"), findText);

			FGsUIHelper::TrayTickerLauncher(findText);
		}
		else if (PD::Result::SELECT_SERVER_ERROR_OVERLOAD == result)
		{
			FText findText;
			FText::FindText(TEXT("LauncherText"), TEXT("Server_Entry_Wanning_02"), findText);

			FGsUIHelper::TrayTickerLauncher(findText);
		}
		else if (PD::Result::LOGIN_ERROR_SERVER_MAINTENANCE == result)
		{
			FText findText;
			FText::FindText(TEXT("NetText"), TEXT("LOGIN_ERROR_SERVER_MAINTENANCE"), findText);

			FString msg = (findText.IsEmpty()) ?
				FString::Format(TEXT("Code({0})\n{1}"), { result, PD::ResultEnumToString(result) }) :
				FString::Format(TEXT("Code({0})\n{1}"), { result, *(findText.ToString()) });

			findText = FText::FromString(msg);

			FGsUIHelper::TrayTickerLauncher(findText);
		}
		else
		{
			FGsUIHelper::PopupNetError(static_cast<PD::Result>(result), true);
		}

		if (false == netManager->SetServerList(pkt))
		{
		}

		GMessage()->GetSystemParam().SendMessage(MessageSystem::NET_GATEWAY_LOGIN_FAILED, nullptr);
		return;
	}

	_cachedSucceededPktAcAckSelectServer = MakeShared<Cached_PKT_AC_ACK_SELECT_SERVER_READ>(*pkt);

	// FGsNetSendService::SendAuthReqAppGuardAuth() 함수가 성공했다면, 서버는 반드시 PKT_AC_ACK_APP_GUARD_AUTH_READ 패킷을 내려주게 되어 있음.
	// 서버에서 별도 로직 거치지 않고 즉시 릴레이로 넘겨주므로, PKT_AC_ACK_APP_GUARD_AUTH_READ 패킷이 정상 실행됨을 보장함.
	// 그리고 이 패킷 수신 시, FGsNetMessageHandlerAuth::AckAppGuardAuth() 를 호출하게 되고, 결과 PostServerSelectAck() 를 정상 호출하게 됨.
	if (!FGsNetSendService::SendAuthReqAppGuardAuth())
	{
		// 만약, 패킷 송신이 실패하는 경우, PostServerSelectAck() 를 즉시 실행시켜서 진행에 문제가 없도록 함.
		// 단, 이처럼 패킷 송신이 실패하는 경우는 매우 특수한 경우로 일반적인 상황은 아님.
		//PostServerSelectAck();
	}
	PostServerSelectAck();
}

void FGsNetMessageHandlerAuth::AckAppGuardAuth(const FGsNet::Packet& inPacket)
{
	// FGsNetSendService::SendAuthReqAppGuardAuth() 에 대한 응답 패킷
	// FGsNetSendService::SendAuthReqAppGuardAuth() 송신 시, 서버 접속이 문제 없다면, 응답은 항상 수신되므로, 본 함수가 실행됨을 보장함.
	// 이 Ack 를 수신받지 못했다면, 네트워크 단절 등으로 재연결이나 재로그인이 발생하므로, 이 후 작업하지 않아도 무관함.
	GSLOG(Verbose, TEXT("[NetAuthLog] FGsNetMessageHandlerAuth::AckAppGuardAuth()"));

	FPlayGuardModule* PlayGuardModule = FPlayGuardModule::Get();
	if (nullptr != PlayGuardModule)
	{
		// 모든 UniqueClientId 관련 프로세스가 종료되었으므로, 다시 로그인 하지 않는 한 재시도하지 않도록 종료 상태로 전환.
		PlayGuardModule->SetUniqueClientIdState(EUniqueClientIdState::Finished);
	}

	PKT_AC_ACK_APP_GUARD_AUTH_READ* pkt = reinterpret_cast<PKT_AC_ACK_APP_GUARD_AUTH_READ*>(inPacket.Buffer);
	if (nullptr == pkt || pkt->Result() != PD::Result::RESULT_SUCCESS)
	{
		GSLOG(Error, TEXT("[NetAuthLog] nullptr == pkt || pkt->Result() != PD::Result::RESULT_SUCCESS"));
	}

	//PostServerSelectAck();
}

void FGsNetMessageHandlerAuth::PostServerSelectAck()
{
	GSLOG(Log, TEXT("[NetAuthLog] FGsNetMessageHandlerAuth::PostServerSelectAck()"));

	FGsNetManager* netManager = GNet();
	if (nullptr == netManager)
	{
		// 딤드 오프
		FGsUIHelper::TrayDimmed(false);
		GSLOG(Error, TEXT("[NetAuthLog] FGsNetMessageHandlerAuth::PostServerSelectAck(), nullptr == netManager"));
		return;
	}

#ifdef LOBBY_FLOW_UI_HIVE_1ST
#else // LOBBY_FLOW_UI_HIVE_1ST
	// 다운로드가 있으면 다운로드팝업
	// 없으면 캐릭터 선택화면으로 이동
	if (UGsLevelManager* level = GLevel())
	{
		GSLOG(Log, TEXT("[NetAuthLog] Call UGsLevelManager::LoadLobby()"));		
		level->LoadLobby();
	}
	//GMessage()->GetStage().SendMessage(MessageStage::ENTER_CHARACTERSELECT_STAGE);
#endif // LOBBY_FLOW_UI_HIVE_1ST

	// 다운로드 이후 처리해야하는 항목 처리
	GScope()->OnResourceDownloadComplete();
	GSound()->SetSoundMaster();

	if (!_cachedSucceededPktAcAckSelectServer.IsValid())
	{
		// 이 함수가 호출되는 시점에 _cachedSucceededPktAcAckSelectServer 은 항상 유효해야 함.
		// 이 내부 블럭이 실행된다면, 이 블럭이 문제가 아니라 어딘가에서 _cachedSucceededPktAcAckSelectServer 가 잘못 리셋되었다는 의미.
		GSLOG(Error, TEXT("[NetAuthLog] !_cachedSucceededPktAcAckSelectServer.IsValid()"));
		return;
	}
	else
	{
		TSharedPtr<FGsNetBase> netGame = netManager->Find(FGsNet::Mode::GAME);
		if (netGame.IsValid())
		{
			FString address(_cachedSucceededPktAcAckSelectServer->_ip);

			GSLOG(Log, TEXT("[NetAuthLog] GateWay Address [%s  :  %d]"), *address, _cachedSucceededPktAcAckSelectServer->_port);

#pragma todo("게이트워이 주소를 강제로 로컬로 변경한다.. 테스트 용처")

			netGame->InitConnection();
			netGame->SetPort(_cachedSucceededPktAcAckSelectServer->_port);
			netManager->SetTicket(_cachedSucceededPktAcAckSelectServer->_authTicket);
			netManager->SetWorldId(_cachedSucceededPktAcAckSelectServer->_planetWorldId);
			netManager->SetClientIpAddreass(_cachedSucceededPktAcAckSelectServer->_clientIpAddress);

			GSLOG(Log, TEXT("[NetAuthLog] PostServerSelectAck, cacheData port = %d"), _cachedSucceededPktAcAckSelectServer->_port);
			GSLOG(Log, TEXT("[NetAuthLog] PostServerSelectAck, cacheData planetWorldId = %d"), _cachedSucceededPktAcAckSelectServer->_planetWorldId);
			GSLOG(Log, TEXT("[NetAuthLog] PostServerSelectAck, cacheData clientIpAddress = %s"), *_cachedSucceededPktAcAckSelectServer->_clientIpAddress);

			if (false == netGame->GetAddressQuery<FGsNetGame>(static_cast<FGsNetGame*>(netGame.Get()), address))
			{
				FGsUIHelper::TrayDimmed(false);
				FGsUIHelper::HideBlockUI();
			}
		}

		_cachedSucceededPktAcAckSelectServer.Reset();
	}

	GMessage()->GetSystemParam().SendMessage(MessageSystem::NET_GATEWAY_LOGIN_SUCCEEDED, nullptr);
}

void FGsNetMessageHandlerAuth::AckDeleteAccount(const FGsNet::Packet& inPacket)
{
	PKT_AC_ACK_DELETE_ACCOUNT_READ* pkt = reinterpret_cast<PKT_AC_ACK_DELETE_ACCOUNT_READ*>(inPacket.Buffer);

	GSLOG(Log, TEXT("[NetAuthLog] NetMessageHandlerAuth recv:PKT_AC_ACK_DELETE_ACCOUNT_READ"));

#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Ack, Auth Server AC_ACK_DELETE_ACCOUNT"));

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
			GMessage()->GetSystem().SendMessage(MessageSystem::EXIT_GAME);

		});
	return;
#endif

	if (nullptr == pkt)
	{
		GSLOG(Log, TEXT("[NetAuthLog] NetMessageHandlerAuth recv -> nullptr == pkt"));
		return;
	}

	int32_t result = pkt->Result();
	if (result != PD::Result::RESULT_SUCCESS)
	{
		GSLOG(Error, TEXT("[NetAuthLog] FGsNetMessageHandlerAuth::AckDeleteAccount(%d)"), pkt->Result());
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result), true);
		return;
	}

	// UI 다 닫고
	GUI()->CloseAllStack();

	UGsHiveManager* hiveMgr = GHive();
	if (nullptr != hiveMgr)
	{
		hiveMgr->SetIsDeleteAccountSuccess(true);

		if (true == hiveMgr->IsSignIn())
		{
			hiveMgr->SignOut();
		}
	}

#if WITH_EDITOR
	GMessage()->GetSystem().SendMessage(MessageSystem::EXIT_GAME);
#else
	GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
#endif
}

void FGsNetMessageHandlerAuth::IdpConflictPlayerAck(const FGsNet::Packet& inPacket)
{
	GSLOG(Log, TEXT("[NetAuthLog] FGsNetMessageHandlerAuth::IdpConflictPlayerAck()"));

	PKT_AC_ACK_IDP_CONFLICT_PLYER_READ* pkt = reinterpret_cast<PKT_AC_ACK_IDP_CONFLICT_PLYER_READ*>(inPacket.Buffer);

	if (nullptr == pkt || pkt->Result() != PD::Result::RESULT_SUCCESS)
	{
		GSLOG(Error, TEXT("[NetAuthLog] nullptr == pkt || pkt->Result() != PD::Result::RESULT_SUCCESS"));
		return;
	}

	UGsHiveManager* hiveMgr = GHive();
	if (nullptr != hiveMgr)
	{
		TArray<TMap<FString, FString>> UserInfoMapArray;
		TArray<FString> ServerNameArray;
		TArray<FString> UserNameArray;
		for (PD::AC::PKT_AC_ACK_IDP_CONFLICT_PLYER_READ::UserListIterator iterator = pkt->GetFirstUserListIterator();
			iterator != pkt->GetLastUserListIterator();
			++iterator
			)
		{
			int32 idx = UserInfoMapArray.Add(TMap<FString, FString>());

			UserInfoMapArray[idx].Add(iterator->UserInfoType0(), iterator->UserInfoValue0());
			UserInfoMapArray[idx].Add(iterator->UserInfoType1(), iterator->UserInfoValue1());

			GSLOG(Log, TEXT("[NetAuthLog] UserInfoMapArray.Add(TMap<FString, FString>()) idx : %d"), idx);
			GSLOG(Log, TEXT("[NetAuthLog] iterator->UserInfoType0() : %s, iterator->UserInfoValue0() : %s"), *(iterator->UserInfoType0()), *(iterator->UserInfoValue0()));
			GSLOG(Log, TEXT("[NetAuthLog] iterator->UserInfoType1() : %s, iterator->UserInfoValue1() : %s"), *(iterator->UserInfoType1()), *(iterator->UserInfoValue1()));

			ServerNameArray.Add(iterator->ServerName());
			UserNameArray.Add(iterator->UserName());

			GSLOG(Log, TEXT("iterator->ServerName() : %s, iterator->UserName() : %s"), *(iterator->ServerName()), *(iterator->UserName()));
		}

		hiveMgr->ShowConflictHelperLobbyAndInGame(UserInfoMapArray, ServerNameArray, UserNameArray);
	}
}

FString LexToString(const PKT_AC_ACK_PRE_LOGIN_READ& InPacket)
{
	PD::AC::PKT_AC_ACK_PRE_LOGIN_READ* ReadPacket = const_cast<PD::AC::PKT_AC_ACK_PRE_LOGIN_READ*>(&InPacket);
	FString Logs;
	Logs.Appendf(TEXT("PKT_AC_ACK_PRE_LOGIN_READ: Result: %s, ResultDetail: %s, CanZPay: %s, RegisterDevice: %s, ConectionWorldId: %s, PlayingUserName: %s, AppendixName: %s, HiveHostCategory: %s\n"), *LexToString(ReadPacket->Result()), *LexToString(ReadPacket->ResultDetail()), *LexToString(ReadPacket->CanZPay()), *LexToString(ReadPacket->RegisterDevice()), *LexToString(ReadPacket->ConectionWorldId()), *ReadPacket->PlayingUserName(), *ReadPacket->AppendixName(), *ReadPacket->HiveHostCategory());
	return Logs;
}
