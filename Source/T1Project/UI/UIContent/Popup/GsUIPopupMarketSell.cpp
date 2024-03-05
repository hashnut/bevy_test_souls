// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupMarketSell.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UILib/Manager/GsTooltipManager.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Common/Item/GsUIItemDefaultInfo.h"
#include "UI/UIContent/Common/Item/GsUIItemDetailInfo.h"
#include "UI/UIContent/Common/Numpad/GsUINumpadDefault.h"
#include "UI/UIContent/Popup/GsUIPopupMarketSellConfirm.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Popup/Market/GsUIMarketPriceInfo.h"
#include "Market/GsMarketData.h"
#include "Market/GsMarketItemData.h"
#include "Market/GsMarketHelper.h"
#include "Currency/GsCurrencyHelper.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "DataSchema/Market/GsSchemaMarketConfig.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Message/MessageParam/GsMarketMessageParam.h"
#include "Net/GsNetSendServiceMarket.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"

//----------------------------------------------------------------------------------------------------------------------
void UGsUIPopupMarketSell::FGsMarketConfigValue::SetConfigValue(const FGsSchemaMarketConfig* InConfig, bool bIsWorldMarket)
{
	if (bIsWorldMarket)
	{
		_registrationFee = InConfig->wdRegistrationFee;
		_minPrice = InConfig->wdMinPrice;
		_maxPrice = InConfig->wdMaxPrice;
		_maxRegistrationCount = InConfig->wdMaxRegistrationCount;
		_maxSettlementCount = InConfig->wdMaxSettlementCount;
	}
	else
	{
		_registrationFee = InConfig->registrationFee;
		_minPrice = InConfig->minPrice;
		_maxPrice = InConfig->maxPrice;
		_maxRegistrationCount = InConfig->maxRegistrationCount;
		_maxSettlementCount = InConfig->maxSettlementCount;
	}
}

int32 UGsUIPopupMarketSell::FGsMarketConfigValue::CalcRegistrationFee(int32 InPrice, bool bIsWorldMarket)
{
	if (bIsWorldMarket)
	{
		// 거래소의 경우 만분율 반올림
		int32 fee = FMath::RoundToInt(InPrice * static_cast<float>(_registrationFee) * 0.0001f);
		if (1 > fee)
		{
			// 1미만이면 1로 보정
			fee = 1;
		}

		return fee;
	}

	return InPrice * _registrationFee;
}
//----------------------------------------------------------------------------------------------------------------------
void UGsUIPopupMarketSell::NativeOnInitialized()
{
	Super::NativeOnInitialized();	
	
	_numPadUI->OnChangeNumber().BindUObject(this, &UGsUIPopupMarketSell::OnChangeNumpadNumber);

	_sBtnSwap->OnClicked.AddDynamic(this, &UGsUIPopupMarketSell::OnClickSwap);
	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupMarketSell::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupMarketSell::OnClickCancel);	
	_toggleSellCount->OnClicked.AddDynamic(this, &UGsUIPopupMarketSell::OnClickSellCount);
	_togglePrice->OnClicked.AddDynamic(this, &UGsUIPopupMarketSell::OnClickPrice);
}

void UGsUIPopupMarketSell::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();

	// 기본적으로 시세정보가 먼저 보이게 처리
	SetUISwapInfoPage(0);
}

void UGsUIPopupMarketSell::NativeDestruct()
{
	Super::NativeDestruct();

	UnbindMessages();

	_textBlockFocused = nullptr;
	_itemDBId = INVALID_ITEM_DB_ID;
	_itemId = INVALID_ITEM_ID;
	_itemAmount = 0;
	_bNeedCheckCloseTooltip = false;
}

void UGsUIPopupMarketSell::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_bNeedCheckCloseTooltip)
	{
		_bNeedCheckCloseTooltip = false;

		// 툴팁 닫기(C2URWQ-7352)
		if (UGsTooltipManager* tooltipMgr = GUI()->GetTooltipManager())
		{
			tooltipMgr->CloseNameTooltip();
		}
	}
}

