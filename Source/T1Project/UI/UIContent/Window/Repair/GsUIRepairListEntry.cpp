// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRepairListEntry.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "DataSchema/Item/GsSchemaItemCommon.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"

#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "Item/GsItemEquipment.h"


void UGsUIRepairListEntry::BeginDestroy()
{
	if (nullptr != _btnSlot)
	{
		_btnSlot->OnClicked.RemoveDynamic(this, &UGsUIRepairListEntry::OnClickItemSlot);
	}
	_btnSlot = nullptr;

	Super::BeginDestroy();
}

void UGsUIRepairListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIRepairListEntry::OnClickItemSlot);
}

void UGsUIRepairListEntry::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsSelected(false);
}

void UGsUIRepairListEntry::NativeDestruct()
{
	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
		_uiIcon = nullptr;
	}

	Super::NativeDestruct();
}

void UGsUIRepairListEntry::OnClickItemSlot()
{
	OnClickedItemSlot.ExecuteIfBound(this);
}

void UGsUIRepairListEntry::SetData(const FGsSchemaItemCommon* InItemTableData, Level InItemLevel, EnchantBonusIndex InEnchantBonusIndex, ItemDBId InItemDBId)
{
	if (nullptr == InItemTableData)
		return;

	if (nullptr == _uiIcon)
	{
		_uiIcon = Cast<UGsUIIconItemInventory>(_iconSelector->CreateIcon());
		_iconSelector->AddChild(_uiIcon);
	}

	if (nullptr == _uiIcon)
		return;
	
	_uiIcon->SetNotOwnItem(InItemTableData->id, 1, InItemLevel, 0, InEnchantBonusIndex);
	_uiIcon->SetItemNameVisibility(false);

	FGsItem* itemData = _uiIcon->GetItemIconData();
	itemData->SetDBID(InItemDBId);

	FGsItemEquipment* itemEquipData = _uiIcon->GetItemIconData()->GetCastItem<FGsItemEquipment>();
	if (nullptr == itemEquipData)
		return;

	const FLinearColor itemColor = FGsUIColorHelper::GetColorItemGrade(InItemTableData->grade);
	FString strItemName = InItemTableData->name.ToString();

	FString strEnchantBonusName = itemEquipData->GetBonusStatTitleName();
	if (strEnchantBonusName.IsEmpty())
	{
		FString strText = FString::Format(TEXT("{0}\n+{1}"), { *strItemName, InItemLevel });
		_textBlockName->SetText(FText::FromString(strText));
		_textBlockName->SetColorAndOpacity(itemColor);
	}
	else
	{
		FString strText = FString::Format(TEXT("{0}\n+{1} {2}"), { *strItemName, InItemLevel, *strEnchantBonusName });
		_textBlockName->SetText(FText::FromString(strText));
		_textBlockName->SetColorAndOpacity(itemColor);
	}
	_imageFocus->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIRepairListEntry::SetIsSelected(bool InIsSelected)
{
	_imageFocus->SetVisibility(InIsSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

const FGsItem* UGsUIRepairListEntry::GetItemData()
{
	return _uiIcon->GetItemIconData();
}
