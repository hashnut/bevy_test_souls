// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNetMessageHandlerItem.h"
#include "Shared/Client/SharedEnums/SharedSpawnEnum.h"
#include "Shared/Client/SharedEnums/SharedDebugEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

#include "UTIL/GsText.h"
#include "UTIL/GsDir.h"
#include "UTIL/GsLevelUtil.h"
#include "UTIL/GsDebugUtil.h"
#include "UTIL/GsTableUtil.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"
#include "Management/ScopeGame/GsQuickSlotManager.h"
#include "Management/ScopeGame/GsNpcShopManager.h"
#include "Management/ScopeGame/GsDepotManager.h"
#include "Management/ScopeGame/GsItemCollectionManager.h"
#include "Management/ScopeGame/GsInventoryManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsItemEnchantManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGame/GsSaveBatteryManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"

#include "GameMode/GsGameModeWorld.h"
#include "Sound/GsSoundPlayer.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsCoolTimeMessageParam.h"
#include "Message/MessageParam/GsNpcShopMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"

#include "Item/GsItemManager.h"
#include "Item/InputBlock/GsInputBlockDefine.h"
#include "Item/InputBlock/GsInputBlockHandler.h"
//RSQL
#include "DataSchema/GameObject/GsSchemaShapeData.h"
#include "DataSchema/GameObject/Projectile/GsSchemaProjectileSet.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/GsSchemaGameMode.h"
#include "DataSchema/GsSchemaEnums.h"

#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/ObjectClass/GsGameObjectPlayer.h"

#include "UI/UIContent/Popup/GsUIPopupItemReceive.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"

#include "Currency/GsCurrencyHelper.h"

#include "Option/GsServerOption.h"

#include "AI/ContentsCondition/GsAIContentsConditionDefault.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Guild/GsAgitHandler.h"
#include "GameObject/Quest/GsQuestHandler.h"


using namespace PD::SC;
void FGsNetMessageHandlerItem::InitializeMessage(FGsNetManager* inManager)
{
	// 패킷 바인딩
	MProtocalWorld& worldProtocal = inManager->GetProtocalWorld();
	
	InitializeMessageDelegateList(&worldProtocal);

	// Packet
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ITEMS_UPDATE, this, &FGsNetMessageHandlerItem::ItemUpdateNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_INVENTORY, this, &FGsNetMessageHandlerItem::InventoryItemNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_INVENTORY_EQUIP, this, &FGsNetMessageHandlerItem::EquipItemNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_USER_USE_ITEM, this, &FGsNetMessageHandlerItem::UserUseItemNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ITEM_CREATE, this, &FGsNetMessageHandlerItem::CreateItemAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_ITEM_USE, this, &FGsNetMessageHandlerItem::UseItemAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_ITEM_EQUIP, this, &FGsNetMessageHandlerItem::UseEquipItemAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_ITEM_LOCK, this, &FGsNetMessageHandlerItem::UseItemLockAck
	));	
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ITEM_DELETE, this, &FGsNetMessageHandlerItem::ItemDeleteNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_ITEM_ENCHANT, this, &FGsNetMessageHandlerItem::EquipItemEnchantAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_ITEM_ENCHANT_MULTI, this, &FGsNetMessageHandlerItem::EquipItemMultiEnchantAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_ITEM_REFINE, this, &FGsNetMessageHandlerItem::EquipItemRefineOptionAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_ITEM_MAGICAL_FORGE, this, &FGsNetMessageHandlerItem::EquipItemMagicalForgeAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_ITEM_DECOMPOSE, this, &FGsNetMessageHandlerItem::ItemDecomposeAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_COOLDOWN, this, &FGsNetMessageHandlerItem::ItemCoolTimeNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_QUICK_SLOT, this, &FGsNetMessageHandlerItem::QuickSlotItemListNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_QUICK_SLOT_SET, this, &FGsNetMessageHandlerItem::ChangeQuickSlotInfoAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_QUICK_SLOT_AUTO, this, &FGsNetMessageHandlerItem::QuickSlotAutoUseStateAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_QUICK_SLOT_AUTO_USE, this, &FGsNetMessageHandlerItem::AutoUseQuickSlotItemNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_ITEM_BUY, this, &FGsNetMessageHandlerItem::NpcShopItemBuyAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_ITEM_SELL, this, &FGsNetMessageHandlerItem::NpcShopItemSellAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_NPC_EVENT_SHOP_PURCHASE, this, &FGsNetMessageHandlerItem::NpcEventShopItemBuyAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_DEPOT, this, &FGsNetMessageHandlerItem::DepotItemNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_DEPOT_EQUIP, this, &FGsNetMessageHandlerItem::DepotEquipItemNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_MOVE_INVENTORY_ITEM_TO_DEPOT, this, &FGsNetMessageHandlerItem::DepositItemAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_MOVE_DEPOT_ITEM_TO_INVENTORY, this, &FGsNetMessageHandlerItem::TakeItemAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_DEPOT_EXTENDED_SLOT, this, &FGsNetMessageHandlerItem::ExtendDepotSlotAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_DEPOT_SLOT, this, &FGsNetMessageHandlerItem::DepotMaxSlotCountNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_ITEM_SELECT_REWARD, this, &FGsNetMessageHandlerItem::PickOneItemAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_INVENTORY_SLOT, this, &FGsNetMessageHandlerItem::InventorySlotNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_INVENTORY_EXTENDED_SLOT, this, &FGsNetMessageHandlerItem::ExtendInvenSlotAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_AUTO_PURCHASE_UPDATE, this, &FGsNetMessageHandlerItem::NpcAutoSettingAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_AUTO_PURCHASE, this, &FGsNetMessageHandlerItem::NpcAutoSettingNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_POTION_AUTO_USE_ON_OFF, this, &FGsNetMessageHandlerItem::AutoUsePotionStateAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_INFORM_ITEM_EXPIRED, this, &FGsNetMessageHandlerItem::ExpiredTimeNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_NOTIFY_DISPLAY_REWARD, this, &FGsNetMessageHandlerItem::DisplayRewardNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_INVENTORY_CUSTOM_SORT, this, &FGsNetMessageHandlerItem::InvenSortValueAck
	));

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_ITEM_USE_SEVERAL, this, &FGsNetMessageHandlerItem::SeveralUseItemAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_ITEM_SELECT_REWARD_SEVERAL, this, &FGsNetMessageHandlerItem::SeveralPickOneItemAck
	));

	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ITEM_USE_DELIVERY_BOX_EQUIP, this, &FGsNetMessageHandlerItem::ItemUseDeliveryBoxEquipNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ITEM_USE_DELIVERY_BOX_FAIRY_COMPOSE, this, &FGsNetMessageHandlerItem::ItemUseDeliveryBoxFairyComposeNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ITEM_USE_DELIVERY_BOX_COSTUME_COMPOSE, this, &FGsNetMessageHandlerItem::ItemUseDeliveryBoxCostumeComposeNotify
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_DELIVERY_BOX_RECOVER_EQUIP, this, &FGsNetMessageHandlerItem::DeliveryBoxRecoverEquipAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_CHARACTER_CUSTOMIZING_TICKET_USE, this, &FGsNetMessageHandlerItem::CharacterCustomizingTicketUseAck
	));
	AddMessageDelegate(worldProtocal.AddRaw(
		SCPacketId::SC_ACK_ITEM_FUSE, this, &FGsNetMessageHandlerItem::AckItemFusionResult
	));
}

void FGsNetMessageHandlerItem::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

