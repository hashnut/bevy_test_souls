#pragma once

#include "GsNetBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GSNet.h"
#include "Shared/Shared/SharedPackets/PD_Auth_Client.h"
#include "T1Project/UTIL/GsTimeUtil.h"

struct FAddressInfoResult;

class FGsNetLobby : public FGsNetBase, protected FGsNetBase::ISessionNotify
{
	using Super = FGsNetBase;

	static constexpr double LIMIT_TIME_SEND_GATEWAY_PONG = 7.0;
	double LastTime = 0.0;

	bool										_isSocketConnecting = false;	// 소켓 연결 중 여부
	FGsSimpleTimer								_socketConnectFailedTimer;		// 소켓 연결 실패 감지 타이머

	bool										_isHandshakeCompleted = false;
	TPair<PD::AC::ACPacketId, FDelegateHandle>	_pongPacketDelegate;

public:
	FGsNetLobby() { SetSessionName(TEXT("Auth")); }
	virtual ~FGsNetLobby() {}

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float indeltaTime) override;

	virtual void SetRestMode(bool InIsRest) override;

public:
	virtual void ReConnect() override;

private:
	void RecvPongAck(const FGsNet::Packet& inPacket);

private:
	void SendAuthPing();

private:
	void OnSocketConnectFailedByTimer();

public:
	virtual void OnHandshakeCompleted() override;
	virtual void OnKeepAliveCheck() override;
	virtual void OnDisConnected(const FClientSession* inSession) override;
	virtual void OnConnected(const FClientSession* inSession) override;
	virtual void OnPacketReceived(unsigned char*, int) override;
	virtual void OnErrorRecevied(int32 inErrorCode) override;
	virtual void OnExitGame() override;
	virtual void OnAddressQueryRecevied(const FAddressInfoResult&) override;

	virtual uint32 GetPort() override { return _port;}
};
