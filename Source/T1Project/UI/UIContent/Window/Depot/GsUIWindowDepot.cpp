// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowDepot.h"

#include "T1Project.h"
#include "SBoxPanel.h"

#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/HorizontalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "DataSchema/GsSchemaAutoDepotConfig.h"

#include "GameObject/Stat/GsStatInfo.h"

#include "Management/ScopeGame/GsDepotManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsInventoryManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "UI/UIContent/Popup/GsUIPopupCommonPayInventory.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIContent/Popup/GsUIPopupCalculator.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemShop.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Window/Depot/GsUIItemSlotLockWidget.h"
#include "UI/UIContent/Window/Depot/GsUIAutoDepositItemList.h"
#include "UI/UIContent/Window/Depot/GsUIAutoDepositEntry.h"

#include "UTIL/GsTimeUtil.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "Item/GsItemManager.h"
#include "Item/GsItemHelper.h"

#include "Currency/GsCostPackage.h"
#include "Currency/GsCurrencyHelper.h"


void UGsUIWindowDepot::BeginDestroy()
{
	if (nullptr != _iconSlotHelper)
	{
		_iconSlotHelper = nullptr;
	}

	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}

	if (nullptr != _autoSettingPanelSlotHelper)
	{
		_autoSettingPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowDepot::AutoSettingListRefreshEntry);
		_autoSettingPanelSlotHelper = nullptr;
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

	_toggleGroup.Clear();
	_toggleGroupAutoDeposit.Clear();

	Super::BeginDestroy();
}

void UGsUIWindowDepot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_endScrollOffset = 0.f;

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(_iconSelector);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIWindowDepot::OnRefreshIconWrapBox);
	}

	if (nullptr == _firstPageIconSlotHelper)
	{
		_firstPageIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_firstPageIconSlotHelper->Initialize(_firstPageIconSelector);
		_firstPageIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIWindowDepot::OnRefreshIconWrapBox_FristPage);
	}
	if (nullptr == _secondPageIconSlotHelper)
	{
		_secondPageIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_secondPageIconSlotHelper->Initialize(_secondPageIconSelector);
		_secondPageIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIWindowDepot::OnRefreshIconWrapBox_SecondPage);
	}
	if (nullptr == _thirdPageIconSlotHelper)
	{
		_thirdPageIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_thirdPageIconSlotHelper->Initialize(_thirdPageIconSelector);
		_thirdPageIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIWindowDepot::OnRefreshIconWrapBox_ThirdPage);
	}
	if (nullptr == _fourthPageIconSlotHelper)
	{
		_fourthPageIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_fourthPageIconSlotHelper->Initialize(_fourthPageIconSelector);
		_fourthPageIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIWindowDepot::OnRefreshIconWrapBox_fourthPage);
	}
	if (nullptr == _fifthPageIconSlotHelper)
	{
		_fifthPageIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_fifthPageIconSlotHelper->Initialize(_fifthPageIconSelector);
		_fifthPageIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIWindowDepot::OnRefreshIconWrapBox_fifthPage);
	}
	if (nullptr == _sixthPageIconSlotHelper)
	{
		_sixthPageIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_sixthPageIconSlotHelper->Initialize(_sixthPageIconSelector);
		_sixthPageIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIWindowDepot::OnRefreshIconWrapBox_sixthPage);
	}
	
	if (nullptr == _autoSettingPanelSlotHelper)
	{
		_autoSettingPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_autoSettingPanelSlotHelper->Initialize(_autoSettingEntryWidgetClass, _autoStorageItemListScrollBox);
		_autoSettingPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowDepot::AutoSettingListRefreshEntry);
	}

	_toggleGroup.Clear();
	_toggleGroup.AddToggleWidgetByParentPanel(_panelInvenTab);
	_toggleGroup.OnSlotClicked.BindUObject(this, &UGsUIWindowDepot::OnSlotClickedTab);
	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIWindowDepot::OnSelectChangedTab);

	_toggleGroupAutoDeposit.Clear();
	_toggleGroupAutoDeposit.OnSlotClicked.BindUObject(this, &UGsUIWindowDepot::OnSlotClickedAutoDepositGroup);
	_toggleGroupAutoDeposit.OnSelectChanged.BindUObject(this, &UGsUIWindowDepot::OnSelectChangedAutoDepositGroup);

	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowDepot::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowDepot::OnClickCloseAllStack);
	_btnDepotListClose->OnClicked.AddDynamic(this, &UGsUIWindowDepot::OnClickCloseAllStack);
	_btnItemSlotLock->OnClicked.AddDynamic(this, &UGsUIWindowDepot::OnClickSlotLock);
	_btnDepotSlotSortList->OnClicked.AddDynamic(this, &UGsUIWindowDepot::OnClickDepotSlotSortList);

	_btnDefaultSort->OnClicked.AddDynamic(this, &UGsUIWindowDepot::OnClickDefaultSort);
	_btnGradeSort->OnClicked.AddDynamic(this, &UGsUIWindowDepot::OnClickGradeSort);
	_btnWeightSort->OnClicked.AddDynamic(this, &UGsUIWindowDepot::OnClickWeightSort);
	_btnConfirmAutoSetting->OnClicked.AddDynamic(this, &UGsUIWindowDepot::OnClickAutoSetting);
	_btnConfirm->OnClicked.AddDynamic(this, &UGsUIWindowDepot::OnClickConfirm);

	_btnSetupAutoDeposit->OnClicked.AddDynamic(this, &UGsUIWindowDepot::OnClickSetup);
	_btnCloseSetup->OnClicked.AddDynamic(this, &UGsUIWindowDepot::OnClickCloseSetup);

	_rootScrollBox->OnUserScrolled.AddDynamic(this, &UGsUIWindowDepot::OnChangeMainScrollEvent);

	_autoDepositItemList->OnClickedIcon.BindUObject(this, &UGsUIWindowDepot::OnClickAutoDepositIcon);

	_timer = NewObject<UGsSimpleTimer>();
	float min;
	float max;
	_curve->GetTimeRange(min, max);
	_timer->SetMaxTime(max);

	_timer->Stop();
}

