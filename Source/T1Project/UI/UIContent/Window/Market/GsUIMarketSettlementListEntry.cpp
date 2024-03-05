// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMarketSettlementListEntry.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Market/GsMarketData.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Net/GsNetSendServiceMarket.h"
#include "Currency/GsCurrencyHelper.h"
#include "T1Project.h"


void UGsUIMarketSettlementListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSettlement->OnClicked.AddDynamic(this, &UGsUIMarketSettlementListEntry::OnClickSettlement);
}

void UGsUIMarketSettlementListEntry::NativeDestruct()
{
	_transactionId = 0;

	Super::NativeDestruct();
}

void UGsUIMarketSettlementListEntry::SetData(const FGsMarketSettlementData* InData)
{
	if (nullptr == InData)
	{
		return;
	}

	const FGsSchemaItemCommon* table = InData->GetTable();
	if (nullptr == table)
	{
		return;
	}

	_transactionId = InData->_transactionId;
	_bIsWorldMarket = InData->_bIsWorldMarket;
	_diaEarnings = InData->_earnings;

	// 이름
	SetUIItemName(table->name, InData->_enchantLevel, table->grade);

	// 아이콘
	if (_iconPtr.IsValid())
	{
		_iconPtr->SetNotOwnItem(InData->GetItemId(), InData->_amount, InData->_enchantLevel);
	}

	// 스탯 표시
	SetUIItemStat(_textBlockStat);

	_textBlockPrice->SetText(FText::AsNumber(InData->_price));
	_textBlockEarnings->SetText(FText::AsNumber(InData->_earnings));

	FDateTime dateTimeSale;
	FText textSaleDate;
	FText textSaleTime;
	FGsMarketSettlementData::GetSaleTimeInfo(InData->_timeOfSale, dateTimeSale, textSaleDate, textSaleTime);

	_textBlockSaleDate->SetText(textSaleDate);
	_textBlockSaleTime->SetText(textSaleTime);

//#if WITH_EDITOR
//	// 로그의 시간과 화면의 시간이 동일하게보여야 정상
//	GSLOG(Log, TEXT("%s, time: %s"), *table->name.ToString(), *dateTimeSale.ToString());
//#endif
}

void UGsUIMarketSettlementListEntry::OnClickSettlement()
{
	if (0 == _transactionId)
	{
		return;
	}

	if ((FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::DIA) + _diaEarnings) > MAX_GEM)
	{
		// TEXT: 다이아 최대 보유개수를 초과하여 정산할 수 없습니다
		FText textMsg;
		if (FText::FindText(TEXT("MarketText"), TEXT("Ticker_Adjustment_Fail_MaxDia"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}

		return;
	}

	FGsNetSendServiceMarket::SendReqCalculate(_bIsWorldMarket, false, _transactionId);
}
