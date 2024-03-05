// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMarketPageSettlement.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/Market/GsUIMarketSettlementListEntry.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "Market/GsMarketData.h"
#include "Market/GsMarketHelper.h"
#include "DataSchema/Market/GsSchemaMarketConfig.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Net/GsNetSendServiceMarket.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"


void UGsUIMarketPageSettlement::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIMarketPageSettlement::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIMarketPageSettlement::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelList);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIMarketPageSettlement::OnRefreshEntry);
	_slotHelper->SetEmptyListPanel(_panelEmptyList);

	_btnSettlementAll->OnClicked.AddDynamic(this, &UGsUIMarketPageSettlement::OnClickSettlementAll);
}

void UGsUIMarketPageSettlement::Show(bool bIsWorldMarket)
{
	Super::Show(bIsWorldMarket);

	GSMarket()->SendReqSettlementList(IsWorldMarket());

	UpdateItemList();
	
	if (const FGsSchemaMarketConfig* config = FGsMarketHelper::GetMarketConfig())
	{
		int32 maxSettlementCount = IsWorldMarket() ? config->wdMaxSettlementCount : config->maxSettlementCount;
		_textBlockMaxCount->SetText(FText::AsNumber(maxSettlementCount));
	}
}

void UGsUIMarketPageSettlement::Hide()
{
	GGameData()->SetMarketSettlementCount(IsWorldMarket(), _dataList.Num());
}

void UGsUIMarketPageSettlement::OnUpdateItemList()
{
	UpdateItemList();
}

void UGsUIMarketPageSettlement::OnRcvItemDetailInfo(MarketTransactionId InTransactionId)
{
	FGsMarketControllerBase* controller = GSMarket()->GetController(IsWorldMarket());
	if (nullptr == controller)
	{
		return;
	}


	if (const FGsMarketSettlementData* itemData = controller->GetSettlementData(InTransactionId))
	{
		if (itemData->_item)
		{
			FGsItemDetailOpenMessageParam param(itemData->_item, false, false , UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
			GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
		}
	}
}

void UGsUIMarketPageSettlement::UpdateItemList()
{
	FGsMarketControllerBase* controller = GSMarket()->GetController(IsWorldMarket());
	if (nullptr == controller)
	{
		return;
	}

	_dataList.Empty();
	TArray<FGsMarketSettlementData*> tempList = controller->GetSettlementList();
	_dataList.Append(tempList);

	// 최근 날짜 순으로 소팅
	_dataList.Sort([](FGsMarketSettlementData& A, FGsMarketSettlementData& B)
		{
			return (A._timeOfSale >= B._timeOfSale) ? true : false;
		});

	_slotHelper->RefreshAll(_dataList.Num());

	// 판매개수
	_textBlockCount->SetText(FText::AsNumber(_dataList.Num()));

	// 판매총액
	int32 totalEarnings = 0;
	for (FGsMarketSettlementData* data : _dataList)
	{
		totalEarnings += data->_earnings;
	}
	_textBlockTotalPrice->SetText(FText::AsNumber(totalEarnings));

	// 정산 내역이 없으면 버튼 비활성화
	_btnSettlementAll->SetIsEnabled((0 < _dataList.Num()) ? true : false);
}

void UGsUIMarketPageSettlement::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (_dataList.IsValidIndex(InIndex))
	{
		if (UGsUIMarketSettlementListEntry* entry = Cast<UGsUIMarketSettlementListEntry>(InEntry))
		{
			entry->SetData(_dataList[InIndex]);
		}
	}
}

void UGsUIMarketPageSettlement::OnClickSettlementAll()
{
	FGsNetSendServiceMarket::SendReqCalculate(IsWorldMarket(), true, INVALID_MARKET_TRANSACTION_ID);
}
