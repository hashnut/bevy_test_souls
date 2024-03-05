// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMaterialSetGainInfoMarketPrice.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/Material/GsUIMaterialSetGainInfoMarketPriceListEntry.h"
#include "UI/UIContent/Popup/GsUIPopupCollectionMarketPurchase.h"
#include "Item/Collection/GsItemCollectionField.h"
#include "Market/GsMarketCollectionData.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Net/GsNetSendServiceMarket.h"
#include "Message/MessageParam/GsMarketMessageParam.h"
#include "Market/GsMarketHelper.h"


void UGsUIMaterialSetGainInfoMarketPrice::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIMaterialSetGainInfoMarketPrice::OnCreateEntry);
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIMaterialSetGainInfoMarketPrice::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIMaterialSetGainInfoMarketPrice::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelRootList);
	_slotHelper->OnCreateEntry.AddDynamic(this, &UGsUIMaterialSetGainInfoMarketPrice::OnCreateEntry);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIMaterialSetGainInfoMarketPrice::OnRefreshEntry);

	_checkerRefresh.SetIntervalSecond(5.f);
}

void UGsUIMaterialSetGainInfoMarketPrice::NativeDestruct()
{	
	_collectionData = nullptr;
	_marketData = nullptr;
		
	Super::NativeDestruct();
}

void UGsUIMaterialSetGainInfoMarketPrice::SetData(const FGsItemCollectionField* InData)
{
	_collectionData = InData;

	_marketData = GSMarket()->CreateOrGetCollectionData(_collectionData->Id());

	SendReqestLowestPrice();

	_scroll->ScrollToStart();
	_slotHelper->RefreshAll(_collectionData->Conditions().Num());
}

void UGsUIMaterialSetGainInfoMarketPrice::RefreshPrice()
{
	// 광클방지 타이머 리셋
	if (_marketData)
	{
		_marketData->ResetCoolTime(true);
	}

	SendReqestLowestPrice();
}

void UGsUIMaterialSetGainInfoMarketPrice::OnClickRefresh()
{
	if (false == _checkerRefresh.IsTimeOver())
	{
		return;
	}

	RefreshPrice();

	_checkerRefresh.UpdateTime();
}

void UGsUIMaterialSetGainInfoMarketPrice::OnClickPurchase()
{
	if (nullptr == _marketData)
	{
		return;
	}

	TArray<TPair<ItemId, Level>> keyList;

	const TArray<FGsItemCollectionCondition>& conditionList = _collectionData->Conditions();
	for (const FGsItemCollectionCondition& condition : conditionList)
	{
		if (IsActiveCollect(condition.ActionType()))
		{
			const TPair<ItemId, Level>& keyPair = condition.EntryMaterialInfo();

			TPair<ItemId, Level> key(keyPair);
			keyList.AddUnique(key);
		}
	}	

	OpenPurchasePopup(keyList);
}

bool UGsUIMaterialSetGainInfoMarketPrice::IsExistMarketData() const
{
	if (_marketData)
	{
		TArray<FGsMarketCollectionItemData>& list = _marketData->GetDataList(true);
		return (0 < list.Num()) ? true : false;
	}

	return false;
}

void UGsUIMaterialSetGainInfoMarketPrice::OnReceiveMarketPriceList(const IGsMessageParam* InParam)
{
	if (nullptr == InParam ||
		nullptr == _collectionData)
	{
		return;
	}

	if (const FGsMarketMsgParamId* param = InParam->Cast<const FGsMarketMsgParamId>())
	{
		if (_collectionData->Id() == param->_id)
		{
			InvalidateList();
		}
	}
}

void UGsUIMaterialSetGainInfoMarketPrice::OnRequestRefreshPrice(const IGsMessageParam*)
{
	RefreshPrice();
}

void UGsUIMaterialSetGainInfoMarketPrice::OnCreateEntry(UWidget* InEntry)
{
	if (UGsUIMaterialSetGainInfoMarketPriceListEntry* itemEntry = Cast<UGsUIMaterialSetGainInfoMarketPriceListEntry>(InEntry))
	{
		itemEntry->OnPurchase.BindUObject(this, &UGsUIMaterialSetGainInfoMarketPrice::OnClickPurchaseListEntry);
	}
}

