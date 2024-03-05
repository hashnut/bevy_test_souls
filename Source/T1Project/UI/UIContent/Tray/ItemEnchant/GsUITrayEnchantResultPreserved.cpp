// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Tray/ItemEnchant/GsUITrayEnchantResultPreserved.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemManager.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"



void UGsUITrayEnchantResultPreserved::BeginDestroy()
{
	if (nullptr != _targetIconSlotHelper)
	{
		_targetIconSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUITrayEnchantResultPreserved::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _targetIconSlotHelper)
	{
		_targetIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_targetIconSlotHelper->Initialize(_targetItemIconSelector);
		_targetIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUITrayEnchantResultPreserved::OnRefreshTargetIcon);
		_targetIconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUITrayEnchantResultPreserved::OnCreateTargetIcon);
	}
}

void UGsUITrayEnchantResultPreserved::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 클릭까지 2초 정도의 텀을 둠 (바로 광클로 인한 혹이나 꼬임현상 방지 / 리니지2m 동일함)
	_currentTime = 0.f;
	_lockMaxTime = 1.f;

	_btnConfirm->OnClicked.AddDynamic(this, &UGsUITrayEnchantResultPreserved::OnclickConfirm);
}

void UGsUITrayEnchantResultPreserved::NativeDestruct()
{
	_targetItemIconSelector->RemoveAllChildren();

	_targetItemDbid = 0;

	_btnConfirm->OnClicked.RemoveDynamic(this, &UGsUITrayEnchantResultPreserved::OnclickConfirm);

	Super::NativeDestruct();
}

void UGsUITrayEnchantResultPreserved::SetDisplayItem(const ItemDBId InItemDbid, float InLockMaxTime, bool InIsEnchant)
{
	_targetItemDbid = InItemDbid;
	_lockMaxTime = InLockMaxTime;
	
	if (InIsEnchant)
	{
		SetDisplayEnchant();
	}
	else
	{
		SetDisplayMagicalForge();
	}
}

void UGsUITrayEnchantResultPreserved::OnclickConfirm()
{
	if (false == IsPossibleOnClick())
		return;

	Close();
}

void UGsUITrayEnchantResultPreserved::OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon)
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

void UGsUITrayEnchantResultPreserved::OnCreateTargetIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->SetItemNameVisibility(false);
	//itemIcon->SetItemCount(0);
}

bool UGsUITrayEnchantResultPreserved::IsPossibleOnClick() const
{
	return (_currentTime >= _lockMaxTime);
}

void UGsUITrayEnchantResultPreserved::SetDisplayEnchant()
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	TWeakPtr<FGsItem> findItem = itemManager->FindItem(_targetItemDbid, ItemType::EQUIP);
	if (false == findItem.IsValid())
		return;

	// 실패 문구 : 강화 실패
	FText failText;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_Fail"), failText);
	_textBlockFail->SetText(failText);

	// 아이템 이름
	FText itemNameText;
	GetItemNameText(findItem.Pin()->GetName(), findItem.Pin()->GetLevel(), itemNameText);
	_itemNameText->SetText(itemNameText);

	// 아이템 아이콘
	_targetIconSlotHelper->RefreshAll(1);

	// 추가 정보 감추기
	_panelCount->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUITrayEnchantResultPreserved::SetDisplayMagicalForge()
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	TWeakPtr<FGsItem> findItem = itemManager->FindItem(_targetItemDbid, ItemType::EQUIP);
	if (false == findItem.IsValid())
		return;

	FGsItemEquipment* equipmentItem = findItem.Pin()->GetCastItem<FGsItemEquipment>();
	if (nullptr == equipmentItem)
		return;

	// 실패 문구 : 마법의 힘이 흩어졌습니다...
	FText failText;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_MagicalForge_ResultFail"), failText);
	_textBlockFail->SetText(failText);

	// 아이템 이름
	FText itemNameText;
	GetItemNameText(findItem.Pin()->GetName(), findItem.Pin()->GetLevel(), itemNameText);
	_itemNameText->SetText(itemNameText);

	// 아이템 아이콘
	_targetIconSlotHelper->RefreshAll(1);

	// 추가 정보 감추기
	_panelCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_textBlockCount->SetText(FText::AsNumber(equipmentItem->GetMagicalForgeData().mLevel));
}

void UGsUITrayEnchantResultPreserved::GetItemNameText(const FText& InNameText, const Level InLevel, OUT FText& OutNameText)
{
	if (0 < InLevel)
	{
		FText mark;
		FText::FindText(TEXT("MarkFormatingText"), TEXT("Plus"), mark);
		FString resultStr = FString::Format(TEXT("{0}{1} {2}"), { *mark.ToString(), InLevel, *InNameText.ToString() });
		OutNameText = FText::FromString(resultStr);
	}
	else
	{
		OutNameText = InNameText;
	}
}

// 버튼 클릭까지 2초 정도의 텀을 둠 (바로 광클로 인한 혹이나 꼬임현상 방지 / 리니지2m 동일함)
void UGsUITrayEnchantResultPreserved::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	_currentTime += InDeltaTime;
	Super::NativeTick(MyGeometry, InDeltaTime);
}