void UGsUIWindowDepot::NativeConstruct()
{
	Super::NativeConstruct();

	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
		return;

	FGsDepotManager* depotManager = GSDepot();
	if (nullptr == depotManager)
		return;

	// BindMessages
	_itemContentsDelegateList.Emplace(msg->GetItemContents().AddUObject(MessageContentItem::DEPOT_BASKET_INVALIDATE,
		this, &UGsUIWindowDepot::OnChangeBasketItems));
	_itemContentsDelegateList.Emplace(msg->GetItemContents().AddUObject(MessageContentItem::DEPOT_BASKET_REMOVE,
		this, &UGsUIWindowDepot::RemoveBasketItems));
	_itemContentsDelegateList.Emplace(msg->GetItemContents().AddUObject(MessageContentItem::DEPOT_BASKET_ADD,
		this, &UGsUIWindowDepot::AddBasketItems));
	_itemContentsDelegateList.Emplace(msg->GetItemContents().AddUObject(MessageContentItem::DEPOT_BASKET_ADD_LIST,
		this, &UGsUIWindowDepot::AddBasketItemList));
	_itemContentsDelegateList.Emplace(msg->GetItemContents().AddUObject(MessageContentItem::DEPOT_INVALIDATE_ALL,
		this, &UGsUIWindowDepot::OnAllUpdateDepotItems));
	_itemContentsDelegateList.Emplace(msg->GetItemContents().AddUObject(MessageContentItem::DEPOT_INVALIDATE,
		this, &UGsUIWindowDepot::UpdateDepotItem));
	_itemContentsDelegateList.Emplace(msg->GetItemContents().AddUObject(MessageContentItem::DEPOT_STATE_DEPOSIT,
		this, &UGsUIWindowDepot::OnChangeStateDeposit));
	_itemContentsDelegateList.Emplace(msg->GetItemContents().AddUObject(MessageContentItem::DEPOT_STATE_TAKEOUT,
		this, &UGsUIWindowDepot::OnChangeStateTake));
	_itemContentsDelegateList.Emplace(msg->GetItemContents().AddUObject(MessageContentItem::DEPOT_EXPEND_SLOT,
		this, &UGsUIWindowDepot::ExtendDepotSlot));

	_inventoryDelegateList.Emplace(msg->GetInventoryEvent().AddUObject(MessageContentInven::UPDATE_ITEM_WEIGHT,
		this, &UGsUIWindowDepot::OnUpdateWeight));
	_inventoryDelegateList.Emplace(msg->GetInventoryEvent().AddUObject(MessageContentInven::UPDATE_ITEM_WEIGHT_PENALTY,
		this, &UGsUIWindowDepot::OnUpdateWeight));

	_userInfoDelegateList.Emplace(msg->GetUserBasicInfo().AddUObject(MessageUserInfo::CURRENCY,
		this, &UGsUIWindowDepot::OnUpdateCurrency));

	// 창고 목록 UI 갱신 (기본 정렬, 전체 탭)
	_eDepotSortType = DepotSortType::SortDefault;
	_toggleGroup.SetSelectedIndex(0, true);

	SwitchLeftPopupUI(SWITCHER_INDEX_STORAGE);

	// 정렬 패널 OFF
	SetSortPanelVisibility(false);

	// 물품 목록 관련 변수 초기화
	_eSlotBasketIndexType = SlotIndexType::FirstSlotGroupIndex;
	_eSlotBasketPageType = SlotPageType::FirstPage;
	_eScrollDirectionType = ScrollDircetionType::Scroll_None;

	_timer->Stop();
	_isScrolling = false;
	_isAutoScrolling = false;
	_findEndScrollOffeset = false;
	_slotMoveCount = 0;

	// 보관 상태 설정 및 UI 갱신(무게, 가격, 물품 목록)
	OnChangeStateDeposit(nullptr);

	// 물품 목록 위치 정보 UI 갱신 (목록 아래의 조그만 점들)
	SetFocusingIndexSwitcher(ScrollDircetionType::Scroll_None);

	// 확장 버튼 UI 갱신
	SetExtendButtonVisibility(depotManager->CanExtendDepotSlot());

	// 자동 보관 목록 설정
	const UGsTableAutoDepotConfig* autoDepotConfigTable = Cast<UGsTableAutoDepotConfig>(FGsSchemaAutoDepotConfig::GetStaticTable());
	if (nullptr == autoDepotConfigTable)
		return;

	TArray<const FGsSchemaAutoDepotConfig*> autoDepotConfigList;
	if (false == autoDepotConfigTable->GetAllRows(autoDepotConfigList))
		return;

	_autoDepositSettingList.Empty();
	for (const FGsSchemaAutoDepotConfig* autoDepotConfig : autoDepotConfigList)
	{
		AutoDepositSetting autoDepositSetting;
		autoDepositSetting._groupId = autoDepotConfig->groupId;
		autoDepositSetting._grade = depotManager->GetAutoDepositGrade(autoDepotConfig->groupId);
		autoDepositSetting._autoDepositConfig = autoDepotConfig;
		
		TArray<const FGsSchemaItemCommon*> itemCommonList;
		for (const FGsSchemaItemCommonRow& itemCommonRow : autoDepotConfig->itemIdList)
		{
			const FGsSchemaItemCommon* itemCommon = itemCommonRow.GetRow();
			if (nullptr == itemCommon)
				continue;

			itemCommonList.Emplace(itemCommon);
		}

		itemCommonList.Sort([](const FGsSchemaItemCommon& itemA, const FGsSchemaItemCommon& itemB)
			{
				// 아이템 등급 오름차순
				if (itemA.grade != itemB.grade)
				{
					return (uint8)itemA.grade < (uint8)itemB.grade;
				}

				// 아이템 그룹 오름차순 (비귀속, 귀속, 이벤트, 상점, ... )
				if (itemA.itemGroupId != itemB.itemGroupId)
				{
					return itemA.itemGroupId < itemB.itemGroupId;
				}

				// 아이템 TID 오름차순
				return itemA.id < itemB.id;
			}
		);

		for (const FGsSchemaItemCommon* itemCommon : itemCommonList)
		{
			autoDepositSetting._itemIdList.Emplace(itemCommon->id, itemCommon->grade);
			//autoDepositSetting._autoDepositMap.Emplace(itemCommon->id, depotManager->IsSelectedAutoSlot(itemCommon->id));
		}

		_autoDepositSettingList.Emplace(autoDepositSetting);
	}
	_isAutoDepositGroupGradeRefresh = false;
	_autoSettingPanelSlotHelper->RefreshAll(_autoDepositSettingList.Num());

	_toggleGroupAutoDeposit.Clear();
	_toggleGroupAutoDeposit.AddToggleWidgetByParentPanel(_autoStorageItemListScrollBox);
	_toggleGroupAutoDeposit.SetSelectedIndexNoEvent(-1);
}

void UGsUIWindowDepot::NativeDestruct()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
		return;

	FGsDepotManager* depotManager = GSDepot();
	if (nullptr == depotManager)
		return;

	for (auto& iter : _itemContentsDelegateList)
	{
		msg->GetItemContents().Remove(iter);
	}
	_itemContentsDelegateList.Empty();

	for (auto& iter : _inventoryDelegateList)
	{
		msg->GetInventoryEvent().Remove(iter);
	}
	_inventoryDelegateList.Empty();

	for (auto& iter : _userInfoDelegateList)
	{
		msg->GetUserBasicInfo().Remove(iter);
	}
	_userInfoDelegateList.Empty();

	_displayItemList.Empty();
	_autoDepositSettingList.Empty();
	_basketSlotPositionMap.Empty();

	_firstPageIconSlotHelper->Clear();
	_secondPageIconSlotHelper->Clear();
	_thirdPageIconSlotHelper->Clear();
	_fourthPageIconSlotHelper->Clear();
	_fifthPageIconSlotHelper->Clear();
	_sixthPageIconSlotHelper->Clear();

	depotManager->ResetBasketItems();

	Super::NativeDestruct();
}

