// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/ItemEnchant/GsUIPopupItemEnchantMain.h"
#include "UI/UIContent/Common/Inventory/GsUIInventoryCommon.h"
#include "UI/UIContent/Common/Inventory/GsInventoryLogicEnchantInven.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemIngredient.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemHelper.h"
#include "Message/MessageParam/GsEnchantMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsItemEnchantManager.h"
#include "Management/ScopeGame/GsInventoryManager.h"
#include "Management/ScopeGame/GsStatChangeNotifyManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "EventProgress/GsEventProgressDefine.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "GsUIItemEnchantGroup.h"
#include "GsUIItemMultiEnchantGroup.h"
#include "GsUIItemRefineGroup.h"
#include "UI/UIContent/Popup/ItemEnchant/GsUIItemMagicalForgeGroup.h"

#include "T1Project.h"

#include "DataSchema/Item/GsSchemaItemEquipConfig.h"

#include "Data/GsDataContainManager.h"



void UGsUIPopupItemEnchantMain::BeginDestroy()
{
	SAFE_DELETE(_invenLogic);
	_toggleGroupTab.Clear();

	Super::BeginDestroy();
}

void UGsUIPopupItemEnchantMain::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_toggleGroupTab.AddToggleWidget(_enchantSwitcherBtn);
	_toggleGroupTab.AddToggleWidget(_multiEnchantSwitcherBtn);
	_toggleGroupTab.AddToggleWidget(_refineSwitcherBtn);

	if (IsBranch(EGsGameClientBranchType::MAGICAL_FORGE))
	{
		_magicalForgeTabDeco->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_magicalForgeSwitcherBtn->SetVisibility(ESlateVisibility::Visible);
		_toggleGroupTab.AddToggleWidget(_magicalForgeSwitcherBtn);
	}
	else
	{
		_magicalForgeTabDeco->SetVisibility(ESlateVisibility::Collapsed);
		_magicalForgeSwitcherBtn->SetVisibility(ESlateVisibility::Collapsed);
	}

	_toggleGroupTab.SetSelectedIndex(0);
	_toggleGroupTab.OnSelectChanged.BindUObject(this, &UGsUIPopupItemEnchantMain::OnSelectedTab);
	_toggleGroupTab.OnChangeBlocked.BindUObject(this, &UGsUIPopupItemEnchantMain::OnClickBlockedTab);


	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupItemEnchantMain::OnClickClose);

	_invenLogic = new FGsInventoryLogicEnchantInven();
}

void UGsUIPopupItemEnchantMain::NativeConstruct()
{
	Super::NativeConstruct();

	/* 탭 별 언락컨텐츠타입 연결 */
	_unlockTypeMap.Empty();
	_unlockTypeMap.Emplace(EGsItemEnchantActionType::ENCHANT_SINGLE, EGsUnlockContentsType::ITEM_ENCHANT);
	_unlockTypeMap.Emplace(EGsItemEnchantActionType::ENCHANT_MULTI, EGsUnlockContentsType::ITEM_ENCHANT);
	_unlockTypeMap.Emplace(EGsItemEnchantActionType::REFINE_OPTION, EGsUnlockContentsType::ITEM_REFINE);
	_unlockTypeMap.Emplace(EGsItemEnchantActionType::MAGICAL_FORGE, EGsUnlockContentsType::ITEM_MAGICAL_FORGE);

	CheckUnlock();

	BindMessages();

	if (nullptr == _invenLogic)
	{
		_invenLogic = new FGsInventoryLogicEnchantInven();
	}

	_toggleGroupTab.SetSelectedIndex(0);
	_invenUI->SetData(_invenLogic);
	_invenUI->SetMultiEnchantInvenState(false);
	_invenLogic->SetCurrTabType(EInventoryTab::ConsumableTab);
	_invenUI->SetEnchantInvenState();
	_invenUI->GetToggleGroupTab().OnSelectChanged.Unbind();
	if (false == _invenUI->GetToggleGroupTab().OnSelectChanged.IsBoundToObject(this))
	{
		_invenUI->GetToggleGroupTab().OnSelectChanged.BindUObject(this, &UGsUIPopupItemEnchantMain::OnSelectInvenTab);
	}
	if (false == _invenUI->OnClickItem().IsBoundToObject(this))
	{
		_invenUI->OnClickItem().BindUObject(this, &UGsUIPopupItemEnchantMain::OnClickInventoryItem);
	}
	if (false == _invenUI->OnClickMultiEnchantReset().IsBoundToObject(this))
	{
		_invenUI->OnClickMultiEnchantReset().BindUObject(this, &UGsUIPopupItemEnchantMain::OnClickMultiEnchantResetBtn);
	}
	if (false == _invenUI->OnClickMultiEnchantRegister().IsBoundToObject(this))
	{
		_invenUI->OnClickMultiEnchantRegister().BindUObject(this, &UGsUIPopupItemEnchantMain::OnClickMultiEnchantRegisterBtn);
	}
	if (false == _singleEnchantUI->OnClickMainIconSlot.IsBoundToObject(this))
	{
		_singleEnchantUI->OnClickMainIconSlot.AddUObject(this, &UGsUIPopupItemEnchantMain::OnClickEnchantMainSlot);
	}
	if (false == _refineOptionUI->OnClickRefineMainIconSlot.IsBoundToObject(this))
	{
		_refineOptionUI->OnClickRefineMainIconSlot.AddUObject(this, &UGsUIPopupItemEnchantMain::OnClickEnchantMainSlot);
	}
	if (false == _magicalForgeUI->OnClickMagicalForgeIconSlot.IsBoundToObject(this))
	{
		_magicalForgeUI->OnClickMagicalForgeIconSlot.AddUObject(this, &UGsUIPopupItemEnchantMain::OnClickEnchantMainSlot);
	}

	_enchantActionType = EGsItemEnchantActionType::ACTION_MAX;

	if (FGsStatChangeNotifyManager* statChangeNotifyMgr = GSStatChangeNotify())
	{
		statChangeNotifyMgr->Save(StatUpdateReason::ITEM_ENCHANT);
	}
}

