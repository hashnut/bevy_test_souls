// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/BMShop/GsUIBMShopProductDetail.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"
#include "T1Project.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../../Common/Icon/GsUIIconItem.h"
#include "GameObject/Define/EBMShopProductTagType.h"
#include "DataSchema/GsSchemaEnums.h"


void UGsUIBMShopProductDetail::BeginDestroy()
{
	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}
	if (nullptr != _slotHelper)
	{
		_slotHelper->Clear();
		_slotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIBMShopProductDetail::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _slotHelper)
	{
		_slotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_slotHelper->Initialize(_iconSelector);
		_slotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIBMShopProductDetail::OnRefreshIconWrapBox);
		_slotHelper->OnCreateIcon.AddDynamic(this, &UGsUIBMShopProductDetail::OnCreateIconWrapBox);
	}
}

void UGsUIBMShopProductDetail::NativeDestruct()
{
	SetIsTutorial(false);

	Super::NativeDestruct();
}

void UGsUIBMShopProductDetail::OnShow()
{
	_productID = 0;
	_limitRoot->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIBMShopProductDetail::OnHide()
{
	_slotHelper->Clear();

}

void UGsUIBMShopProductDetail::SetProductData(const FGsBMShopProductData* InProductData, const bool InDisableStepupBlock)
{
	if (nullptr == InProductData)
		return;

	_productID = InProductData->GetProductID();
	SetProductIcon();
	SetProductLimitBuyerText(InProductData);
	SetProductResetText(InProductData);
	SetProductStickerTag(InProductData);
	SetProductAmountInfo(InProductData);
	SetLimit(InProductData, InDisableStepupBlock);

	_couponDisplayWidget->SetVisibility(InProductData->IsCouponProduct() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIBMShopProductDetail::SetProductIcon()
{
	_slotHelper->RefreshAll(1);
}

void UGsUIBMShopProductDetail::SetProductResetText(const FGsBMShopProductData* InData)
{
	FText findText;
	if (ResetTimeTicket::RTT_NONE == InData->GetResetTimeType())
	{
		_resetTimeText->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	if (0 >= InData->GetPurchaseQuantiyLimit())
	{
		_resetTimeText->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	switch (InData->GetResetTimeType())
	{
	case ResetTimeTicket::RTT_BM_DAILY: // 일일 리셋
		FText::FindText(TEXT("BMShopText"), TEXT("purchaseLimitCycleD"), findText);
		break;
	case ResetTimeTicket::RTT_BM_WEEKLY:  // 주간 리셋
		FText::FindText(TEXT("BMShopText"), TEXT("purchaseLimitCycleW"), findText);
		break;
	case ResetTimeTicket::RTT_BM_MONTHLY: // 월간 리셋
		FText::FindText(TEXT("BMShopText"), TEXT("purchaseLimitCycleM"), findText);
		break;
	}

	_resetTimeText->SetVisibility(ESlateVisibility::Visible);
	_resetTimeText->SetText(findText);
}

void UGsUIBMShopProductDetail::SetProductStickerTag(const FGsBMShopProductData* InData)
{
	if (InData->IsStepUpProduct())
	{
		_tagSwitcher->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	switch (InData->GetTagType())
	{
	case EBMShopProductTagType::Tag_None:
		_tagSwitcher->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EBMShopProductTagType::Tag_Event:
		_tagSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_tagSwitcher->SetActiveWidgetIndex(1);
		break;
	case EBMShopProductTagType::Tag_Hot:
		_tagSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_tagSwitcher->SetActiveWidgetIndex(2);
		break;
	case EBMShopProductTagType::Tag_New:
		_tagSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_tagSwitcher->SetActiveWidgetIndex(0);
		break;
	}
}

void UGsUIBMShopProductDetail::SetProductAmountInfo(const FGsBMShopProductData* InData)
{
	FText markText;
	FText::FindText(TEXT("MarkFormatingText"), TEXT("Slash"), markText);
	uint16 maxLimitCount = InData->GetPurchaseQuantiyLimit();
	if (0 < maxLimitCount)
	{
		_quantiyCount->SetVisibility(ESlateVisibility::Visible);

		FString limitString = FString::FromInt(maxLimitCount);
		FString quantiy = FString::FromInt(InData->GetPurchaseQuantiy());
		FString result = quantiy + markText.ToString() + limitString;
		_quantiyCount->SetText(FText::FromString(result));
	}
	else
	{
		_quantiyCount->SetVisibility(ESlateVisibility::Collapsed);
		/*
		FText findText;
		FText::FindText(TEXT("BMShopText"), TEXT("saleEndUndecided"), findText);
		_quantiyCount->SetText(findText);
		*/
	}
}

void UGsUIBMShopProductDetail::SetProductLimitBuyerText(const FGsBMShopProductData* InData)
{
	FText findText;
	if (BMPurchaseLimitBuyerType::NONE == InData->GetLimitBuyerType())
	{
		_buyerLimitText->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	if (0 >= InData->GetPurchaseQuantiyLimit())
	{
		_buyerLimitText->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	switch (InData->GetLimitBuyerType())
	{
	case BMPurchaseLimitBuyerType::ACCOUNT:
		FText::FindText(TEXT("BMShopText"), TEXT("purchaseLimitBuyerA"), findText);
		break;
	case BMPurchaseLimitBuyerType::SERVER:
		FText::FindText(TEXT("BMShopText"), TEXT("purchaseLimitBuyerS"), findText);
		break;
	}

	_buyerLimitText->SetVisibility(ESlateVisibility::Visible);
	_buyerLimitText->SetText(findText);
}

void UGsUIBMShopProductDetail::SetLimit(const FGsBMShopProductData* InData, const bool InDisableStepupBlock)
{
	FTextKey key;
	if (true == InData->IsExpireSale()) // 1 판매 기간 종료
	{
		key = TEXT("tagExpired");
	}
	else if (true == InData->IsSlodOut()) // 2 품절
	{
		key = TEXT("tagSoldout");
	}
	else if (false == InData->IsEnoughLevelLimit() || false == InData->IsEnoughGuildGiftLevelLimit()) // 3 레벨 제한 , 기시단 레벨 제한
	{
		_limitRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		FText findText;
		if (false == InData->IsEnoughLevelLimit())
		{
			if (true == FText::FindText(TEXT("BMShopText"), TEXT("tagLevelLimit"), findText))
			{
				FText resultText = findText.Format(findText, InData->GetLimitLevel());
				_limitText->SetText(resultText);
			}
		}
		else
		{
			if (true == FText::FindText(TEXT("GuildText"), TEXT("UI_GuildShop_LevelLimit"), findText))
			{
				FText resultText = findText.Format(findText, InData->GetGuildGiftLimitLevel());
				_limitText->SetText(resultText);
			}
		}

		return;
	}
	else if (true == InData->IsBeforSale()) // 4 판매 시작 전
	{
		key = TEXT("tagLater");
	}
	else
	{
		if (false == InDisableStepupBlock && InData->IsStepUpProduct()) // 스탭업 상품 일때 처리
		{
			FGsBMShopManager* shopMgr = GBMShop();
			FGsBMShopManager::StepUpState state = shopMgr->GetStepUpProductState(_productID);
			if (FGsBMShopManager::StepUpState::State_IsLock == state)
			{
				const FGsBMShopProductData* beforeData = shopMgr->GetBeforeStepUpProductData(_productID);
				if (nullptr != beforeData)
				{
					FText findText;
					if (true == FText::FindText(TEXT("BMShopText"), TEXT("stepUpBan"), findText))
					{
						_limitRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
						FText resultText = findText.Format(findText, InData->GetStepUpLevel() - 1, beforeData->GetStepUpBuyLimitCount());
						_limitText->SetText(resultText);
					}
				}
			}
			else
			{
				_limitRoot->SetVisibility(ESlateVisibility::Collapsed);
			}

			return;
		}
		else
		{
			_limitRoot->SetVisibility(ESlateVisibility::Collapsed);
			return;
		}
	}

	_limitRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	FText findText;
	FText::FindText(TEXT("BMShopText"), key, findText);
	_limitText->SetText(findText);
}

void UGsUIBMShopProductDetail::OnRefreshIconWrapBox(int32 InIndex, class UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	FGsBMShopProductData* data = GBMShop()->FindProduct(_productID);
	if (nullptr == data)
	{
		itemIcon->SetOwnedItem(0);
		itemIcon->SetItemNameToolTipFlag(false);
		itemIcon->SetGrade(ItemGrade::NONE);
		itemIcon->SetSlotEffect(EGsItemIconSlotEffectType::None);
		return;
	}

	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIBMShopProductDetail::OnLongPressIcon);
	}
	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIBMShopProductDetail::OnClickSlot);
	}

	itemIcon->SetNotOwnItem(data->GetItemTID(),1);
	if (nullptr != data->GetIconPath())
	{
		itemIcon->SetIconImage(*data->GetIconPath());
	}
	itemIcon->SetActiveIconBg(false);
	itemIcon->SetRedDot(false);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetItemNameToolTipFlag(false);
	itemIcon->SetGrade(ItemGrade::NONE);
	itemIcon->SetSlotEffect(EGsItemIconSlotEffectType::None);
}

void UGsUIBMShopProductDetail::OnCreateIconWrapBox(class UGsUIIconBase* InIcon)
{
		UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->SetActiveIconBg(false);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetItemCount(0);
	itemIcon->SetCoolTimeBg(false);
}

void UGsUIBMShopProductDetail::OnLongPressIcon(class UGsUIIconItem& InIcon)
{
	if (_bIsTutorial)
	{
		return;
	}

	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

void UGsUIBMShopProductDetail::OnClickSlot(class UGsUIIconItem& InIcon)
{
	if (_bIsTutorial)
	{
		OnTutorialInteraction.ExecuteIfBound();
		return;
	}

	if (0 >= _productID)
	{
		return;
	}

	FGsBMShopManager::DisplayBuyPopup(_productID);
}

void UGsUIBMShopProductDetail::SetIsTutorial(bool bIsTutorial)
{
	_bIsTutorial = bIsTutorial;

	if (false == _bIsTutorial)
	{
		OnTutorialInteraction.Unbind();
	}
}
