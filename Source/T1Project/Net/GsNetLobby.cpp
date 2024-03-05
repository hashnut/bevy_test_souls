#include "GsNetLobby.h"
#include "GsNet.h"
#include "Shared/Shared/SharedPackets/PD_Client_Auth.h"
#include "Shared/Shared/SharedPackets/PktStringTable.h"

#include "Message/GsMessageSystem.h"
#include "Message/GsMessageStage.h"
#include "Message/MessageParam/GsStageMessageParam.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "AddressInfoTypes.h"

//----------------------------------------------------------------------
// FGsNetBase
//----------------------------------------------------------------------
void FGsNetLobby::Enter()
{
	Super::Enter();

	// 재연결 타이머 셋팅
	_socketConnectFailedTimer.SetInterval(10.f);
	_socketConnectFailedTimer.Router.AddRaw(this, &FGsNetLobby::OnSocketConnectFailedByTimer);

	_socketConnectFailedTimer.Reset();
	_socketConnectFailedTimer.Start();
	_isHandshakeCompleted = false;
	bool bConnected = Connect<FGsNetLobby>(this);
	
	if (false == bConnected)
	{
		GMessage()->GetSystem().SendMessage(MessageSystem::AUTH_SERVER_CONNECTION_FAIL);
		return;
	}

	MProtocalAuth& protocalAuthHandler = GNet()->GetProtocalAuth();

	_pongPacketDelegate = protocalAuthHandler.AddRaw(
		PD::AC::ACPacketId::AC_PONG, this, &FGsNetLobby::RecvPongAck
	);


	// 패킷전송을 허가한다
	SetPacketSendBlock(false);
}

void FGsNetLobby::Exit()
{
	Super::Exit();
}

void FGsNetLobby::Update(float indeltaTime)
{
	Super::Update(indeltaTime);

	_socketConnectFailedTimer.Update(indeltaTime);
}

void FGsNetLobby::SetRestMode(bool InIsRest)
{
	Super::SetRestMode(InIsRest);

	if (InIsRest)
	{
		if (false == _isHandshakeCompleted)
		{	// 2023/9/5 PKT - 연결된 적이 없다면..빠져 나감.
			return;
		}

		_socketConnectFailedTimer.Stop();
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

void FGsNetLobby::ReConnect()
{
	Super::ReConnect();

	_socketConnectFailedTimer.Start();
	_isHandshakeCompleted = false;

	bool bConnected = Connect<FGsNetLobby>(this);

	if (false == bConnected)
	{
		GMessage()->GetSystem().SendMessage(MessageSystem::AUTH_SERVER_CONNECTION_FAIL);
		return;
	}
}

//----------------------------------------------------------------------
// implementaion
//----------------------------------------------------------------------
void FGsNetLobby::RecvPongAck(const FGsNet::Packet& inPacket)
{
	//GSLOG(Log, TEXT("===== Recv Ping from AUTH ====="));
}

void FGsNetLobby::SendAuthPing()
{
	FGsNetBuffer* sendBuffer = GetLocalSendBufferPtr();
	
	unsigned short ping = rand() % 32768;
	PD::CA::PKT_CA_PING_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), ping);
	Send(builder.mPktWriter.GetPktSize());

	GSLOG(Log, TEXT("===== Send Ping to AUTH ====="));
}

void FGsNetLobby::OnSocketConnectFailedByTimer()
{
	Disconnect();
	GMessage()->GetSystem().SendMessage(MessageSystem::AUTH_SERVER_CONNECTION_FAIL);
}

//----------------------------------------------------------------------
// FGsNetBase::ISessionNotify
//----------------------------------------------------------------------
void FGsNetLobby::OnHandshakeCompleted()
{
	GSLOG(Log, TEXT("===== Cryto Handshake completed to AUTH ====="));
	GSLOG(Log, TEXT("===== Send ReqLogin to AUTH ====="));	

	_socketConnectFailedTimer.Stop();
	_isHandshakeCompleted = true;
	// 점검 요청 타이머 중지
	//StopCheckMaintenanceTimer();

	GMessage()->GetSystem().SendMessage(MessageSystem::NET_AUTH_SERVER_CONNECTION_SUCCEEDED);
	//FGsLobbyFlowMsgParam param(MessageStage::SUCCEEDED_AUTH_SERVER_CONNECTION);
	//GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
}

