// Fill out your copyright notice in the Description page of Project Settings.


#include "GsItemUtil.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Item/GsSchemaItemCommon.h"
#include "GsTableUtil.h"
#include "Item/GsItemManager.h"
#include "Guild/GsGuildStorageData.h"
#include "Management/ScopeGame/GsDepotManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "GameObject/Define/EGsDropItemCategory.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "DataSchema/Drop/GsSchemaDropData.h"
#include "DataSchema/Drop/GsSchemaDropItemGroupInfo.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"

EGsDropItemCategory UGsItemUtil::GetDropObjectCategory(int32 inItemId)
{
	const FGsSchemaItemCommon* item = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(inItemId);

	if (nullptr == item)
	{
		return EGsDropItemCategory::Max;
	}	

	switch (item->categoryMain)
	{
	case ItemCategoryMain::WEAPON:
		return EGsDropItemCategory::Weapon;
		break;
	case ItemCategoryMain::ARMOR:
		return EGsDropItemCategory::Armor;
		break;
	//accessories
	case ItemCategoryMain::ACCESSORY:
		return EGsDropItemCategory::Accessory;
		break;
	default:
		return EGsDropItemCategory::Consumable;		
		break;
	}

	return EGsDropItemCategory::Max;
}

bool UGsItemUtil::IsSendBlockItem(ItemCategorySub inItemCategory)
{
	bool sendblock = false;
	if (inItemCategory == ItemCategorySub::WARP_TO_TOWN_SCROLL)
	{
		sendblock = true;
	}
	return sendblock;
}

FText UGsItemUtil::GetItemName(int32 inItemName)
{
	const FGsSchemaItemCommon* item = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(inItemName);
	if (nullptr == item)
	{
		return FText::GetEmpty();
	}

	return item->name;
}

const FGsItem* UGsItemUtil::FindItemUtil(uint64 InItemDbid)
{
	const ItemStorageType type = FindItemStorageType(InItemDbid);
	const FGsItem* findItemData = FindItemByStorageType(InItemDbid, type);
	return findItemData;
}

const FGsItem* UGsItemUtil::FindItemByStorageType(uint64 InItemDbid, ItemStorageType InStorageType)
{
	if (0 >= InItemDbid || nullptr == GItem() || nullptr == GSDepot() || nullptr == GGuild())
	{
		return nullptr;
	}

	switch (InStorageType)
	{
	case ItemStorageType::INVENTORY:
	{
		TWeakPtr<FGsItem> findItem = GItem()->FindItem(InItemDbid);
		if (false == findItem.IsValid())
			return nullptr;

		return findItem.Pin().Get();
	}
	case ItemStorageType::DEPOT:
	{
		TWeakPtr<FGsItem> findDepotItem = GSDepot()->FindDepotItemByDBID(InItemDbid);
		if (false == findDepotItem.IsValid())
			return nullptr;

		return findDepotItem.Pin().Get();
	}
	case ItemStorageType::DEATH:
		break;
	case ItemStorageType::MARKET:
		break;
	case ItemStorageType::GUILD_STORE:
	{
		FGsGuildStorageData* data = GGuild()->GetMyGuildStorageData();
		if (nullptr != data)
		{
			return data->GetItemData(InItemDbid);
		}
		break;
	}
	case ItemStorageType::MAIL:
		break;
	case ItemStorageType::MAX:
		break;
	default:
		break;
	}

	return nullptr;
}

const ItemStorageType UGsItemUtil::FindItemStorageType(uint64 InItemDbid)
{
	if (0 >= InItemDbid || nullptr == GItem() || nullptr == GSDepot() || nullptr == GGuild())
	{
		return ItemStorageType::MAX;
	}
	
	TWeakPtr<FGsItem> findItem = GItem()->FindItem(InItemDbid);
	if (true == findItem.IsValid())
	{
		return ItemStorageType::INVENTORY;
	}

	TWeakPtr<FGsItem> findDepotItem = GSDepot()->FindDepotItemByDBID(InItemDbid);
	if (true == findDepotItem.IsValid())
	{
		return ItemStorageType::DEPOT;
	}
	if (FGsGuildStorageData* data = GGuild()->GetMyGuildStorageData())
	{
		if (nullptr != data->GetItemData(InItemDbid))
		{
			return ItemStorageType::GUILD_STORE;
		}
	}

	return ItemStorageType::MAX;
}

bool UGsItemUtil::TryGetServerDropItem(CreatureId inNpcId, OUT TArray<const FGsSchemaItemCommon*>& outEquipItemArray, OUT TArray<const FGsSchemaItemCommon*>& outEtcItemArray)
{		
	TArray<const FGsSchemaDropData*> dropDataArray;
	if (false == TryGetServerDropData(inNpcId, dropDataArray))
	{
		return false;
	}

	return TryConvertDropToItemData(dropDataArray, outEquipItemArray, outEtcItemArray);	
}

