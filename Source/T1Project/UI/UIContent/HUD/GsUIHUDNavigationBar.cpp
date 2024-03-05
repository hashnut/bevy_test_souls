// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDNavigationBar.h"
#include "Components/WidgetSwitcher.h"
#include "Currency/GsCurrencyHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIContent/Common/GsUICurrencyList.h"
#include "UI/UIContent/Common/GsUICurrencyDetailInfo.h"
#include "Management/GsMessageholder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "DataSchema/GsSchemaCurrencyData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


void UGsUIHUDNavigationBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();	

	// 슬롯 버튼 이벤트 연결
	_slotDia->OnClickCurrencySlot.BindUObject(this, &UGsUIHUDNavigationBar::OnClickCurrencyEntry);
	_slotDia->OnClickCurrencyCharge.BindUObject(this, &UGsUIHUDNavigationBar::OnClickCurrencyCharge);
	_slotGold->OnClickCurrencySlot.BindUObject(this, &UGsUIHUDNavigationBar::OnClickCurrencyEntry);
	_slotGold->OnClickCurrencyCharge.BindUObject(this, &UGsUIHUDNavigationBar::OnClickCurrencyCharge);

	// 드롭다운 메뉴
	_btnDropDownCurrency->OnPressed.AddDynamic(this, &UGsUIHUDNavigationBar::OnClickDropDownCurrency);
}

void UGsUIHUDNavigationBar::NativeConstruct()
{
	Super::NativeConstruct();

	_uiCurrencyList->SetCurrencyInfo(_currencyInfo);

	// 드롭다운 버튼 상시 출력
	SetEnableDropDown(true);

	SetOpenDropDownCurrency(false);

	FGsMessageHolder* msg = GMessage();
	
	// 재화 갱신 메시지 받을 경우	
	_handlerUserInfo = msg->GetUserBasicInfo().AddUObject(MessageUserInfo::CURRENCY,
		this, &UGsUIHUDNavigationBar::OnUpdateCurrency);
}

void UGsUIHUDNavigationBar::NativeDestruct()
{
	// 메시지 해제
	if (FGsMessageHolder* msgMgr = GMessage())
	{
		msgMgr->GetUserBasicInfo().Remove(_handlerUserInfo);
	}

	Super::NativeDestruct();
}

void UGsUIHUDNavigationBar::NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusChanging(PreviousFocusPath, NewWidgetPath, InFocusEvent);

	if (InFocusEvent.GetCause() == EFocusCause::Mouse)
	{
		if (!NewWidgetPath.ContainsWidget(_uiCurrencyList->GetCachedWidget().ToSharedRef()) &&
			!NewWidgetPath.ContainsWidget(_currencyInfo->GetCachedWidget().ToSharedRef()) &&
			!NewWidgetPath.ContainsWidget(_btnDropDownCurrency->GetCachedWidget().ToSharedRef()))
		{
			SetOpenDropDownCurrency(false);
			InvalidateCurrencyList();
		}
	}
}

void UGsUIHUDNavigationBar::InvalidateAllInternal()
{
	InvalidateCurrencyList();
}

void UGsUIHUDNavigationBar::InvalidateCurrencyList()
{
	UpdateCurrency(CurrencyType::DIA);
	UpdateCurrency(CurrencyType::GOLD);

	// 드롭다운 가능여부에 따라 좌측 버튼
	_btnDropDownCurrency->SetVisibility((_bEnableDropDown) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	// _btnDropDownCurrency 하위에 있으므로 Visible조절은 필요 없음
	_switcherDropDownArrow->SetActiveWidgetIndex((_bIsDropDownOpen) ? 1 : 0);

	// 재화 보유 현황 일괄 안내 팝업 UI 설정
	_uiCurrencyList->SetVisibility(_bIsDropDownOpen ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	_bIsDropDownOpen ? _uiCurrencyList->Show() : _uiCurrencyList->Hide();
}

void UGsUIHUDNavigationBar::OnUpdateCurrency(uint64 InType)
{
	CurrencyType targetCurrency = static_cast<CurrencyType>(InType);
	UpdateCurrency(targetCurrency);
}

void UGsUIHUDNavigationBar::UpdateCurrency(CurrencyType InType)
{
	if (CurrencyType::GOLD == InType)
	{
		_slotGold->SetDataLocalPlayer(InType);
		_slotGold->SetVisibilityBtnCharge(ESlateVisibility::Collapsed);
	}
	else if (CurrencyType::DIA == InType ||
		CurrencyType::FREE_DIA == InType)
	{
		// 다이아로 넣으면 합산해서 표시됨
		_slotDia->SetDataLocalPlayer(CurrencyType::DIA);
	}
}

void UGsUIHUDNavigationBar::OnClickCurrencyEntry(UWidget* InEntry)
{
	if (UGsUICurrencySlot* slot = Cast<UGsUICurrencySlot>(InEntry))
	{
		FGsCurrencyHelper::TryToNavigate(slot->GetCurrencyType());
	}
}

void UGsUIHUDNavigationBar::OnClickCurrencyCharge(UWidget* InEntry)
{
	if (UGsUICurrencySlot* slot = Cast<UGsUICurrencySlot>(InEntry))
	{
		FGsCurrencyHelper::TryToNavigate(slot->GetCurrencyType());
	}
}

void UGsUIHUDNavigationBar::OnClickDropDownCurrency()
{
	if (_bEnableDropDown)
	{
		SetOpenDropDownCurrency(!_bIsDropDownOpen);
		InvalidateCurrencyList();
	}
}