// 아이템 신규 습득했을 시 내려오는 Packet
void FGsNetMessageHandlerItem::CreateItemAck(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("CreateItemAck()"));

	PKT_SC_ITEM_CREATE_READ* pkt = reinterpret_cast<PKT_SC_ITEM_CREATE_READ*>(inPacket.Buffer);
	int result = pkt->Result();
	if (PACKET_RESULT_SUCCESS == result)
	{
		return;
	}

	// auto play시 drop item 있을때 멍때리고 있는 이슈 대응
	// 자동 이동시 ai에서 서버 응답대기 건거 해지
	// https://jira.com2us.com/jira/browse/C2URWQ-4599
	if (FGsAIManager* aiManager = GSAI())
	{
		aiManager->GetContentsConditionDefault()->SetIsLootServerResponseWait(false);
	}
	
	FText findText;
	switch (result)
	{
	case PD::Result::ITEM_ERROR_ADD_INVENTORY_FULL:
	{
		// 가방이 가득찼습니다.			
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}
		break;
	case  PD::Result::ITEM_ERROR_ADD_WEIGHT_FULL:
	{
		// 소지 무게가 최대치입니다.			
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}
		break;
	case PD::Result::DROP_OBJECT_LOOT_ERROR_ALREADY_IN_LOOT:
	{
		FText::FindText(TEXT("DropObjectText"), TEXT("AlreadyLoot"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}
		break;
	case PD::Result::DROP_OBJECT_LOOT_ERROR_NOT_MY_ITEM_PARTY_LOOT:
	{
		//다른 파티원이 아이템 획득 했을 때 내려오는 에러 로그, 정확히는 에러 로그가 아니고 파티원이 아이템 먹을 수 있는 정상 상황, 디버깅용 로그
		return;
	}
		break;
	}

	FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
}

void FGsNetMessageHandlerItem::ItemUpdateNotify(const FGsNet::Packet& inPacket)
{
	PKT_SC_ITEMS_UPDATE_READ* Pkt = reinterpret_cast<PKT_SC_ITEMS_UPDATE_READ*>(inPacket.Buffer);
	check(nullptr != Pkt);

	const ItemStorageType CurrentItemStoragetype = Pkt->ItemStoragetype();

	// 로그
#if !UE_BUILD_SHIPPING
	{
		TSet<ItemDBId> DuplicateChecker;
		FString DebugLogs = FString::Printf(TEXT("CurrentItemStoragetype:%d\n"), CurrentItemStoragetype);

		for (int offset = 0; offset < Pkt->ItemDatasCreatedSize(); offset += sizeof(ItemDataCreated))
		{
			const ItemDataCreated* data = reinterpret_cast<const ItemDataCreated*>(Pkt->ItemDatasCreated() + offset);
			check(nullptr != data);
			check(INVALID_ITEM_DB_ID != data->mItemDBId);
			check(INVALID_ITEM_ID != data->mTemplateId);
			check(0 < data->mItemAmount);
			DebugLogs += FString::Printf(TEXT("ItemDatasCreated[]: {mItemDBId:%llu, mTemplateId:%d, mItemAmount:%d}\n"), data->mItemDBId, data->mTemplateId, data->mItemAmount);

			if (DuplicateChecker.Contains(data->mItemDBId))
			{
				GSLOG(Error, TEXT("DuplicateChecker.Contains(%llu)"), data->mItemDBId);
			}
			else
			{
				DuplicateChecker.Add(data->mItemDBId);
			}
		}

		for (int offset = 0; offset < Pkt->ItemDatasEquipCreatedSize(); offset += sizeof(ItemDataEquip))
		{
			const ItemDataEquip* data = reinterpret_cast<const ItemDataEquip*>(Pkt->ItemDatasEquipCreated() + offset);
			check(nullptr != data);
			check(INVALID_ITEM_DB_ID != data->mItemDBId);
			check(INVALID_ITEM_ID != data->mTemplateId);
			DebugLogs += FString::Printf(TEXT("ItemDatasEquipCreated[]: {mItemDBId:%llu, mTemplateId:%d, mItemAmount:%d}\n"), data->mItemDBId, data->mTemplateId, data->mItemAmount);

			if (DuplicateChecker.Contains(data->mItemDBId))
			{
				GSLOG(Error, TEXT("DuplicateChecker.Contains(%llu)"), data->mItemDBId);
			}
			else
			{
				DuplicateChecker.Add(data->mItemDBId);
			}
		}

		for (int offset = 0; offset < Pkt->ItemDBIdsDeletedSize(); offset += sizeof(ItemDBId))
		{
			const ItemDBId* data = reinterpret_cast<const ItemDBId*>(Pkt->ItemDBIdsDeleted() + offset);
			check(nullptr != data);
			check(INVALID_ITEM_DB_ID != *data);
			DebugLogs += FString::Printf(TEXT("ItemDBIdDeleted[]: %llu\n"), *data);

			if (DuplicateChecker.Contains(*data))
			{
				GSLOG(Error, TEXT("DuplicateChecker.Contains(%llu)"), *data);
			}
			else
			{
				DuplicateChecker.Add(*data);
			}
		}

		for (int offset = 0; offset < Pkt->ItemsStackUpdatedSize(); offset += sizeof(ItemDBIdAmountPair))
		{
			const ItemDBIdAmountPair* data = reinterpret_cast<const ItemDBIdAmountPair*>(Pkt->ItemsStackUpdated() + offset);
			check(nullptr != data);
			check(INVALID_ITEM_DB_ID != data->mItemDBId);
			check(0 < data->mItemAmount);
			DebugLogs += FString::Printf(TEXT("ItemsStackUpdated[]: {mItemDBId:%llu, mItemAmount:%d}\n"), data->mItemDBId, data->mItemAmount);

			if (DuplicateChecker.Contains(data->mItemDBId))
			{
				GSLOG(Error, TEXT("DuplicateChecker.Contains(%llu)"), data->mItemDBId);
			}
			else
			{
				DuplicateChecker.Add(data->mItemDBId);
			}
		}

		GSLOG(Log, TEXT("%s"), *DebugLogs);
	}
#endif

	// 데이터 준비

	// 비장비 아이템 추가
	TArray<ItemDataCreated> ItemDatasCreated;
	const int ItemDatasCreatedSize = Pkt->ItemDatasCreatedSize();
	if (0 < ItemDatasCreatedSize)
	{
		ItemDatasCreated.Reset(ItemDatasCreatedSize);
		Pkt->MakeItemDatasCreatedVector<TArray<ItemDataCreated>, ItemDataCreated>(ItemDatasCreated);
	}

	// 장비 아이템 추가
	TArray<ItemDataEquip> ItemDatasEquipCreated;
	const int ItemDatasEquipCreatedSize = Pkt->ItemDatasEquipCreatedSize();
	if (0 < ItemDatasEquipCreatedSize)
	{
		ItemDatasEquipCreated.Reset(ItemDatasEquipCreatedSize);
		Pkt->MakeItemDatasEquipCreatedVector<TArray<ItemDataEquip>, ItemDataEquip>(ItemDatasEquipCreated);
	}

	// 아이템 제거
	TArray<ItemDBId> ItemDBIdsDeleted;
	const int ItemDBIdsDeletedSize = Pkt->ItemDBIdsDeletedSize();
	if (0 < ItemDBIdsDeletedSize)
	{
		ItemDBIdsDeleted.Reset(ItemDBIdsDeletedSize);
		Pkt->MakeItemDBIdsDeletedVector<TArray<ItemDBId>, ItemDBId>(ItemDBIdsDeleted);
	}

	// Stack Count 만 변경
	TArray<ItemDBIdAmountPair> ItemsStackUpdated;
	const int ItemsStackUpdatedSize = Pkt->ItemsStackUpdatedSize();
	if (0 < ItemsStackUpdatedSize)
	{
		ItemsStackUpdated.Reset(ItemsStackUpdatedSize);
		Pkt->MakeItemsStackUpdatedVector<TArray<ItemDBIdAmountPair>, ItemDBIdAmountPair>(ItemsStackUpdated);
	}

	// 데이터 적용

	if (ItemStorageType::INVENTORY == CurrentItemStoragetype)
	{
		// 아이템 저장소 적용

		UGsItemManager* ItemManager = GItem();
		if (nullptr == ItemManager)
		{
			GSLOG(Error, TEXT("nullptr == ItemManager"));
			return;
		}

		if (0 < ItemDBIdsDeleted.Num())
		{
			ItemManager->RemoveItemList(ItemDBIdsDeleted);
		}

		for (const ItemDBIdAmountPair& ItemStackUpdated : ItemsStackUpdated)
		{
			ItemManager->UpdateItemAmountByDBID(ItemStackUpdated.mItemDBId, ItemStackUpdated.mItemAmount);
		}

		if (0 < ItemDatasCreated.Num())
		{
			ItemManager->AddItemList(ItemDatasCreated, true);
		}
		if (0 < ItemDatasEquipCreated.Num())
		{
			ItemManager->AddEquipItemList(ItemDatasEquipCreated, true);
		}

		/*
		for (const ItemDataEquip& ItemDataEquipCreated : ItemDatasEquipCreated)
		{
			ItemManager->AddItem(&ItemDataEquipCreated, true, ItemDatasEquipCreated.Num());
		}
		*/

		//GMessage()->GetItemContents().SendMessage(MessageContentItem::INVALIDATE_INVENTORY, nullptr);
	}
	else if (ItemStorageType::DEPOT == CurrentItemStoragetype)
	{
		// 창고 저장소 적용

		FGsDepotManager* DepotManager = GSDepot();
		if (nullptr == DepotManager)
		{
			GSLOG(Error, TEXT("nullptr == DepotManager"));
			return;
		}

		if (0 < ItemDBIdsDeleted.Num())
		{
			DepotManager->RemoveDepotItemList(ItemDBIdsDeleted);
		}

		for (const ItemDBIdAmountPair& ItemStackUpdated : ItemsStackUpdated)
		{
			DepotManager->AddOrUpdateDepotItem(ItemStackUpdated.mItemDBId, ItemStackUpdated.mItemAmount);
		}

		for (const ItemDataCreated& CurrentItemDataCreated : ItemDatasCreated)
		{
			DepotManager->AddOrUpdateDepotItem(&CurrentItemDataCreated);
		}

		for (const ItemDataEquip& ItemDataEquipCreated : ItemDatasEquipCreated)
		{
			DepotManager->AddOrUpdateDepotItem(&ItemDataEquipCreated);
		}

		GMessage()->GetItemContents().SendMessage(MessageContentItem::INVALIDATE_DEPOT, nullptr);
	}
	else
	{
		GSLOG(Error, TEXT("Need to implementation: CurrentItemStoragetype:%d"), CurrentItemStoragetype);
	}

	ItemUpdateReason itemUpdateReason = Pkt->ItemUpdateReason();
	if (itemUpdateReason  == ItemUpdateReason::DROP_GET)
	{
		// Release after server response
		if(FGsAIManager* aiManager = GSAI())
		{ 
			aiManager->GetContentsConditionDefault()->SetIsLootServerResponseWait(false);
		}
	}
	else if(itemUpdateReason == ItemUpdateReason::ITEM_USE)
	{ 
		if(FGsInteractionManager* interactionManager = GSInteraction())
		{ 
			interactionManager->ClearCurrentConsumeCategory();
		}
	}
}

void FGsNetMessageHandlerItem::InventoryItemNotify(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("InventoryItemNotify()"));

	PKT_SC_INVENTORY_READ* pkt = reinterpret_cast<PKT_SC_INVENTORY_READ*>(inPacket.Buffer);
	const uchar* Invenbuf = pkt->ItemDataList(); // ItemData들의 메모리 시작점
	int size = pkt->ItemDataListSize(); // ItemData들의 바이트 크기

	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
		return;

	if (0 < size)
	{
		TArray<const ItemData*> itemDataList;
		int dataSize = sizeof(ItemData);
		int count = size / dataSize;

		int binIndex = 0;
		for (int i = 0; i < count; ++i)
		{
			const ItemData* data = reinterpret_cast<const ItemData*>(&Invenbuf[binIndex]);
			if (nullptr != data)
			{
				itemDataList.Add(data);
			}
			binIndex += sizeof(ItemData);
		}
		itemMgr->AddItem(itemDataList, false);
	}
	//GMessage()->GetItemContents().SendMessage(MessageContentItem::INVALIDATE_INVENTORY, nullptr);

	GSLOG(Warning, TEXT("SC_INVENTORY ItemDataListSize:%d"), size);
}

void FGsNetMessageHandlerItem::EquipItemNotify(const FGsNet::Packet &inPacket)
{
	GSLOG(Warning, TEXT("EquipItemNotify()"));

	PKT_SC_INVENTORY_EQUIP_READ* pkt = reinterpret_cast<PKT_SC_INVENTORY_EQUIP_READ*>(inPacket.Buffer);
	const uchar* Invenbuf = pkt->ItemDataEquipList(); // ItemData들의 메모리 시작점
	int size = pkt->ItemDataEquipListSize(); // ItemData들의 바이트 크기

	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
		return;

	if (0 < size)
	{
		TArray<const ItemDataEquip*> itemDataEquipList;
		int dataSize = sizeof(ItemDataEquip);
		int count = size / dataSize;

		int binIndex = 0;
		for (int i = 0; i < count; ++i)
		{
			const ItemDataEquip* data = reinterpret_cast<const ItemDataEquip*>(&Invenbuf[binIndex]);
			if (nullptr != data)
			{
				itemDataEquipList.Add(data);
			}
			binIndex += sizeof(ItemDataEquip);
		}
		itemMgr->AddItem(itemDataEquipList, false);
	}
	//GMessage()->GetItemContents().SendMessage(MessageContentItem::INVALIDATE_INVENTORY, nullptr);

	GSLOG(Warning, TEXT("SC_INVENTORY_EQUIP ItemDataListSize:%d"), size);
}

void FGsNetMessageHandlerItem::InventorySlotNotify(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("InventorySlotNotify()"));

	PKT_SC_INVENTORY_SLOT_READ* pkt = reinterpret_cast<PKT_SC_INVENTORY_SLOT_READ*>(inPacket.Buffer);
	if (true == pkt->Validate())
	{
		UGsInventoryManager* invenMgr = GSInventory();
		if (nullptr == invenMgr)
			return;

		invenMgr->SetInvenSlotMaxCount(pkt->MaxSlotCount());
		invenMgr->SetInvenTotalExtendCount(pkt->ExtendedNumber());
		invenMgr->WarmUpInvenIconSlot();
	}
}

