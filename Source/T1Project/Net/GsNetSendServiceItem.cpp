// Fill out your copyright notice in the Description page of Project Settings.

#include "GsNetSendServiceItem.h"
#include "Management/ScopeGame/GsQuickSlotManager.h"
#include "Util/GsItemUtil.h"
#include "Item/GsItemManager.h"
#include "T1Project.h"
#include "UTIL/GsGameObjectUtil.h"
#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Client_Gateway.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "UI/UIContent/Helper/GsUIHelper.h"


// 단일 사용
void FGsNetSendServiceItem::SendUseItem(const uint64 InItemDBID, const ItemCategorySub InSubCategory)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ITEM_USE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InItemDBID);

			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();
			/*
			if (ItemCategorySub::HP_POTION == InSubCategory || ItemCategorySub::HP_POTION_MANUAL == InSubCategory)
			{
				FGsUIHelper::ShowBlockUI();
			}
			else
			{
				FText findText;
				FText::FindText(TEXT("UICommonText"), TEXT("Requesting"), findText);
				FGsUIHelper::ShowBlockUI();
			}
			*/
		}
	}
}

// 일괄 사용(리워드 형태의 박스형 소모품 아이템만 사용 가능함)
// ItemData에서 CategorySub가 RewardBox 인 아이템
void FGsNetSendServiceItem::SendSeveralUseItem(const ItemDBIdAmountPair InData)
{
	if (0 >= InData.mItemDBId || 0 >= InData.mItemAmount)
		return;

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ITEM_USE_SEVERAL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), 
				InData.mItemDBId, InData.mItemAmount);

			netBase->Send(builder.mPktWriter.GetPktSize());

			FText findText;
			FText::FindText(TEXT("UICommonText"), TEXT("Requesting"), findText);
			FGsUIHelper::ShowBlockUI();
		}
	}
}

// 장착 아이템 착용할때 보내는 Packet(장비 Swap 되는것도 이 패킷에서 처리함)
void FGsNetSendServiceItem::SendAttachEquipItem(const uint8 InPresetID, const uint64 InEquipItemDBID, const uint8 InEquipItemSlotIndex, const uint64 InUnEquipItemDBID, const uint8 InUnEquipItemSlotIndex)
{
	TArray<ItemDataEquipRequestInfo> equipItemInfo;
	TArray<ItemDataEquipRequestInfo> unEquipItemInfo;

	ItemDataEquipRequestInfo info = { 0, 0 };
	info.mItemDBId = InEquipItemDBID;
	info.mItemEquipSlotIndex = InEquipItemSlotIndex;
	equipItemInfo.Emplace(info);


	if (0 < InUnEquipItemDBID)
	{
		info.mItemDBId = InUnEquipItemDBID;
		info.mItemEquipSlotIndex = InUnEquipItemSlotIndex;
		unEquipItemInfo.Emplace(info);
	}

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ITEM_EQUIP_WRITE
				builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InPresetID, &equipItemInfo, &unEquipItemInfo);

			/*
			GSLOG(Warning, TEXT("InPresetID : %d"), InPresetID);
			if (0 < equipItemInfo.Num())
			{
				GSLOG(Warning, TEXT("equipItemInfo[0].mItemDBId : %llu"), equipItemInfo[0].mItemDBId);
				GSLOG(Warning, TEXT("equipItemInfo[0].mItemEquipSlotIndex : %llu"), equipItemInfo[0].mItemEquipSlotIndex);
			}
			if (0 < unEquipItemInfo.Num())
			{
				GSLOG(Warning, TEXT("unEquipItemInfo[0].mItemDBId : %llu"), unEquipItemInfo[0].mItemDBId);
				GSLOG(Warning, TEXT("unEquipItemInfo[0].mItemEquipSlotIndex : %llu"), unEquipItemInfo[0].mItemEquipSlotIndex);
			}
			*/

			netBase->Send(builder.mPktWriter.GetPktSize());
			GSLOG(Warning, TEXT("SendAttachEquipItem()"));

			//FGsUIHelper::ShowBlockUI();

			GItem()->SetIsRecvSameEquipTypeProcPacket(false);
		}
	}
}