void UGsUIWindowDepot::OnAllUpdateDepotItems(const IGsMessageParam* InParam)
{
	// 현재 탭 전체 갱신
	_toggleGroup.SetSelectedIndex(_toggleGroup.GetSelectedIndex(), true);
}

void UGsUIWindowDepot::UpdateDepotItem(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
		return;

	const FGsUpdateDepotItem* param = InParam->Cast<const FGsUpdateDepotItem>();
	if (nullptr == param)
		return;

	for (int32 i = 0; i < _displayItemList.Num(); ++i)
	{
		if (param->_itemDBID == _displayItemList[i])
		{
			_iconSlotHelper->RefreshByIndex(i);
			break;
		}
	}
}

void UGsUIWindowDepot::AddBasketItems(const IGsMessageParam* InParam)
{
	FGsDepotManager* depotManager = GSDepot();
	if (nullptr == depotManager)
		return;

	if (nullptr == InParam)
		return;

	const FGsOnChangeDepotBasketItem* param = InParam->Cast<const FGsOnChangeDepotBasketItem>();
	if (nullptr == param)
		return;

	// 창고 인벤토리의 선택 UI 갱신
	for (int32 i = 0; i < _displayItemList.Num(); ++i)
	{
		uint64 displayDBID = _displayItemList[i];
		if (displayDBID == param->_itemDBID)
		{
			_iconSlotHelper->RefreshByIndex(i);
			break;
		}
	}

	// 중앙 물품 목록 UI 갱신
	int32 basketIndex = 0;
	if (depotManager->GetItemIndexInBasketByDBID(param->_itemDBID, basketIndex))
	{
		InvalidateBasketScroll(basketIndex / SLOT_MAX_COUNT);
	}
	OnChangeBasketItems(nullptr);
}

void UGsUIWindowDepot::AddBasketItemList(const IGsMessageParam* InParam)
{
	FGsDepotManager* depotManager = GSDepot();
	if (nullptr == depotManager)
		return;

	if (nullptr == InParam)
		return;

	const FGsOnAddDepotBasketItemList* param = InParam->Cast<const FGsOnAddDepotBasketItemList>();
	if (nullptr == param)
		return;

	ItemDBId lastItemDBId = 0;
	for (ItemDBId itemDBId : param->_itemDBIdList)
	{
		lastItemDBId = itemDBId;

		// 찾기 상태일 때만 유효한 로직
		// 창고 인벤토리의 선택 UI 갱신
		if (false == _isDeposit)
		{
			for (int32 i = 0; i < _displayItemList.Num(); ++i)
			{
				uint64 displayDBID = _displayItemList[i];
				if (displayDBID == itemDBId)
				{
					_iconSlotHelper->RefreshByIndex(i);
					break;
				}
			}
		}
	}

	// 중앙 물품 목록 UI 갱신
	int32 basketIndex = 0;
	if (depotManager->GetItemIndexInBasketByDBID(lastItemDBId, basketIndex))
	{
		InvalidateBasketScroll(basketIndex / SLOT_MAX_COUNT);
	}
	OnChangeBasketItems(nullptr);
}

void UGsUIWindowDepot::RemoveBasketItems(const IGsMessageParam* InParam)
{
	FGsDepotManager* depotManager = GSDepot();
	if (nullptr == depotManager)
		return;

	if (nullptr == InParam)
		return;

	const FGsOnChangeDepotBasketItem* param = InParam->Cast<const FGsOnChangeDepotBasketItem>();
	if (nullptr == param)
		return;

	// 창고 인벤토리의 선택 UI 갱신
	for (int32 i = 0; i < _displayItemList.Num(); ++i)
	{
		uint64 displayDBID = _displayItemList[i];
		if (displayDBID == param->_itemDBID)
		{
			_iconSlotHelper->RefreshByIndex(i);
			break;
		}
	}

	// 중앙 물품 목록 UI 갱신
	int32 basketIndex = (depotManager->GetBasketItemCount() <= 0) ? 0 : depotManager->GetBasketItemCount() - 1;
	int32 pageIndex = basketIndex / SLOT_MAX_COUNT;
	SlotPageType slotBasketPageType = static_cast<SlotPageType>(pageIndex);
	if (_eSlotBasketPageType > slotBasketPageType)
	{
		InvalidateBasketScroll(pageIndex);
	}
	OnChangeBasketItems(nullptr);
}

void UGsUIWindowDepot::OnChangeBasketItems(const IGsMessageParam* InParam)
{
	_firstPageIconSlotHelper->RefreshAll(SLOT_MAX_COUNT);
	_secondPageIconSlotHelper->RefreshAll(SLOT_MAX_COUNT);
	_thirdPageIconSlotHelper->RefreshAll(SLOT_MAX_COUNT);
	_fourthPageIconSlotHelper->RefreshAll(SLOT_MAX_COUNT);
	_fifthPageIconSlotHelper->RefreshAll(SLOT_MAX_COUNT);
	_sixthPageIconSlotHelper->RefreshAll(SLOT_MAX_COUNT);

	InvalidatePrice();
	InvalidateWeight();
}

void UGsUIWindowDepot::OnChangeStateDeposit(const IGsMessageParam* InParam)
{
	_iconSlotHelper->RefreshAll();
	OnChangeBasketItems(nullptr);

	_isDeposit = true;

	// TEXT: 보관할 물품
	FText findText;
	FText::FindText(TEXT("DepotText"), TEXT("Deposit_Goods"), findText);
	_actionModeText->SetText(findText);

	// TEXT: 보관 비용
	FText findText2;
	FText::FindText(TEXT("DepotText"), TEXT("Deposit_Cost"), findText2);
	_priceGuideText->SetText(findText2);
}

void UGsUIWindowDepot::OnChangeStateTake(const IGsMessageParam* InParam)
{
	CloseItemDetailUI();
	_iconSlotHelper->RefreshAll();
	OnChangeBasketItems(nullptr);

	_isDeposit = false;

	// TEXT: 찾을 물품
	FText findText;
	FText::FindText(TEXT("DepotText"), TEXT("Withdraw_Goods"), findText);
	_actionModeText->SetText(findText);

	// TEXT: 찾는 비용
	FText findText2;
	FText::FindText(TEXT("DepotText"), TEXT("Withdraw_Cost"), findText2);
	_priceGuideText->SetText(findText2);
}