void UGsUIPopupItemEnchantMain::NativeDestruct()
{
	_unlockTypeMap.Empty();

	if (FGsStatChangeNotifyManager* statChangeNotifyMgr = GSStatChangeNotify())
	{
		statChangeNotifyMgr->CheckNotify(StatUpdateReason::ITEM_ENCHANT);
	}

	if (FGsMessageHolder* msg = GMessage())
	{
		msg->GetItemEnchant().SendMessage(MessageContentEnchant::CLOSE_ALL_RESULT_WIDGET, nullptr);
	}

	SetTutorial(INVALID_ITEM_DB_ID);

	FGsItemEnchantManager* mgr = GSItemEnchant();
	if (mgr)
	{
		mgr->AllStopSound();
	}

	RemoveMessage();
	_invenLogic->SetSelectedItemDBId(0);

	FGsEventProgressManager* eventMgr = GSEventProgress();
	if (eventMgr)
	{
		// 이벤트 진행 순서에 전송
		eventMgr->OnReqFinishEvent(EGsEventProgressType::UI_POPUP_ITEM_ENCHANT);
	}

#ifdef RESTORE_INVENTORY
	if (UGsInventoryManager* inventoryMgr = GSInventory())
	{
		// 내부에서 CheckIsHUDState 검사 함
		inventoryMgr->RestoreInventoryAndDetailPopup();
	}
#endif

	Super::NativeDestruct();
}

void UGsUIPopupItemEnchantMain::BindMessages()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		_listItemDelegate.Emplace(msg->GetItem().AddUObject(MessageItem::ADDITEMLIST, this, &UGsUIPopupItemEnchantMain::OnAddItemList));
		_listItemDelegate.Emplace(msg->GetItem().AddUObject(MessageItem::UPDATEITEM, this, &UGsUIPopupItemEnchantMain::OnUpdateItem));
		_listItemDelegate.Emplace(msg->GetItem().AddUObject(MessageItem::REMOVEITEM, this, &UGsUIPopupItemEnchantMain::OnRemoveItem));

		_listItemEnchantDelegate.Emplace(msg->GetItemEnchant().AddUObject(MessageContentEnchant::INVALIDATE, this, &UGsUIPopupItemEnchantMain::UpdateSingleItemEnchantInfo));
		_listItemEnchantDelegate.Emplace(msg->GetItemEnchant().AddUObject(MessageContentEnchant::INVALIDATE_MULTI, this, &UGsUIPopupItemEnchantMain::UpdateMultiItemEnchantInfo));
		_listItemEnchantDelegate.Emplace(msg->GetItemEnchant().AddUObject(MessageContentEnchant::INVALIDATE_REFINE, this, &UGsUIPopupItemEnchantMain::UpdateRefineOptionEnchantInfo));
		_listItemEnchantDelegate.Emplace(msg->GetItemEnchant().AddUObject(MessageContentEnchant::INVALIDATE_MAGICALFORGE, this, &UGsUIPopupItemEnchantMain::UpdateMagicalForgeEnchantInfo));

		_handlerUserInfo = msg->GetUserBasicInfo().AddUObject(MessageUserInfo::CURRENCY, this, &UGsUIPopupItemEnchantMain::OnUpdateCurrency);

		_listUnlockDelegate.Emplace(msg->GetUnlock().AddUObject(MessageContentUnlock::NOTIFY_CONTENTS_UNLOCK,
			this, &UGsUIPopupItemEnchantMain::OnContentsUnlock));
		_listUnlockDelegate.Emplace(msg->GetUnlock().AddUObject(MessageContentUnlock::INVALIDATE_ALL_UNLOCK,
			this, &UGsUIPopupItemEnchantMain::OnContentsUnlock));
	}
}

void UGsUIPopupItemEnchantMain::RemoveMessage()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		for (TPair<MessageItem, FDelegateHandle>& handle : _listItemDelegate)
		{
			msg->GetItem().Remove(handle);
		}
		_listItemDelegate.Empty();

		for (TPair<MessageContentEnchant, FDelegateHandle>& handle : _listItemEnchantDelegate)
		{
			msg->GetItemEnchant().Remove(handle);
		}
		_listItemEnchantDelegate.Empty();

		msg->GetUserBasicInfo().Remove(_handlerUserInfo);

		for (TPair<MessageContentUnlock, FDelegateHandle>& handle : _listUnlockDelegate)
		{
			msg->GetUnlock().Remove(handle);
		}
		_listUnlockDelegate.Empty();
	}
}

void UGsUIPopupItemEnchantMain::OnAddItemList(struct FGsItemMessageParamBase& InParam)
{
	InvalidateInvenDimmedState();
}

void UGsUIPopupItemEnchantMain::OnUpdateItem(struct FGsItemMessageParamBase& InParam)
{
	FGsUpdateItem* paramData = static_cast<FGsUpdateItem*>(&InParam);
	if (paramData)
	{
		UpdateEnchantInfo(paramData->_itemDBID, _enchantActionType);
	}
}