// 아이템 장착 혹은 해지 패킷 모두 동일한 패킷을 사용한다함
// 더 명확하게 하기위해 똑같은 코드지만 함수명을 달리하여 사용함
// 이 함수는 장비를 교체하는게 아닌, 순수 Detach 용으로만 사용하는 Packet
void FGsNetSendServiceItem::SendDetacheEquipItem(const uint8 InPresetID, const uint64 InUnEquipItemDBID, const uint8 InUnEquipSlotIndex)
{
	TArray<ItemDataEquipRequestInfo> equipItemInfo;
	TArray<ItemDataEquipRequestInfo> unEquipItemInfo;

	ItemDataEquipRequestInfo info = { 0, 0 };
	info.mItemDBId = InUnEquipItemDBID;
	info.mItemEquipSlotIndex = InUnEquipSlotIndex;
	unEquipItemInfo.Emplace(info);

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ITEM_EQUIP_WRITE
				builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InPresetID, &equipItemInfo, &unEquipItemInfo);

			//GSLOG(Warning, TEXT("InPresetID : %d"), InPresetID); 
			//GSLOG(Warning, TEXT("equipItemInfo[0].mItemDBId : %llu"), equipItemInfo[0].mItemDBId);
			//GSLOG(Warning, TEXT("equipItemInfo[0].mItemEquipSlotIndex : %llu"), equipItemInfo[0].mItemEquipSlotIndexJ);
			//GSLOG(Warning, TEXT("unEquipItemInfo[0].mItemDBId : %llu"), unEquipItemInfo[0].mItemDBId);
			//GSLOG(Warning, TEXT("unEquipItemInfo[0].mItemEquipSlotIndex : %llu"), unEquipItemInfo[0].mItemEquipSlotIndexJ);

			netBase->Send(builder.mPktWriter.GetPktSize());
			GSLOG(Warning, TEXT("SendDetacheEquipItem()"));

			//FGsUIHelper::ShowBlockUI();

			GItem()->SetIsRecvSameEquipTypeProcPacket(false);
		}
	}
}

void FGsNetSendServiceItem::SendUseCampConsumItem(const uint64 InItemDBID, const CampPosition InCampPosInfo)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_SPAWN_CAMP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InItemDBID, InCampPosInfo);
			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();
		}
	}
}

void FGsNetSendServiceItem::SendItemEquipChangeWeapon(ItemDBId InItemId, float InDuration)
{
	GItem()->SetIsRecvDiffEquipTypeProcPacket(false);

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ITEM_EQUIP_CHANGE_WEAPON_TYPE_WRITE builder(sendBuffer->GetBuffer(),
				sendBuffer->GetSize(), InItemId, InDuration);
			netBase->Send(builder.mPktWriter.GetPktSize());

			GSLOG(Warning, TEXT("SendItemEquipChangeWeapon()"));
#if WITH_EDITOR
			GSLOG(Warning, TEXT("SendItemEquipChangeWeapon() DBID : %llu"), InItemId);
			GSLOG(Warning, TEXT("SendItemEquipChangeWeapon() totalRuntime : %f"), InDuration);
#endif
		}
	}
}

void FGsNetSendServiceItem::SendItemLockState(const uint64 InItemDBID, const bool InLock, const ItemStorageType InStorageType)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ITEM_LOCK_WRITE builder(sendBuffer->GetBuffer(),sendBuffer->GetSize() , InItemDBID , InLock, InStorageType);
			netBase->Send(builder.mPktWriter.GetPktSize());

			//GSLOG(Warning, TEXT("SendItemLockState()"));

			FGsUIHelper::ShowBlockUI();
		}
	}
}

