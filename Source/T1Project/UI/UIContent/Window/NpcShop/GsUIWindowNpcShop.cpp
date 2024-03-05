// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowNpcShop.h"

#include "T1Project.h"
#include "SBoxPanel.h"
#include "PaperSprite.h"

#include "Components/Image.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsNpcShopManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsInventoryManager.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIControlLib/ContentWidget/GsCheckBox.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Popup/GsUIPopupCalculator.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Window/NpcShop/GsUIWeaponFilter.h"

#include "UTIL/GsTimeUtil.h"

#include "Components/ProgressBar.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"

#include "Item/GsItemManager.h"
#include "Item/GsItemHelper.h"

#include "NpcShop/GsNpcShopHelper.h"

#include "Message/MessageParam/GsNpcShopMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "GameFlow/GameContents/ContentsGame/NpcShop/MsgReceiver/GsNpcShopUIMsgReceiver.h"

#include "GameObject/Stat/GsStatInfo.h"

#include "UI/UIContent/Common/Icon/GsUIIconItemShop.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Currency/GsCurrencyHelper.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"


void UGsUIWindowNpcShop::BeginDestroy()
{
	if (nullptr != _buyListPanelSlotHelper)
	{
		_buyListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowNpcShop::ItemListRefreshEntry);
		_buyListPanelSlotHelper = nullptr;
	}
	if (nullptr != _autoBuyListPanelSlotHelper)
	{
		_autoBuyListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowNpcShop::AutoBuyItemListRefreshEntry);
		_autoBuyListPanelSlotHelper = nullptr;
	}
	if (nullptr != _autoSellListPanelSlotHelper)
	{
		_autoSellListPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowNpcShop::AutoSellItemListRefreshEntry);
		_autoSellListPanelSlotHelper = nullptr;
	}

	if (nullptr != _firstPageIconSlotHelper)
	{
		_firstPageIconSlotHelper = nullptr;
	}
	if (nullptr != _secondPageIconSlotHelper)
	{
		_secondPageIconSlotHelper = nullptr;
	}
	if (nullptr != _thirdPageIconSlotHelper)
	{
		_thirdPageIconSlotHelper = nullptr;
	}
	if (nullptr != _fourthPageIconSlotHelper)
	{
		_fourthPageIconSlotHelper = nullptr;
	}
	if (nullptr != _fifthPageIconSlotHelper)
	{
		_fifthPageIconSlotHelper = nullptr;
	}
	if (nullptr != _sixthPageIconSlotHelper)
	{
		_sixthPageIconSlotHelper = nullptr;
	}

	if (nullptr != _uiMsgReceiver)
	{
		delete _uiMsgReceiver;
		_uiMsgReceiver = nullptr;
	}

	_toggleGroup.Clear();
	_toggleGroupSetup.Clear();

	Super::BeginDestroy();
}

void UGsUIWindowNpcShop::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_endScrollOffset = 0.f;

	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowNpcShop::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowNpcShop::OnClickCloseAllStack);
	_btnProductListClose->OnClicked.AddDynamic(this, &UGsUIWindowNpcShop::OnClickCloseAllStack);
	_btnCloseSetup->OnClicked.AddDynamic(this, &UGsUIWindowNpcShop::OnClickCloseSetup);
	_btnSetupAutoOrder->OnClicked.AddDynamic(this, &UGsUIWindowNpcShop::OnClickSetup);

	_btnAutoResetSetting->OnClicked.AddDynamic(this, &UGsUIWindowNpcShop::OnClickResetAutoSetting);
	_btnAutoConfirmSetting->OnClicked.AddDynamic(this, &UGsUIWindowNpcShop::OnClickConfirmAutoSetting);

	_btnAutoBuy->OnClicked.AddDynamic(this, &UGsUIWindowNpcShop::OnClickAutoBuy);

	_btnConfirm->OnClicked.AddDynamic(this, &UGsUIWindowNpcShop::OnClickConfirm);

	_shoppingBasketScrollBox->OnUserScrolled.AddDynamic(this, &UGsUIWindowNpcShop::OnChangeMainScrollEvent);

	_toggleGroup.OnSlotClicked.BindUObject(this, &UGsUIWindowNpcShop::OnSlotClickedTab);
	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIWindowNpcShop::OnSelectChangedTab);

	if (nullptr == _buyListPanelSlotHelper)
	{
		_buyListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_buyListPanelSlotHelper->Initialize(_buyListEntryWidgetClass, _productListScrollBox);
		_buyListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowNpcShop::ItemListRefreshEntry);
	}
	_productListScrollBox->OnUserScrolled.AddDynamic(this, &UGsUIWindowNpcShop::OnScrollProductList);

	if (nullptr == _autoBuyListPanelSlotHelper)
	{
		_autoBuyListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_autoBuyListPanelSlotHelper->Initialize(_autoBuyListEntryWidgetClass, _panelAutoBuyItemList);
		_autoBuyListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowNpcShop::AutoBuyItemListRefreshEntry);
	}
	if (nullptr == _autoSellListPanelSlotHelper)
	{
		_autoSellListPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_autoSellListPanelSlotHelper->Initialize(_autoSellListEntryWidgetClass, _panelAutoSellItemList);
		_autoSellListPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowNpcShop::AutoSellItemListRefreshEntry);
	}

	_toggleGroupSetup.AddToggleWidget(_stupBuyToggleBtn);
	_toggleGroupSetup.AddToggleWidget(_stupSellToggleBtn);
	_toggleGroupSetup.OnSelectChanged.BindUObject(this, &UGsUIWindowNpcShop::OnSelectSetupTab);
	_toggleGroupSetup.SetSelectedIndex(0);	

	if (nullptr == _firstPageIconSlotHelper)
	{
		_firstPageIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_firstPageIconSlotHelper->Initialize(_firstPageIconSelector);
		_firstPageIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIWindowNpcShop::OnRefreshIconWrapBox_FristPage);
	}
	if (nullptr == _secondPageIconSlotHelper)
	{
		_secondPageIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_secondPageIconSlotHelper->Initialize(_secondPageIconSelector);
		_secondPageIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIWindowNpcShop::OnRefreshIconWrapBox_SecondPage);
	}
	if (nullptr == _thirdPageIconSlotHelper)
	{
		_thirdPageIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_thirdPageIconSlotHelper->Initialize(_thirdPageIconSelector);
		_thirdPageIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIWindowNpcShop::OnRefreshIconWrapBox_ThirdPage);
	}
	if (nullptr == _fourthPageIconSlotHelper)
	{
		_fourthPageIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_fourthPageIconSlotHelper->Initialize(_fourthPageIconSelector);
		_fourthPageIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIWindowNpcShop::OnRefreshIconWrapBox_fourthPage);
	}
	if (nullptr == _fifthPageIconSlotHelper)
	{
		_fifthPageIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_fifthPageIconSlotHelper->Initialize(_fifthPageIconSelector);
		_fifthPageIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIWindowNpcShop::OnRefreshIconWrapBox_fifthPage);
	}
	if (nullptr == _sixthPageIconSlotHelper)
	{
		_sixthPageIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_sixthPageIconSlotHelper->Initialize(_sixthPageIconSelector);
		_sixthPageIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIWindowNpcShop::OnRefreshIconWrapBox_sixthPage);
	}

	_shopProductListTabWidgetMap.Empty();
	_shopProductListTabWidgetMap.Emplace(ENpcShopProductListTab::ProductList_AllTab, _checkBox_All);
	_shopProductListTabWidgetMap.Emplace(ENpcShopProductListTab::ProductList_Filter, _checkBox_Filter);
	_shopProductListTabWidgetMap.Emplace(ENpcShopProductListTab::ProductList_ConsumableTab, _checkBox_Potion);
	_shopProductListTabWidgetMap.Emplace(ENpcShopProductListTab::ProductList_MaterialTab, _checkBox_etc);
	_shopProductListTabWidgetMap.Emplace(ENpcShopProductListTab::ProductList_Weapon, _checkBox_EquipWeapon);
	_shopProductListTabWidgetMap.Emplace(ENpcShopProductListTab::ProductList_Armor, _checkBox_EquipArmor);
	_shopProductListTabWidgetMap.Emplace(ENpcShopProductListTab::ProductList_Accessory, _checkBox_EquipAccessory);

	TArray<ENpcShopProductListTab> tabList;
	tabList.Emplace(ENpcShopProductListTab::ProductList_AllTab);
	tabList.Emplace(ENpcShopProductListTab::ProductList_ConsumableTab);
	tabList.Emplace(ENpcShopProductListTab::ProductList_MaterialTab);
	_npcFunctionTypeTabListMap.Emplace(NpcFunctionType::SHOP, tabList);

	tabList.Empty();
	tabList.Emplace(ENpcShopProductListTab::ProductList_AllTab);
	_npcFunctionTypeTabListMap.Emplace(NpcFunctionType::EVENT_SHOP, tabList);

	tabList.Empty();
	tabList.Emplace(ENpcShopProductListTab::ProductList_AllTab);
	tabList.Emplace(ENpcShopProductListTab::ProductList_Weapon);
	tabList.Emplace(ENpcShopProductListTab::ProductList_Armor);
	tabList.Emplace(ENpcShopProductListTab::ProductList_Accessory);
	_npcFunctionTypeTabListMap.Emplace(NpcFunctionType::SHOP_EQUIP, tabList);

	tabList.Empty();
	tabList.Emplace(ENpcShopProductListTab::ProductList_AllTab);
	tabList.Emplace(ENpcShopProductListTab::ProductList_Filter);
	_npcFunctionTypeTabListMap.Emplace(NpcFunctionType::SHOP_SKILL_BOOK, tabList);

	_uiMsgReceiver = new FGsNpcShopUIMsgReceiver();
	_timer = NewObject<UGsSimpleTimer>();
	float min;
	float max;
	_curve->GetTimeRange(min, max);
	_timer->SetMaxTime(max);

	_timer->Stop();

	_uiWeaponFilter->OnWeaponFilterSave.Unbind();
	_uiWeaponFilter->OnWeaponFilterSave.BindUObject(this, &UGsUIWindowNpcShop::OnWeaponFilterSave);
}