void UGsUIPopupItemEnchantMain::OnRemoveItem(struct FGsItemMessageParamBase& InParam)
{
	FGsRemoveItem* paramData = static_cast<FGsRemoveItem*>(&InParam);
	if (paramData)
	{
		switch (_enchantActionType)
		{
		case EGsItemEnchantActionType::ENCHANT_SINGLE:
			_singleEnchantUI->RemoveItemInfo(paramData->_itemDBID);
			break;
		case EGsItemEnchantActionType::ENCHANT_MULTI:
			_multiEnchantUI->RemoveItemInfo(paramData->_itemDBID);
			break;
		case EGsItemEnchantActionType::REFINE_OPTION:
			_refineOptionUI->RemoveItemInfo(paramData->_itemDBID);
			break;
		case EGsItemEnchantActionType::MAGICAL_FORGE:
			_magicalForgeUI->RemoveItemInfo(paramData->_itemDBID);
			break;
		}
	}
}

void UGsUIPopupItemEnchantMain::UpdateSingleItemEnchantInfo(const struct IGsMessageParam* InMessageParam)
{
	if (EGsItemEnchantActionType::ENCHANT_SINGLE != _enchantActionType)
		return;

	if (nullptr == InMessageParam)
		return;

	const FGsUpdateEnchantItem* paramData = InMessageParam->Cast<const FGsUpdateEnchantItem>();
	if (nullptr == paramData)
		return;

	_singleEnchantUI->UpdateEnchantResultInfo(paramData->_equipItemDBID, paramData->_scollItemDBID, paramData->_enchantAckResultType);
}

void UGsUIPopupItemEnchantMain::UpdateMultiItemEnchantInfo(const struct IGsMessageParam* InMessageParam)
{
	if (EGsItemEnchantActionType::ENCHANT_MULTI != _enchantActionType)
		return;

	if (nullptr == InMessageParam)
		return;

	const FGsUpdateMultiEnchantItem* paramData = InMessageParam->Cast<const FGsUpdateMultiEnchantItem>();
	if (nullptr == paramData)
		return;

	_multiEnchantUI->UpdateMultiEnchantResultInfo(paramData->_enchantReusltDataList, paramData->_useIngredientItemDbidList);
}

void UGsUIPopupItemEnchantMain::UpdateRefineOptionEnchantInfo(const struct IGsMessageParam* InMessageParam)
{
	if (EGsItemEnchantActionType::REFINE_OPTION != _enchantActionType)
		return;

	_refineOptionUI->UpdateRefineOptionResultInfo();
}

void UGsUIPopupItemEnchantMain::UpdateMagicalForgeEnchantInfo(const struct IGsMessageParam* InMessageParam)
{
	if (EGsItemEnchantActionType::MAGICAL_FORGE != _enchantActionType)
		return;

	const FGsUpdateMagicalForgeItem* paramData = InMessageParam->Cast<const FGsUpdateMagicalForgeItem>();
	if (nullptr == paramData)
		return;

	_magicalForgeUI->UpdateMagicalForgeResultInfo(paramData->_targetItemDBId, paramData->_resultType);
}

void UGsUIPopupItemEnchantMain::UpdateEnchantInfo(const ItemDBId InTargetItemDbid, const EGsItemEnchantActionType InEnchantActionType)
{
	switch (InEnchantActionType)
	{
	case EGsItemEnchantActionType::ENCHANT_SINGLE:
	{
		if (_singleEnchantUI->UpdateItemInfo(InTargetItemDbid))
		{
			InvalidateInvenSelection();
			InvalidateInvenDimmedState();
		}
	}
	break;
	case EGsItemEnchantActionType::ENCHANT_MULTI:
	{
		if(_multiEnchantUI->IsUpdateInfo(InTargetItemDbid))
		{
			_multiEnchantUI->UpdateMultiEnchantInfo(InTargetItemDbid);

			InvalidateInvenSelection();
			InvalidateInvenDimmedState();
		}
	}
	break;
	case EGsItemEnchantActionType::REFINE_OPTION:
	{
		if (_refineOptionUI->UpdateItemInfo(InTargetItemDbid))
		{
			InvalidateInvenSelection();
			InvalidateInvenDimmedState();
		}
	}
	break;
	case EGsItemEnchantActionType::MAGICAL_FORGE:
	{
		if (_magicalForgeUI->UpdateItemInfo(InTargetItemDbid))
		{
			InvalidateInvenSelection();
			InvalidateInvenDimmedState();
		}
	}
	break;
	}
}

