#include "GsNetGame.h"

#include "GsNetSendService.h"
#include "Shared/Shared/SharedPackets/PD_Client_Gateway.h"
#include "Shared/Shared/SharedPackets/PktStringTable.h"
#include "Shared/Client/SharedEnums/KickReasonEnum.h"

#include "GameFlow/GsGameFlow.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"

#include "Message/GsMessageSystem.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGlobal/GsPopupSystemManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "AddressInfoTypes.h"

FGsNetGame::FGsNetGame()
{
	SetSessionName(TEXT("GateWay"));

	// 재연결 타이머 셋팅
	_socketReconnectTImer.SetInterval(3.0f);
	_socketReconnectTImer.Router.AddRaw(this, &FGsNetGame::OnReconnectSocketByTimer);

	// 재연결 요청 시도 타이머 셋팅
	// TODO: 일정 시간 지난 후 패킷 재전송을 위한 용도이기에, 타이머보다 효율적인 호출방법이 있다면 개선 필요
	_reqSendReconnectTimer.SetInterval(3.0f);	
	_reqSendReconnectTimer.Router.AddRaw(this, &FGsNetGame::OnReqSendReconnectByTimer);

	// 월드 타임 싱크용 타이머 세팅. UGsGlobalConstant 값을 쓰지만 못가져올 경우를 대비해 초기화 값을 넣음
	_reqSendSyncWorldTimeTimer.SetInterval(60.f);
	_reqSendSyncWorldTimeTimer.Router.AddRaw(this, &FGsNetGame::OnReqSendSyncWorldTimeByTimer);

	// LOGIN_ERROR_ACCOUNT_EXPIRING_RETRY_AGAIN 상황 대처용 타이머
	// https://jira.gamevilcom2us.com/wiki/pages/viewpage.action?pageId=241001303
	_reqGCAckLoginTimer.SetInterval(1.0f);
	_reqGCAckLoginTimer.Router.AddRaw(this, &FGsNetGame::OnReqRetrySendGatewayLoginByTimer);
}

//----------------------------------------------------------------------
// FGsNetBase
//----------------------------------------------------------------------
void FGsNetGame::Enter()
{	

	Super::Enter();

	// 접속, 재연결 관련 변수 초기화
	_isReconnectionStarted = false;
	_isSocketReconnecting = false;
	_isHandshakeCompleted = false;

	//재연결 가능구간은 SpawnMe 이후임( 결국 재연결 가능구간 체크용임을 확인)
	SetReconnectAble(false);

	
	if (0 == Connect<FGsNetGame>(this))
	{
		GMessage()->GetSystem().SendMessage(MessageSystem::GAME_SERVER_CONNECTION_FAIL);
		return;
	}

	// 패킷 재연결 요청이 올때 동작한다.
	_systemMessageRetryGCAckLogin = GMessage()->GetSystem().AddRaw(MessageSystem::GATEWAY_SERVER_LOGIN_RETRY,
		this, &FGsNetGame::OnRetryGateWayLogin);


	auto& protocalGatewayHandler = GNet()->GetProtocalGateway();
	_listGcPacketDelegate.Emplace(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_ACK_RECONNECT,
		this, &FGsNetGame::RecvReconnectAck));

	_listGcPacketDelegate.Emplace(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_KICKED_REASON,
		this, &FGsNetGame::RecvKickedByReason));

	_listGcPacketDelegate.Emplace(protocalGatewayHandler.AddRaw(PD::GC::GCPacketId::GC_PONG,
		this, &FGsNetGame::RecvGatewayPongAck));

	// 월드 타임 싱크 패킷 전송 주기설정, 타이머 시작 
	if (UGsDataContainManager* dataContainMgr = GData())
	{
		if (const UGsGlobalConstant* globalConst = dataContainMgr->GetGlobalData())
		{
			_reqSendSyncWorldTimeTimer.SetInterval(globalConst->_syncWorldTimeDuration);
		}
	}
	_reqSendSyncWorldTimeTimer.Reset();
	_reqSendSyncWorldTimeTimer.Start();

	// 패킷전송을 허가한다
	SetPacketSendBlock(false);
}

void FGsNetGame::Exit()
{
	Super::Exit();

	MProtocalGateway& protocalGatewayHandler = GNet()->GetProtocalGateway();
	for (TPair<PD::GC::GCPacketId, FDelegateHandle>& elemStage : _listGcPacketDelegate)
	{
		protocalGatewayHandler.Remove(elemStage);
	}
	
	GMessage()->GetSystem().Remove(_systemMessageRetryGCAckLogin);
}

