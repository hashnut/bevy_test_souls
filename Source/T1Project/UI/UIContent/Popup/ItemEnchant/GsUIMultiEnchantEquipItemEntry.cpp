// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/ItemEnchant/GsUIMultiEnchantEquipItemEntry.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "Components/WidgetSwitcher.h"
#include "Management/ScopeGame/GsItemEnchantManager.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "Item/GsItemManager.h"
#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"


void UGsUIMultiEnchantEquipItemEntry::BeginDestroy()
{
	if (nullptr != _IconSlotHelper)
	{
		_IconSlotHelper->OnRefreshIcon.RemoveDynamic(this, &UGsUIMultiEnchantEquipItemEntry::OnRefreshTargetIcon);
		_IconSlotHelper->Clear();
		_IconSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIMultiEnchantEquipItemEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _IconSlotHelper)
	{
		_IconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_IconSlotHelper->Initialize(_itemIconSelector);
		_IconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIMultiEnchantEquipItemEntry::OnRefreshTargetIcon);
	}
	_btnOnClick->OnClicked.AddDynamic(this, &UGsUIMultiEnchantEquipItemEntry::OnclickEqiupItem);
}

void UGsUIMultiEnchantEquipItemEntry::OnShow()
{
	_isLockItem = false;
	_equipItemTid = 0;
	_equipItemLevel = 0;
	_resultType = ItemEnchantResult::NONE;

	_stateSwitcher->SetActiveWidgetIndex(0);
	_enchantProgressEffectRoot->SetVisibility(ESlateVisibility::Collapsed);

	if (false == _normalProgressEffect->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_normalProgressEffect->OnVFXAnimationFinished.BindUObject(this, &UGsUIMultiEnchantEquipItemEntry::BindEnchantProcFinishedEvent);
	}
	if (false == _cursedProgressEffect->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_cursedProgressEffect->OnVFXAnimationFinished.BindUObject(this, &UGsUIMultiEnchantEquipItemEntry::BindEnchantProcFinishedEvent);
	}
	if (false == _bigSuccessEffect->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_bigSuccessEffect->OnVFXAnimationFinished.BindUObject(this, &UGsUIMultiEnchantEquipItemEntry::BindEnchantResultFinishedEvent);
	}
	if (false == _cursedEffect->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_cursedEffect->OnVFXAnimationFinished.BindUObject(this, &UGsUIMultiEnchantEquipItemEntry::BindEnchantResultFinishedEvent);
	}
	if (false == _successEffect->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_successEffect->OnVFXAnimationFinished.BindUObject(this, &UGsUIMultiEnchantEquipItemEntry::BindEnchantResultFinishedEvent);
	}
	if (false == _destroyEffect->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_destroyEffect->OnVFXAnimationFinished.BindUObject(this, &UGsUIMultiEnchantEquipItemEntry::BindEnchantResultFinishedEvent);
	}
	if (false == _shieldEffect->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_shieldEffect->OnVFXAnimationFinished.BindUObject(this, &UGsUIMultiEnchantEquipItemEntry::BindEnchantResultFinishedEvent);
	}
	if (false == _preserveEffect->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_preserveEffect->OnVFXAnimationFinished.BindUObject(this, &UGsUIMultiEnchantEquipItemEntry::BindEnchantResultFinishedEvent);
	}
}

void UGsUIMultiEnchantEquipItemEntry::OnHide()
{
	if (OnClickEquipItem.IsBound())
	{
		OnClickEquipItem.Clear();
	}
	if (OnFinishedSlotEffect.IsBound())
	{
		OnFinishedSlotEffect.Clear();
	}
	
	_enchantProgressEffectRoot->SetVisibility(ESlateVisibility::Collapsed);
	_itemIconSelector->RemoveAllChildren();

	_normalProgressEffect->HideVFX();
	_cursedProgressEffect->HideVFX();
	_bigSuccessEffect->HideVFX();
	_cursedEffect->HideVFX();
	_successEffect->HideVFX();
	_destroyEffect->HideVFX();
	_shieldEffect->HideVFX();
	_preserveEffect->HideVFX();

	_isLockItem = false;
}

/*
*  _stateSwitcher;						//  0 : empty  1 : expect  2 : result
		_enchantExpectEffectSwitcher;	//  0 :  succFx  1 : failFx
		_resultSwitcher;				// 0 : bigSucc  1 : cursed   2 : fail  3 : preserve  4 : succ  5 : shield
*/