void UGsUIWindowNpcShop::NativeConstruct()
{
	Super::NativeConstruct();

	_currentInvenTabIndex = 0;

	_leftPopupSwitcher->SetActiveWidgetIndex(ProductAreaType::DisPlay_Product);

	if (false == _uiMsgReceiver->IsValidUI())
	{
		_uiMsgReceiver->SetUI(this);
	}
	_uiMsgReceiver->RegisterMessage();

	_eShoppingBasketSlotIndexType = ShoppingBasketSlotIndexType::FirstSlotGroupIndex;
	_eShoppingBasketPageType = ShoppingBasketPage::FirstPage;
	_eScrollDirectionType = ScrollDircetionType::Scroll_None;

	_timer->Stop();
	_isScrolling = false;
	_isAutoScrolling = false;
	_findEndScrollOffeset = false;
	_isBuyMode = true;
	_slotMoveCount = 0;

	InvalidateAllShoppingBasketUI();
	SetFocusingIndexSwitcher(ScrollDircetionType::Scroll_None);
}

void UGsUIWindowNpcShop::NativeDestruct()
{
	CloseCalculatorUI();

	FGsItemDetailCloseMessageParam param;
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, &param);

	_firstPageIconSlotHelper->Clear();
	_secondPageIconSlotHelper->Clear();
	_thirdPageIconSlotHelper->Clear();
	_fourthPageIconSlotHelper->Clear();
	_fifthPageIconSlotHelper->Clear();
	_sixthPageIconSlotHelper->Clear();

	_shopGroupIdList.Empty();
	_uiMsgReceiver->RemoveMsg();

	_disPlayItemInfoList.Empty();
	_shoppingBasketSlotPositionMap.Empty();

	SetTutorial(INVALID_SKILL_ID);
	SetSlotClickLock(false);

	_shopNpcFunctionType = NpcFunctionType::NONE;

	Super::NativeDestruct();
}

void UGsUIWindowNpcShop::InvalidateAllShoppingBasketUI()
{
	_firstPageIconSlotHelper->RefreshAll(SHOPPINGBASKET_SLOT_COUNT);
	_secondPageIconSlotHelper->RefreshAll(SHOPPINGBASKET_SLOT_COUNT);
	_thirdPageIconSlotHelper->RefreshAll(SHOPPINGBASKET_SLOT_COUNT);
	_fourthPageIconSlotHelper->RefreshAll(SHOPPINGBASKET_SLOT_COUNT);
	_fifthPageIconSlotHelper->RefreshAll(SHOPPINGBASKET_SLOT_COUNT);
	_sixthPageIconSlotHelper->RefreshAll(SHOPPINGBASKET_SLOT_COUNT);

	InvalidateText();
}

void UGsUIWindowNpcShop::InvalidateShoppingBasketScroll(int32 InSlotIndex)
{
	if (0 <= InSlotIndex)
	{
		// 패널 초기화
		for (int32 i = 0; i < _slotMoveCount; ++i)
		{
			SetSlotMove(ShoppingBasketSlotIndexType::FirstSlotGroupIndex, ShoppingBasketSlotIndexType::SixthSlotGroupIndex);
		}
		for (int32 i = _slotMoveCount; i < 0; ++i)
		{
			SetSlotMove(ShoppingBasketSlotIndexType::SixthSlotGroupIndex, ShoppingBasketSlotIndexType::FirstSlotGroupIndex);
		}

		// 패널 위치 재설정 및 스크롤 위치 설정
		int32 pageIndex = InSlotIndex / SHOPPINGBASKET_SLOT_COUNT;
		ShoppingBasketSlotIndexType nextIndex = static_cast<ShoppingBasketSlotIndexType>(pageIndex);
		if (nextIndex == ShoppingBasketSlotIndexType::FirstSlotGroupIndex)
		{
			_slotMoveCount = 1;
			SetSlotMove(ShoppingBasketSlotIndexType::SixthSlotGroupIndex, ShoppingBasketSlotIndexType::FirstSlotGroupIndex);
			SetSlotPositionValueByIndexType(ShoppingBasketSlotIndexType::SecondSlotGroupIndex);
		}
		else if (nextIndex == ShoppingBasketSlotIndexType::SixthSlotGroupIndex)
		{
			_slotMoveCount = -1;
			SetSlotMove(ShoppingBasketSlotIndexType::FirstSlotGroupIndex, ShoppingBasketSlotIndexType::SixthSlotGroupIndex);
			SetSlotPositionValueByIndexType(ShoppingBasketSlotIndexType::FifthSlotGroupIndex);
		}
		else
		{
			_slotMoveCount = 0;
			SetSlotPositionValueByIndexType(nextIndex);
		}

		_eShoppingBasketPageType = static_cast<ShoppingBasketPage>(pageIndex);
		SetFocusingIndexSwitcher(ScrollDircetionType::Scroll_None);
	}
}