bool UGsItemUtil::TryGetServerDropData(CreatureId inNpcId, OUT TArray<const FGsSchemaDropData*>& outDropDataArray)
{
	//check server id
	uint16 worldId = GNet()->GetPlanetWorldId();
	FString serverId = FString::FromInt(worldId);
	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(inNpcId);
	if(nullptr == npcData)
	{
		return false;
	}

	//find server data
	const UGsTableDropData* dropDataTable = Cast<const UGsTableDropData>(FGsSchemaDropData::GetStaticTable());
	if (nullptr == dropDataTable)
	{
		return false;
	}

	TArray<const FGsSchemaDropData*> dropDataArray;
	dropDataTable->GetAllRows(dropDataArray);
	int32 level = npcData->fixedLevel;
	
	TArray<const FGsSchemaDropData*> matchedDropDataArray;
	for (const FGsSchemaDropData* dropData : dropDataArray)
	{
		if (dropData->dropType == DropCheckType::SERVER
		&& level == dropData->level)
		{		
			if (dropData->dropValue.Equals(serverId))
			{
				matchedDropDataArray.Add(dropData);
			}
		}
	}

	return outDropDataArray.Num() > 0;
}

bool UGsItemUtil::TryGetInvadeDropItem(CreatureId inNpcId, InvadeDropType inInvadeType, OUT TArray<const struct FGsSchemaItemCommon*>& outEquipItemArray, OUT TArray<const struct FGsSchemaItemCommon*>& outEtcItemArray)
{
	TArray<const FGsSchemaDropData*> dropDataArray;
	if (false == TryGetInvadeDropData(inNpcId, inInvadeType, dropDataArray))
	{
		return false;
	}

	return TryConvertDropToItemData(dropDataArray, outEquipItemArray, outEtcItemArray);	
}

bool UGsItemUtil::TryGetInvadeDropData(CreatureId inNpcId, InvadeDropType inInvadeType, OUT TArray<const FGsSchemaDropData*>& outDropDataArray)
{
	//find server data
	const UGsTableDropData* dropDataTable = Cast<const UGsTableDropData>(FGsSchemaDropData::GetStaticTable());
	if (nullptr == dropDataTable)
	{
		return false;
	}

	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(inNpcId);
	if (nullptr == npcData)
	{
		return false;
	}

	TArray<const FGsSchemaDropData*> dropDataArray;
	dropDataTable->GetAllRows(dropDataArray);

	TArray<const FGsSchemaDropData*> matchedDropDataArray;
	FString invadeText;
	bool isMatch = false;
	int32 level = npcData->fixedLevel;
	for (const FGsSchemaDropData* dropData : dropDataArray)
	{
		if (DropCheckType::INVADE != dropData->dropType)
		{
			continue;
		}

		if (level == dropData->level)
		{
			invadeText = FGsTextUtil::GetStringFromEnum(TEXT("InvadeDropType"), inInvadeType);
			isMatch = invadeText.Equals(dropData->dropValue);
			if (isMatch)
			{
				outDropDataArray.Add(dropData);
			}
		}		
	}

	return outDropDataArray.Num() > 0;
}

bool UGsItemUtil::TryConvertDropToItemData(TArray<const struct FGsSchemaDropData*>& inDropDataArray, OUT TArray<const struct FGsSchemaItemCommon*>& outEquipItemArray, OUT TArray<const struct FGsSchemaItemCommon*>& outEtcItemArray)
{
	int32 num = inDropDataArray.Num();
	TArray<const FGsSchemaItemCommon*> itemArray;
	for (const FGsSchemaDropData* dropData : inDropDataArray)
	{
		const TArray<FGsSchemaDropItemGroupInfo>& dropItemGroupList = dropData->itemGroup.GetRow()->dropItemGroupList;
		for (const FGsSchemaDropItemGroupInfo& itemInfo : dropItemGroupList)
		{
			if (false == itemArray.Contains(itemInfo.itemId.GetRow()))
			{
				itemArray.Add(itemInfo.itemId.GetRow());
			}
		}
	}

	num = itemArray.Num();
	const FGsSchemaItemCommon* findItem = nullptr;
	for (const FGsSchemaItemCommon* item : itemArray)
	{
		if (ItemType::EQUIP == item->type)
		{
			if (false == outEquipItemArray.Contains(item))
			{
				outEquipItemArray.Add(item);
			}
		}
		else
		{
			if (false == outEtcItemArray.Contains(item))
			{
				outEtcItemArray.Add(item);
			}
		}
	}

	return true;
}

int32 UGsItemUtil::GetItemSortOrder(ItemType inType)
{
	switch (inType)
	{
	case ItemType::CURRENCY:
		return 10;
		break;
	}

	return 1;
}
