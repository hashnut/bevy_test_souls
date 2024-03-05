// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/GSNet.h"
#include "GsNetMessageHandleInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_Auth_Client.h"

/**
 *
 */
class T1PROJECT_API FGsNetMessageHandlerAuth : public IGsNetMessageHandler, FGsNetMessageAuth
{
private:
	struct Cached_PKT_AC_ACK_SELECT_SERVER_READ final
	{
		const FString _ip;
		const uint16 _port;
		const AuthTicket _authTicket;
		const WorldId _planetWorldId;
		const FString _clientIpAddress;

		Cached_PKT_AC_ACK_SELECT_SERVER_READ(PD::AC::PKT_AC_ACK_SELECT_SERVER_READ& InPacket)
			: _ip(InPacket.Ip())
			, _port(InPacket.Port())
			, _authTicket(InPacket.AuthTicket())
			, _planetWorldId(InPacket.PlanetWorldId())
			, _clientIpAddress(InPacket.ClientIpAddress())
		{
		}
	};

	/**
	 * PKT_AC_ACK_SELECT_SERVER_READ 패킷 수신 시, 성공인 경우에 한하여, 나중에 사용하기 위해 일부 데이터를 캐쉬함.
	 */
	TSharedPtr<Cached_PKT_AC_ACK_SELECT_SERVER_READ> _cachedSucceededPktAcAckSelectServer;

public:
	FGsNetMessageHandlerAuth() = default;
	virtual ~FGsNetMessageHandlerAuth() = default;

public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

protected:
	void KickedByTimeOutAck(const FGsNet::Packet &inPacket);

	void PasswordAck(const FGsNet::Packet &inPacket);
	void CheckVersionAck(const FGsNet::Packet& inPacket);
	void PreLoginAck(const FGsNet::Packet& inPacket);
	void LoginAck(const FGsNet::Packet &inPacket);
	void ServerSelectAck(const FGsNet::Packet& inPacket);
	void AckAppGuardAuth(const FGsNet::Packet& inPacket);
	void PostServerSelectAck();
	void AckDeleteAccount(const FGsNet::Packet& inPacket);

	void IdpConflictPlayerAck(const FGsNet::Packet& inPacket);
};

FString LexToString(const PD::AC::PKT_AC_ACK_PRE_LOGIN_READ& InPacket);
