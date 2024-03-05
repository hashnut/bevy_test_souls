// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMarketItemData.h"
#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h" 
#include "Shared/Shared/SharedInclude/SharedFuncs.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "UTIL/GsTimeSyncUtil.h"


FGsMarketItemData::~FGsMarketItemData()
{
	if (_item)
	{
		delete _item;
		_item = nullptr;
	}
}

void FGsMarketItemData::Clear()
{
	_itemId = INVALID_ITEM_ID;
	_transactionId = INVALID_MARKET_TRANSACTION_ID;
	_price = 0;

	if (_item)
	{
		delete _item;
		_item = nullptr;
	}

	_sellStartTime = 0;
	_registeredServerTime = 0;
}

void FGsMarketItemData::SetInstackableItem(ItemId InItemId, void* InData)
{
	PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_LIST_INSTACKABLE_READ::TransactionList* data = 
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_LIST_INSTACKABLE_READ::TransactionList*>(InData);

	_itemId = InItemId;
	_transactionId = data->MarketTransactionId();
	_price = data->Price();

	ItemDataEquip equipData;
	equipData.mTemplateId = InItemId;
	equipData.mLevel = data->Level(); // 강화레벨
	equipData.mEnchantShield = data->EnchantShield(); // 실드 정보
	equipData.mEnchantBonusIndex = data->EnchantBonusIndex(); // 특성 정보

	// 제련 스탯 정보
	int32 index = 0;
	for (PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_LIST_INSTACKABLE_READ::TransactionList::StatInfoListIterator iter =
		data->GetFirstStatInfoListIterator(); iter != data->GetLastStatInfoListIterator(); ++iter)
	{
		equipData.mRefineOptionArray[index].mType = iter->StatType();
		equipData.mRefineOptionArray[index].mValue = iter->StatValue();
		++index;
	}

	// 제련 보너스(환경효과 저항) 정보
	const RefineBonusInfo& refineBonusInfo = data->ConstRefRefineBonusInfo();
	equipData.mRefineBonusInfo.mBonusId = refineBonusInfo.mBonusId;
	equipData.mRefineBonusInfo.mValue = refineBonusInfo.mValue;

	if (nullptr == _item)
	{
		_item = FGsItem::Create(InItemId);
	}

	// 개화 정보
	equipData.mMagicalForgeData = data->MagicalForgeData();

	_item->UpdateBy(&equipData);
}

void FGsMarketItemData::SetStackableItem(ItemId InItemId, void* InData)
{
	PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_LIST_STACKABLE_READ::TransactionList* data =
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_LIST_STACKABLE_READ::TransactionList*>(InData);

	_itemId = InItemId;
	_transactionId = data->MarketTransactionId();
	_price = data->Price();

	if (nullptr == _item)
	{
		_item = FGsItem::Create(InItemId, data->Amount());
	}
	else
	{
		_item->SetAmount(data->Amount());
	}
}

void FGsMarketItemData::SetSellItem(void* InData)
{
	PD::GC::PKT_GC_ACK_MARKET_MY_TRANSACTION_LIST_READ::TransactionList* data =
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_MY_TRANSACTION_LIST_READ::TransactionList*>(InData);

	_itemId = data->ItemId();
	_transactionId = data->MarketTransactionId();
	_price = data->Price();

	if (nullptr == _item)
	{
		_item = FGsItem::Create(data->ItemId(), 0);
	}
	_item->SetAmount(data->Amount());
	_item->SetLevel(data->Level());

	if (_item->IsEquipment())
	{
		if (FGsItemEquipment* equipItem = StaticCast<FGsItemEquipment*>(_item))
		{
			equipItem->UpdateMagicalForge(data->MagicalForgeData());
		}
	}
	
	_sellStartTime = data->TimeSalesStart();
}

