// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIDepotInven.h"

#include "T1Project.h"

#include "Item/GsItem.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsDepotManager.h"
#include "Management/ScopeGame/GsInventoryManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Item/GsItemManager.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"

#include "UI/UIContent/Popup/GsUIPopupCalculator.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIContent/Popup/GsUIPopupInventory.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"

#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsItemMessageParam.h"

#include "GameObject/Stat/GsStatInfo.h"


void UGsUIDepotInven::BeginDestroy()
{
	if (nullptr != _iconSlotHelper)
	{
		_iconSlotHelper = nullptr;
	}

	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}

	_toggleGroup.Clear();


	Super::BeginDestroy();
}

void UGsUIDepotInven::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _iconSlotHelper)
	{
		_iconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_iconSlotHelper->Initialize(_iconSelector);
		_iconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIDepotInven::OnRefreshIconWrapBox);
	}

	_toggleGroup.Clear();
	_toggleGroup.OnSlotClicked.BindUObject(this, &UGsUIDepotInven::OnSlotClickedTab);
	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIDepotInven::OnSelectChangedTab);
	_toggleGroup.AddToggleWidgetByParentPanel(_panelInvenTab);

	_btnClose->OnClicked.AddDynamic(this, &UGsUIDepotInven::OnClickClose);
	_btnAutoDeposit->OnClicked.AddDynamic(this, &UGsUIDepotInven::OnClickAutoDeposit);

	UGsInventoryManager* invenMgr = GSInventory();
	_frameRate = invenMgr->FRAME_RATE;
	_maxCount = invenMgr->CREATE_MAX_COUNT_BY_TICK;
	_minCount = invenMgr->CREATE_MIN_COUNT_BY_TICK;
	_balanceCount = invenMgr->BALANCE_COUNT;
	_createSlotCountEveryTick = invenMgr->CREATE_EVERY_TICK_COUNT_DEFAULT;
}

void UGsUIDepotInven::NativeConstruct()
{
	Super::NativeConstruct();

	RegisterUIMsg();

	_invenTotalSlotCount = GSInventory()->GetInvenSlotMaxCount();
	_toggleGroup.SetSelectedIndex(0, true);

	InvalidateSlotCount();
}

void UGsUIDepotInven::NativeDestruct()
{
	RemoveUIMsg();

	_displayItemList.Empty();

	Super::NativeDestruct();
}

void UGsUIDepotInven::RegisterUIMsg()
{
	FGsMessageHolder* msg = GMessage();
	_itemContentsDelegateList.Emplace(msg->GetItemContents().AddUObject(MessageContentItem::DEPOT_INVEN_INVALIDATE,
		this, &UGsUIDepotInven::InvalidateInventory));
	_itemContentsDelegateList.Emplace(msg->GetItemContents().AddUObject(MessageContentItem::DEPOT_BASKET_ADD,
		this, &UGsUIDepotInven::AddBasketItems));
	_itemContentsDelegateList.Emplace(msg->GetItemContents().AddUObject(MessageContentItem::DEPOT_BASKET_ADD_LIST,
		this, &UGsUIDepotInven::AddBasketItemList));
	_itemContentsDelegateList.Emplace(msg->GetItemContents().AddUObject(MessageContentItem::DEPOT_BASKET_REMOVE,
		this, &UGsUIDepotInven::RemoveBasketItems));
	_itemContentsDelegateList.Emplace(msg->GetItemContents().AddUObject(MessageContentItem::DEPOT_STATE_DEPOSIT,
		this, &UGsUIDepotInven::OnChangeStateDeposit));
	_itemContentsDelegateList.Emplace(msg->GetItemContents().AddUObject(MessageContentItem::DEPOT_STATE_TAKEOUT,
		this, &UGsUIDepotInven::OnChangeStateTake));

	_listItemDelegate.Emplace(msg->GetItem().AddUObject(MessageItem::ADDITEMLIST,
		this, &UGsUIDepotInven::AddItemList));
	_listItemDelegate.Emplace(msg->GetItem().AddUObject(MessageItem::UPDATEITEMLIST,
		this, &UGsUIDepotInven::UpdateItemList));
	_listItemDelegate.Emplace(msg->GetItem().AddUObject(MessageItem::REMOVEITEMS,
		this, &UGsUIDepotInven::RemoveItemList));
}

