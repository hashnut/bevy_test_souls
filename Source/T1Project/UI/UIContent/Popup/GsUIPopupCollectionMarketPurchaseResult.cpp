// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupCollectionMarketPurchaseResult.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Popup/ItemCollection/GsUICollectionMarketPurchaseListEntry.h"
#include "Market/GsMarketCollectionData.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"


void UGsUIPopupCollectionMarketPurchaseResult::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupCollectionMarketPurchaseResult::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupCollectionMarketPurchaseResult::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupCollectionMarketPurchaseResult::OnClickClose);

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelRootList);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupCollectionMarketPurchaseResult::OnRefreshEntry);
}

void UGsUIPopupCollectionMarketPurchaseResult::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();
}

void UGsUIPopupCollectionMarketPurchaseResult::NativeDestruct()
{
	UnbindMessages();

	_dataList.Empty();

	if (FGsMarketManager* marketMgr = GSMarket())
	{
		marketMgr->ClearPurchaseResult();
	}

	Super::NativeDestruct();
}

void UGsUIPopupCollectionMarketPurchaseResult::OnInputCancel()
{
	Close();
}

void UGsUIPopupCollectionMarketPurchaseResult::BindMessages()
{
	MMarket& msgMarket = GMessage()->GetMarket();

	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::RCV_PURCHASE_RESULT,
		this, &UGsUIPopupCollectionMarketPurchaseResult::OnRcvPurchaseResult));
}

void UGsUIPopupCollectionMarketPurchaseResult::UnbindMessages()
{
	MMarket& msgMarket = GMessage()->GetMarket();

	for (MsgHandleMarket& handle : _msgHandleMarketList)
	{
		msgMarket.Remove(handle);
	}
	_msgHandleMarketList.Empty();
}

void UGsUIPopupCollectionMarketPurchaseResult::SetData(TArray<const FGsMarketCollectionItemData*>& InList)
{
	_scroll->ScrollToStart();

	_dataList.Empty();
	_dataList.Append(InList);

	_slotHelper->RefreshAll(_dataList.Num());

	_textBlockSuccessCount->SetText(FText::AsNumber(0));
	_textBlockFailCount->SetText(FText::AsNumber(0));
	_textBlockTotalPay->SetText(FText::AsNumber(0));

	InvalidatePurchaseResult();
}

void UGsUIPopupCollectionMarketPurchaseResult::InvalidatePurchaseResult()
{
	FGsMarketManager* marketMgr = GSMarket();
	if (nullptr == marketMgr)
	{
		return;
	}

	int32 successCount = 0;
	int32 failCount = 0;
	Currency totalPay = 0;
	
	for (int i = 0; i < _dataList.Num(); ++i)
	{
		const FGsMarketCollectionItemData* itemData = _dataList[i];
		if (nullptr == itemData)
		{
			continue;
		}

		int32 resultValue = marketMgr->GetPurchaseResult(itemData->_bIsWorldMarket, itemData->_transactionId);
		if (0 <= resultValue)
		{
			UGsUICollectionMarketPurchaseListEntry::EGsMarketPurchaseResult resultType;
			bool bIsSuccess = (PACKET_RESULT_SUCCESS == resultValue) ? true : false;

			if (bIsSuccess)
			{
				// 성공
				resultType = UGsUICollectionMarketPurchaseListEntry::EGsMarketPurchaseResult::SUCCESS;
				++successCount;
				totalPay += itemData->_price;
			}
			else
			{
				// 실패
				resultType = UGsUICollectionMarketPurchaseListEntry::EGsMarketPurchaseResult::FAIL;
				++failCount;
			}

			if (UWidget* entry = _slotHelper->GetEntry(i))
			{
				if (UGsUICollectionMarketPurchaseListEntry* itemEntry =
					Cast<UGsUICollectionMarketPurchaseListEntry>(entry))
				{
					itemEntry->SetPurchaseResult(resultType);

					if (false == bIsSuccess)
					{
						itemEntry->SetTextFailResult(resultValue);
					}
				}
			}
		}
	}

	_textBlockSuccessCount->SetText(FText::AsNumber(successCount));
	_textBlockFailCount->SetText(FText::AsNumber(failCount));
	_textBlockTotalPay->SetText(FText::AsNumber(totalPay));	
	
}

void UGsUIPopupCollectionMarketPurchaseResult::OnRcvPurchaseResult(const IGsMessageParam* InParam)
{	
	InvalidatePurchaseResult();
}

void UGsUIPopupCollectionMarketPurchaseResult::OnRefreshEntry(int32 InIndex, class UWidget* InEntry)
{
	if (false == _dataList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUICollectionMarketPurchaseListEntry* itemEntry =
		Cast<UGsUICollectionMarketPurchaseListEntry>(InEntry))
	{
		itemEntry->SetData(_dataList[InIndex], true);
	}
}

void UGsUIPopupCollectionMarketPurchaseResult::OnClickClose()
{
	OnInputCancel();

	// 갱신 메시지 보냄
	GMessage()->GetMarket().SendMessage(MessageContentMarket::REQ_REFRESH_COLLECTION, nullptr);
}
