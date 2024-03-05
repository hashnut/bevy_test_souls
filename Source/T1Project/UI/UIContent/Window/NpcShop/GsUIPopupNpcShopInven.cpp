// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupNpcShopInven.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsInventoryManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/MessageParam/GsNpcShopMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "Item/GsItemManager.h"

#include "GameObject/Stat/GsStatInfo.h"

#include "GameFlow/GameContents/ContentsGame/NpcShop/MsgReceiver/GsNpcShopInvenUIMsgReceiver.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Popup/GsUIPopupCalculator.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIContent/Popup/GsUIPopupInventory.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"

#include "NpcShop/GsNpcShopHelper.h"

#include "T1Project.h"

void UGsUIPopupNpcShopInven::BeginDestroy()
{
	if (nullptr != _iconSlotHelper)
	{
		_iconSlotHelper = nullptr;
	}
	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}
	if (nullptr != _uiMsgReceiver)
	{
		delete _uiMsgReceiver;
		_uiMsgReceiver = nullptr;
	}

	_toggleGroup.Clear();

	Super::BeginDestroy();
}

void UGsUIPopupNpcShopInven::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(_iconSelector);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIPopupNpcShopInven::OnRefreshIconWrapBox);
		_iconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUIPopupNpcShopInven::OnCreateIconWrapBox);
	}

	_toggleGroup.Clear();
	_toggleGroup.OnSlotClicked.BindUObject(this, &UGsUIPopupNpcShopInven::OnSlotClickedTab);
	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIPopupNpcShopInven::OnSelectChangedTab);
	_toggleGroup.AddToggleWidgetByParentPanel(_panelInvenTab);

	_btn_close->OnClicked.AddDynamic(this, &UGsUIPopupNpcShopInven::OnClickClose);
	_btnAutoSell->OnClicked.AddDynamic(this, &UGsUIPopupNpcShopInven::OnClickAutoSell);

	UGsInventoryManager* invenMgr = GSInventory();
	_frameRate = invenMgr->FRAME_RATE;
	_maxCount = invenMgr->CREATE_MAX_COUNT_BY_TICK;
	_minCount = invenMgr->CREATE_MIN_COUNT_BY_TICK;
	_balanceCount = invenMgr->BALANCE_COUNT;
	_createSlotCountEveryTick = invenMgr->CREATE_EVERY_TICK_COUNT_DEFAULT;

	_uiMsgReceiver = new FGsNpcShopInvenUIMsgReceiver();
}

void UGsUIPopupNpcShopInven::NativeConstruct()
{
	Super::NativeConstruct();

	_isBuyMode = true;
	_invenTotalSlotCount = GSInventory()->GetInvenSlotMaxCount();
	_currentInvenTabIndex = EInventoryTab::AllTab;
	_toggleGroup.SetSelectedIndex((int32)_currentInvenTabIndex);

	if (false == _uiMsgReceiver->IsValidUI())
	{
		_uiMsgReceiver->SetUI(this);
	}
	_uiMsgReceiver->RegisterMessage();

	_redDotImageList.Add(EInventoryTab::AllTab, _allTabRedDot);
	_redDotImageList.Add(EInventoryTab::EquipmentTab, _equipmentTabRedDot);
	_redDotImageList.Add(EInventoryTab::ConsumableTab, _consumableTabRedDot);
	_redDotImageList.Add(EInventoryTab::MaterialTab, _etcItemTypeRedDot);

	InvalidateInvenItemList();
	InvalidateAllInternal();
}

void UGsUIPopupNpcShopInven::OnClickClose()
{
	CloseItemDetailUI();
	CloseCalculatorUI();

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->CloseAllStack();
	}
}

void UGsUIPopupNpcShopInven::NativeDestruct()
{
	CloseItemDetailUI();
	CloseCalculatorUI();

	_curSelectIcon.Reset();
	
	_uiMsgReceiver->RemoveMsg();
	_iconSlotHelper->Clear();
	_displayItemList.Empty();
	_redDotImageList.Empty();

	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::INVEN, true);

	Super::NativeDestruct();
}

