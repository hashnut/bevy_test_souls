/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/BMShop/GsSchemaBMShopProductData.h"
FGsSchemaBMShopProductData::FGsSchemaBMShopProductData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaBMShopProductData)
	
	id = 0;
	itemId = 0;
	purchaseCostType = CostType::CURRENCY;
	purchaseCurrencyItemId = 0;
	purchaseCurrencyType = CurrencyType::NONE;
	purchaseCurrencyPrice = 0;
	rebateCurrencyType = CurrencyType::NONE;
	rebateCurrencyAmount = 0;
	saleStartTime = FString(TEXT("0000.00.00 00.00.00"));
	saleEndTime = FString(TEXT("0000.00.00 00.00.00"));
	displaySaleEndCountdownTimeValue = 0;
	pcLevelLimit = 0;
	quantityLimit = 0;
	bmShopLimitBuyerType = BMPurchaseLimitBuyerType::NONE;
	resetTimeTicketType = ResetTimeTicket::RTT_NONE;
	visibilityFlag = true;
	parentProductId = 0;
	bmShopParentTabId = 0;
	bmShopSubTabId = 0;
	displayOrder = 0;
	detailPopupType = EBMShopProductDetailPopupType::ContractGuideType;
	tagType = EBMShopProductTagType::Tag_None;
	highlightType = EBMShopHighlightType::Highlight_Off;
	guildLevelLimit = 0;
	isGuildGiftProduct = false;
	bannerDisplayOrder = 0;
	isBulkSale = false;
	stepGroupID = 0;
	stepLevel = 0;
	stepBuyLimitCount = 0;
	popupConditionId = 0;
	popupSaleDurationTime = 0;
	popuphiveBannerId = 0;
	isOOAP = false;
	immediatelyUse = false;
	resetAfterServerMigrate = false;
	resetConditionType = BMResetConditionType::NONE;
	isMultiplePurchase = false;
}
