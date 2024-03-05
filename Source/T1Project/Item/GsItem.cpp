// Fill out your copyright notice in the Description page of Project Settings.


#include "GsItem.h"
#include "GsItemManager.h"
#include "T1Project.h"
#include "T1Project/UTIL/GsClassUtil.h"
#include "T1Project/UTIL/GsTimeSyncUtil.h"
#include "GsItemCurrency.h"
#include "GsItemEquipment.h"
#include "GsItemConsumable.h"
#include "GsItemConsumableBox.h"
#include "GsItemConsumableSummon.h"
#include "GsItemIngredient.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Management/ScopeGame/GsDepotManager.h"
#include "Management/ScopeGame/GsUtilizingContentsManager.h"
#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/GsScopeHolder.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Preset/GsPresetHelper.h"


FGsItem::FGsItem(const int32 In_ItemTID)
{
	UpdateTableData(In_ItemTID);
}

FGsItem::~FGsItem()
{
	_itemTableData = nullptr;
}

FGsItem* FGsItem::Create(const ItemData* In_ItemData)
{
	FGsItem* item = Create(In_ItemData->mTemplateId);
	if (nullptr == item)
		return nullptr;

	item->UpdateBy(In_ItemData);
	return item;
}

FGsItem* FGsItem::Create(const ItemDataEquip* In_ItemEquipData)
{
	FGsItem* item = Create(In_ItemEquipData->mTemplateId);
	if (nullptr == item)
		return nullptr;

	item->UpdateBy(In_ItemEquipData);
	return item;
}

FGsItem* FGsItem::Create(const FGsItem* In_Item)
{
	if (nullptr == In_Item)
	{
		GSLOG(Warning, TEXT("CreateItem Failed - In_Item is null"));
		return nullptr;
	}

	FGsItem* item = Create(In_Item->GetTID());
	if (nullptr == item)
		return nullptr;

	item->UpdateBy(In_Item);
	return item;
}

FGsItem* FGsItem::Create(const uint32 In_ItemTID, const ItemAmount In_ItemAmount)
{
	const FGsSchemaItemCommon* tableData = GetTableDataByID(In_ItemTID);
	if (nullptr == tableData)
	{
		if (UGsItemManager* itemMgr = GItem())
		{
			itemMgr->ShowErrorTidTicker(In_ItemTID);
		}
		GSLOG(Log, TEXT("Not Exist DC_TableData : %d "), In_ItemTID);
		//check(tableData);
		return nullptr;
	}

	// 장착류 아이템
	// 기타 아이템들
	FGsItem* newItem = nullptr;
	if (ItemType::EQUIP == tableData->type)
	{
		newItem = new FGsItemEquipment(tableData);
	}
	else if (ItemType::CONSUME == tableData->type)
	{
		if (ItemCategorySub::REWARD_BOX == tableData->categorySub)
		{
			newItem = new FGsItemConsumableBox(tableData->detailId);
		}
		else if (ItemCategorySub::SUMMON_COSTUME == tableData->categorySub ||
			ItemCategorySub::SUMMON_FAIRY== tableData->categorySub)
		{
			newItem = new FGsItemConsumableSummon(tableData->detailId);
		}
		else
		{
			newItem = new FGsItemConsumable(tableData->detailId);
		}
	}
	else if (ItemType::INGREDIENT == tableData->type)
	{
		newItem = new FGsItemIngredient(tableData);
	}
	else if (ItemType::CURRENCY == tableData->type)
	{
		newItem = new FGsItemCurrency(tableData);
	}
	else
	{
		int32 id = UGsTableUtil::ConvertRow<int32>(tableData);
		newItem = new FGsItem(id);
	}

	check(newItem);
	newItem->UpdateTableData(In_ItemTID);
	newItem->_itemAmount = In_ItemAmount;
	return newItem;
}

void FGsItem::UpdateTableData(const uint32 In_ItemID)
{
	const FGsSchemaItemCommon* tableData = GetTableDataByID(In_ItemID);
	if (nullptr == tableData)
	{
		check(tableData);
		GSLOG(Warning, TEXT("UpdateTableData - DC_TableData is null !! ItemID : %d"), In_ItemID);
		return;
	}

	_itemTableData = tableData;
}

