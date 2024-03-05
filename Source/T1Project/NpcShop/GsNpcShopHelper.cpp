// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNpcShopHelper.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"

#include "Net/GsNetSendServiceItem.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "DataSchema/GsSchemaResetTimeTicketConfig.h"

#include "UTIL/GsCronUtil.h"


bool FGsNpcShopHelper::IsAllowedSellItem(const FGsItem* InItem, OUT EGsNotAllowedSellReaon& OutReason)
{
	if (nullptr == InItem)
		return false;

	if (false == InItem->IsStoreSellable())
		return false;

	if (true == InItem->GetLock())
		return false;

	if (true == InItem->IsEquipped())
		return false;

	if (true == InItem->IsPresetEquipped())
		return false;

	return true;
}

bool FGsNpcShopHelper::IsAllowedBuyItem(const FGsNpcShopItemInfo* InItemInfo, OUT EGsNotAllowedBuyReason& OutReaon)
{
	if (nullptr == InItemInfo)
	{
		OutReaon = EGsNotAllowedBuyReason::OTHER_REASON;
		return false;
	}

	bool resultAllowedBuyItem = true;
	// 매진 여부 확인
	resultAllowedBuyItem &= IsOverLimitPurchase(InItemInfo, OutReaon);
	// 레벨 제한 검사
	resultAllowedBuyItem &= IsOverLimitLevel(InItemInfo->_pcLimitLevel, OutReaon);
	// 기사단 레벨 제한 검사
	resultAllowedBuyItem &= IsOverLimitGuildLevel(InItemInfo->_pcLimitGuildLevel, OutReaon);
	// 기사단 계급 제한 검사
	resultAllowedBuyItem &= IsOverLimitGuildMemberGrade(InItemInfo->_pcLimitGuildMemberGrade, OutReaon);

	return resultAllowedBuyItem;
}

bool FGsNpcShopHelper::IsOverLimitLevel(const int32 InLimitLevel, OUT EGsNotAllowedBuyReason& OutReaon)
{
	if (EGsNotAllowedBuyReason::NONE != OutReaon)
		return false;

	FGsGameDataManager* dataManager = GGameData();
	if (nullptr == dataManager)
	{
		OutReaon = EGsNotAllowedBuyReason::OTHER_REASON;
		return false;
	}

	const FGsNetUserData* userData = dataManager->GetUserData();
	if (nullptr == userData)
	{
		OutReaon = EGsNotAllowedBuyReason::OTHER_REASON;
		return false;
	}

	if (InLimitLevel > userData->mLevel)
	{
		OutReaon = EGsNotAllowedBuyReason::UNDER_LIMIT_LEVEL;
		return false;
	}

	return true;
}

bool FGsNpcShopHelper::IsOverLimitGuildLevel(const GuildLevel InLimitGuildLevel, OUT EGsNotAllowedBuyReason& OutReaon)
{
	if (EGsNotAllowedBuyReason::NONE != OutReaon)
		return false;

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
	{
		OutReaon = EGsNotAllowedBuyReason::OTHER_REASON;
		return false;
	}

	if (InLimitGuildLevel > guildManager->GetGuildLevel())
	{
		OutReaon = EGsNotAllowedBuyReason::UNDER_LIMIT_GUILD_LEVEL;
		return false;
	}

	return true;
}

bool FGsNpcShopHelper::IsOverLimitGuildMemberGrade(const GuildMemberGradeType InLimitGuildMemberGrade, OUT EGsNotAllowedBuyReason& OutReaon)
{
	if (EGsNotAllowedBuyReason::NONE != OutReaon)
		return false;

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
	{
		OutReaon = EGsNotAllowedBuyReason::OTHER_REASON;
		return false;
	}

	// 멤버 등급이 클수록 값이 크다.
	if (static_cast<uint8>(InLimitGuildMemberGrade) > static_cast<uint8>(guildManager->GetMemeberGrade()))
	{
		OutReaon = EGsNotAllowedBuyReason::UNDER_LIMIT_GUILD_MEMBER_GRADE;
		return false;
	}

	return true;
}

bool FGsNpcShopHelper::IsOverLimitPurchase(const FGsNpcShopItemInfo* InItemInfo, OUT EGsNotAllowedBuyReason& OutReaon)
{
	if (EGsNotAllowedBuyReason::NONE != OutReaon)
		return false;

	if (0 >= InItemInfo->_pcLimitBuyerAmount)
	{
		return true;
	}
		
	if (InItemInfo->_pcLimitBuyerAmount <= InItemInfo->_purchasedAmount)
	{
		OutReaon = EGsNotAllowedBuyReason::SOLD_OUT;
		return false;
	}

	return true;
}