void FGsNetGame::Update(float indeltaTime)
{
	Super::Update(indeltaTime);

	_socketReconnectTImer.Update(indeltaTime);
	_reqSendReconnectTimer.Update(indeltaTime);
	_reqSendSyncWorldTimeTimer.Update(indeltaTime);

	_reqGCAckLoginTimer.Update(indeltaTime);
}

void FGsNetGame::ReConnect()
{
	Super::ReConnect();

	_isSocketReconnecting = true;
	_socketReconnectTImer.Reset();

	_socketReconnectTImer.Start();
	_isHandshakeCompleted = false;

	SetReconnectAble(false);

	Connect<FGsNetGame>(this, true);
}

void FGsNetGame::SetRestMode(bool InIsRest)
{
	Super::SetRestMode(InIsRest);

	if (InIsRest)
	{
		if (false == _isHandshakeCompleted)
		{	// 2023/9/5 PKT - 연결된 적이 없다면..빠져 나감.
			return;
		}

		// 2023/9/5 PKT - 화면 입력 막음.( 해제는 리컨넥팅 할때 됨. )
		FGsUIHelper::ShowBlockUI();

		_isSocketReconnecting = false;
		_socketReconnectTImer.Reset();
		_isHandshakeCompleted = false;

		Disconnect(false);
	}
	else
	{
		if (true == _isHandshakeCompleted)
		{
			return;
		}

		ReConnect();
	}
}

void FGsNetGame::Send(int size, bool ignoreSendObserver)
{
	// 통보없이 연결을 끊는 경우가 생겨서 경우 체크를 한다.
	// GatewayServer에 SC_SPAWN_ME_COMPLETE 을 받고 패킷을 보내야 한다. 그 전에 메세지는 모두 리턴
	
	// 스폰상태가 아닌데 Send 하는 경우를 강제 제외시키는 테스트
	// 해당 문제가 발생하지 않아 #if 0 처리 해놓았습니다
	// 유사 문제 발생시 확인하기 위하여 코드를 남겨 놓습니다
#if 0
	if (false == GetReconnectionState())
	{
		FGsNetBuffer* sendBuffer = GetLocalSendBufferPtr();
		uint16 packetId = ((uint16*)sendBuffer->GetBuffer())[1];
		GSLOG(Log, TEXT("===== FGsNetGame::Send, (false == GetReconnectionState()), packetId: %d ====="), packetId);
		if ((7000 < packetId) && (7499 > packetId))
			return;
	}
#endif
	Super::Send(size, ignoreSendObserver);
}


//----------------------------------------------------------------------
// implementaion
//----------------------------------------------------------------------
void FGsNetGame::SendGatewayPing()
{
	if (false == IsHandshakeCompleted())
		return;

#if WITH_EDITOR
	// 에디터 일 때에는 서버 브레이브포인트 디버깅 상황을 위해, 연결을 끊지 않는다
	// 또한, 디버깅 중에 Ping 전송 패킷이 쌓이지 않도록, _sendPingSeq 와 _recvPongSeq 일치할 때에만 핑을 전송한다
	if (_sendPingSeq != _recvPongSeq)
		return;
#else
	// _sendPingSeq 과 _recvPongSeq 차이가 LIMIT_PING_PONG_SEQ_DELTA 크면,
	if (LIMIT_PING_PONG_SEQ_DELTA < (_sendPingSeq - _recvPongSeq))
	{
		// _sendPingSeq 과 _recvPongSeq 차이가 LIMIT_PING_PONG_SEQ_DELTA 값보다 차이가 크면 '통신 장애'
		Disconnect(true);
		return;
	}
#endif

	FGsNetBuffer* sendBuffer = GetLocalSendBufferPtr();
	PD::CG::PKT_CG_PING_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), ++_sendPingSeq);
	Send(builder.mPktWriter.GetPktSize(), true);
	
	//GSLOG(Log, TEXT("===== Send Ping to GATEWAY ====="));
}