void UGsUIWindowDepot::OnClickSlotLock()
{
	// 최대 수치까지 확장 완료했을 경우 티커 메시지 출력
	FGsDepotManager* depotMgr = GSDepot();
	if (nullptr == depotMgr)
		return;

	if (false == depotMgr->CanExtendDepotSlot())
	{
		FText findText;
		FText::FindText(TEXT("DepotText"), TEXT("Notice_FullExtend"), findText);
		findText = FText::Format(findText, depotMgr->GetExtendedSlotNumber());
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	if (UGsUIPopupCommonPayInventory* popupCommonPayInventory =
		Cast<UGsUIPopupCommonPayInventory>(GUI()->OpenAndGetWidget(TEXT("PopupCommonPayInventory"))))
	{
		FText findText;
		FText::FindText(TEXT("DepotText"), TEXT("ExtendSlot_Check"), findText);

		Level pastCount = depotMgr->GetExtendedSlotNumber();
		Level leftCount = depotMgr->GetEnoughDepotExtendLineCount();

		popupCommonPayInventory->SetData(findText, pastCount, leftCount, true,
			[depotMgr](bool InIsOk, bool InIsEnough, Level InExtendLevel)
		{
			if (!InIsOk)
				return;

			// 재화 충분할 경우, 확장 (선택한 레벨만큼)
			if (InIsEnough)
			{
				depotMgr->SendExtendDepotSlot(InExtendLevel);
				return;
			}

			// 재화 모자름 ticker 출력
			FText Text;
			FText::FindText(TEXT("DepotText"), TEXT("Alert_NoCost"), Text);
			FGsUIHelper::TrayMessageTicker(Text);
		});
		
	}
}

void UGsUIWindowDepot::OnClickDepotSlotSortList()
{
	SetSortPanelVisibility(!_sortItemListPanel->IsVisible());
}

void UGsUIWindowDepot::UpdateDepotSort()
{
	_defaultSortSwitcher->SetActiveWidgetIndex(_eDepotSortType == DepotSortType::SortDefault ? 0 : 1);
	_gradeSortSwitcher->SetActiveWidgetIndex(_eDepotSortType == DepotSortType::SortGrade ? 0 : 1);
	_weightSortSwitcher->SetActiveWidgetIndex(_eDepotSortType == DepotSortType::SortWeight ? 0 : 1);
}

void UGsUIWindowDepot::OnRefreshIconWrapBox(int32 InIndex, class UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	FGsDepotManager* depotMgr = GSDepot();
	if (nullptr == depotMgr)
		return;

	if (false == _displayItemList.IsValidIndex(InIndex))
	{
		itemIcon->SetEmptyIcon();
		return;
	}

	if (false == itemIcon->OnFinishedLongPressEvent.IsBound())
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIWindowDepot::OnLongPressIcon);
	}

	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIWindowDepot::OnClickDepotIconSlot);
	}

	itemIcon->SetOwnedItem(_displayItemList[InIndex], UGsUIIconItem::FindItemStorageType::Storage_Depot);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetItemNameToolTipFlag(true);
	itemIcon->SetItemNameToolTipHoverMode(true);
	itemIcon->SetSlotIntParamData(InIndex);
	itemIcon->SetItemCollectionMarking(itemIcon->GetItemIconData(), true);

	if (0 >= depotMgr->GetBasketItemCount())
	{
		itemIcon->SetSlotSelectionImage(false);
	}
	else
	{
		itemIcon->SetSlotSelectionImage(depotMgr->IsExistItemInBasketByDBID(_displayItemList[InIndex]));
	}
}

void UGsUIWindowDepot::OnRefreshIconWrapBox_Basket(int32 InStartIndex, int32 InOffset, UGsUIIconBase* InIcon)
{
	UGsUIIconItemShop* itemIcon = Cast<UGsUIIconItemShop>(InIcon);
	if (nullptr == itemIcon)
		return;

	uint64 dbid = 0;
	ItemAmount amount = 0;
	if (true == GSDepot()->GetBasketItemInfoBySlotPageIndex(InStartIndex, InOffset, dbid, amount))
	{
		itemIcon->SetOwnedItem(dbid, UGsUIIconItem::FindItemStorageType::Storage_All);
		itemIcon->SetItemCount(amount);
		itemIcon->SetItemNameToolTipFlag(true);
		itemIcon->SetItemNameToolTipHoverMode(true);
	}
	else
	{
		itemIcon->SetEmptyIcon();
		itemIcon->SetItemCount(0);
	}

	itemIcon->SetSlotIntParamData(InStartIndex + InOffset);
	itemIcon->SetActiveIconBg(true);

	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIWindowDepot::OnClickIconSlot);
	}

	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIWindowDepot::OnLongPressIcon);
	}
}

void UGsUIWindowDepot::OnRefreshIconWrapBox_FristPage(int32 InIndex, UGsUIIconBase* InIcon)
{
	OnRefreshIconWrapBox_Basket(0, InIndex, InIcon);
}

void UGsUIWindowDepot::OnRefreshIconWrapBox_SecondPage(int32 InIndex, UGsUIIconBase* InIcon)
{
	OnRefreshIconWrapBox_Basket(5, InIndex, InIcon);
}

void UGsUIWindowDepot::OnRefreshIconWrapBox_ThirdPage(int32 InIndex, UGsUIIconBase* InIcon)
{
	OnRefreshIconWrapBox_Basket(10, InIndex, InIcon);
}

void UGsUIWindowDepot::OnRefreshIconWrapBox_fourthPage(int32 InIndex, UGsUIIconBase* InIcon)
{
	OnRefreshIconWrapBox_Basket(15, InIndex, InIcon);
}

void UGsUIWindowDepot::OnRefreshIconWrapBox_fifthPage(int32 InIndex, UGsUIIconBase* InIcon)
{
	OnRefreshIconWrapBox_Basket(20, InIndex, InIcon);
}

void UGsUIWindowDepot::OnRefreshIconWrapBox_sixthPage(int32 InIndex, UGsUIIconBase* InIcon)
{
	OnRefreshIconWrapBox_Basket(25, InIndex, InIcon);
}

void UGsUIWindowDepot::OnSlotClickedAutoDepositGroup(int32 InIndex, bool bInIsSelected)
{
	if (false == bInIsSelected)
	{
		SetAutoDepositPanelVisibility(false);
		_toggleGroupAutoDeposit.SetSelectedIndexNoEvent(-1);
	}
}

void UGsUIWindowDepot::OnSelectChangedAutoDepositGroup(int32 InIndex)
{
	SetAutoDepositPanelVisibility(true, InIndex);
}

void UGsUIWindowDepot::OnChangeAutoDepositGrade(int32 InIndex, ItemGrade InGrade)
{
	if (false == _autoDepositSettingList.IsValidIndex(InIndex))
		return;

	AutoDepositSetting& autoDepositSetting = _autoDepositSettingList[InIndex];

	if (autoDepositSetting._grade != InGrade)
	{
		autoDepositSetting._grade = InGrade;

		int32 inGradeValue = static_cast<int32>(autoDepositSetting._grade);
		for (TPair<ItemId, ItemGrade> itemInfo : autoDepositSetting._itemIdList)
		{
			if (false == autoDepositSetting._autoDepositMap.Contains(itemInfo.Key))
			{
				autoDepositSetting._autoDepositMap.Emplace(itemInfo.Key, false);
			}

			// 새로 설정된 등급보다 높은 등급의 아이템은 자동 보관이 해제되고
			// 같거나 낮은 등급의 아이템은 자동 보관이 설정된다.
			int32 itemGradeValue = static_cast<int32>(itemInfo.Value);
			autoDepositSetting._autoDepositMap[itemInfo.Key] = inGradeValue >= itemGradeValue;
		}
	}

	_autoDepositItemList->InvalidateItemListSelect(autoDepositSetting._autoDepositMap);
	InvalidateAll();
}