void FGsNpcShopHelper::ShowTickerNotAllowedBuy(EGsNotAllowedBuyReason InReason)
{
	FText findText;
	switch (InReason)
	{
	case EGsNotAllowedBuyReason::NOT_ENOUGH_COST:
	{
		FText::FindText(TEXT("DepotText"), TEXT("Alert_NoCost"), findText);
	}
	break;
	case EGsNotAllowedBuyReason::NOT_ENOUGH_COST_GOLD:
	{
		FText::FindText(TEXT("NpcShopText"), TEXT("Notice_Gold"), findText);
	}
	break;
	case EGsNotAllowedBuyReason::OVER_INVENTORY_WEIGHT:
	{
		FText::FindText(TEXT("DepotText"), TEXT("Alert_FullWeight"), findText);
	}
	break;
	case EGsNotAllowedBuyReason::OVER_INVENTORY_SLOT:
	{
		FText::FindText(TEXT("DepotText"), TEXT("Alert_FullInventory"), findText);
	}
	break;
	case EGsNotAllowedBuyReason::UNDER_LIMIT_LEVEL:
	{
		FText::FindText(TEXT("NpcShopText"), TEXT("Alert_NotEnoughLevel"), findText);
	}
	break;
	case EGsNotAllowedBuyReason::UNDER_LIMIT_GUILD_LEVEL:
	{

	}
	break;
	case EGsNotAllowedBuyReason::UNDER_LIMIT_GUILD_MEMBER_GRADE:
	{

	}
	break;
	case EGsNotAllowedBuyReason::OVER_LIMIT_AVAILABLE_AMOUNT:
	{

	}
	break;
	case EGsNotAllowedBuyReason::SOLD_OUT:
	{

	}
	break;
	case EGsNotAllowedBuyReason::CURRENCY_ITEM_LOCKED:
	{
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Use_Notice_Locked"), findText);
	}
	break;
	case EGsNotAllowedBuyReason::OTHER_REASON:
	{
		
	}
	break;
	}

	FGsUIHelper::TrayMessageTicker(findText);
}

void FGsNpcShopHelper::ShowTickerNotAllowedSell(EGsNotAllowedSellReaon InReason)
{
	// 별다른 분기 요청이 없어서 일괄적으로 출력
	FText findText;
	FText::FindText(TEXT("NpcShopText"), TEXT("alert_NotForSale"), findText);
	FGsUIHelper::TrayMessageTicker(findText);
}

void FGsNpcShopHelper::SaveAutoBuySettingData(TMap<int32, ItemAmount>& InSettingDataMap)
{
	TArray<ItemIdAmountPair> paidDataList;
	for (const auto& iter : InSettingDataMap)
	{
		paidDataList.Emplace(ItemIdAmountPair(iter.Key, iter.Value));
	}

	FGsNetSendServiceItem::SendOnChangeAutoBuy(paidDataList);
}

void FGsNpcShopHelper::SaveAutoSellConsumeSettingData(TMap<int32, bool>& InSettingDataMap)
{
	TArray<ItemId> itemIdList;
	for (const auto& iter : InSettingDataMap)
	{
		if (true == iter.Value)
		{
			itemIdList.Emplace(iter.Key);
		}
	}

	FGsNetSendServiceItem::SendOnChangeAutoSellConsumable(itemIdList);
}

void FGsNpcShopHelper::SaveAutoSellEquipSettingData(TArray<TPair<bool, TPair<ItemCategoryMain, ItemGrade>>>& InSettingDataList)
{
	TArray<AutoShopSellEquipPair> pairDataList;
	for (int32 i = 0; i < InSettingDataList.Num(); ++i)
	{
		TPair<bool, TPair<ItemCategoryMain, ItemGrade>> pairData = InSettingDataList[i];
		if (true == pairData.Key)
		{
			TPair<ItemCategoryMain, ItemGrade> data = pairData.Value;
			pairDataList.Emplace(AutoShopSellEquipPair(data.Value, data.Key));
		}
	}

	FGsNetSendServiceItem::SendOnChangeAutoSellEquipment(pairDataList);
}

void FGsNpcShopHelper::GetTextByRTT(ResetTimeTicket InRTT, OUT FText& OutText)
{
	OutText = FText::GetEmpty();
	const UGsTableResetTimeTicketConfig* table = Cast<UGsTableResetTimeTicketConfig>(FGsSchemaResetTimeTicketConfig::GetStaticTable());
	if (nullptr == table)
		return;

	const FGsSchemaResetTimeTicketConfig* row = nullptr;
	if (false == table->FindRowById(InRTT, row))
		return;

	FString strCron = row->makeCron;
	if (strCron.IsEmpty())
		return;

	TArray<FString> fields;
	strCron.ParseIntoArray(fields, TEXT(" "));
	
	if (6 != fields.Num())
		return;
		
	int32 tempIndex = 0;
	FText findText;
	if (false == fields[3].FindChar('*', tempIndex))
	{ // 매월
		FText timeText;
		timeText = FText::FromString(fields[3]);
		FText::FindText(TEXT("NpcEventShopText"), TEXT("Purchase_Limit_Reset_Monthly"), findText);
		findText = FText::Format(findText, timeText);
	}
	else if (false == fields[5].FindChar('*', tempIndex))
	{ // 매주
		FString strCronString;
		FGsCronUtil::GetCronString(strCron, strCronString);
		findText = FText::FromString(strCronString.TrimEnd() + TEXT(" 갱신"));
	}
	else
	{ // 매일
		int32 cronHour = FCString::Atoi(*fields[2]);
		FTimespan clientUTC = FDateTime::Now() - FDateTime::UtcNow();
		int32 addHour = clientUTC.GetHours();

		FText timeText = FText::AsNumber((cronHour + addHour) % 24);
		FText::FindText(TEXT("NpcEventShopText"), TEXT("Purchase_Limit_Reset_Daily"), findText);
		findText = FText::Format(findText, timeText);
	}

	OutText = findText;
}