void FGsNetGame::RecvGatewayPongAck(const FGsNet::Packet& inPacket)
{	
	if (PD::GC::PKT_GC_PONG_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_PONG_READ*>(inPacket.Buffer))
	{
		uint32 pingPongSeq = pkt->PingPongSeq();		

		// pingPongSeq 와 _recvPongSeq 를 비교하여 이번에 받아야 하는 값과 일치하지 않으면 '통신 장애'		
		if ((_recvPongSeq + 1) != pingPongSeq)
		{
			Disconnect(true);
			return;
		}

		// 현재 pingPongSeq 을 _recvPongSeq 에 저장한다
		_recvPongSeq = pingPongSeq;		
	}

	//GSLOG(Log, TEXT("===== Recv Ping from GATEWAY ====="));
}

void FGsNetGame::RecvReconnectAck(const FGsNet::Packet& inPacket)
{
	if (PD::GC::PKT_GC_ACK_RECONNECT_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_ACK_RECONNECT_READ*>(inPacket.Buffer))
	{
		int32_t result = pkt->Result();
		// 재접속 요청을 다시 시도 하시오
		if (PD::Result::RECONNECT_ERROR_NOT_DISCONNECTED_YET == result)
		{
			GSLOG(
				Log, 
				TEXT("FGsNetGame::RecvReconnectAck(), RECONNECT_ERROR_NOT_DISCONNECTED_YET")
			);			

			// 잠시 후 다시 재접속 요청을 전송한다
			_reqSendReconnectTimer.Reset();
			_reqSendReconnectTimer.Start();
		}
		// 재접속 실패
		else if (result != PACKET_RESULT_SUCCESS)
		{
			GSLOG(Log, TEXT("FGsNetGame::RecvReconnectAck(), result != PACKET_RESULT_SUCCESS"));

			// 블록을 푼다
			FGsUIHelper::HideBlockUI();

			// 타이틀로 보낸다.
			BackToTitle();
		}
		// 재접속 성공
		else
		{
			// 재연결 프로세스 중단됨
			_isReconnectionStarted = false;

			GSLOG(Log, TEXT("FGsNetGame::RecvReconnectAck(), SUCCESS"));

			// 블록을 푼다
			FGsUIHelper::HideBlockUI();

			// 재연결 성공 메시지 발송
			GMessage()->GetSystem().SendMessage(MessageSystem::GAME_SERVER_RECONNECTION_SUCCESS);
			
			// 재접속 티켓을 셋팅한다
			GNet()->SetReconnectTicket(pkt->ReconnectTicket());

			// 재접속 가능 상태로 변경
			SetReconnectAble(true);

			// 패킷 전송이 가능하도록 한다
			SetPacketSendBlock(false);
		}
	}
}