void UGsUIWindowDepot::OnChangeMainScrollEvent(float InCurOffset)
{
	if (true == _rootScrollBox->HasMouseCapture())
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
			_rootScrollBox->SetScrollOffset(GetSlotPositionValueByIndexType(_eSlotBasketIndexType));
		}

		return;
	}

	float curPagePosValue = GetSlotPositionValueByIndexType(_eSlotBasketIndexType);
	float offsetValue = curPagePosValue - InCurOffset;
	float convertAbs = fabsf(offsetValue);

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
}

void UGsUIWindowDepot::OnSlotClickedTab(int32 InIndex, bool bInIsSelected)
{
	_scrollBox->ScrollToStart();
	_scrollBox->EndInertialScrolling();
}

void UGsUIWindowDepot::OnSelectChangedTab(int32 InIndex)
{
	FGsDepotManager* depotManager = GSDepot();
	if (nullptr == depotManager)
		return;

	int32 ownedItemCount = depotManager->GetOwnedItemSlotCount();
	int32 maxSlotCount = depotManager->GetDepotMaxSlotCount();
	
	// 표시할 아이템 목록 정보 갱신
	depotManager->GetItemDBIDByStorageType((EInventoryTab)InIndex, _eDepotSortType, _displayItemList);

	// UI 갱신
	_iconSlotHelper->RefreshAll(maxSlotCount);

	// 슬롯 카운트 텍스트 갱신
	FText slotCountText;
	FGsUIStringHelper::GetTwoValueSlashedText(ownedItemCount, maxSlotCount, slotCountText);
	EGsUIColorType colorType = (ownedItemCount > maxSlotCount) ? EGsUIColorType::DEFAULT_LACK : EGsUIColorType::DEFAULT_ENOUGH;

	_depotSlotCountText->SetText(slotCountText);
	_depotSlotCountText->SetColorAndOpacity(FGsUIColorHelper::GetColor(colorType));

	// 아이템 목록이 비었을 경우 패널 노출
	_panelEmptyItemList->SetVisibility((0 < _displayItemList.Num()) ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}

void UGsUIWindowDepot::AutoSettingListRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	UGsUIAutoDepositEntry* entry = Cast<UGsUIAutoDepositEntry>(InEntry);
	if (nullptr == entry)
		return;

	if (false == _autoDepositSettingList.IsValidIndex(InIndex))
		return;

	if (_isAutoDepositGroupGradeRefresh)
	{
		entry->SetGrade(_autoDepositSettingList[InIndex]._grade, _autoDepositSettingList[InIndex].HasAutoDepositItem());
		return;
	}

	const FGsSchemaAutoDepotConfig* config = _autoDepositSettingList[InIndex]._autoDepositConfig;
	if (nullptr == config)
		return;

	if (false == entry->OnChangeAutoDepositGrade.IsBound())
	{
		entry->OnChangeAutoDepositGrade.AddUObject(this, &UGsUIWindowDepot::OnChangeAutoDepositGrade);
	}

	entry->SetIcon(config->groupIcon);
	entry->SetGroupName(config->groupName);
	entry->SetGradeRange(config->minGrade, config->maxGrade);
	entry->SetTooltipText(config->groupDesc);

	// 설정 버튼을 눌렀을 때 _isAutoDepositGroupGradeRefresh = true 인 UI 갱신 로직을 이용한다.
	//entry->SetGrade(_autoDepositSettingList[InIndex]._grade, _autoDepositSettingList[InIndex].HasAutoDepositItem());
}

void UGsUIWindowDepot::OnRefreshIconAutoDeposit(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
		return;

	if (false == _autoDepositSettingList.IsValidIndex(_toggleGroupAutoDeposit.GetSelectedIndex()))
		return;

	const AutoDepositSetting& autoDepositSetting = _autoDepositSettingList[_toggleGroupAutoDeposit.GetSelectedIndex()];
	if (false == autoDepositSetting._itemIdList.IsValidIndex(InIndex))
		return;

	ItemId itemId = autoDepositSetting._itemIdList[InIndex].Key;

	if (_isAutoDepositSelectionRefresh)
	{
		itemIcon->SetSlotSelectionImage(autoDepositSetting._autoDepositMap[itemId]);
		return;
	}

	if (false == itemIcon->OnFinishedLongPressEvent.IsBound())
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIWindowDepot::OnLongPressAutoDepositIcon);
	}

	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIWindowDepot::OnClickAutoDepositIcon);
	}

	itemIcon->SetNotOwnItem(itemId, 1);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetItemNameToolTipFlag(true);
	itemIcon->SetItemNameToolTipHoverMode(true);
	itemIcon->SetSlotSelectionImage(autoDepositSetting._autoDepositMap[autoDepositSetting._itemIdList[InIndex].Key]);
}

void UGsUIWindowDepot::OnClickIconSlot(class UGsUIIconItem& InIcon)
{
	FGsItem* itemData = InIcon.GetItemIconData();
	if (nullptr == itemData)
		return;

	GSDepot()->RemoveDepotBasketItem(itemData->GetDBID());
}

void UGsUIWindowDepot::OnClickDepotIconSlot(class UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
		return;

	if (nullptr == itemIcon->GetItemIconData())
		return;

	if (_curSelectIcon.IsValid())
	{
		if (_curSelectIcon == itemIcon)
		{
			CloseCalculatorUI();
			return;
		}
		ResetSelection();
	}

	FGsItem* itemData = itemIcon->GetItemIconData();
	if (nullptr == itemData)
		return;

	_curSelectIcon = itemIcon;
	_curSelectIcon.Get()->SetSlotSelectionImage(true);

	FGsDepotManager* depotMgr = GSDepot();
	if (true == depotMgr->IsNeedCalculator(itemData->GetDBID(), false))
	{
		OpenCalculatorUI(itemData);
	}
	else
	{
		depotMgr->AddOrUpdateDepotBasketItem(itemData->GetDBID(), 1, FGsDepotManager::DepotActionState::State_Take);
		CloseCalculatorUI();
	}
}

void UGsUIWindowDepot::CloseCalculatorUI()
{
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->CloseByKeyName(TEXT("PopupCalculator"));
	}

	ResetSelection();
}

