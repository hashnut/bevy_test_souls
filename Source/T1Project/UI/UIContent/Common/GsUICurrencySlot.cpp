// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICurrencySlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "Item/GsItemManager.h"
#include "Currency/GsCostPackage.h"
#include "Currency/GsCurrencyHelper.h"
#include "T1Project.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"


void UGsUICurrencySlot::OnShow()
{
	_textBlockName = Cast<UTextBlock>(GetWidgetFromName(TEXT("C_TextBlock_Name")));

	// 버튼은 있을 수도, 없을 수도 있다
	_btnSlot = Cast<UGsButton>(GetWidgetFromName(TEXT("C_Btn_Slot")));
	if (_btnSlot)
	{
		if (false == _btnSlot->OnClicked.IsBound())
		{
			_btnSlot->OnClicked.AddDynamic(this, &UGsUICurrencySlot::OnClickSlot);
		}
	}

	_btnCharge = Cast<UGsButton>(GetWidgetFromName(TEXT("C_Btn_Charge")));
	if (_btnCharge)
	{
		if (false == _btnCharge->OnClicked.IsBound())
		{
			_btnCharge->OnClicked.AddDynamic(this, &UGsUICurrencySlot::OnClickCharge);
		}
	}

	_textBlockDiscountAmount = Cast<UTextBlock>(GetWidgetFromName(TEXT("_textBlockDiscountAmount")));
	_discountSwitcher = Cast<UWidgetSwitcher>(GetWidgetFromName(TEXT("_discountSwitcher")));
}

void UGsUICurrencySlot::OnHide()
{
	if (_btnSlot)
	{
		_btnSlot->OnClicked.RemoveAll(this);
	}

	if (_btnCharge)
	{
		_btnCharge->OnClicked.RemoveAll(this);
	}

	if (OnClickCurrencySlot.IsBound())
	{
		OnClickCurrencySlot.Unbind();
	}

	if (OnClickCurrencyCharge.IsBound())
	{
		OnClickCurrencyCharge.Unbind();
	}

	_limitAmount = 0;
	_costType = CostType::CURRENCY;
	_currencyType = CurrencyType::NONE;
	_currencyItemId = 0;
	_amount = 0;
}

void UGsUICurrencySlot::SetDataLocalPlayer(CurrencyType InType)
{
	Currency amount = 0;
	amount = FGsCurrencyHelper::GetCurrencyAmount(InType);
	SetData(InType, amount);
}

void UGsUICurrencySlot::SetDataLocalPlayer(ItemId InItemId)
{
	Currency amount = 0;
	amount = GItem()->GetAllItemCountByTID(InItemId);
	SetData(InItemId, amount);
}

