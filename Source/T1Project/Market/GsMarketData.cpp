// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMarketData.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Item/GsItem.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"

//----------------------------------------------------------------------------------------------------------------------
void FGsMarketItemPriceData::Clear()
{
	_itemId = INVALID_ITEM_ID;
	_enchantLevel = INVALID_LEVEL;

	_lowestPrice = 0;
	_heighestPrice = 0;
	_averagePrice = 0;
	_latestPrice = 0;
	_lowestPriceCurrent = 0;

	_bIsWorldMarket = false;
}

void FGsMarketItemPriceData::SetData(PD::GC::PKT_GC_ACK_MARKET_ITEM_PRICE_READ* InData)
{
	_itemId = InData->ItemId();
	_enchantLevel = InData->Level();

	_lowestPrice = InData->LowestPrice();
	_heighestPrice = InData->HighestPrice();
	_averagePrice = InData->AveragePrice();
	_latestPrice = InData->LatestPrice();
	_lowestPriceCurrent = InData->LowestPriceCurrent();

	_bIsWorldMarket = false;
}

void FGsMarketItemPriceData::SetDataWd(PD::GC::PKT_GC_ACK_WDMARKET_ITEM_PRICE_READ* InData)
{
	_itemId = InData->ItemId();
	_enchantLevel = InData->Level();

	_lowestPrice = InData->LowestPrice();
	_heighestPrice = InData->HighestPrice();
	_averagePrice = InData->AveragePrice();
	_latestPrice = InData->LatestPrice();
	_lowestPriceCurrent = InData->LowestPriceCurrent();

	_bIsWorldMarket = true;
}

//----------------------------------------------------------------------------------------------------------------------
FGsMarketSettlementData::~FGsMarketSettlementData()
{
	if (_item)
	{
		delete _item;
		_item = nullptr;
	}
}

void FGsMarketSettlementData::GetSaleTimeInfo(const time_t& InTime, OUT FDateTime& OutDateTime, OUT FText& OutDateFixed, OUT FText& OutTimeFixed)
{
	FDateTime dateTimeSale = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(InTime);

	// FText::AsDate, FText::AsTime을 그냥 사용하면 클라 시간으로 변환한다. InvariantTimeZone값을 넣어 그대로 찍도록 함.
	// https://docs.unrealengine.com/4.26/en-US/ProductionPipelines/Localization/Formatting/

	FString strInvarientTimeZone = FText::GetInvariantTimeZone();
	OutDateTime = dateTimeSale;

	FString textDate;
	FGsTimeStringHelper::GetTimeStringNYMD(dateTimeSale, textDate);

	//OutDateFixed = FText::AsDate(dateTimeSale.GetDate(), EDateTimeStyle::Default, strInvarientTimeZone);
	OutDateFixed = FText::FromString(textDate);
	OutTimeFixed = FText::AsTime(dateTimeSale, EDateTimeStyle::Default, strInvarientTimeZone);
}

void FGsMarketSettlementData::Clear()
{
	_transactionId = INVALID_MARKET_TRANSACTION_ID;
	_itemId = INVALID_ITEM_ID;
	_enchantLevel = 0;
	_amount = 0;
	_price = 0;
	_earnings = 0;
	_timeOfSale = 0;

	if (_item)
	{
		delete _item;
		_item = nullptr;
	}

	_bIsWorldMarket = false;
}

void FGsMarketSettlementData::SetData(void* InData)
{
	PD::GC::PKT_GC_ACK_MARKET_CALCULATED_LIST_READ::CalculatedList* data = 
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_CALCULATED_LIST_READ::CalculatedList*>(InData);

	_transactionId = data->MarketTransactionId();
	_itemId = data->ItemId();
	_enchantLevel = data->Level();
	_amount = data->Amount();
	_price = data->Price();
	_earnings = data->CalculatedPrice();
	_timeOfSale = data->TimeSalesComplete();

	_bIsWorldMarket = false;
}

void FGsMarketSettlementData::SetLogData(void* InData)
{
	PD::GC::PKT_GC_ACK_MARKET_CALCULATED_LIST_READ::CalculatedList* data = 
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_CALCULATED_LIST_READ::CalculatedList*>(InData);

	_transactionId = data->MarketTransactionId();
	_itemId = data->ItemId();
	_enchantLevel = data->Level();
	_amount = data->Amount();
	_price = data->Price();
	_earnings = data->CalculatedPrice();
	_timeOfSale = data->TimeSalesComplete();

	_bIsWorldMarket = false;
}

void FGsMarketSettlementData::SetDetailData(PD::GC::PKT_GC_ACK_MARKET_TRANSACTION_DETAIL_EQUIP_READ* InData)
{
	if (nullptr == _item)
	{
		_item = FGsItem::Create(_itemId, 0);
	}
	_item->SetAmount(_amount);
	_item->SetLevel(_enchantLevel);

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

void FGsMarketSettlementData::SetDataWd(void* InData)
{
	PD::GC::PKT_GC_ACK_WDMARKET_CALCULATED_LIST_READ::CalculatedList* data = 
		reinterpret_cast<PD::GC::PKT_GC_ACK_WDMARKET_CALCULATED_LIST_READ::CalculatedList*>(InData);

	_transactionId = data->MarketTransactionId();
	_itemId = data->ItemId();
	_enchantLevel = data->Level();
	_amount = data->Amount();
	_price = data->Price();
	_earnings = data->CalculatedPrice();
	_timeOfSale = data->TimeSalesComplete();
	
	_bIsWorldMarket = true;
}

void FGsMarketSettlementData::SetLogDataWd(void* InData)
{
	PD::GC::PKT_GC_ACK_WDMARKET_CALCULATED_LIST_READ::CalculatedList* data =
		reinterpret_cast<PD::GC::PKT_GC_ACK_WDMARKET_CALCULATED_LIST_READ::CalculatedList*>(InData);

	_transactionId = data->MarketTransactionId();
	_itemId = data->ItemId();
	_enchantLevel = data->Level();
	_amount = data->Amount();
	_price = data->Price();
	_earnings = data->CalculatedPrice();
	_timeOfSale = data->TimeSalesComplete();

	_bIsWorldMarket = true;
}

void FGsMarketSettlementData::SetDetailDataWd(PD::GC::PKT_GC_ACK_WDMARKET_TRANSACTION_DETAIL_EQUIP_READ* InData)
{
	if (nullptr == _item)
	{
		_item = FGsItem::Create(_itemId, 0);
	}
	_item->SetAmount(_amount);
	_item->SetLevel(_enchantLevel);

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

const FGsSchemaItemCommon* FGsMarketSettlementData::GetTable() const
{
	if (INVALID_ITEM_ID != _itemId)
	{
		return UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(_itemId);
	}

	return nullptr;
}