void UGsUIPopupNpcShopInven::OnSlotClickedTab(int32 InIndex, bool bInIsSelected)
{
	// 탭 아이콘 클릭 시 인벤토리 최상위로 이동한다.
	_scrollBox->ScrollToStart();
	_scrollBox->EndInertialScrolling();
}

// 0: AllItem  1: Equipment  2: Consumable  3: etc
void UGsUIPopupNpcShopInven::OnSelectChangedTab(int32 InIndex)
{
	if ((int32)_currentInvenTabIndex == InIndex)
		return;

	_currentInvenTabIndex = ((int32)EInventoryTab::TabMax <= InIndex) ? 
		EInventoryTab::AllTab : static_cast<EInventoryTab>(InIndex);

	InvalidateInvenItemList();
	CloseItemDetailUI();
	CloseCalculatorUI();
}

bool UGsUIPopupNpcShopInven::IsMatchedCurTab(const ItemType InType)
{
	switch (_currentInvenTabIndex)
	{
	case EInventoryTab::AllTab:
		return true;
	case EInventoryTab::EquipmentTab:
		return InType == ItemType::EQUIP;
	case EInventoryTab::ConsumableTab:
		return InType == ItemType::CONSUME;
	case EInventoryTab::MaterialTab:
		return InType == ItemType::INGREDIENT || InType == ItemType::CURRENCY;
	}
	return false;
}

void UGsUIPopupNpcShopInven::ConvertItemTypeByInvenTabType(OUT TArray<ItemType>& outTypeList)
{
	outTypeList.Empty();
	switch (_currentInvenTabIndex)
	{
	case EInventoryTab::AllTab:
	{
		outTypeList.Emplace(ItemType::EQUIP);
		outTypeList.Emplace(ItemType::CONSUME);
		outTypeList.Emplace(ItemType::INGREDIENT);
		outTypeList.Emplace(ItemType::CURRENCY);
		break;
	}
	case EInventoryTab::EquipmentTab:
		outTypeList.Emplace(ItemType::EQUIP);
		break;
	case EInventoryTab::ConsumableTab:
		outTypeList.Emplace(ItemType::CONSUME);
		break;
	case EInventoryTab::MaterialTab:
	{
		outTypeList.Emplace(ItemType::INGREDIENT);
		outTypeList.Emplace(ItemType::CURRENCY);
		break;
	}
	}
}

void UGsUIPopupNpcShopInven::InvalidateInvenItemList()
{
	ChangeShopMode(_isBuyMode);

	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	// 현재 탭에 표시되는 아이템 목록 생성
	TArray<ItemType> typeList;
	ConvertItemTypeByInvenTabType(typeList);
	_displayItemList.Empty();
	for (int32 i = 0; i < typeList.Num(); ++i)
	{
		TArray<TWeakPtr<FGsItem>> findItems = itemManager->FindItem(typeList[i]);
		int32 findItemsCount = findItems.Num();
		for (int32 j = 0; j < findItemsCount; ++j)
		{
			if (false == findItems[j].IsValid())
				continue;

			_displayItemList.Emplace(findItems[j].Pin()->GetDBID());
		}
	}

	// 목록 정렬
	itemManager->SortItemListByDBID(UGsUIPopupInventory::ESortUIList::DefaultType, _displayItemList);

	_createMaxSlotCountByTick = _invenTotalSlotCount;
	InvalidateAll();
}

void UGsUIPopupNpcShopInven::InvalidateInventorySimple()
{
	_iconSlotHelper->RefreshAll();
	CheckRedDot();
	InvalidateInvenTabSlotCount();
}

// 신규로 추가되는 아이템은 무조건 맨뒤에 add 된다고 가정
void UGsUIPopupNpcShopInven::AddItem(const uint64 InItemDBID)
{
	if (false == _displayItemList.Contains(InItemDBID))
	{
		TWeakPtr<FGsItem> itemData = GItem()->FindItem(InItemDBID);
		if (false == itemData.IsValid())
			return;

		if (false == IsMatchedCurTab(itemData.Pin()->GetType()))
			return;

		_displayItemList.Emplace(InItemDBID);
		// itemicon 모두 로드된 상태
		// nullptr 이라면 로드후에 알아서 갱신 처리함
		if (nullptr != _iconSlotHelper->GetIcon(_displayItemList.Num() - 1))
		{
			_iconSlotHelper->RefreshByIndex(_displayItemList.Num() - 1);
		}
	}
	else
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[UGsUIPopupNpcShopInven::AddItem] InItem is Already Exist Item !!! "));
#endif
	}
}

