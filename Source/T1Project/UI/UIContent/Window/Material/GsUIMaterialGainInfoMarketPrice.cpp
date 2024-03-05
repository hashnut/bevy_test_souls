// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMaterialGainInfoMarketPrice.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Popup/Market/GsUIMarketPriceInfo.h"
#include "UI/UIContent/Popup/GsUIPopupCollectionMarketPurchase.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Message/MessageParam/GsMarketMessageParam.h"
#include "Market/GsMarketCollectionData.h"
#include "Net/GsNetSendServiceMarket.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "UTIL/GsTableUtil.h"


void UGsUIMaterialGainInfoMarketPrice::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_sBtnPurchase->OnClicked.AddDynamic(this, &UGsUIMaterialGainInfoMarketPrice::OnClickPurchase);
}

void UGsUIMaterialGainInfoMarketPrice::SetData(CollectionId InCollectionId, ItemId InMaterialId, Level InLevel)
{
	_collectionId = InCollectionId;
	_itemId = InMaterialId;
	_enchantLevel = InLevel;
	_isRequestLowestPrice = false;
	
	// 비우고 패킷 받은 후 갱신한다
	_uiPriceInfo->SetEmpty();
	_sBtnPurchase->SetSwitcherIndex(0);
	_sBtnPurchase->SetIsEnabled(false);

	bool bIsTradable = IsTradable(_itemId);
	if (bIsTradable)
	{
		SendRequestLowestPrice();
	}	

	SetUITradable(bIsTradable);
}

void UGsUIMaterialGainInfoMarketPrice::SendRequestLowestPrice()
{
	FGsMarketManager* markgetMgr = GSMarket();
	if (nullptr == markgetMgr)
	{
		return;
	}

	FGsMarketCollectionData* marketData = markgetMgr->CreateOrGetCollectionData(_collectionId);
	if (nullptr == marketData)
	{
		return;
	}

	// 광클 방지 로직
	if (false == marketData->IsCoolTime(true))
	{
		TArray<FGsMarketLowestPriceRequestInfo> reqList;
		reqList.Emplace(FGsMarketLowestPriceRequestInfo(_itemId, _enchantLevel, 1));

		// 주의: 여기선 모든 정보를 받는게 아니므로 광클 방지 쿨타임 갱신은 하지 않았음
		FGsNetSendServiceMarket::SendReqCollectionItemMarketPriceLowest(true, _collectionId, reqList);
		_isRequestLowestPrice = true;
	}
	else
	{
		CheckAndSendRequestPrice(marketData);
	}
}

void UGsUIMaterialGainInfoMarketPrice::SendRequestPrice(bool bIsWorldMarket)
{
	_isRequestWorldMarket = bIsWorldMarket;
	FGsNetSendServiceMarket::SendReqItemPrice(bIsWorldMarket, _itemId, _enchantLevel);
}

void UGsUIMaterialGainInfoMarketPrice::CheckAndSendRequestPrice(FGsMarketCollectionData* InCollectionData)
{
	if (nullptr == InCollectionData)
	{
		return;
	}

	if (false == IsTradable(_itemId))
	{
		return;
	}

	bool bIsExist = false;
	bool bIsWorldMarket = false;

	TPair<ItemId, Level> keyPair(_itemId, _enchantLevel);
	if (const FGsMarketCollectionItemData* itemData = InCollectionData->GetLowestPricePerUnitItem(keyPair))
	{
		bIsWorldMarket = itemData->_bIsWorldMarket;
		bIsExist = true;
	}

	SendRequestPrice(bIsWorldMarket);

	_sBtnPurchase->SetIsEnabled(bIsExist);
	_sBtnPurchase->SetSwitcherIndex(bIsWorldMarket ? 1 : 0);

	_textBlockState->SetVisibility(InCollectionData->IsOtherLevelExist(_itemId) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIMaterialGainInfoMarketPrice::SetUITradable(bool bIsTradable)
{
	_switcherTradable->SetActiveWidgetIndex(bIsTradable ? 0 : 1); 
}

bool UGsUIMaterialGainInfoMarketPrice::IsTradable(ItemId InItemId) const
{
	if (const FGsSchemaItemCommon* row = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(InItemId))
	{
		return row->tradable;
	}

	return false;
}

void UGsUIMaterialGainInfoMarketPrice::OpenPurchasePopup()
{
	// 이미 열려있으면 무시
	if (GUI()->IsActiveWidget(TEXT("PopupCollectionMarketPurchase")))
	{
		return;
	}

	FGsMarketCollectionData* marketData = GSMarket()->CreateOrGetCollectionData(_collectionId);
	if (nullptr == marketData)
	{
		return;
	}
	
	TArray<TPair<ItemId, Level>> keyList;
	keyList.Emplace(TPair<ItemId, Level>(_itemId, _enchantLevel));

	TArray<const FGsMarketCollectionItemData*> itemList;
	marketData->GetMarketItemList(keyList, true, itemList);

	if (0 == itemList.Num())
	{
		// TEXT: 거래소에서 거래되지 않는 아이템입니다.
		FText textMsg;
		if (FText::FindText(TEXT("ItemCollectionMainText"), TEXT("ItemDetail_Market_Quick_Ticker_NotInMarket"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}

		return;
	}

	// 구매 팝업
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupCollectionMarketPurchase"));
	if (widget.IsValid())
	{
		if (UGsUIPopupCollectionMarketPurchase* popup = Cast<UGsUIPopupCollectionMarketPurchase>(widget.Get()))
		{
			popup->SetData(_collectionId, keyList);
		}
	}
}

void UGsUIMaterialGainInfoMarketPrice::OnClickPurchase()
{
	OpenPurchasePopup();
}

void UGsUIMaterialGainInfoMarketPrice::OnReceiveCollectionPriceList(const IGsMessageParam* InParam)
{
	const FGsMarketMsgParamId* param = InParam->Cast<const FGsMarketMsgParamId>();
	if (nullptr == param)
	{
		return;
	}
	
	if (param->_id != _collectionId)
	{
		return;
	}

	// 최저가 패킷이 같아서 광클 시 요청한 적이 있는지 확인하는 로직 삽입
	if (false == _isRequestLowestPrice)
	{
		return;
	}

	_isRequestLowestPrice = false;

	CheckAndSendRequestPrice(GSMarket()->CreateOrGetCollectionData(_collectionId));
}

void UGsUIMaterialGainInfoMarketPrice::OnReceiveItemPriceInfo(const IGsMessageParam*)
{
	FGsMarketControllerBase* controller = GSMarket()->GetController(_isRequestWorldMarket);
	if (nullptr == controller)
	{
		return;
	}

	const FGsMarketItemPriceData& priceData = controller->GetItemPriceData();
	if (priceData._itemId == _itemId &&
		priceData._enchantLevel == _enchantLevel)
	{
		_uiPriceInfo->SetData(priceData);
	}
}

void UGsUIMaterialGainInfoMarketPrice::OnRequestRefreshPrice(const IGsMessageParam*)
{
	// 비우고 패킷 받은 후 갱신한다
	_uiPriceInfo->SetEmpty();
	_sBtnPurchase->SetIsEnabled(false);
	_sBtnPurchase->SetSwitcherIndex(0);

	bool bIsTradable = IsTradable(_itemId);
	if (IsTradable(_itemId))
	{
		SendRequestLowestPrice();
	}
}