void UGsUIDepotInven::RemoveUIMsg()
{
	FGsMessageHolder* msg = GMessage();
	for (auto& iter : _itemContentsDelegateList)
	{
		msg->GetItemContents().Remove(iter);
	}
	_itemContentsDelegateList.Empty();

	for (auto& iter : _listItemDelegate)
	{
		msg->GetItem().Remove(iter);
	}
	_listItemDelegate.Empty();
}

void UGsUIDepotInven::AddBasketItems(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
		return;

	const FGsOnChangeDepotBasketItem* param = InParam->Cast<const FGsOnChangeDepotBasketItem>();
	if (nullptr == param)
		return;

	InvalidateIcon(param->_itemDBID);
}

void UGsUIDepotInven::AddBasketItemList(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
		return;

	const FGsOnAddDepotBasketItemList* param = InParam->Cast<const FGsOnAddDepotBasketItemList>();
	if (nullptr == param)
		return;

	for (ItemDBId itemDBId : param->_itemDBIdList)
	{
		InvalidateIcon(itemDBId);
	}
}

void UGsUIDepotInven::RemoveBasketItems(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
		return;

	const FGsOnChangeDepotBasketItem* param = InParam->Cast<const FGsOnChangeDepotBasketItem>();
	if (nullptr == param)
		return;

	InvalidateIcon(param->_itemDBID);
}

void UGsUIDepotInven::OnChangeStateDeposit(const IGsMessageParam* InParam)
{

}

void UGsUIDepotInven::OnChangeStateTake(const IGsMessageParam* InParam)
{
	_iconSlotHelper->RefreshAll();
	CloseItemDetailUI();
}

void UGsUIDepotInven::RspInvenItemList(const IGsMessageParam* InParam)
{
	const FGsDepotInvenItemList* param = InParam->Cast<const FGsDepotInvenItemList>();
	if (nullptr == param)
		return;

	_displayItemList.Empty();
	_displayItemList = param->_itemDBIDList;
	
	GItem()->SortItemListByDBID(UGsUIPopupInventory::ESortUIList::DefaultType, _displayItemList);

	_iconSlotHelper->RefreshAll(_invenTotalSlotCount);

	InvalidateSlotCount();
	InvalidateEmptyItemList();
	CloseItemDetailUI();
}

void UGsUIDepotInven::InvalidateEmptyItemList()
{
	_panelEmptyItemList->SetVisibility((0 < _displayItemList.Num()) ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}

void UGsUIDepotInven::OnClickAutoDeposit()
{
	FGsDepotManager* depotManager = GSDepot();
	if (nullptr == depotManager)
		return;

	depotManager->AutoDepositProc();
}

void UGsUIDepotInven::OnClickClose()
{
	CloseItemDetailUI();

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->CloseAllStack();
	}
}

void UGsUIDepotInven::OnSlotClickedTab(int32 InIndex, bool bInIsSelected)
{
	_scrollBox->ScrollToStart();
	_scrollBox->EndInertialScrolling();
}

void UGsUIDepotInven::OnSelectChangedTab(int32 InIndex)
{
	UGsInventoryManager* invenMgr = GSInventory();
	if (nullptr == invenMgr)
		return;

	_displayItemList.Empty();
	_displayItemList = invenMgr->GetDisplayItemList(ConvertInvenTabType(InIndex));

	_iconSlotHelper->RefreshAll(_invenTotalSlotCount);

	InvalidateEmptyItemList();
	InvalidateSlotCount();
	CloseItemDetailUI();
}

