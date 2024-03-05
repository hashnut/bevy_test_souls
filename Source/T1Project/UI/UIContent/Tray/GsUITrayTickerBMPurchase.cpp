// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Tray/GsUITrayTickerBMPurchase.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"



void UGsUITrayTickerBMPurchase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_finishDelegate.BindDynamic(this, &UGsUITrayTickerBMPurchase::OnAnimationFinish);
	BindToAnimationFinished(_anim, _finishDelegate);
}

void UGsUITrayTickerBMPurchase::NativeConstruct()
{
	Super::NativeConstruct();

}

void UGsUITrayTickerBMPurchase::SetData(TPair<FGsBMShopProductData*, ItemAmount> InPairData, bool InIsBulkSale, bool InIsCash)
{
	FText findTitle;
	FText::FindText(TEXT("BMShopText"), TEXT("Ticker_TextTitle"), findTitle);
	_titleText->SetText(findTitle);

	if (InIsBulkSale)
	{
		// 1. 일괄 구매 시(IsBulkSale = TRUE)
		// ㄴ Ticker_TextTitle : 상점
		// ㄴ Ticker_BulkSale : 일괄 구매 완료
		// ㄴ Ticker_GetInventory : 가방 및 보유 재화를 확인하세요
		FText findDesc1;
		FText findDesc2;
		FText::FindText(TEXT("BMShopText"), TEXT("Ticker_BulkSale"), findDesc1);
		FText::FindText(TEXT("BMShopText"), TEXT("Ticker_GetInventory"), findDesc2);
		_productNameText->SetText(findDesc1);
		_descText->SetText(findDesc2);
	}
	else
	{
		FGsBMShopProductData* data = InPairData.Key;
		ItemAmount amount = InPairData.Value;
		if (nullptr != data && 0 < amount)
		{
			if (InIsCash)
			{
				FText findDesc1;
				FText findDesc2;
				FText resultText;
				if (1 < amount)
				{
					FText::FindText(TEXT("BMShopText"), TEXT("Ticker_ProductName_Count"), findDesc1);
					resultText = FText::Format(findDesc1, data->GetName(), amount);
				}
				else
				{
					FText::FindText(TEXT("BMShopText"), TEXT("Ticker_ProductName"), findDesc1);
					resultText = FText::Format(findDesc1, data->GetName());
				}
				FText::FindText(TEXT("BMShopText"), TEXT("Ticker_GetStorageBox"), findDesc2);

				_productNameText->SetText(resultText);
				_descText->SetText(findDesc2);
			}
			else
			{
				FText findDesc1;
				FText findDesc2;
				FText resultText;
				if (1 < amount)
				{
					FText::FindText(TEXT("BMShopText"), TEXT("Ticker_ProductName_Count"), findDesc1);
					resultText = FText::Format(findDesc1, data->GetName(), amount);
				}
				else
				{
					FText::FindText(TEXT("BMShopText"), TEXT("Ticker_ProductName"), findDesc1);
					resultText = FText::Format(findDesc1, data->GetName());
				}
				FText::FindText(TEXT("BMShopText"), TEXT("Ticker_GetInventory"), findDesc2);

				_productNameText->SetText(resultText);
				_descText->SetText(findDesc2);
			}
		}
	}

	PlayAnimation(_anim);
}

void UGsUITrayTickerBMPurchase::OnAnimationFinish()
{
	Close(true);
}


/*

2 - 1. 1개 구매 - 보관함 이동(패키지 구매)
ㄴ Ticker_TextTitle : 상점
ㄴ Ticker_ProductName : {0}
ㄴ Ticker_GetStorageBox : 보관함을 확인하세요

2 - 3. N개 이상 구매 - 보관함 이동(패키지 구매)
ㄴ Ticker_TextTitle : 상점
ㄴ Ticker_ProductName : {0} + "n개"
ㄴ Ticker_GetStorageBox : 보관함을 확인하세요




2 - 2. 1개 구매 - 인벤토리 이동(HC, SC 상품 구매)
ㄴ Ticker_TextTitle : 상점
ㄴ Ticker_ProductName : {0}
ㄴ Ticker_GetInventory : 가방 및 보유 재화를 확인하세요

2 - 4.  N개 이상 구매 - 보관함 이동(HC, SC 상품 구매)
ㄴ Ticker_TextTitle : 상점
ㄴ Ticker_ProductName : {0} + "n개"
ㄴ Ticker_GetInventory : 가방 및 보유 재화를 확인하세요

*/