void UGsUIWindowNpcShop::InvalidateText()
{
	UGsNpcShopManager* npcShopManager = GSNpcShop();
	if (nullptr == npcShopManager)
		return;

	// 예상 가격 계산
	Currency expectPrice = 0;
	const TArray<FGsNpcShopQuickSlotItemInfo>& basketList = npcShopManager->GetBasketList();
	for (const FGsNpcShopQuickSlotItemInfo& basketItemInfo : basketList)
	{
		// 구매모드
		if (_isBuyMode)
		{
			TArray<FGsNpcShopItemInfo*> shopList = npcShopManager->GetDisPlayItemListByGroupIDAndType(_shopNpcFunctionType, _shopGroupIdList, ENpcShopProductListTab::ProductList_AllTab);
			for (int32 j = 0; j < shopList.Num(); ++j)
			{
				if (basketItemInfo._itemTID == shopList[j]->_itemTID)
				{
					expectPrice += basketItemInfo._itemAmount * shopList[j]->GetCurrencyAmount(_shopNpcFunctionType);
					continue;
				}
			}
		}
		// 판매모드
		else
		{
			Currency eachSellPrice = 0;
			if (const FGsSchemaItemCommon* itemCommonData = FGsItem::GetTableDataByID(basketItemInfo._itemTID))
			{
				eachSellPrice = itemCommonData->sellPrice;
			}

			expectPrice += basketItemInfo._itemAmount * eachSellPrice;
		}
	}

	_shopPriceText->SetText(FText::AsNumber(expectPrice));

	// 예상 무게 텍스트
	int32 expectWeight = npcShopManager->GetExpectWeightValue(_isBuyMode);
	float expectWeightPercent = FGsItemHelper::GetWeightPercent(expectWeight);

	FText weightPercentText;
	FGsItemHelper::GetWeightPercentText(expectWeightPercent, weightPercentText);
	_expectWeightPerText->SetText(weightPercentText);

	float progressValue = expectWeightPercent > 0.f ? expectWeightPercent / 100.f : 0.f;
	_expectWeightProgressBar->SetPercent(progressValue);

	float weightPenaltyPercentFirst = GSInventory()->GetOwnedItemPenaltyValueByType(WeightPenaltyGrade::FIRST);
	float weightPenaltyPercentSecond = GSInventory()->GetOwnedItemPenaltyValueByType(WeightPenaltyGrade::SECOND);

	// 페널티 적용을 받지 않을 때
	EGsUIColorType penaltyColor = EGsUIColorType::WEIGHT_NORMAL;
	// 2단계 페널티 이상일 때
	if (expectWeightPercent >= weightPenaltyPercentSecond)
	{
		penaltyColor = EGsUIColorType::WEIGHT_PENALTY_TWO;
	}
	// 1단계 페널티 이상일 때
	else if (expectWeightPercent >= weightPenaltyPercentFirst)
	{
		penaltyColor = EGsUIColorType::WEIGHT_PENALTY_ONE;
	}

	_expectWeightProgressBar->SetFillColorAndOpacity(FGsUIColorHelper::GetColor(penaltyColor));
	_expectWeightPerText->SetColorAndOpacity(FGsUIColorHelper::GetColor(penaltyColor));

	if (FGsCurrencyHelper::IsTaxNpcType(_shopNpcFunctionType))
	{
		_panelTaxInfo->SetVisibility(ESlateVisibility::HitTestInvisible);

		_textBlockTotalTax->SetText(FGsCurrencyHelper::GetTextTotalTax(CurrencyType::GOLD));
		_textBlockTaxDetail->SetText(FGsCurrencyHelper::GetTextTaxInfo(CurrencyType::GOLD));
	}
	else
	{
		_panelTaxInfo->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIWindowNpcShop::OnSlotClickedTab(int32 InIndex, bool bInIsSelected)
{
	ENpcShopProductListTab tabType = GetProductListTabType(_shopNpcFunctionType, InIndex);
	if (tabType == ENpcShopProductListTab::ProductList_Filter)
	{
		_uiWeaponFilter->ShowOrHide();
	}
	else
	{
		_uiWeaponFilter->Hide();
		_productListScrollBox->ScrollToStart();
		_productListScrollBox->EndInertialScrolling();
	}
}

void UGsUIWindowNpcShop::OnSelectChangedTab(int32 InIndex)
{
	if (_currentInvenTabIndex == InIndex)
		return;

	ENpcShopProductListTab tabType = GetProductListTabType(_shopNpcFunctionType, InIndex);
	if (tabType == ENpcShopProductListTab::ProductList_Filter)
	{
		if (UGsNpcShopManager* npcShopManager = GSNpcShop())
		{
			if (npcShopManager->IsFilterEmpty())
			{
				_toggleGroup.SetSelectedIndex(_currentInvenTabIndex);
				return;
			}
		}
	}

	_currentInvenTabIndex = InIndex;
	InvalidateProductItemList(_shopNpcFunctionType, _shopGroupIdList, tabType, _isBuyMode);

	FGsItemDetailCloseMessageParam param;
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, &param);
	CloseCalculatorUI();
}

void UGsUIWindowNpcShop::ChangeShopTab(const NpcFunctionType InType, const int32 InIndex)
{
	_toggleGroup.Clear();

	for (auto& checkBox : _shopProductListTabWidgetMap)
	{
		checkBox.Value->SetVisibility(ESlateVisibility::Collapsed);
	}

	TArray<ENpcShopProductListTab>& tabList = _npcFunctionTypeTabListMap[_shopNpcFunctionType];
	for (auto& tab : tabList)
	{
		if (true == _shopProductListTabWidgetMap.Contains(tab))
		{
			_shopProductListTabWidgetMap[tab]->SetVisibility(ESlateVisibility::Visible);
		}
	}
	_toggleGroup.AddToggleWidgetByParentPanel(_panelInvenTab);
	_toggleGroup.SetSelectedIndex(InIndex);
}

ENpcShopProductListTab UGsUIWindowNpcShop::GetProductListTabType(NpcFunctionType InFunctionType, int32 InIndex)
{
	if (false == _npcFunctionTypeTabListMap.Contains(InFunctionType))
		return ENpcShopProductListTab::ProductList_AllTab;

	if(InIndex >= _npcFunctionTypeTabListMap[InFunctionType].Num())
		return ENpcShopProductListTab::ProductList_AllTab;

	return _npcFunctionTypeTabListMap[InFunctionType][InIndex];
}

void UGsUIWindowNpcShop::OnClickEntryWidget(UGsUINpcShopProductEntry* InEntryUI)
{
	if (_slotClickLock)
	{
		return;
	}

	FGsItemDetailCloseMessageParam param;
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, &param);

	if (nullptr == InEntryUI)
		return;

	FGsNpcShopItemInfo* targetInfo = InEntryUI->GetShopProductInfo();
	if (nullptr == targetInfo)
		return;

	// 선택 초기화
	ResetSelctionInfo();

	// 테이블에서 설정한 구매제한으로 구매가 불가능한지 여부 체크(level 등등)
	UGsNpcShopManager* npcShopManager = GSNpcShop();
	if (nullptr == npcShopManager)
		return;

	EGsNotAllowedBuyReason type = EGsNotAllowedBuyReason::NONE;
	if (false == FGsNpcShopHelper::IsAllowedBuyItem(targetInfo, type))
	{
		FGsNpcShopHelper::ShowTickerNotAllowedBuy(type);
		CloseCalculatorUI();
		return;
	}

	// 가격,인벤무게,인벤슬롯 카운트 체크
	ItemAmount maximumCount = npcShopManager->GetMaximumCalculatorCount(_shopNpcFunctionType, targetInfo, type);
	if (0 >= maximumCount)
	{
		if (type == EGsNotAllowedBuyReason::NOT_ENOUGH_COST)
		{
			if (CurrencyType::GOLD == InEntryUI->_shopProduct->_currencyType)
			{
				Currency basketAmount = GetTaxIncludedPriceInBasket();
				Currency targetAmount = targetInfo->GetCurrencyAmount(_shopNpcFunctionType);
				Currency myAmount = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GOLD);

				// 골드 부족 시엔 팝업으로 띄움
				FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType::GOLD, basketAmount + targetAmount - myAmount);
				return;
			}
		}
		FGsNpcShopHelper::ShowTickerNotAllowedBuy(type);
		return;
	}

	// 누른 슬롯의 하이라이트 설정
	_curSelectedEntryInfo = InEntryUI;
	_curSelectedEntryInfo->SetSlotSelection(true);

	// 계산기 Open 또는 단일 아이템을 장바구니에 추가
	bool isStackable = false;
	if (const FGsSchemaItemCommon* itemCommonData = FGsItem::GetTableDataByID(_curSelectedEntryInfo->GetProductItemTID()))
	{
		isStackable = (1 < itemCommonData->maxStackCount);
	}

	if (isStackable && 1 < maximumCount)
	{
		if (IsOpenedCalculatorUI())
		{
			InitCalculatorUI();
		}
		else
		{
			OpenCalculatorUI();
		}
	}
	else
	{
		ConfirmItemStackCount(1);
	}
}

void UGsUIWindowNpcShop::OpenCalculatorUI()
{
	//if (_bIsTutorial)
	//{
	//	return;
	//}

	if (UGsUIManager* uiManager = GUI())
	{
		if (nullptr != uiManager->OpenAndGetWidget(TEXT("PopupCalculator")))
		{
			EGsNotAllowedBuyReason type = EGsNotAllowedBuyReason::NONE;
			ItemAmount maximumCount = GSNpcShop()->GetMaximumCalculatorCount(_shopNpcFunctionType, _curSelectedEntryInfo->GetShopProductInfo(), type);

			UGsUIPopupCalculator* popup = Cast<UGsUIPopupCalculator>(uiManager->GetWidgetByKey(TEXT("PopupCalculator")));
			popup->SetResetCount();
			popup->SetMaximumCount(maximumCount);
			popup->SetMinimumCount(0);
			popup->SetDefaultCount(1);
			popup->SetNpcShopCalcInfo(true);
			popup->InvalidateNumText();
			popup->SetTouchState(false);

			if (UGsGameObjectLocalPlayer* player = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer)))
			{
				if (const FGsStatInfo* statInfo = player->GetCreatureStatInfoByType(StatType::MAX_WEIGHT))
				{
					const FGsSchemaItemCommon* itemData = GItem()->GetItemTableDataByTID(_curSelectedEntryInfo->GetShopProductInfo()->_itemTID);
					popup->SetWeight(itemData->weight, statInfo->GetStatValue(), GSNpcShop()->GetExpectWeightValue(true));
					popup->SetMode(true);
				}
			}

			if (false == popup->OnFinishCalculateDelegate.IsBoundToObject(this))
			{
				popup->OnFinishCalculateDelegate.AddUObject(this, &UGsUIWindowNpcShop::ConfirmItemStackCount);
				popup->OnCancelCalculateDelegate.AddUObject(this, &UGsUIWindowNpcShop::ResetSelctionInfo);
			}
		}
	}
}

void UGsUIWindowNpcShop::CloseCalculatorUI()
{
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->CloseByKeyName(TEXT("PopupCalculator"));
		ResetSelctionInfo();
	}
}