void FGsItem::UpdateBy(const FGsItem* In_Item)
{
	check(In_Item);
	UpdateTableData(In_Item->GetTID());

	_itemDBID			= In_Item->GetDBID();
	_itemAmount			= In_Item->GetAmount();
	_itemLevel			= In_Item->GetLevel();
	_itemLock			= In_Item->GetLock();
	_expiredTime		= In_Item->GetExpiredTime();
	_isDepotItem		= In_Item->IsDepotItem();
	_invenSortValue		= In_Item->GetInvenSortValue();
	_isOtherUserItem	= In_Item->IsOtherUserItem();
}

// Server ItemData Packet 으로 덮어씀
void FGsItem::UpdateBy(const ItemData* In_ServerData)
{
	check(In_ServerData);
	UpdateTableData(In_ServerData->mTemplateId);

	_itemDBID			= In_ServerData->mItemDBId;
	_itemAmount			= In_ServerData->mItemAmount;
	_itemLock			= In_ServerData->mIsLocked;
	_expiredTime		= In_ServerData->mExpiredTime;
	_invenSortValue		= In_ServerData->mInventorySortValue;
}

// Server EquipItemData Packet 으로 덮어씀
void FGsItem::UpdateBy(const ItemDataEquip* In_ServerData)
{
	check(In_ServerData);
	UpdateTableData(In_ServerData->mTemplateId);

	_itemDBID			= In_ServerData->mItemDBId;
	_itemAmount			= In_ServerData->mItemAmount;
	_itemLevel			= In_ServerData->mLevel;
	_itemLock			= In_ServerData->mIsLocked;
	_presetValue		= In_ServerData->mPresetValue;
	_parentItemDBId		= In_ServerData->mParentItemDBId;
	_socketId			= In_ServerData->mSocketId;
	_expiredTime		= In_ServerData->mExpiredTime;
	_invenSortValue		= In_ServerData->mInventorySortValue;
}

const FGsSchemaItemCommon* FGsItem::GetTableDataByID(const uint32 In_ItemTID)
{
	const UGsTableItemCommon* table = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaItemCommon* row = nullptr;
	if (false == table->FindRowById(In_ItemTID, row))
	{
		return nullptr;
	}

	return row;
}

bool FGsItem::IsValidItemByDBID(const uint64 InItemDBID, const ItemType InItemType)
{
	if (0 >= InItemDBID)
	{
		return false;
	}

	TWeakPtr<FGsItem> findItem =  GItem()->FindItem(InItemDBID, InItemType);
	return findItem.IsValid();
}

bool FGsItem::IsValidItem(const FGsItem* In_Item)
{
	if (nullptr == In_Item)
	{
		return false;
	}
	
	if (nullptr == In_Item->_itemTableData)
	{
		return false;
	}

	return true;
}

bool FGsItem::IsEquipped() const
{
	return GItem()->IsCurrentPresetItem(this);
}

bool FGsItem::IsPresetEquipped() const
{
	if (false == IsEquipment())
		return false;

	return FGsPresetHelper::IsPreset(_presetValue);
}

const FSoftObjectPath* FGsItem::GetIconPath() const
{
	return (_itemTableData) ? &_itemTableData->icon : nullptr;
}

bool FGsItem::IsRefineOptionStoneItem() const
{
	ItemCategorySub subCateogry = GetSubCategory();
	if (ItemCategorySub::OPTION_STONE == subCateogry
		|| ItemCategorySub::BLESSED_OPTION_STONE == subCateogry)
	{
		return true;
	}

	return false;
}

bool FGsItem::IsEnchantScrollItem() const
{
	ItemCategoryMain mainCategory =  GetMainCategory();
	if (ItemCategoryMain::SCROLL != mainCategory)
		return false;

	ItemCategorySub subCategory = GetSubCategory();
	if (subCategory == ItemCategorySub::ENCHANT_WEAPON_SCROLL || subCategory == ItemCategorySub::BLESSED_ENCHANT_WEAPON
		|| subCategory == ItemCategorySub::CURSED_ENCHANT_WEAPON || subCategory == ItemCategorySub::ENCHANT_ARMOR
		|| subCategory == ItemCategorySub::BLESSED_ENCHANT_ARMOR || subCategory == ItemCategorySub::CURSED_ENCHANT_ARMOR
		|| subCategory == ItemCategorySub::ENCHANT_ACCESSORY || subCategory == ItemCategorySub::BLESSED_ENCHANT_ACCESSORY
		|| subCategory == ItemCategorySub::CURSED_ENCHANT_ACCESSORY || subCategory == ItemCategorySub::ENCHANT_COLLECTABLE_SCROLL)
	{
		return true;
	}
	return false;
}

