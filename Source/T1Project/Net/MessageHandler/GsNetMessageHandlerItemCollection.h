#pragma once

#include "GsNetMessageHandleInterface.h"

class FGsNetManager;

class T1PROJECT_API FGsNetMessageHandlerItemCollection : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerItemCollection() = default;

	/************************************************************************/
	/* instancing                                                           */
	/************************************************************************/

public:
	void InitializeMessage(FGsNetManager* InManager) override;
	void FinalizeMessage() override;

	/************************************************************************/
	/* notify                                                               */
	/************************************************************************/

protected:
	void Notify_ITEM_COLLECTION(const FGsNet::Packet& InPacket);

	void Notify_ITEM_COLLECTION_EVENT_ON(const FGsNet::Packet& InPacket);
	void Notify_ITEM_COLLECTION_EVENT_OFF(const FGsNet::Packet& InPacket);

	/************************************************************************/
	/* acknowledge                                                          */
	/************************************************************************/
protected:
	void Ack_ITEM_COLLECTION_REGISTER(const FGsNet::Packet& InPacket);

	void Ack_ITEM_COLLECTION_BOOKMARK_CREATE(const FGsNet::Packet& InPacket);
	void Ack_ITEM_COLLECTION_BOOKMARK_DELETE(const FGsNet::Packet& InPacket);
};