void UGsUIPopupMarketSell::OnInputOk()
{
	if (INVALID_ITEM_DB_ID == _itemDBId)
	{
		return;
	}

	ItemAmount amount = ConvertTextToNumber(_textBlockSellCount->GetText());
	if (0 == amount)
	{
		// TEXT: 1개 이상 등록해야 합니다
		ShowTicker(TEXT("Ticker_Sell_InvalidAmountMin"));
		return;
	}

	int32 price = ConvertTextToNumber(_textBlockPrice->GetText());
	if (_configValue._minPrice > price)
	{
		// TEXT: 판매 금액은 최소 {0}입니다
		FText textFormat;
		if (FText::FindText(TEXT("MarketText"), TEXT("Ticker_Sell_InvalidPriceMin"), textFormat))
		{
			FText textMsg = FText::Format(textFormat, _configValue._minPrice);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}
		return;
	}
	else if (_configValue._maxPrice < price)
	{
		// TEXT: 등록 최대 금액을 넘어갑니다.
		ShowTicker(TEXT("Ticker_Sell_InvalidPriceMax"));
		return;
	}

	// 수수료 체크
	int32 registrationFee = _configValue.CalcRegistrationFee(price, _bIsWorldMarket);

	if (_bIsWorldMarket)
	{
		// 젠이 충분한지 확인
		if (FGsCurrencyHelper::CheckCurrency(_wdRegistrationFeeType, registrationFee, false))
		{
			// 젠이 잠겨있을 경우
			Currency myAmountLockCheck = FGsCurrencyHelper::GetItemCurrencyAmount(_wdRegistrationFeeType, true);
			if (registrationFee > myAmountLockCheck)
			{
				// TEXT: 젠이 잠금상태로 되어있어 월드 거래소에 판매 물품을 등록할 수 없습니다.
				ShowTicker(TEXT("Ticker_Sell_Fail_ZenLock"));

				return;
			}
		}
		else
		{
			Close(true);

			Currency myAmount = FGsCurrencyHelper::GetItemCurrencyAmount(_wdRegistrationFeeType);
			FGsCurrencyHelper::OpenLackItemPopup(_wdRegistrationFeeType, registrationFee - myAmount);

			return;
		}
	}
	else
	{
		if (false == FGsCurrencyHelper::CheckCurrency(CurrencyType::GOLD, registrationFee, false))
		{
			Close(true);

			Currency myAmount = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GOLD);
			FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType::GOLD, registrationFee - myAmount);

			return;
		}
	}

	FGsMarketControllerBase* controller = GSMarket()->GetController(_bIsWorldMarket);
	if (nullptr == controller)
	{
		return;
	}

	TArray<FGsMarketItemData*> sellItemList = controller->GetSellItemList();
	if (_configValue._maxRegistrationCount <= sellItemList.Num())
	{
		// TEXT: 더 이상 아이템을 판매 등록할 수 없습니다
		ShowTicker(TEXT("Ticker_Sales_Fail_MaxRegistration"));
		return;
	}

	if (_configValue._maxSettlementCount <= GGameData()->GetMarketSettlementCount(_bIsWorldMarket))
	{
		// TEXT: 정산하지 않은 아이템이 가득 차 더 이상 아이템을 판매 등록할 수 없습니다. 정산 후 시도해주세요
		ShowTicker(TEXT("Ticker_Sales_Fail_MaxAdjustment"));
		return;
	}

	TWeakPtr<FGsItem> targetItem = GItem()->FindItem(_itemDBId);
	if (false == targetItem.IsValid())
	{
		return;
	}

	// 에픽 미만은 바로 등록
	if (ItemGrade::EPIC > targetItem.Pin()->GetGradeType())
	{
		// 서버에 등록 요청
		FGsNetSendServiceMarket::SendReqTransactionRegister(_bIsWorldMarket, _itemDBId, amount, price);

		Close();

		return;
	}

	// 에픽 이상은 확인 후 등록
	TWeakObjectPtr<UGsUIWidgetBase> popupWidget = GUI()->OpenAndGetWidget(TEXT("PopupMarketSellConfirm"));
	if (popupWidget.IsValid())
	{
		if (UGsUIPopupMarketSellConfirm* popup = Cast<UGsUIPopupMarketSellConfirm>(popupWidget.Get()))
		{
			popup->SetData(_bIsWorldMarket, _itemDBId, amount, price, registrationFee);

			TWeakObjectPtr<UGsUIPopupMarketSell> weakThis(this);
			ItemAmount amountParam = amount;
			int32 priceParam = price;

			popup->SetCallback([weakThis, amountParam, priceParam](bool bIsOk, ItemDBId InItemDBId)
				{
					if (bIsOk)
					{
						if (weakThis.IsValid())
						{
							if (weakThis->GetItemDBId() == InItemDBId)
							{
								FGsNetSendServiceMarket::SendReqTransactionRegister(weakThis->IsWorldMarket(),
									InItemDBId, amountParam, priceParam);
							}

							weakThis->Close();
						}
					}
				});
		}
	}
}