// 창고에 있으면 인벤토리 노티파이 패킷에서는 해당 아이템을 안내려주는지??
void FGsNetMessageHandlerItem::DepotItemNotify(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("DepotItemNotify()"));

	PKT_SC_DEPOT_READ* pkt = reinterpret_cast<PKT_SC_DEPOT_READ*>(inPacket.Buffer);
	const uchar* buf = pkt->ItemDataList(); // ItemData들의 메모리 시작점
	int size = pkt->ItemDataListSize(); // ItemData들의 바이트 크기
	if (0 < size)
	{
		FGsDepotManager* depotMgr = GSDepot();
		if (nullptr == depotMgr)
			return;

		int dataSize = sizeof(ItemData);
		int count = size / dataSize;

		int binIndex = 0;
		for (int i = 0; i < count; ++i)
		{
			const ItemData* data = reinterpret_cast<const ItemData*>(&buf[binIndex]);
			if (nullptr != data)
			{
				depotMgr->AddOrUpdateDepotItem(data);
			}
			binIndex += sizeof(ItemData);
		}
	}
	GSLOG(Warning, TEXT("DepotItemNotify - ItemDataListSize:%d"), size);
}

void FGsNetMessageHandlerItem::DepotEquipItemNotify(const FGsNet::Packet& inPacket)
{
	check(GItem());
	GSLOG(Warning, TEXT("DepotEquipItemNotify"));

	PKT_SC_DEPOT_EQUIP_READ* pkt = reinterpret_cast<PKT_SC_DEPOT_EQUIP_READ*>(inPacket.Buffer);
	const uchar* buf = pkt->ItemDataEquipList(); // ItemData들의 메모리 시작점
	int size = pkt->ItemDataEquipListSize(); // ItemData들의 바이트 크기
	if (0 < size)
	{
		FGsDepotManager* depotMgr = GSDepot();
		if (nullptr == depotMgr)
			return;

		int dataSize = sizeof(ItemDataEquip);
		int count = size / dataSize;

		int binIndex = 0;
		for (int i = 0; i < count; ++i)
		{
			const ItemDataEquip* data = reinterpret_cast<const ItemDataEquip*>(&buf[binIndex]);
			if (nullptr != data)
			{
				depotMgr->AddOrUpdateDepotItem(data);
			}
			binIndex += sizeof(ItemDataEquip);
		}
	}
	GSLOG(Warning, TEXT("DepotEquipItemNotify - ItemDataListSize:%d"), size);
}