void UGsUIPopupNpcShopInven::RemoveItem(const uint64 InItemDBID)
{
	if (_displayItemList.Contains(InItemDBID))
	{
		_displayItemList.Remove(InItemDBID);
	}
}

void UGsUIPopupNpcShopInven::UpdateItem(const uint64 InItemDBID)
{
	int32 index = _displayItemList.Find(InItemDBID);
	if (INDEX_NONE == index)
	{
		return;
	}
	_iconSlotHelper->RefreshByIndex(index);
}

void UGsUIPopupNpcShopInven::ChangeShopMode(const bool InIsBuyMode)
{
	_isBuyMode = InIsBuyMode;
}

void UGsUIPopupNpcShopInven::ChangeShopType(const NpcFunctionType InNpcFunctionType)
{
	_shopNpcFunctionType = InNpcFunctionType;
	switch (InNpcFunctionType)
	{
	case NpcFunctionType::SHOP:
	{
		_btnAutoSell->SetVisibility(ESlateVisibility::Visible);
		_imageDevideLineUnder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_sizeBoxUnder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	break;
	case NpcFunctionType::EVENT_SHOP:
	case NpcFunctionType::SHOP_EQUIP:
	case NpcFunctionType::SHOP_SKILL_BOOK:
	{
		_btnAutoSell->SetVisibility(ESlateVisibility::Collapsed);
		_imageDevideLineUnder->SetVisibility(ESlateVisibility::Collapsed);
		_sizeBoxUnder->SetVisibility(ESlateVisibility::Collapsed);
	}
	break;
	}
}

// 한번에 많은양의 icon 을 그리면서 부하가 생기는 이슈가 발생하여
// tick 마다 분할 로드 하는 방식
void UGsUIPopupNpcShopInven::CreateIconSlot(float InDeltaTime)
{
	if (0 < _createMaxSlotCountByTick)
	{
		if (InDeltaTime > _frameRate) // 저사양
		{
			// 틱당 생성하는 슬롯 갯수를 줄인다
			if (_createSlotCountEveryTick > _minCount)
			{
				// 틱당 생성하는 슬롯 갯수가 최소 갯수보다 적으면 최소 갯수로 설정함
				_createSlotCountEveryTick = _minCount >= (_createSlotCountEveryTick - _balanceCount) ?
					_minCount : _createSlotCountEveryTick - _balanceCount;
			}
		}
		else // 고사양
		{
			// 틱당 생성하는 슬롯 늘린다
			if (_createSlotCountEveryTick < _maxCount)
			{
				// 틱당 생성하는 슬롯 갯수가 최대 갯수보다 많으면 최대 갯수로 설정함
				_createSlotCountEveryTick = _maxCount <= (_createSlotCountEveryTick + _balanceCount) ?
					_maxCount : _createSlotCountEveryTick + _balanceCount;
			}
		}

		_iconSlotHelper->CreateIconByTick(_createSlotCountEveryTick, _invenTotalSlotCount);
		_iconSlotHelper->RefreshIconByTick(_createSlotCountEveryTick, (_invenTotalSlotCount - _createMaxSlotCountByTick));
		_createMaxSlotCountByTick -= _createSlotCountEveryTick;

		// 마지막 생성 틱 - 로드는 끝났상태
		if (0 >= _createMaxSlotCountByTick)
		{
			_createMaxSlotCountByTick = 0;
		}
	}
}

void UGsUIPopupNpcShopInven::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	CreateIconSlot(InDeltaTime);
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIPopupNpcShopInven::InvalidateAllInternal()
{
	CheckRedDot();
	InvalidateInvenTabSlotCount();
}

void UGsUIPopupNpcShopInven::CheckRedDot()
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	bool equip = itemManager->CheckRedDotItemByItemType(ItemType::EQUIP);
	bool consume = itemManager->CheckRedDotItemByItemType(ItemType::CONSUME);
	bool ingredient = itemManager->CheckRedDotItemByItemType(ItemType::INGREDIENT) || itemManager->CheckRedDotItemByItemType(ItemType::CURRENCY);

	SetInvenTabRedDot(EInventoryTab::AllTab, (equip || consume || ingredient));
	SetInvenTabRedDot(EInventoryTab::EquipmentTab, equip);
	SetInvenTabRedDot(EInventoryTab::ConsumableTab, consume);
	SetInvenTabRedDot(EInventoryTab::MaterialTab, ingredient);

	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::INVEN, (equip || consume || ingredient));
}