void FGsNetGame::RecvKickedByReason(const FGsNet::Packet& inPacket)
{
	PD::GC::PKT_GC_KICKED_REASON_READ* pkt = reinterpret_cast<PD::GC::PKT_GC_KICKED_REASON_READ*>(inPacket.Buffer);

	GSLOG(Warning, TEXT("FGsNetGame::RecvKickedByReason, KickReason: %d"), pkt->Reason());

	GNet()->SetReconnectTicket(INVALID_RECONNECT_TICKET);

	KickReason reason = (KickReason)pkt->Reason();
	FString strReason = KickReasonEnumToDisplayKey(reason);
	FText errorText;
	FText::FindText(TEXT("NetText"), strReason, errorText);

	//킥당할때는 중복전송 방지 쿨타이머 리셋
	GNet()->GetCoolTime().Clear();

#pragma todo("Refactoring TODO")
	// 명시적으로 재연결에 실패한 경우는 재연결을 포기하고 로비로 돌려보낸다
	if (KickReason::RECONNECT_FAILED == reason)
	{
		GSLOG(Log, TEXT("FGsNetGame::RecvKickedByReason, KickReason::RECONNECT_FAILED == reason, BackToTitle"));

		// 블록을 푼다
		FGsUIHelper::HideBlockUI();

		//타이틀 화면으로 돌아간다.
		BackToTitle();
	}
	else if (KickReason::ZPAY_SESSION_KICK_WHEN_USER_DISCONNECTED == reason)
	{
		GSLOG(Error, TEXT("FGsNetGame::RecvKickedByReason, KickReason::ZPAY_SESSION_KICK_WHEN_USER_DISCONNECTED"));

		FGsUIHelper::HideBlockUI();

		// 패킷전송을 허가한다
		SetPacketSendBlock(false);

		FText::FindText(TEXT("BMShopTex"), TEXT("SyncPay_Desc_DisConnect"), errorText);
		FGsUIHelper::PopupSystemMsg(errorText, []()
			{
				GMessage()->GetBMShop().SendMessage(FGsBMShopKickZpayUser::MESSAGE_ID, FGsSharedParam(nullptr));
			});
	}
	else if (KickReason::ZPAY_SESSION_KICK_WHEN_OFFLINE_START == reason)
	{
		GSLOG(Error, TEXT("FGsNetGame::RecvKickedByReason, KickReason::ZPAY_SESSION_KICK_WHEN_OFFLINE_START"));

		FGsUIHelper::HideBlockUI();

		// 패킷전송을 허가한다
		SetPacketSendBlock(false);

		FText::FindText(TEXT("NetText"), TEXT("ZPAY_SESSION_KICK_WHEN_OFFLINE_START"), errorText);
		FGsUIHelper::PopupSystemMsg(errorText, []()
			{
				GMessage()->GetBMShop().SendMessage(FGsBMShopKickZpayUser::MESSAGE_ID, FGsSharedParam(nullptr));
			});
	}
	else if (KickReason::KICK_USER_FOR_SERVER_MAINTENANCE == reason
		|| KickReason::GM_TOOL_KICK_USER == reason
		|| KickReason::GM_TOOL_KICK_USER_TO_BLOCK == reason
		|| KickReason::ADMIN_COMMAND_RESTART == reason
		)
	{
		OnExitGame();

		FGsUIHelper::PopupSystemMsg(errorText, [this]()
			{
				if (UGsHiveManager* hiveMgr = GHive())
				{
					hiveMgr->ExitGame();
				}
			});
	}
	else if (KickReason::REDUNDANT_LOGIN == reason)
	{
		// 블록을 푼다
		FGsUIHelper::HideBlockUI();

		OnExitGame();
		// errorText = FText::FromString(strReason);
		FGsUIHelper::PopupSystemMsg(errorText, [this]()
			{
				////타이틀 화면으로 돌아간다.
				//BackToTitle();
				
				// 게임(앱)종료
				if (UGsHiveManager* hiveMgr = GHive())
				{
					hiveMgr->ExitGame();
				}
			});
	}
	else if (KickReason::SERVER_MIGRATE_START == reason)
	{
		// 다른 곳에서 Ack 처리되고 있어 메세지 없이 그냥 나간다
		// 블록을 푼다
		FGsUIHelper::HideBlockUI();

		SetPacketSendBlock(false);
		OnExitGame();

		GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
	}
	else
	{
		FGsUIHelper::HideBlockUI();

		// 패킷전송을 허가한다
		SetPacketSendBlock(false);
		// 재연결 티켓 및 타이머 clear
		OnExitGame();

		// 텍스트가 없을 경우 대체 텍스트 출력
		if (errorText.IsEmpty())
		{
			// TEXT: 알수 없는 오류로 연결을 종료합니다.
			FText::FindText(TEXT("NetText"), TEXT("KICKREASON_UNKNOWN"), errorText);
		}

		// 대체 텍스트도 없을 경우 EnumString을 찍도록 처리
		if (errorText.IsEmpty())
		{
			errorText = FText::FromString(strReason);
		}
		
		FGsUIHelper::PopupSystemMsg(errorText, []()
		{			
			GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
		});
		// 실제 Ok
	}
}

void FGsNetGame::OnExitGame()
{
	// 모든 System팝업을 정리
	GPopupSystem()->RemoveAll();
	GUI()->CloseAllStack();

	// 서버와의 연결을 확실히 끊어준다, 이 함수는 내부에서 OnDisconnected 를 호출하지 않도록 되어 있다
	Disconnect();

	// 재연결 티켓 삭제
	if (FGsNetManager* netManager = GNet())
		netManager->SetReconnectTicket(INVALID_RECONNECT_TICKET);

	// 재연결 프로세스 중단됨
	_isReconnectionStarted = false;

	// 타이머 중단
	_isSocketReconnecting = false;
	_socketReconnectTImer.Stop();
	_reqSendReconnectTimer.Stop();
	_reqSendSyncWorldTimeTimer.Stop();
}