void FGsMarketItemData::SetDetailData(PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_DETAIL_EQUIP_READ* InData)
{
	// 장비 정보 아니면 저장할 게 따로 없음, 이미 item은 만들어져 있어야 함
	if (nullptr == _item ||
		false == _item->IsEquipment())
	{
		return;
	}

	ItemDataEquip equipData;
	equipData.mTemplateId = _itemId;
	equipData.mLevel = _item->GetLevel(); // 강화레벨
	equipData.mEnchantShield = InData->EnchantShield(); // 실드 정보
	equipData.mEnchantBonusIndex = InData->EnchantBonusIndex(); // 특성 정보

	// 제련 스탯 정보
	int32 index = 0;
	for (PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_DETAIL_EQUIP_READ::StatInfoListIterator iter =
		InData->GetFirstStatInfoListIterator(); iter != InData->GetLastStatInfoListIterator(); ++iter)
	{
		equipData.mRefineOptionArray[index].mType = iter->StatType();
		equipData.mRefineOptionArray[index].mValue = iter->StatValue();
		++index;
	}

	// 제련 보너스(환경효과 저항) 정보
	const RefineBonusInfo& refineBonusInfo = InData->ConstRefRefineBonusInfo();
	equipData.mRefineBonusInfo.mBonusId = refineBonusInfo.mBonusId;
	equipData.mRefineBonusInfo.mValue = refineBonusInfo.mValue;

	// 개화 정보
	equipData.mMagicalForgeData = InData->MagicalForgeData();

	_item->UpdateBy(&equipData);
}

void FGsMarketItemData::SetSellRegisteredItem(PD::SC::PKT_SC_ACK_MARKET_TRANSACTION_REGISTER_READ* InData)
{
	_itemId = InData->ItemId();
	_transactionId = InData->MarketTransactionId();
	_price = InData->Price();

	if (nullptr == _item)
	{
		_item = FGsItem::Create(InData->ItemId());
	}
	_item->SetAmount(InData->Amount());
	_item->SetLevel(InData->Level());

	if (_item->IsEquipment())
	{
		if (FGsItemEquipment* equipItem = StaticCast<FGsItemEquipment*>(_item))
		{
			equipItem->UpdateMagicalForge(InData->MagicalForgeData());
		}
	}

	_sellStartTime = InData->TimeSalesStart();

	_registeredServerTime = FGsTimeSyncUtil::GetServerNowSecond();
}

void FGsMarketItemData::SetInstackableItemWd(ItemId InItemId, void* InData)
{
	PD::GC::PKT_GC_ACK_WDMARKET_TRANSACTION_LIST_INSTACKABLE_READ::TransactionList* data =
		reinterpret_cast<PD::GC::PKT_GC_ACK_WDMARKET_TRANSACTION_LIST_INSTACKABLE_READ::TransactionList*>(InData);

	_itemId = InItemId;
	_transactionId = data->MarketTransactionId();
	_price = data->Price();

	ItemDataEquip equipData;
	equipData.mTemplateId = InItemId;
	equipData.mLevel = data->Level(); // 강화레벨
	equipData.mEnchantShield = data->EnchantShield(); // 실드 정보
	equipData.mEnchantBonusIndex = data->EnchantBonusIndex(); // 특성 정보

	// 제련 스탯 정보
	int32 index = 0;
	for (PD::GC::PKT_GC_ACK_WDMARKET_TRANSACTION_LIST_INSTACKABLE_READ::TransactionList::StatInfoListIterator iter =
		data->GetFirstStatInfoListIterator(); iter != data->GetLastStatInfoListIterator(); ++iter)
	{
		equipData.mRefineOptionArray[index].mType = iter->StatType();
		equipData.mRefineOptionArray[index].mValue = iter->StatValue();
		++index;
	}

	// 제련 보너스(환경효과 저항) 정보
	const RefineBonusInfo& refineBonusInfo = data->ConstRefRefineBonusInfo();
	equipData.mRefineBonusInfo.mBonusId = refineBonusInfo.mBonusId;
	equipData.mRefineBonusInfo.mValue = refineBonusInfo.mValue;

	if (nullptr == _item)
	{
		_item = FGsItem::Create(InItemId);
	}

	// 개화 정보
	equipData.mMagicalForgeData = data->MagicalForgeData();

	_item->UpdateBy(&equipData);
}

void FGsMarketItemData::SetStackableItemWd(ItemId InItemId, void* InData)
{
	PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_LIST_STACKABLE_READ::TransactionList* data =
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_LIST_STACKABLE_READ::TransactionList*>(InData);

	_itemId = InItemId;
	_transactionId = data->MarketTransactionId();
	_price = data->Price();

	if (nullptr == _item)
	{
		_item = FGsItem::Create(InItemId, data->Amount());
	}
	else
	{
		_item->SetAmount(data->Amount());
	}
}

