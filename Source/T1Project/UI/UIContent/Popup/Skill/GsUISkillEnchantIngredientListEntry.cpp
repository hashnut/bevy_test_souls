// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillEnchantIngredientListEntry.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "Item/GsItemManager.h"
#include "Item/GsItem.h"


void UGsUISkillEnchantIngredientListEntry::NativeDestruct()
{
	// 아이콘 제거(_iconSelector에 Add되었던 것 삭제)
	_iconSelector->RemoveAllChildren();	

	Super::NativeDestruct();
}

void UGsUISkillEnchantIngredientListEntry::SetData(ItemId InItemId, ItemAmount InNeedAmount)
{
	// 아이콘 표시
	if (UGsUIIconItemInventory* icon = GetIcon())
	{
		icon->SetNotOwnItem(InItemId, 0);
		icon->SetItemNameVisibility(false);
	}

	// Lock걸린 아이템 제외하고 개수 얻어오기
	ItemAmount currCount = GItem()->GetAllItemCountByTID(InItemId, true);

	_textBlockCurrCount->SetText(FText::AsNumber(currCount));
	_textBlockCurrCount->SetColorAndOpacity((currCount >= InNeedAmount) ?
		FGsUIColorHelper::GetColor(EGsUIColorType::ENCHANT_ENOUGH_COST) : 
		FGsUIColorHelper::GetColor(EGsUIColorType::ENCHANT_NEED_COST));

	_textBlockNeedCount->SetText(FText::AsNumber(InNeedAmount));
}

void UGsUISkillEnchantIngredientListEntry::OnLongPressIcon(class UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

UGsUIIconItemInventory* UGsUISkillEnchantIngredientListEntry::GetIcon()
{
	if (0 == _iconSelector->GetChildrenCount())
	{
		UGsUIIconBase* newIcon = _iconSelector->CreateIcon();
		_iconSelector->AddChild(newIcon);

		if (UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(newIcon))
		{			
			//itemIcon->SetEnableDetailPopup(true);
			//itemIcon->SetCenterPopup(true);
			itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUISkillEnchantIngredientListEntry::OnLongPressIcon);

			return itemIcon;
		}
	}

	return Cast<UGsUIIconItemInventory>(_iconSelector->GetChildAt(0));
}