void FGsNetSendServiceItem::SendDeleteItem(const uint64 InItemDBID, const ItemAmount InItemCount)
{
	if (0 >= InItemCount)
		return;

	TArray<ItemDBIdAmountPair> itemDelete;
	ItemDBIdAmountPair itemDBIdCountPair(InItemDBID, InItemCount);
	itemDelete.Add(MoveTemp(itemDBIdCountPair));

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ITEM_DELETE_WRITE
				builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), &itemDelete);

			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();
		}
	}
}

void FGsNetSendServiceItem::SendEnchantItem(const uint64 InUseConsumItemDBID, const ItemAmount InAmountConsumItem, const uint64 InTargetEquipItemDBID)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ITEM_ENCHANT_WRITE builder(sendBuffer->GetBuffer(),
				sendBuffer->GetSize(), InUseConsumItemDBID, InAmountConsumItem, InTargetEquipItemDBID);
			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendEnchantItem()"));
		}
	}
}

void FGsNetSendServiceItem::SendMultiEnchantItem(TArray<ItemDBIdAmountPair> InIngredientItemList, TArray<ItemDBId> InEquipItemDbidList, Level InNextLevel)
{
	if (0 >= InIngredientItemList.Num() || 0 >= InEquipItemDbidList.Num())
		return;

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ITEM_ENCHANT_MULTI_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), 
				&InIngredientItemList, &InEquipItemDbidList, InNextLevel);
			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendEnchantItem()"));
		}
	}
}

void FGsNetSendServiceItem::SendRefineOptionItem(const ItemDBId InTargetItemDBID, const ItemDBId InOptionStoneItemDBID)
{
	if (0 >= InTargetItemDBID || 0 >= InOptionStoneItemDBID)
		return;

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ITEM_REFINE_WRITE builder(sendBuffer->GetBuffer(),
				sendBuffer->GetSize(), InOptionStoneItemDBID, InTargetItemDBID);
			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendRefineOptionItem()"));
		}
	}
}

void FGsNetSendServiceItem::SendMagicalForgeItem(const ItemDBId InTargetItemDBId, const ItemDBId InIngredientItemDBId)
{
	if (0 >= InTargetItemDBId || 0 >= InIngredientItemDBId)
		return;

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ITEM_MAGICAL_FORGE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InTargetItemDBId, InIngredientItemDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendMagicalForgeItem()"));
		}
	}
}

void FGsNetSendServiceItem::SendItemDecompose(const TArray<ItemDBIdAmountPair> InDecomposeTargetItemList)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ITEM_DECOMPOSE_WRITE builder(sendBuffer->GetBuffer(),
				sendBuffer->GetSize(), &InDecomposeTargetItemList);

			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();
		}
	}
}

// 아이템 등록 및 해지할때 사용할 패킷
// InItemDBID 및 InItemTID 를 0으로 보내면 해제 요청 
void FGsNetSendServiceItem::SendItemRegistOrRemoveQuickSlot(const QuickSlotId InSlotId , const uint64 InItemDBID , const uint32 InItemTID)
{
	if (0 >= InSlotId)
		return;

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUICK_SLOT_SET_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());
			builder.CreateQuickSlotSetList(InSlotId, InItemDBID, InItemTID);

			netBase->Send(builder.mPktWriter.GetPktSize());
			netMgr->GetCoolTime().AddCoolTime(static_cast<uint16>(PD::CS::CSPacketId::CS_REQ_QUICK_SLOT_SET));

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendModifyQuickSlot()"));
		}
	}
}

// 그룹형식으로 한번에 여러개의 등록 / 해지 패킷 요청할때
void FGsNetSendServiceItem::SendQuickSlotRegistItemGroup(TMap<QuickSlotId, TPair<uint64, uint32>>& InSwapPairDataList)
{
	if (0 >= InSwapPairDataList.Num())
		return;

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUICK_SLOT_SET_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize());

			for (const auto& iter : InSwapPairDataList)
			{
				QuickSlotId slotID = iter.Key;
				TPair<uint64, uint32> valueData = iter.Value;
				builder.CreateQuickSlotSetList(slotID, valueData.Key , valueData.Value);
			}

			netBase->Send(builder.mPktWriter.GetPktSize());
			netMgr->GetCoolTime().AddCoolTime(static_cast<uint16>(PD::CS::CSPacketId::CS_REQ_QUICK_SLOT_SET));

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendQuickSlotRegistItemGroup()"));
		}
	}
}