void UGsUICurrencySlot::SetData(CurrencyType InType)
{
	_currencyType = InType;

	if (_btnCharge)
	{
		bool bCanUseChargeBtn = FGsCurrencyHelper::CanTryToNavigate(InType);
		SetVisibilityBtnCharge(bCanUseChargeBtn ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		_btnSlot->SetVisibility(bCanUseChargeBtn ? ESlateVisibility::Visible : ESlateVisibility::HitTestInvisible);
	}

	if (_imgIcon)
	{
		UPaperSprite* sprite = GetIcon(InType);
		if (sprite)
		{
			_imgIcon->SetVisibility(ESlateVisibility::Visible);
			_imgIcon->SetBrushFromAtlasInterface(sprite);
		}
		else
		{
			_imgIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (_textBlockName)
	{
		_textBlockName->SetText(FGsCurrencyHelper::GetCurrencyLocalizedText(InType));
	}
}

void UGsUICurrencySlot::SetRebateCurrencyData(CurrencyType InType, Currency InAmount)
{
	SetData(InType);
	_currencyItemId = 0;
	_amount = InAmount;

	FText resultText;
	FText findText;
	if (FText::FindText(TEXT("BMShopText"), TEXT("rebatePrice"), findText))
	{
		resultText = findText.Format(findText, FText::AsNumber(InAmount));
	}
	_textBlockAmount->SetText(resultText);
}

void UGsUICurrencySlot::SetData(CurrencyType InType, Currency InAmount)
{
	SetData(InType);
	_currencyItemId = 0;
	_amount = InAmount;
	FText textAmount;
	FGsUIStringHelper::GetAmountText(InAmount, _limitAmount, textAmount);
	_textBlockAmount->SetText(textAmount);
}

void UGsUICurrencySlot::SetData(CurrencyType InType, FText InAmountText)
{
	SetData(InType);
	_currencyItemId = 0;
	_amount = 0; // 주의: 
	_textBlockAmount->SetText(InAmountText);
}

void UGsUICurrencySlot::SetData(ItemId InItemId, Currency InAmount)
{
	_currencyType = CurrencyType::NONE;
	_currencyItemId = InItemId;
	_amount = InAmount;

	if (_imgIcon)
	{
		UPaperSprite* sprite = GetIcon(InItemId);
		if (sprite)
		{
			_imgIcon->SetVisibility(ESlateVisibility::Visible);
			_imgIcon->SetBrushFromAtlasInterface(sprite);
		}
		else
		{
			_imgIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	FText textAmount;
	FGsUIStringHelper::GetAmountText(InAmount, _limitAmount, textAmount);
	_textBlockAmount->SetText(textAmount);
}

void UGsUICurrencySlot::SetData(ItemId InItemId, FText InAmountText)
{
	_currencyType = CurrencyType::NONE;
	_currencyItemId = InItemId;
	_amount = 0; // 주의: 

	if (_imgIcon)
	{
		UPaperSprite* sprite = GetIcon(InItemId);
		if (sprite)
		{
			_imgIcon->SetVisibility(ESlateVisibility::Visible);
			_imgIcon->SetBrushFromAtlasInterface(sprite);
		}
		else
		{
			_imgIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	_textBlockAmount->SetText(InAmountText);
}

void UGsUICurrencySlot::SetChangeDisplayType(bool InIsDiscountFlag)
{
	if (_discountSwitcher)
	{
		_discountSwitcher->SetActiveWidgetIndex(InIsDiscountFlag ? 1 : 0);
	}
}

void UGsUICurrencySlot::SetVisibilityBtnCharge(ESlateVisibility InVisibility)
{
	if (_btnCharge)
	{
		_btnCharge->SetVisibility(InVisibility);
	}
}

void UGsUICurrencySlot::SetDisCountOriData(CurrencyType InType, Currency InAmount)
{
	if (_textBlockDiscountAmount && _discountSwitcher)
	{
		_discountSwitcher->SetActiveWidgetIndex(1);

		SetData(InType);
		_currencyItemId = 0;
		_amount = InAmount;
		FText textAmount;
		FGsUIStringHelper::GetAmountText(InAmount, _limitAmount, textAmount);
		_textBlockDiscountAmount->SetText(textAmount);
	}
}

void UGsUICurrencySlot::SetDisCountOriData(uint32 ItemTid, Currency InAmount)
{
	if (_textBlockDiscountAmount && _discountSwitcher)
	{
		_discountSwitcher->SetActiveWidgetIndex(1);

		SetData(ItemTid, InAmount);
		FText textAmount;
		FGsUIStringHelper::GetAmountText(InAmount, _limitAmount, textAmount);
		_textBlockDiscountAmount->SetText(textAmount);
	}
}

void UGsUICurrencySlot::SetDisCountData(CurrencyType InType, Currency InDiscountAmount)
{
	if (_discountSwitcher)
	{
		_discountSwitcher->SetActiveWidgetIndex(0);

		SetData(InType);
		_currencyItemId = 0;
		_amount = InDiscountAmount;
		FText textAmount;
		FGsUIStringHelper::GetAmountText(InDiscountAmount, _limitAmount, textAmount);
		_textBlockAmount->SetText(textAmount);
	}
}

// 현금결제 상품 가격표시에 사용되는 함수
void UGsUICurrencySlot::SetData(FText InAmountText)
{
	_imgIcon->SetVisibility(ESlateVisibility::Collapsed);
	_textBlockAmount->SetText(InAmountText);
}

void UGsUICurrencySlot::SetData(const FGsCostElement& InCostElements)
{
	if (CostType::CURRENCY == InCostElements._costType)
	{
		SetData(InCostElements.GetCurrencyType(), InCostElements.GetCostAmount());
	}
	else if (CostType::ITEM == InCostElements._costType)
	{
		SetData(InCostElements.GetItemId(), InCostElements.GetCostAmount());
	}
	else
	{
		GSLOG(Error, TEXT("Invalid InCostElements._costType"));
	}
}

void UGsUICurrencySlot::SetBMShopCostData(const FGsBMShopProductData* InProductData)
{
	if (nullptr == InProductData)
		return;

	if (InProductData->IsItemCurrencyType())
	{
		SetData(InProductData->GetPurchaseCurrencyItemId(), InProductData->GetPurchasePrice());
	}
	else
	{
		SetData(InProductData->GetPurchaseCurrencyType(), InProductData->GetPurchasePrice());
	}
}

UPaperSprite* UGsUICurrencySlot::GetIcon(CurrencyType InType)
{
	return FGsUIHelper::LoadSpriteFromPath(FGsCurrencyHelper::GetCurrencyIconPath(InType));
}

UPaperSprite* UGsUICurrencySlot::GetIcon(ItemId InItemId)
{
	const UGsTableItemCommon* table = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	if (table)
	{
		const FGsSchemaItemCommon* row;
		if (table->FindRowById(InItemId, row))
		{
			return FGsUIHelper::LoadSpriteFromPath(row->icon);
		}
	}
#if WITH_EDITOR
	GSLOG(Warning, TEXT("Fail to get icon. ItemId: %d"), InItemId);
#endif

	return nullptr;
}

void UGsUICurrencySlot::OnClickSlot()
{
	OnClickCurrencySlot.ExecuteIfBound(this);
}

void UGsUICurrencySlot::OnClickCharge()
{
	OnClickCurrencyCharge.ExecuteIfBound(this);
}

void UGsUICurrencySlot::SetAmountTextColor(const EGsUIColorType InColorType)
{
	_textBlockAmount->SetColorAndOpacity(FGsUIColorHelper::GetColor(InColorType));
}

void UGsUICurrencySlot::UpdateAmountTextColorLocalPlayer()
{
	bool bIsEnough = 0 < _currencyItemId ? FGsCurrencyHelper::CheckCurrency(_currencyItemId, _amount, false) :
		FGsCurrencyHelper::CheckCurrency(_currencyType, _amount, false);

	SetAmountTextColor(bIsEnough ? EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK);
}

void UGsUICurrencySlot::UpdateDiscountAmountTextColorLocalPlayer(Currency InAmount)
{
	bool bIsEnough = 0 < _currencyItemId ? FGsCurrencyHelper::CheckCurrency(_currencyItemId, InAmount,false) : 
		FGsCurrencyHelper::CheckCurrency(_currencyType, InAmount, false);

	SetAmountTextColor(bIsEnough ? EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK);
}