void UGsUIWindowDepot::OpenCalculatorUI(const FGsItem* InTargetItem)
{
	if (UGsUIManager* uiManager = GUI())
	{
		bool isOpened = uiManager->IsActiveWidget(TEXT("PopupCalculator"));

		if (UGsUIPopupCalculator* popup = Cast<UGsUIPopupCalculator>(uiManager->OpenAndGetWidget(TEXT("PopupCalculator"))))
		{
			popup->SetResetCount();
			popup->SetMaximumCount(InTargetItem->GetAmount());
			popup->SetMinimumCount(0);
			popup->SetDefaultCount(InTargetItem->GetAmount());
			popup->InvalidateNumText();
			popup->SetTouchState(false);

			FText findText;
			FText::FindText(TEXT("ContextUIText"), TEXT("calculator_receive"), findText);
			popup->SetTitleText(findText);

			if (UGsGameObjectLocalPlayer* player = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer)))
			{
				if (const FGsStatInfo* statInfo = player->GetCreatureStatInfoByType(StatType::MAX_WEIGHT))
				{
					int32 currentWeight = GItem()->GetOwnedItemWeight() + GSDepot()->GetBasketTotalWeight() - GSDepot()->GetItemTotalWeightInBasket(InTargetItem->GetDBID());

					popup->SetWeight(InTargetItem->GetWeight(), statInfo->GetStatValue(), currentWeight);
					popup->SetMode(true);
				}
			}

			if (false == popup->OnFinishCalculateDelegate.IsBoundToObject(this))
			{
				if (isOpened && popup->OnResetCalculateDelegate.IsBound())
				{
					popup->OnResetCalculateDelegate.Broadcast();
				}
					
				popup->OnResetCalculateDelegate.Clear();
				popup->OnResetCalculateDelegate.AddUObject(this, &UGsUIWindowDepot::ResetSelection);

				popup->OnFinishCalculateDelegate.Clear();
				popup->OnFinishCalculateDelegate.AddUObject(this, &UGsUIWindowDepot::ConfirmItemStackCount);

				popup->OnCancelCalculateDelegate.Clear();
				popup->OnCancelCalculateDelegate.AddUObject(this, &UGsUIWindowDepot::CancelCalculatorPopup);
			}
		}
	}
}

void UGsUIWindowDepot::CloseItemDetailUI()
{
	_curSelectIcon.Reset();
}

void UGsUIWindowDepot::ConfirmItemStackCount(int64 InCount)
{
	FGsDepotManager* depotManager = GSDepot();
	if (nullptr == depotManager)
		return;

	if (false == _curSelectIcon.IsValid())
		return;

	FGsItem* itemData = _curSelectIcon.Get()->GetItemIconData();
	if (nullptr == itemData)
		return;

	if (InCount == 0)
	{
		depotManager->RemoveDepotBasketItem(itemData->GetDBID());
	}
	else
	{
		depotManager->AddOrUpdateDepotBasketItem(itemData->GetDBID(), InCount, FGsDepotManager::DepotActionState::State_Take);
	}
	
	CloseCalculatorUI();
}

void UGsUIWindowDepot::CancelCalculatorPopup()
{
	CloseCalculatorUI();
}

void UGsUIWindowDepot::ResetSelection()
{
	if (_curSelectIcon.IsValid())
	{
		_iconSlotHelper->RefreshByIndex(_curSelectIcon.Get()->GetSlotIntParamData());
	}

	_curSelectIcon.Reset();
}

void UGsUIWindowDepot::OnClickAutoDepositIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
		return;

	if (nullptr == itemIcon->GetItemIconData())
		return;

	FGsItem* itemData = itemIcon->GetItemIconData();
	if (nullptr == itemData)
		return;

	int32 selectedGroupIndex = _toggleGroupAutoDeposit.GetSelectedIndex();
	if (false == _autoDepositSettingList.IsValidIndex(selectedGroupIndex))
		return;

	AutoDepositSetting& autoDepositSetting = _autoDepositSettingList[selectedGroupIndex];
	if (false == autoDepositSetting._autoDepositMap.Contains(itemData->GetTID()))
		return;

	// 목록에서 아이콘 클릭 시 보관 상태 토글
	bool isSelected = !autoDepositSetting._autoDepositMap[itemData->GetTID()];

	autoDepositSetting._autoDepositMap[itemData->GetTID()] = isSelected;
	autoDepositSetting._grade = ItemGrade::NONE;

	itemIcon->SetSlotSelectionImage(isSelected);

	// 그룹 슬롯의 등급 상태 갱신 (설정 없음 or 유저 설정)
	UGsUIAutoDepositEntry* groupEntry = Cast<UGsUIAutoDepositEntry>(_toggleGroupAutoDeposit.GetEntry(selectedGroupIndex));
	if (nullptr == groupEntry)
		return;

	groupEntry->SetGrade(autoDepositSetting._grade, autoDepositSetting.HasAutoDepositItem());
}