// 초기 open 진입 및 탭 변경시에만 호출 (초기 세팅용)
void UGsUIPopupItemEnchantMain::SetEnchantInfo(const ItemDBId InTargetItemDbid, const EGsItemEnchantActionType InEnchantActionType, const bool InIsEquipment)
{
	_enchantActionType = InEnchantActionType;
	EGsItemEnchantActionType type = (EGsItemEnchantActionType)_toggleGroupTab.GetSelectedIndex();
	if (InEnchantActionType != type)
	{
		_enchantActionType = InEnchantActionType;
		_toggleGroupTab.SetSelectedIndex((uint8)InEnchantActionType);
	}

	switch (InEnchantActionType)
	{
	case EGsItemEnchantActionType::ENCHANT_SINGLE:
	{
		_enchantTypeSwitcher->SetActiveWidgetIndex(0);
		_singleEnchantUI->SetEnchantInfo(InTargetItemDbid, EGsItemEnchantActionType::ENCHANT_SINGLE);
		_invenLogic->SetIngredientEffectType(ItemEffectType::ITEM_ENCHANT);
		_invenUI->SetInvenTab(InIsEquipment ? 3 : 1);
		_invenLogic->SetCurrTabType(InIsEquipment ? EInventoryTab::MaterialTab : EInventoryTab::EquipmentTab);
		_invenUI->SetMultiEnchantInvenState(false);
	}
	break;
	case EGsItemEnchantActionType::ENCHANT_MULTI:
	{
		_enchantTypeSwitcher->SetActiveWidgetIndex(1);
		_invenLogic->SetIngredientEffectType(ItemEffectType::ITEM_ENCHANT);
		_invenUI->SetMultiEnchantInvenState(true);
		_invenUI->SetInvenTab(InIsEquipment ? 3 : 1);
		_invenLogic->SetCurrTabType(InIsEquipment ? EInventoryTab::MaterialTab : EInventoryTab::EquipmentTab);
		_multiEnchantUI->SetMultiEnchantInfo(InTargetItemDbid, 0);
		if (false == _multiEnchantUI->OnEquipItemSlot.IsBoundToObject(this))
		{
			_multiEnchantUI->OnEquipItemSlot.AddUObject(this, &UGsUIPopupItemEnchantMain::OnClickMultiEnchantEquipSlot);
		}
	}
	break;
	case EGsItemEnchantActionType::REFINE_OPTION:
	{
		_enchantTypeSwitcher->SetActiveWidgetIndex(2);
		_refineOptionUI->SetRefineInfo(InTargetItemDbid);
		_invenLogic->SetIngredientEffectType(ItemEffectType::OPTION_STONE);
		_invenUI->SetInvenTab(InIsEquipment ? 3 : 1);
		_invenLogic->SetCurrTabType(InIsEquipment ? EInventoryTab::MaterialTab : EInventoryTab::EquipmentTab);
		_invenUI->SetMultiEnchantInvenState(false);
	}
	break;
	case EGsItemEnchantActionType::MAGICAL_FORGE:
	{
		_enchantTypeSwitcher->SetActiveWidgetIndex(3);
		ItemDBId equipItemDBId = InIsEquipment ? InTargetItemDbid : 0;
		ItemDBId ingredientItemDBId = InIsEquipment ? 0 : InTargetItemDbid;
		_magicalForgeUI->SetItemDBId(equipItemDBId, ingredientItemDBId);
		_invenLogic->SetIngredientEffectType(ItemEffectType::MAGICAL_FORGE);
		_invenUI->SetInvenTab(InIsEquipment ? 3 : 1);
		_invenLogic->SetCurrTabType(InIsEquipment ? EInventoryTab::MaterialTab : EInventoryTab::EquipmentTab);
		_invenUI->SetMultiEnchantInvenState(false);
	}
	break;
	}

	InvalidateInvenDimmedState();
	InvalidateInvenSelection();
}

void UGsUIPopupItemEnchantMain::OnSelectedTab(int32 inIndex)
{
	if (inIndex >= (uint8)EGsItemEnchantActionType::ACTION_MAX)
	{
		return;
	}
	if ((EGsItemEnchantActionType)inIndex == _enchantActionType)
	{
		return;
	}

	FGsItemEnchantManager* enchantManager = GSItemEnchant();
	if (nullptr == enchantManager)
		return;

	EGsItemEnchantActionType preActionType = _enchantActionType;
	EGsItemEnchantActionType nextActionType = (EGsItemEnchantActionType)inIndex;

	// 이전 탭에 선택되어있던 아이템 정보
	ItemDBId targetItemDBId = INVALID_ITEM_DB_ID;
	ItemDBId ingredientItemDBId = INVALID_ITEM_DB_ID;

	switch (preActionType)
	{
	case EGsItemEnchantActionType::ENCHANT_SINGLE:
	{
		 targetItemDBId = _singleEnchantUI->GetTargetItemDbid();
		 if (EGsItemEnchantActionType::ENCHANT_MULTI == nextActionType)
		 {
			 ingredientItemDBId = _singleEnchantUI->GetIngrdientItemDbid();
		 }
		 break;
	}
	case EGsItemEnchantActionType::ENCHANT_MULTI:
	{
		TArray<ItemDBId> targetItemDBIdList = _multiEnchantUI->GetTargetItemDbidList();
		if (targetItemDBIdList.Num() > 0)
		{
			targetItemDBId = targetItemDBIdList[0];
		}
		if (EGsItemEnchantActionType::ENCHANT_SINGLE == nextActionType)
		{
			TArray<ItemDBId> ingredientItemDBIdList = _multiEnchantUI->GetIngredientItemDbidList();
			if (ingredientItemDBIdList.Num() > 0)
			{
				ingredientItemDBId = ingredientItemDBIdList[0];
			}
		}
		break;
	}
	case EGsItemEnchantActionType::REFINE_OPTION:
	{
		targetItemDBId = _refineOptionUI->GetTargetItemDbid();
		break;
	}
	case EGsItemEnchantActionType::MAGICAL_FORGE:
	{
		targetItemDBId = _magicalForgeUI->GetTargetItemDBId();
		break;
	}
	}

	// 탭이 바뀌기 전에 이전 탭 초기화 처리
	ResetContentsInfo();

	_enchantTypeSwitcher->SetActiveWidgetIndex(inIndex);

	bool isValidTarget = FGsItem::IsValidItemByDBID(targetItemDBId, ItemType::EQUIP);
	EInventoryTab openInventoryTab = isValidTarget ? EInventoryTab::MaterialTab : EInventoryTab::EquipmentTab;

	_invenUI->SetInvenTab(static_cast<uint8>(openInventoryTab));
	_invenUI->SetMultiEnchantInvenState(nextActionType == EGsItemEnchantActionType::ENCHANT_MULTI);
	_invenLogic->SetCurrTabType(openInventoryTab);
	_invenLogic->SetIngredientEffectType(GetIngredientEffectType(nextActionType));

	_enchantActionType = nextActionType;
	switch (_enchantActionType)
	{
	case EGsItemEnchantActionType::ENCHANT_SINGLE:
	{
		_singleEnchantUI->SetInputItemDbid(targetItemDBId, ingredientItemDBId);
	}
	break;
	case EGsItemEnchantActionType::ENCHANT_MULTI:
	{
		if (enchantManager->IsNotAllowedMultiEnchantItem(targetItemDBId))
		{
			targetItemDBId = INVALID_ITEM_DB_ID;
		}
		_multiEnchantUI->SetMultiEnchantInfo(targetItemDBId, ingredientItemDBId);

		if (false == _multiEnchantUI->OnEquipItemSlot.IsBoundToObject(this))
		{
			_multiEnchantUI->OnEquipItemSlot.AddUObject(this, &UGsUIPopupItemEnchantMain::OnClickMultiEnchantEquipSlot);
		}
	}
	break;
	case EGsItemEnchantActionType::REFINE_OPTION:
	{
		_refineOptionUI->SetRefineInfo(targetItemDBId);
	}
	break;
	case EGsItemEnchantActionType::MAGICAL_FORGE:
	{
		_magicalForgeUI->SetItemDBId(targetItemDBId, 0);
	}
	break;
	}

	if (preActionType != nextActionType)
	{
		_invenUI->InvalidateData();
	}
	InvalidateInvenDimmedState();
	InvalidateInvenSelection();
	enchantManager->AllStopSound();
}