void FGsNetSendServiceItem::SendAutoUseStateQuickSlot(const QuickSlotId InSlotID, const uint64 InItemDBID, const bool InFlag)
{
	if (0 >= InSlotID)
		return;

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_QUICK_SLOT_AUTO_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(),
				InSlotID, InItemDBID, InFlag);

			netBase->Send(builder.mPktWriter.GetPktSize());

			GSLOG(Warning, TEXT("SendAutoUseStateQuickSlot()"));
		}
	}
}


/*
// NPCSHOP 아이템 구매정보
// 구매 ShopGroupID , 아이템 ID, 갯수
struct NpcShopItemBuyPair

	NpcShopGroupId	mnpcShopGroupId = INVALID_NPCSHOPGROUP_ID;
	ItemId			mItemId = INVALID_ITEM_DB_ID;
	ItemAmount		mItemAmount = 0;

CreatureId == npcData.json 에 있는 ID
*/

void FGsNetSendServiceItem::SendBuyNpcShopItem(CreatureId InCreatureID, TArray<NpcShopItemBuyPair>& InBuyItemList)
{
	if (0 >= InBuyItemList.Num())
		return;

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			// 나의 위치, 방향정보
			FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
			int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ITEM_BUY_WRITE
				builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InCreatureID, &InBuyItemList, location, dir);

			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendBuyNpcShopItem()"));
		}
	}
}

/*
// 아이템 DB ID와 갯수
struct ItemDBIdAmountPair
{
	ItemDBId	mItemDBId = INVALID_ITEM_DB_ID;
	ItemAmount	mItemAmount = 0;


public:
	ItemDBIdAmountPair() = default;
	explicit ItemDBIdAmountPair(ItemDBId dbId, ItemAmount amount)
		:
		mItemDBId(dbId),
		mItemAmount(amount)
	{
	}
};
*/
void FGsNetSendServiceItem::SendSellNpcShopItem(CreatureId InCreatureID, TArray<ItemDBIdAmountPair>& InSellItemList)
{
	if (0 >= InSellItemList.Num())
		return;

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			// 나의 위치, 방향정보
			FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
			int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ITEM_SELL_WRITE
				builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InCreatureID, &InSellItemList, location, dir);

			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendSellNpcShopItem()"));
		}
	}
}

void FGsNetSendServiceItem::SendBuyNpcEventShopItem(int64 InGameID, TArray<NpcEventShopItemBuyPair>& InBuyItemList)
{
	if (0 >= InBuyItemList.Num())
		return;

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			// 나의 위치, 방향정보
			FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
			int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_NPC_EVENT_SHOP_PURCHASE_WRITE
				builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InGameID, &InBuyItemList, location, dir);

			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendBuyNpcEventShopItem()"));
		}
	}
}

void FGsNetSendServiceItem::SendDepositItems(CreatureId InCreatureID, TArray<ItemDBIdAmountPair>& InItemList)
{
	if (0 >= InItemList.Num())
		return;

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			// 나의 위치, 방향정보
			FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
			int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_MOVE_INVENTORY_ITEM_TO_DEPOT_WRITE
				builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InCreatureID, &InItemList, location, dir);

			netBase->Send(builder.mPktWriter.GetPktSize());

#if WITH_EDITOR
			GSLOG(Warning, TEXT("SendDepositItems() - InCreatureID : %d"), InCreatureID);
#endif

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendDepositItems()"));
		}
	}
}

