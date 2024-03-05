// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayEnchantResultSuccess.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemEquipment.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"


void UGsUITrayEnchantResultSuccess::BeginDestroy()
{
	if (nullptr != _targetIconSlotHelper)
	{
		_targetIconSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUITrayEnchantResultSuccess::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _targetIconSlotHelper)
	{
		_targetIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_targetIconSlotHelper->Initialize(_targetItemIconSelector);
		_targetIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUITrayEnchantResultSuccess::OnRefreshTargetIcon);
		_targetIconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUITrayEnchantResultSuccess::OnCreateTargetIcon);
	}

}

void UGsUITrayEnchantResultSuccess::NativeConstruct()
{
	Super::NativeConstruct();

	_destroyCallback = nullptr;

	// 버튼 클릭까지 2초 정도의 텀을 둠 (바로 광클로 인한 혹이나 꼬임현상 방지 / 리니지2m 동일함)
	_currentTime = 0.f;
	_lockMaxTime = 1.f;

	_btnConfirm->OnClicked.AddDynamic(this, &UGsUITrayEnchantResultSuccess::OnclickConfirm);

	FText successText;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_Success"), successText);
	SetSuccessText(successText);
}

void UGsUITrayEnchantResultSuccess::NativeDestruct()
{
	_targetItemIconSelector->RemoveAllChildren();

	_btnConfirm->OnClicked.RemoveDynamic(this, &UGsUITrayEnchantResultSuccess::OnclickConfirm);

	if (_destroyCallback)
	{
		_destroyCallback();
	}

	_targetItemDbid = 0;

	Super::NativeDestruct();
}

void UGsUITrayEnchantResultSuccess::SetDisplayItem(const ItemDBId InItemDbid, float InLockMaxTime, bool InIsEnchant)
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

void UGsUITrayEnchantResultSuccess::SetDestroyCallback(TFunction<void()> InDestroyCallback)
{
	_destroyCallback = InDestroyCallback;
}

void UGsUITrayEnchantResultSuccess::SetSuccessText(const FText& InText)
{
	_itemSuccessText->SetText(InText);
}

void UGsUITrayEnchantResultSuccess::OnclickConfirm()
{
	if (false == IsPossibleOnClick())
		return;

	Close();
}

void UGsUITrayEnchantResultSuccess::OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon)
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

void UGsUITrayEnchantResultSuccess::OnCreateTargetIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->SetItemNameVisibility(false);
	//itemIcon->SetItemCount(0);
}

bool UGsUITrayEnchantResultSuccess::IsPossibleOnClick() const
{
	return (_currentTime >= _lockMaxTime);
}

void UGsUITrayEnchantResultSuccess::SetDisplayEnchant()
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

	// 성공 문구
	FText successText;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_Notice_Success"), successText);
	SetSuccessText(successText);

	// 아이템 이름
	FText itemNameText;
	GetItemNameText(equipmentItem->GetName(), equipmentItem->GetLevel(), itemNameText);
	_itemNameText->SetText(itemNameText);

	// 아이템 아이콘
	_targetIconSlotHelper->RefreshAll(1);

	// 강화 보호 수치
	_switcherIcon->SetActiveWidgetIndex(SHOW_SHIELD);
	_itemShieldCountText->SetText(FText::AsNumber(equipmentItem->GetEnchantShieldCount()));
}

void UGsUITrayEnchantResultSuccess::SetDisplayMagicalForge()
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

	// 성공 문구
	FText successText;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_MagicalForge_ResultSuccess"), successText);
	SetSuccessText(successText);

	// 아이템 이름
	FText itemNameText;
	GetItemNameText(equipmentItem->GetName(), equipmentItem->GetLevel(), itemNameText);
	_itemNameText->SetText(itemNameText);

	// 아이템 아이콘
	_targetIconSlotHelper->RefreshAll(1);

	// 마법 부여 정보
	_switcherIcon->SetActiveWidgetIndex(SHOW_MAGICAL_FORGE);
	_itemShieldCountText->SetText(FText::AsNumber(equipmentItem->GetMagicalForgeData().mLevel));
}

void UGsUITrayEnchantResultSuccess::GetItemNameText(const FText& InNameText, const Level InLevel, OUT FText& OutNameText)
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
void UGsUITrayEnchantResultSuccess::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	_currentTime += InDeltaTime;
	Super::NativeTick(MyGeometry, InDeltaTime);
}