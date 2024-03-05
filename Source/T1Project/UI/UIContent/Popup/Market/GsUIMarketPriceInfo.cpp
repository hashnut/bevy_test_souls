// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMarketPriceInfo.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Market/GsMarketData.h"
#include "Market/GsMarketItemGroupData.h"
#include "Market/GsMarketHelper.h"
#include "Management/ScopeGame/GsMarketManager.h"


void UGsUIMarketPriceInfo::SetData(const FGsMarketItemPriceData& InData)
{
	FGsMarketControllerBase* controller = GSMarket()->GetController(InData._bIsWorldMarket);

	_switcherServer->SetActiveWidgetIndex(InData._bIsWorldMarket ? 1 : 0);

	bool bIsStackable = false;
	if (const FGsMarketItemGroupData* itemGroup = controller->GetItemGroupData(InData._itemId, false))
	{
		bIsStackable = itemGroup->IsStackableItem();
	}

	SetTextTarget(InData._lowestPrice, bIsStackable, _textBlockLowestPrice);
	SetTextTarget(InData._heighestPrice, bIsStackable, _textBlockHighestPrice);
	SetTextTarget(InData._averagePrice, bIsStackable, _textBlockAveragePrice);
	SetTextTarget(InData._latestPrice, bIsStackable, _textBlockLatestPrice);
	SetTextTarget(InData._lowestPriceCurrent, bIsStackable, _textBlockLowestPriceCurrent);
}

void UGsUIMarketPriceInfo::SetEmpty()
{
	FText textZero = FText::AsNumber(0);

	_textBlockLowestPrice->SetText(textZero);
	_textBlockHighestPrice->SetText(textZero);
	_textBlockAveragePrice->SetText(textZero);
	_textBlockLatestPrice->SetText(textZero);
	_textBlockLowestPriceCurrent->SetText(textZero);
}

void UGsUIMarketPriceInfo::SetTextTarget(MarketPrice InPrice, bool bIsFractionalDigits, OUT UTextBlock* OutTextBlock)
{
	FText textPrice;
	FGsMarketHelper::GetMarketPriceText(InPrice, bIsFractionalDigits, textPrice);

	OutTextBlock->SetText(textPrice);
}