void UGsUIDepotInven::OnRefreshIconWrapBox(int32 InIndex, class UGsUIIconBase* InIcon)
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
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIDepotInven::OnLongPressIcon);
	}

	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIDepotInven::OnClickIconSlot);
	}

	itemIcon->SetOwnedItem(_displayItemList[InIndex]);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetItemNameToolTipFlag(true);
	itemIcon->SetItemNameToolTipHoverMode(true);
	itemIcon->SetSlotIntParamData(InIndex);
	itemIcon->SetItemCollectionMarking(itemIcon->GetItemIconData(), true);

	FGsDepotManager* depotMgr = GSDepot();
	itemIcon->SetDimmedImage(!depotMgr->IsAllowedDepositItem(_displayItemList[InIndex]));
	if (0 >= depotMgr->GetBasketItemCount())
	{
		itemIcon->SetSlotSelectionImage(false);
	}
	else
	{
		itemIcon->SetSlotSelectionImage(depotMgr->IsExistItemInBasketByDBID(_displayItemList[InIndex]));
	}
}

void UGsUIDepotInven::InvalidateInventory(const IGsMessageParam* InParam)
{
	// OnSelectChangedTab 수행
	_toggleGroup.SetSelectedIndex(_toggleGroup.GetSelectedIndex(), true);
}

void UGsUIDepotInven::InvalidateIcon(ItemDBId InItemDBId)
{
	for (int32 i = 0; i < _displayItemList.Num(); ++i)
	{
		ItemDBId displayDBID = _displayItemList[i];
		if (displayDBID == InItemDBId)
		{
			_iconSlotHelper->RefreshByIndex(i);
			return;
		}
	}
}

// 아이템 카운트
void UGsUIDepotInven::InvalidateSlotCount()
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

void UGsUIDepotInven::OnClickIconSlot(class UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
		return;

	FGsItem* itemData = itemIcon->GetItemIconData();
	if (false == FGsItem::IsValidItem(itemData))
		return;

	FGsDepotManager* depotManager = GSDepot();
	if (nullptr == depotManager)
		return;

	if (false == depotManager->IsAllowedDepositItem(itemData->GetDBID()))
		return;

	if (_curSelectIcon.IsValid())
	{
		if (_curSelectIcon == itemIcon)
		{
			CloseItemDetailUI();
			return;
		}
		ResetSelection();
	}
	_curSelectIcon = itemIcon;
	_curSelectIcon.Get()->SetSlotSelectionImage(true);

	if (true == depotManager->IsNeedCalculator(itemData->GetDBID(), true))
	{
		if (true == IsOpenedCalculatorUI())
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
		depotManager->AddOrUpdateDepotBasketItem(itemData->GetDBID(), 1, FGsDepotManager::DepotActionState::State_Deposit);
		CloseCalculatorUI();
	}
}

void UGsUIDepotInven::OpenCalculatorUI(const FGsItem* InTargetItem)
{
	if (UGsUIManager* uiManager = GUI())
	{
		if (nullptr != uiManager->OpenAndGetWidget(TEXT("PopupCalculator")))
		{
			UGsUIPopupCalculator* popup = Cast<UGsUIPopupCalculator>(uiManager->GetWidgetByKey(TEXT("PopupCalculator")));
			popup->SetResetCount();
			popup->SetMaximumCount(InTargetItem->GetAmount());
			popup->SetMinimumCount(0);
			popup->SetDefaultCount(InTargetItem->GetAmount());
			popup->InvalidateNumText();
			popup->SetTouchState(false);

			FText findText;
			FText::FindText(TEXT("ContextUIText"), TEXT("calculator_keep"), findText);
			popup->SetTitleText(findText);

			if (UGsGameObjectLocalPlayer* player = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer)))
			{
				if (const FGsStatInfo* statInfo = player->GetCreatureStatInfoByType(StatType::MAX_WEIGHT))
				{
					int32 currentWeight = GItem()->GetOwnedItemWeight() - GSDepot()->GetBasketTotalWeight() + GSDepot()->GetItemTotalWeightInBasket(InTargetItem->GetDBID());

					popup->SetWeight(InTargetItem->GetWeight(), statInfo->GetStatValue(), currentWeight);
					popup->SetMode(false);
				}
			}

			if (false == popup->OnFinishCalculateDelegate.IsBoundToObject(this))
			{
				popup->OnFinishCalculateDelegate.AddUObject(this, &UGsUIDepotInven::ConfirmItemStackCount);
				popup->OnCancelCalculateDelegate.AddUObject(this, &UGsUIDepotInven::CancelCalculatorPopup);
				popup->OnResetCalculateDelegate.AddUObject(this, &UGsUIDepotInven::ResetSelection);
			}
		}
	}
}

