#pragma once

#include "CoreMinimal.h"
#include "GsNetMessageHandleInterface.h"

class FGsNetManager;
/**
 *
 */
class T1PROJECT_API FGsNetMessageHandlerItem : public IGsNetMessageHandler, FGsNetMessageWorld
{
public:
	FGsNetMessageHandlerItem() = default;
	virtual ~FGsNetMessageHandlerItem() = default;

public:
	virtual void InitializeMessage(FGsNetManager* inManager) override;
	virtual void FinalizeMessage() override;

	// Ack
public:
	void CreateItemAck(const FGsNet::Packet& inPacket);
	void UseItemAck(const FGsNet::Packet& inPacket);
	void SeveralUseItemAck(const FGsNet::Packet& inPacket);
	void UseEquipItemAck(const FGsNet::Packet& inPacket);
	void UseItemLockAck(const FGsNet::Packet& inPacket);
	void EquipItemEnchantAck(const FGsNet::Packet& inPacket);
	void EquipItemMultiEnchantAck(const FGsNet::Packet& inPacket);
	void EquipItemRefineOptionAck(const FGsNet::Packet& inPacket);
	void EquipItemMagicalForgeAck(const FGsNet::Packet& inPacket);
	void ItemDecomposeAck(const FGsNet::Packet& inPacket);
	void ChangeQuickSlotInfoAck(const FGsNet::Packet& inPacket);
	void QuickSlotAutoUseStateAck(const FGsNet::Packet& inPacket);
	void NpcShopItemBuyAck(const FGsNet::Packet& inPacket);
	void NpcShopItemSellAck(const FGsNet::Packet& inPacket);
	void NpcEventShopItemBuyAck(const FGsNet::Packet& inPacket);
	void DepositItemAck(const FGsNet::Packet& inPacket);
	void TakeItemAck(const FGsNet::Packet& inPacket);
	void ExtendDepotSlotAck(const FGsNet::Packet& inPacket);
	void ExtendInvenSlotAck(const FGsNet::Packet& inPacket);
	void PickOneItemAck(const FGsNet::Packet& inPacket);
	void SeveralPickOneItemAck(const FGsNet::Packet& inPacket);
	void NpcAutoSettingAck(const FGsNet::Packet& inPacket);
	void AutoUsePotionStateAck(const FGsNet::Packet& inPacket);
	void InvenSortValueAck(const FGsNet::Packet& inPacket);
	void DeliveryBoxRecoverEquipAck(const FGsNet::Packet& inPacket);
	void CharacterCustomizingTicketUseAck(const FGsNet::Packet& inPacket);

	void AckItemFusionResult(const FGsNet::Packet& inPacket);

	// Notify
protected:
	void ItemUpdateNotify(const FGsNet::Packet& inPacket);
	void InventoryItemNotify(const FGsNet::Packet& inPacket);
	void InventorySlotNotify(const FGsNet::Packet& inPacket);
	void EquipItemNotify(const FGsNet::Packet& inPacket);
	void UserUseItemNotify(const FGsNet::Packet& inPacket);	
	void ItemDeleteNotify(const FGsNet::Packet& inPacket);
	void ItemCoolTimeNotify(const FGsNet::Packet& inPacket);
	void QuickSlotItemListNotify(const FGsNet::Packet& inPacket);
	void AutoUseQuickSlotItemNotify(const FGsNet::Packet& inPacket);
	void DepotItemNotify(const FGsNet::Packet& inPacket);
	void DepotEquipItemNotify(const FGsNet::Packet& inPacket);
	void DepotMaxSlotCountNotify(const FGsNet::Packet& inPacket);
	void NpcAutoSettingNotify(const FGsNet::Packet& inPacket);
	void ExpiredTimeNotify(const FGsNet::Packet& inPacket);
	void DisplayRewardNotify(const FGsNet::Packet& inPacket);
	void ItemUseDeliveryBoxEquipNotify(const FGsNet::Packet& inPacket);
	void ItemUseDeliveryBoxFairyComposeNotify(const FGsNet::Packet& inPacket);
	void ItemUseDeliveryBoxCostumeComposeNotify(const FGsNet::Packet& inPacket);
};