void FGsNetMessageHandlerItem::UseEquipItemAck(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("RespUseEquipItem()"));

	//FGsUIHelper::HideBlockUI();
	GItem()->SetIsRecvSameEquipTypeProcPacket(true);

	PKT_SC_ACK_ITEM_EQUIP_READ* pkt = reinterpret_cast<PKT_SC_ACK_ITEM_EQUIP_READ*>(inPacket.Buffer);
	int64 result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		if (result == PD::Result::ITEM_ERROR_INVALID_USER_STATUS) // 상태이상 상태일때 내려오는 errorID
		{
			// 스턴 상태일때 불가능 합니다
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		else if (result == PD::Result::CHANGE_WEAPON_TYPE_ERROR_IN_CHANGE_CLASS)
		{
			// 아이템 장착에 실패하였습니다. 다시 시도해주세요.
			FText findText;
			FText::FindText(TEXT("UICommonText"), TEXT("EquipFailure"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			return;
		}
		// 잠긴 아이템을 사용하려고 할 때 (C2URWQ-4443)
		else if (result == PD::Result::ITEM_ERROR_USE_LOCKED)
		{
			// 잠긴 아이템은 사용할 수 없습니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Use_Notice_Locked"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText, TrayCondition::TC_Close);
			return;
		}

		FGsUIHelper::PopupNetError((PD::Result)pkt->Result());
		GSLOG(Warning, TEXT("[RespUseEquipItem - Error Result] ResultID : %d"), result);
		return;
	}

	// ItemUpdateNotify() 함수에서 일괄 처리하고 있으므로 기존 처리부분 주석
	// 혹여나 다른곳에 영향 받을 수 있을까봐 우선은 주석처리
	//TArray<ItemDataEquip> itemsDataEquipList;
	//pkt->MakeItemDataUpdatedVector<TArray<ItemDataEquip>, ItemDataEquip>(itemsDataEquipList);
	//itemMgr->UpdateEquipItem(itemsDataEquipList);
}

// 내가 사용한 아이템에 대한 packet
void FGsNetMessageHandlerItem::UseItemAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
		return;

	if (FGsInputBlockHandler* inputBlockHandler = itemMgr->GetInputBlockHandler())
	{
		inputBlockHandler->DeleteBlockCondition(EGsInputBlockConditionType::RECV_USE_ITEM);
	}

	GSLOG(Warning, TEXT("RespUseItem()"));

	PKT_SC_ACK_ITEM_USE_READ* pkt = reinterpret_cast<PKT_SC_ACK_ITEM_USE_READ*>(inPacket.Buffer);
	int64 result = pkt->Result();
	
	if (result != PACKET_RESULT_SUCCESS)
	{
		if (result == PD::Result::ITEM_ERROR_ABNORMALITY_STAT_ALREADY_IN_EFFECT)
		{
			FText findText;
			FText::FindText(TEXT("NetText"), TEXT("ITEM_ERROR_ABNORMALITY_STAT_ALREADY_IN_EFFECT"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		if (result == PD::Result::ITEM_ERROR_INVALID_USER_STATUS || // 상태이상 상태일때 내려오는 errorID
			result == PD::Result::WARP_ERROR_CAN_NOT_DO_ACTION_WARP)
		{
			// 스턴 상태일때 불가능 합니다
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), findText);
			FGsUIHelper::TrayMessageTicker(findText);

			// 이동 못하는곳에서 워프 사용시 클리어
			// https://jira.com2us.com/jira/browse/C2URWQ-5584
			if (FGsInteractionManager* interactionManager = GSInteraction())
			{
				interactionManager->ClearCurrentConsumeCategory();
			}

			// 워프관련 이동 아이템 사용여부 삭제
			if (GSQuest())
			{
				if (FGsQuestHandler* questHandler = GSQuest()->GetQuestHandler())
				{
					questHandler->ClearUsingWarpScroll();
				}
				return;
			}
			return;
		}
		if (result == PD::Result::ITEM_ERROR_USE_POTION_HP_MP_FULL) // 만피 일때
		{
			return;
		}
		if (result == PD::Result::TREASURE_MONSTER_ERROR_ALREADY_CREATED ||
			result == PD::Result::TREASURE_MONSTER_ERROR_SAFE_TYTERRITORY ||
			result == PD::Result::TREASURE_MONSTER_ERROR_INVALID_AREA)
		{
			FGsUIHelper::TraySectionMessageTickerNetError(static_cast<PD::Result>(result));
			return;
		}
		
		if (result == PD::Result::ITEM_ERROR_UNLOCK)
		{
			FText findText;
			FText::FindText(TEXT("UICommonText"), TEXT("ItemUseFail_Alert"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		
		if(result == PD::Result::CHARACTER_SLOT_UNLOCK_ERROR_UNKNOWN)
		{
			// 해당 아이템은 사용할 수 없습니다.
			FText findText;
			FText::FindText(TEXT("UnlockText"), TEXT("Notice_Item_use_Lock"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		if (result == PD::Result::INVADE_ERROR_INVALID_TIME ||
			result == PD::Result::INVADE_ERROR_PORTAL_MAX_COUNT ||
			result == PD::Result::INVADE_ERROR_INVALID_MAP)
		{
			FGsUIHelper::TraySectionMessageTickerNetError(static_cast<PD::Result>(result));
			return;
		}
		if (result == PD::Result::INVADE_ERROR_UNKNOWN)
		{
			FText findText;
			if (true == FText::FindText(TEXT("InvadeText"), TEXT("Invade_Cant_Spawn_01"), findText))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText, TrayCondition::TC_Close);
			}
			//FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		if (result == PD::Result::CAMP_ERROR_NEAR_CAMPS_EXIST || result == PD::Result::CAMP_ERROR_SPAWN_CAMP)
		{
			return;
		}

		if (result == PD::Result::CHARACTER_SLOT_UNLOCK_ERROR_EXCEEDED)
		{
			FText findText;
			if (true == FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_CharacterSlotFull"), findText))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText, TrayCondition::TC_Close);
			}
			//FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		if (result == PD::Result::SKILL_ERROR_LEARN_INVALID_ESSENTIAL_PASSIVITY)
		{
			// TEXT: 필수 패시브를 보유해야 해당 스킬을 배울 수 있습니다
			FText findText;
			if (FText::FindText(TEXT("SkillUIText"), TEXT("Notice_UseSkillBook_NoPreviousSkill"), findText))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText, TrayCondition::TC_Close);
			}

			return;
		}
		if (result == PD::Result::INVADE_ERROR_NOT_FOUND_INVADE_MAP_DATA)
		{
			// TEXT: 현재 지역에서는 침공 포털을 생성할 수 없습니다.
			FText findText;
			if (FText::FindText(TEXT("NetText"), TEXT("INVADE_ERROR_NOT_FOUND_INVADE_MAP_DATA"), findText))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText, TrayCondition::TC_Close);
			}
			return;
		}

		// [B1] | ejrrb10 | 퀵슬롯에 등록한 아이템을 PC 에서 바인딩한 키로 광클 시, 에러 팝업 출력 이슈 해결
		// 전사 테스트 버그 제보 122 번
		//  https://docs.google.com/spreadsheets/d/10lZh9h72yNBLd7j-LqgbL1EpyczaM_lOJAG2Zzmv-qE/edit?pli=1#gid=0
		if (result == PD::Result::ITEM_ERROR_USE_IN_COOLDOWN)
		{
			// TEXT: 아이템 사용 실패 - 재사용 대기시간이 지나지 않았습니다.
			FText findText;
			if (FText::FindText(TEXT("NetText"), TEXT("ITEM_ERROR_USE_IN_COOL_DOWN"), findText))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText, TrayCondition::TC_Close);
			}
			return;
		}
		if (result == PD::Result::ITEM_ERROR_USE_INVALID_EFFECT_PARAM)
		{			
			FText findText;
			if (FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Resurrection_Full"), findText))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText, TrayCondition::TC_Close);
			}
			return;
		}

		if (result == PD::Result::ITEM_ERROR_USE_MAX_USED_BONUS_STAT_POINT_ITEM)
		{
			FText findText;
			FText::FindText(TEXT("StatText"), TEXT("Notice_BonusStatItem_MaxConsume"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText, TrayCondition::TC_Close);
			return;
		}

		// 잠긴 아이템을 사용하려고 할 때 (C2URWQ-4443)
		if (result == PD::Result::ITEM_ERROR_LOCK_ALREADY_LOCKED)
		{
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Use_Notice_Locked"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText, TrayCondition::TC_Close);
			return;
		}

		//pvp 성향이 0일때 면죄부 사용할 경우 예외 처리(jira.com2us.com/jira/browse/CHR-20387)
		if (result == PD::Result::ITEM_ERROR_USE_PVP_ALIGNMENT)
		{
			FText findText;
			FText::FindText(TEXT("NetText"), TEXT("ITEM_ERROR_USE_PVP_ALIGNMENT"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText, TrayCondition::TC_Close);
			return;
		}

		if (result == PD::Result::ITEM_ERROR_NOT_ALLOWED_MAP)
		{
			FGsUIHelper::TraySectionMessageTickerNetError(static_cast<PD::Result>(result));
			return;
		}

		// 기사단 영지 관련 에러코드 체크
		if (FGsAgitHandler::CheckAgitError(pkt->Result()))
		{
			return;
		}

		FGsUIHelper::PopupNetError((PD::Result)pkt->Result());
		GSLOG(Warning, TEXT("[RespUseItem - Error Result] ResultID : %d"), result);
		return;
	}

	const ItemId UsedItemId = pkt->UseItemId();
	if (INVALID_ITEM_ID == UsedItemId)
	{
		GSLOG(Warning, TEXT("UsedItemId:%d"), UsedItemId);
	}
	else
	{
		itemMgr->UsedItemEffect(UsedItemId);
		itemMgr->SendItemEventUI(UsedItemId);
	}
}

void FGsNetMessageHandlerItem::SeveralUseItemAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	GSLOG(Warning, TEXT("SeveralUseItemAck()"));

	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
		return;

	PKT_SC_ACK_ITEM_USE_SEVERAL_READ* pkt = reinterpret_cast<PKT_SC_ACK_ITEM_USE_SEVERAL_READ*>(inPacket.Buffer);
	int64 result = pkt->Result();

	if (result != PACKET_RESULT_SUCCESS)
	{
		if (result == PD::Result::ITEM_ERROR_INVALID_USER_STATUS) // 상태이상 상태일때 내려오는 errorID
		{
			// 스턴 상태일때 불가능 합니다
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		if (result == PD::Result::TREASURE_MONSTER_ERROR_ALREADY_CREATED ||
			result == PD::Result::TREASURE_MONSTER_ERROR_SAFE_TYTERRITORY ||
			result == PD::Result::TREASURE_MONSTER_ERROR_INVALID_AREA)
		{
			FGsUIHelper::TraySectionMessageTickerNetError(static_cast<PD::Result>(result));
			return;
		}
		if (result == PD::Result::ITEM_ERROR_UNLOCK)
		{
			// 해당 아이템은 사용할 수 없습니다.
			FText findText;
			FText::FindText(TEXT("UnlockText"), TEXT("Notice_Item_use_Lock"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		if (result == PD::Result::INVADE_ERROR_INVALID_TIME ||
			result == PD::Result::INVADE_ERROR_PORTAL_MAX_COUNT ||
			result == PD::Result::INVADE_ERROR_INVALID_MAP)
		{
			FGsUIHelper::TraySectionMessageTickerNetError(static_cast<PD::Result>(result));
			return;
		}
		if (result == PD::Result::ITEM_ERROR_ADD_INVENTORY_FULL) // 인벤토리 풀
		{
			// 가방이 가득찼습니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		if (result == PD::Result::ITEM_ERROR_ADD_WEIGHT_FULL) // 인벤토리 무게 풀
		{
			// 소지 무게가 최대치입니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		const ItemId UsedItemId = pkt->UseItemId();
		if (INVALID_ITEM_ID == UsedItemId || 0 >= pkt->UseItemAmount())
		{
			FGsUIHelper::PopupNetError((PD::Result)pkt->Result());
			GSLOG(Warning, TEXT("[RespUseItem - Error Result] ResultID : %d"), result);
		}
		else
		{
			if (0 < pkt->UseItemAmount())
			{
				itemMgr->UsedItemEffect(UsedItemId);
				itemMgr->SendItemEventUI(UsedItemId);
				if (pkt->UseItemAmount() < pkt->ReqItemAmount())
				{
					FText findText;
					if (true == FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_BatchUse_NotEnoughSlot"), findText))
					{
						FText resultText = FText::Format(findText, pkt->UseItemAmount());
						FGsUIHelper::TrayMessageTicker(resultText);
					}
				}
			}
		}

		return;
	}

	const ItemId UsedItemId = pkt->UseItemId();	
	if (INVALID_ITEM_ID == UsedItemId)
	{
		GSLOG(Warning, TEXT("UsedItemId:%d"), UsedItemId);
	}
	else
	{
		if (0 < pkt->UseItemAmount())
		{
			itemMgr->UsedItemEffect(UsedItemId);
			itemMgr->SendItemEventUI(UsedItemId);

			if (pkt->UseItemAmount() < pkt->ReqItemAmount())
			{
				FText findText;
				if (true == FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_BatchUse_NotEnoughSlot"), findText))
				{
					FText resultText = FText::Format(findText, pkt->UseItemAmount());
					FGsUIHelper::TrayMessageTicker(resultText);
				}
			}
		}
	}
}

// 타유저가 아이템 사용한거에 대한 Resp
void FGsNetMessageHandlerItem::UserUseItemNotify(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("RespUserUseItem()"));

	PKT_SC_USER_USE_ITEM_READ* pkt = reinterpret_cast<PKT_SC_USER_USE_ITEM_READ*>(inPacket.Buffer);

	GSLOG(Warning, TEXT("[RespUserUseItem] GameId : %d"), pkt->GameId());
	GSLOG(Warning, TEXT("[RespUserUseItem] ItemId : %d"), pkt->ItemId());

	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		if (UGsGameObjectBase* objBase = mgr->FindObject(EGsGameObjectType::Player, pkt->GameId()))
		{
			if (UGsGameObjectPlayer* player = Cast<UGsGameObjectPlayer>(objBase))
			{
				const FGsSchemaItemCommon* findTbl =
					UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(pkt->ItemId());
				if (findTbl != nullptr)
				{
					player->OnUseItem(findTbl->categorySub);
				}
			}
		}
	}
}

void FGsNetMessageHandlerItem::UseItemLockAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("UseItemLockAck()"));

	PKT_SC_ACK_ITEM_LOCK_READ* pkt = reinterpret_cast<PKT_SC_ACK_ITEM_LOCK_READ*>(inPacket.Buffer);
	int64 result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		if (result == PD::Result::ITEM_ERROR_INVALID_USER_STATUS) // 상태이상 상태일때 내려오는 errorID
		{
			// 스턴 상태일때 불가능 합니다
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		// 광클로 인해 잠김 상태를 서버 응답이 오기 전에 갱신 하는 경우, 굳이 티커 메시지를 따로 띄워주지 않아도
		// 잠금 UI 에서 알아서 티커 메시지를 띄워주므로, 에러 팝업만 띄워주지 않아도 될 것으로 보인다
		// 이미 잠겨 있을 때 내려오는 errorID
		if (result == PD::Result::ITEM_ERROR_LOCK_ALREADY_LOCKED) 
		{
			//// 이미 잠겨있는 아이템입니다.
			//FText findText;
			//FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Already_Notice_Locked"), findText);
			//FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		// 이미 잠금 해제되어 있을 때 내려오는 errorID
		if (result == PD::Result::ITEM_ERROR_LOCK_ALREADY_UNLOCKED)
		{
			//// 이미 잠금 해제된 아이템입니다.
			//FText findText;
			//FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_AlreadyOpen_Notice_Locked"), findText);
			//FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		if (result == PD::Result::ITEM_ERROR_INVALID_ITEM_DB_ID)
		{
			// 삭제된 아이템에 대한 요청이므로 무시한다.
			return;
		}

		FGsUIHelper::PopupNetError((PD::Result)pkt->Result());
		GSLOG(Warning, TEXT("[UseItemLockAck - Error Result] ResultID : %d"), result);
		return;
	}

	GItem()->UpdateItemLockByDBID(pkt->ItemDBId(), pkt->Locked(), pkt->ItemStoragetype());
}

void FGsNetMessageHandlerItem::ItemDeleteNotify(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("ItemDeleteNotify()"));

	PKT_SC_ITEM_DELETE_READ* pkt = reinterpret_cast<PKT_SC_ITEM_DELETE_READ*>(inPacket.Buffer);
	int64 result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		if (result == PD::Result::ITEM_ERROR_INVALID_USER_STATUS) // 상태이상 상태일때 내려오는 errorID
		{
			// 스턴 상태일때 불가능 합니다
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		FGsUIHelper::PopupNetError((PD::Result)pkt->Result());
		GSLOG(Warning, TEXT("[ItemDeleteNotify - Error Result] ResultID : %d"), result);
		return;
	}
}

void FGsNetMessageHandlerItem::EquipItemRefineOptionAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	GSLOG(Warning, TEXT("EquipItemRefineOptionAck()"));
	PKT_SC_ACK_ITEM_REFINE_READ* pkt = reinterpret_cast<PKT_SC_ACK_ITEM_REFINE_READ*>(inPacket.Buffer);
	int64 result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		UGsUIManager* uiManager = GUI();
		if (nullptr == uiManager)
			return;

		if (true == uiManager->IsActiveWidget(TEXT("PopupItemEnchant")))
		{
			uiManager->CloseByKeyName(TEXT("PopupItemEnchant"));
		}

		if (result == PD::Result::ITEM_ERROR_INVALID_USER_STATUS) // 상태이상 상태일때 내려오는 errorID
		{
			// 스턴 상태일때 불가능 합니다
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		if (result == PD::Result::ITEM_ERROR_REFINE_LOCKED)
		{
			// TEXT: 잠금 처리된 아이템은 재련할 수 없습니다
			FText findText;
			FText::FindText(TEXT("ItemEnchantText"), TEXT("UI_Refinemnet_Ticker_ItemLocked"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		FGsUIHelper::TraySectionMessageTickerNetError((PD::Result)pkt->Result());
		GSLOG(Warning, TEXT("[EquipItemRefineOptionAck - Error Result] ResultID : %d"), result);
		return;
	}
	ItemRefineResult resultType = pkt->RefineResult();
	ItemDataEquip targetItemData = pkt->TargetItemDataUpdated();
	ItemDBId useItemDbid = pkt->UseItemDBId();

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[EquipItemRefineOptionAck] targetItem DBID : %llu"), targetItemData.mItemDBId);
	GSLOG(Warning, TEXT("[EquipItemRefineOptionAck] ItemRefineResult : %d"), (uint8)resultType);
	// GSLOG(Warning, TEXT("[EquipItemRefineOptionAck] finalMoney : %llu"), finalMoney);
#endif
	GItem()->SendItemEventUI(useItemDbid);
	GSItemEnchant()->UpdateRefineOptionItem(&targetItemData, resultType);
	//GMessage()->GetItemContents().SendMessage(MessageContentItem::INVALIDATE_INVENTORY, nullptr);
}

void FGsNetMessageHandlerItem::EquipItemMagicalForgeAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	GSLOG(Warning, TEXT("EquipItemMagicalForgeAck()"));
	PKT_SC_ACK_ITEM_MAGICAL_FORGE_READ* pkt = reinterpret_cast<PKT_SC_ACK_ITEM_MAGICAL_FORGE_READ*>(inPacket.Buffer);
	int64 result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		UGsUIManager* uiManager = GUI();
		if (nullptr == uiManager)
			return;

		if (true == uiManager->IsActiveWidget(TEXT("PopupItemEnchant")))
		{
			uiManager->CloseByKeyName(TEXT("PopupItemEnchant"));
		}

		if (result == PD::Result::ITEM_ERROR_INVALID_USER_STATUS)
		{
			// 스턴 상태일때 불가능 합니다
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		FGsUIHelper::PopupNetError((PD::Result)pkt->Result());
		return;
	}

	ItemMagicalForgeResult resultType = pkt->ItemMagicalForgeResult();
	ItemDataEquip targetItemData = pkt->TargetItemDataUpdated();

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[EquipItemMagicalForgeAck] Target Item DBID : %llu"), targetItemData.mItemDBId);
	GSLOG(Warning, TEXT("[EquipItemMagicalForgeAck] Result : %d"), (uint8)resultType);
#endif
	//GItem()->SendItemEventUI(useItemDbid);
	if (FGsItemEnchantManager* enchantManager = GSItemEnchant())
	{
		enchantManager->UpdateMagicalForgeItem(&targetItemData, resultType);
	}
}

void FGsNetMessageHandlerItem::EquipItemMultiEnchantAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	GSLOG(Warning, TEXT("EquipItemMultiEnchantAck()"));
	PKT_SC_ACK_ITEM_ENCHANT_MULTI_READ* pkt = reinterpret_cast<PKT_SC_ACK_ITEM_ENCHANT_MULTI_READ*>(inPacket.Buffer);
	int64 result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::CLOSE, nullptr);
		
		if (result == PD::Result::ITEM_ERROR_INVALID_USER_STATUS) // 상태이상 상태일때 내려오는 errorID
		{
			// 스턴 상태일때 불가능 합니다
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		// 인벤토리 무게 초과 상태
		// 인벤토리 슬롯 풀 상태
		if (result == PD::Result::ITEM_ERROR_ADD_WEIGHT_FULL || result == PD::Result::ITEM_ERROR_ADD_INVENTORY_FULL)
		{
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_cannot_try"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		FGsUIHelper::PopupNetError((PD::Result)pkt->Result());
		GSLOG(Warning, TEXT("[EquipItemEnchantAck - Error Result] ResultID : %d"), result);
		return;
	}

	UGsItemManager* itemMgr = GItem();
	TArray<ItemDataCreated> ItemDatasCreated;
	TArray<ItemDBIdAmountPair> ItemDatasIncrease;
	TArray<ItemDBIdAmountPair> totalOwnedPairList;
	TArray<ItemEnchantResultData> resultDataList;
	TArray<ItemDBId> resultIngredientDBidList;
	if (0 < pkt->ItemEnchantResultDataListSize())
	{
		resultDataList.Reset(pkt->ItemEnchantResultDataListSize());
		pkt->MakeItemEnchantResultDataListVector<TArray<ItemEnchantResultData>, ItemEnchantResultData>(resultDataList);
#if WITH_EDITOR
		for (ItemEnchantResultData& data : resultDataList)
		{
			GSLOG(Warning, TEXT("[EquipItemMultiEnchantAck] ItemEnchantResult : %d"), (uint8)data.mItemEnchantResult);
			GSLOG(Warning, TEXT("[EquipItemMultiEnchantAck] targetItem DBID : %llu"), data.mItemDataEquip.mItemDBId);
			GSLOG(Warning, TEXT("[EquipItemMultiEnchantAck] targetItem Level : %d"), data.mItemDataEquip.mLevel);
		}
#endif
	}
	if (0 < pkt->UseScrollItemDBIdListSize())
	{
		resultIngredientDBidList.Reset(pkt->UseScrollItemDBIdListSize());
		pkt->MakeUseScrollItemDBIdListVector<TArray<ItemDBId>, ItemDBId >(resultIngredientDBidList);
		itemMgr->SendItemEventUI(resultIngredientDBidList);
#if WITH_EDITOR
		for (ItemDBId iter : resultIngredientDBidList)
		{
			GSLOG(Warning, TEXT("[EquipItemMultiEnchantAck] UseScrollItemDbid : %llu"), iter);
		}
#endif
	}
	if (0 < pkt->ItemDataCreatedSize())
	{
		ItemDatasCreated.Reset(pkt->ItemDataCreatedSize());
		pkt->MakeItemDataCreatedVector<TArray<ItemDataCreated>, ItemDataCreated>(ItemDatasCreated);
		TArray<const ItemData*> itemDataList;
		for (const ItemDataCreated& iter : ItemDatasCreated)
		{
			itemDataList.Add(&iter);
			totalOwnedPairList.Emplace(iter.mItemDBId, iter.mItemAmount);
		}
		itemMgr->AddItem(itemDataList, true);
	}
	if (0 < pkt->ItemIncreasedSize())
	{
		ItemDatasIncrease.Reset(pkt->ItemIncreasedSize());
		pkt->MakeItemIncreasedVector<TArray<ItemDBIdAmountPair>, ItemDBIdAmountPair>(ItemDatasIncrease);
		for (const ItemDBIdAmountPair& iter : ItemDatasIncrease)
		{
			TWeakPtr<FGsItem> findItem = itemMgr->FindItem(iter.mItemDBId);
			if (findItem.IsValid())
			{
				ItemAmount oriCount = findItem.Pin()->GetAmount();
				itemMgr->UpdateItemAmountByDBID(iter.mItemDBId, iter.mItemAmount);

				if (ItemCategoryMain::SCROLL == findItem.Pin()->GetMainCategory())
				{
					itemMgr->SendItemEventUI(findItem.Pin()->GetDBID());
					continue;
				}
				ItemAmount resultAmount = iter.mItemAmount - oriCount;
				totalOwnedPairList.Emplace(iter.mItemDBId, resultAmount);
			}
		}
	}
	if (0 < totalOwnedPairList.Num())
	{
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_EnchantResult_Destroy"), findText);
		FGsUIHelper::AddChatMessageSystem(findText);
		for (const ItemDBIdAmountPair& iter : totalOwnedPairList)
		{
			TWeakPtr<FGsItem> findItem = itemMgr->FindItem(iter.mItemDBId);
			if (findItem.IsValid())
			{
				if (true == FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_EnchantFail"), findText))
				{
					FText resultText = FText::Format(findText, findItem.Pin()->GetName(), iter.mItemAmount);
					FGsUIHelper::AddChatMessageSystem(resultText);
				}
			}
		}
	}

	GSItemEnchant()->UpdateMultiEnchantItem(resultDataList, resultIngredientDBidList);
}

void FGsNetMessageHandlerItem::EquipItemEnchantAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	GSLOG(Warning, TEXT("EquipItemEnchantAck()"));
	PKT_SC_ACK_ITEM_ENCHANT_READ* pkt = reinterpret_cast<PKT_SC_ACK_ITEM_ENCHANT_READ*>(inPacket.Buffer);
	int64 result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		UGsUIManager* uiManager = GUI();
		if (nullptr == uiManager)
			return;

		if (true == uiManager->IsActiveWidget(TEXT("TrayEnchantItemWait")))
		{
			uiManager->CloseByKeyName(TEXT("TrayEnchantItemWait"));
		}
		if (true == uiManager->IsActiveWidget(TEXT("PopupItemEnchant")))
		{
			uiManager->CloseByKeyName(TEXT("PopupItemEnchant"));
		}
		if (true == uiManager->IsActiveWidget(TEXT("PopupCollectionEnchant")))
		{
			uiManager->CloseByKeyName(TEXT("PopupCollectionEnchant"));
		}

		if (result == PD::Result::ITEM_ERROR_INVALID_USER_STATUS) // 상태이상 상태일때 내려오는 errorID
		{
			// 스턴 상태일때 불가능 합니다
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		// 인벤토리 무게 초과 상태
		// 인벤토리 슬롯 풀 상태
		if (result == PD::Result::ITEM_ERROR_ADD_WEIGHT_FULL || result == PD::Result::ITEM_ERROR_ADD_INVENTORY_FULL)
		{
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_cannot_try"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		FGsUIHelper::PopupNetError((PD::Result)pkt->Result());
		GSLOG(Warning, TEXT("[EquipItemEnchantAck - Error Result] ResultID : %d"), result);
		return;
	}

	const ItemEnchantResult enchantResultType = pkt->EnchantResult();
	const ItemDataEquip targetItemData = pkt->TargetItemDataUpdated();
	const ItemDBId scollItemDBId = pkt->ScrollItemDBId();

	UGsItemManager* itemMgr = GItem();
	TArray<ItemDataCreated> ItemDatasCreated;
	TArray<ItemDBIdAmountPair> ItemDatasIncrease;
	TArray<ItemDBIdAmountPair> totalOwnedPairList;
	if (0 < pkt->ItemDataCreatedSize())
	{
		ItemDatasCreated.Reset(pkt->ItemDataCreatedSize());
		pkt->MakeItemDataCreatedVector<TArray<ItemDataCreated>, ItemDataCreated>(ItemDatasCreated);
		TArray<ItemDataCreated> itemDataList;
		for (const ItemDataCreated& iter : ItemDatasCreated)
		{
			itemDataList.Emplace(iter);
			totalOwnedPairList.Emplace(iter.mItemDBId, iter.mItemAmount);
		}
		itemMgr->AddItemList(itemDataList, true);
	}
	if (0 < pkt->ItemIncreasedSize())
	{
		ItemDatasIncrease.Reset(pkt->ItemIncreasedSize());
		pkt->MakeItemIncreasedVector<TArray<ItemDBIdAmountPair>, ItemDBIdAmountPair>(ItemDatasIncrease);
		for (const ItemDBIdAmountPair& iter : ItemDatasIncrease)
		{
			TWeakPtr<FGsItem> findItem = itemMgr->FindItem(iter.mItemDBId);
			if (findItem.IsValid())
			{
				ItemAmount oriCount = findItem.Pin()->GetAmount();
				itemMgr->UpdateItemAmountByDBID(iter.mItemDBId, iter.mItemAmount);

				if (ItemCategoryMain::SCROLL == findItem.Pin()->GetMainCategory())
					continue;

				ItemAmount resultAmount = iter.mItemAmount - oriCount;
				totalOwnedPairList.Emplace(iter.mItemDBId, resultAmount);
			}
		}
	}

	FText findText;
	if (ItemEnchantResult::ENCHANT_FAILED_DESTROYED == enchantResultType)
	{
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_EnchantResult_Destroy"), findText);
		FGsUIHelper::AddChatMessageSystem(findText);
	}
	for (const ItemDBIdAmountPair& iter : totalOwnedPairList)
	{
		TWeakPtr<FGsItem> findItem = itemMgr->FindItem(iter.mItemDBId);
		if (findItem.IsValid())
		{
			if (true == FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_EnchantFail"), findText))
			{
				FText resultText = FText::Format(findText, findItem.Pin()->GetName(), iter.mItemAmount);
				FGsUIHelper::AddChatMessageSystem(resultText);
			}
		}
	}

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[EquipItemEnchantAck] targetItem DBID : %llu"), targetItemData.mItemDBId);
	GSLOG(Warning, TEXT("[EquipItemEnchantAck] ItemEnchantResult : %d"), (uint8)enchantResultType);
	GSLOG(Warning, TEXT("[EquipItemEnchantAck] scollItemDBId : %d"), scollItemDBId);
#endif

	itemMgr->SendItemEventUI(scollItemDBId);
	GSItemEnchant()->UpdateSingleEnchantItem(&targetItemData, scollItemDBId, enchantResultType);
	//GMessage()->GetItemContents().SendMessage(MessageContentItem::INVALIDATE_INVENTORY, nullptr);
}

void FGsNetMessageHandlerItem::ItemDecomposeAck(const FGsNet::Packet& inPacket)
{
	PKT_SC_ACK_ITEM_DECOMPOSE_READ* pkt = reinterpret_cast<PKT_SC_ACK_ITEM_DECOMPOSE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
		return;

	// 자동 분해일 경우 Notify 성격이므로 BlockUI를 해제하지 않는다.
	if (false == pkt->IsAutoDecompose())
	{
		FGsUIHelper::HideBlockUI();
	}
	
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		if (result == PD::Result::ITEM_ERROR_ADD_WEIGHT_FULL) // 인벤토리 무게 풀
		{
			// 소지 무게가 최대치입니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		else if (result == PD::Result::ITEM_ERROR_DELETE_LOCKED)
		{
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Decompose_Notice_Locked"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	TArray<ItemIdAmountPair> itemListCreated; // 분해 후 얻은 아이템 정보
	pkt->MakeResultItemDecomposeVector<TArray<ItemIdAmountPair>, ItemIdAmountPair>(itemListCreated);

	FGsSaveBatteryManager* batteryManager = GSSaveBattery();
	if (nullptr == batteryManager)
		return;

	// 절전모드 ON : 분해 결과를 모았다가 보여준다.
	if (batteryManager->IsSaveBatteryTurnedOn())
	{
		for (ItemIdAmountPair itemDataCreated : itemListCreated)
		{
			batteryManager->AddAutoDecomposeItemAmount(itemDataCreated.mItemId, itemDataCreated.mItemAmount);
		}
	}
	// 절전모드 OFF : 분해 결과를 바로 보여준다.
	else
	{
		// "아이템을 분해했습니다."
		FText completeText;
		FText::FindText(TEXT("DecomposeText"), TEXT("Decompose_Complete"), completeText);
		FGsUIHelper::TrayMessageTicker(completeText);

		for (ItemIdAmountPair itemDataCreated : itemListCreated)
		{
			const FGsSchemaItemCommon* itemCommon = UGsItemManager::GetItemTableDataByTID(itemDataCreated.mItemId);
			if (nullptr == itemCommon)
				continue;

			FText findText;
			if (true == FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Decompose_result"), findText))
			{
				FText resultText = FText::Format(findText, itemCommon->name, itemDataCreated.mItemAmount);
				FGsUIHelper::AddChatMessageSystem(resultText);
			}
		}
	}

	FGsItemMultiDecomposeResult param(pkt->IsAutoDecompose());
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DECOMPOSE_RESULT, &param);
}

// 패킷받는 시점 : 인게임 진입할 때 / 맵 전환 할때 / 재연결 됬을때
void FGsNetMessageHandlerItem::ItemCoolTimeNotify(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("ItemCoolTimeNotify()"));
	PKT_SC_COOLDOWN_READ* pkt = reinterpret_cast<PKT_SC_COOLDOWN_READ*>(inPacket.Buffer);
	using cooldownIter = PktIterator<PD::SC::PKT_SC_COOLDOWN_READ::CooldownList>;
	for (cooldownIter iter = pkt->GetFirstCooldownListIterator(); iter != pkt->GetLastCooldownListIterator(); ++iter)
	{
		ItemCooldownGroupId groupID = iter->ItemCooldownGroupId();
		float remainTime = iter->RemainTime();

#if WITH_EDITOR
		GSLOG(Warning, TEXT("FGsNetMessageHandlerItem::ItemCoolTimeNotify - GroupID : %d"), groupID);
		GSLOG(Warning, TEXT("FGsNetMessageHandlerItem::ItemCoolTimeNotify - RemainTime : %f"), remainTime);
#endif

		GCoolTime()->SetCoolTime(EGsCoolTime::Item, groupID, 0, remainTime);
		FGsCoolTimeMessageParam messageParam(EGsCoolTime::Item, groupID, 0, remainTime);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_COOLTIME_START, &messageParam);
	}
}

void FGsNetMessageHandlerItem::QuickSlotItemListNotify(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("QuickSlotItemListNotify()"));

	UGsQuickSlotManager* slotMgr = GSQuickSlot();
	PKT_SC_QUICK_SLOT_READ* pkt = reinterpret_cast<PKT_SC_QUICK_SLOT_READ*>(inPacket.Buffer);
	using quickslotIter = PktIterator<PD::SC::PKT_SC_QUICK_SLOT_READ::QuickSlotList>;
	for (quickslotIter iter = pkt->GetFirstQuickSlotListIterator(); iter != pkt->GetLastQuickSlotListIterator(); ++iter)
	{
		QuickSlotId slotID = iter->QuickSlotId();
		ItemDBId itemDBID = iter->ItemDBId();
		ItemId itemTID = iter->ItemId();
		bool isUsingAuto = iter->AutoUse();

#if WITH_EDITOR
		GSLOG(Warning, TEXT("FGsNetMessageHandlerItem::QuickSlotItemListNotify - slotID : %d"), slotID);
		GSLOG(Warning, TEXT("FGsNetMessageHandlerItem::QuickSlotItemListNotify - itemDBID : %llu"), itemDBID);
		GSLOG(Warning, TEXT("FGsNetMessageHandlerItem::QuickSlotItemListNotify - itemTID : %d"), itemTID);
		GSLOG(Warning, TEXT("FGsNetMessageHandlerItem::QuickSlotItemListNotify - isUsingAuto : %d"), (uint8)isUsingAuto);
#endif

		slotMgr->SetPacketData(slotID, itemDBID, itemTID, isUsingAuto);
	}
	//GSQuickSlot()->FinishedPacketData();
}

void FGsNetMessageHandlerItem::ChangeQuickSlotInfoAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("ChangeQuickSlotInfoAck()"));

	UGsQuickSlotManager* slotMgr = GSQuickSlot();
	PKT_SC_ACK_QUICK_SLOT_SET_READ* pkt = reinterpret_cast<PKT_SC_ACK_QUICK_SLOT_SET_READ*>(inPacket.Buffer);
	int64 result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError((PD::Result)pkt->Result());
		GSLOG(Warning, TEXT("[ChangeQuickSlotInfoAck - Error Result] ResultID : %d"), result);
		return;
	}

	bool isUnRegister = false;
	TArray<QuickSlotId> invalidateSlotIdList;
	using quickslotIter = PktIterator<PD::SC::PKT_SC_ACK_QUICK_SLOT_SET_READ::QuickSlotList>;
	for (quickslotIter iter = pkt->GetFirstQuickSlotListIterator(); iter != pkt->GetLastQuickSlotListIterator(); ++iter)
	{
		isUnRegister = 0 >= iter->ItemId();
		QuickSlotId slotID = iter->QuickSlotId();
		ItemDBId itemDBID = iter->ItemDBId();
		ItemId itemTID = iter->ItemId();
		bool isUsingAuto = iter->AutoUse();
		
#if WITH_EDITOR
		GSLOG(Warning, TEXT("FGsNetMessageHandlerItem::ChangeQuickSlotInfoAck - slotID : %d"), slotID);
		GSLOG(Warning, TEXT("FGsNetMessageHandlerItem::ChangeQuickSlotInfoAck - itemDBID : %llu"), itemDBID);
		GSLOG(Warning, TEXT("FGsNetMessageHandlerItem::ChangeQuickSlotInfoAck - itemTID : %d"), itemTID);
		GSLOG(Warning, TEXT("FGsNetMessageHandlerItem::ChangeQuickSlotInfoAck - isUsingAuto : %d"), (uint8)isUsingAuto);
#endif

		slotMgr->SetPacketData(slotID, itemDBID, itemTID, isUsingAuto);
		invalidateSlotIdList.Emplace(slotID);
	}
	slotMgr->InvalidateSlotList(invalidateSlotIdList);
	slotMgr->CheckSound(true == (1 == invalidateSlotIdList.Num() && isUnRegister) ? false : true);
}

void FGsNetMessageHandlerItem::QuickSlotAutoUseStateAck(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("QuickSlotAutoUseStateAck()"));

	UGsQuickSlotManager* slotMgr = GSQuickSlot();
	PKT_SC_ACK_QUICK_SLOT_AUTO_READ* pkt = reinterpret_cast<PKT_SC_ACK_QUICK_SLOT_AUTO_READ*>(inPacket.Buffer);
	int64 result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		GSLOG(Warning, TEXT("[QuickSlotAutoUseStateAck - Error Result] ResultID : %d"), result);

		PD::Result resultEnum = static_cast<PD::Result>(pkt->Result());

		// U1: https://jira.com2us.com/jira/browse/C2URWQ-6177
		// - 빠르게 패킷을 보냈을 경우 오게될 에러코드(혹은 월드에 스폰되지 않거나 죽었는데 보냈을 때)
		// - 에러 팝업이 뜨지 않도록 로그만 찍음		 
		if (PD::Result::QUICK_SLOT_ERROR_INVALID_USER_STATUS == resultEnum)
		{
			return;
		}

		FGsUIHelper::PopupNetError(resultEnum);
		
		return;
	}

	TArray<QuickSlotId> invalidateSlotIdList;
	using quickslotIter = PktIterator<PD::SC::PKT_SC_ACK_QUICK_SLOT_AUTO_READ::QuickSlotList>;
	for (quickslotIter iter = pkt->GetFirstQuickSlotListIterator(); iter != pkt->GetLastQuickSlotListIterator(); ++iter)
	{
		QuickSlotId slotID = iter->QuickSlotId();
		ItemDBId itemDBID = iter->ItemDBId();
		bool isUsingAuto = iter->AutoUse();

#if WITH_EDITOR
		GSLOG(Warning, TEXT("FGsNetMessageHandlerItem::QuickSlotAutoUseStateAck - slotID : %d"), slotID);
		GSLOG(Warning, TEXT("FGsNetMessageHandlerItem::QuickSlotAutoUseStateAck - itemDBID : %llu"), itemDBID);
		GSLOG(Warning, TEXT("FGsNetMessageHandlerItem::QuickSlotAutoUseStateAck - isUsingAuto : %d"), (uint8)isUsingAuto);
#endif

		slotMgr->SetPacketData(slotID, itemDBID, isUsingAuto);
		invalidateSlotIdList.Emplace(slotID);
	}
	
	slotMgr->InvalidateAutoState(invalidateSlotIdList);

	FGsOnReceiveQuickSlotAutoState param;
	param._slotList.Append(invalidateSlotIdList);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::QUICKSLOT_AUTOSTATE_RECEIVE, &param);
}

void FGsNetMessageHandlerItem::AutoUseQuickSlotItemNotify(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("AutoUseQuickSlotItemNotify()"));

	UGsItemManager* itemMgr = GItem();
	PKT_SC_QUICK_SLOT_AUTO_USE_READ* pkt = reinterpret_cast<PKT_SC_QUICK_SLOT_AUTO_USE_READ*>(inPacket.Buffer);
	const ItemId UsedItemId = pkt->UseItemId();
	if (INVALID_ITEM_ID == UsedItemId)
	{
		GSLOG(Warning, TEXT("UsedItemId:%d"), UsedItemId);
	}
	else
	{
		itemMgr->UsedItemEffect(UsedItemId);
		itemMgr->SendItemEventUI(UsedItemId);
	}
}

void FGsNetMessageHandlerItem::NpcShopItemBuyAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("NpcShopItemBuyAck()"));

	PKT_SC_ACK_ITEM_BUY_READ* pkt = reinterpret_cast<PKT_SC_ACK_ITEM_BUY_READ*>(inPacket.Buffer);
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		if (result == PD::Result::ITEM_ERROR_ADD_INVENTORY_FULL) // 인벤토리 풀
		{
			// 가방이 가득찼습니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		if (result == PD::Result::ITEM_ERROR_ADD_WEIGHT_FULL) // 인벤토리 무게 풀
		{
			// 소지 무게가 최대치입니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	GSNpcShop()->AckNpcShopPacket(true);
}

void FGsNetMessageHandlerItem::NpcShopItemSellAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("NpcShopItemSellAck()"));

	PKT_SC_ACK_ITEM_SELL_READ* pkt = reinterpret_cast<PKT_SC_ACK_ITEM_SELL_READ*>(inPacket.Buffer);
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	GSNpcShop()->AckNpcShopPacket(false);
}