void UGsUIPopupNpcShopInven::SetInvenTabRedDot(EInventoryTab InTab, bool InActiveState)
{
	if (_redDotImageList.Contains(InTab))
	{
		if (nullptr != _redDotImageList[InTab])
		{
			ESlateVisibility slateType = InActiveState ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
			_redDotImageList[InTab]->SetVisibility(slateType);
		}
	}
}

void UGsUIPopupNpcShopInven::InvalidateInvenTabSlotCount()
{
	FText slotCountText;
	int32 ownedItemCount = GItem()->GetOwnedItemCount();
	int32 maxSlotCount = _invenTotalSlotCount;

	FGsUIStringHelper::GetTwoValueSlashedText(ownedItemCount, maxSlotCount, slotCountText);
	EGsUIColorType colorType = (ownedItemCount > maxSlotCount) ?
		EGsUIColorType::DEFAULT_LACK : EGsUIColorType::DEFAULT_ENOUGH;

	_slotCountText->SetText(slotCountText);
	_slotCountText->SetColorAndOpacity(FGsUIColorHelper::GetColor(colorType));
}

void UGsUIPopupNpcShopInven::OnClickIconSlot(class UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
		return;

	FGsItem* itemData = itemIcon->GetItemIconData();
	if (nullptr == itemData)
		return;

	// 잡화상인일 경우에만 반응한다. (티커 처리등이 필요할 경우 변경 필요)
	if (NpcFunctionType::SHOP != _shopNpcFunctionType)
		return;

	EGsNotAllowedSellReaon reason = EGsNotAllowedSellReaon::NONE;
	if (false == FGsNpcShopHelper::IsAllowedSellItem(itemData, reason))
	{
		FGsNpcShopHelper::ShowTickerNotAllowedSell(reason);
		return;
	}

	if (_curSelectIcon.IsValid())
	{
		if (_curSelectIcon == itemIcon)
		{
			CloseItemDetailUI();
			CloseCalculatorUI();
			return;
		}
		ResetSelection();
	}

	_curSelectIcon = itemIcon;
	_curSelectIcon.Get()->SetSlotSelectionImage(true);

	if (1 < itemData->GetAmount())
	{
		if (IsOpenedCalculatorUI())
		{
			InitCalculatorUI(itemData);
		}
		else
		{
			OpenCalculatorUI(itemData);
		}
	}
	else
	{
		ConfirmItemStackCount(1);
		CloseCalculatorUI();
	}
}

void UGsUIPopupNpcShopInven::OpenCalculatorUI(const FGsItem* InTargetItem)
{
	if (UGsUIManager* uiManager = GUI())
	{
		if (nullptr != uiManager->OpenAndGetWidget(POPUP_CALCULATOR))
		{
			UGsUIPopupCalculator* popup = Cast<UGsUIPopupCalculator>(uiManager->GetWidgetByKey(POPUP_CALCULATOR));
			popup->SetResetCount();
			popup->SetMaximumCount(InTargetItem->GetAmount());
			popup->SetMinimumCount(0);
			popup->SetDefaultCount(1);
			popup->SetNpcShopCalcInfo(false);
			popup->InvalidateNumText();
			popup->SetTouchState(false);

			if (UGsGameObjectLocalPlayer* player = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer)))
			{
				if (const FGsStatInfo* statInfo = player->GetCreatureStatInfoByType(StatType::MAX_WEIGHT))
				{
					popup->SetWeight(InTargetItem->GetWeight(), statInfo->GetStatValue(), GSNpcShop()->GetExpectWeightValue(false));
					popup->SetMode(false);
				}
			}

			if (false == popup->OnFinishCalculateDelegate.IsBoundToObject(this))
			{
				popup->OnFinishCalculateDelegate.AddUObject(this, &UGsUIPopupNpcShopInven::OnFinishCalculate);
				popup->OnCancelCalculateDelegate.AddUObject(this, &UGsUIPopupNpcShopInven::OnCancelCalculate);
				popup->OnResetCalculateDelegate.AddUObject(this, &UGsUIPopupNpcShopInven::ResetSelection);
			}
		}
	}
}