void UGsUIMaterialSetGainInfoMarketPrice::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (nullptr == _collectionData)
	{
		return;
	}

	const TArray<FGsItemCollectionCondition>& conditionList = _collectionData->Conditions();
	if (false == conditionList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIMaterialSetGainInfoMarketPriceListEntry* itemEntry = Cast<UGsUIMaterialSetGainInfoMarketPriceListEntry>(InEntry))
	{
		const FGsItemCollectionCondition& condition = conditionList[InIndex];		

		MarketPrice lowestPrice = 0;
		bool bIsWorldMarket = false;
		bool bOtherLevelExist = false;
		if (_marketData)
		{
			const TPair<ItemId, Level> keyPair = condition.EntryMaterialInfo();
			if (const FGsMarketCollectionItemData* marketData = _marketData->GetLowestPricePerUnitItem(keyPair))
			{
				lowestPrice = marketData->_price;
				bIsWorldMarket = marketData->_bIsWorldMarket;
			}

			bOtherLevelExist = _marketData->IsOtherLevelExist(keyPair.Key);
		}

		itemEntry->SetData(InIndex, &condition, lowestPrice, bIsWorldMarket, bOtherLevelExist);
	}
}

void UGsUIMaterialSetGainInfoMarketPrice::OnClickPurchaseListEntry(int32 InIndex)
{
	if (nullptr == _marketData || 
		nullptr == _collectionData)
	{
		return;
	}

	const TArray<FGsItemCollectionCondition>& conditionList = _collectionData->Conditions();
	if (false == conditionList.IsValidIndex(InIndex))
	{
		return;
	}

	const FGsItemCollectionCondition& condition = conditionList[InIndex];	
	const TPair<ItemId, Level> keyPair = condition.EntryMaterialInfo();

	bool bIsPrice = false;
	if (const FGsMarketCollectionItemData* marketData = _marketData->GetLowestPricePerUnitItem(keyPair))
	{
		if (0 < marketData->_price)
		{
			bIsPrice = true;
		}
	}

	if (bIsPrice)
	{
		TArray<TPair<ItemId, Level>> keyList;
		keyList.Emplace(keyPair);

		OpenPurchasePopup(keyList);
	}
	else
	{
		FGsMarketHelper::OpenMarketFromCollection(_collectionData->Id(), keyPair.Key);
	}
}

void UGsUIMaterialSetGainInfoMarketPrice::InvalidateList()
{
	if (_collectionData)
	{
		_slotHelper->RefreshAll(_collectionData->Conditions().Num());
	}
}

void UGsUIMaterialSetGainInfoMarketPrice::SendReqestLowestPrice()
{
	if (nullptr == _collectionData ||
		nullptr == _marketData)
	{
		return;
	}

	// 광클 방지로직
	if (_marketData->IsCoolTime(true))
	{
		InvalidateList();
	}
	else
	{
		TArray<FGsMarketLowestPriceRequestInfo> reqList;
		FGsMarketCollectionData::GetLowestPriceRequestInfo(_collectionData, reqList);

		// 서버에 요청 보내기
		FGsNetSendServiceMarket::SendReqCollectionItemMarketPriceLowest(true, _collectionData->Id(), reqList);

		_marketData->UpdateCoolTime(true);
	}
}

void UGsUIMaterialSetGainInfoMarketPrice::OpenPurchasePopup(TArray<TPair<ItemId, Level>>& InData)
{
	if (_marketData)
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupCollectionMarketPurchase"));
		if (widget.IsValid())
		{
			if (UGsUIPopupCollectionMarketPurchase* popup = Cast<UGsUIPopupCollectionMarketPurchase>(widget.Get()))
			{
				popup->SetData(_marketData->GetCollectionId(), InData);
			}
		}
	}
}

bool UGsUIMaterialSetGainInfoMarketPrice::IsActiveCollect(ECollectionMaterialActionType InType) const
{
	if (ECollectionMaterialActionType::Completed == InType)
	{
		return false;
	}

	return true;
}