void FGsNetMessageHandlerItem::NpcEventShopItemBuyAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
#if WITH_EDITOR
	GSLOG(Warning, TEXT("NpcEventShopItemBuyAck()"));
#endif

	PKT_SC_ACK_NPC_EVENT_SHOP_PURCHASE_READ* pkt = reinterpret_cast<PKT_SC_ACK_NPC_EVENT_SHOP_PURCHASE_READ*>(inPacket.Buffer);
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		if (result == PD::Result::ITEM_ERROR_ADD_INVENTORY_FULL) // 인벤토리 풀
		{
			// 가방이 가득찼습니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		if (result == PD::Result::ITEM_ERROR_ADD_WEIGHT_FULL) // 인벤토리 무게 풀
		{
			// 소지 무게가 최대치입니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	// 구매 횟수 업데이트
	FGsUpdatePurchasedItem purchasedItemParam;
	pkt->MakePurchasedItemsVector<TArray<ItemIdAmountPair>, ItemIdAmountPair>(purchasedItemParam._purchasedItemList);
	GMessage()->GetNpcShop().SendMessage(MessageContentNpcShop::UPDATE_PURCHASE_AMOUNT, &purchasedItemParam);

	GSNpcShop()->AckNpcShopPacket(true);
}

void FGsNetMessageHandlerItem::DepositItemAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("DepositItemAck()"));

	///// 인벤토리에서의 액션 /////
	PKT_SC_ACK_MOVE_INVENTORY_ITEM_TO_DEPOT_READ* pkt = reinterpret_cast<PKT_SC_ACK_MOVE_INVENTORY_ITEM_TO_DEPOT_READ*>(inPacket.Buffer);
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	FGsDepotManager* depotMgr = GSDepot();
	depotMgr->AckDepotPacket();
}