void UGsUIDepotInven::ConfirmItemStackCount(int64 InCount)
{
	if (false == _curSelectIcon.IsValid())
		return;

	FGsItem* itemData = _curSelectIcon.Get()->GetItemIconData();
	if (nullptr == itemData)
		return;

	if (InCount == 0)
	{
		GSDepot()->RemoveDepotBasketItem(itemData->GetDBID());
	}
	else
	{
		// 계산기를 열 때 입력 가능했던 수량이 확인을 눌렀을 때 부족해질 수 있다. (자동 사용 등)
		ItemAmount depositAmount = (itemData->GetAmount() < InCount) ? itemData->GetAmount() : InCount;
		GSDepot()->AddOrUpdateDepotBasketItem(itemData->GetDBID(), depositAmount, FGsDepotManager::DepotActionState::State_Deposit);
	}
	
	CloseCalculatorUI();
}

void UGsUIDepotInven::CancelCalculatorPopup()
{
	CloseCalculatorUI();
}

void UGsUIDepotInven::ResetSelection()
{
	if (_curSelectIcon.IsValid())
	{
		_iconSlotHelper->RefreshByIndex(_curSelectIcon.Get()->GetSlotIntParamData());
	}

	_curSelectIcon.Reset();
}

void UGsUIDepotInven::CloseCalculatorUI()
{
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->CloseByKeyName(TEXT("PopupCalculator"));
	}

	ResetSelection();
}

bool UGsUIDepotInven::IsOpenedCalculatorUI()
{
	if (UGsUIManager* uiManager = GUI())
	{
		return uiManager->IsActiveWidget(TEXT("PopupCalculator"));
	}
	return false;
}

void UGsUIDepotInven::InitCalculatorUI(const FGsItem* InTargetItem)
{
	if (UGsUIManager* uiManager = GUI())
	{
		if (UGsUIPopupCalculator* popup = Cast<UGsUIPopupCalculator>(uiManager->GetWidgetByKey(TEXT("PopupCalculator"))))
		{
			popup->SetResetCount();
			popup->SetMaximumCount(InTargetItem->GetAmount());
			popup->SetMinimumCount(0);
			popup->SetDefaultCount(InTargetItem->GetAmount());
			popup->InvalidateNumText();
			popup->SetTouchState(false);

			FText findText;
			FText::FindText(TEXT("ContextUIText"), TEXT("calculator_keep"), findText);
			popup->SetTitleText(findText);

			if (UGsGameObjectLocalPlayer* player = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer)))
			{
				if (const FGsStatInfo* statInfo = player->GetCreatureStatInfoByType(StatType::MAX_WEIGHT))
				{
					int32 currentWeight = GItem()->GetOwnedItemWeight() - GSDepot()->GetBasketTotalWeight() + GSDepot()->GetItemTotalWeightInBasket(InTargetItem->GetDBID());

					popup->SetWeight(InTargetItem->GetWeight(), statInfo->GetStatValue(), currentWeight);
					popup->SetMode(false);
				}
			}

			if (false == popup->OnFinishCalculateDelegate.IsBoundToObject(this))
			{
				if (popup->OnResetCalculateDelegate.IsBound())
					popup->OnResetCalculateDelegate.Broadcast();
				popup->OnResetCalculateDelegate.Clear();
				popup->OnResetCalculateDelegate.AddUObject(this, &UGsUIDepotInven::ResetSelection);

				popup->OnFinishCalculateDelegate.Clear();
				popup->OnFinishCalculateDelegate.AddUObject(this, &UGsUIDepotInven::ConfirmItemStackCount);

				popup->OnCancelCalculateDelegate.Clear();
				popup->OnCancelCalculateDelegate.AddUObject(this, &UGsUIDepotInven::CancelCalculatorPopup);
			}
		}
	}
}

