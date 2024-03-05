// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayNavigationBar.h"

#include "Components/WrapBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Spacer.h"

#include "Currency/GsCurrencyHelper.h"

#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIContent/Common/GsUICurrencyList.h"
#include "UI/UIContent/Common/GsUICurrencyDetailInfo.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "Management/GsMessageholder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/MessageParam/GsUIMessageParam.h"

#include "DataSchema/GsSchemaCurrencyData.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Item/GsItemManager.h"


void UGsUITrayNavigationBar::BeginDestroy()
{
	if (_currencySlotHelper)
	{
		_currencySlotHelper->OnCreateEntry.RemoveDynamic(this, &UGsUITrayNavigationBar::OnCreateCurrencyEntry);
		_currencySlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUITrayNavigationBar::OnRefreshCurrencyEntry);
	}
	_currencySlotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUITrayNavigationBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 표시할 재화 리스트 만들어 두기
	_currencyList.Empty();
	const UGsTable* currencyTable = FGsSchemaCurrencyData::GetStaticTable();
	if (currencyTable)
	{
		TArray<const FGsSchemaCurrencyData*> dataList;
		if (currencyTable->GetAllRows(dataList))
		{
			for (const FGsSchemaCurrencyData* currencyData : dataList)
			{
				// 일부 Currency만 보여준다. 이후 필요시 테이블에 보여줄 것만 체크하도록 변경하자
				if (CurrencyType::GUILD_ASSET > currencyData->currencyType)
				{
					_currencyList.Emplace(currencyData->currencyType);
				}
			}
		}
	}

	// 드롭다운 메뉴
	_btnDropDownCurrency->OnPressed.AddDynamic(this, &UGsUITrayNavigationBar::OnClickDropDownCurrency);

	// 슬롯 헬퍼 생성 및 이벤트 연결
	_currencySlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_currencySlotHelper->Initialize(_currencyEntryClass, _panelCurrency);	
	_currencySlotHelper->OnCreateEntry.AddDynamic(this, &UGsUITrayNavigationBar::OnCreateCurrencyEntry);
	_currencySlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUITrayNavigationBar::OnRefreshCurrencyEntry);
}

void UGsUITrayNavigationBar::NativeConstruct()
{
	Super::NativeConstruct();

	_uiCurrencyList->SetCurrencyInfo(_currencyInfo);

	// 드롭다운 버튼 상시 출력
	SetEnableDropDown(true);

	SetOpenDropDownCurrency(false);

	FGsMessageHolder* msg = GMessage();
	
	// 재화 갱신 메시지 받을 경우	
	_handlerUserInfo = msg->GetUserBasicInfo().AddUObject(MessageUserInfo::CURRENCY,
		this, &UGsUITrayNavigationBar::OnUpdateCurrency);
	
	_msgItemHandleList.Emplace(msg->GetItem().AddUObject(MessageItem::ADDITEMLIST,
		this, &UGsUITrayNavigationBar::OnAddItemList));
	_msgItemHandleList.Emplace(msg->GetItem().AddUObject(MessageItem::UPDATEITEMLIST,
		this, &UGsUITrayNavigationBar::OnUpdateItemList));
	_msgItemHandleList.Emplace(msg->GetItem().AddUObject(MessageItem::REMOVEITEM,
		this, &UGsUITrayNavigationBar::OnRemoveItem));
	
	// 재화 종류 설정
	_msgUIHandleList.Emplace(msg->GetUI().AddUObject(MessageUI::SET_NAVIGATION_BAR_CURRENCY,
		this, &UGsUITrayNavigationBar::OnSetOptionalCurrency));
	_msgUIHandleList.Emplace(msg->GetUI().AddUObject(MessageUI::SET_NAVIGATION_BAR_ITEM,
		this, &UGsUITrayNavigationBar::OnSetOptionalCurrencyItem));

	// -1은 추가 슬롯 세팅 하지 않는 것.
	SetOptionalCurrency(CurrencyType::MAX_USER_TYPE);
}

void UGsUITrayNavigationBar::NativeDestruct()
{
	// 메시지 해제
	FGsMessageHolder* msgMgr = GMessage();
	
	msgMgr->GetUserBasicInfo().Remove(_handlerUserInfo);

	for (MsgItemHandle& handle : _msgItemHandleList)
	{
		msgMgr->GetItem().Remove(handle);
	}
	_msgItemHandleList.Empty();

	for (MsgUIHandle& handle : _msgUIHandleList)
	{
		msgMgr->GetUI().Remove(handle);
	}
	_msgUIHandleList.Empty();

	Super::NativeDestruct();
}

void UGsUITrayNavigationBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	IGsInvalidateUIInterface::CheckInvalidate();
}

