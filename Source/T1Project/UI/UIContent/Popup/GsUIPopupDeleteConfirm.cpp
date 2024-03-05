// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupDeleteConfirm.h"
#include "Components/TextBlock.h"
#include "Item/GsItemManager.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Message/MessageParam/GsInvenMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "UTIL/GsItemUtil.h"


void UGsUIPopupDeleteConfirm::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupDeleteConfirm::OnClickClose);
	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupDeleteConfirm::OnClickOk);
}

void UGsUIPopupDeleteConfirm::NativeDestruct()
{
	_amount = 0;
	_itemDbid = 0;

	Super::NativeDestruct();
}

void UGsUIPopupDeleteConfirm::SetDeleteItemInfo(const FGsItem* InItem, ItemAmount InAmount)
{
	if (false == FGsItem::IsValidItem(InItem) || 0 >= InItem->GetDBID())
	{
		OnClickClose();
		return;
	}

	FText findText;
	do 
	{
		if (InItem->IsCashItem())
		{
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Delete_Warning_Description"), findText);
			break;
		}

		if (InItem->GetEquipPresetValue() != INVALID_PRESET_VALUE && false == InItem->IsEquipped())
		{
			FText::FindText(TEXT("ItemUIText"), TEXT("PresetUi_Delete_Preset_Description"), findText);
			break;
		}

		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Delete_Description"), findText);
	} while (0);

	_deleteWarnText->SetText(findText);

	FText convertText = InItem->GetName();
	FText::FindText(TEXT("MarkFormatingText"), TEXT("Multiply"), findText);
	FString resultStr;
	resultStr.Append(FString::Format(TEXT("{0} {1} {2}"), {
		*convertText.ToString(),
		* findText.ToString() ,
		InAmount }));
	
	_deleteItemInfoText->SetText(FText::FromString(resultStr));

	_itemDbid = InItem->GetDBID();
	_amount = InAmount;
}

void UGsUIPopupDeleteConfirm::OnClickClose()
{
	Close();
}

void UGsUIPopupDeleteConfirm::OnClickOk()
{
	const FGsItem* findData = UGsItemUtil::FindItemUtil(_itemDbid);
	if (nullptr == findData)
	{
		OnClickClose();
		return;
	}
	if (0 >= findData->GetAmount())
	{
		OnClickClose();
		return;
	}

	if (findData->GetAmount() < _amount)
	{
		_amount = findData->GetAmount();
	}

	if (true == GItem()->TryDeleteItem(findData, _amount))
	{
		FGsItemDetailCloseMessageParam param;
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, &param);

		OnClickClose();
	}
	else
	{
		Close();
	}
}

void UGsUIPopupDeleteConfirm::OnInputOk()
{
	OnClickOk();
}

void UGsUIPopupDeleteConfirm::OnInputCancel()
{
	OnClickClose();
}