void FGsNetMessageHandlerItem::TakeItemAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("TakeItemAck()"));

	///// 인벤토리에서의 액션 /////
	PKT_SC_ACK_MOVE_DEPOT_ITEM_TO_INVENTORY_READ* pkt = reinterpret_cast<PKT_SC_ACK_MOVE_DEPOT_ITEM_TO_INVENTORY_READ*>(inPacket.Buffer);
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		if (result == PD::Result::ITEM_ERROR_ADD_INVENTORY_FULL) // 인벤토리 풀
		{
			// 가방이 가득찼습니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		if (result == PD::Result::ITEM_ERROR_ADD_WEIGHT_FULL) // 인벤토리 무게 풀
		{
			// 소지 무게가 최대치입니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	FGsDepotManager* depotMgr = GSDepot();
	depotMgr->AckDepotPacket();

}

void FGsNetMessageHandlerItem::ExtendDepotSlotAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("ExtendDepotSlotAck()"));

	PKT_SC_ACK_DEPOT_EXTENDED_SLOT_READ* pkt = reinterpret_cast<PKT_SC_ACK_DEPOT_EXTENDED_SLOT_READ*>(inPacket.Buffer);
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	// 창고가 확장되었습니다.
	FText findText;
	FText::FindText(TEXT("DepotText"), TEXT("Notice_ExtendSlot"), findText);
	FGsUIHelper::TrayMessageTicker(findText);

	GSDepot()->SetDepotMaxSlotCount(pkt->TotalDepotSlotCount());
	GSDepot()->SetCurExtendedSlotNumber(pkt->ExtendedNumber());
	
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_EXPEND_SLOT, nullptr);
}