void UGsUIMultiEnchantEquipItemEntry::SetData(const int32 InIndex, const ItemDBId InItemDbid, const int32 InTargetLevel, const bool InIsCursed)
{
	if (0 < InItemDbid)
	{
		TWeakPtr<FGsItem> findItemData = GItem()->FindItem(InItemDbid, ItemType::EQUIP);
		if (findItemData.IsValid())
		{
			_isLockItem = findItemData.Pin()->GetLock();
			_stateSwitcher->SetActiveWidgetIndex(0 > InTargetLevel ? 0 : 1);
			if (0 != _stateSwitcher->GetActiveWidgetIndex())
			{
				bool flag =  IsActiveEnchantProcEffect(findItemData, InTargetLevel, InIsCursed);
				if (false == flag)
				{
					_stateSwitcher->SetActiveWidgetIndex(0);
				}
			}
			_equipItemTid = findItemData.Pin()->GetTID();
			_equipItemLevel = findItemData.Pin()->GetLevel();
			if (FGsItemEquipment* equipData = findItemData.Pin()->GetCastItem<FGsItemEquipment>())
			{
				bool isRiskEnchant = false;
				if (false == InIsCursed)
				{
					if (InTargetLevel - 1 < equipData->GetMaxLevel())
					{
						int32 rate = GSItemEnchant()->GetEnchantItemRate(ItemEnchantResult::ENCHANT_FAILED, equipData->GetEnchantRateId(), InTargetLevel - 1);
						isRiskEnchant = 0 < rate;
					}
				}
				_enchantExpectEffectSwitcher->SetActiveWidgetIndex(isRiskEnchant ? 1 : 0);
			}
		}
		else
		{
			// destroy item
			_isLockItem = false;
		}
	}
	else
	{
		_isLockItem = false;
		_equipItemTid = 0;
		_equipItemLevel = 0;
		_targetItemDbid = 0;
		_targetLevel = 0;
		_isCursed = false;
		_stateSwitcher->SetActiveWidgetIndex(0);
	}

	_slotIndex = InIndex;
	_targetItemDbid = InItemDbid;
	_targetLevel = InTargetLevel;
	_isCursed = InIsCursed;
	_IconSlotHelper->RefreshAll(1);

	AllOffHideEffect();
}

float UGsUIMultiEnchantEquipItemEntry::SetEnchantWaitProcEffect(const bool InIsCursed, float InAnimSpeed)
{
	_enchantProgressEffectRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (false == InIsCursed)
	{
		_normalProgressEffect->_playbackSpeed = InAnimSpeed;
		_normalProgressEffect->ShowVFX();
		return _normalProgressEffect->GetAnimEndTime();
	}
	else
	{
		_cursedProgressEffect->_playbackSpeed = InAnimSpeed;
		_cursedProgressEffect->ShowVFX();
		return _cursedProgressEffect->GetAnimEndTime();
	}
	return 0.f;
}

void UGsUIMultiEnchantEquipItemEntry::BindEnchantProcFinishedEvent()
{
	_enchantProgressEffectRoot->SetVisibility(ESlateVisibility::Collapsed);
}

//UWidgetSwitcher* _resultSwitcher;  // 0 : bigSucc  1 : cursed   2 : fail  3 : preserve  4 : succ  5 : shield
int32 UGsUIMultiEnchantEquipItemEntry::OnEffect(const ItemEnchantResult InResultType, float InAnimSpeed)
{
	GSItemEnchant()->PlayEffectSound(FGsItemEnchantManager::EnchantEffectSoundResType::MultiEnchant_End, InAnimSpeed);

	_stateSwitcher->SetActiveWidgetIndex(2);
	switch (InResultType)
	{
	case ItemEnchantResult::ENCHANT_FAILED_DESTROYED:
	{
		_resultSwitcher->SetActiveWidgetIndex(2);
		_destroyEffect->_playbackSpeed = InAnimSpeed;
		_destroyEffect->ShowVFX();
	}
	break;
	case ItemEnchantResult::ENCHANT_FAILED_SHIELDED:
	{
		_resultSwitcher->SetActiveWidgetIndex(5);
		_shieldEffect->_playbackSpeed = InAnimSpeed;
		_shieldEffect->ShowVFX();
	}
	break;
	case ItemEnchantResult::ENCHANT_PRESERVED:
	{
		_resultSwitcher->SetActiveWidgetIndex(3);
		_preserveEffect->_playbackSpeed = InAnimSpeed;
		_preserveEffect->ShowVFX();
	}
	break;
	case ItemEnchantResult::ENCHANT_SUCCEEDED_CURSED:
	{
		_resultSwitcher->SetActiveWidgetIndex(1);
		_cursedEffect->_playbackSpeed = InAnimSpeed;
		_cursedEffect->ShowVFX();
	}
	break;
	case ItemEnchantResult::ENCHANT_SUCCEEDED:
	{
		_resultSwitcher->SetActiveWidgetIndex(4);
		_successEffect->_playbackSpeed = InAnimSpeed;
		_successEffect->ShowVFX();
	}
	break;
	case ItemEnchantResult::ENCHANT_SUCCEEDED_BLESSED:
	{
		_resultSwitcher->SetActiveWidgetIndex(0);
		_bigSuccessEffect->_playbackSpeed = InAnimSpeed;
		_bigSuccessEffect->ShowVFX();
	}
	break;
	}

	_resultType = InResultType;
	return _slotIndex;
}