void UGsUIPopupNpcShopInven::CloseCalculatorUI()
{
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->CloseByKeyName(POPUP_CALCULATOR);
	}
	
	ResetSelection();
}

bool UGsUIPopupNpcShopInven::IsOpenedCalculatorUI()
{
	if (UGsUIManager* uiManager = GUI())
	{
		return uiManager->IsActiveWidget(POPUP_CALCULATOR);
	}
	return false;
}

void UGsUIPopupNpcShopInven::InitCalculatorUI(const FGsItem* InTargetItem)
{
	if (UGsUIManager* uiManager = GUI())
	{
		if (UGsUIPopupCalculator* popup = Cast<UGsUIPopupCalculator>(uiManager->GetWidgetByKey(POPUP_CALCULATOR)))
		{
			popup->SetResetCount();
			popup->SetMaximumCount(InTargetItem->GetAmount());
			popup->SetMinimumCount(0);
			popup->SetDefaultCount(1);
			popup->SetNpcShopCalcInfo(false);
			popup->InvalidateNumText();
			popup->SetTouchState(false);

			if (UGsGameObjectLocalPlayer* player = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer)))
			{
				if (const FGsStatInfo* statInfo = player->GetCreatureStatInfoByType(StatType::MAX_WEIGHT))
				{
					popup->SetWeight(InTargetItem->GetWeight(), statInfo->GetStatValue(), GSNpcShop()->GetExpectWeightValue(false));
					popup->SetMode(false);
				}
			}

			if (false == popup->OnFinishCalculateDelegate.IsBoundToObject(this))
			{
				if (popup->OnResetCalculateDelegate.IsBound())
					popup->OnResetCalculateDelegate.Broadcast();
				popup->OnResetCalculateDelegate.Clear();
				popup->OnResetCalculateDelegate.AddUObject(this, &UGsUIPopupNpcShopInven::ResetSelection);
				popup->OnFinishCalculateDelegate.Clear();
				popup->OnFinishCalculateDelegate.AddUObject(this, &UGsUIPopupNpcShopInven::ConfirmItemStackCount);
				popup->OnCancelCalculateDelegate.Clear();
				popup->OnCancelCalculateDelegate.AddUObject(this, &UGsUIPopupNpcShopInven::CloseItemDetailUI);
			}
		}
	}
}

void UGsUIPopupNpcShopInven::ConfirmItemStackCount(int64 InCount)
{
	if (false == _curSelectIcon.IsValid())
		return;

	FGsItem* itemData = _curSelectIcon.Get()->GetItemIconData();
	if (nullptr == itemData)
		return;

	TArray<FGsBasketTryInputInfo> itemList;
	itemList.Emplace(itemData->GetDBID(), itemData->GetTID(), 0, InCount);
	FGsInputShoppingBasket param(itemList, false, true);
	GMessage()->GetNpcShop().SendMessage(MessageContentNpcShop::INSERT_BASKET, &param);
}

void UGsUIPopupNpcShopInven::CloseItemDetailUI()
{
	FGsItemDetailCloseMessageParam param;
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, &param);
}

void UGsUIPopupNpcShopInven::OnRefreshIconWrapBox(int32 InIndex, class UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	if (false == _displayItemList.IsValidIndex(InIndex))
	{
		itemIcon->SetEmptyIcon();
		return;
	}

	if (false == itemIcon->OnFinishedLongPressEvent.IsBound())
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIPopupNpcShopInven::OnLongPressIcon);
	}

	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIPopupNpcShopInven::OnClickIconSlot);
	}

	EGsNotAllowedSellReaon reason = EGsNotAllowedSellReaon::NONE;

	itemIcon->SetOwnedItem(_displayItemList[InIndex]);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetItemNameToolTipFlag(true);
	itemIcon->SetItemNameToolTipHoverMode(true);
	itemIcon->SetDimmedImage((false == FGsNpcShopHelper::IsAllowedSellItem(itemIcon->GetItemIconData(), reason)) || (NpcFunctionType::SHOP != _shopNpcFunctionType));
	itemIcon->SetSlotSelectionImage(IsExistItemInShoppingBasket(_displayItemList[InIndex]));
	itemIcon->SetSlotIntParamData(InIndex);
	itemIcon->SetItemCollectionMarking(itemIcon->GetItemIconData(), true);
}

