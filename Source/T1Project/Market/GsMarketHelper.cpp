// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMarketHelper.h"
#include "DataSchema/Market/GsSchemaMarketConfig.h"
#include "DataSchema/Market/GsSchemaMarketFilterSetting.h"
#include "DataSchema/Market/GsSchemaMarketCategoryData.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Item/GsItemManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "T1Project.h"


const FGsSchemaMarketConfig* FGsMarketHelper::GetMarketConfig()
{
	// 1개 열밖에 없음.
	const FGsSchemaMarketConfig* row = nullptr;
	if (const UGsTable* table = FGsSchemaMarketConfig::GetStaticTable())
	{
		if (table->FindRow(TEXT("Default"), row))
		{
			return row;
		}
	}

#if WITH_EDITOR
	GSLOG(Error, TEXT("[Market] Fail to get FGsSchemaMarketConfig."));
#endif
	return nullptr;
}

const FGsSchemaMarketFilterSetting* FGsMarketHelper::GetMarketFilterSetting()
{
	// 1개 열밖에 없음.
	const FGsSchemaMarketFilterSetting* row = nullptr;
	if (const UGsTable* table = FGsSchemaMarketFilterSetting::GetStaticTable())
	{
		if (table->FindRow(TEXT("Default"), row))
		{
			return row;
		}
	}

#if WITH_EDITOR
	GSLOG(Error, TEXT("[Market] Fail to get FGsSchemaMarketFilterSetting."));
#endif
	return nullptr;
}

const FGsSchemaMarketCategoryData* FGsMarketHelper::GetMarketCategoryData(EGsMarketUICategory InCategory)
{
	const UGsTableMarketCategoryData* table = Cast<const UGsTableMarketCategoryData>(
		FGsSchemaMarketCategoryData::GetStaticTable());
	if (table)
	{
		const FGsSchemaMarketCategoryData* row = nullptr;
		if (table->FindRowById(InCategory, row))
		{
			return row;
		}
	}

	return nullptr;
}

bool FGsMarketHelper::GetSellRemainTime(bool bIsWorldMarket, const time_t& InSellStartTime, OUT FTimespan& OutRemainTime)
{
	// 판매 시작 시간이 없으면 판매 대기중
	if (0 < InSellStartTime)
	{
		if (const FGsSchemaMarketConfig* config = GetMarketConfig())
		{
			int32 sellPeriodH = bIsWorldMarket ? config->wdSellPeriodH : config->sellPeriodH;

			FTimespan sellPeriod = FTimespan::FromHours(sellPeriodH);
			FDateTime startTime = FGsTimeSyncUtil::ConvertToDateTime(InSellStartTime);
			FDateTime serverNow = FGsTimeSyncUtil::GetServerNowDateTime();

			// 판매 남은 시간 = 판매 시작 시간 + 판매 기간 - 서버 현재 시간
			OutRemainTime = startTime + sellPeriod - serverNow;
#if WITH_EDITOR
			GSLOG(Log, TEXT("[Market] selllStartTime: %s, serverNow: %s, remainTime: %s"),
				*startTime.ToString(), *serverNow.ToString(), *OutRemainTime.ToString());
#endif
			return true;
		}
	}

	return true;
}

void FGsMarketHelper::GetMarketPriceText(double InPrice, bool bIsFractionalDigits, OUT FText& OutText)
{
	if (bIsFractionalDigits)
	{
		// 부동소수점 불확실성 문제로(300.10000000000002 이런 값) 올림 연산 시 문제되어
		// 소수점 4째자리 이하는 제거하고 연산
		double tempNum = FMath::FloorToDouble(InPrice * 1000);

		// 소수점 3째자리에서 올림처리
		tempNum = tempNum * 0.1;
		tempNum = FMath::CeilToDouble(tempNum);

		// 소수점 2째자리까지 표기
		tempNum *= 0.01;		
		FGsUIStringHelper::GetFormatedFractionalDigitsTextD(tempNum, 0, 2, OutText);
	}
	else
	{
		FGsUIStringHelper::GetFormatedFractionalDigitsTextD(InPrice, 0, 0, OutText);
	}
}

bool FGsMarketHelper::CheckInventoryState(int32 InWeight, bool bUseMessage)
{
	UGsItemManager* itemMgr = GItem();

	// 주의: 슬롯이 꽉차있어도 쌓을 수 있는 아이템이 구매 여유가 있으면 가능함. 체크로직 삭제
	//if (itemMgr->IsInvenSlotFull())
	//{
	//	if (bUseMessage)
	//	{
	//		// TEXT: 가방이 가득 찼습니다
	//		FText textMsg;
	//		if (FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), textMsg))
	//		{
	//			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	//		}
	//	}		

	//	return false;
	//}

	if (itemMgr->IsInvenWeightFull(InWeight))
	{
		if (bUseMessage)
		{
			// TEXT: 소지 무게가 최대치입니다.
			FText textMsg;
			if (FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), textMsg))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}
		}		

		return false;
	}

	return true;
}

ItemId FGsMarketHelper::GetWorldMarketRegistrationFeeItemId()
{
	if (const FGsSchemaMarketConfig* config = GetMarketConfig())
	{
		if (const FGsSchemaItemCommon* itemData = config->wdRegistrationFeeItemId.GetRow())
		{
			return itemData->id;
		}
	}

	return INVALID_ITEM_ID;
}

ItemGrade FGsMarketHelper::GetWorldMarketMinimumItemGrade()
{
	if (const FGsSchemaMarketConfig* config = GetMarketConfig())
	{
		return config->wdMinimumItemGrade;
	}

	return ItemGrade::NONE;
}

ItemGrade FGsMarketHelper::GetWorldMarketShortcutItemGrade()
{
	if (const FGsSchemaMarketConfig* config = GetMarketConfig())
	{
		return config->WorldMarketShortcutGrade;
	}

	return ItemGrade::NONE;
}

void FGsMarketHelper::OpenMarketFromCollection(CollectionId InCollectionId, ItemId InItemId)
{
	if (FGsMarketManager* marketMgr = GSMarket())
	{
		if (FGsMarketCollectionData* marketData = marketMgr->GetCollectionData(InCollectionId))
		{
			// 다른 레벨 아이템 유무 감지한 정보가 있을경우 해당 거래소로 보냄
			bool bIsWorldMarket = false;
			if (marketData->GetOtherLevelMarket(InItemId, bIsWorldMarket))
			{
				FGsItemDetailOpenMarketParam param(InItemId, bIsWorldMarket ?
					FGsItemDetailOpenMarketParam::EGsMarketSearchOption::WORLD_SERVER :
					FGsItemDetailOpenMarketParam::EGsMarketSearchOption::NORMAL_SERVER);

				GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_OPEN_MARKET, &param);

				return;
			}
		}
	}

	FGsItemDetailOpenMarketParam param(InItemId, FGsItemDetailOpenMarketParam::EGsMarketSearchOption::DEFAULT);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_OPEN_MARKET, &param);
}
