// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDQuickSlot.h"

#include "T1Project.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsQuickSlotManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Option/GsGameUserSettings.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsItemMessageParam.h"

#include "UI/UIContent/Common/ReuseIconSlot/GsUIReuseScrollHUD.h"


void UGsUIHUDQuickSlot::NativeDestruct()
{
	RemoveMsg();

	// Äü½½·Ô ÆäÀÌÁö ÀúÀå
	UGsGameUserSettings* userSetting = GGameUserSettings();
	if (nullptr == userSetting)
		return;

	UGsQuickSlotManager* slotMgr = GSQuickSlot();
	if (nullptr == slotMgr)
		return;

	userSetting->SetCombatSettingByType(EGsOptionCombat::QUICKSLOT_PAGE_INDEX, slotMgr->GetLastQuickSlotPage());

	Super::NativeDestruct();
}

void UGsUIHUDQuickSlot::NativeConstruct()
{
	Super::NativeConstruct();
	
	RegisterMsg();

	// Äü½½·Ô ÆäÀÌÁö ·Îµå
	UGsGameUserSettings* userSetting = GGameUserSettings();
	if (nullptr != userSetting)
	{
		uint8 slotPage = userSetting->GetCombatSetting(EGsOptionCombat::QUICKSLOT_PAGE_INDEX);
		_qickSlotList->SetTargetPage((EGsIconSlotCountType)slotPage);
	}

	UGsQuickSlotManager* slotMgr = GSQuickSlot();
	if (nullptr == slotMgr)
		return;

	slotMgr->CompleteLoadUI();
}

bool UGsUIHUDQuickSlot::UpdateManagerTick(float InDeltaTime)
{
	if (false == Super::UpdateManagerTick(InDeltaTime))
	{
		return false;
	}

	_qickSlotList->UpdateManagerTick(InDeltaTime);

	return true;
}

void UGsUIHUDQuickSlot::RemoveMsg()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
		return;

	for (TPair<MessageContentItem, FDelegateHandle>& elemQuickSlot : _listQuickSlotDelegate)
	{
		msg->GetItemContents().Remove(elemQuickSlot);
	}
	_listQuickSlotDelegate.Empty();

	msg->GetItem().Remove(_itemDelegate);
}

void UGsUIHUDQuickSlot::RegisterMsg()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg) 
		return;

	//_listQuickSlotDelegate.Emplace(msg->GetItemContents().AddUObject(MessageContentItem::QUICKSLOT_RESET_SELECTION,
		//this, &UGsUIHUDQuickSlot::ResetSelction));

	_itemDelegate = msg->GetItem().AddUObject(MessageItem::LOCKITEM, this, &UGsUIHUDQuickSlot::OnChangeQuickSlotDimmedState);
}


void UGsUIHUDQuickSlot::QuickSlotInvalidateAll()
{
	_qickSlotList->InvalidateAll();
}

void UGsUIHUDQuickSlot::QuickSlotInvalidateBySlotIndex(TArray<QuickSlotId> InSlotIndexList)
{
	_qickSlotList->InvalidateSlot(InSlotIndexList);
}

void UGsUIHUDQuickSlot::InvalidateAutoState(TArray<QuickSlotId> InSlotIndexList)
{
	_qickSlotList->InvalidateAutoState(InSlotIndexList);
}

void UGsUIHUDQuickSlot::OnChangeQuickSlotEditEffectState(bool InIsEditEffect)
{
	_qickSlotList->OnChangeEditMode(InIsEditEffect);
}

void UGsUIHUDQuickSlot::ResetSelectionInfo(const QuickSlotId InSlotId)
{
	_qickSlotList->ResetSelection(InSlotId);
}

void UGsUIHUDQuickSlot::ResetAllSelectionInfo()
{
	_qickSlotList->ResetAllSelection();
}


void UGsUIHUDQuickSlot::OnChangeQuickSlotDimmedState(struct FGsItemMessageParamBase& InParam)
{
	FGsReactionItemLock* paramData = static_cast<FGsReactionItemLock*>(&InParam);
	if (nullptr == paramData)
		return;

	_qickSlotList->SetSlotDimmed(paramData->_isLock, paramData->_lockItemCategorySubList);
}