void UGsUIPopupItemEnchantMain::OnClickBlockedTab(int32 InIndex)
{
	FGsUnlockManager* unlockManager = GSUnlock();
	if (nullptr == unlockManager)
		return;

	EGsItemEnchantActionType clickedTab = StaticCast<EGsItemEnchantActionType>(InIndex);
	if (_unlockTypeMap.Contains(clickedTab))
	{
		unlockManager->ShowLockTicker(_unlockTypeMap[clickedTab]);
	}
}

void UGsUIPopupItemEnchantMain::InvalidateInvenDimmedState()
{
	// 아이콘 딤드상태 일괄 처리
	// 단일강화/재련 동일 조건 처리
	// 멀티강화 개별 조건 처리
	FGsItemEnchantManager* enchantMgr = GSItemEnchant();
	EInventoryTab curTabType = _invenLogic->GetCurrTabType();
	TArray<FGsItem*> invenItemList = _invenLogic->GetDataList();
	switch (_enchantActionType)
	{
	case EGsItemEnchantActionType::ENCHANT_SINGLE:
	{
		ItemDBId targetItemDbid = EInventoryTab::EquipmentTab == curTabType ? _singleEnchantUI->GetIngrdientItemDbid() : _singleEnchantUI->GetTargetItemDbid();
		for (FGsItem* iter : invenItemList)
		{
			bool state = true;
			if (0 < targetItemDbid)
			{
				state = enchantMgr->IsMatchedEnchantItem(targetItemDbid, iter->GetDBID(), false);
			}
			_invenUI->SetInvenSlotDimmed(!state, iter->GetDBID());
		}
	}
	break;
	case EGsItemEnchantActionType::REFINE_OPTION:
	{
		ItemDBId targetItemDbid = EInventoryTab::EquipmentTab == curTabType ? _refineOptionUI->GetIngrdientItemDbid() : _refineOptionUI->GetTargetItemDbid();
		for (FGsItem* iter : invenItemList)
		{
			bool state = true;
			if (0 < targetItemDbid)
			{
				state = enchantMgr->IsMatchedEnchantItem(targetItemDbid, iter->GetDBID(), false);
			}
			_invenUI->SetInvenSlotDimmed(!state, iter->GetDBID());
		}
	}
	break;
	case EGsItemEnchantActionType::ENCHANT_MULTI:
	{
		TArray<ItemDBId> equipItemDbidList = _multiEnchantUI->GetTargetItemDbidList();
		TArray<ItemDBId> ingredientItemDbidList = _multiEnchantUI->GetIngredientItemDbidList();
		ItemDBId equipItemDbid = equipItemDbidList.IsValidIndex(0) ? equipItemDbidList[0] : 0;
		ItemDBId ingredientItemDbid = ingredientItemDbidList.IsValidIndex(0) ? ingredientItemDbidList[0] : 0;
		for (FGsItem* iter : invenItemList)
		{
			 bool state = enchantMgr->IsNotAllowedMultiEnchantItem(iter->GetDBID()) ? false : enchantMgr->IsMatchedEnchantItem(curTabType, equipItemDbid, ingredientItemDbid, iter->GetDBID());
			_invenUI->SetInvenSlotDimmed(!state, iter->GetDBID());
		}
	}
	break;
	case EGsItemEnchantActionType::MAGICAL_FORGE:
	{
		const FGsSchemaItemEquipConfig* itemEquipConfig = UGsTableUtil::FindFirstRow<FGsSchemaItemEquipConfig>();
		if (nullptr == itemEquipConfig)
			return;

		uint8 minMagicalForgeGrade = static_cast<uint8>(itemEquipConfig->magicalForgeGrade);
		for (FGsItem* iter : invenItemList)
		{
			bool state = true;
			if (EInventoryTab::EquipmentTab == curTabType)
			{
				// ItemEquipConfig 에 등록된 등급 이상인가?
				state = static_cast<uint8>(iter->GetGradeType()) >= minMagicalForgeGrade;

				if (0 < _magicalForgeUI->GetIngredientItemDBId())
				{
					state = state && FGsItemHelper::IsMatchedMagicalForge(iter->GetDBID(), _magicalForgeUI->GetIngredientItemDBId());
				}
			}
			else if (EInventoryTab::MaterialTab == curTabType && 0 < _magicalForgeUI->GetTargetItemDBId())
			{
				state = FGsItemHelper::IsMatchedMagicalForge(_magicalForgeUI->GetTargetItemDBId(), iter->GetDBID());
			}
			_invenUI->SetInvenSlotDimmed(!state, iter->GetDBID());
		}
	}
	break;
	}
}