void FGsNetLobby::OnKeepAliveCheck()
{
	if (false == _isHandshakeCompleted)
		return;

	double DeltaTime = FPlatformTime::Seconds() - LastTime;

	if (LIMIT_TIME_SEND_GATEWAY_PONG > DeltaTime)
	{
		return;
	}

	LastTime = FPlatformTime::Seconds();

	SendAuthPing();
}

void FGsNetLobby::OnDisConnected(const FClientSession* inSession)
{
	// 패킷전송을 허가한다
	SetPacketSendBlock(false);

	GMessage()->GetSystem().SendMessage(MessageSystem::AUTH_SERVER_DISCONNECTION);
	
	GSLOG(Warning, TEXT("FGsNetLobby::OnDisConnected"));	
	//GMessage()->GetSystem().SendMessage(MessageSystem::EXIT_GAME);
}

void FGsNetLobby::OnConnected(const FClientSession* inSession)
{
	GSLOG(Warning, TEXT("FGsNetLobby::OnConnected"));
}

void FGsNetLobby::OnPacketReceived(unsigned char* buffer, int size)
{
	FGsNet::PacketID packetid = *(unsigned short*)(buffer + sizeof(FGsNet::PacketID));
#if !UE_BUILD_SHIPPING
	GSLOG(Warning, TEXT("==== FGsNetLobby::OnPacketReceived packetid : [%s], size : [%d]"), *FString(GetPdlString(packetid)), size);
#endif
	//GMessage()->GetProtocalAuth().SendMessage(static_cast<PD::AC::ACPacketId>(packetid), FGsNet::Packet(buffer, size));

	FGsNet::Packet packetd(buffer, size);

	GNet()->GetProtocalAuth().SendMessage(static_cast<PD::AC::ACPacketId>(packetid), packetd);
}

void FGsNetLobby::OnErrorRecevied(int32 inErrorCode)
{
	/*네크워크 연결이 종료되었습니다
	(0 : [에러코드])*/
	FGsUIHelper::HideBlockUI();

	FText errorText;
	FText::FindText(TEXT("NetText"), TEXT("NetDisconected"), errorText);

	FString text = FString::Format(*errorText.ToString(), { FGsNet::Mode::LOBBY,  inErrorCode });
	FGsUIHelper::PopupNetError(FText::FromString(text));

#if WITH_EDITOR
	GMessage()->GetSystem().SendMessage(MessageSystem::EXIT_GAME);
#else
	GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
	
#endif
}

void FGsNetLobby::OnExitGame()
{
	Disconnect();
}

void FGsNetLobby::OnAddressQueryRecevied(const FAddressInfoResult& inResult)
{
	GSLOG(Warning, TEXT("Got %d GAI Results for hostname %s. Error Code: [%d]"), inResult.Results.Num(), *inResult.QueryHostName, inResult.ReturnCode);

	if (SE_NO_ERROR != inResult.ReturnCode)
	{
		GMessage()->GetSystem().SendMessageAsync(MessageSystem::AUTH_SERVER_QUERY_IP_FAIL);
	}
	else
	{
		if (inResult.Results.Num() < 1)
		{
			GMessage()->GetSystem().SendMessageAsync(MessageSystem::AUTH_SERVER_QUERY_IP_FAIL);
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

				if (_addressInfo.IsValid())
				{
					break;
				}
			}

			if (false == _addressInfo.IsValid())
			{
				_addressInfo = inResult.Results[0].Address;
			}
			
			GMessage()->GetSystem().SendMessageAsync(_addressInfo.IsValid() ? MessageSystem::AUTH_SERVER_QUERY_IP_SUCCESS : MessageSystem::AUTH_SERVER_QUERY_IP_FAIL);
#else
			_addressInfo = inResult.Results[0].Address;

			GMessage()->GetSystem().SendMessageAsync(MessageSystem::AUTH_SERVER_QUERY_IP_SUCCESS);
#endif			
		}
	}
}
