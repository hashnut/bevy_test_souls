// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project/UTIL/GsTableUtil.h"
#include "Data/GsItemTableStruct.h"
#include "util/GsClassUtil.h"
#include "DataSchema/item/GsSchemaItemCommon.h"
#include "DataSchema/UI/GsSchemaItemCategoryDisPlayName.h"
#include "DataSchema/Item/GsSchemaItemExpiredTime.h"
#include "DataSchema/GsSchemaEnums.h"

/**
 * 인게임에서 사용되는 아이템 객체
 */
class T1PROJECT_API FGsItem
{
protected:
	const int64	INVALID_OBJECT_ID = 0;

protected:
	uint64 _itemDBID					= INVALID_OBJECT_ID;
	ItemAmount _itemAmount					= INVALID_OBJECT_ID;
	int32 _itemLevel						= INVALID_OBJECT_ID;
	bool  _itemLock						= false;
	const FGsSchemaItemCommon*	_itemTableData	= nullptr;
	uint8 _presetValue					= INVALID_OBJECT_ID; // 프리셋 비트필드 값 (복수의 프리셋에 속할 수 있다)
	uint64 _parentItemDBId			= INVALID_OBJECT_ID; // 자신이 소켓팅된 부모 아이템
	uint8 _socketId						= INVALID_OBJECT_ID; // 자신이 소켓팅된 소켓 ID
	bool _isNewItem					= false;
	bool _isDepotItem					= false;
	time_t _expiredTime				= 0;
	uint16 _invenSortValue			= 0;
	bool _isOtherUserItem = false;

public:
	explicit FGsItem(const int32 In_ItemTID);
	FGsItem() = default;
	virtual ~FGsItem();

public:
	static FGsItem* Create(const ItemData* In_ItemData);
	static FGsItem* Create(const ItemDataEquip* In_ItemEquipData);
	static FGsItem* Create(const FGsItem* In_Item);
	static FGsItem* Create(const uint32 In_ItemTID, const ItemAmount In_ItemAmount = 1);

public:
	static bool IsValidItem(const FGsItem* In_Item);
	static bool IsValidItemByDBID(const uint64 InItemDBID, const ItemType InItemType = ItemType::MAX);

public:
	virtual void UpdateBy(const FGsItem* In_Item);

	// Packet Data로 덮어씀
	virtual void UpdateBy(const ItemData* In_ServerData);
	virtual void UpdateBy(const ItemDataEquip* In_ServerData);

	// Consumable 및 Ingredient 가 동일한 내용으로 사용하는 함수모음
public:
	virtual bool IsCooldownTimeItem() const { return false; }
	virtual bool IsUseItem() const { return false; }
	virtual float GetMaxCoolTime() const { return 0.f; }
	virtual int32 GetCooldownGroupID() const { return 0; }
	virtual int32 GetTooltipDurationTime() const { return 0; }
	virtual int32 GetPickOneTypeRewardID() const { return 0; }
	virtual int32 GetRewardId() const { return 0; }
	virtual FText GetUIContentsText() const { return FText::GetEmpty(); }
		
public:
	void UpdateTableData(const uint32 In_ItemID);

public:
	static const FGsSchemaItemCommon* GetTableDataByID(const uint32 In_ItemTID);

public:
	bool IsEquipped() const;
	bool IsPresetEquipped() const;
	bool IsEnchantScrollItem() const;
	bool IsCursedEnchantScrollItem() const;
	bool IsRefineOptionStoneItem() const;
	bool IsAttackConsumeItem() const;
	bool IsWeaponType() const;
	bool IsArmorType() const;
	bool IsAccessoryType() const;
	bool IsCollectableType() const;
	bool IsDisableLockItem() const;
	bool IsExpiredTime() const;
	const FSoftObjectPath* GetIconPath() const;
	void GetCategoryName(OUT FText& outText) const;
	float GetWeightPer() const;
	const bool GetConvertServerUTCExpiredTime(FDateTime& outDateTime) const;
	const bool GetExpiredRemainTime(FTimespan& outSpanTime) const;
	const FGsSchemaItemExpiredTime* GetExpriedTimeData() const;
	bool IsRemoveAfterExpired() const;
	const bool IsAutoHpPotionItem() const;
	const bool IsSpecialAccItem() const;

private:
	const FGsSchemaItemCategoryDisPlayName* GetCategoryNameData() const;


///////////////////////////////////////////////////////////////////////////////
////////////////////////////// Get , Set //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
public:
	void SetDBID(const uint64 In_DBID)						{ _itemDBID = In_DBID; }
	void SetAmount(const ItemAmount In_Amount)				{ _itemAmount = In_Amount; }
	void SetLevel(const int32 In_ItemLv)					{ _itemLevel = In_ItemLv; }
	void SetLock(bool In_Lock)								{ _itemLock = In_Lock; }
	void SetEquipPresetValue(const PresetValue In_PresetValue)	{ _presetValue = In_PresetValue; }
	void SetEquipParentItemDBID(const uint64 In_ParentDBID)	{ _parentItemDBId = In_ParentDBID; }
	void SetEquipSocketID(const uint8 In_SocketID)			{ _socketId = In_SocketID; }
	void SetIsNew(bool In_isNewItem)						{ _isNewItem = In_isNewItem; }
	void SetIsDepotItem(bool In_isDepotItem)				{ _isDepotItem = In_isDepotItem; }
	void SetExpiredTime(time_t InTime)						{ _expiredTime = InTime; }
	void SetInvenSortValue(const uint16 In_SortValue)		{ _invenSortValue = In_SortValue; }
	void SetIsOtherUserItem(bool In_IsOtherUserItem)		{ _isOtherUserItem = In_IsOtherUserItem; }

public:
	const uint64 GetDBID() const						{ return _itemDBID; }
	const ItemAmount GetAmount() const					{ return _itemAmount; }
	const Level GetLevel() const						{ return _itemLevel; }
	const bool GetLock() const							{ return _itemLock; }
	const ItemGrade GetGradeType() const				{ return _itemTableData->grade; }
	const FGsSchemaItemCommon* GetTableData() const		{ return _itemTableData; }
	const PresetValue GetEquipPresetValue() const		{ return _presetValue; }
	const uint64 GetEquipParentItemDBID() const			{ return _parentItemDBId; }
	const uint8 GetEquipSocketID() const				{ return _socketId; }
	const bool IsStackable() const						{ return 1 < GetMaxStackCount(); }
	const bool IsNew() const							{ return _isNewItem; }
	const bool IsCashItem() const						{ return ItemAcquireType::PAID == _itemTableData->acquireType; }
	const bool IsDepotItem() const						{ return _isDepotItem; }
	const uint16 GetInvenSortValue() const				{ return _invenSortValue; }
	const bool IsOtherUserItem() const					{ return _isOtherUserItem; }