void UGsUIDepotInven::CloseItemDetailUI()
{
	CloseCalculatorUI();

	FGsItemDetailCloseMessageParam param;
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, &param);
}

void UGsUIDepotInven::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	CloseCalculatorUI();

	FGsItemDetailOpenMessageParamEX::DetailOption option;
	option._bHideBG = true;
	option._bUseCollectionMarking = true;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), FGsItemDetailOpenMessageParamEX::PosDir::CenterPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
}

void UGsUIDepotInven::AddItemList(struct FGsItemMessageParamBase& InParam)
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	FGsAddItemList* paramData = static_cast<FGsAddItemList*>(&InParam);
	if (nullptr == paramData)
		return;

	const TArray<ItemDBId>& itemDBIdList = paramData->_itemDbidList;
	for (ItemDBId itemDBId : itemDBIdList)
	{
		if (_displayItemList.Contains(itemDBId))
		{
#if WITH_EDITOR
			GSLOG(Warning, TEXT("[UGsUIDepotInven::AddItemList] InItem is Already Exist Item !!! "));
#endif
		}
		else
		{
			TWeakPtr<FGsItem> itemData = itemManager->FindItem(itemDBId);
			if (false == itemData.IsValid())
				continue;

			if (false == IsMatchedCurTab(itemData.Pin()->GetType()))
				continue;

			_displayItemList.Emplace(itemDBId);
			if (nullptr != _iconSlotHelper->GetIcon(_displayItemList.Num() - 1))
			{
				_iconSlotHelper->RefreshByIndex(_displayItemList.Num() - 1);
			}
		}
	}

	InvalidateSlotCount();
	InvalidateEmptyItemList();
}

void UGsUIDepotInven::UpdateItemList(struct FGsItemMessageParamBase& InParam)
{
	FGsUpdateItemList* paramData = static_cast<FGsUpdateItemList*>(&InParam);
	if (nullptr == paramData)
		return;

	for (ItemDBId itemDBId : paramData->_itemDbidList)
	{
		int32 index = _displayItemList.Find(itemDBId);
		if (INDEX_NONE == index)
		{
			return;
		}
		_iconSlotHelper->RefreshByIndex(index);
	}

	if (FGsDepotManager* depotManager = GSDepot())
	{
		depotManager->UpdateDepotBasketItemList(paramData->_itemDbidList);
	}
}

void UGsUIDepotInven::RemoveItemList(struct FGsItemMessageParamBase& InParam)
{
	FGsDepotManager* depotManager = GSDepot();
	if (nullptr == depotManager)
		return;

	FGsRemoveItemList* paramData = static_cast<FGsRemoveItemList*>(&InParam);
	if (nullptr == paramData)
		return;

	const TArray<ItemDBId>& itemDBIdList = paramData->_itemDBIdList;
	for (ItemDBId itemDBId : itemDBIdList)
	{
		if (_displayItemList.Contains(itemDBId))
		{
			_displayItemList.Remove(itemDBId);
		}

		if (depotManager->IsExistItemInBasketByDBID(itemDBId))
		{
			depotManager->RemoveDepotBasketItem(itemDBId);
		}
	}

	_iconSlotHelper->RefreshAll();
	InvalidateSlotCount();
	InvalidateEmptyItemList();
}

EInventoryTab UGsUIDepotInven::ConvertInvenTabType(const int32 InTabIndex)
{
	switch (InTabIndex)
	{
	case 0:
		return EInventoryTab::AllTab;
	case 1:
		return EInventoryTab::EquipmentTab;
	case 2:
		return EInventoryTab::ConsumableTab;
	case 3:
		return EInventoryTab::MaterialTab;
	}

	return EInventoryTab::AllTab;
}

bool UGsUIDepotInven::IsMatchedCurTab(const ItemType InType)
{
	EInventoryTab tabType = ConvertInvenTabType(_toggleGroup.GetSelectedIndex());
	switch (tabType)
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