void UGsUIPopupMarketSell::OnInputCancel()
{
	Close();
}

void UGsUIPopupMarketSell::BindMessages()
{
	MMarket& msgMarket = GMessage()->GetMarket();

	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::RCV_ITEM_PRICE,
		this, &UGsUIPopupMarketSell::OnReceiveItemPriceInfo));
}

void UGsUIPopupMarketSell::UnbindMessages()
{
	MMarket& msgMarket = GMessage()->GetMarket();

	for (MsgHandleMarket& handle : _msgHandleMarketList)
	{
		msgMarket.Remove(handle);
	}
	_msgHandleMarketList.Empty();
}

void UGsUIPopupMarketSell::SetData(bool bIsWorldMarket, ItemDBId InItemDBId)
{
	_bIsWorldMarket = bIsWorldMarket;
	_bNeedCheckCloseTooltip = true;

	TWeakPtr<FGsItem> item = GItem()->FindItem(InItemDBId);
	if (false == item.IsValid())
	{
		Close();
		return;
	}

	const FGsSchemaMarketConfig* config = FGsMarketHelper::GetMarketConfig();
	if (nullptr == config)
	{
		return;
	}

	// 일반/월드 에 맞는 config 정보 입력
	_configValue.SetConfigValue(config, _bIsWorldMarket);

	if (const FGsSchemaItemCommon* itemData = config->wdRegistrationFeeItemId.GetRow())
	{
		_wdRegistrationFeeType = itemData->id;
	}
	else
	{
		_wdRegistrationFeeType = INVALID_ITEM_ID;
	}	

	_itemDBId = InItemDBId;
	_itemId = item.Pin()->GetTID();
	_itemAmount = item.Pin()->GetAmount();

	// 최대 등록개수는 넘지 않도록 설정
	if (MAX_MARKET_ITEM_REGISTER_COUNT < _itemAmount)
	{
		_itemAmount = MAX_MARKET_ITEM_REGISTER_COUNT;
	}

	// 서버에 가격정보 정보요청
	FGsNetSendServiceMarket::SendReqItemPrice(_bIsWorldMarket, _itemId, item.Pin()->GetLevel());
	
	// 가진 개수대로 표시, 최소 판매 금액으로 세팅
	_textBlockSellCount->SetText(FText::AsNumber(_itemAmount));
	_textBlockPrice->SetText(FText::AsNumber(_configValue._minPrice));

	// 개당 판매단가, 등록 수수료 갱신
	InvalidateSellInfo();

	// 1개보다 적으면 금액에 바로 포커스가 가게 함
	SetFocusedNumpad((1 < _itemAmount) ? EGSNumpadFocusedType::SELL_COUNT : EGSNumpadFocusedType::PRICE);

	_defaultInfoUI->SetData(item.Pin().Get(), true);
	_detailInfoUI->SetData(item.Pin().Get());
	_detailInfoUI->SetItemCollectionMarking(item.Pin().Get(), true);

	// 시세정보는 0으로 비우고 패킷 받으면 채움
	_priceInfoUI->SetEmpty();

	// 세금 정보
	_textBlockTax->SetText(FGsCurrencyHelper::GetTextTotalTax(CurrencyType::DIA));
	_textBlockTaxSub->SetText(FGsCurrencyHelper::GetTextTaxInfo(CurrencyType::DIA));
}

void UGsUIPopupMarketSell::SetUISwapInfoPage(int32 InIndex)
{	
	// 0: Price Info, 1: Detail Info
	_switcherInfo->SetActiveWidgetIndex(InIndex);

	// 스왑버튼이므로 위와 반대로 표시된다
	// 0: Detail Info, 1: Price Info
	_sBtnSwap->SetSwitcherIndex(InIndex);
}

