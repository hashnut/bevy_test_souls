// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/BMShop/GsUIBMShopProvideGroup.h"
#include "GsUIBMShopProductItem.h"
#include "T1Project.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"



void UGsUIBMShopProvideGroup::BeginDestroy()
{
	if (nullptr != _itemListPanelSlotHelper)
	{
		_itemListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIBMShopProvideGroup::OnRefreshItem);
		_itemListPanelSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIBMShopProvideGroup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _itemListPanelSlotHelper)
	{
		_itemListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_itemListPanelSlotHelper->Initialize(_itemListEntryWidgetClass, _itemListRootPanel);
		_itemListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIBMShopProvideGroup::OnRefreshItem);
	}
}

void UGsUIBMShopProvideGroup::OnShow()
{
	_hideDetailBtn = false;

	_cashRebateInfo.mCurrencyType = CurrencyType::NONE;
	_cashRebateInfo.mCurrency = 0;
}

void UGsUIBMShopProvideGroup::OnHide()
{
	_hideDetailBtn = false;

	_cashRebateInfo.mCurrencyType = CurrencyType::NONE;
	_cashRebateInfo.mCurrency = 0;

	_itemListPanelSlotHelper->RefreshAll(0);
}

void UGsUIBMShopProvideGroup::SetProvideItem(FGsRewardUnboxingData InData, bool InIsUseLastData, bool InHideDetailBtn)
{
	_hideDetailBtn = InHideDetailBtn;
	_data = InData;

	SetTitleText(_data._rewardType);

	int32 refreshNum = _data._itemDataList.Num();
	if (InIsUseLastData && CurrencyType::NONE != _cashRebateInfo.mCurrencyType)
	{
		refreshNum += 1;
	}
	_itemListPanelSlotHelper->RefreshAll(refreshNum);

	/*
	for (uint8 i = 0; i < InItemDataList.Num(); ++i)
	{
		GSLOG(Warning, TEXT("[UGsUIBMShopProvideGroup::SetProvideItem] - itemTID : %d"), InItemDataList[i]->itemTID);
	}
	*/
}

void UGsUIBMShopProvideGroup::SetTitleText(const RewardType InType)
{
	FText findText;
	FText findText1;
	FString resultText;
	switch (InType)
	{
	case RewardType::ALL: // fixed
		FText::FindText(TEXT("BMShopText"), TEXT("itemListFixed"), findText);
		break;
	case RewardType::PICK_ONE: // select Type
		FText::FindText(TEXT("BMShopText"), TEXT("itemListSelect"), findText);
		break;
	case RewardType::RANDOM_ONE: // fixed random type
		FText::FindText(TEXT("BMShopText"), TEXT("itemListFixedRandom"), findText);
		break;

		/*
	case EBMShopProductGroupType::Guild_Buyer_Type: // 구매자
		FText::FindText(TEXT("BMShopText"), TEXT("itemListFixed"), findText);
		FText::FindText(TEXT("BMShopText"), TEXT("itemListGuildBuyer"), findText1);
		resultText = FString::Format(TEXT("{0} {1}"), { findText1.ToString(), findText.ToString() });
		findText = FText::FromString(resultText);
		break;
	case EBMShopProductGroupType::Guild_Member_Type: // 기사단원
		FText::FindText(TEXT("BMShopText"), TEXT("itemListFixed"), findText);
		FText::FindText(TEXT("BMShopText"), TEXT("itemListGuildMember"), findText1);
		resultText = FString::Format(TEXT("{0} {1}"), { findText1.ToString(), findText.ToString() });
		findText = FText::FromString(resultText);
		break;
		*/
	}

	_provideTitleText->SetText(findText);
}

void UGsUIBMShopProvideGroup::OnRefreshItem(int32 InIndex, UWidget* InEntry)
{
	UGsUIBMShopProductItem* entry = Cast<UGsUIBMShopProductItem>(InEntry);
	if (nullptr == entry)
		return;

	if (_data._itemDataList.IsValidIndex(InIndex))
	{
		const FGsSchemaRewardItemBagData* bagData = _data._itemDataList[InIndex];
		entry->SetData(bagData);
		if (_hideDetailBtn)
		{
			entry->SetDetailBtnVisibility(false);
		}
	}
	else
	{
		if (CurrencyType::NONE != _cashRebateInfo.mCurrencyType)
		{
			entry->SetData(_cashRebateInfo);
			entry->SetDetailBtnVisibility(false);

			_cashRebateInfo.mCurrencyType = CurrencyType::NONE;
			_cashRebateInfo.mCurrency = 0;
		}
	}
}