bool FGsItem::IsCursedEnchantScrollItem() const
{
	ItemCategoryMain mainCategory = GetMainCategory();
	if (ItemCategoryMain::SCROLL != mainCategory)
		return false;

	ItemCategorySub subCategory = GetSubCategory();
	if (subCategory == ItemCategorySub::CURSED_ENCHANT_WEAPON || subCategory == ItemCategorySub::CURSED_ENCHANT_ARMOR
		|| subCategory == ItemCategorySub::CURSED_ENCHANT_ACCESSORY)
	{
		return true;
	}
	return false;
}

bool FGsItem::IsWeaponType() const
{
	ItemCategoryMain mainCategory = GetMainCategory();
	if (ItemCategoryMain::WEAPON == mainCategory)
	{
		return true;
	}
	return false;
}

bool FGsItem::IsArmorType() const
{
	ItemCategoryMain mainCategory = GetMainCategory();
	if (ItemCategoryMain::ARMOR == mainCategory)
	{
		return true;
	}
	return false;
}

const FGsSchemaItemCategoryDisPlayName* FGsItem::GetCategoryNameData() const
{
	const UGsTableItemCategoryDisPlayName* table = Cast<UGsTableItemCategoryDisPlayName>(FGsSchemaItemCategoryDisPlayName::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaItemCategoryDisPlayName* row = nullptr;
	if (false == table->FindRowById(_itemTableData->categorySub , row))
	{
		return nullptr;
	}

	return row;
}

const ItemId FGsItem::GetFirstItemInItemGroup() const
{
	return UGsItemManager::GetFirstItemIdInGroup(GetTID());
}

ItemId FGsItem::GetTradableItemIdInGroup() const
{
	if (IsTradable())
		return GetTID();

	const TSet<ItemId> ItemIdSet = UGsItemManager::GetAllItemIdsTheSameGroup(GetTID());
	for (ItemId itemId : ItemIdSet)
	{
		const FGsSchemaItemCommon* itemCommon = FGsItem::GetTableDataByID(itemId);
		if (nullptr == itemCommon)
			continue;

		if (itemCommon->tradable)
			return itemCommon->id;
	}

	return INVALID_ITEM_ID;
}

FText FGsItem::MakeSupplyHyperLink() const
{
	const UGsSupplyManager* SupplyManager = GSSupply();
	if (nullptr == SupplyManager || nullptr == _itemTableData)
	{
		return FText();
	}

	return SupplyManager->MakeRichTextItemId(StaticCast<ItemId>(_itemTableData->id));
}

FText FGsItem::MakeUtilizeHyperLink() const
{
	const FGsUtilizingContentsManager* utilizeContentsMgr = GSUtilizingContents();
	if (nullptr == utilizeContentsMgr || nullptr == _itemTableData)
	{
		return FText();
	}

	return utilizeContentsMgr->MakeRichText(StaticCast<ItemId>(_itemTableData->id));
}

FText FGsItem::MakeSummonPopupHyperLink() const
{
	if (auto fairyManager = GSFairy())
	{
		auto detailId = GetDetailID();
		auto subCategory = GetSubCategory();
		
		static TMap<ItemCategorySub, IGsBasePortrait::PortraitType> category =
		{
			{ItemCategorySub::SUMMON_COSTUME, IGsBasePortrait::PortraitType::Costume},
			{ItemCategorySub::SUMMON_FAIRY, IGsBasePortrait::PortraitType::Fairy},
		};

		auto iter = category.Find(subCategory);
		if (!iter) return FText::GetEmpty();

		return fairyManager->MakeRichText(*iter, detailId);
	}
	return FText::GetEmpty();
}

void FGsItem::GetCategoryName(OUT FText& outText) const
{
	const FGsSchemaItemCategoryDisPlayName* data = GetCategoryNameData();
	if (nullptr != data)
	{
		outText = data->displayName;
	}
}

float FGsItem::GetWeightPer() const
{
	float wieght = (float)_itemTableData->weight;
	if (0.f >= wieght)
		return 0.f;

	float calc = (wieght / 100.0f);
	return calc;
}

bool FGsItem::IsAttackConsumeItem() const
{
	ItemCategorySub subCategory = GetSubCategory();
	switch (subCategory)
	{
	case ItemCategorySub::ARROW:
	case ItemCategorySub::MAGIC_STONE:
		return true;
	}

	return false;
}

bool FGsItem::IsAccessoryType() const
{
	ItemCategoryMain mainCategory = GetMainCategory();
	if (ItemCategoryMain::ACCESSORY == mainCategory)
	{
		return true;
	}
	return false;
}

bool FGsItem::IsCollectableType() const
{
	ItemCategoryMain mainCategory = GetMainCategory();
	if (ItemCategoryMain::COLLECTABLE == mainCategory)
	{
		return true;
	}
	return false;
}

bool FGsItem::IsDisableLockItem() const
{
	switch (GetSubCategory())
	{
	case ItemCategorySub::ARCAS:
	case ItemCategorySub::SPIRIT_SHOT:
	case ItemCategorySub::MAGIC_STONE:
	case ItemCategorySub::SPELL_STONE:
	case ItemCategorySub::ARROW:
	case ItemCategorySub::HP_POTION:
	case ItemCategorySub::DISCOUNT_COUPON:
		return true;
	}

	return false;
}

const bool FGsItem::GetConvertServerUTCExpiredTime(FDateTime& outDateTime) const
{
	if(false == IsPeriodItem() || 0 >= _expiredTime)
		return false;

	//outDateTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_expiredTime);
	outDateTime = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(_expiredTime);
	return true;
}

const bool FGsItem::GetExpiredRemainTime(FTimespan& outSpanTime) const
{
	if (false == IsPeriodItem())
		return false;

	const FGsSchemaItemExpiredTime* data = GetExpriedTimeData();
	if (nullptr == data)
		return false;

	FDateTime serverDataTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC();
	FDateTime expiredTime;
	if (false == data->endTime.IsEmpty() && true == FDateTime::Parse(data->endTime, expiredTime))
	{
		if (expiredTime <= serverDataTime)
		{
			return false;
		}

		outSpanTime = expiredTime - serverDataTime;
		return true;
	}

	if (true == GetConvertServerUTCExpiredTime(expiredTime))
	{
		if (expiredTime <= serverDataTime)
		{
			return false;
		}
		outSpanTime = expiredTime - serverDataTime;
		return true;
	}

	return false;
}

const FGsSchemaItemExpiredTime* FGsItem::GetExpriedTimeData() const
{
	if (false == IsPeriodItem())
		return nullptr;

 	const UGsTableItemExpiredTime* table = Cast<UGsTableItemExpiredTime>(FGsSchemaItemExpiredTime::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaItemExpiredTime* row = nullptr;
	if (false == table->FindRowById(_itemTableData->expiredTimeId, row))
	{
		return nullptr;
	}

	return row;
}

bool FGsItem::IsExpiredTime() const
{
	if (false == IsPeriodItem())
		return false;

	const FGsSchemaItemExpiredTime* data = GetExpriedTimeData();
	if (nullptr == data)
		return false;

	FDateTime serverDataTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC();
	FDateTime expiredTime;
	if (false == data->endTime.IsEmpty() && true == FDateTime::Parse(data->endTime, expiredTime))
	{	
		if (expiredTime <= serverDataTime)
		{
			return true;
		}

		return false;
	}

	if (true == GetConvertServerUTCExpiredTime(expiredTime))
	{
		if (expiredTime <= serverDataTime)
		{
			return true;
		}
	}

	return false;
}

bool FGsItem::IsRemoveAfterExpired() const
{
	const FGsSchemaItemExpiredTime* data = GetExpriedTimeData();
	if (nullptr == data)
		return false;
	
	return data->isAllowToDelete;
}

const bool FGsItem::IsAutoHpPotionItem() const
{
	if (_itemTableData)
	{
		return ItemType::CONSUME == _itemTableData->type && ItemCategoryMain::POTION == _itemTableData->categoryMain 
			&& ItemCategorySub::HP_POTION == _itemTableData->categorySub;
	}

	return false;
}

// 참 같은경우 파괴되지 않으면서 강화는 지속해야 되는 아이템
const bool FGsItem::IsSpecialAccItem() const
{
	if (_itemTableData)
	{
		return _itemTableData->categorySub == ItemCategorySub::CHARM || _itemTableData->categorySub == ItemCategorySub::DURAL;
	}
	return false;
}