void FGsNetMessageHandlerItem::ExtendInvenSlotAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("ExtendInvenSlotAck()"));

	PKT_SC_ACK_INVENTORY_EXTENDED_SLOT_READ* pkt = reinterpret_cast<PKT_SC_ACK_INVENTORY_EXTENDED_SLOT_READ*>(inPacket.Buffer);
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	GSInventory()->OnChangeInvenSlotCount(pkt->TotalDepotSlotCount(), pkt->ExtendedNumber());
	GMessage()->GetInventoryEvent().SendMessage(MessageContentInven::UPDATE_INVEN_EXTEND_SCROLL, nullptr);
}

void FGsNetMessageHandlerItem::DepotMaxSlotCountNotify(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("DepotMaxSlotCountNotify()"));

	PKT_SC_DEPOT_SLOT_READ* pkt = reinterpret_cast<PKT_SC_DEPOT_SLOT_READ*>(inPacket.Buffer);
	if (true == pkt->Validate())
	{
		FGsDepotManager* depotMgr = GSDepot();
		depotMgr->SetDepotMaxSlotCount(pkt->MaxSlotCount());
		depotMgr->SetCurExtendedSlotNumber(pkt->ExtendedNumber());
	}
}

void FGsNetMessageHandlerItem::PickOneItemAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	GSLOG(Warning, TEXT("PickOneItemAck()"));
	PKT_SC_ACK_ITEM_SELECT_REWARD_READ* pkt = reinterpret_cast<PKT_SC_ACK_ITEM_SELECT_REWARD_READ*>(inPacket.Buffer);
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		if (result == PD::Result::ITEM_ERROR_ADD_INVENTORY_FULL) // 인벤토리 풀
		{
			// 가방이 가득찼습니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		if (result == PD::Result::ITEM_ERROR_ADD_WEIGHT_FULL) // 인벤토리 무게 풀
		{
			// 소지 무게가 최대치입니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	// 사운드 출력
	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		soundPlayer->PlaySound2D(TEXT("UI_PickupItem"));
	}

	// Ticker 출력 필요
	/*
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
	*/


}

void FGsNetMessageHandlerItem::SeveralPickOneItemAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	GSLOG(Warning, TEXT("SeveralPickOneItemAck()"));
	PKT_SC_ACK_ITEM_SELECT_REWARD_SEVERAL_READ* pkt = reinterpret_cast<PKT_SC_ACK_ITEM_SELECT_REWARD_SEVERAL_READ*>(inPacket.Buffer);
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		if (result == PD::Result::ITEM_ERROR_ADD_INVENTORY_FULL) // 인벤토리 풀
		{
			// 가방이 가득찼습니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		if (result == PD::Result::ITEM_ERROR_ADD_WEIGHT_FULL) // 인벤토리 무게 풀
		{
			// 소지 무게가 최대치입니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	// 사운드 출력
	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		soundPlayer->PlaySound2D(TEXT("UI_PickupItem"));
	}
}

void FGsNetMessageHandlerItem::NpcAutoSettingNotify(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("NpcAutoSettingNotify()"));

	FGsDepotManager* depotManager = GSDepot();
	if (nullptr == depotManager)
		return;

	UGsNpcShopManager* npcShopManager = GSNpcShop();
	if (nullptr == npcShopManager)
		return;

	PKT_SC_AUTO_PURCHASE_READ* pkt = reinterpret_cast<PKT_SC_AUTO_PURCHASE_READ*>(inPacket.Buffer);
	if (true == pkt->Validate())
	{
		// 자동 구매
		TArray<ItemIdAmountPair> autoBuyPairList;
		pkt->MakeAutoShopBuyVector<TArray<ItemIdAmountPair>, ItemIdAmountPair>(autoBuyPairList);
		npcShopManager->OnChangeAutoBuySettingData(autoBuyPairList);

		// 자동 판매 설정
		// 소모품류
		TArray<ItemId> autoSellConsumePairList;
		pkt->MakeAutoShopSellConsumeVector<TArray<ItemId>, ItemId>(autoSellConsumePairList);
		npcShopManager->OnChangeAutoSellConsumSettingData(autoSellConsumePairList);

		// 자동 판매 설정
		// 장착류
		TArray<AutoShopSellEquipPair> autoSellEquipmentList;
		pkt->MakeAutoShopSellEquipVector<TArray<AutoShopSellEquipPair>, AutoShopSellEquipPair>(autoSellEquipmentList);
		npcShopManager->OnChangeAutoSellEquipSettingData(autoSellEquipmentList);

		// 창고 자동 보관설정 - ItemId 목록
		TArray<ItemId> autoDepotIDList;
		pkt->MakeAutoDepotKeepVector<TArray<ItemId>, ItemId>(autoDepotIDList);
		depotManager->OnChangeAutoDepositSettingData(autoDepotIDList);

		// 창고 자동 보관설정 - (GroupId, ItemGrade) 목록
		TArray<AutoDepotConfigPair> autoDepotConfigList;
		pkt->MakeAutoDepotConfigPairListVector<TArray<AutoDepotConfigPair>, AutoDepotConfigPair>(autoDepotConfigList);
		depotManager->OnChangeAutoDepositGradeData(autoDepotConfigList);
	}
}