void FGsNetGame::OnAddressQueryRecevied(const FAddressInfoResult& inResult)
{
	if (SE_NO_ERROR != inResult.ReturnCode)
	{
		GMessage()->GetSystem().SendMessageAsync(MessageSystem::GAME_SERVER_QUERY_IP_FAIL);
	}
	else
	{
		if (inResult.Results.Num() < 1)
		{
			GMessage()->GetSystem().SendMessageAsync(MessageSystem::GAME_SERVER_QUERY_IP_FAIL);
		}
		else
		{
#if PLATFORM_IOS
			for (int i = 0; i < inResult.Results.Num(); ++i)
			{
				if (inResult.Results[i].Address->GetProtocolType() == FNetworkProtocolTypes::IPv6)
				{
					_addressInfo = inResult.Results[i].Address;
				}

				if(_addressInfo.IsValid())
				{
					break;
				}
			}
			if(false == _addressInfo.IsValid())
			{
				_addressInfo = inResult.Results[0].Address;
			}

			GMessage()->GetSystem().SendMessageAsync(_addressInfo.IsValid() ? MessageSystem::GAME_SERVER_QUERY_IP_SUCCESS : MessageSystem::GAME_SERVER_QUERY_IP_FAIL);
#else
			_addressInfo = inResult.Results[0].Address;
			GMessage()->GetSystem().SendMessageAsync(MessageSystem::GAME_SERVER_QUERY_IP_SUCCESS);
#endif
		}
	}
}

//타이틀로 돌려보내는 코드!! 어떤상태든 복구하기 힘든상태에서 처리되는 함수
void FGsNetGame::BackToTitle(FText message)
{
	// 서버와의 연결을 확실히 끊어준다, 이 함수는 내부에서 OnDisconnected 를 호출하지 않도록 되어 있다
	Disconnect();

	// 재연결 티켓 삭제
	if (FGsNetManager* netManager = GNet())
		netManager->SetReconnectTicket(INVALID_RECONNECT_TICKET);

	// 재연결 프로세스 중단됨
	_isReconnectionStarted = false;

	// 타이머 중단
	_isSocketReconnecting = false;
	_socketReconnectTImer.Stop();
	_reqSendReconnectTimer.Stop();
	_reqSendSyncWorldTimeTimer.Stop();
	
	// 재연결 실패 -  재접속을 실패하였습니다. 
	// 게임을 다시 시작하시겠습니까 ?
	FGsUIHelper::HideBlockUI();

	// 패킷전송을 허가한다
	SetPacketSendBlock(false);
	
	if (message.IsEmpty())
	{
		FText::FindText(TEXT("NetText"), TEXT("RECONNECT_ERROR_FAILED"), message);
	}
	
	FGsUIHelper::PopupSystemMsg(message, []()
	{
		GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
	});
}

void FGsNetGame::ReconnectSocket()
{
	++_socketReconnectCount;

	// 지정한 재시도 횟수를 초과하면 포기하고 타이틀화면으로 보낸다
	if (_socketReconnectCount >= 10)
	{
		GSLOG(Log, TEXT("==== ReconnectSocket(), _socketReconnectCount >= 10 ===="));

		// 블록을 푼다
		FGsUIHelper::HideBlockUI();

		// 타이틀 화면으로 돌아간다.
		BackToTitle();
	}
	// 다시 접속을 시도한다
	else
	{
		GSLOG(
			Log,
			TEXT("==== ReconnectSocket(), _socketReconnectCount: %d ===="),
			_socketReconnectCount
		);

		//// 다시 접속을 시도 하기 전에 
		//// 점검팝업이 있는지 확인 요청
		//OnCheckMaintenance();

		// GatewayServer 재접속 시도 타이머를 시작한다
		_isSocketReconnecting = true;
		_socketReconnectTImer.Reset();

		_socketReconnectTImer.Start();
		_isHandshakeCompleted = false;

		SetReconnectAble(false);

		Connect<FGsNetGame>(this, true);
	}
}

void FGsNetGame::OnReconnectSocketByTimer()
{
	GSLOG(Log, TEXT("==== OnReconnectSocketByTimer() ===="));

	_isSocketReconnecting = false;


	ReconnectSocket();
}

void FGsNetGame::OnReqSendReconnectByTimer()
{
	GSLOG(Log, TEXT("==== OnReqSendReconnectByTimer() ===="));

	// 재접속 요청 타이머를 멈춘다
	_reqSendReconnectTimer.Stop();

	// 재접속 요청을 전송한다
	FGsNetSendService::SendRequestReconnect();
}