	const ItemId GetTID() const							{ return _itemTableData->id; }
	const int32 GetDetailID() const						{ return _itemTableData->detailId; }
	const int32 GetBelongingTID() const					{ return _itemTableData->belongingItemId; }
	const int32 GetItemGroupId() const					{ return _itemTableData->itemGroupId; }
	const int32 GetMinLevel() const						{ return _itemTableData->minLevel; }
	const int32 GetMaxLevel() const						{ return _itemTableData->maxLevel; }
	const int32 GetUseLevel() const						{ return _itemTableData->useLevel; }
	const ItemAmount GetMaxStackCount() const			{ return _itemTableData->maxStackCount; }
	const bool IsFull()	const							{ return _itemAmount == _itemTableData->maxStackCount;}
	const bool IsCanDelete() const						{ return _itemTableData->canDelete; }
	const ItemType GetType() const						{ return _itemTableData->type; }
	const ItemCategoryMain GetMainCategory() const		{ return _itemTableData->categoryMain; }
	const ItemCategorySub GetSubCategory() const		{ return _itemTableData->categorySub; }
	const CreatureGenderType GetGenderType() const		{ return _itemTableData->genderType; }
	const int32 GetSellPrice() const					{ return _itemTableData->sellPrice; }
	const int32 GetWeight() const						{ return _itemTableData->weight; }
	const bool IsStoreSellable() const					{ return _itemTableData->storeSellable; }
	const bool IsTradable() const						{ return _itemTableData->tradable; }
	const bool IsWarehouseStorable() const				{ return _itemTableData->warehouseStorable; }
	const bool IsDecomposable() const					{ return _itemTableData->decomposable; }
	const FText& GetName() const						{ return _itemTableData->name; }
	const FText& GetToolTipDesc() const					{ return _itemTableData->tooltip; }
	const bool IsEquipment() const						{ return ItemType::EQUIP == _itemTableData->type; }
	const bool IsConsumable() const						{ return ItemType::CONSUME == _itemTableData->type; }
	const bool IsIngredient() const						{ return ItemType::INGREDIENT == _itemTableData->type; }
	const bool IsCurrency() const                       { return ItemType::CURRENCY == _itemTableData->type; }
	const bool IsMaxLevel() const						{ return _itemLevel >= GetMaxLevel(); }
	const bool IsMinLevel() const						{ return _itemLevel <= GetMinLevel(); }
	const bool IsPeriodItem() const						{ return 0 < _itemTableData->expiredTimeId; }
	const time_t GetExpiredTime() const					{ return _expiredTime; }
	const ItemAmount GetLeftStackCount() const			{ return GetMaxStackCount() - GetAmount(); }
	const bool IsPersonalTradable() const				{ return _itemTableData->personalTradable; }
	const EGsItemIconSlotEffectType GetIconSlotEffectType() const	{ return _itemTableData->iconSlotEffectType; }


	///////////////////////////////////////////////////////////////////////////////
	////////////////////////////// ItemGroup //////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
public:
	/** 아이템그룹의 첫번째 아이템ID */
	const ItemId GetFirstItemInItemGroup() const;

	/** 해당 아이템의 거래 가능 아이템 ID 조회 */
	ItemId GetTradableItemIdInGroup() const;

public:
	FText MakeSupplyHyperLink() const;
	FText MakeUtilizeHyperLink() const;

	FText MakeSummonPopupHyperLink() const;

public:
	template <typename T = FGsItem>
	T* GetCastItem();
};

template <typename T>
T* FGsItem::GetCastItem()
{
	static_assert(TIsDerivedFrom<T, FGsItem>::IsDerived, "tCast must be derived from FGsItem");
	return FGsClassUtil::FCheckStaticCast<T>(this);
}