void UGsUIWindowDepot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);

	if (false == _findEndScrollOffeset)
	{
		float curOffset = _rootScrollBox->GetScrollOffset();
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
	if (false == _rootScrollBox->HasMouseCapture() && true == _isScrolling)
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

void UGsUIWindowDepot::InvalidateWeight()
{
	FGsDepotManager* depotManager = GSDepot();
	if (nullptr == depotManager)
		return;

	UGsInventoryManager* inventoryManager = GSInventory();
	if (nullptr == inventoryManager)
		return;

	// 예상 무게 텍스트
	int32 expectWeight = depotManager->GetExpectWeight();
	float expectWeightPercent = FGsItemHelper::GetWeightPercent(expectWeight);

	FText expectPercentText;
	FGsItemHelper::GetWeightPercentText(expectWeightPercent, expectPercentText);
	_expectWeightPerText->SetText(expectPercentText);

	float progressValue = expectWeightPercent > 0.f ? expectWeightPercent / 100.f : 0.f;
	_expectWeightProgressBar->SetPercent(progressValue);

	float weightPenaltyPercentFirst = inventoryManager->GetOwnedItemPenaltyValueByType(WeightPenaltyGrade::FIRST);
	float weightPenaltyPercentSecond = inventoryManager->GetOwnedItemPenaltyValueByType(WeightPenaltyGrade::SECOND);

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
}

void UGsUIWindowDepot::InvalidatePrice()
{
	FGsDepotManager* depotManager = GSDepot();
	if (nullptr == depotManager)
		return;

	int32 expectCost = depotManager->GetBasketItemsPrice();
	bool isEnoughCost = FGsCurrencyHelper::CheckCurrency(CurrencyType::GOLD, expectCost, false);

	_priceText->SetText(FText::AsNumber(expectCost));
	_priceText->SetColorAndOpacity(FGsUIColorHelper::GetColor(isEnoughCost ? EGsUIColorType::ENCHANT_ENOUGH_COST : EGsUIColorType::ENCHANT_NEED_COST));
}

void UGsUIWindowDepot::SortDepotItemList(DepotSortType InSortType)
{
	_eDepotSortType = InSortType;

	SetSortPanelVisibility(false);

	FGsDepotManager* depotManager = GSDepot();
	if (nullptr == depotManager)
		return;

	depotManager->SortItemListByDBID(InSortType, _displayItemList);
	_iconSlotHelper->RefreshAll();
}

void UGsUIWindowDepot::OnClickDefaultSort()
{
	SortDepotItemList(DepotSortType::SortDefault);
}

void UGsUIWindowDepot::OnClickGradeSort()
{
	SortDepotItemList(DepotSortType::SortGrade);
}

void UGsUIWindowDepot::OnClickWeightSort()
{
	SortDepotItemList(DepotSortType::SortWeight);
}

void UGsUIWindowDepot::OnClickAutoSetting()
{
	FGsDepotManager* depotMgr = GSDepot();
	if (nullptr == depotMgr)
		return;

	TArray<TPair<int32, ItemGrade>> itemGradeList;
	TMap<ItemId, bool> settingData;
	for (const AutoDepositSetting& autoDepositSetting : _autoDepositSettingList)
	{
		for (const TPair<ItemId, bool>& itemInfo : autoDepositSetting._autoDepositMap)
		{
			if (false == settingData.Contains(itemInfo.Key))
			{
				settingData.Emplace(itemInfo.Key, itemInfo.Value);
			}
		}

		itemGradeList.Emplace(autoDepositSetting._groupId, autoDepositSetting._grade);
	}

	depotMgr->SaveAutoDepositSettingData(settingData, itemGradeList);

	SwitchLeftPopupUI(SWITCHER_INDEX_STORAGE);
}

void UGsUIWindowDepot::OnClickConfirm()
{
	FGsDepotManager* depotMgr = GSDepot();
	if (nullptr == depotMgr)
		return;

	if (_isDeposit)
	{
		depotMgr->SendDepositItems();
	}
	else
	{
		depotMgr->SendTakeItems();
	}
}

void UGsUIWindowDepot::OnClickSetup()
{
	if (_depotAreaSwitcher->GetActiveWidgetIndex() == SWITCHER_INDEX_AUTO_DEPOSIT)
		return;

	FGsDepotManager* depotManager = GSDepot();
	if (nullptr == depotManager)
		return;

	// 서버에 저장된 정보로 초기화
	for (AutoDepositSetting& autoDepositSetting : _autoDepositSettingList)
	{
		autoDepositSetting._grade = depotManager->GetAutoDepositGrade(autoDepositSetting._groupId);
		autoDepositSetting._autoDepositMap.Empty();
		for (const TPair<ItemId, ItemGrade>& itemInfo : autoDepositSetting._itemIdList)
		{
			autoDepositSetting._autoDepositMap.Emplace(itemInfo.Key, depotManager->IsSelectedAutoSlot(itemInfo.Key) || depotManager->IsLowerOrEqualAutoDepositGrade(autoDepositSetting._groupId, itemInfo.Value));
		}
	}

	// 자동 보관 설정 목록의 등급 정보 UI 갱신
	_isAutoDepositGroupGradeRefresh = true;
	_autoSettingPanelSlotHelper->RefreshAll(_autoDepositSettingList.Num());
	_isAutoDepositGroupGradeRefresh = false;

	// 자동 보관 설정 목록의 슬롯 선택 정보 초기화
	_toggleGroupAutoDeposit.SetSelectedIndexNoEvent(-1);
	
	// 자동 보관 설정 팝업으로 스위칭
	SwitchLeftPopupUI(SWITCHER_INDEX_AUTO_DEPOSIT);

	SetSortPanelVisibility(false);
}

void UGsUIWindowDepot::OnClickCloseSetup()
{
	SwitchLeftPopupUI(SWITCHER_INDEX_STORAGE);
}

void UGsUIWindowDepot::AutoScroll(float InCurveData)
{
	_isAutoScrolling = true;

	float curOffset = _rootScrollBox->GetScrollOffset();
	float curSlotPos = GetSlotPositionValueByIndexType(_eSlotBasketIndexType);
	float endOffset = _rootScrollBox->GetScrollOffsetOfEnd();

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
			_rootScrollBox->SetScrollOffset(curOffset - InCurveData);
		}
		else
		{
			_isScrolling = false;
			_isAutoScrolling = false;
			_timer->Stop();
			_rootScrollBox->SetScrollOffset(curSlotPos);
			_eScrollDirectionType = ScrollDircetionType::Scroll_None;
		}
	}
	break;

	case ScrollDircetionType::Scroll_Return_Right:
	{
		// 도착했으면 강제 setOffset
		if (curOffset < curSlotPos)
		{
			_rootScrollBox->SetScrollOffset(curOffset + InCurveData);
		}
		else
		{
			_isScrolling = false;
			_isAutoScrolling = false;
			_timer->Stop();
			_rootScrollBox->SetScrollOffset(curSlotPos);
			_eScrollDirectionType = ScrollDircetionType::Scroll_None;
		}
	}
	break;

	case ScrollDircetionType::Scroll_Left:
	{
		float value = GetSlotPositionValueByIndexType(GetExpectPosType(false));
		if (value < curOffset)
		{
			_rootScrollBox->SetScrollOffset(curOffset - InCurveData);
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
			_rootScrollBox->SetScrollOffset(curOffset + InCurveData);
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

void UGsUIWindowDepot::CurrentFocusingSlotPositionCheck()
{
	float curOffset = _rootScrollBox->GetScrollOffset();
	float startOffset = GetSlotPositionValueByIndexType(SlotIndexType::FirstSlotGroupIndex);
	float endOffset = GetSlotPositionValueByIndexType(SlotIndexType::SixthSlotGroupIndex);
	float calcStartOffset = curOffset - startOffset;
	float calcEndOffset = curOffset - endOffset;

	float reusltStartOffset = fabs(calcStartOffset);
	float reusltEndOffset = fabs(calcEndOffset);

	if (FISNISHED_GOAL_POSITION_VALIDCHECK_VALUE >= reusltStartOffset) // start pos 
	{
		++_slotMoveCount;
		SetSlotMove(SlotIndexType::SixthSlotGroupIndex, SlotIndexType::FirstSlotGroupIndex);
		SetSlotPositionValueByIndexType(SlotIndexType::SecondSlotGroupIndex);
	}
	else if (FISNISHED_GOAL_POSITION_VALIDCHECK_VALUE >= reusltEndOffset) // end Pos
	{
		--_slotMoveCount;
		SetSlotMove(SlotIndexType::FirstSlotGroupIndex, SlotIndexType::SixthSlotGroupIndex);
		SetSlotPositionValueByIndexType(SlotIndexType::FifthSlotGroupIndex);
	}

}

void UGsUIWindowDepot::SetDefaultFocusingPos()
{
	float calcOffset = _endScrollOffset / (float)SlotIndexType::SixthSlotGroupIndex;
	for (uint8 i = 0; i < SlotIndexType::SlotGroupMax; ++i)
	{
		_basketSlotPositionMap.Emplace((SlotIndexType)i, i * calcOffset);
	}

	SetSlotPositionValueByIndexType(SlotIndexType::FirstSlotGroupIndex);
}

UGsUIWindowDepot::SlotIndexType UGsUIWindowDepot::GetExpectPosType(bool InIsAfter)
{
	if (_eSlotBasketIndexType == SlotIndexType::FirstSlotGroupIndex)
	{
		return false == InIsAfter ? SlotIndexType::SixthSlotGroupIndex : SlotIndexType::SecondSlotGroupIndex;
	}
	else if (_eSlotBasketIndexType == SlotIndexType::SixthSlotGroupIndex)
	{
		return true == InIsAfter ? SlotIndexType::FirstSlotGroupIndex : SlotIndexType::FifthSlotGroupIndex;
	}

	return true == InIsAfter ? (SlotIndexType)(_eSlotBasketIndexType + 1) : (SlotIndexType)(_eSlotBasketIndexType - 1);
}

float UGsUIWindowDepot::GetSlotPositionValueByIndexType(const SlotIndexType InType)
{
	if (false == _basketSlotPositionMap.Contains(InType))
		return 0.f;

	return _basketSlotPositionMap[InType];
}

void UGsUIWindowDepot::SetSlotPositionValueByIndexType(const SlotIndexType InType)
{
	_rootScrollBox->SetScrollOffset(GetSlotPositionValueByIndexType(InType));
	_eSlotBasketIndexType = InType;
}

void UGsUIWindowDepot::SetFocusingIndexSwitcher(const ScrollDircetionType InDirType)
{
	if (ScrollDircetionType::Scroll_Left == InDirType)
	{
		_eSlotBasketPageType = SlotPageType::FirstPage == _eSlotBasketPageType ?
			SlotPageType::SixthPage : (SlotPageType)(_eSlotBasketPageType - 1);
	}
	else if (ScrollDircetionType::Scroll_Right == InDirType)
	{
		_eSlotBasketPageType = SlotPageType::SixthPage == _eSlotBasketPageType ?
			SlotPageType::FirstPage : (SlotPageType)(_eSlotBasketPageType + 1);
	}

	_firstPageSwitcher->SetActiveWidgetIndex(SlotPageType::FirstPage == _eSlotBasketPageType ? 0 : 1);
	_secondPageSwitcher->SetActiveWidgetIndex(SlotPageType::SecondPage == _eSlotBasketPageType ? 0 : 1);
	_thirdPageSwitcher->SetActiveWidgetIndex(SlotPageType::ThirdPage == _eSlotBasketPageType ? 0 : 1);
	_fourthPageSwitcher->SetActiveWidgetIndex(SlotPageType::FourthPage == _eSlotBasketPageType ? 0 : 1);
	_fifthPageSwitcher->SetActiveWidgetIndex(SlotPageType::FifthPage == _eSlotBasketPageType ? 0 : 1);
	_sixthPageSwitcher->SetActiveWidgetIndex(SlotPageType::SixthPage == _eSlotBasketPageType ? 0 : 1);
}

void UGsUIWindowDepot::SetSlotMove(int32 InMoveTargetPageIndex, int32 InDestPageIndex)
{
	TSharedRef<class SWidget> sharedRef = _rootPanel->TakeWidget();
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

void UGsUIWindowDepot::InvalidateBasketScroll(int32 InTargetPageIndex)
{
	// 패널 초기화
	for (int32 i = 0; i < _slotMoveCount; ++i)
	{
		SetSlotMove(SlotIndexType::FirstSlotGroupIndex, SlotIndexType::SixthSlotGroupIndex);
	}
	for (int32 i = _slotMoveCount; i < 0; ++i)
	{
		SetSlotMove(SlotIndexType::SixthSlotGroupIndex, SlotIndexType::FirstSlotGroupIndex);
	}

	// 패널 위치 재설정 및 스크롤 위치 설정
	SlotIndexType nextIndex = static_cast<SlotIndexType>(InTargetPageIndex);
	if (nextIndex == SlotIndexType::FirstSlotGroupIndex)
	{
		_slotMoveCount = 1;
		SetSlotMove(SlotIndexType::SixthSlotGroupIndex, SlotIndexType::FirstSlotGroupIndex);
		SetSlotPositionValueByIndexType(SlotIndexType::SecondSlotGroupIndex);
	}
	else if (nextIndex == SlotIndexType::SixthSlotGroupIndex)
	{
		_slotMoveCount = -1;
		SetSlotMove(SlotIndexType::FirstSlotGroupIndex, SlotIndexType::SixthSlotGroupIndex);
		SetSlotPositionValueByIndexType(SlotIndexType::FifthSlotGroupIndex);
	}
	else
	{
		_slotMoveCount = 0;
		SetSlotPositionValueByIndexType(nextIndex);
	}

	_eSlotBasketPageType = static_cast<SlotPageType>(InTargetPageIndex);
	SetFocusingIndexSwitcher(ScrollDircetionType::Scroll_None);
}

FReply UGsUIWindowDepot::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

void UGsUIWindowDepot::InvalidateAllInternal()
{
	_autoDepositItemList->InvalidateAll();
}

void UGsUIWindowDepot::ExtendDepotSlot(const IGsMessageParam* InParam)
{
	_toggleGroup.SetSelectedIndex(_toggleGroup.GetSelectedIndex(), true);

	FGsDepotManager* depotMgr = GSDepot();
	if (nullptr == depotMgr)
		return;

	SetExtendButtonVisibility(depotMgr->CanExtendDepotSlot());

	// 확장을 성공하면, 인벤토리 스크롤 박스를 제일 아래로 내려준다
	_scrollBox->ScrollToEnd();
	_scrollBox->EndInertialScrolling();
}

void UGsUIWindowDepot::OnUpdateWeight(const struct IGsMessageParam* InParam)
{
	InvalidateWeight();
}

void UGsUIWindowDepot::OnUpdateCurrency(uint64 InCurrencyType)
{
	CurrencyType currencyType = static_cast<CurrencyType>(InCurrencyType);
	if (CurrencyType::GOLD == currencyType)
	{
		InvalidatePrice();
	}
}

void UGsUIWindowDepot::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	FGsItemDetailOpenMessageParamEX::DetailOption option;
	option._bHideButtons = true;
	option._bUseCollectionMarking = true;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), UGsUIPopupItemDetailMain::CenterPosType::CenterPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
}

void UGsUIWindowDepot::OnLongPressAutoDepositIcon(UGsUIIconItem& InIcon)
{
	FGsItemDetailOpenMessageParamEX::DetailOption option;
	option._bHideButtons = true;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), UGsUIPopupItemDetailMain::CenterPosType::CenterPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
}

