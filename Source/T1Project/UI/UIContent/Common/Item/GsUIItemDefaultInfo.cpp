// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIItemDefaultInfo.h"

#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/Spacer.h"
#include "Components/Image.h"

#include "Data/GsDataContainManager.h"

#include "DataSchema/Item/GsSchemaItemEquipConfig.h"

#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemManager.h"

#include "UTIL/GsItemUtil.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "Currency/GsCurrencyHelper.h"


void UGsUIItemDefaultInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_sBtnLock->OnClicked.AddDynamic(this, &UGsUIItemDefaultInfo::OnClickLock);
}

void UGsUIItemDefaultInfo::NativeDestruct()
{
	if (_iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}
	Super::NativeDestruct();
}

void UGsUIItemDefaultInfo::NativeConstruct()
{
	Super::NativeConstruct();

	_ItemDBID = 0;
}

void UGsUIItemDefaultInfo::SetData(const FGsItem* InItem,  bool bInHideButtons, bool InUseLock, bool bInItemCollectionMarking /*= false*/)
{
	if (nullptr == InItem)
	{
		return;
	}

	_ItemDBID = InItem->GetDBID();
	_isUseLock = InUseLock;

	// 아이콘
	if (UGsUIIconItemInventory* itemIcon = GetIcon())
	{
		if (GItem()->IsOwnedItem(InItem->GetDBID(), InItem->GetType()))
		{
			itemIcon->SetOwnedItem(InItem->GetDBID());
		}
		else
		{
			itemIcon->SetNotOwnItem(InItem->GetTID(), InItem->GetAmount(), InItem->GetLevel());
			itemIcon->SetSlotEffectMagicalForge(const_cast<FGsItem*>(InItem));
		}

		itemIcon->SetActiveIconBg(true);
		itemIcon->SetItemNameVisibility(false);
		itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);
		itemIcon->SetItemCollectionMarking(InItem, bInItemCollectionMarking);
	}

	// 이름
	SetUIItemName(InItem->GetName(), InItem->GetLevel(), InItem->GetGradeType());

	// 카테고리 이름
	FText textCategoryName;
	InItem->GetCategoryName(textCategoryName);
	_tbCategoryName->SetText(textCategoryName);
	
	if (ItemType::EQUIP == InItem->GetType())
	{
		// 안전강화, 실드 카운트
		FGsItem* tempItem = const_cast<FGsItem*>(InItem);
		if (const FGsItemEquipment* equipItem = tempItem->GetCastItem<const FGsItemEquipment>())
		{
			_tbSafeEnchantCount->SetText(FText::AsNumber(equipItem->GetSafeEnchantCount()));

			if (UNCONFIRMED_ENCHANT_SHIELD == equipItem->GetEnchantShieldCount())
			{
				FString str = 0 >= equipItem->GetEnchantShieldId() ? "0" : "?";
				_tbShieldCount->SetText(FText::FromString(str));
			}
			else
			{
				_tbShieldCount->SetText(FText::AsNumber(equipItem->GetEnchantShieldCount()));
			}

			// 마법 부여 정보
			const FGsSchemaItemEquipConfig* itemEquipConfig = UGsTableUtil::FindFirstRow<FGsSchemaItemEquipConfig>();
			if (false == IsBranch(EGsGameClientBranchType::MAGICAL_FORGE) ||
				nullptr == itemEquipConfig || 
				static_cast<uint8>(equipItem->GetGradeType()) < static_cast<uint8>(itemEquipConfig->magicalForgeGrade))
			{
				_spacerMagicalForge->SetVisibility(ESlateVisibility::Collapsed);
				_imageMagicalForge->SetVisibility(ESlateVisibility::Collapsed);
				_textBlockMagicalForge->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				_spacerMagicalForge->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				_imageMagicalForge->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				_textBlockMagicalForge->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

				_textBlockMagicalForge->SetText(FText::AsNumber(equipItem->GetMagicalForgeData().mLevel));
			}
		}

		// 캐쉬템 여부
		_vcItemPanel->SetVisibility(InItem->IsCashItem() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		_panelCountRoot->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		_tbSafeEnchantCount->SetText(FText::AsNumber(0));
		_tbShieldCount->SetText(FText::AsNumber(0));

		// 캐쉬템 여부, 장비 아닐 땐 안보여줌
		_vcItemPanel->SetVisibility(ESlateVisibility::Collapsed);
		_panelCountRoot->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	// 락 표시
	_sBtnLock->SetSwitcherIndex(InItem->GetLock() ? 1 : 0);

	// 인벤토리일 일때와 잠금 처리되지 말아야할 아이템일때 제외하고 출력
	bool isHide = (bInHideButtons || InItem->IsDisableLockItem() || (false == _isUseLock && false == InItem->GetLock()));
	_sBtnLock->SetVisibility(isHide ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

void UGsUIItemDefaultInfo::SetData(const CurrencyType InCurrencyType, const Currency InAmount, bool bInItemCollectionMarking /*= false*/)
{
	// 아이콘
	if (UGsUIIconItemInventory* itemIcon = GetIcon())
	{
		itemIcon->SetCurrency(InCurrencyType, InAmount);
		itemIcon->SetRedDot(false);

		itemIcon->SetActiveIconBg(true);
		itemIcon->SetItemNameVisibility(false);
		itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);
		// 2023/4/18 PKT - 주화는 계획 없음
		itemIcon->SetItemCollectionMarking(nullptr, bInItemCollectionMarking);
	}

	// 이름
	SetUIItemName(FGsCurrencyHelper::GetCurrencyLocalizedText(InCurrencyType), 0, ItemGrade::NONE);

	// 카테고리 이름
	FText textCategoryName;
	if (!FText::FindText(TEXT("CurrencyText"), TEXT("Currency"), textCategoryName))
	{
		GSLOG(Warning, TEXT("!FText::FindText(TEXT('CurrencyText'), TEXT('Currency'), textCategoryName)"));
		textCategoryName = FText::FromString(TEXT("Currency"));
	}
	_tbCategoryName->SetText(textCategoryName);

	_tbSafeEnchantCount->SetText(FText::AsNumber(0));
	_tbShieldCount->SetText(FText::AsNumber(0));

	// 캐쉬템 여부
	_vcItemPanel->SetVisibility(ESlateVisibility::Collapsed);
	_panelCountRoot->SetVisibility(ESlateVisibility::Collapsed);

	// 락 표시
	_sBtnLock->SetSwitcherIndex(0);
	_sBtnLock->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIItemDefaultInfo::SetItemAmount(int64 InAmount)
{
	if (UGsUIIconItemInventory* itemIcon = GetIcon())
	{
		itemIcon->SetItemCount(InAmount);
	}
}

void UGsUIItemDefaultInfo::OnClickLock()
{
	if (false == _isUseLock)
		return;

	ItemStorageType type = UGsItemUtil::FindItemStorageType(_ItemDBID);
	if (ItemStorageType::MAX == type)
		return;

	const FGsItem* findData = UGsItemUtil::FindItemByStorageType(_ItemDBID, type);
	if (nullptr == findData)
		return;

	if (false == findData->IsDisableLockItem())
	{
		bool bIsLock = (_sBtnLock->GetSwitcherIndex() == 1) ? true : false;
		bIsLock = !bIsLock; // 스왑 위해 반대값을 넣음
		GItem()->SendItemLock(findData->GetDBID(), bIsLock, type);
	}
}

void UGsUIItemDefaultInfo::SetUIItemName(const FText& InNameText, Level InEnchantLevel, ItemGrade InGrade)
{
	_tbName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(InGrade));

	if (0 < InEnchantLevel)
	{
		FString strItemName = FString::Format(TEXT("+{0} {1}"), { InEnchantLevel, *InNameText.ToString() });
		_tbName->SetText(FText::FromString(strItemName));
	}
	else
	{
		_tbName->SetText(InNameText);
	}
}

UGsUIIconItemInventory* UGsUIItemDefaultInfo::GetIcon()
{
	if (0 == _iconSelector->GetChildrenCount())
	{
		UGsUIIconBase* newIcon = _iconSelector->CreateIcon();
		if (newIcon)
		{
			_iconSelector->AddChild(newIcon);

			if (UGsUIIconItemInventory* newItemIcon = Cast<UGsUIIconItemInventory>(newIcon))
			{
				newItemIcon->ResetCoolTime();
				//newItemIcon->SetItemNameVisibility(false); // 어짜피 또해줌
				newItemIcon->SetItemCount(0);

				return newItemIcon;
			}
		}
	}

	// 아이콘을 하나만 생성하므로 0번째 아이콘 리턴
	return Cast<UGsUIIconItemInventory>(_iconSelector->GetChildAt(0));
}