void UGsUIPopupItemEnchantMain::InvalidateInvenSelection()
{
	TArray<ItemDBId> selectionList = GetSelectedItemDbidList(_enchantActionType);
	TArray<FGsItem*> invenItemList = _invenLogic->GetDataList();
	for (FGsItem* iter : invenItemList)
	{
		ItemDBId dbid = iter->GetDBID();
		ItemDBId* findData = selectionList.FindByPredicate([&dbid](ItemDBId InDbid)
			{
				return InDbid == dbid;
			});
		_invenUI->SetInvenSlotSelectionImg(nullptr != findData, iter->GetDBID());
	}
	_invenLogic->SetSeletedItemDbidList(selectionList);
}

void UGsUIPopupItemEnchantMain::ResetContentsInfo()
{
	switch (_enchantActionType) // 바뀌기 전 탭
	{
	case EGsItemEnchantActionType::ENCHANT_SINGLE:
	{
		_singleEnchantUI->CloseEnchant();
	}
	break;
	case EGsItemEnchantActionType::ENCHANT_MULTI:
	{
		_multiEnchantUI->CloseMultiEnchant();
	}
	break;
	case EGsItemEnchantActionType::REFINE_OPTION:
	{
		_refineOptionUI->CloseRefineOption();
	}
	break;
	case EGsItemEnchantActionType::MAGICAL_FORGE:
	{
		_magicalForgeUI->ResetContents();
	}
	break;
	}

	//_invenLogic->SetSeletedItemDbidList(GetSelectedItemDbidList(_enchantActionType));
}