void UGsUIWindowDepot::SwitchLeftPopupUI(int32 InIndex)
{
	SetAutoDepositPanelVisibility(false);
	_depotAreaSwitcher->SetActiveWidgetIndex(InIndex);
}

void UGsUIWindowDepot::SetExtendButtonVisibility(bool InIsVisible)
{
	_btnItemSlotLock->SetVisibility(InIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUIWindowDepot::SetSortPanelVisibility(bool InIsVisible)
{
	if (InIsVisible)
	{
		_sortItemListPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UpdateDepotSort();
	}
	else
	{
		_sortItemListPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIWindowDepot::SetAutoDepositPanelVisibility(bool InIsVisible, int32 InIndex /*= 0*/)
{
	if (InIsVisible)
	{
		if (_autoDepositSettingList.IsValidIndex(InIndex))
		{
			TArray<ItemId> itemIdList;
			AutoDepositSetting& autoDepositSetting = _autoDepositSettingList[InIndex];
			for (const TPair<ItemId, ItemGrade>& autoDepositData : autoDepositSetting._itemIdList)
			{
				itemIdList.Emplace(autoDepositData.Key);
			}

			_autoDepositItemList->SetVisibility(ESlateVisibility::Visible);
			_autoDepositItemList->SetItemList(itemIdList, autoDepositSetting._autoDepositMap);
			InvalidateAll();
		}
		else
		{
			_autoDepositItemList->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		_autoDepositItemList->SetVisibility(ESlateVisibility::Collapsed);
	}
}
