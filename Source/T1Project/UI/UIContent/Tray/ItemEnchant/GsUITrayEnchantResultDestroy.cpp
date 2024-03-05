// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayEnchantResultDestroy.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"


void UGsUITrayEnchantResultDestroy::BeginDestroy()
{
	if (nullptr != _targetIconSlotHelper)
	{
		_targetIconSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUITrayEnchantResultDestroy::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _targetIconSlotHelper)
	{
		_targetIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_targetIconSlotHelper->Initialize(_targetItemIconSelector);
		_targetIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUITrayEnchantResultDestroy::OnRefreshTargetIcon);
		_targetIconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUITrayEnchantResultDestroy::OnCreateTargetIcon);
	}

}

void UGsUITrayEnchantResultDestroy::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 클릭까지 2초 정도의 텀을 둠 (바로 광클로 인한 혹이나 꼬임현상 방지 / 리니지2m 동일함)
	_currentTime = 0.f;
	_lockMaxTime = 1.f;
	_itemTid = 0;

	_btnConfirm->OnClicked.AddDynamic(this, &UGsUITrayEnchantResultDestroy::OnclickConfirm);
}

void UGsUITrayEnchantResultDestroy::NativeDestruct()
{
	_targetItemIconSelector->RemoveAllChildren();
	_btnConfirm->OnClicked.RemoveDynamic(this, &UGsUITrayEnchantResultDestroy::OnclickConfirm);

	Super::NativeDestruct();
}

void UGsUITrayEnchantResultDestroy::SetDisplayItem(ItemId InItemTid, float InLockMaxTime)
{
	_itemTid = InItemTid;
	_lockMaxTime = InLockMaxTime;

	_targetIconSlotHelper->RefreshAll(1);
}

void UGsUITrayEnchantResultDestroy::OnclickConfirm()
{
	if (false == IsPossibleOnClick())
		return;

	Close();
}

void UGsUITrayEnchantResultDestroy::OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->SetNotOwnItem(_itemTid, 1);
	itemIcon->SetItemNameVisibility(false);
}

void UGsUITrayEnchantResultDestroy::OnCreateTargetIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetItemCount(0);
}

bool UGsUITrayEnchantResultDestroy::IsPossibleOnClick() const
{
	return (_currentTime >= _lockMaxTime);
}

// 버튼 클릭까지 2초 정도의 텀을 둠 (바로 광클로 인한 혹이나 꼬임현상 방지 / 리니지2m 동일함)
void UGsUITrayEnchantResultDestroy::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	_currentTime += InDeltaTime;
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUITrayEnchantResultDestroy::OnDissolveEffect()
{
	if (nullptr != _targetIconSlotHelper->GetIcon(0))
	{
		UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(_targetIconSlotHelper->GetIcon(0));
		itemIcon->PlayDissolveEffect();
	}
}