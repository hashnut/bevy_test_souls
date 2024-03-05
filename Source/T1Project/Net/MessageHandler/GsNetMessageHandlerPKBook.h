// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsNetMessageHandleInterface.h"
#include "../Message/GsMessageNet.h"
#include "Net/GSNet.h"



/**
 *
 */
class T1PROJECT_API FGsNetMessageHandlerPKBook : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerPKBook() = default;
	virtual ~FGsNetMessageHandlerPKBook() = default;

	virtual void InitializeMessage(class FGsNetManager* InManager) override;
	virtual void FinalizeMessage() override;

protected:
	/**
	 * PK Record Data
	 */
	void NOTIFY_PK_BOOK_LIST(const FGsNet::Packet& InPacket);
	void NOTIFY_ADD_PK_BOOK_INFO(const FGsNet::Packet& InPacket);
	
	/**
	 * PK Record Detail Info
	 */
	void ACK_PK_BOOK_DETAIL(const FGsNet::Packet& InPacket);
	void ACK_PK_BOOK_REVENGE(const FGsNet::Packet& InPacket);
	void ACK_PK_BOOK_TEASE(const FGsNet::Packet& InPacket);

	/**
	 * PK Detection Target Data
	 */
	void NOTIFY_MORNITORING_USER_LIST(const FGsNet::Packet& InPacket);
	void ACK_ADD_MONITORING_USER(const FGsNet::Packet& InPacket);
	void ACK_DELETE_MONITORING_USER(const FGsNet::Packet& InPacket);

	/**
	 * PK Message
	 */
	void NOTIFY_PK_MESSAGE_INFO(const FGsNet::Packet& InPacket);
	void NOTIFY_MONITORING_USER_LOGIN_INFO(const FGsNet::Packet& InPacket);
	void ACK_SAVE_PK_MESSAGE(const FGsNet::Packet& InPacket);
	void NOTIFY_PK_BOOK_TEASE_MESSAGE(const FGsNet::Packet& InPacket);

	void ACK_SAVE_PK_TEASE_MESSAGE(const FGsNet::Packet& InPacket);

	// 2023/1/18 PKT - 우리 길드원이 다른 길드원에게 죽었을 때.
	void NOTIFY_PK_GUILD_I_GOT_PK(const FGsNet::Packet& InPacket);
	// 2023/1/18 PKT - 우리 길드원이 다른 길드원을 죽였을 때.
	void NOTIFY_PK_GUILD_I_DID_PK(const FGsNet::Packet& InPacket);
};
