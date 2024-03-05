/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/NpcShop/EventShop/GsSchemaNpcEventShopData.h"
FGsSchemaNpcEventShopData::FGsSchemaNpcEventShopData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaNpcEventShopData)
	
	groupId = 0;
	itemId = 0;
	productId = 0;
	displayPriority = 0;
	costType = CostType::CURRENCY;
	currencyType = CurrencyType::NONE;
	currencyItemId = 0;
	currencyAmount = 0;
	purchaseLimitLevel = 0;
	purchaseLimitGuildLevel = 0;
	purchaseLimitGuildMemberGradeType = GuildMemberGradeType::UNKNOWN;
	purchaseLimitType = PurchaseLimitBuyerType::NONE;
	purchaseLimitAmount = 0;
	purchaseLimitResetType = ResetTimeTicket::RTT_NONE;
}