void FGsNetGame::OnReqSendSyncWorldTimeByTimer()
{
	if (false == IsBlockingSend())
	{
		if (FGsGameFlowManager* gameFlowMgr = GMode())
		{
			if (FGsGameFlow::Mode::GAME == gameFlowMgr->GetCurrentFlowType())
			{
				FGsNetSendService::SendRequestWorldTime();
			}
		}
	}

	_reqSendSyncWorldTimeTimer.Reset();
	_reqSendSyncWorldTimeTimer.Start();
}

void FGsNetGame::OnRetryGateWayLogin()
{
	FGsUIHelper::ShowBlockUI();

	_reqGCAckLoginTimer.Reset();
	_reqGCAckLoginTimer.Start();
}

void FGsNetGame::OnReqRetrySendGatewayLoginByTimer()
{
	FGsNetSendService::SendGatewayReqLogin();
}

FGsNetGame::ReconnectionState FGsNetGame::GetReconnectionState() const
{
	// 재연결 프로세스 시작 됨이고,
	if (true == _isReconnectionStarted)
	{
		// 핸드쉐이크까지 끝났으면, 연결 완료 /아니면, 연결 중
		if(true == _isHandshakeCompleted)
			return RECONNECTION_STATE_CONNECTED;
		else
			return RECONNECTION_STATE_CONNECTING;
	}

	// 스폰 된 상태가 아니면, 재연결 불가능
	if (false == GetReconnectAble())
		return RECONNECTION_STATE_IMPOSSIBLE;

	// 재연결 티켓을 가지고 있는지 여부
	FGsNetManager* netManager = GNet();	
	bool hasReconnectTicket =
		(nullptr != netManager) && (INVALID_RECONNECT_TICKET != netManager->GetReconnectTicket());

	// 재연결 티켓을 가지고 있지 않으면, 재연결 불가능
	if (false == hasReconnectTicket)
		return RECONNECTION_STATE_IMPOSSIBLE;

	// 재연결 가능
	return RECONNECTION_STATE_POSSIBLE;
}

//----------------------------------------------------------------------
// FGsNetBase::ISessionNotify
//----------------------------------------------------------------------
void FGsNetGame::OnHandshakeCompleted()
{
	GSLOG(Log, TEXT("===== Cryto Handshake completed to GateWay ====="));

	// 핸드쉐이크가 완료되면 핑퐁 프로세스가 돌아간다, 관련 변수를 초기화 한다
	_sendPingSeq = 0;
	_recvPongSeq = 0;	

	// 서버에서는 Connection 감지가 안되었는데, OnConnected 오는 경우가 있다
	// 따라서, 핸드쉐이크가 완료된 시점에 재연결 타이머를 종료한다
	// GatewayServer 재접속 시도 타이머 종료
	if (true == _isSocketReconnecting)
	{
		_isSocketReconnecting = false;
		_socketReconnectTImer.Stop();
	}

	FGsNetManager* netManager = GNet();
	// 재연결 티켓을 가지고 있는지 여부
	bool hasReconnectTicket = 
		(nullptr != netManager) && (INVALID_RECONNECT_TICKET != netManager->GetReconnectTicket());

	if (false == hasReconnectTicket)
	{
		GSLOG(Log, TEXT("FGsNetGame::OnHandshakeCompleted, hasReconnectTicket == false"));

		FGsNetSendService::SendGatewayReqLogin();
	}
	else
	{
		GSLOG(Log, TEXT("FGsNetGame::OnHandshakeCompleted, hasReconnectTicket == true"));

		FGsNetSendService::SendRequestReconnect();
	}

	_isHandshakeCompleted = true;	
	
	//Bak1210 : 접속 완료 노티는 HandShakeComplete가 완료된 시점으로 판단
	GMessage()->GetSystem().SendMessage(MessageSystem::GAME_SERVER_CONNECTION_SUCCESS);
	// 점검 요청 타이머 중지
	//StopCheckMaintenanceTimer();
}

void FGsNetGame::OnKeepAliveCheck()
{
	if (false == _isHandshakeCompleted)
		return;

	double DeltaTime = FPlatformTime::Seconds() - _sendPingLastTime;

	if (LIMIT_TIME_SEND_PING > DeltaTime)
	{
		return;
	}

	_sendPingLastTime = FPlatformTime::Seconds();

	SendGatewayPing();
}