void UGsUIPopupNpcShopInven::OnCreateIconWrapBox(class UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->SetEmptyIcon();
}

void UGsUIPopupNpcShopInven::OnClickAutoSell()
{
	UGsNpcShopManager* shopManager = GSNpcShop();
	if (nullptr == shopManager)
		return;

	// 자동 판매가 설정된 아이템이 있는가?
	if (false == shopManager->IsExistAutoSellItemData())
	{
		FText findText;
		FText::FindText(TEXT("NpcShopText"), TEXT("Empty_AutoSellSetting"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	FGsMessageHolder* msgHolder = GMessage();
	if (nullptr == msgHolder)
		return;

	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	TArray<FGsBasketTryInputInfo> itemList;
	TArray<TWeakPtr<FGsItem>> inventoryItemList = itemManager->FindAllItems();
	for (TWeakPtr<FGsItem> itemData : inventoryItemList)
	{
		if (false == itemData.IsValid())
			continue;

		EGsNotAllowedSellReaon reason = EGsNotAllowedSellReaon::NONE;
		if (false == FGsNpcShopHelper::IsAllowedSellItem(itemData.Pin().Get(), reason))
			continue;

		// 장비의 자동 판매 허용 확인
		ItemAmount amount = 0;
		if (itemData.Pin()->IsEquipment())
		{
			if (false == shopManager->IsEnableAutoSellEquipment(itemData.Pin()->GetMainCategory(), itemData.Pin()->GetGradeType()))
				continue;

			amount = 1;
		}
		else
		{
			if (false == shopManager->IsEnableAutoSellConsumable(itemData.Pin()->GetTID()))
				continue;

			amount = itemData.Pin()->GetAmount();
		}

		itemList.Emplace(itemData.Pin()->GetDBID(), itemData.Pin()->GetTID(), 0, amount);
	}

	if (itemList.Num() > 0)
	{
		FGsInputShoppingBasket param(itemList, false, true);
		msgHolder->GetNpcShop().SendMessage(MessageContentNpcShop::INSERT_BASKET, &param);
	}

	CloseCalculatorUI();
}

bool UGsUIPopupNpcShopInven::IsExistItemInShoppingBasket(const uint64 InItemDBID)
{
	UGsNpcShopManager* shopManager = GSNpcShop();
	if (nullptr == shopManager)
		return false;

	const TArray<FGsNpcShopQuickSlotItemInfo>& basketList = shopManager->GetBasketList();
	for (const FGsNpcShopQuickSlotItemInfo& basketItemInfo : basketList)
	{
		if (InItemDBID == basketItemInfo._itemDBID)
		{
			return true;
		}
	}

	return false;
}

void UGsUIPopupNpcShopInven::ResetSelection()
{
	if (_curSelectIcon.IsValid())
	{
		_iconSlotHelper->RefreshByIndex(_curSelectIcon.Get()->GetSlotIntParamData());
	}
	_curSelectIcon.Reset();
}

void UGsUIPopupNpcShopInven::OnFinishCalculate(int64 InCount)
{
	ConfirmItemStackCount(InCount);
	ResetSelection();
}

void UGsUIPopupNpcShopInven::OnCancelCalculate()
{
	CloseItemDetailUI();
	ResetSelection();
}

void UGsUIPopupNpcShopInven::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	FGsItemDetailOpenMessageParamEX::DetailOption option;
	option._bHideButtons = true;
	option._bHideBG = false;
	option._bHideButtonsBottom = false;
	option._bUseLock = false;
	option._bUseCollectionMarking = true;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), FGsItemDetailOpenMessageParamEX::PosDir::CenterPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
}