void FGsNetSendServiceItem::SendTakeItems(CreatureId InCreatureID, TArray<ItemDBIdAmountPair>& InItemList)
{
	if (0 >= InItemList.Num())
		return;

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			// 나의 위치, 방향정보
			FVector location = UGsGameObjectUtil::GetLocalPlayerLocation();
			int16 dir = UGsGameObjectUtil::GetLocalPlayerDirToAngle();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_MOVE_DEPOT_ITEM_TO_INVENTORY_WRITE
				builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InCreatureID, &InItemList, location, dir);

			netBase->Send(builder.mPktWriter.GetPktSize());

#if WITH_EDITOR
			GSLOG(Warning, TEXT("SendTakeItems() - InCreatureID : %d"), InCreatureID);
#endif

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendTakeItems()"));
		}
	}
}

void FGsNetSendServiceItem::SendExtendDepotSlot(uint8 InExtendLevel)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_DEPOT_EXTEND_SLOT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InExtendLevel);
			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendExtendDepotSlot()"));
		}
	}
}

void FGsNetSendServiceItem::SendPickOneItem(const uint64 InItemDBID, const uint8 Index)
{
	if (0 >= InItemDBID)
	{
		return;
	}

	//GSLOG(Warning, TEXT("SendPickOneItem() InItemDBID : %llu"), InItemDBID);
	//GSLOG(Warning, TEXT("SendPickOneItem() Index : %d"), Index);

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ITEM_SELECT_REWARD_WRITE builder(sendBuffer->GetBuffer(), 
				sendBuffer->GetSize(), InItemDBID, Index);
			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendPickOneItem()"));
		}
	}
}

void FGsNetSendServiceItem::SendSeveralPickOneItem(const uint64 InItemDBID, const ItemAmount InAmount, const uint8 Index)
{
	if (0 >= InItemDBID || 0 >= InAmount)
	{
		return;
	}

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ITEM_SELECT_REWARD_SEVERAL_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), 
				InItemDBID, Index, InAmount);
			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();
		}
	}
}

void FGsNetSendServiceItem::SendExtendInvenSlot(uint8 InExtendLevel)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_INVENTORY_EXTEND_SLOT_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InExtendLevel);
			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendExtendInvenSlot()"));
		}
	}
}

void FGsNetSendServiceItem::SendOnChangeAutoDeposit(TArray<ItemId> InDataList)
{
	TArray<ItemIdAmountPair> autoBuyList;
	TArray<ItemId> autoSellConsumeList;
	TArray<AutoShopSellEquipPair> autoSellEquipmentList;
	TArray<AutoDepotConfigPair> autoDepotConfigList;

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_AUTO_PURCHASE_UPDATE_WRITE builder(sendBuffer->GetBuffer(),
					sendBuffer->GetSize(), AutoPurchaseType::AUTO_DEPOT_KEEP, &autoBuyList, &autoSellConsumeList, &autoSellEquipmentList, &InDataList, &autoDepotConfigList);

			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendOnChangeAutoDeposit()"));
		}
	}
}

void FGsNetSendServiceItem::SendOnChangeAutoDepositGrade(const TArray<AutoDepotConfigPair>& InDataList)
{
	TArray<ItemIdAmountPair> autoBuyList;
	TArray<ItemId> autoSellConsumeList;
	TArray<AutoShopSellEquipPair> autoSellEquipmentList;
	TArray<ItemId> autoDespositList;

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_AUTO_PURCHASE_UPDATE_WRITE builder(sendBuffer->GetBuffer(),
				sendBuffer->GetSize(), AutoPurchaseType::AUTO_DEPOT_KEEP_GROUP, &autoBuyList, &autoSellConsumeList, &autoSellEquipmentList, &autoDespositList, &InDataList);

			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendOnChangeAutoDeposit()"));
		}
	}
}

