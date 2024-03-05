// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMarketSellListEntry.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "DataSchema/Market/GsSchemaMarketConfig.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Market/GsMarketItemData.h"
#include "Market/GsMarketHelper.h"
#include "Net/GsNetSendServiceMarket.h"
#include "Item/GsItemManager.h"


void UGsUIMarketSellListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 두 버튼 다 동작 같음
	_sBtnCancel->OnClicked.AddDynamic(this, &UGsUIMarketSellListEntry::OnClickCancel);
}

void UGsUIMarketSellListEntry::NativeDestruct()
{
	_transactionId = INVALID_MARKET_TRANSACTION_ID;
	_sellStartTime = 0;
	_currState = EGsSellStateType::Waiting;
	_stateCheckRemainTime = 0.f;
	_bIsEquipType = false;
	_itemWeight = 0;
	_bIsStackable = false;
	_registeredServerTime = 0;

	Super::NativeDestruct();
}

void UGsUIMarketSellListEntry::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 판매중일 경우 주기적으로 시간갱신 (현재 60초)
	if (EGsSellStateType::OnSale == _currState)
	{
		_stateCheckRemainTime -= InDeltaTime;
		if (0 > _stateCheckRemainTime)
		{
			UpdateState();
		}
	}
}

void UGsUIMarketSellListEntry::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	// equip타입이면 상세정보 받고 각 페이지에서 열기
	if (_bIsEquipType)
	{
		FGsNetSendServiceMarket::SendReqTransactionDetailEquip(_bIsWorldMarket, _transactionId);
	}
	else
	{
		Super::OnLongPressIcon(InIcon);
	}
}

void UGsUIMarketSellListEntry::SetData(bool bIsWorldMarket, FGsMarketItemData* InData)
{
	_bIsWorldMarket = bIsWorldMarket;

	if (nullptr == InData ||
		nullptr == InData->_item)
	{
		return;
	}

	const FGsSchemaItemCommon* table = InData->GetTable();
	if (nullptr == table)
	{
		return;
	}

	_transactionId = InData->_transactionId;
	_sellStartTime = InData->_sellStartTime;
	_bIsEquipType = (ItemType::EQUIP == table->type) ? true : false;
	_itemWeight = InData->_item->GetWeight();
	_bIsStackable = (1 < table->maxStackCount) ? true : false;
	_registeredServerTime = InData->_registeredServerTime;

	// 이름
	SetUIItemName(table->name, InData->GetEnchantLevel(), table->grade);

	// 아이콘
	if (_iconPtr.IsValid())
	{
		_iconPtr->SetNotOwnItem(InData->GetItemId(), InData->GetAmount(), InData->GetEnchantLevel());
		_iconPtr->SetSlotEffectMagicalForge(InData->_item);
	}

	// 스탯 표시
	SetUIItemStat(_textBlockStat);

	// 개당 판매금액 소수점 4째자리까지 표기
	FText textPricePerUnit;
	FGsMarketHelper::GetMarketPriceText(InData->GetPricePerUnit(), true, textPricePerUnit);
	_textBlockPricePerUnit->SetText(textPricePerUnit);

	// 판매 금액
	_textBlockPrice->SetText(FText::AsNumber(InData->_price));

	UpdateState();
}

void UGsUIMarketSellListEntry::OnClickCancel()
{
	// 회수/취소 동작 둘다 같음
	if (0 == _transactionId)
	{
		return;
	}

	// 월드 거래소는 처리에 시간이 걸리므로 3초 쿨타임을 준다(빠르게 등록 취소 반복하는 것을 방지)
	if (_bIsWorldMarket)
	{
		if (0 < _registeredServerTime)
		{
			time_t diff = FGsTimeSyncUtil::GetServerNowSecond() - _registeredServerTime;
			if (3 > diff)
			{
				// TEXT: 잠시 후 다시 시도해주세요
				FText textMsg;
				if (FText::FindText(TEXT("MarketText"), TEXT("Ticker_Sales_Fail_Register"), textMsg))
				{
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
				}

				return;
			}
		}
	}	

	// 각종 예외사항 체크
	UGsItemManager* itemMgr = GItem();

	// 스택 가능한 아이템의 경우 합쳐질 수 있기 때문에, 일단 보내보고 에러 메시지를 처리
	if (false == _bIsStackable)
	{
		if (itemMgr->IsInvenSlotFull())
		{
			// TEXT: 가방이 가득 찼습니다
			FText textMsg;
			if (FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), textMsg))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}

			return;
		}
	}

	if (itemMgr->IsInvenWeightFull(_itemWeight))
	{
		// TEXT: 소지 무게가 최대치입니다.
		FText textMsg;
		if (FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}

		return;
	}

	FGsNetSendServiceMarket::SendReqTransactionCancel(_bIsWorldMarket, _transactionId);
}

void UGsUIMarketSellListEntry::UpdateState()
{
	// salesTime : 판매 시작 시간
	// salesTime 0 일 경우 -> 판매 대기중 노출
	// salesTime 0 보다 클 경우 -> 판매 남은 시간 있을으면 판매중, 없으면 판매실패

	FText textState;
	int32 cancelBtnIndex = 0; // 0: 판매취소, 1: 회수

	if (0 == _sellStartTime)
	{
		_currState = EGsSellStateType::Waiting;

		// TEXT: 판매 준비중
		FText::FindText(TEXT("MarketText"), TEXT("SellState_Waiting"), textState);
	}
	else
	{
		FTimespan remainTime;
		FGsMarketHelper::GetSellRemainTime(_bIsWorldMarket, _sellStartTime, remainTime);

		if (0 < remainTime.GetTotalSeconds())
		{
			_currState = EGsSellStateType::OnSale;

			// 남은 시간 표시
			FString strTime;
			FGsTimeStringHelper::GetTimeStringCeil(remainTime, strTime);
			textState = FText::FromString(strTime);
		}
		else
		{
			_currState = EGsSellStateType::SaleFailed;

			// TEXT: 판매 실패
			FText::FindText(TEXT("MarketText"), TEXT("SellState_SaleFailed"), textState);

			cancelBtnIndex = 1;
		}
	}

	_textBlockStateText->SetText(textState);
	_sBtnCancel->SetSwitcherIndex(cancelBtnIndex); // 0: 판매취소, 1: 회수

	_stateCheckRemainTime = (EGsSellStateType::OnSale == _currState) ? REFRESH_TIME_DURATION_SEC : 0.f;
}