void UGsUIWindowNpcShop::InitCalculatorUI()
{
	if (UGsUIManager* uiManager = GUI())
	{
		if (UGsUIPopupCalculator* popup = Cast<UGsUIPopupCalculator>(uiManager->GetWidgetByKey(TEXT("PopupCalculator"))))
		{
			EGsNotAllowedBuyReason type = EGsNotAllowedBuyReason::NONE;
			ItemAmount maximumCount = GSNpcShop()->GetMaximumCalculatorCount(_shopNpcFunctionType, _curSelectedEntryInfo->GetShopProductInfo(), type);

			popup->SetResetCount();
			popup->SetMaximumCount(maximumCount);
			popup->SetMinimumCount(0);
			popup->SetDefaultCount(1);
			popup->SetNpcShopCalcInfo(true);
			popup->InvalidateNumText();
			popup->SetTouchState(false);

			if (UGsGameObjectLocalPlayer* player = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer)))
			{
				if (const FGsStatInfo* statInfo = player->GetCreatureStatInfoByType(StatType::MAX_WEIGHT))
				{
					const FGsSchemaItemCommon* itemData = GItem()->GetItemTableDataByTID(_curSelectedEntryInfo->GetShopProductInfo()->_itemTID);
					popup->SetWeight(itemData->weight, statInfo->GetStatValue(), GSNpcShop()->GetExpectWeightValue(true));
					popup->SetMode(true);
				}
			}

			if (false == popup->OnFinishCalculateDelegate.IsBoundToObject(this))
			{
				if (popup->OnResetCalculateDelegate.IsBound())
					popup->OnResetCalculateDelegate.Broadcast();
				popup->OnResetCalculateDelegate.Clear();
				popup->OnResetCalculateDelegate.AddUObject(this, &UGsUIWindowNpcShop::ResetSelctionInfo);
				popup->OnFinishCalculateDelegate.Clear();
				popup->OnFinishCalculateDelegate.AddUObject(this, &UGsUIWindowNpcShop::ConfirmItemStackCount);
				popup->OnCancelCalculateDelegate.Clear();
				popup->OnCancelCalculateDelegate.AddUObject(this, &UGsUIWindowNpcShop::ResetSelctionInfo);
			}
		}
	}
}

bool UGsUIWindowNpcShop::IsOpenedCalculatorUI()
{
	if (UGsUIManager* uiManager = GUI())
	{
		return uiManager->IsActiveWidget(TEXT("PopupCalculator"));
	}
	return false;
}

void UGsUIWindowNpcShop::ConfirmItemStackCount(int64 InCount)
{
	if (nullptr == _curSelectedEntryInfo)
		return;

	TArray<FGsBasketTryInputInfo> itemList;
	itemList.Emplace(0, _curSelectedEntryInfo->GetProductItemTID(), _curSelectedEntryInfo->GetProductId(), InCount);
	FGsInputShoppingBasket param(itemList, true, true);
	GMessage()->GetNpcShop().SendMessage(MessageContentNpcShop::INSERT_BASKET, &param);

	CloseCalculatorUI();
}

void UGsUIWindowNpcShop::ResetSelctionInfo()
{
	if (nullptr != _curSelectedEntryInfo)
	{
		_curSelectedEntryInfo->SetSlotSelection(false);
		_curSelectedEntryInfo = nullptr;
	}
}

void UGsUIWindowNpcShop::InvalidateProductItemList(const NpcFunctionType InNpcFunctionType, const TArray<int32>& InShopGroupIdList, const ENpcShopProductListTab InTabType, const bool InIsBuyMode)
{
	if (0 >= InShopGroupIdList.Num())
		return;

	_shopNpcFunctionType = InNpcFunctionType;
	_shopGroupIdList = InShopGroupIdList;
	_shopTabType = InTabType;
	_isBuyMode = InIsBuyMode;

	UpdateShopItemList();

	// _shopNpcFunctionType에 따른 세금정보 출력 때문에 텍스트도 갱신
	InvalidateText();
}

void UGsUIWindowNpcShop::InvalidateShopType(const NpcFunctionType InNpcFunctionType)
{
	_shopNpcFunctionType = InNpcFunctionType;
	_isCurrencyDirty = true;

	ChangeShopTab(InNpcFunctionType, _currentInvenTabIndex);

	switch (InNpcFunctionType)
	{
	case NpcFunctionType::SHOP:
	{
		_btnSetupAutoOrder->SetVisibility(ESlateVisibility::Visible);
		_btnAutoBuy->SetVisibility(ESlateVisibility::Visible);
		_imageDevideLineUnder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_sizeBoxUnder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	break;
	case NpcFunctionType::SHOP_EQUIP:
	case NpcFunctionType::SHOP_SKILL_BOOK:
	case NpcFunctionType::EVENT_SHOP:
	{
		_btnSetupAutoOrder->SetVisibility(ESlateVisibility::Collapsed);
		_btnAutoBuy->SetVisibility(ESlateVisibility::Collapsed);
		_imageDevideLineUnder->SetVisibility(ESlateVisibility::Collapsed);
		_sizeBoxUnder->SetVisibility(ESlateVisibility::Collapsed);
	}
	break;
	}
}

void UGsUIWindowNpcShop::InvalidateShopMode(const bool InIsBuyMode)
{
	_isBuyMode = InIsBuyMode;

	FText findText;
	FTextKey textKey = true == _isBuyMode ? TEXT("BuyList") : TEXT("SellList");
	FText::FindText(TEXT("NpcShopText"), textKey, findText);
	_shopActionModeText->SetText(findText);

	FText findText2;
	FTextKey textKey2 = true == _isBuyMode ? TEXT("BuyTotalPrice") : TEXT("SellTotalPrice");
	FText::FindText(TEXT("NpcShopText"), textKey2, findText2);
	_shopPriceGuideText->SetText(findText2);


	/*
	
	UTextBlock* _shopPriceText;
	*/

	// 구매록록 <-> 판매록록
	// 구매금액 <-> 판매금액
	// 퀵슬롯 강제로 첫번째로 이동
}

void UGsUIWindowNpcShop::InvalidateCurrencyIcon(const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId)
{
	if (CostType::CURRENCY == InCostType)
	{
		UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(FGsCurrencyHelper::GetCurrencyIconPath(InCurrencyType));
		_imgCurrencyIcon->SetBrushFromAtlasInterface(spriteImg);

		FGsUIMsgParamInt param(static_cast<int32>(CurrencyType::NONE));
		GMessage()->GetUI().SendMessage(MessageUI::SET_NAVIGATION_BAR_CURRENCY, &param);
	}
	else if (CostType::ITEM == InCostType)
	{
		const UGsTableItemCommon* table = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
		if (table)
		{
			const FGsSchemaItemCommon* row;
			if (table->FindRowById(InItemId, row))
			{
				UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(row->icon);
				_imgCurrencyIcon->SetBrushFromAtlasInterface(spriteImg);
			}
		}

		FGsUIMsgParamInt param(InItemId);
		GMessage()->GetUI().SendMessage(MessageUI::SET_NAVIGATION_BAR_ITEM, &param);
	}
}

void UGsUIWindowNpcShop::ItemListRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	UGsUINpcShopProductEntry* entry = Cast<UGsUINpcShopProductEntry>(InEntry);
	if (nullptr == entry)
		return;

	if (false == _disPlayItemInfoList.IsValidIndex(InIndex))
		return;

	if (false == entry->OnSelectProductItemDelegate.IsBoundToObject(this))
	{
		entry->OnSelectProductItemDelegate.AddUObject(this, &UGsUIWindowNpcShop::OnClickEntryWidget);
	}
	entry->SetItemInfo(_disPlayItemInfoList[InIndex], InIndex, _shopNpcFunctionType);
}

void UGsUIWindowNpcShop::OnChangeMainScrollEvent(float InCurOffset)
{	
	//GSLOG(Warning, TEXT("OnChangeMainScrollEvent - InCurOffset : %f"), InCurOffset);
	//GSLOG(Warning, TEXT("OnChangeMainScrollEvent : aver : %f"), _shoppingBasketScrollBox->GetScrollOffsetOfEnd() / 5.f);

	if (true == _shoppingBasketScrollBox->HasMouseCapture())
	{
		if (true == _isAutoScrolling)
			return;

		_isScrolling = true;
		_timer->Stop();
	}
	else
	{
		if (false == _isAutoScrolling && false == _isScrolling)
		{
			_shoppingBasketScrollBox->SetScrollOffset(GetSlotPositionValueByIndexType(_eShoppingBasketSlotIndexType));
		}

		return;
	}

	float curPagePosValue = GetSlotPositionValueByIndexType(_eShoppingBasketSlotIndexType);
	float offsetValue = curPagePosValue - InCurOffset;
	float convertAbs = fabsf(offsetValue);
	/*
	GSLOG(Warning, TEXT("curPagePosValue : %f"), curPagePosValue);
	GSLOG(Warning, TEXT("offsetValue : %f"), offsetValue);
	GSLOG(Warning, TEXT("convertAbs : %f"), convertAbs);
	GSLOG(Warning, TEXT("InCurOffset : %f"), InCurOffset);
	*/
	if (0 < offsetValue)
	{
		_eScrollDirectionType = convertAbs >= AUTO_SCROLL_TERM_VALUE ? ScrollDircetionType::Scroll_Left
			: ScrollDircetionType::Scroll_Return_Left;	
	}
	else
	{
		_eScrollDirectionType = convertAbs >= AUTO_SCROLL_TERM_VALUE ? ScrollDircetionType::Scroll_Right
			: ScrollDircetionType::Scroll_Return_Right;
	}

	//GSLOG(Warning, TEXT("_eScrollDirectionType : %d"), (uint8)_eScrollDirectionType);
}