void FGsNetSendServiceItem::SendOnChangeAutoBuy(TArray<ItemIdAmountPair> InDataList)
{
	TArray<ItemId> autoSellConsumeList;
	TArray<AutoShopSellEquipPair> autoSellEquipmentList;
	TArray<ItemId> autoDespositList;
	TArray<AutoDepotConfigPair> autoDepotConfigList;

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_AUTO_PURCHASE_UPDATE_WRITE builder(sendBuffer->GetBuffer(),
					sendBuffer->GetSize(), AutoPurchaseType::AUTO_SHOP_BUY, &InDataList, &autoSellConsumeList, &autoSellEquipmentList, &autoDespositList, &autoDepotConfigList);

			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendChangeAutoBuySetting()"));
		}
	}
}

void FGsNetSendServiceItem::SendOnChangeAutoSellConsumable(TArray<ItemId> InDataList)
{
	TArray<ItemIdAmountPair> autoBuyList;
	TArray<AutoShopSellEquipPair> autoSellEquipmentList;
	TArray<ItemId> autoDespositList;
	TArray<AutoDepotConfigPair> autoDepotConfigList;

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_AUTO_PURCHASE_UPDATE_WRITE builder(sendBuffer->GetBuffer(),
					sendBuffer->GetSize(), AutoPurchaseType::AUTO_SHOP_SELL_CONSUME, &autoBuyList, &InDataList, &autoSellEquipmentList, &autoDespositList, &autoDepotConfigList);

			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendOnChangeAutoSellConsumable()"));
		}
	}
}

void FGsNetSendServiceItem::SendOnChangeAutoSellEquipment(TArray<AutoShopSellEquipPair> InDataList)
{
	TArray<ItemIdAmountPair> autoBuyList;
	TArray<ItemId> autoSellConsumeList;
	TArray<AutoShopSellEquipPair> autoSellEquipmentList;
	TArray<ItemId> autoDespositList;
	TArray<AutoDepotConfigPair> autoDepotConfigList;

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_AUTO_PURCHASE_UPDATE_WRITE builder(sendBuffer->GetBuffer(),
					sendBuffer->GetSize(), AutoPurchaseType::AUTO_SHOP_SELL_EQUIP, &autoBuyList, &autoSellConsumeList, &InDataList, &autoDespositList, &autoDepotConfigList);

			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();

			GSLOG(Warning, TEXT("SendOnChangeAutoSellEquipment()"));
		}
	}
}

void FGsNetSendServiceItem::SendUseAutoPotionState(const bool InIsActive)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_POTION_AUTO_USE_ON_OFF_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InIsActive);
			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();
		}
	}
}

void FGsNetSendServiceItem::SendInventorySortValue(TArray<uint64> InSortItemList)
{
	if (0 >= InSortItemList.Num())
		return;

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_INVENTORY_CUSTOM_SORT_WRITE
				builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), &InSortItemList);

			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();
		}
	}
}

void FGsNetSendServiceItem::SendItemUseDeliveryBoxEquip(ItemDBId InBoxItemDBId, ItemDBId InRecoverItemDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_DELIVERY_BOX_RECOVER_EQUIP_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InBoxItemDBId, InRecoverItemDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();
		}
	}
}

void FGsNetSendServiceItem::SendItemUseDeliveryBoxFairyCompose(ItemDBId InBoxItemDBId, FairyGrade InFairyGrade)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_DELIVERY_BOX_RETRY_FAIRY_COMPOSE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InBoxItemDBId, InFairyGrade);
			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();
		}
	}
}

void FGsNetSendServiceItem::SendItemUseDeliveryBoxCostumeCompose(ItemDBId InBoxItemDBId, CostumeGrade InCostumeGrade)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_DELIVERY_BOX_RETRY_COSTUME_COMPOSE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InBoxItemDBId, InCostumeGrade);
			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();
		}
	}
}

void FGsNetSendServiceItem::SendTryItemFusion(ItemDBId InSrcItemDBId, ItemDBId InDestItemDBId, ItemDBId InRefinedItemDBId)
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CS::PKT_CS_REQ_ITEM_FUSE_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InSrcItemDBId, InDestItemDBId, InRefinedItemDBId);
			netBase->Send(builder.mPktWriter.GetPktSize());

			FGsUIHelper::ShowBlockUI();
		}
	}
}