void FGsNetMessageHandlerItem::NpcAutoSettingAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("NpcAutoSettingAck()"));

	PKT_SC_ACK_AUTO_PURCHASE_UPDATE_READ* pkt = reinterpret_cast<PKT_SC_ACK_AUTO_PURCHASE_UPDATE_READ*>(inPacket.Buffer);
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	// 실제 변경된 컨텐츠만 update
	switch (pkt->UpdateAutoPurchaseType())
	{
	case AutoPurchaseType::AUTO_DEPOT_KEEP:
	{
		// 창고 자동 보관설정
		TArray<ItemId> autoDepotIDList;
		pkt->MakeAutoDepotKeepVector<TArray<ItemId>, ItemId>(autoDepotIDList);
		GSDepot()->OnChangeAutoDepositSettingData(autoDepotIDList);
	}
	break;
	case AutoPurchaseType::AUTO_DEPOT_KEEP_GROUP:
	{
		// 창고 자동 보관설정 - (GroupId, ItemGrade) 목록
		TArray<AutoDepotConfigPair> autoDepotConfigList;
		pkt->MakeAutoDepotConfigPairListVector<TArray<AutoDepotConfigPair>, AutoDepotConfigPair>(autoDepotConfigList);
		GSDepot()->OnChangeAutoDepositGradeData(autoDepotConfigList);
	}
	break;
	case AutoPurchaseType::AUTO_SHOP_BUY:
	{
		TArray<ItemIdAmountPair> autoBuyPairList;
		pkt->MakeAutoShopBuyVector<TArray<ItemIdAmountPair>, ItemIdAmountPair>(autoBuyPairList);
		GSNpcShop()->OnChangeAutoBuySettingData(autoBuyPairList);
	}
	break;
	case AutoPurchaseType::AUTO_SHOP_SELL_CONSUME:
	{
		// 자동 판매 설정
		// 소모품류
		TArray<ItemId> autoSellConsumePairList;
		pkt->MakeAutoShopSellConsumeVector<TArray<ItemId>, ItemId>(autoSellConsumePairList);
		GSNpcShop()->OnChangeAutoSellConsumSettingData(autoSellConsumePairList);
	}
	break;
	case AutoPurchaseType::AUTO_SHOP_SELL_EQUIP:
	{
		// 자동 판매 설정
		// 장착류
		TArray<AutoShopSellEquipPair> autoSellEquipmentList;
		pkt->MakeAutoShopSellEquipVector<TArray<AutoShopSellEquipPair>, AutoShopSellEquipPair>(autoSellEquipmentList);
		GSNpcShop()->OnChangeAutoSellEquipSettingData(autoSellEquipmentList);
	}
	break;

	}
}

void FGsNetMessageHandlerItem::AutoUsePotionStateAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("AutoUsePotionStateAck()"));

	PKT_SC_ACK_POTION_AUTO_USE_ON_OFF_READ* pkt = reinterpret_cast<PKT_SC_ACK_POTION_AUTO_USE_ON_OFF_READ*>(inPacket.Buffer);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_UPDATE_AUTO_POTION, nullptr);
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	if (FGsServerOption* serverOption = GServerOption())
	{
		serverOption->SetisAutoUsePotion(pkt->PotionAutoUseOnOff());
	}
	
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CONSUMEABLE);
}

void FGsNetMessageHandlerItem::ExpiredTimeNotify(const FGsNet::Packet& inPacket)
{
	GSLOG(Warning, TEXT("ExpiredTimeNotify()"));
	PKT_SC_INFORM_ITEM_EXPIRED_READ* pkt = reinterpret_cast<PKT_SC_INFORM_ITEM_EXPIRED_READ*>(inPacket.Buffer);
	TArray<ItemId> expiredItemList;
	pkt->MakeExpiredItemIdListVector<TArray<ItemId>, ItemId>(expiredItemList);
	GItem()->ExpiredDeleteItem(expiredItemList);
}

void FGsNetMessageHandlerItem::DisplayRewardNotify(const FGsNet::Packet& inPacket)
{
	PKT_SC_NOTIFY_DISPLAY_REWARD_READ* pkt = reinterpret_cast<PKT_SC_NOTIFY_DISPLAY_REWARD_READ*>(inPacket.Buffer);
	
	TArray<ItemDataCreated> itemDataCreatedList;
	pkt->MakeItemDeltaVector<TArray<ItemDataCreated>, ItemDataCreated>(itemDataCreatedList);

	TArray<CurrencyPair> currencyPairList;
	pkt->MakeCurrencyDeltaVector<TArray<CurrencyPair>, CurrencyPair>(currencyPairList);

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupItemReceive"));
	if (widget.IsValid())
	{
		if (UGsUIPopupItemReceive* popup = Cast<UGsUIPopupItemReceive>(widget.Get()))
		{			
			popup->SetData(itemDataCreatedList, currencyPairList);
		}
	}
}

void FGsNetMessageHandlerItem::InvenSortValueAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("InvenSortValueAck()"));

	PKT_SC_ACK_INVENTORY_CUSTOM_SORT_READ* pkt = reinterpret_cast<PKT_SC_ACK_INVENTORY_CUSTOM_SORT_READ*>(inPacket.Buffer);
	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	TArray<ItemDbIdSortPair> itemSortPairList;
	const int size = pkt->ItemDbIdSortPairListSize();
	if (0 < size)
	{
		itemSortPairList.Reset(size);
		pkt->MakeItemDbIdSortPairListVector<TArray<ItemDbIdSortPair>, ItemDbIdSortPair>(itemSortPairList);
		GItem()->UpdateInvenSlotValue(itemSortPairList);
	}
}

void FGsNetMessageHandlerItem::DeliveryBoxRecoverEquipAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();

	PKT_SC_ACK_DELIVERY_BOX_RECOVER_EQUIP_READ* pkt = reinterpret_cast<PKT_SC_ACK_DELIVERY_BOX_RECOVER_EQUIP_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
		return;

	int result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("WindowRepair"));
		if (widget.IsValid())
		{
			widget->Close();
		}
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(result));
		return;
	}

	ItemDataCreated itemData = pkt->ItemDatasCreated();
	GItem()->GetItemDeliveryBoxData().ShowRepairResult(itemData);
}

void FGsNetMessageHandlerItem::ItemUseDeliveryBoxEquipNotify(const FGsNet::Packet& inPacket)
{
	PKT_SC_ITEM_USE_DELIVERY_BOX_EQUIP_READ* pkt = reinterpret_cast<PKT_SC_ITEM_USE_DELIVERY_BOX_EQUIP_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
		return;

	TArray<ItemDataCreated> itemDataCreatedList;
	pkt->MakeItemDatasCreatedVector<TArray<ItemDataCreated>, ItemDataCreated>(itemDataCreatedList);

	if (itemDataCreatedList.Num() <= 0)
	{
		FText findText;
		FText::FindText(TEXT("DeliveryBoxText"), TEXT("DeliveryBoxText_Ticker_Equip_Empty_Reward"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
	}
	else
	{
		FGsItemDeliveryBoxData& deliveryBoxData = GItem()->GetItemDeliveryBoxData();
		deliveryBoxData.SetEquipData(itemDataCreatedList);
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_REPAIR);
	}
}

void FGsNetMessageHandlerItem::ItemUseDeliveryBoxFairyComposeNotify(const FGsNet::Packet& inPacket)
{
	PKT_SC_ITEM_USE_DELIVERY_BOX_FAIRY_COMPOSE_READ* pkt = reinterpret_cast<PKT_SC_ITEM_USE_DELIVERY_BOX_FAIRY_COMPOSE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
		return;

	TArray<FairyGrade> fairyGradeList;
	for (auto iter = pkt->GetFirstFairyGradeListIterator(); iter != pkt->GetLastFairyGradeListIterator(); ++iter)
	{
		fairyGradeList.Emplace(iter->FairyGrade());
	}

	if (fairyGradeList.Num() <= 0)
	{
		FText findText;
		FText::FindText(TEXT("DeliveryBoxText"), TEXT("DeliveryBoxText_Ticker_Fairy_Empty_Reward"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
	}
	else
	{
		FGsItemDeliveryBoxData& deliveryBoxData = GItem()->GetItemDeliveryBoxData();
		deliveryBoxData.SetFairyData(fairyGradeList);
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_RECOMPOSE);
	}
}

void FGsNetMessageHandlerItem::ItemUseDeliveryBoxCostumeComposeNotify(const FGsNet::Packet& inPacket)
{
	PKT_SC_ITEM_USE_DELIVERY_BOX_COSTUME_COMPOSE_READ* pkt = reinterpret_cast<PKT_SC_ITEM_USE_DELIVERY_BOX_COSTUME_COMPOSE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
		return;

	TArray<CostumeGrade> costumeGradeList;
	for (auto iter = pkt->GetFirstCostumeGradeListIterator(); iter != pkt->GetLastCostumeGradeListIterator(); ++iter)
	{
		costumeGradeList.Emplace(iter->CostumeGrade());
	}

	if (costumeGradeList.Num() <= 0)
	{
		FText findText;
		FText::FindText(TEXT("DeliveryBoxText"), TEXT("DeliveryBoxText_Ticker_Costume_Empty_Reward"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
	}
	else
	{
		FGsItemDeliveryBoxData& deliveryBoxData = GItem()->GetItemDeliveryBoxData();
		deliveryBoxData.SetCostumeData(costumeGradeList);
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_RECOMPOSE);
	}
}

void FGsNetMessageHandlerItem::CharacterCustomizingTicketUseAck(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	PKT_SC_CHARACTER_CUSTOMIZING_TICKET_USE_READ* pkt = reinterpret_cast<PKT_SC_CHARACTER_CUSTOMIZING_TICKET_USE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
		return;

	int32 result = pkt->CustomizingTicketCount();
	FText findText;
	if (true == FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_UseCustomizingTicket"), findText))
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText, TrayCondition::TC_Close);
	}
}

void FGsNetMessageHandlerItem::AckItemFusionResult(const FGsNet::Packet& inPacket)
{
	FGsUIHelper::HideBlockUI();
	PKT_SC_ACK_ITEM_FUSE_READ* pkt = reinterpret_cast<PKT_SC_ACK_ITEM_FUSE_READ*>(inPacket.Buffer);
	if (nullptr == pkt)
		return;

	int32 result = pkt->Result();
	if (result != PACKET_RESULT_SUCCESS)
	{
		FGsUIHelper::PopupNetError((PD::Result)pkt->Result());
		GSLOG(Warning, TEXT("[AckItemFusionResult - Error Result] ResultID : %d"), result);
		
		// 2023/10/11 PKT - 실패했다고 여기서 리턴 시키면 안됨. 끝까지 가서 Data 정리를 해줘야 함.
	}

	if (GMessage())
	{
		FGsPrimitivePairUInt64 param = FGsPrimitivePairUInt64(pkt->Result(), pkt->ResultItemDBId());
		GMessage()->GetItemFusion().SendMessage(MessageContentsFusion::RECV_FUSTION_RESULT, &param);
	}
}