void UGsUIWindowNpcShop::SetDefaultFocusingPos()
{
	float calcOffset = _endScrollOffset / (float)ShoppingBasketSlotIndexType::SixthSlotGroupIndex;
	for (uint8 i = 0; i < ShoppingBasketSlotIndexType::SlotGroupMax; ++i)
	{
		_shoppingBasketSlotPositionMap.Emplace((ShoppingBasketSlotIndexType)i , i * calcOffset);
	}

	SetSlotPositionValueByIndexType(ShoppingBasketSlotIndexType::FirstSlotGroupIndex);
}

void UGsUIWindowNpcShop::SetSlotPositionValueByIndexType(const ShoppingBasketSlotIndexType InType)
{
	_shoppingBasketScrollBox->SetScrollOffset(GetSlotPositionValueByIndexType(InType));
	_eShoppingBasketSlotIndexType = InType;
}

float UGsUIWindowNpcShop::GetSlotPositionValueByIndexType(const ShoppingBasketSlotIndexType InType)
{
	if (false == _shoppingBasketSlotPositionMap.Contains(InType))
		return 0.f;

	return _shoppingBasketSlotPositionMap[InType];
}

void UGsUIWindowNpcShop::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// slateUI 에서 스크롤 end 되는 시점을 알 수 없기에 tick 처리에서 값을 가져와서 초기 세팅함
	if (false == _findEndScrollOffeset)
	{
		float curOffset = _shoppingBasketScrollBox->GetScrollOffset();
		if (0.f < curOffset)
		{
			if (0.f >= _endScrollOffset)
			{
				_endScrollOffset = curOffset;
			}
			
			_findEndScrollOffeset = true;

			SetDefaultFocusingPos();
			CurrentFocusingSlotPositionCheck();
		}
	}

	// 스크롤 하다가 릴리즈 한 상태
	if (false == _shoppingBasketScrollBox->HasMouseCapture() && true == _isScrolling)
	{
		// 다음틱에서..
		if (false == _timer->CanTick())
		{
			_timer->Reset();
			_timer->Start();
			return;
		}

		if (_timer->IsFinish())
		{
			_timer->Stop();
		}

		_timer->Tick(InDeltaTime);

		if (_curve)
		{
			float curveValue = _curve->GetFloatValue(_timer->GetCurrentTime());
			AutoScroll(curveValue);
		}
	}
}

void UGsUIWindowNpcShop::AutoScroll(float InCurveData)
{
	_isAutoScrolling = true;

	float curOffset = _shoppingBasketScrollBox->GetScrollOffset();
	float curSlotPos = GetSlotPositionValueByIndexType(_eShoppingBasketSlotIndexType);
	float endOffset = _shoppingBasketScrollBox->GetScrollOffsetOfEnd();
	
	switch (_eScrollDirectionType)
	{
	case ScrollDircetionType::Scroll_None:
		_isAutoScrolling = false;
		break;
	case ScrollDircetionType::Scroll_Return_Left:
	{
		// 도착했으면 강제 setOffset
		if (curOffset > curSlotPos)
		{
			_shoppingBasketScrollBox->SetScrollOffset(curOffset - InCurveData);
		}
		else
		{
			_isScrolling = false;
			_isAutoScrolling = false;
			_timer->Stop();
			_shoppingBasketScrollBox->SetScrollOffset(curSlotPos);
			_eScrollDirectionType = ScrollDircetionType::Scroll_None;
		}
	}
	break;

	case ScrollDircetionType::Scroll_Return_Right:
	{
		// 도착했으면 강제 setOffset
		if (curOffset < curSlotPos)
		{
			_shoppingBasketScrollBox->SetScrollOffset(curOffset + InCurveData);
		}
		else
		{
			_isScrolling = false;
			_isAutoScrolling = false;
			_timer->Stop();
			_shoppingBasketScrollBox->SetScrollOffset(curSlotPos);
			_eScrollDirectionType = ScrollDircetionType::Scroll_None;
		}
	}
	break;

	case ScrollDircetionType::Scroll_Left:
	{
		float value = GetSlotPositionValueByIndexType(GetExpectPosType(false));
		if (value < curOffset)
		{
			_shoppingBasketScrollBox->SetScrollOffset(curOffset - InCurveData);
		}
		else
		{
			_isScrolling = false;
			_isAutoScrolling = false;
			_timer->Stop();

			SetSlotPositionValueByIndexType(GetExpectPosType(false));
			CurrentFocusingSlotPositionCheck();
			_eScrollDirectionType = ScrollDircetionType::Scroll_None;
			SetFocusingIndexSwitcher(ScrollDircetionType::Scroll_Left);
		}
	}
	break;

	case ScrollDircetionType::Scroll_Right:
	{
		float value = GetSlotPositionValueByIndexType(GetExpectPosType(true));
		if (value > curOffset)
		{
			_shoppingBasketScrollBox->SetScrollOffset(curOffset + InCurveData);
		}
		else
		{
			_isScrolling = false;
			_isAutoScrolling = false;
			_timer->Stop();

			SetSlotPositionValueByIndexType(GetExpectPosType(true));
			CurrentFocusingSlotPositionCheck();
			_eScrollDirectionType = ScrollDircetionType::Scroll_None;
			SetFocusingIndexSwitcher(ScrollDircetionType::Scroll_Right);
		}
	}
	break;

	}
}

// 현재 indexType 을 기준으로 이전 , 이후 타입을 가져옴( true == InIsAfter 이후 , 반대 이전)
UGsUIWindowNpcShop::ShoppingBasketSlotIndexType UGsUIWindowNpcShop::GetExpectPosType(bool InIsAfter)
{
	if (_eShoppingBasketSlotIndexType == ShoppingBasketSlotIndexType::FirstSlotGroupIndex)
	{
		return false == InIsAfter ? ShoppingBasketSlotIndexType::SixthSlotGroupIndex : ShoppingBasketSlotIndexType::SecondSlotGroupIndex;
	}
	else if (_eShoppingBasketSlotIndexType == ShoppingBasketSlotIndexType::SixthSlotGroupIndex)
	{
		return true == InIsAfter ? ShoppingBasketSlotIndexType::FirstSlotGroupIndex : ShoppingBasketSlotIndexType::FifthSlotGroupIndex;
	}

	return true == InIsAfter ? (ShoppingBasketSlotIndexType)(_eShoppingBasketSlotIndexType + 1)
		: (ShoppingBasketSlotIndexType)(_eShoppingBasketSlotIndexType - 1);
}

// 현재 포커싱 해야하는 슬롯 페이지 인덱스에 맞게 scroll 포커싱 해주고
// +1 , -1 슬롯 페이지 포지션을 체크하여, 비어있다면 해당 page 에 맞게 채워줌
void UGsUIWindowNpcShop::CurrentFocusingSlotPositionCheck()
{
	float curOffset = _shoppingBasketScrollBox->GetScrollOffset();
	float startOffset = GetSlotPositionValueByIndexType(ShoppingBasketSlotIndexType::FirstSlotGroupIndex);
	float endOffset = GetSlotPositionValueByIndexType(ShoppingBasketSlotIndexType::SixthSlotGroupIndex);
	float calcStartOffset = curOffset - startOffset;
	float calcEndOffset = curOffset - endOffset;

	float reusltStartOffset = fabs(calcStartOffset);
	float reusltEndOffset = fabs(calcEndOffset);

	if (FISNISHED_GOAL_POSITION_VALIDCHECK_VALUE >= reusltStartOffset) // start pos 
	{
		++_slotMoveCount;
		SetSlotMove(ShoppingBasketSlotIndexType::SixthSlotGroupIndex, ShoppingBasketSlotIndexType::FirstSlotGroupIndex);
		SetSlotPositionValueByIndexType(ShoppingBasketSlotIndexType::SecondSlotGroupIndex);
	}
	else if (FISNISHED_GOAL_POSITION_VALIDCHECK_VALUE >= reusltEndOffset) // end Pos
	{
		--_slotMoveCount;
		SetSlotMove(ShoppingBasketSlotIndexType::FirstSlotGroupIndex, ShoppingBasketSlotIndexType::SixthSlotGroupIndex);
		SetSlotPositionValueByIndexType(ShoppingBasketSlotIndexType::FifthSlotGroupIndex);
	}

	/*
	GSLOG(Warning, TEXT("[CurrentFocusingSlotPositionCheck] - curOffset : %f"), curOffset);
	GSLOG(Warning, TEXT("[CurrentFocusingSlotPositionCheck] - startOffset : %f"), startOffset);
	GSLOG(Warning, TEXT("[CurrentFocusingSlotPositionCheck] - endOffset : %f"), endOffset);

	GSLOG(Warning, TEXT("[CurrentFocusingSlotPositionCheck] - resultOffset : %f"), resultOffset);
	GSLOG(Warning, TEXT("[CurrentFocusingSlotPositionCheck] - resultOffset2 : %f"), resultOffset2);

	GSLOG(Warning, TEXT("[CurrentFocusingSlotPositionCheck] - _eShoppingBasketSlotIndexType : %d"), (uint8)_eShoppingBasketSlotIndexType);
	*/
}