void UGsUIPopupMarketSell::InvalidateSellInfo()
{
	_switcherRegistrationFeeType->SetActiveWidgetIndex(_bIsWorldMarket ? 1 : 0);

	int32 amount = ConvertTextToNumber(_textBlockSellCount->GetText());
	int32 price = ConvertTextToNumber(_textBlockPrice->GetText());
	
	// 개당 판매단가
	MarketPrice perPrice = 0.f;
	if (0 < amount && 0 < price)
	{
		perPrice = SharedFunc::GetItemPricePerUnit(static_cast<MarketPrice>(price), amount);
	}

	FText textPerPrice;
	FGsMarketHelper::GetMarketPriceText(perPrice, true, textPerPrice);
	_textBlockPricePerUnit->SetText(textPerPrice);

	// 등록 수수료 충분한지 체크해서 컬러처리
	int32 registrationFee = _configValue.CalcRegistrationFee(price, _bIsWorldMarket);
	if (_bIsWorldMarket)
	{
		if (FGsCurrencyHelper::CheckCurrency(_wdRegistrationFeeType, registrationFee, false))
		{
			_textBlockRegistrationFee->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_ENOUGH));
		}
		else
		{
			_textBlockRegistrationFee->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_LACK));
		}
	}
	else
	{
		if (FGsCurrencyHelper::CheckCurrency(CurrencyType::GOLD, registrationFee, false))
		{
			_textBlockRegistrationFee->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_ENOUGH));
		}
		else
		{
			_textBlockRegistrationFee->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_LACK));
		}
	}

	_textBlockRegistrationFee->SetText(FText::AsNumber(registrationFee));
}

void UGsUIPopupMarketSell::SetFocusedNumpad(EGSNumpadFocusedType InType)
{
	bool bIsPrice = false;

	switch (InType)
	{
	case EGSNumpadFocusedType::SELL_COUNT:
	{
		_numPadUI->SetMaxNumber(_itemAmount);
		_textBlockFocused = _textBlockSellCount;
	}	
	break;
	case EGSNumpadFocusedType::PRICE:
	{
		bIsPrice = true;
		_numPadUI->SetMaxNumber(_configValue._maxPrice);
		_textBlockFocused = _textBlockPrice;
	}
	break;
	default:
	{		
		_numPadUI->SetMaxNumber(0);
		_textBlockFocused = nullptr;
	}
	break;
	}

	if (_textBlockFocused)
	{
		int32 currNum = ConvertTextToNumber(_textBlockFocused->GetText());
		_numPadUI->SetNumber(currNum);
	}

	_toggleSellCount->SetIsSelected(!bIsPrice);
	_togglePrice->SetIsSelected(bIsPrice);
}

int32 UGsUIPopupMarketSell::ConvertTextToNumber(const FText& InText)
{
	if (false == InText.IsEmpty())
	{
		FString strValueTemp = InText.ToString();
		if (false == strValueTemp.IsEmpty())
		{
			// 콤마제거
			FString strValue = strValueTemp.Replace(TEXT(","), TEXT(""));
			return FCString::Atoi(*strValue);
		}
	}

	return 0;
}

void UGsUIPopupMarketSell::OnReceiveItemPriceInfo(const IGsMessageParam*)
{
	FGsMarketControllerBase* controller = GSMarket()->GetController(_bIsWorldMarket);
	if (nullptr == controller)
	{
		return;
	}

	const FGsMarketItemPriceData& priceData = controller->GetItemPriceData();
	if (priceData._itemId == _itemId)
	{
		_priceInfoUI->SetData(priceData);
	}
}

void UGsUIPopupMarketSell::OnChangeNumpadNumber(int32 InNumber)
{
	if (_textBlockFocused)
	{
		_textBlockFocused->SetText(FText::AsNumber(InNumber));
	}

	InvalidateSellInfo();
}

void UGsUIPopupMarketSell::OnClickSwap()
{
	int32 index = (_sBtnSwap->GetSwitcherIndex() + 1) % 2;
	SetUISwapInfoPage(index);
}

void UGsUIPopupMarketSell::OnClickSellCount()
{
	SetFocusedNumpad(EGSNumpadFocusedType::SELL_COUNT);
}

void UGsUIPopupMarketSell::OnClickPrice()
{
	SetFocusedNumpad(EGSNumpadFocusedType::PRICE);
}

void UGsUIPopupMarketSell::OnClickOk()
{
	OnInputOk();
}

void UGsUIPopupMarketSell::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupMarketSell::ShowTicker(const FTextKey& InKey)
{
	FText textMsg;
	if (FText::FindText(TEXT("MarketText"), InKey, textMsg))
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	}	
}
