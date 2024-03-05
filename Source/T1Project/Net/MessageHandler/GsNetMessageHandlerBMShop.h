// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsNetMessageHandleInterface.h"

class FGsNetManager;
/**
 * 
 */
class T1PROJECT_API FGsNetMessageHandlerBMShop : public IGsNetMessageHandler, FGsNetMessageWorld
{
protected:
	TArray<TPair<PD::GC::GCPacketId, FDelegateHandle>> _gateWayPackets;

public:
	FGsNetMessageHandlerBMShop() = default;
	virtual ~FGsNetMessageHandlerBMShop() = default;

public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

	// ACK
public:
	void BMShopProductListAck(const FGsNet::Packet& inPacket);
	void BMShopPurchaseProductAck(const FGsNet::Packet& inPacket);
	void BMShopStorageItemListAck(const FGsNet::Packet& inPacket);
	void BMShopReceiveStorageItem(const FGsNet::Packet& inPacket);
	void BMShopRestoreItem(const FGsNet::Packet& inPacket);
	void BMShopProductValidCheckAck(const FGsNet::Packet& inPacket);
	void BMShopPG_PurchaseAck(const FGsNet::Packet& inPacket);
	void BMShopSaveBulkSettingAck(const FGsNet::Packet& inPacket);
	void BMShopOpenPopupShopAck(const FGsNet::Packet& inPacket);
	void BMShopSearchZpayUser(const FGsNet::Packet& inPacket);

public:
	void BMShopZpayProductListAck(const FGsNet::Packet& inPacket);
	void BMShopZpayPurchaseProductAck(const FGsNet::Packet& inPacket);
	void BMShopZpayRestoreItem(const FGsNet::Packet& inPacket);
	void BMShopZpayProductValidCheckAck(const FGsNet::Packet& inPacket);
	
public:
	// Notify
	void BMShopInformGuildGiftNotify(const FGsNet::Packet& inPacket);
	void BMShopSaveBulkSettingNotify(const FGsNet::Packet& inPacket);
	void BMShopOpenPopupShopNotify(const FGsNet::Packet& inPacket);
	void BMShopPurchaseZpayNotify(const FGsNet::Packet& inPacket);
	void BMShopCallZpayUserNotify(const FGsNet::Packet& inPacket);
	void BMShopNothingToRestoreNotify(const FGsNet::Packet& inPacket);
	void BMShopBlockListNotify(const FGsNet::Packet& inPacket);
	void NotifyPgUnconsumedPurchase(const FGsNet::Packet& inPacket);
	
	/**
	 * Server Migrate 
	 */
public:
	void ServerMigrationConditionListAck(const FGsNet::Packet& inPacket);
	void ServerMigrationAck(const FGsNet::Packet& inPacket);
};