void FGsNetGame::OnDisConnected(const FClientSession* inSession)
{
	/*if (FGsNetManager* netManager = GNet())
	{
		if (netManager->GetIsAutoLogin())
		{
			FGsUIHelper::HideBlockUI();
		}
	}*/

	// 패킷전송을 허가한다
	SetPacketSendBlock(false);
	
	// 재연결 상태를 얻어온다
	ReconnectionState checkReconnectionStateResult = GetReconnectionState();

	// 소켓 재연결 진행 중이면, 
	if (RECONNECTION_STATE_CONNECTING == checkReconnectionStateResult)
	{
		GSLOG(Log, TEXT("==== OnDisConnected, RECONNECTION_STATE_CONNECTING ===="));

		// 재연결 진행 중에 처리가 될 것이다. 여기에선 할 일이 없다
	}
	// 소켓 재연결 완료 상태이면, 
	else if (RECONNECTION_STATE_CONNECTED == checkReconnectionStateResult)
	{
		GSLOG(Log, TEXT("==== OnDisConnected, RECONNECTION_STATE_CONNECTED ===="));

		// GatewayServer 재접속 시도 타이머를 시작한다	
		// 소켓 재연결 시도 횟수는 그대로 유지한다
		_isSocketReconnecting = true;
		_socketReconnectTImer.Reset();
		_socketReconnectTImer.Start();
	}
	// 재연결이 불가능한 상태이면,
	else if (RECONNECTION_STATE_IMPOSSIBLE == checkReconnectionStateResult)
	{
		GSLOG(Log, TEXT("==== OnDisConnected, RECONNECTION_STATE_IMPOSSIBLE ===="));

		SetReconnectAble(false);

		FText errorText;				
		if (true == _isSocketReconnecting)
		{
			FText::FindText(TEXT("NetText"), TEXT("RECONNECT_ERROR_FAILED"), errorText);
		}
		else
		{
			// 재연결 시도 중이 아니었다면, RECONNECT_ERROR_FAILED 메시지가 어울리지 않아 수정하였습니다
			FText::FindText(TEXT("NetText"), TEXT("NetDisconected"), errorText);
		}

		// 에러 코드를 0으로 설정하여 다른 에러와 구분처리 하였습니다
		FString text = FString::Format(*errorText.ToString(), { FGsNet::Mode::GAME,  0 });
		
		// 타이틀 화면으로 돌아간다.
		BackToTitle(FText::FromString(text));
		

		//GSLOG(Log, TEXT("==== OnDisConnected, RECONNECTION_STATE_IMPOSSIBLE 1 ===="));
	}
	// 재연결이 가능한 상태이면,
	else if (RECONNECTION_STATE_POSSIBLE == checkReconnectionStateResult)
	{
		GSLOG(Log, TEXT("==== OnDisConnected, RECONNECTION_STATE_POSSIBLE ===="));

		SetReconnectAble(false);

		// TEXT: 재연결 중입니다.
		FText msgText;
		FText::FindText(TEXT("UICommonText"), TEXT("ReconnectBlock"), msgText);
		FGsUIHelper::ShowBlockUI();
		FGsUIHelper::SetBlockUISetting(msgText);

		// 재연결 프로세스 시작 됨
		_isReconnectionStarted = true;		

		// 재연결이 시작되면 패킷전송을 금지한다
		SetPacketSendBlock(true);

		// Disconnect 되었는데도 이동패킷을 쏘는 문제를 좀더 면밀하게 살펴보고자 한텀 쉬고 연결하도록 처리
#if 0
		// GatewayServer로 재접속 시도를 시작한다.
		_socketReconnectCount = 0;
		ReconnectSocket();
#else
		// GatewayServer로 재접속 시도를 시작한다.
		_socketReconnectCount = 0;		
		// GatewayServer 재접속 시도 타이머를 시작한다		
		_isSocketReconnecting = true;
		_socketReconnectTImer.Reset();
		_socketReconnectTImer.Start();

		GMessage()->GetSystem().SendMessage(MessageSystem::RECONNECT_START);
#endif
	}

	// 이 곳의 GMessage()->GetSystem().SendMessage() 코드들은 백태현 파트장님 확인 후, 코드를 살릴 예정입니다
	//GMessage()->GetSystem().SendMessage(MessageSystem::GAME_SERVER_DISCONNECTION);
}

void FGsNetGame::OnConnected(const FClientSession* inSession)
{	
	GSLOG(Log, TEXT("==== OnConnected ===="));

	//bak1210 : 연결이되었다라고 감지하는것은 HandShakeComplete;	
}