void UGsUIWindowNpcShop::SetFocusingIndexSwitcher(const ScrollDircetionType InDirType)
{
	if (ScrollDircetionType::Scroll_Left == InDirType)
	{
		_eShoppingBasketPageType = ShoppingBasketPage::FirstPage == _eShoppingBasketPageType ? 
			ShoppingBasketPage::SixthPage : (ShoppingBasketPage)(_eShoppingBasketPageType - 1);
	}
	else if(ScrollDircetionType::Scroll_Right == InDirType)
	{
		_eShoppingBasketPageType = ShoppingBasketPage::SixthPage == _eShoppingBasketPageType ?
			ShoppingBasketPage::FirstPage : (ShoppingBasketPage)(_eShoppingBasketPageType + 1);
	}

	_firstPageSwitcher->SetActiveWidgetIndex(ShoppingBasketPage::FirstPage == _eShoppingBasketPageType ? 0 : 1);
	_secondPageSwitcher->SetActiveWidgetIndex(ShoppingBasketPage::SecondPage == _eShoppingBasketPageType ? 0 : 1);
	_thirdPageSwitcher->SetActiveWidgetIndex(ShoppingBasketPage::ThirdPage == _eShoppingBasketPageType ? 0 : 1);
	_fourthPageSwitcher->SetActiveWidgetIndex(ShoppingBasketPage::FourthPage == _eShoppingBasketPageType ? 0 : 1);
	_fifthPageSwitcher->SetActiveWidgetIndex(ShoppingBasketPage::FifthPage == _eShoppingBasketPageType ? 0 : 1);
	_sixthPageSwitcher->SetActiveWidgetIndex(ShoppingBasketPage::SixthPage == _eShoppingBasketPageType ? 0 : 1);
}

// InMoveTargetPageIndex 슬롯을 InDestPageIndex 슬롯으로 옮기고, 한칸씩 뒤로 이동함
void UGsUIWindowNpcShop::SetSlotMove(int32 InMoveTargetPageIndex, int32 InDestPageIndex)
{
	TSharedRef<class SWidget> sharedRef = _shoppingBasketRootPanel->TakeWidget();
	class SHorizontalBox* horizontalBox = static_cast<class SHorizontalBox*>(&sharedRef.Get());
	if (nullptr == horizontalBox)
	{
		check(horizontalBox);
		return;
	}

	FChildren* children = horizontalBox->GetChildren();
	TPanelChildren<SHorizontalBox::FSlot>* PanelChildren = static_cast<TPanelChildren<SHorizontalBox::FSlot>*>(children);
	PanelChildren->Move(InMoveTargetPageIndex, InDestPageIndex);
}

void UGsUIWindowNpcShop::OnClickIconSlot(class UGsUIIconItem& InIcon)
{
	UGsUIIconItemShop* itemIcon = Cast<UGsUIIconItemShop>(&InIcon);
	if (nullptr == itemIcon)
		return;

	if (nullptr == itemIcon->GetItemIconData())
		return;
	
	UGsNpcShopManager* npcShopManager = GSNpcShop();
	if (nullptr == npcShopManager)
		return;

	npcShopManager->RemoveBasketItemByIndex(itemIcon->GetSlotIntParamData());
}

void UGsUIWindowNpcShop::OnRefreshIconWrapBox_Basket(int32 InStartIndex, int32 InOffset, UGsUIIconBase* InIcon)
{
	UGsUIIconItemShop* itemIcon = Cast<UGsUIIconItemShop>(InIcon);
	if (nullptr == itemIcon)
		return;

	UGsNpcShopManager* npcShopManager = GSNpcShop();
	if (nullptr == npcShopManager)
		return;

	int32 index = InStartIndex + InOffset;
	const TArray<FGsNpcShopQuickSlotItemInfo>& basketList = npcShopManager->GetBasketList();
	if (true == basketList.IsValidIndex(index))
	{
		const FGsNpcShopQuickSlotItemInfo& disPlayInfo = basketList[index];
		if (0 >= disPlayInfo._itemTID)
		{
			itemIcon->SetEmptyIcon();
			itemIcon->SetItemCount(0);
		}
		else if (0 < disPlayInfo._itemDBID)
		{
			itemIcon->SetOwnedItem(disPlayInfo._itemDBID);
			itemIcon->SetItemCount(disPlayInfo._itemAmount);
		}
		else
		{
			itemIcon->SetNotOwnItem(disPlayInfo._itemTID, disPlayInfo._itemAmount, disPlayInfo._enchantLevel);
			itemIcon->SetItemCount(disPlayInfo._itemAmount);
		}
		itemIcon->SetItemNameToolTipFlag(true);
		itemIcon->SetItemNameToolTipHoverMode(true);
	}
	else
	{
		itemIcon->SetEmptyIcon();
		itemIcon->SetItemCount(0);
	}

	itemIcon->SetSlotIntParamData(index);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemCollectionMarking(itemIcon->GetItemIconData(), true);

	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIWindowNpcShop::OnClickIconSlot);
	}

	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIWindowNpcShop::OnLongPressIcon);
	}
}

void UGsUIWindowNpcShop::OnRefreshIconWrapBox_FristPage(int32 InIndex, UGsUIIconBase* InIcon)
{
	OnRefreshIconWrapBox_Basket(0, InIndex, InIcon);
}

void UGsUIWindowNpcShop::OnRefreshIconWrapBox_SecondPage(int32 InIndex, UGsUIIconBase* InIcon)
{
	OnRefreshIconWrapBox_Basket(5, InIndex, InIcon);
}

void UGsUIWindowNpcShop::OnRefreshIconWrapBox_ThirdPage(int32 InIndex, UGsUIIconBase* InIcon)
{
	OnRefreshIconWrapBox_Basket(10, InIndex, InIcon);
}

void UGsUIWindowNpcShop::OnRefreshIconWrapBox_fourthPage(int32 InIndex, UGsUIIconBase* InIcon)
{
	OnRefreshIconWrapBox_Basket(15, InIndex, InIcon);
}

void UGsUIWindowNpcShop::OnRefreshIconWrapBox_fifthPage(int32 InIndex, UGsUIIconBase* InIcon)
{
	OnRefreshIconWrapBox_Basket(20, InIndex, InIcon);
}

void UGsUIWindowNpcShop::OnRefreshIconWrapBox_sixthPage(int32 InIndex, UGsUIIconBase* InIcon)
{
	OnRefreshIconWrapBox_Basket(25, InIndex, InIcon);
}

void UGsUIWindowNpcShop::OnClickConfirm()
{
	CloseCalculatorUI();

	FGsItemDetailCloseMessageParam param;
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, &param);

	if (_isBuyMode)
	{
		GMessage()->GetNpcShop().SendMessage(MessageContentNpcShop::PURCHASE, nullptr);
	}
	else
	{
		GMessage()->GetNpcShop().SendMessage(MessageContentNpcShop::SALE, nullptr);
	}
}

void UGsUIWindowNpcShop::OnWeaponFilterSave()
{
	UGsNpcShopManager* npcShopManager = GSNpcShop();
	if (nullptr == npcShopManager)
		return;

	if (npcShopManager->IsFilterEmpty())
	{
		_toggleGroup.SetSelectedIndex(0);
	}
	else
	{
		TArray<ENpcShopProductListTab>& tabList = _npcFunctionTypeTabListMap[_shopNpcFunctionType];
		for (int32 i = 0;i < tabList.Num();++i)
		{
			if (tabList[i] == ENpcShopProductListTab::ProductList_Filter)
			{
				_currentInvenTabIndex = i;
				_toggleGroup.SetSelectedIndex(i);

				InvalidateProductItemList(_shopNpcFunctionType, _shopGroupIdList, ENpcShopProductListTab::ProductList_Filter, _isBuyMode);
				FGsItemDetailCloseMessageParam param;
				GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, &param);
				CloseCalculatorUI();
				break;
			}
		}
	}
}