void FGsMarketItemData::SetSellItemWd(void* InData)
{
	PD::GC::PKT_GC_ACK_WDMARKET_MY_TRANSACTION_LIST_READ::TransactionList* data =
		reinterpret_cast<PD::GC::PKT_GC_ACK_WDMARKET_MY_TRANSACTION_LIST_READ::TransactionList*>(InData);

	_itemId = data->ItemId();
	_transactionId = data->MarketTransactionId();
	_price = data->Price();

	if (nullptr == _item)
	{
		_item = FGsItem::Create(data->ItemId(), 0);
	}
	_item->SetAmount(data->Amount());
	_item->SetLevel(data->Level());

	if (_item->IsEquipment())
	{
		if (FGsItemEquipment* equipItem = StaticCast<FGsItemEquipment*>(_item))
		{
			equipItem->UpdateMagicalForge(data->MagicalForgeData());
		}
	}

	_sellStartTime = data->TimeSalesStart();
}

void FGsMarketItemData::SetDetailDataWd(PD::GC::PKT_GC_ACK_WDMARKET_TRANSACTION_DETAIL_EQUIP_READ* InData)
{
	// 장비 정보 아니면 저장할 게 따로 없음, 이미 item은 만들어져 있어야 함
	if (nullptr == _item ||
		false == _item->IsEquipment())
	{
		return;
	}

	ItemDataEquip equipData;
	equipData.mTemplateId = _itemId;
	equipData.mLevel = _item->GetLevel(); // 강화레벨
	equipData.mEnchantShield = InData->EnchantShield(); // 실드 정보
	equipData.mEnchantBonusIndex = InData->EnchantBonusIndex(); // 특성 정보

	// 제련 스탯 정보
	int32 index = 0;
	for (PD::GC::PKT_GC_ACK_WDMARKET_TRANSACTION_DETAIL_EQUIP_READ::StatInfoListIterator iter = 
		InData->GetFirstStatInfoListIterator(); iter != InData->GetLastStatInfoListIterator(); ++iter)
	{
		equipData.mRefineOptionArray[index].mType = iter->StatType();
		equipData.mRefineOptionArray[index].mValue = iter->StatValue();
		++index;
	}

	// 제련 보너스(환경효과 저항) 정보
	const RefineBonusInfo& refineBonusInfo = InData->ConstRefRefineBonusInfo();
	equipData.mRefineBonusInfo.mBonusId = refineBonusInfo.mBonusId;
	equipData.mRefineBonusInfo.mValue = refineBonusInfo.mValue;

	// 개화 정보
	equipData.mMagicalForgeData = InData->MagicalForgeData();

	_item->UpdateBy(&equipData);
}

void FGsMarketItemData::SetSellRegisteredItemWd(PD::SC::PKT_SC_ACK_WDMARKET_TRANSACTION_REGISTER_READ* InData)
{
	_itemId = InData->ItemId();
	_transactionId = InData->MarketTransactionId();
	_price = InData->Price();

	if (nullptr == _item)
	{
		_item = FGsItem::Create(InData->ItemId());
	}
	_item->SetAmount(InData->Amount());
	_item->SetLevel(InData->Level());

	if (_item->IsEquipment())
	{
		if (FGsItemEquipment* equipItem = StaticCast<FGsItemEquipment*>(_item))
		{
			equipItem->UpdateMagicalForge(InData->MagicalForgeData());
		}
	}

	_sellStartTime = InData->TimeSalesStart();

	_registeredServerTime = FGsTimeSyncUtil::GetServerNowSecond();
}

const FGsSchemaItemCommon* FGsMarketItemData::GetTable() const
{
	return _item ? _item->GetTableData() : nullptr;
}

MarketPrice FGsMarketItemData::GetPricePerUnit() const
{
	ItemAmount amount = (_item) ? _item->GetAmount() : 0;
	if (0 == amount)
	{
		return _price;
	}

	return SharedFunc::GetItemPricePerUnit(_price, amount);
}

int32 FGsMarketItemData::GetEnchantLevel() const
{
	return _item ? _item->GetLevel() : 0;
}

ItemAmount FGsMarketItemData::GetAmount() const
{
	return _item ? _item->GetAmount() : 0;
}

bool FGsMarketItemData::CanCompare() const
{
	if (const FGsSchemaItemCommon* table = GetTable())
	{
		return (ItemType::EQUIP == table->type) ? true : false;
	}

	return false;
}