void UGsUITrayNavigationBar::NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent)
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

void UGsUITrayNavigationBar::InvalidateAllInternal()
{
	InvalidateCurrencyList();
}

void UGsUITrayNavigationBar::InvalidateCurrencyList()
{
	if (_currencySlotHelper)
	{
		_currencySlotHelper->RefreshAll(2);
	}

	// 드롭다운 가능여부에 따라 좌측 버튼
	_btnDropDownCurrency->SetVisibility((_bEnableDropDown) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	// _btnDropDownCurrency 하위에 있으므로 Visible조절은 필요 없음
	_switcherDropDownArrow->SetActiveWidgetIndex((_bIsDropDownOpen) ? 1 : 0);

	// 재화 보유 현황 일괄 안내 팝업 UI 설정
	_uiCurrencyList->SetVisibility(_bIsDropDownOpen ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	_bIsDropDownOpen ? _uiCurrencyList->Show() : _uiCurrencyList->Hide();

	// 선택 슬롯 세팅
	if (CostType::MAX == _optionalCostType)
	{
		_optionalSlot->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (CostType::CURRENCY == _optionalCostType)
	{
		if (CurrencyType::NONE == _optionalCurrency)
		{
			_optionalSlot->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			_optionalSlot->SetVisibility(ESlateVisibility::Visible);
			_optionalSlot->SetDataLocalPlayer(_optionalCurrency);
		}
	}
	else if (CostType::ITEM == _optionalCostType)
	{
		_optionalSlot->SetVisibility(ESlateVisibility::Visible);
		_optionalSlot->SetDataLocalPlayer(_optionalItemId);
	}
}

void UGsUITrayNavigationBar::SetOptionalCurrency(CurrencyType InType)
{
	// 드롭 다운을 닫는다
	SetOpenDropDownCurrency(false);

	_optionalCostType = CostType::CURRENCY;
	_optionalCurrency = InType;

	if (CurrencyType::MAX_USER_TYPE == InType)
	{
		_optionalCurrency = CurrencyType::NONE;
	}

	InvalidateCurrencyList();

	// HUD를 제외하고는 화면 우측 정렬
	SetNaviBarAlign(EGsNaviBarAlign::RIGHT);
}

void UGsUITrayNavigationBar::SetOptionalCurrencyItem(int32 InItemId)
{
	// 드롭 다운을 닫는다
	SetOpenDropDownCurrency(false);

	_optionalCostType = CostType::ITEM;
	_optionalItemId = InItemId;

	InvalidateCurrencyList();

	// HUD를 제외하고는 화면 우측 정렬
	SetNaviBarAlign(EGsNaviBarAlign::RIGHT);
}

void UGsUITrayNavigationBar::SetNaviBarAlign(EGsNaviBarAlign InType)
{
	switch (InType)
	{
	case EGsNaviBarAlign::RIGHT:
	{
		_spacerRight->SetVisibility(ESlateVisibility::Collapsed);
		_spacerExitBtnSpace->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_spacerMarketSpace->SetVisibility(ESlateVisibility::Collapsed);
	}
	break;
	case EGsNaviBarAlign::CENTER:
	{
		_spacerRight->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_spacerExitBtnSpace->SetVisibility(ESlateVisibility::Collapsed);
		_spacerMarketSpace->SetVisibility(ESlateVisibility::Collapsed);
	}
	break;
	case EGsNaviBarAlign::MARKET:
	{
		_spacerRight->SetVisibility(ESlateVisibility::Collapsed);
		_spacerExitBtnSpace->SetVisibility(ESlateVisibility::Collapsed);
		_spacerMarketSpace->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	break;
	default:
	{
		_spacerRight->SetVisibility(ESlateVisibility::Collapsed);
		_spacerExitBtnSpace->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_spacerMarketSpace->SetVisibility(ESlateVisibility::Collapsed);
	}
	break;
	}
}

void UGsUITrayNavigationBar::OnUpdateCurrency(uint64 InType)
{
	CurrencyType targetCurrency = static_cast<CurrencyType>(InType);

	// 추가슬롯일 경우
	if (targetCurrency == _optionalCurrency)
	{
		_optionalSlot->SetDataLocalPlayer(_optionalCurrency);
		return;
	}

	// 해당 재화의 인덱스를 찾아서 갱신
	int32 targetIndex = _currencyList.IndexOfByKey(targetCurrency);
	if (INDEX_NONE != targetIndex)
	{
		_currencySlotHelper->RefreshByIndex(targetIndex);
	}

	// 무료 다이아일 경우 다이아도 한번 더 갱신
	if (CurrencyType::FREE_DIA == targetCurrency)
	{
		int32 diaIndex = _currencyList.IndexOfByKey(CurrencyType::DIA);
		if (INDEX_NONE != diaIndex)
		{
			_currencySlotHelper->RefreshByIndex(diaIndex);
		}
	}
}

void UGsUITrayNavigationBar::OnAddItemList(FGsItemMessageParamBase& InParam)
{
	FGsAddItemList* paramData = static_cast<FGsAddItemList*>(&InParam);
	if (paramData)
	{
		for (int i = 0; i < paramData->_itemDbidList.Num(); ++i)
		{
			int32 targetItemId = GItem()->GetTIDByDBID(paramData->_itemDbidList[i]);
			if (targetItemId == _optionalItemId)
			{
				_optionalSlot->SetDataLocalPlayer(targetItemId);
			}
		}
	}
}

void UGsUITrayNavigationBar::OnUpdateItemList(FGsItemMessageParamBase& InParam)
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	FGsUpdateItemList* paramData = static_cast<FGsUpdateItemList*>(&InParam);
	if (paramData)
	{
		for (ItemDBId itemDBId : paramData->_itemDbidList)
		{
			// 아이템이 모두 업데이트된 후 메시지를 수신하고, _optionalItemId이 하나기 때문에 1회 설정 후 return 한다.
			int32 targetItemId = itemManager->GetTIDByDBID(itemDBId);
			if (targetItemId == INVALID_ITEM_ID)
				continue;

			if (targetItemId == _optionalItemId)
			{
				_optionalSlot->SetDataLocalPlayer(targetItemId);
				return;
			}
		}
	}
}

void UGsUITrayNavigationBar::OnRemoveItem(FGsItemMessageParamBase& InParam)
{
	FGsRemoveItem* paramData = static_cast<FGsRemoveItem*>(&InParam);
	if (paramData)
	{
		if (_optionalItemId == paramData->_itemTID)
		{
			_optionalSlot->SetDataLocalPlayer(_optionalItemId);
		}
	}
}

void UGsUITrayNavigationBar::OnSetOptionalCurrency(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamInt* param = InParam->Cast<const FGsUIMsgParamInt>();
	if (param)
	{
		CurrencyType currency = static_cast<CurrencyType>(param->_data);
		SetOptionalCurrency(currency);
	}
}

void UGsUITrayNavigationBar::OnSetOptionalCurrencyItem(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamInt* param = InParam->Cast<const FGsUIMsgParamInt>();
	if (param)
	{
		SetOptionalCurrencyItem(param->_data);
	}
}

void UGsUITrayNavigationBar::OnHideCurrencyList()
{
	if (_bEnableDropDown)
	{
		SetOpenDropDownCurrency(false);
		InvalidateCurrencyList();
	}
}

void UGsUITrayNavigationBar::OnCreateCurrencyEntry(UWidget* InEntry)
{
	UGsUICurrencySlot* slot = Cast<UGsUICurrencySlot>(InEntry);
	if (slot)
	{
		// 클릭 이벤트 연결
		slot->OnClickCurrencySlot.BindUObject(this, &UGsUITrayNavigationBar::OnClickCurrencyEntry);
		slot->OnClickCurrencyCharge.BindUObject(this, &UGsUITrayNavigationBar::OnClickCurrencyCharge);
	}
}

void UGsUITrayNavigationBar::OnRefreshCurrencyEntry(int32 InIndex, UWidget* InEntry)
{	
	if (false == _currencyList.IsValidIndex(InIndex))
	{
		return;
	}

	// 슬롯 갱신
	UGsUICurrencySlot* slot = Cast<UGsUICurrencySlot>(InEntry);
	if (slot)
	{
		slot->SetDataLocalPlayer(_currencyList[InIndex]);

		// 골드는 +버튼 보여주지 않음
		if (CurrencyType::GOLD == _currencyList[InIndex])
		{
			slot->SetVisibilityBtnCharge(ESlateVisibility::Collapsed);
		}
	}
}

void UGsUITrayNavigationBar::OnClickCurrencyEntry(UWidget* InEntry)
{
	if (UGsUICurrencySlot* slot = Cast<UGsUICurrencySlot>(InEntry))
	{
		FGsCurrencyHelper::TryToNavigate(slot->GetCurrencyType());
	}
}

void UGsUITrayNavigationBar::OnClickCurrencyCharge(UWidget* InEntry)
{
	if (UGsUICurrencySlot* slot = Cast<UGsUICurrencySlot>(InEntry))
	{
		FGsCurrencyHelper::TryToNavigate(slot->GetCurrencyType());
	}
}

void UGsUITrayNavigationBar::OnClickDropDownCurrency()
{
	if (_bEnableDropDown)
	{
		SetOpenDropDownCurrency(!_bIsDropDownOpen);
		InvalidateCurrencyList();
	}
}