FReply UGsUIWindowNpcShop::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

bool UGsUIWindowNpcShop::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsNpcShop))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_NPC_SHOP);
		return true;
	}

	return false;
}

void UGsUIWindowNpcShop::OnClickSetup()
{
	_leftPopupSwitcher->SetActiveWidgetIndex(ProductAreaType::DisPlay_SetupAutoOrder);
	_toggleGroupSetup.SetSelectedIndex(0, true);
	_eAutoSettingType = AutoSettingType::AutoBuySetting;
	
	OnSelectSetupTab(AutoSettingType::AutoBuySetting);

	CloseCalculatorUI();
}

// 0 구매 , 1 판매
void UGsUIWindowNpcShop::OnSelectSetupTab(int32 index)
{
	_eAutoSettingType = (AutoSettingType)index;
	if (AutoSettingType::AutoBuySetting == _eAutoSettingType)
	{
		_panelAutoBuyItemList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_panelAutoSellItemList->SetVisibility(ESlateVisibility::Collapsed);

		// 상점에서 판매중인 아이템들 똑같이 보여줌
		_disPlayItemInfoList.Empty();
		TArray<FGsNpcShopItemInfo*> shopList = GSNpcShop()->GetDisPlayItemListByGroupIDAndType(_shopNpcFunctionType, _shopGroupIdList, ENpcShopProductListTab::ProductList_AllTab);
		for (int32 i = 0; i < shopList.Num(); ++i)
		{
			_disPlayItemInfoList.Emplace(shopList[i]);
		}

		_disPlayItemInfoList.StableSort([](FGsNpcShopItemInfo& lhs, FGsNpcShopItemInfo& rhs)
			{
				return lhs._displayPriority < rhs._displayPriority;
			});

		_autoBuyListPanelSlotHelper->RefreshAll(_disPlayItemInfoList.Num());
	}
	else if (AutoSettingType::AutoSellSetting == _eAutoSettingType)
	{
		_panelAutoBuyItemList->SetVisibility(ESlateVisibility::Collapsed);
		_panelAutoSellItemList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		TArray<uint32> autoBuyItemList = GSNpcShop()->GetAutoSellItemTidList();
		_autoSellListPanelSlotHelper->RefreshAll((uint8)NpcShopAutoSellEquipmentType::EquipmentType_Max + autoBuyItemList.Num());
	}
	
	//GSLOG(Warning, TEXT("Call UGsUIWindowNpcShop::OnSelectSetupTab - index : %d"), index);
}

void UGsUIWindowNpcShop::OnClickResetAutoSetting()
{
	if (AutoSettingType::AutoBuySetting == _eAutoSettingType)
	{
		for (int32 i = 0; i < _autoBuyListPanelSlotHelper->GetItemCount(); ++i)
		{
			if (nullptr == _autoBuyListPanelSlotHelper->GetEntry(i))
				continue;

			UGsUINpcShopAutoBuyListEntry* entryPanel = Cast<UGsUINpcShopAutoBuyListEntry>(_autoBuyListPanelSlotHelper->GetEntry(i));
			if (nullptr == entryPanel)
				continue;

			entryPanel->SetAmount(0);
		}
	}
	else if (AutoSettingType::AutoSellSetting == _eAutoSettingType)
	{
		for (int32 i = 0; i < _autoSellListPanelSlotHelper->GetItemCount(); ++i)
		{
			if (nullptr == _autoSellListPanelSlotHelper->GetEntry(i))
				continue;

			UGsUINpcShopAutoSellListEntry* entryPanel = Cast<UGsUINpcShopAutoSellListEntry>(_autoSellListPanelSlotHelper->GetEntry(i));
			if (nullptr == entryPanel)
				continue;

			entryPanel->SetSlotSelect(false);
		}
	}
}

void UGsUIWindowNpcShop::OnClickConfirmAutoSetting()
{
	if (AutoSettingType::AutoBuySetting == _eAutoSettingType)
	{
		TMap<int32, ItemAmount> dataMap;
		for (int32 i = 0; i < _autoBuyListPanelSlotHelper->GetItemCount(); ++i)
		{
			if(nullptr == _autoBuyListPanelSlotHelper->GetEntry(i))
				continue;

			UGsUINpcShopAutoBuyListEntry* entryPanel = Cast<UGsUINpcShopAutoBuyListEntry>(_autoBuyListPanelSlotHelper->GetEntry(i));
			if(nullptr == entryPanel)
				continue;

			if(true == dataMap.Contains(entryPanel->GetSettingItemTID()))
				continue;

			dataMap.Emplace(entryPanel->GetSettingItemTID(), entryPanel->GetSettingItemAmount());
		}

		FGsNpcShopHelper::SaveAutoBuySettingData(dataMap);
	}
	else if(AutoSettingType::AutoSellSetting == _eAutoSettingType)
	{
		TMap<int32, bool> consumDataMap;
		TArray<TPair<bool, TPair<ItemCategoryMain, ItemGrade>>> equipmentSlotPairList;
		for (int32 i = 0; i < _autoSellListPanelSlotHelper->GetItemCount(); ++i)
		{
			if (nullptr == _autoSellListPanelSlotHelper->GetEntry(i))
				continue;

			UGsUINpcShopAutoSellListEntry* entryPanel = Cast<UGsUINpcShopAutoSellListEntry>(_autoSellListPanelSlotHelper->GetEntry(i));
			if (nullptr == entryPanel)
				continue;

			if (true == entryPanel->IsEquipmentSlot())
			{
				equipmentSlotPairList.Emplace(entryPanel->IsSelectedSlot(), 
					TPair<ItemCategoryMain, ItemGrade>(entryPanel->GetSlotItemCategory(), entryPanel->GetSlotItemGrade()));
			}
			else
			{
				consumDataMap.Emplace(entryPanel->GetDisplayItemTID(), entryPanel->IsSelectedSlot());
			}
		}

		FGsNpcShopHelper::SaveAutoSellConsumeSettingData(consumDataMap);
		FGsNpcShopHelper::SaveAutoSellEquipSettingData(equipmentSlotPairList);
	}

	_leftPopupSwitcher->SetActiveWidgetIndex(ProductAreaType::DisPlay_Product);
}

void UGsUIWindowNpcShop::AutoBuyItemListRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	UGsUINpcShopAutoBuyListEntry* entry = Cast<UGsUINpcShopAutoBuyListEntry>(InEntry);
	if (nullptr == entry)
		return;

	// 현재 상점에서 판매중인 아이템 리스트를 불러와야함
	if (false == _disPlayItemInfoList.IsValidIndex(InIndex) || nullptr == _disPlayItemInfoList[InIndex])
		return;

	ItemAmount amountValue = GSNpcShop()->GetAutoBuySettingData((uint32)_disPlayItemInfoList[InIndex]->_itemTID);

	entry->SetDisplayItem(_disPlayItemInfoList[InIndex]->_itemTID, amountValue, _disPlayItemInfoList[InIndex]->_autoPurchaseMaxCount);
}

void UGsUIWindowNpcShop::AutoSellItemListRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	UGsUINpcShopAutoSellListEntry* entry = Cast<UGsUINpcShopAutoSellListEntry>(InEntry);
	if (nullptr == entry)
		return;

	UGsNpcShopManager* shopMgr = GSNpcShop();
	// 저장되어있는 데이터 불러오기
	if (InIndex < (uint8)NpcShopAutoSellEquipmentType::EquipmentType_Max) // 장착류 아이템 먼저 출력함
	{
		bool flag = false;
		switch ((NpcShopAutoSellEquipmentType)InIndex)
		{
		case NpcShopAutoSellEquipmentType::Weapon_NormalGrade:
			flag = shopMgr->IsEnableAutoSellEquipment(ItemCategoryMain::WEAPON, ItemGrade::NORMAL);
			break;
		case NpcShopAutoSellEquipmentType::Weapon_MagicGrade:
			flag = shopMgr->IsEnableAutoSellEquipment(ItemCategoryMain::WEAPON, ItemGrade::MAGIC);
			break;
		case NpcShopAutoSellEquipmentType::Weapon_RareGrade:
			flag = shopMgr->IsEnableAutoSellEquipment(ItemCategoryMain::WEAPON, ItemGrade::RARE);
			break;
		case NpcShopAutoSellEquipmentType::Armor_NormalGrade:
			flag = shopMgr->IsEnableAutoSellEquipment(ItemCategoryMain::ARMOR, ItemGrade::NORMAL);
			break;
		case NpcShopAutoSellEquipmentType::Armor_MagicGrade:
			flag = shopMgr->IsEnableAutoSellEquipment(ItemCategoryMain::ARMOR, ItemGrade::MAGIC);
			break;
		case NpcShopAutoSellEquipmentType::Armor_RareGrade:
			flag = shopMgr->IsEnableAutoSellEquipment(ItemCategoryMain::ARMOR, ItemGrade::RARE);
			break;
		case NpcShopAutoSellEquipmentType::Accessory_NormalGrade:
			flag = shopMgr->IsEnableAutoSellEquipment(ItemCategoryMain::ACCESSORY, ItemGrade::NORMAL);
			break;
		case NpcShopAutoSellEquipmentType::Accessory_MagicGrade:
			flag = shopMgr->IsEnableAutoSellEquipment(ItemCategoryMain::ACCESSORY, ItemGrade::MAGIC);
			break;
		case NpcShopAutoSellEquipmentType::Accessory_RareGrade:
			flag = shopMgr->IsEnableAutoSellEquipment(ItemCategoryMain::ACCESSORY, ItemGrade::RARE);
			break;
		}
		entry->SetDisplayItem((NpcShopAutoSellEquipmentType)InIndex, flag);
	}
	else // 소모품류 아이템 출력
	{
		TArray<uint32> autoConsumSellItemList = shopMgr->GetAutoSellItemTidList();
		int32 realIndex = InIndex - (uint8)NpcShopAutoSellEquipmentType::EquipmentType_Max;
		if (true == autoConsumSellItemList.IsValidIndex(realIndex))
		{
			uint32 itemTID = autoConsumSellItemList[realIndex];
			bool flag = shopMgr->IsEnableAutoSellConsumable(itemTID);
			entry->SetDisplayItem(autoConsumSellItemList[realIndex], flag);
		}
	}
}

