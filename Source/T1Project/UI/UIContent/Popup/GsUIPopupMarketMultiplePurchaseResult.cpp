// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupMarketMultiplePurchaseResult.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Popup/Market/GsUIMarketMultiplePurchaseListEntry.h"
#include "Market/GsMarketItemData.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"


void UGsUIPopupMarketMultiplePurchaseResult::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupMarketMultiplePurchaseResult::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupMarketMultiplePurchaseResult::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupMarketMultiplePurchaseResult::OnClickClose);

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelRootList);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupMarketMultiplePurchaseResult::OnRefreshEntry);
}

void UGsUIPopupMarketMultiplePurchaseResult::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();
}

void UGsUIPopupMarketMultiplePurchaseResult::NativeDestruct()
{
	UnbindMessages();

	_tempDataList.Empty();
	_idList.Empty();
	_priceMap.Empty();

	if (FGsMarketManager* marketMgr = GSMarket())
	{
		marketMgr->ClearPurchaseResult();
	}

	Super::NativeDestruct();
}

void UGsUIPopupMarketMultiplePurchaseResult::OnInputCancel()
{
	Close();
}

void UGsUIPopupMarketMultiplePurchaseResult::BindMessages()
{
	MMarket& msgMarket = GMessage()->GetMarket();

	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::RCV_PURCHASE_RESULT,
		this, &UGsUIPopupMarketMultiplePurchaseResult::OnRcvPurchaseResult));
}

void UGsUIPopupMarketMultiplePurchaseResult::UnbindMessages()
{
	MMarket& msgMarket = GMessage()->GetMarket();

	for (MsgHandleMarket& handle : _msgHandleMarketList)
	{
		msgMarket.Remove(handle);
	}
	_msgHandleMarketList.Empty();
}

void UGsUIPopupMarketMultiplePurchaseResult::SetData(bool bIsWorldMarket, TArray<MarketTransactionId>& InIdList)
{
	_bIsWorldMarket = bIsWorldMarket;
	_idList.Empty();
	_tempDataList.Empty();
	_priceMap.Empty();

	if (0 == InIdList.Num())
	{
		return;
	}

	FGsMarketManager* marketMgr = GSMarket();
	if (nullptr == marketMgr)
	{
		return;
	}

	FGsMarketControllerBase* controller = marketMgr->GetController(_bIsWorldMarket);
	if (nullptr == controller)
	{
		return;
	}

	_scroll->ScrollToStart();

	_idList.Append(InIdList);

	// 데이터 채우기
	for (int i = 0; i < _idList.Num(); ++i)
	{
		if (const FGsMarketItemData* itemData = controller->GetItemData(_idList[i]))
		{
			_tempDataList.Emplace(itemData);

			_priceMap.Emplace(itemData->_transactionId, itemData->_price);
		}
	}

	_slotHelper->RefreshAll(_tempDataList.Num());

	_textBlockSuccessCount->SetText(FText::AsNumber(0));
	_textBlockFailCount->SetText(FText::AsNumber(0));
	_textBlockTotalPay->SetText(FText::AsNumber(0));

	InvalidatePurchaseResult();
	
	// 슬롯 그릴때만 사용하고 지움
	_tempDataList.Empty();
}

void UGsUIPopupMarketMultiplePurchaseResult::InvalidatePurchaseResult()
{
	FGsMarketManager* marketMgr = GSMarket();
	if (nullptr == marketMgr)
	{
		return;
	}

	int32 successCount = 0;
	int32 failCount = 0;
	Currency totalPay = 0;
	
	for (int i = 0; i < _idList.Num(); ++i)
	{
		MarketTransactionId transactionId = _idList[i];
		
		int32 resultValue = marketMgr->GetPurchaseResult(_bIsWorldMarket, transactionId);

		if (0 <= resultValue)
		{
			UGsUIMarketMultiplePurchaseListEntry::EGsMarketPurchaseResult resultType;
			bool bIsSuccess = (PACKET_RESULT_SUCCESS == resultValue) ? true : false;

			if (bIsSuccess)
			{
				// 성공
				resultType = UGsUIMarketMultiplePurchaseListEntry::EGsMarketPurchaseResult::SUCCESS;
				++successCount;

				if (_priceMap.Contains(transactionId))
				{
					totalPay += _priceMap[transactionId];
				}
			}
			else
			{
				// 실패
				resultType = UGsUIMarketMultiplePurchaseListEntry::EGsMarketPurchaseResult::FAIL;
				++failCount;
			}

			if (UWidget* entry = _slotHelper->GetEntry(i))
			{
				if (UGsUIMarketMultiplePurchaseListEntry* itemEntry =
					Cast<UGsUIMarketMultiplePurchaseListEntry>(entry))
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

void UGsUIPopupMarketMultiplePurchaseResult::OnRcvPurchaseResult(const IGsMessageParam* InParam)
{	
	InvalidatePurchaseResult();
}

void UGsUIPopupMarketMultiplePurchaseResult::OnRefreshEntry(int32 InIndex, class UWidget* InEntry)
{
	if (false == _tempDataList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIMarketMultiplePurchaseListEntry* itemEntry =
		Cast<UGsUIMarketMultiplePurchaseListEntry>(InEntry))
	{
		itemEntry->SetData(_tempDataList[InIndex], true);
	}
}

void UGsUIPopupMarketMultiplePurchaseResult::OnClickClose()
{
	OnInputCancel();

	// 갱신 메시지 보냄
	GMessage()->GetMarket().SendMessage(MessageContentMarket::REQ_REFRESH_COLLECTION, nullptr);
}
