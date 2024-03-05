// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Tray/ItemEnchant/GsUITrayEnchantResultDown.h"
#include "Components/TextBlock.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"




void UGsUITrayEnchantResultDown::BeginDestroy()
{
	if (nullptr != _targetIconSlotHelper)
	{
		_targetIconSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUITrayEnchantResultDown::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _targetIconSlotHelper)
	{
		_targetIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_targetIconSlotHelper->Initialize(_targetItemIconSelector);
		_targetIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUITrayEnchantResultDown::OnRefreshTargetIcon);
		_targetIconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUITrayEnchantResultDown::OnCreateTargetIcon);
	}
}

void UGsUITrayEnchantResultDown::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 클릭까지 1초 정도의 텀을 둠 (바로 광클로 인한 혹이나 꼬임현상 방지 / 리니지2m 동일함)
	_currentTime = 0.f;
	_lockMaxTime = 1.f;

	_btnConfirm->OnClicked.AddDynamic(this, &UGsUITrayEnchantResultDown::OnclickConfirm);
}

void UGsUITrayEnchantResultDown::NativeDestruct()
{
	_targetItemIconSelector->RemoveAllChildren();

	_targetItemDbid = 0;

	_btnConfirm->OnClicked.RemoveDynamic(this, &UGsUITrayEnchantResultDown::OnclickConfirm);

	Super::NativeDestruct();
}

void UGsUITrayEnchantResultDown::SetDisplayItem(const ItemDBId InItemDbid, float InLockMaxTime)
{
	TWeakPtr<FGsItem> findItem = GItem()->FindItem(InItemDbid, ItemType::EQUIP);
	if (false == findItem.IsValid())
		return;

	_targetItemDbid = InItemDbid;
	_lockMaxTime = InLockMaxTime;

	FString resultStr;
	if (0 < findItem.Pin()->GetLevel())
	{
		FText mark;
		FText statName = findItem.Pin()->GetName();
		FText::FindText(TEXT("MarkFormatingText"), TEXT("Plus"), mark);
		resultStr = FString::Format(TEXT("{0}{1} {2}"), { *mark.ToString(), findItem.Pin()->GetLevel(), *statName.ToString() });
	}
	else
	{
		resultStr = findItem.Pin()->GetName().ToString();
	}
	_itemNameText->SetText(FText::FromString(resultStr));
	_targetIconSlotHelper->RefreshAll(1);
}

void UGsUITrayEnchantResultDown::OnclickConfirm()
{
	if (false == IsPossibleOnClick())
		return;

	Close();
}

void UGsUITrayEnchantResultDown::OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->SetOwnedItem(_targetItemDbid);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetItemEnchantLevel(0);
}

void UGsUITrayEnchantResultDown::OnCreateTargetIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->SetItemNameVisibility(false);
	//itemIcon->SetItemCount(0);
}

bool UGsUITrayEnchantResultDown::IsPossibleOnClick() const
{
	return (_currentTime >= _lockMaxTime);
}

// 버튼 클릭까지 1초 정도의 텀을 둠 (바로 광클로 인한 혹이나 꼬임현상 방지 / 리니지2m 동일함)
void UGsUITrayEnchantResultDown::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	_currentTime += InDeltaTime;
	Super::NativeTick(MyGeometry, InDeltaTime);
}