void UGsUIWindowNpcShop::OnClickAutoBuy()
{
	UGsNpcShopManager* npcShopManager = GSNpcShop();
	if (nullptr == npcShopManager)
		return;

	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	FGsMessageHolder* msgHolder = GMessage();
	if (nullptr == msgHolder)
		return;
	
	// 자동 구매 설정한 아이템이 하나라도 있는지 확인
	if (false == npcShopManager->IsExistAutoBuyItemData())
	{
		FText findText;
		FText::FindText(TEXT("NpcShopText"), TEXT("Empty_AutoBuySetting"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	// 장바구니 초기화
	npcShopManager->ClearBasketList(false);

	TArray<FGsBasketTryInputInfo> itemList;
	for (int32 i = 0; i < _disPlayItemInfoList.Num(); ++i)
	{
		if(nullptr == _disPlayItemInfoList[i])
			continue;

		// 자동 구매 설정된 아이템인지 확인
		ItemAmount settingValue = npcShopManager->GetAutoBuySettingData((uint32)_disPlayItemInfoList[i]->_itemTID);
		if (0 >= settingValue)
			continue;

		// 구매 가능한 지 확인
		EGsNotAllowedBuyReason type = EGsNotAllowedBuyReason::NONE;
		if (false == FGsNpcShopHelper::IsAllowedBuyItem(_disPlayItemInfoList[i], type))
			continue;

		// 인벤토리에 이미 보유한 아이템이 설정된 수량보다 많은지 확인
		ItemAmount ownedAmount = itemManager->GetAllItemCountByTID(_disPlayItemInfoList[i]->_itemTID);
		if(ownedAmount >= settingValue)
			continue;

		// 구매 가능한 수량
		ItemAmount resultSettingCount = settingValue - ownedAmount;
		itemList.Emplace(0, _disPlayItemInfoList[i]->_itemTID, _disPlayItemInfoList[i]->_productId, resultSettingCount);
	}

	// 자동 구매 설정된 아이템은 모두 보내고 받는 쪽에서 처리 여부를 결정한다.
	if (itemList.Num() > 0)
	{
		FGsInputShoppingBasket insertParam(itemList, true, false);
		msgHolder->GetNpcShop().SendMessage(MessageContentNpcShop::INSERT_BASKET, &insertParam);
	}

	CloseCalculatorUI();
}

void UGsUIWindowNpcShop::OnClickCloseSetup()
{
	_leftPopupSwitcher->SetActiveWidgetIndex(ProductAreaType::DisPlay_Product);
}

void UGsUIWindowNpcShop::SetShopName(const NpcFunctionType InType, const FGsSchemaNpcData* InNpcData)
{
	_shopNameText->SetText(InNpcData->nameText);
}

void UGsUIWindowNpcShop::UpdateShopItemList()
{
	_disPlayItemInfoList.Empty();
	TArray<FGsNpcShopItemInfo*> shopList = GSNpcShop()->GetDisPlayItemListByGroupIDAndType(_shopNpcFunctionType, _shopGroupIdList, _shopTabType);
	for (int32 i = 0; i < shopList.Num(); ++i)
	{
		_disPlayItemInfoList.Emplace(shopList[i]);
	}

	_disPlayItemInfoList.StableSort([](FGsNpcShopItemInfo& lhs, FGsNpcShopItemInfo& rhs)
		{
			return lhs._displayPriority < rhs._displayPriority;
		});

	if (IsTutorial())
	{
		FGsNpcShopItemInfo* tutorialInfo = nullptr;
		for (int i = 0, maxCount = _disPlayItemInfoList.Num(); i < maxCount; ++i)
		{	
			if (_tutorialItemId == _disPlayItemInfoList[i]->_itemTID)
			{
				tutorialInfo = _disPlayItemInfoList[i];
				_disPlayItemInfoList.Remove(tutorialInfo);

				break;
			}
		}

		// 제일 앞으로 배치
		if (tutorialInfo)
		{
			_disPlayItemInfoList.EmplaceAt(0, tutorialInfo);
		}
	}

	_panelEmptyItemList->SetVisibility(0 < _disPlayItemInfoList.Num() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	_buyListPanelSlotHelper->RefreshAll(_disPlayItemInfoList.Num());

	InvalidateShopMode(_isBuyMode);
	if (true == _isCurrencyDirty && 0 < _disPlayItemInfoList.Num())
	{
		InvalidateCurrencyIcon(_disPlayItemInfoList[0]->_costType, _disPlayItemInfoList[0]->_currencyType, _disPlayItemInfoList[0]->_currencyItemTID);
		_isCurrencyDirty = false;
	}

	if (IsTutorial())
	{
		if (0 < _disPlayItemInfoList.Num())
		{
			if (0 < _productListScrollBox->GetChildrenCount())
			{
				if (UWidget* tutorialTarget = _productListScrollBox->GetChildAt(0))
				{
					OnTutorialListItem.ExecuteIfBound(tutorialTarget);
				}
			}
		}
	}
}

void UGsUIWindowNpcShop::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	FGsItem* itemData = InIcon.GetItemIconData();
	if (nullptr == itemData)
		return;

	FGsItemDetailOpenMessageParamEX::DetailOption option;
	if (INVALID_ITEM_DB_ID == itemData->GetDBID())
	{
		// 상점 아이템
		option._bHideButtons = false;
		option._bHideBG = false;
		option._bHideButtonsBottom = true;
		option._bUseLock = false;
		option._bUseCollectionMarking = true;
	}
	else
	{
		// 인벤토리 아이템
		option._bHideButtons = true;
		option._bHideBG = false;
		option._bHideButtonsBottom = true;
		option._bUseLock = false;
		option._bUseCollectionMarking = true;
	}

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), UGsUIPopupItemDetailMain::CenterPosType::CenterPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
}

void UGsUIWindowNpcShop::SetTutorial(ItemId InItemId)
{
	_tutorialItemId = InItemId;

	if (INVALID_ITEM_ID == InItemId)
	{
		OnTutorialListItem.Unbind();

		_productListScrollBox->SetAllowOverscroll(true);
	}
	else
	{
		_productListScrollBox->SetAllowOverscroll(false);

		// 강제로 한 번 수행
		UpdateShopItemList();
	}
}

void UGsUIWindowNpcShop::OnScrollProductList(float InCurrentOffset)
{
	// 튜토리얼 스크롤 락
	if (IsTutorial())
	{
		_productListScrollBox->SetScrollOffset(0.f);
	}
}

bool UGsUIWindowNpcShop::OnBack()
{
	CloseCalculatorUI();
	Close();

	return Super::OnBack();
}

Currency UGsUIWindowNpcShop::GetTaxIncludedPriceInBasket()
{
	UGsNpcShopManager* npcShopMgr = GSNpcShop();
	if (nullptr == npcShopMgr)
	{
		return 0;
	}

	Currency totalPrice = 0;
	const TArray<FGsNpcShopQuickSlotItemInfo>& basketList = npcShopMgr->GetBasketList();
	for (const FGsNpcShopQuickSlotItemInfo& iter : basketList)
	{
		if (FGsNpcShopItemInfo* shopInfo = npcShopMgr->GetShopItemInfoByItemTID(
			_shopNpcFunctionType, iter._shopGroupId, iter._itemTID))
		{
			totalPrice += shopInfo->GetCurrencyAmount(_shopNpcFunctionType) * iter._itemAmount;
		}
	}

	return totalPrice;
}
