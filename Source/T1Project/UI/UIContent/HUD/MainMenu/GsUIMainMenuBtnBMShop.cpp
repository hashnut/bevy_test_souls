// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMainMenuBtnBMShop.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "RedDot/GsRedDotCheckFunc.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Message/MessageParam/GsMessageParam.h"

void UGsUIMainMenuBtnBMShop::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateBMShopIconEffect(false);
	UpdateBMShopRemainTime(false);
	_displayProductCountRoot->SetVisibility(ESlateVisibility::Hidden);

	_btnSlot->SetOnClickLockSec(1.f);
}

void UGsUIMainMenuBtnBMShop::UpdateManagerTick(float InDeltaTime)
{
	if (false == IsVisible())
	{
		return;
	}

	if (_textBlockLimitTime->IsVisible())
	{
		FTimespan span = _bmShopEffectTimer - FGsTimeSyncUtil::GetServerNowDateTimeUTC();
		if (0 <= span.GetTicks())
		{
			//FText findText;
			FString timeStr;
			FGsTimeStringHelper::GetTimeStringHMS(span, timeStr, false);
			//FText::FindText(TEXT("BMShopText"), TEXT("leftSaleTime"), findText);
			//FText text = FText::Format(findText, span.GetHours(), span.GetMinutes(), span.GetSeconds());
			//FText text = FText::Format(findText, FText::FromString(timeStr));
			_textBlockLimitTime->SetText(FText::FromString(timeStr));
			_textBlockLimitTime->SetColorAndOpacity(3600 >= span.GetTotalSeconds() ? FLinearColor::Red : FLinearColor::White);
		}
	}
}

void UGsUIMainMenuBtnBMShop::SetMenuOpen(bool bIsOpen)
{
	OnSubMenuOpen(bIsOpen);
}

void UGsUIMainMenuBtnBMShop::UpdateBMShopEffect(FGsSharedParam& InMessageParam)
{
	bool state = GBMShop()->GetPopupShopLessRemainTimeProduct(_bmShopEffectTimer);
	UpdateBMShopRemainTime(state);
	UpdateBMShopIconEffect(state);
	UpdateBMShopOpenProductCount(state);
}

void UGsUIMainMenuBtnBMShop::UpdateBMShopIconEffect(bool InState)
{
	_uiEffect->SetVisibility(InState ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIMainMenuBtnBMShop::UpdateBMShopRemainTime(bool InState)
{
	_textBlockLimitTime->SetVisibility(InState ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIMainMenuBtnBMShop::UpdateBMShopOpenProductCount(bool InState)
{
	if (InState)
	{
		uint8 openCount = GBMShop()->GetOpenPopupShopProductCount();
		_displayProductCountRoot->SetVisibility(0 < openCount ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
		_countText->SetText(FText::AsNumber(openCount));

		return;
	}
	_displayProductCountRoot->SetVisibility(ESlateVisibility::Hidden);	
}

void UGsUIMainMenuBtnBMShop::SetRedDotCheck(IGsRedDotCheck* InLogic, EGsIconRedDotType InType)
{
	_redDotCheck = InLogic;
	_redDotIconType = InType;
}

bool UGsUIMainMenuBtnBMShop::UpdateRedDot(bool bUpdate)
{
	bool bIsRedDot = false;
	if (_redDotCheck)
	{
		bIsRedDot = _redDotCheck->CheckRedDot(bUpdate);
		if (_bmShopRootRedDot)
		{
			_bmShopRootRedDot->SetRedDot(bIsRedDot, _redDotIconType, _redDotCheck->GetRedDotCount());
		}
	}

	return bIsRedDot;
}

