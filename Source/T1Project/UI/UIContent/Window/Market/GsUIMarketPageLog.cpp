// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMarketPageLog.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "UI/UIContent/Window/Market/List/GsMarketLogListItem.h"
#include "UI/UIContent/Window/Market/List/GsMarketListViewControlLog.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Market/GsMarketHelper.h"
#include "DataSchema/Market/GsSchemaMarketConfig.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"


void UGsUIMarketPageLog::BeginDestroy()
{
	if (_listViewControl)
	{
		delete _listViewControl;
		_listViewControl = nullptr;
	}

	if (_poolListItem)
	{
		_poolListItem->RemovePool();
		_poolListItem = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIMarketPageLog::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	int32 listItemCountPerPage = 10;
	if (const FGsSchemaMarketConfig* config = FGsMarketHelper::GetMarketConfig())
	{
		listItemCountPerPage = IsWorldMarket() ? config->wdListItemCountPerPage : config->listItemCountPerPage;
	}
	int32 offsetIndex = FMath::FloorToInt(listItemCountPerPage / 2);
	
	// ListItem Pool
	_poolListItem = NewObject<UGsPoolUObject>(this); // UGsMarketLogListItem

	_listViewControl = new FGsMarketListViewControlLog(_listView, offsetIndex, _poolListItem);

	_listView->OnListViewScrolled().AddUObject(this, &UGsUIMarketPageLog::OnListViewScrolled);
	_listView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
}

void UGsUIMarketPageLog::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_bNeedCheckCount)
	{
		FVector2D listSize = _listView->GetCachedGeometry().GetAbsoluteSize();
		if (false == listSize.IsZero())
		{
			const TArray<UUserWidget*> entryList = _listView->GetDisplayedEntryWidgets();
			if (0 < entryList.Num())
			{
				FVector2D entrySize = entryList[0]->GetCachedGeometry().GetAbsoluteSize();

				if (false == entrySize.IsZero())
				{
					if (listSize.Y > (entrySize.Y * _listViewControl->GetDataCount()))
					{
						GSMarket()->SendReqLogList(IsWorldMarket(), false, true);						
					}

					_bNeedCheckCount = false;
				}
			}
		}
	}
}

void UGsUIMarketPageLog::Show(bool bIsWorldMarket)
{
	Super::Show(bIsWorldMarket);

	GSMarket()->SendReqLogList(IsWorldMarket(), true);
	InvalidateList(true);

	_bIsFirst = true;
	_bNeedCheckCount = false;
	_bIsLocked = false;
}

void UGsUIMarketPageLog::Hide()
{
	_bIsFirst = false;
	_bNeedCheckCount = false;
	_bIsLocked = false;
}

void UGsUIMarketPageLog::OnUpdateItemList()
{
	InvalidateList(false);

	// 주의: Search쪽 리스트는 없는 아이템도 포함되며 리스트가 일정 길이 보장이 되는데, 
	// 여기는 10개를 받았을 때 해상도에 따라 스크롤이 안될 수도 있다. 
	// 화면에 리스트 노출개수에 따라 리스트를 다시 요청한다.
	if (_bIsFirst)
	{
		_bNeedCheckCount = true;
		_bIsFirst = false;
	}

	_bIsLocked = false;
}

void UGsUIMarketPageLog::OnRcvItemDetailInfo(MarketTransactionId InTransactionId)
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
			FGsItemDetailOpenMessageParam param(itemData->_item, false, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
			GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
		}
	}
}

void UGsUIMarketPageLog::InvalidateList(bool bResetScroll)
{
	FGsMarketManager* marketMgr = GSMarket();	
	FGsMarketControllerBase* controller = marketMgr->GetController(IsWorldMarket());
	if (nullptr == controller)
	{
		return;
	}

	if (_listViewControl)
	{
		TArray<FGsMarketSettlementData*> logList = controller->GetSettlementLogList();

		_listViewControl->ClearData();

		for (const FGsMarketSettlementData* iter : logList)
		{
			UGsMarketLogListItem* listItem = _poolListItem->Claim<UGsMarketLogListItem>();
			listItem->SetData(iter);

			_listViewControl->SetData(listItem);
		}

		// 해당 스크롤 위치까지 다 받았는지 검사
		if (false == _listViewControl->UpdateList(bResetScroll, logList.Num()))
		{
			// 리스트를 다 받지 않았으면 요청 보냄
			if (false == controller->IsLogListComplete())
			{
				marketMgr->SendReqLogList(IsWorldMarket(), false);
			}
		}

		_listViewControl->RefreshAll();

		_panelEmptyList->SetVisibility(_listViewControl->IsEmptyList() ?
			ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}

	// 총 개수, 총 가격 표시
	_textBlockCount->SetText(FText::AsNumber(controller->GetLogTotalCount()));
	_textBlockTotalPrice->SetText(FText::AsNumber(controller->GetLogTotalPrice()));
}

void UGsUIMarketPageLog::OnListViewScrolled(float InOffsetInItems, float InDistanceRemaining)
{
	if (_listViewControl)
	{
		if (0.f != _prevDistRemaining)
		{
			// 0.1 밑으로 떨어지는 순간을 체크, 아래로 스크롤 일 때만
			if (_prevDistRemaining > InDistanceRemaining)
			{
				int32 prev = FMath::FloorToInt(_prevDistRemaining * 10);
				int32 curr = FMath::FloorToInt(InDistanceRemaining * 10);

				if (0 < prev && 0 == curr)
				{
					GSMarket()->SendReqLogList(IsWorldMarket(), false, true);
					_bIsLocked = true;
				}
			}
		}

		_prevDistRemaining = InDistanceRemaining;
	}
}