void FGsNetGame::OnPacketReceived(unsigned char* buffer, int size)
{
	FGsNet::PacketID packetid = *(unsigned short*)(buffer + sizeof(FGsNet::PacketID));
	FGsNet::Packet packetd(buffer, size);

	bool bShow = true;
#if !UE_BUILD_SHIPPING
	auto IsPrintLog = [](FGsNet::PacketID inMessage) -> bool
	{
		bool printLog = true;
		switch (inMessage)
		{
		case static_cast<int>(PD::GC::GCPacketId::GC_SERVER_PING):
		case static_cast<int>(PD::GC::GCPacketId::GC_PONG):
		case static_cast<int>(PD::SC::SCPacketId::SC_DEBUG_SHAPE):
		case static_cast<int>(PD::SC::SCPacketId::SC_DEBUG_SHAPE_CYLINDER_LIST):
		case static_cast<int>(PD::SC::SCPacketId::SC_DEBUG_SHAPE_LINE) :
		case static_cast<int>(PD::SC::SCPacketId::SC_DEBUG_TEXT_INFO):

		case static_cast<int>(PD::SC::SCPacketId::SC_SPAWN_USER):
		case static_cast<int>(PD::SC::SCPacketId::SC_SPAWN_NPC):
		case static_cast<int>(PD::SC::SCPacketId::SC_SPAWN_PROP):
		case static_cast<int>(PD::SC::SCPacketId::SC_SPAWN_DROP_OBJECT):
		case static_cast<int>(PD::SC::SCPacketId::SC_DESPAWN_USER):
		case static_cast<int>(PD::SC::SCPacketId::SC_DESPAWN_NPC):
		case static_cast<int>(PD::SC::SCPacketId::SC_DESPAWN_PROP):
		case static_cast<int>(PD::SC::SCPacketId::SC_DESPAWN_DROP_OBJECT):
		case static_cast<int>(PD::SC::SCPacketId::SC_MOVE_TO_LOCATION):
		case static_cast<int>(PD::SC::SCPacketId::SC_MOVE_IMMEDIATE_STOP):

		case static_cast<int>(PD::SC::SCPacketId::SC_START_SKILL):
		case static_cast<int>(PD::SC::SCPacketId::SC_SKILL_FINISHED):
		case static_cast<int>(PD::SC::SCPacketId::SC_SKILL_COOLDOWN):
		case static_cast<int>(PD::SC::SCPacketId::SC_SPAWN_PROJECTILE):
		case static_cast<int>(PD::SC::SCPacketId::SC_DESPAWN_PROJECTILE):
		case static_cast<int>(PD::SC::SCPacketId::SC_SKILL_DAMAGED):
		case static_cast<int>(PD::SC::SCPacketId::SC_PASSIVITY_DAMAGED):
		case static_cast<int>(PD::SC::SCPacketId::SC_IFF_STATUS):
			printLog = false;
		}
		return printLog;
	};

	bShow = IsPrintLog(packetid);
	if (bShow)
	{
		GSLOG(Warning, TEXT("==== OnPacketReceived [%s], size : [%d]"), *FString(GetPdlString(packetid)), size);
	}
	
#endif
	// 패킷 아이디가 PD::CS::C_BYPASS_START 보다 작으면 Gateway 패킷
	if (packetid < static_cast<FGsNet::PacketID>(PD::GC::GCPacketId::PacketIdMax))
	{
		GNet()->GetProtocalGateway().SendMessage(static_cast<PD::GC::GCPacketId>(packetid), packetd, bShow);
	}
	// 패킷 아이디가 PD::CS::C_BYPASS_START 이상이면 World 패킷
	else
	{
		GNet()->GetProtocalWorld().SendMessage(static_cast<PD::SC::SCPacketId>(packetid), packetd, bShow);
	}
}

void FGsNetGame::OnErrorRecevied(int32 inErrorCode)
{
	/*네크워크 연결이 종료되었습니다
	(1 : [에러코드])*/

	FGsUIHelper::HideBlockUI();

	FText errorText;
	FText::FindText(TEXT("NetText"), TEXT("NetDisconected"), errorText);
	
	FString text = FString::Format(*errorText.ToString(), { FGsNet::Mode::GAME,  inErrorCode });
	FGsUIHelper::PopupNetError(FText::FromString(text));

	GMessage()->GetSystem().SendMessage(MessageSystem::NET_DISCONECTED);
}