void UGsUIMultiEnchantEquipItemEntry::BindEnchantResultFinishedEvent()
{
	if (ItemEnchantResult::ENCHANT_FAILED_DESTROYED == _resultType)
	{
		_equipItemTid = 0;
		_equipItemLevel = 0;
		_stateSwitcher->SetActiveWidgetIndex(0);
	}
	else
	{
		TWeakPtr<FGsItem> findItemData = GItem()->FindItem(_targetItemDbid, ItemType::EQUIP);
		if (findItemData.IsValid())
		{
			_stateSwitcher->SetActiveWidgetIndex(0 > _targetLevel ? 0 : 1);
			if (0 != _stateSwitcher->GetActiveWidgetIndex())
			{
				bool flag = IsActiveEnchantProcEffect(findItemData, _targetLevel, _isCursed);
				if (false == flag)
				{
					_stateSwitcher->SetActiveWidgetIndex(0);
				}
			}

			_equipItemTid = findItemData.Pin()->GetTID();
			_equipItemLevel = findItemData.Pin()->GetLevel();
		}
	}
	_resultType = ItemEnchantResult::NONE;
	_IconSlotHelper->RefreshAll(1);

	if (OnFinishedSlotEffect.IsBound())
	{
		OnFinishedSlotEffect.Broadcast(_slotIndex, _targetItemDbid);
		_targetItemDbid = ItemEnchantResult::ENCHANT_FAILED_DESTROYED == _resultType ? 0 : _targetItemDbid;
		_targetLevel = ItemEnchantResult::ENCHANT_FAILED_DESTROYED == _resultType ? 0 : _targetLevel;
		_isCursed = ItemEnchantResult::ENCHANT_FAILED_DESTROYED == _resultType ? false : _isCursed;
	}
}

bool UGsUIMultiEnchantEquipItemEntry::IsActiveEnchantProcEffect(TWeakPtr<FGsItem> InItemData, int32 InTargetLv, bool InIsCursed)
{
	if (0 > InTargetLv)
		return true;

	if (InIsCursed)
	{
		if (InItemData.Pin()->GetLevel() == InItemData.Pin()->GetMinLevel())
			return false;

		return InItemData.Pin()->GetLevel() > InTargetLv;
	}
	else
	{
		Level currentLevel = InItemData.Pin()->GetLevel();
		Level maxLevel = InItemData.Pin()->GetMaxLevel();

		if (currentLevel == maxLevel)
			return false;

		return currentLevel < InTargetLv;
	}

	return false;
}

void UGsUIMultiEnchantEquipItemEntry::AllOffHideEffect()
{
	_normalProgressEffect->HideVFX();
	_cursedProgressEffect->HideVFX();
	_bigSuccessEffect->HideVFX();
	_cursedEffect->HideVFX();
	_successEffect->HideVFX();
	_destroyEffect->HideVFX();
	_shieldEffect->HideVFX();
	_preserveEffect->HideVFX();
}

void UGsUIMultiEnchantEquipItemEntry::OnclickEqiupItem()
{
	if (OnClickEquipItem.IsBound())
	{
		OnClickEquipItem.Broadcast(_slotIndex, _targetItemDbid);
	}
}

void UGsUIMultiEnchantEquipItemEntry::OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}
	itemIcon->ResetCoolTime();
	itemIcon->SetNotOwnItem(_equipItemTid, 1, _equipItemLevel);
	itemIcon->SetLockImage(_isLockItem);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameToolTipFlag(true);
	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);
}

void UGsUIMultiEnchantEquipItemEntry::OnDissolveEffect()
{
	if (nullptr != _IconSlotHelper->GetIcon(0))
	{
		UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(_IconSlotHelper->GetIcon(0));
		itemIcon->PlayDissolveEffect();
	}
}