void UGsUIPopupItemEnchantMain::OnClickInventoryItem(uint64 InItemDBId)
{
	TWeakPtr<FGsItem> findItem = GItem()->FindItem(InItemDBId);
	if (false == findItem.IsValid())
		return;

	if (findItem.Pin()->GetLock())
	{
		InvalidateInvenDimmedState();
		InvalidateInvenSelection();
		return;
	}

	EInventoryTab tabType = _invenLogic->GetCurrTabType();
	switch (_enchantActionType)
	{
	case EGsItemEnchantActionType::ENCHANT_SINGLE:
	{
		if (_singleEnchantUI->IsPlayingAnim() || _singleEnchantUI->IsAutoEnchant())
		{
			_invenUI->SetInvenSlotSelectionImg(false, InItemDBId);
			return;
		}
		if (findItem.Pin()->GetDBID() == _singleEnchantUI->GetIngrdientItemDbid() || findItem.Pin()->GetDBID() == _singleEnchantUI->GetTargetItemDbid())
			return;

		ItemDBId compareInputItemDbid = EInventoryTab::EquipmentTab == tabType ? _singleEnchantUI->GetIngrdientItemDbid() : _singleEnchantUI->GetTargetItemDbid();
		if (0 < compareInputItemDbid)
		{
			if (false == GSItemEnchant()->IsMatchedEnchantItem(findItem.Pin()->GetDBID(), compareInputItemDbid,false))
			{
				// 기존에 들어가있던 아이템 reset 처리
				_singleEnchantUI->SetInputItemDbid(findItem.Pin()->IsEquipment() ? findItem.Pin()->GetDBID() : 0 , findItem.Pin()->IsIngredient() ? findItem.Pin()->GetDBID() : 0);
			}
			else
			{
				_singleEnchantUI->SetEnchantItemDbid(findItem.Pin()->GetDBID());
			}
		}
		else
		{
			_singleEnchantUI->SetEnchantItemDbid(findItem.Pin()->GetDBID());
		}
	}
	break;
	case EGsItemEnchantActionType::ENCHANT_MULTI:
	{
		if (_multiEnchantUI->IsAutoMultiEnchant())
		{
			_invenUI->SetInvenSlotSelectionImg(false, InItemDBId);
			return;
		}

		FGsItemEnchantManager* enchantMgr = GSItemEnchant();
		TArray<ItemDBId> equipItemDbidList = _multiEnchantUI->GetTargetItemDbidList();
		TArray<ItemDBId> ingredientItemDbidList = _multiEnchantUI->GetIngredientItemDbidList();
		ItemDBId equipItemDbid = equipItemDbidList.IsValidIndex(0) ? equipItemDbidList[0] : 0;
		ItemDBId ingredientItemDbid = ingredientItemDbidList.IsValidIndex(0) ? ingredientItemDbidList[0] : 0;
		if (enchantMgr->IsNotAllowedMultiEnchantItem(InItemDBId))
		{
			// 아이템 특성 상, 다중강화가 안되는 아이템에 대해서는 별도 티커 처리
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_CannotMultiEnchant"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
		}
		else if (false == enchantMgr->IsMatchedEnchantItem(tabType, equipItemDbid, ingredientItemDbid, InItemDBId))
		{
			FText findText;
			FText::FindText(TEXT("ItemEnchantText"), TEXT("UI_EnchantMulti_Ticker_ItemTypeDiff"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
		}
		else if (findItem.Pin()->IsEquipment() && _multiEnchantUI->GetEquipItemSlotMaxCount() <= equipItemDbidList.Num() 
			&& INDEX_NONE == _multiEnchantUI->GetTargetItemDbidList().Find(findItem.Pin()->GetDBID()))
		{
			FText findText;
			FText::FindText(TEXT("ItemEnchantText"), TEXT("UI_EnchantMulti_Ticker_SlotFull"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
		}
		else
		{
			_multiEnchantUI->OnClickInventory(findItem);
		}
	}
	break;
	case EGsItemEnchantActionType::REFINE_OPTION:
	{
		if (_refineOptionUI->IsPlayingAnim())
		{
			_invenUI->SetInvenSlotSelectionImg(false, InItemDBId);
			return;
		}
		if (findItem.Pin()->GetDBID() == _refineOptionUI->GetIngrdientItemDbid() || findItem.Pin()->GetDBID() == _refineOptionUI->GetTargetItemDbid())
			return;

		ItemDBId compareInputItemDbid = EInventoryTab::EquipmentTab == tabType ? _refineOptionUI->GetIngrdientItemDbid() : _refineOptionUI->GetTargetItemDbid();
		if (0 < compareInputItemDbid)
		{
			if (false == GSItemEnchant()->IsMatchedEnchantItem(findItem.Pin()->GetDBID(), compareInputItemDbid, false))
			{
				// 기존에 들어가있던 아이템 reset 처리
				_refineOptionUI->SetInputItemDbid(findItem.Pin()->IsEquipment() ? findItem.Pin()->GetDBID() : 0, findItem.Pin()->IsIngredient() ? findItem.Pin()->GetDBID() : 0);
			}
			else
			{
				_refineOptionUI->SetRefineInfo(findItem.Pin()->GetDBID());
			}
		}
		else
		{
			_refineOptionUI->SetRefineInfo(findItem.Pin()->GetDBID());
		}
	}
	break;
	case EGsItemEnchantActionType::MAGICAL_FORGE:
	{
		if (_magicalForgeUI->IsPlayingAnim())
		{
			_invenUI->SetInvenSlotSelectionImg(false, InItemDBId);
			return;
		}

		ItemDBId findItemDBId = findItem.Pin()->GetDBID();

		// 장비를 선택한 경우
		if (EInventoryTab::EquipmentTab == tabType)
		{
			_magicalForgeUI->SetTargetItemDBId(findItemDBId);
		}
		// 재료를 선택한 경우
		else
		{
			_magicalForgeUI->SetIngredientItemDBId(findItemDBId);
		}
		_magicalForgeUI->InvalidateAll();
	}
	break;
	}

	InvalidateInvenDimmedState();
	InvalidateInvenSelection();
}

void UGsUIPopupItemEnchantMain::OnClickMultiEnchantEquipSlot()
{
	InvalidateInvenDimmedState();
	InvalidateInvenSelection();
}

// true : mainTargetSlot / false : ingredientSlot
void UGsUIPopupItemEnchantMain::OnClickEnchantMainSlot(const bool InTarget, const ItemDBId InItemDbid)
{
	if (InTarget)
	{
		// 장착탭 출력
		_invenLogic->SetSeletedItemDbidList(GetSelectedItemDbidList(_enchantActionType));
		_invenUI->SetInvenTab(1);
		_invenLogic->SetCurrTabType(EInventoryTab::EquipmentTab);
	}
	else
	{
		// 재료탭 출력
		_invenLogic->SetSeletedItemDbidList(GetSelectedItemDbidList(_enchantActionType));
		_invenUI->SetInvenTab(3);
		_invenLogic->SetCurrTabType(EInventoryTab::MaterialTab);
	}

	InvalidateInvenDimmedState();
	InvalidateInvenSelection();
}

TArray<ItemDBId> UGsUIPopupItemEnchantMain::GetSelectedItemDbidList(const EGsItemEnchantActionType InCurType)
{
	TArray<ItemDBId> seltectedDbidList;
	switch (InCurType)
	{
	case EGsItemEnchantActionType::ENCHANT_SINGLE:
	{
		seltectedDbidList.Emplace(_singleEnchantUI->GetTargetItemDbid());
		seltectedDbidList.Emplace(_singleEnchantUI->GetIngrdientItemDbid());
	}
	break;
	case EGsItemEnchantActionType::ENCHANT_MULTI:
	{
		seltectedDbidList.Append(_multiEnchantUI->GetTargetItemDbidList());
		seltectedDbidList.Append(_multiEnchantUI->GetIngredientItemDbidList());
	}
	break;
	case EGsItemEnchantActionType::REFINE_OPTION:
	{
		seltectedDbidList.Emplace(_refineOptionUI->GetTargetItemDbid());
		seltectedDbidList.Emplace(_refineOptionUI->GetIngrdientItemDbid());
	}
	break;
	case EGsItemEnchantActionType::MAGICAL_FORGE:
	{
		seltectedDbidList.Emplace(_magicalForgeUI->GetTargetItemDBId());
		seltectedDbidList.Emplace(_magicalForgeUI->GetIngredientItemDBId());
	}
	break;
	}

	return seltectedDbidList;
}

ItemEffectType UGsUIPopupItemEnchantMain::GetIngredientEffectType(const EGsItemEnchantActionType InActionType)
{
	switch (InActionType)
	{
	case EGsItemEnchantActionType::ENCHANT_SINGLE:
		return ItemEffectType::ITEM_ENCHANT;
	case EGsItemEnchantActionType::ENCHANT_MULTI:
		return ItemEffectType::ITEM_ENCHANT;
	case EGsItemEnchantActionType::REFINE_OPTION:
		return ItemEffectType::OPTION_STONE;
	case EGsItemEnchantActionType::MAGICAL_FORGE:
		return ItemEffectType::MAGICAL_FORGE;
	}

	return ItemEffectType::ITEM_ENCHANT;
}

void UGsUIPopupItemEnchantMain::OnSelectInvenTab(int32 InIndex)
{
	if (_invenLogic)
	{
		EInventoryTab tabType = static_cast<EInventoryTab>(InIndex);
		if (tabType == _invenLogic->GetCurrTabType())
		{
			return;
		}

		_invenLogic->OnSelectTab(tabType);
	}

	_invenUI->InvalidateData();
	InvalidateInvenDimmedState();
	InvalidateInvenSelection();
}

void UGsUIPopupItemEnchantMain::OnClickMultiEnchantResetBtn()
{
	if (EGsItemEnchantActionType::ENCHANT_MULTI == _enchantActionType)
	{
		_multiEnchantUI->OnClickReset();
	}
}

void UGsUIPopupItemEnchantMain::OnClickMultiEnchantRegisterBtn()
{
	if (EGsItemEnchantActionType::ENCHANT_MULTI == _enchantActionType)
	{
		_multiEnchantUI->OnClickRegisterSameItem();
	}
}

void UGsUIPopupItemEnchantMain::OnUpdateCurrency(uint64 InData)
{
	CurrencyType currencyType = static_cast<CurrencyType>(InData);
	switch (_enchantActionType)
	{
	case EGsItemEnchantActionType::ENCHANT_SINGLE:
		_singleEnchantUI->InvalidateCurrencyInfo();
		break;
	case EGsItemEnchantActionType::ENCHANT_MULTI:
		_multiEnchantUI->InvalidateCurrencyInfo();
		break;
	case EGsItemEnchantActionType::REFINE_OPTION:
		_refineOptionUI->InvalidateCurrencyInfo();
		break;
	case EGsItemEnchantActionType::MAGICAL_FORGE:
		_magicalForgeUI->InvalidateCurrencyInfo();
		break;
	}
}

void UGsUIPopupItemEnchantMain::OnContentsUnlock(const IGsMessageParam* InParam)
{
	CheckUnlock();

	// 보고 있던 탭이 잠겼다면 닫는다.
	int32 currentTabIndex = StaticCast<int32>(_enchantActionType);
	if (_toggleGroupTab.IsBlockedIndex(currentTabIndex))
	{
		Close();
	}
}

void UGsUIPopupItemEnchantMain::CheckUnlock()
{
	FGsUnlockManager* unlockManager = GSUnlock();
	if (nullptr == unlockManager)
		return;

	for (TPair<EGsItemEnchantActionType, EGsUnlockContentsType> tabUnlockPair : _unlockTypeMap)
	{
		bool bIsUnlock = unlockManager->IsContentsUnlock(tabUnlockPair.Value);
		int32 tabIndex = static_cast<int32>(tabUnlockPair.Key);

		if (bIsUnlock)
		{
			// 컨텐츠를 사용할 수 있다.
			_toggleGroupTab.DeleteBlockedIndex(tabIndex);
		}
		else
		{
			// 컨텐츠를 사용할 수 없다.
			if (false == _toggleGroupTab.IsBlockedIndex(tabIndex))
			{
				_toggleGroupTab.AddBlockedIndex(tabIndex);
			}
		}
	}
}

// 상태이상일때 딤드 처리 요청하면 주석해지 처리
void UGsUIPopupItemEnchantMain::SetAllIconDimmed(const bool InState)
{
	/*
	TArray<FGsItem*> invenItemList = _invenLogic->GetDataList();
	for (FGsItem* iter : invenItemList)
	{
		_invenUI->SetInvenSlotDimmed(InState, iter->GetDBID());
	}
	*/
}

void UGsUIPopupItemEnchantMain::SetBG(const bool InUseBg)
{
	if (_bgImage)
	{
		_bgImage->SetVisibility(InUseBg ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UGsUIPopupItemEnchantMain::SetTutorial(uint64 InItemDBId)
{
	if (INVALID_ITEM_DB_ID == InItemDBId)
	{
		_invenUI->OnTutorialListEvent.Unbind();

		_invenUI->SetTutorial(false);
		_invenLogic->SetTutorial(INVALID_ITEM_DB_ID);
		return;
	}

	_invenUI->OnTutorialListEvent.BindUObject(this, &UGsUIPopupItemEnchantMain::OnTutorialSettingComplete);
	_invenUI->SetTutorial(true);
	_invenLogic->SetTutorial(InItemDBId);

	_invenUI->InvalidateData();
}

void UGsUIPopupItemEnchantMain::OnTutorialSettingComplete(UWidget* InTargetWidget)
{
	OnTutorialListEvent.ExecuteIfBound(InTargetWidget);
}

void UGsUIPopupItemEnchantMain::OnInputOk()
{

}

void UGsUIPopupItemEnchantMain::OnInputCancel()
{
	/*
	if (FGsItemEnchantManager* enchantMgr = GSItemEnchant())
	{
		if (enchantMgr->IsOpenEnchantWaitWidget())
		{
			return;
		}
	}
	*/
	Close();
}

void UGsUIPopupItemEnchantMain::OnClickClose()
{
	/*
	if (FGsItemEnchantManager* enchantMgr = GSItemEnchant())
	{
		if (enchantMgr->IsOpenEnchantWaitWidget())
		{
			return;
		}
	}
	*/
	Close();
}

/*
bool UGsUIPopupItemEnchantMain::OnBack()
{
	if (FGsItemEnchantManager* enchantMgr = GSItemEnchant())
	{
		if (enchantMgr->IsOpenEnchantWaitWidget())
		{
			return true;
		}
	}
	Close();
	return true;
}
*/